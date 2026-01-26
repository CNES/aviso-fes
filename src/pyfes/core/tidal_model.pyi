import numpy
import numpy.typing
import typing
from typing import ClassVar

from . import Formulae, Axis, mesh
from ..type_hints import (
    VectorFloat64,
    MatrixInt32,
    VectorInt64,
    VectorInt8,
    VectorComplex128,
    VectorComplex64,
)

RADIAL: TideType
TIDE: TideType

class AbstractTidalModelComplex128:
    dynamic: list[str]
    def __init__(self, *args, **kwargs) -> None: ...
    def accelerator(
        self,
        formulae: Formulae = ...,
        time_tolerance: typing.SupportsFloat = ...,
    ) -> Accelerator: ...
    def add_constituent(
        self,
        name: str,
        wave: VectorComplex128,
    ) -> None: ...
    def clear(self) -> None: ...
    def identifiers(self) -> list[str]: ...
    def interpolate(
        self,
        lon: VectorFloat64,
        lat: VectorFloat64,
        num_threads: typing.SupportsInt = ...,
    ) -> tuple[
        dict[
            str,
            VectorComplex128,
        ],
        VectorInt8,
    ]: ...
    def __bool__(self) -> bool: ...
    def __len__(self) -> int: ...

class AbstractTidalModelComplex64:
    dynamic: list[str]
    def __init__(self, *args, **kwargs) -> None: ...
    def accelerator(
        self,
        formulae: Formulae = ...,
        time_tolerance: typing.SupportsFloat = ...,
    ) -> Accelerator: ...
    def add_constituent(
        self,
        name: str,
        wave: VectorComplex64,
    ) -> None: ...
    def clear(self) -> None: ...
    def identifiers(self) -> list[str]: ...
    def interpolate(
        self,
        lon: VectorFloat64,
        lat: VectorFloat64,
        num_threads: typing.SupportsInt = ...,
    ) -> tuple[
        dict[
            str,
            VectorComplex64,
        ],
        VectorInt8,
    ]: ...
    def __bool__(self) -> bool: ...
    def __len__(self) -> int: ...

class Accelerator:
    def __init__(self, *args, **kwargs) -> None: ...

class CartesianComplex128(AbstractTidalModelComplex128):
    def __init__(
        self,
        lon: Axis,
        lat: Axis,
        constituent_map,
        tide_type: TideType = ...,
        longitude_major: bool = ...,
    ) -> None: ...
    def lat(self) -> Axis: ...
    def lon(self) -> Axis: ...

class CartesianComplex64(AbstractTidalModelComplex64):
    def __init__(
        self,
        lon: Axis,
        lat: Axis,
        constituent_map,
        tide_type: TideType = ...,
        longitude_major: bool = ...,
    ) -> None: ...
    def lat(self) -> Axis: ...
    def lon(self) -> Axis: ...

class LGP1Complex128(AbstractTidalModelComplex128):
    def __init__(
        self,
        index: mesh.Index,
        codes: typing.Annotated[numpy.typing.ArrayLike, numpy.int32, '[m, 3]'],
        constituent_map,
        tide_type: TideType = ...,
        max_distance: typing.SupportsFloat = ...,
        bbox: tuple[
            typing.SupportsFloat,
            typing.SupportsFloat,
            typing.SupportsFloat,
            typing.SupportsFloat,
        ]
        | None = ...,
    ) -> None: ...
    def index(self) -> mesh.Index: ...
    def selected_indices(
        self,
    ) -> VectorInt64: ...

class LGP1Complex64(AbstractTidalModelComplex64):
    def __init__(
        self,
        index: mesh.Index,
        codes: MatrixInt32,
        constituent_map,
        tide_type: TideType = ...,
        max_distance: typing.SupportsFloat = ...,
        bbox: tuple[
            typing.SupportsFloat,
            typing.SupportsFloat,
            typing.SupportsFloat,
            typing.SupportsFloat,
        ]
        | None = ...,
    ) -> None: ...
    def index(self) -> mesh.Index: ...
    def selected_indices(
        self,
    ) -> VectorInt64: ...

class LGP2Complex128(AbstractTidalModelComplex128):
    def __init__(
        self,
        index: mesh.Index,
        codes: MatrixInt32,
        constituent_map,
        tide_type: TideType = ...,
        max_distance: typing.SupportsFloat = ...,
        bbox: tuple[
            typing.SupportsFloat,
            typing.SupportsFloat,
            typing.SupportsFloat,
            typing.SupportsFloat,
        ]
        | None = ...,
    ) -> None: ...
    def index(self) -> mesh.Index: ...
    def selected_indices(
        self,
    ) -> typing.Annotated[numpy.typing.NDArray[numpy.int64], '[m, 1]']: ...

class LGP2Complex64(AbstractTidalModelComplex64):
    def __init__(
        self,
        index: mesh.Index,
        codes: MatrixInt32,
        constituent_map,
        tide_type: TideType = ...,
        max_distance: typing.SupportsFloat = ...,
        bbox: tuple[
            typing.SupportsFloat,
            typing.SupportsFloat,
            typing.SupportsFloat,
            typing.SupportsFloat,
        ]
        | None = ...,
    ) -> None: ...
    def index(self) -> mesh.Index: ...
    def selected_indices(
        self,
    ) -> VectorInt64: ...

class TideType:
    __members__: ClassVar[dict] = ...  # read-only
    __entries: ClassVar[dict] = ...
    RADIAL: ClassVar[TideType] = ...
    TIDE: ClassVar[TideType] = ...
    def __init__(self, value: typing.SupportsInt) -> None: ...
    def __eq__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    @property
    def name(self) -> str: ...
    @property
    def value(self) -> int: ...
