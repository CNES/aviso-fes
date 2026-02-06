import typing

from ..type_hints import MatrixInt32, VectorInt64
from . import (
    TidalModelInterfaceComplex128,
    TidalModelInterfaceComplex64,
    Axis,
    TideType,
)
from .mesh import Index

class CartesianComplex128(TidalModelInterfaceComplex128):
    def __init__(
        self,
        lon: Axis,
        lat: Axis,
        tide_type: TideType = ...,
        longitude_major: bool = ...,
    ) -> None: ...
    def lat(self) -> Axis: ...
    def lon(self) -> Axis: ...

class CartesianComplex64(TidalModelInterfaceComplex64):
    def __init__(
        self,
        lon: Axis,
        lat: Axis,
        tide_type: TideType = ...,
        longitude_major: bool = ...,
    ) -> None: ...
    def lat(self) -> Axis: ...
    def lon(self) -> Axis: ...

class LGP1Complex128(TidalModelInterfaceComplex128):
    def __init__(
        self,
        index: Index,
        codes: MatrixInt32,
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
    def index(self) -> Index: ...
    def selected_indices(self) -> VectorInt64: ...

class LGP1Complex64(TidalModelInterfaceComplex64):
    def __init__(
        self,
        index: Index,
        codes: MatrixInt32,
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
    def index(self) -> Index: ...
    def selected_indices(self) -> VectorInt64: ...

class LGP2Complex128(TidalModelInterfaceComplex128):
    def __init__(
        self,
        index: Index,
        codes: MatrixInt32,
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
    def index(self) -> Index: ...
    def selected_indices(self) -> VectorInt64: ...

class LGP2Complex64(TidalModelInterfaceComplex64):
    def __init__(
        self,
        index: Index,
        codes: MatrixInt32,
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
    def index(self) -> Index: ...
    def selected_indices(self) -> VectorInt64: ...
