# Copyright (c) 2024 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
"""
Tidal Model Configuration Module
================================

This module provides functionality to create a tidal model from a YAML file that
describes the type of grid model (Cartesian, lgp1, lgp2) and the waves modelled
by the numerical model.
"""
from __future__ import annotations

from typing import Any, NamedTuple, Union
from collections.abc import Callable
import dataclasses
import enum
import os
import re
from re import Match

import netCDF4
import numpy
import yaml

from . import core
from .core import (
    AbstractTidalModelComplex64,
    AbstractTidalModelComplex128,
    Constituent,
)
from .typing import Matrix, Vector

#: Alias for a tidal type.
TidalModel = Union[AbstractTidalModelComplex64, AbstractTidalModelComplex128]

#: Pattern to search for an environment variable.
PATTERN: Callable[[str], Match | None] = re.compile(r'\${(\w+)}').search

#: Maximum number of nested environment variables.
MAX_INTERPOLATION_DEPTH = 10

#: Alias to LPG classes known to this software.
LGPModel = type[core.tidal_model.LGP1Complex64
                | core.tidal_model.LGP1Complex128
                | core.tidal_model.LGP2Complex64
                | core.tidal_model.LGP2Complex128]

#: Alias to Cartesian classes known to this software.
CartesianModel = type[core.tidal_model.CartesianComplex64
                      | core.tidal_model.CartesianComplex128]


class Loader(yaml.Loader):
    """Allows to load the objects defined for this software from YAML
    documents."""


class InterpolationError(Exception):
    """Exception thrown when the user tries to interpolate an environment
    variable that is not defined."""


class InterpolationDepthError(Exception):
    """Exception thrown when the interpolation of an environment variable
    defines itself."""


def _expand(rawval: str) -> str:
    """Interpolation of environment variables present in a character string.

    Args:
        rawval: String to parse

    Returns
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
                f"The shell variable {name!r} doesn't exist.")
    if '$' in result and interpolation:
        raise InterpolationDepthError(
            f'Value interpolation too deeply recursive: {rawval!r}.')
    return result


def _parse(contents: Any) -> Any:
    """Interpolation of the environment variables contained in the YAML
    document.

    Args:
        contents: The YAML document to parse.

    Returns:
        The YAML document with the environment variables interpolated.
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


def load_cartesian_model(
    path: str,
    lon_name: str,
    lat_name: str,
    amp_name: str,
    pha_name: str,
) -> tuple[Vector, Vector, Matrix]:
    """Load a cartesian tidal model.

    Args:
        path: Path to the tidal model to be loaded into memory
        lon_name: Name of the longitude variable
        lat_name: Name of the latitude variable
        amp_name: Name of the amplitude variable
        pha_name: Name of the phase variable

    Returns:
        A tuple containing the longitude, latitude and tidal model.
    """
    with netCDF4.Dataset(path) as ds:  # type: ignore
        lon: Vector = ds.variables[lon_name][:]
        lat: Vector = ds.variables[lat_name][:]
        amp: Matrix = numpy.ma.filled(ds.variables[amp_name][:], numpy.nan)
        pha: Matrix = numpy.ma.filled(ds.variables[pha_name][:], numpy.nan)
        if ds.variables[pha_name].units.lower() in [
                'degree', 'degrees', 'deg'
        ]:
            pha = numpy.radians(pha)

    wave: Matrix = amp * numpy.cos(pha) + 1j * amp * numpy.sin(pha)

    return lon, lat, wave


class TideType(enum.Enum):
    """Tide type."""
    TIDE = core.kTide
    RADIAL = core.kRadial


class LGPType(enum.Enum):
    """LGP type."""
    LGP1 = 'lgp1'
    LGP2 = 'lgp2'


