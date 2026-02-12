# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""Tidal Model Configuration Module.

This module provides functionality to create a tidal model from a YAML file that
describes the type of grid model (Cartesian, lgp1, lgp2) and the waves modelled
by the numerical model.
"""

from __future__ import annotations

from typing import TYPE_CHECKING, Any, Literal, NamedTuple, Union, cast
import dataclasses
import enum
import os
import re
from re import Match
import warnings

import netCDF4
import numpy
import yaml

from .core import (
    RADIAL as _RADIAL,
    TIDE as _TIDE,
    Axis,
    FESSettings,
    PerthSettings,
    Settings,
    TidalModelInterfaceComplex64,
    TidalModelInterfaceComplex128,
    mesh,
    parse_constituent,
    tidal_model,
)


if TYPE_CHECKING:
    from collections.abc import Callable, Iterable

    from .type_hints import Matrix, Vector

#: Alias for a tidal type.
TidalModel = Union[TidalModelInterfaceComplex64, TidalModelInterfaceComplex128]

#: Pattern to search for an environment variable.
PATTERN: Callable[[str], Match | None] = re.compile(r'\${(\w+)}').search

#: Maximum number of nested environment variables.
MAX_INTERPOLATION_DEPTH = 10

#: Number of dimensions expected in the wave data.
WAVE_DIMENSIONS = 2

#: Alias to LPG classes known to this software.
LGPModel = type[
    tidal_model.LGP1Complex64
    | tidal_model.LGP1Complex128
    | tidal_model.LGP2Complex64
    | tidal_model.LGP2Complex128
]

#: Alias to Cartesian classes known to this software.
CartesianModel = type[
    tidal_model.CartesianComplex64 | tidal_model.CartesianComplex128
]


class Loader(yaml.Loader):
    """Load objects from YAML documents."""


class InterpolationError(Exception):
    """Indicate that an undefined environment variable was referenced.

    Raised when interpolation of an environment variable fails because the
    variable is not set in the environment.
    """


class InterpolationDepthError(Exception):
    """Signal a self-referential environment variable interpolation.

    Raised when an environment variable used during interpolation expands to a
    value that references itself, causing excessive recursion.
    """


def _expand(rawval: str) -> str:
    """Interpolation of environment variables present in a character string.

    Args:
        rawval: String to parse

    Returns:
        The interpolated character string.

    Raises:
        InterpolationError: If the environment variable found is not defined.
        InterpolationDepthError :If the environment variable found defines
            itself.

    """
    result: str = rawval
    if '$' not in result:
        return result
    interpolation = False
    depth = MAX_INTERPOLATION_DEPTH
    while depth:
        depth -= 1
        match: Match[str] | None = PATTERN(result)
        if not match:
            break

        istart: int
        iend: int
        istart, iend = match.span(0)
        name: str = match.group(1)
        if name in os.environ:
            interpolation = True
            tail: str = result[iend:]
            result = result[:istart] + os.environ[name]
            result += tail
        else:
            raise InterpolationError(
                f"The shell variable {name!r} doesn't exist."
            )
    if '$' in result and interpolation:
        raise InterpolationDepthError(
            f'Value interpolation too deeply recursive: {rawval!r}.'
        )
    return result


def _parse(contents: Any) -> Any:  # noqa: ANN401
    """Interpolate environment variables in a YAML document.

    Recursively traverse mappings, sequences, and strings in `contents` and
    expand any environment-variable references found in strings.

    Args:
        contents: A YAML document represented as nested dicts, lists, and
            strings.

    Returns:
        The input structure with environment variables expanded in all
        strings. The operation may modify `contents` in place and returns
        the (possibly mutated) structure.

    """
    if isinstance(contents, dict):
        for key in contents:
            contents[key] = _parse(contents[key])
    elif isinstance(contents, list):
        for idx, item in enumerate(contents):
            contents[idx] = _parse(item)
    elif isinstance(contents, str):
        contents = _expand(contents)
    return contents


@dataclasses.dataclass(frozen=True)
class BoundingBox:
    """Represent a bounding box in 2D space."""

    x0: int
    x1: int
    y0: int
    y1: int


def extract_data_with_bounding_box(
    nx: int,
    ny: int,
    bbox: BoundingBox,
    ncvar: netCDF4.Variable,
) -> Matrix:
    """Extract the data from a NetCDF variable with a bounding box."""
    flip = ncvar.shape == (nx, ny)
    # If the provided bounding box traverses the date line, we need to
    # split the bounding box into two parts.
    if bbox.x0 > bbox.x1:
        slices1 = [slice(bbox.y0, bbox.y1 + 1), slice(bbox.x0, nx)]
        slices2 = [slice(bbox.y0, bbox.y1 + 1), slice(0, bbox.x1 + 1)]
        if flip:
            slices1 = slices1[::-1]
            slices2 = slices2[::-1]
        array1 = numpy.ma.filled(ncvar[slices1], numpy.nan)
        array2 = numpy.ma.filled(ncvar[slices2], numpy.nan)
        return numpy.concatenate((array1, array2), axis=ncvar.shape.index(nx))
    slices = [slice(bbox.y0, bbox.y1 + 1), slice(bbox.x0, bbox.x1 + 1)]
    if flip:
        slices = slices[::-1]
    return numpy.ma.filled(ncvar[slices], numpy.nan)


@dataclasses.dataclass(frozen=True)
class VariableNames:
    """Variable names used to load a cartesian tidal model."""

    lon: str = 'longitude'
    lat: str = 'latitude'
    amp: str = 'amplitude'
    pha: str = 'phase'


def load_cartesian_model(
    path: str,
    var_names: VariableNames,
    epsilon: float,
    bbox: tuple[float, float, float, float] | None,
) -> tuple[Vector, Vector, Matrix, bool]:
    """Load a cartesian tidal model.

    Args:
        path: Path to the tidal model to be loaded into memory
        var_names: Variable names to use for loading the model
        epsilon: The tolerance used to determine if the longitude axis is
            circular.
        bbox: Bounding box to consider when loading the tidal model. It is
            represented as a tuple of four floats: (min_lon, min_lat, max_lon,
            max_lat). If None, the whole grid is loaded.

    Returns:
        A tuple containing the longitude, latitude, tidal model and a flag
        indicating if grid is longitude-major.

    """
    with netCDF4.Dataset(path) as ds:
        lon: Vector = ds.variables[var_names.lon][:]
        lat: Vector = ds.variables[var_names.lat][:]
        longitude_major = ds.variables[var_names.amp].shape[0] == lon.size

        nc_amp = ds.variables[var_names.amp]
        nc_pha = ds.variables[var_names.pha]
        amp: Matrix
        pha: Matrix

        if bbox is not None:
            x_axis = Axis(lon, epsilon=epsilon, is_periodic=True)
            y_axis = Axis(lat)
            x0 = x_axis.find_index(bbox[0], bounded=True)
            x1 = x_axis.find_index(bbox[2], bounded=True)
            y0 = y_axis.find_index(bbox[1], bounded=True)
            y1 = y_axis.find_index(bbox[3], bounded=True)

            amp = extract_data_with_bounding_box(
                len(x_axis),
                len(y_axis),
                BoundingBox(x0, x1, y0, y1),
                nc_amp,
            )
            pha = extract_data_with_bounding_box(
                len(x_axis),
                len(y_axis),
                BoundingBox(x0, x1, y0, y1),
                nc_pha,
            )
            lat = lat[y0 : y1 + 1]
            lon = (
                lon[x0 : x1 + 1]
                if x0 <= x1
                else numpy.concatenate((lon[x0:], lon[: x1 + 1]))
            )
        else:
            amp = numpy.ma.filled(nc_amp[:], numpy.nan)
            pha = numpy.ma.filled(nc_pha[:], numpy.nan)

        if nc_pha.units.lower() in ['degree', 'degrees', 'deg']:
            pha = numpy.radians(pha)

    wave: Matrix = amp * numpy.exp(1j * pha)

    return lon, lat, wave, longitude_major


class TideType(enum.Enum):
    """Tide type."""

    TIDE = _TIDE
    RADIAL = _RADIAL


class LGPType(enum.Enum):
    """LGP type."""

    LGP1 = 'lgp1'
    LGP2 = 'lgp2'


class Engine(enum.Enum):
    """Tidal prediction engine type.

    The library supports two tidal prediction engines:

    - **DARWIN**: Based on Schureman/Darwin formulation. Uses nodal corrections
      and admittance calculations following the classical approach. Used by FES
      models.
    - **PERTH**: Based on Doodson numbers. Supports additional features like
      group modulations and different inference interpolation types. Used by
      GOT models.
    """

    DARWIN = 'darwin'
    PERTH = 'perth'


@dataclasses.dataclass(frozen=True)
class Common:
    """Common configuration for all the objects."""

    #: The name of the latitude variable.
    latitude: str = 'lat'
    #: The name of the longitude variable.
    longitude: str = 'lon'
    #: Tidal type.
    tidal_type: str = 'tide'
    #: The list of the waves to be considered as part of the given atlas
    #: (evaluated dynamically from the model). The wave declared in this list
    #: will be considered as part of the model components and will be disabled
    #: from the admittance calculation and in the long-period equilibrium
    #: wave calculation routine (``lpe_minus_n_waves``).
    dynamic: list[str] = dataclasses.field(default_factory=list)
    #: Bounding box to consider when loading the tidal model. It is represented
    #: as a tuple of four floats: (min_lon, min_lat, max_lon, max_lat). Default
    #: is None, which means the whole grid is loaded.
    bbox: tuple[float, float, float, float] | None = None
    #: The tidal prediction engine to use. Either 'darwin' for the
    #: Darwin/FES engine or 'perth' for the Perth/GOT engine.
    engine: Engine = Engine.DARWIN

    def __post_init__(self) -> None:
        """Validate the configuration."""
        if self.tidal_type not in tuple(item.name.lower() for item in TideType):
            raise ValueError(f'Unknown tidal type: {self.tidal_type!r}.')
        if not self.longitude:
            raise ValueError('longitude cannot be empty.')
        if not self.latitude:
            raise ValueError('latitude cannot be empty.')
        for item in self.dynamic:
            # Raise ConstituentValidationError if the constituent name is not
            # known.
            parse_constituent(item)

    @staticmethod
    def validate_constituents(names: Iterable[str]) -> None:
        """Validate that the given constituents are known.

        Args:
            names: List of constituent names to validate.

        Raises:
            ValueError: If one of the constituents is unknown.

        """
        unknown: list[str] = []
        for item in names:
            try:
                parse_constituent(item)
            except ValueError:
                unknown.append(item)
        if unknown:
            raise ValueError(f'Unknown constituents: {", ".join(unknown)!r}.')


# Define a named tuple to hold the properties of the cartesian grid.
class GridProperties(NamedTuple):
    """Properties of the cartesian grid."""

    #: The data type handled by the tidal model.
    dtype: numpy.dtype
    #: Latitude axis.
    lat: Vector
    #: Longitude axis.
    lon: Vector
    #: Shape of the tidal model.
    shape: tuple[int, ...]
    #: Flag to indicate if the grid is longitude-major.
    longitude_major: bool

    def __ne__(self, other: object) -> bool:
        """Check if two GridProperties are different."""
        if not isinstance(other, GridProperties):
            return NotImplemented
        return (
            self.shape != other.shape
            or self.dtype != other.dtype
            or self.longitude_major != other.longitude_major
            or not numpy.allclose(self.lon, other.lon)
            or not numpy.allclose(self.lat, other.lat)
        )

    def __repr__(self) -> str:
        """Return a string representation of the GridProperties."""
        return (
            f'GridProperties(dtype={self.dtype}, '
            f'lon=[{self.lon[0]}, ..., {self.lon[-1]}], '
            f'lat=[{self.lat[0]}, ..., {self.lat[-1]}], '
            f'shape={self.shape}, '
            f'longitude_major={self.longitude_major})'
        )


class TidalModelInstance(NamedTuple):
    """Tidal model instance.

    This class holds the tidal model and its grid properties.
    """

    instance: TidalModel
    properties: GridProperties

    def resample(
        self,
        lon: Vector,
        lat: Vector,
        epsilon: float,
        wave: Matrix,
        longitude_major: bool,
    ) -> Vector:
        """Resample the tidal model to a new grid."""
        if not isinstance(
            self.instance,
            (tidal_model.CartesianComplex64, tidal_model.CartesianComplex128),
        ):
            raise TypeError(
                'Resampling is only supported for Cartesian tidal models.'
            )
        return self.instance.resample(
            Axis(lon, epsilon=epsilon, is_periodic=True),
            Axis(lat),
            wave.ravel(),
            longitude_major,
        )


@dataclasses.dataclass(frozen=True)
class Cartesian(Common):
    """Configuration for the Cartesian model."""

    #: The name of the amplitude variable.
    amplitude: str = 'amplitude'
    #: The list of the NetCDF files to use.
    paths: dict[str, str] = dataclasses.field(default_factory=dict)
    #: The name of the phase variable.
    phase: str = 'phase'
    #: The tolerance used to determine if the longitude axis is circular.
    epsilon: float = 1e-6

    def __post_init__(self) -> None:
        """Validate the configuration."""
        super().__post_init__()
        if not self.paths:
            raise ValueError('No NetCDF files specified.')
        if not self.amplitude:
            raise ValueError('amplitude cannot be empty.')
        if not self.phase:
            raise ValueError('phase cannot be empty.')
        self.validate_constituents(self.paths.keys())

    def load(self) -> TidalModel:
        """Load the tidal model defined by the configuration."""
        model: TidalModelInstance | None = None

        # Loop over each constituent and its corresponding NetCDF file path.
        for constituent, path in self.paths.items():
            # Check that the NetCDF file exists.
            if not os.path.exists(path):
                raise FileNotFoundError(f'File not found: {path!r}.')

            lon: Vector
            lat: Vector
            wave: Matrix

            # Load the tidal model for this constituent.
            lon, lat, wave, longitude_major = load_cartesian_model(
                path,
                VariableNames(
                    self.longitude,
                    self.latitude,
                    self.amplitude,
                    self.phase,
                ),
                self.epsilon,
                self.bbox,
            )

            if wave.ndim != WAVE_DIMENSIONS:
                raise ValueError(
                    f'defined constituent {constituent!r} has '
                    f'invalid shape: {wave.shape!r}.'
                )

            # Create the tidal model if it doesn't exist yet.
            if model is None:
                # Determine the type of tidal model to create based on the wave
                # data type.
                type_name: CartesianModel | None = None
                if wave.dtype == numpy.complex64:
                    type_name = tidal_model.CartesianComplex64
                elif wave.dtype == numpy.complex128:
                    type_name = tidal_model.CartesianComplex128

                if type_name is None:
                    raise ValueError(f'Unknown wave type: {wave.dtype!r}.')

                # Create the tidal model instance.
                instance: TidalModel = type_name(
                    Axis(lon, epsilon=self.epsilon, is_periodic=True),
                    Axis(lat),
                    tide_type=TideType[self.tidal_type.upper()].value,
                    longitude_major=longitude_major,
                )

                # Memorize the properties of the grid.
                properties = GridProperties(
                    dtype=wave.dtype,
                    lon=lon,
                    lat=lat,
                    shape=wave.shape,
                    longitude_major=longitude_major,
                )

                model = TidalModelInstance(instance, properties)

            properties = GridProperties(
                dtype=wave.dtype,
                lon=lon,
                lat=lat,
                shape=wave.shape,
                longitude_major=longitude_major,
            )
            resample = model.properties != properties
            if resample:
                warnings.warn(
                    f'Constituent {constituent!r} has different grid '
                    'properties than previously loaded constituents. The grid '
                    'shall be resampled to match same resolution as other '
                    'constituents. '
                    f'Current: {properties!r} vs '
                    f'Previous: {model.properties!r}',
                    UserWarning,
                    stacklevel=3,
                )

            # Add the constituent to the tidal model.
            model.instance.add_constituent(
                constituent,
                model.resample(
                    lon,
                    lat,
                    self.epsilon,
                    wave,
                    longitude_major,
                )
                if resample
                else wave.ravel(),
            )

        # Check that a tidal model was loaded.
        if model is None:
            raise ValueError('No tidal model loaded.')

        # Set the wave to be considered as part of this model, but not defined
        # as a Cartesian grid.
        model.instance.dynamic = self.dynamic

        # Return the tidal model instance.
        return model.instance


@dataclasses.dataclass(frozen=True)
class LGP(Common):
    """Configuration for the LGP model."""

    #: The pattern of the variable containing the amplitudes.
    amplitude: str = '{constituent}_amplitude'
    #: List of the tidal constituents to use.
    constituents: list[str] = dataclasses.field(default_factory=list)
    #: The name of the variable containing the LGP codes.
    codes: str = 'codes'
    #: Max distance allowed to extrapolate.
    max_distance: float = 0.0
    #: The path to the NetCDF file to use.
    path: str = ''
    #: The pattern of the variable containing the phases.
    phase: str = '{constituent}_phase'
    #: The name of the variable containing the vertices of the triangles.
    triangle: str = 'triangle'
    #: Type of LGP discretization to use. Allowed values are ``lgp1`` and
    #: ``lgp2``.
    type: str = 'lgp1'

    def __post_init__(self) -> None:
        """Validate the configuration."""
        super().__post_init__()
        if not self.path:
            raise ValueError('No NetCDF file specified.')
        if not self.triangle:
            raise ValueError('triangles cannot be empty.')
        if not self.codes:
            raise ValueError('codes cannot be empty.')
        if not self.amplitude:
            raise ValueError('amplitude cannot be empty.')
        if not self.phase:
            raise ValueError('phase cannot be empty.')
        self.validate_constituents(self.constituents)
        if self.type not in tuple(item.name.lower() for item in LGPType):
            raise ValueError(f'Unknown LGP type: {self.type!r}.')
        try:
            self.amplitude.format(constituent='M2')
        except KeyError as err:
            raise ValueError(
                f'Invalid amplitude pattern: {self.amplitude!r}.'
            ) from err
        try:
            self.phase.format(constituent='M2')
        except KeyError as err:
            raise ValueError(f'Invalid phase pattern: {self.phase!r}.') from err

    def _lgp_class(self, dtype: numpy.dtype) -> LGPModel:
        """Return the class of the LGP tidal model."""
        if self.type == LGPType.LGP1.name:
            if dtype == numpy.complex64:
                return tidal_model.LGP1Complex64
            if dtype == numpy.complex128:
                return tidal_model.LGP1Complex128
        if dtype == numpy.complex64:
            return tidal_model.LGP2Complex64
        if dtype == numpy.complex128:
            return tidal_model.LGP2Complex128
        raise ValueError(f'Unknown wave type: {dtype!r}.')

    def load(self) -> TidalModel:
        """Load the tidal model defined by the configuration."""
        with netCDF4.Dataset(self.path, 'r') as ds:
            lon: Vector = ds.variables[self.longitude][:]
            lat: Vector = ds.variables[self.latitude][:]
            triangles: Matrix = ds.variables[self.triangle][:]
            codes: Matrix = ds.variables[self.codes][:]

            instance: TidalModel | None = None
            selected_indices: Vector | None = None

            for item in self.constituents:
                try:
                    parse_constituent(item)
                except ValueError as exc:
                    raise ValueError(f'Unknown constituent: {item!r}.') from exc
                amp_name: str = self.amplitude.format(constituent=item)
                if amp_name not in ds.variables:
                    raise ValueError(f'Variable not found: {amp_name!r}.')
                pha_name: str = self.phase.format(constituent=item)
                if pha_name not in ds.variables:
                    raise ValueError(f'Variable not found: {pha_name!r}.')

                if instance is None:
                    type_name: LGPModel = self._lgp_class(
                        (ds.variables[amp_name].dtype.type(0) + 1j).dtype
                    )

                    instance = type_name(
                        mesh.Index(lon, lat, triangles),
                        codes=codes,
                        tide_type=TideType[self.tidal_type.upper()].value,
                        max_distance=self.max_distance,
                        bbox=self.bbox,
                    )
                    if self.bbox is not None:
                        selected_indices = instance.selected_indices()

                amp = numpy.ma.filled(ds.variables[amp_name][:], numpy.nan)
                pha = numpy.ma.filled(ds.variables[pha_name][:], numpy.nan)

                if selected_indices is not None:
                    amp = amp[selected_indices]
                    pha = pha[selected_indices]

                if ds.variables[pha_name].units in ['degree', 'degrees']:
                    pha = numpy.radians(pha)

                wave: Vector = amp * numpy.exp(1j * pha)
                del amp, pha

                instance.add_constituent(item, wave)
        assert instance is not None, 'No tidal model loaded.'

        # Set the wave to be considered as part of this model, but not defined
        # as a Cartesian grid.
        instance.dynamic = self.dynamic

        return instance


def parse(path: str | os.PathLike) -> dict[str, Any]:
    """Load a YAML configuration file.

    Open and parse the YAML file at the given path and interpolate any
    environment variables referenced in the document. The parsed YAML is
    returned as a dictionary.

    Args:
        path: Path to the configuration file to load. May be a str or an
            os.PathLike object.

    Returns:
        dict[str, Any]: The parsed YAML document.

    Raises:
        OSError: If the file cannot be opened.
        yaml.YAMLError: If the YAML cannot be parsed.

    """
    with open(path, encoding='utf-8') as stream:
        return _parse(yaml.load(stream, Loader=Loader))


def _load_model(
    settings: dict[str, Any],
    tidal_type: str,
    engine: Engine,
    bbox: tuple[float, float, float, float] | None = None,
) -> TidalModel:
    """Load a configuration file into memory.

    Args:
        settings: A dictionary defining the YAML document.
        tidal_type: The type of the tidal model to load.
        engine: The tidal prediction engine to use.
        bbox: Bounding box to consider when loading the tidal model. It is
            represented as a tuple of four floats: (min_lon, min_lat, max_lon,
            max_lat). If not provided, the whole grid is loaded.

    Returns:
        The tidal model loaded from the configuration file.

    """

    def tidal_type_exists(config: dict[str, Any], section: str) -> None:
        if 'tidal_type' in config[section]:
            raise TypeError(
                f'{section}.__init__() got an '
                'unexpected keyword argument "tidal_type"'
            )

    if 'cartesian' in settings:
        tidal_type_exists(settings, 'cartesian')
        settings['cartesian'].update(tidal_type=tidal_type, engine=engine)
        return Cartesian(bbox=bbox, **settings['cartesian']).load()
    if 'lgp' in settings:
        tidal_type_exists(settings, 'lgp')
        settings['lgp'].update(tidal_type=tidal_type, engine=engine)
        return LGP(bbox=bbox, **settings['lgp']).load()

    raise ValueError(
        'No tidal model found. Expected either "cartesian" or "lgp".'
    )


class Configuration(NamedTuple):
    """Configuration returned by the load function.

    This class holds the tidal models and the runtime settings to use
    for tidal predictions.
    """

    #: Dictionary mapping tidal type ('tide', 'radial') to tidal model.
    models: dict[Literal['tide', 'radial'], TidalModel]
    #: Runtime settings for the tidal prediction engine.
    settings: Settings

    def memory_usage(self) -> int:
        """Return the total memory usage of the configuration in bytes."""
        return sum(model.memory_usage() for model in self.models.values())


def create_settings(engine: Engine) -> Settings:
    """Create runtime settings for the specified engine.

    Args:
        engine: The tidal prediction engine to use.

    Returns:
        Runtime settings appropriate for the engine.

    """
    if engine == Engine.DARWIN:
        return FESSettings()
    return PerthSettings()


def load(
    path: str | os.PathLike,
    bbox: tuple[float, float, float, float] | None = None,
) -> Configuration:
    """Load a configuration file into memory.

    Args:
        path: Path to the configuration file to be loaded into memory
        bbox: Bounding box to consider when loading the tidal model. It is
            represented as a tuple of four floats: (min_lon, min_lat, max_lon,
            max_lat). If not provided, the whole grid is loaded.

    Returns:
        A Configuration object containing:
        - models: A dictionary mapping tidal type (e.g. ``tide``, ``radial``)
          to the corresponding tidal model.
        - settings: Runtime settings for the tidal prediction engine
          (FESSettings or PerthSettings).

    Raises:
        ConstituentValidationError: If an invalid constituent name is used in
            the configuration.
        InterpolationError: If an environment variable referenced in the
            configuration file is not defined in the environment.
        InterpolationDepthError: If interpolation of environment variables is
            too deeply recursive, indicating a self-referential variable.
        OSError: If the file cannot be opened.
        TypeError: If the configuration file contains unknown sections or
            invalid keys for the tidal model configuration.
        ValueError: If the configuration file is invalid (e.g. missing required
            sections, unknown tidal type, inconsistent grid properties, etc.).
        yaml.YAMLError: If the YAML cannot be parsed.

    Example:
        >>> import pyfes
        >>> config = pyfes.load('ocean_tide.yaml')
        >>> # Access the tide model
        >>> tide_model = config.models['tide']
        >>> # Use the settings for evaluation
        >>> result = pyfes.core.evaluate_tide(
        ...     tide_model, dates, lon, lat, config.settings
        ... )

    """
    models: dict[Literal['tide', 'radial'], TidalModel] = {}
    user_settings: dict[str, Any] = parse(path)

    if user_settings is None or len(user_settings) == 0:
        raise ValueError(f'Configuration file {path!r} is empty.')

    # Extract engine from top-level configuration (default to Darwin)
    engine_str: str = user_settings.pop('engine', 'darwin')
    try:
        engine = Engine(engine_str)
    except ValueError as err:
        raise ValueError(
            f'Configuration file {path!r} is invalid. '
            f'Unknown engine: {engine_str!r}.'
        ) from err

    for key, settings in user_settings.items():
        if key not in ['tide', 'radial']:
            raise ValueError(
                f'Configuration file {path!r} is invalid. '
                f'Expected "tide" or "radial" section.'
            )
        key = cast("Literal['tide', 'radial']", key)

        try:
            model = _load_model(
                settings, tidal_type=key, engine=engine, bbox=bbox
            )
            models[key] = model
        except TypeError as err:
            if 'unexpected keyword argument' in str(err):
                msg = str(err)
                unknown_key = msg.split('unexpected keyword argument ')[1]
                unknown_key = unknown_key.replace("'", '')
                section: str = msg.split('.__init__', maxsplit=1)[0].lower()
                raise ValueError(
                    f'Configuration file {path!r} is invalid. '
                    f'Unknown keyword: {unknown_key!r} in section '
                    f'{section!r}.'
                ) from err
            raise err from None

    return Configuration(models=models, settings=create_settings(engine))