@dataclasses.dataclass(frozen=True)
class Common:
    """Common configuration for all the objects."""
    #: The name of the latitude variable.
    latitude: str = 'lat'
    #: The name of the longitude variable.
    longitude: str = 'lon'
    #: Tidal type.
    tidal_type: str = 'tide'
    #: The list of the waves to be considered as part of the given altlas
    #: (evaluated dynamically from the model). The wave declared in this list
    #: will be considered as part of the model components and will be disabled
    #: from the admittance calculation and and in the long-period equilibrium
    #: wave calculation routine (``lpe_minus_n_waves``).
    dynamic: list[str] = dataclasses.field(default_factory=list)

    def __post_init__(self) -> None:
        if self.tidal_type not in tuple(item.name.lower()
                                        for item in TideType):
            raise ValueError(f'Unknown tidal type: {self.tidal_type!r}.')
        if not self.longitude:
            raise ValueError('longitude cannot be empty.')
        if not self.latitude:
            raise ValueError('latitude cannot be empty.')
        known_constituents = core.constituents.known()
        for item in self.dynamic:
            if item not in known_constituents:
                raise ValueError(f'Unknown wave: {item!r}.')

    @property
    def dynamic_constituents(self) -> list[Constituent]:
        """Return the list of dynamic constituents."""
        return list(map(core.constituents.parse, self.dynamic))


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
        super().__post_init__()
        try:
            list(map(core.constituents.parse, self.paths))
        except ValueError as err:
            raise err from None

        if not self.paths:
            raise ValueError('No NetCDF files specified.')
        if not self.amplitude:
            raise ValueError('amplitude cannot be empty.')
        if not self.phase:
            raise ValueError('phase cannot be empty.')

    def load(self) -> TidalModel:
        """Load the tidal model defined by the configuration."""

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
            shape: tuple[int, int]

            def __ne__(self, other: Any) -> bool:
                if not isinstance(other, GridProperties):
                    return NotImplemented
                return (self.dtype != other.dtype and self.shape != other.shape
                        and not numpy.allclose(self.lon, other.lon)
                        and not numpy.allclose(self.lat, other.lat))

        # The tidal model instance consists of the tidal model and the grid
        # properties.
        class TidalModelInstance(NamedTuple):
            instance: TidalModel
            properties: GridProperties

        tidal_model: TidalModelInstance | None = None

        # Loop over each constituent and its corresponding NetCDF file path.
        for constituent, path in self.paths.items():
            # Check that the NetCDF file exists.
            if not os.path.exists(path):
                raise FileNotFoundError(f'File not found: {path!r}.')

            lon: Vector
            lat: Vector
            wave: Matrix

            # Load the tidal model for this constituent.
            lon, lat, wave = load_cartesian_model(
                path,
                self.longitude,
                self.latitude,
                self.amplitude,
                self.phase,
            )

            if len(wave.shape) != 2:
                raise ValueError(f'defined constituent {constituent!r} has '
                                 f'invalid shape: {wave.shape!r}.')

            # Create the tidal model if it doesn't exist yet.
            if tidal_model is None:

                # Determine the type of tidal model to create based on the wave
                # data type.
                type_name: CartesianModel | None = None
                if wave.dtype == numpy.complex64:
                    type_name = core.tidal_model.CartesianComplex64
                elif wave.dtype == numpy.complex128:
                    type_name = core.tidal_model.CartesianComplex128

                if type_name is None:
                    raise ValueError(f'Unknown wave type: {wave.dtype!r}.')

                # Create the tidal model instance.
                instance: TidalModel = type_name(
                    core.Axis(lon, epsilon=self.epsilon, is_circular=True),
                    core.Axis(lat),
                    tide_type=TideType[self.tidal_type.upper()].value,
                    longitude_major=wave.shape[0] == len(lon),
                )

                # Memorize the properties of the grid.
                properties = GridProperties(dtype=wave.dtype,
                                            lon=lon,
                                            lat=lat,
                                            shape=wave.shape)

                tidal_model = TidalModelInstance(instance, properties)
            # Check that the properties of the loaded grid match the first one.
            elif tidal_model.properties != GridProperties(
                    wave.dtype, lat, lon, wave.shape):
                raise ValueError(f'Inconsistent tidal model: {path!r}.')

            # Add the constituent to the tidal model.
            tidal_model.instance.add_constituent(constituent, wave.ravel())

        # Check that a tidal model was loaded.
        if tidal_model is None:
            raise ValueError('No tidal model loaded.')

        # Set the wave to be considered as part of this model, but not defined
        # as a Cartesian grid.
        tidal_model.instance.dynamic = self.dynamic_constituents

        # Return the tidal model instance.
        return tidal_model.instance


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
        super().__post_init__()
        try:
            list(map(core.constituents.parse, self.constituents))
        except ValueError as err:
            raise err from None

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
        if self.type not in tuple(item.name.lower() for item in LGPType):
            raise ValueError(f'Unknown LGP type: {self.type!r}.')
        try:
            self.amplitude.format(constituent='M2')
        except KeyError as err:
            raise ValueError(
                f'Invalid amplitude pattern: {self.amplitude!r}.') from err
        try:
            self.phase.format(constituent='M2')
        except KeyError as err:
            raise ValueError(
                f'Invalid phase pattern: {self.phase!r}.') from err

    def _lgp_class(self, dtype: numpy.dtype) -> LGPModel:
        """Return the class of the LGP tidal model."""
        if self.type == LGPType.LGP1.name:
            if dtype == numpy.complex64:
                return core.tidal_model.LGP1Complex64
            if dtype == numpy.complex128:
                return core.tidal_model.LGP1Complex128
        if dtype == numpy.complex64:
            return core.tidal_model.LGP2Complex64
        if dtype == numpy.complex128:
            return core.tidal_model.LGP2Complex128
        raise ValueError(f'Unknown wave type: {dtype!r}.')

    def load(self) -> TidalModel:
        """Load the tidal model defined by the configuration."""
        with netCDF4.Dataset(self.path, 'r') as ds:  # type: ignore
            lon: Vector = ds.variables[self.longitude][:]
            lat: Vector = ds.variables[self.latitude][:]
            triangles: Matrix = ds.variables[self.triangle][:]
            codes: Matrix = ds.variables[self.codes][:]

            instance: TidalModel | None = None

            for item in (self.constituents or core.constituents.known()):
                amp_name: str = self.amplitude.format(constituent=item)
                if amp_name not in ds.variables:
                    raise ValueError(f'Variable not found: {amp_name!r}.')
                pha_name: str = self.phase.format(constituent=item)
                if pha_name not in ds.variables:
                    raise ValueError(f'Variable not found: {pha_name!r}.')

                amp: Vector = numpy.ma.filled(ds.variables[amp_name][:],
                                              numpy.nan)
                pha: Vector = numpy.ma.filled(ds.variables[pha_name][:],
                                              numpy.nan)

                if ds.variables[pha_name].units in ['degree', 'degrees']:
                    pha = numpy.radians(pha)

                wave: Vector = amp * numpy.cos(pha) + 1j * amp * numpy.sin(pha)
                del amp, pha

                if instance is None:
                    type_name: LGPModel = self._lgp_class(wave.dtype)

                    instance = type_name(
                        core.mesh.Index(lon, lat, triangles),
                        codes=codes,
                        tide_type=TideType[self.tidal_type.upper()].value,
                        max_distance=self.max_distance,
                    )
                instance.add_constituent(item, wave)
        assert instance is not None, 'No tidal model loaded.'

        # Set the wave to be considered as part of this model, but not defined
        # as a Cartesian grid.
        instance.dynamic = self.dynamic_constituents

        return instance


def parse(path: str | os.PathLike) -> dict[str, Any]:
    """Load a YAML file and interpolate, if necessary, the environment
    variables contained in the YAML document.

    Args:
        path: Path to the configuration file to be loaded into memory

    Returns:
        A dictionary defining the YAML document.
    """
    with open(path, encoding='utf-8') as stream:
        return _parse(yaml.load(stream, Loader=Loader))


def _load_model(settings: dict[str, Any], tidal_type: str) -> TidalModel:
    """Load a configuration file into memory.

    Args:
        section: Name of the section to load.
        settings: A dictionary defining the YAML document.

    Returns:
        The tidal model loaded from the configuration file.
    """

    def tidal_type_exists(config: dict[str, Any], section: str) -> None:
        if 'tidal_type' in config[section]:
            raise TypeError(f'{section}.__init__() got an '
                            "unexpected keyword argument 'tidal_type'")

    if 'cartesian' in settings:
        tidal_type_exists(settings, 'cartesian')
        settings['cartesian'].update(tidal_type=tidal_type)
        return Cartesian(**settings['cartesian']).load()
    if 'lgp' in settings:
        tidal_type_exists(settings, 'lgp')
        settings['lgp'].update(tidal_type=tidal_type)
        return LGP(**settings['lgp']).load()

    raise ValueError('No tidal model found. Expected either "cartesian" or '
                     '"lgp".')


def load(path: str | os.PathLike) -> dict[str, TidalModel]:
    """Load a configuration file into memory.

    Args:
        path: Path to the configuration file to be loaded into memory

    Returns:
        A dictionary defining the configuration of the processing to be
        performed. The dictionary contains the tidal models to be used.
        The key is the type of the tidal model (e.g. ``tide``, ``radial``) and
        the value is the tidal model.
    """
    models: dict[str, TidalModel] = {}
    user_settings: dict[str, Any] = parse(path)

    if user_settings is None or len(user_settings) == 0:
        raise ValueError(f'Configuration file {path!r} is empty.')

    key: str
    for key in user_settings:
        if key not in ['tide', 'radial']:
            raise ValueError(f'Configuration file {path!r} is invalid. '
                             f'Expected "tide" or "radial" section.')
        try:
            models[key] = _load_model(user_settings[key], tidal_type=key)
        except TypeError as err:
            if 'unexpected keyword argument' in str(err):
                msg = str(err)
                key = msg.split('unexpected keyword argument ')[1]
                key = key.replace("'", '')
                section: str = msg.split('.__init__', maxsplit=1)[0].lower()
                raise ValueError(f'Configuration file {path!r} is invalid. '
                                 f'Unknown keyword: {key!r} in section '
                                 f'{section!r}.') from err
            raise err from None
    return models
