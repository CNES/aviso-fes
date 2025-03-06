from . import (
    AbstractTidalModelComplex64,
    AbstractTidalModelComplex128,
    Axis,
    TideType,
    mesh,
)
from ..type_hints import MatrixInt32, VectorInt64

class CartesianComplex128(AbstractTidalModelComplex128):

    def __init__(self,
                 lon: Axis,
                 lat: Axis,
                 tide_type: TideType = ...,
                 longitude_major: bool = ...) -> None:
        ...

    def __getstate__(self) -> bytes:
        ...

    def __setstate__(self, state: bytes) -> None:
        ...

    def lat(self) -> Axis:
        ...

    def lon(self) -> Axis:
        ...


class CartesianComplex64(AbstractTidalModelComplex64):

    def __init__(self,
                 lon: Axis,
                 lat: Axis,
                 tide_type: TideType = ...,
                 longitude_major: bool = ...,
                 bbox: tuple[float, float, float, float] | None = ...) -> None:
        ...

    def __getstate__(self) -> bytes:
        ...

    def __setstate__(self, state: bytes) -> None:
        ...

    def lat(self) -> Axis:
        ...

    def lon(self) -> Axis:
        ...


class LGP1Complex128(AbstractTidalModelComplex128):

    def __init__(self,
                 index: mesh.Index,
                 codes: MatrixInt32,
                 tide_type: TideType = ...,
                 max_distance: float = ...,
                 bbox: tuple[float, float, float, float] | None = ...) -> None:
        ...

    def __getstate__(self) -> bytes:
        ...

    def __setstate__(self, state: bytes) -> None:
        ...

    def index(self) -> mesh.Index:
        ...

    def selected_indices(self) -> VectorInt64:
        ...


class LGP1Complex64(AbstractTidalModelComplex64):

    def __init__(self,
                 index: mesh.Index,
                 codes: MatrixInt32,
                 tide_type: TideType = ...,
                 max_distance: float = ...,
                 bbox: tuple[float, float, float, float] | None = ...) -> None:
        ...

    def __getstate__(self) -> bytes:
        ...

    def __setstate__(self, state: bytes) -> None:
        ...

    def index(self) -> mesh.Index:
        ...

    def selected_indices(self) -> VectorInt64:
        ...


class LGP2Complex128(AbstractTidalModelComplex128):

    def __init__(self,
                 index: mesh.Index,
                 codes: MatrixInt32,
                 tide_type: TideType = ...,
                 max_distance: float = ...,
                 bbox: tuple[float, float, float, float] | None = ...) -> None:
        ...

    def __getstate__(self) -> bytes:
        ...

    def __setstate__(self, state: bytes) -> None:
        ...

    def index(self) -> mesh.Index:
        ...

    def selected_indices(self) -> VectorInt64:
        ...


class LGP2Complex64(AbstractTidalModelComplex64):

    def __init__(self,
                 index: mesh.Index,
                 codes: MatrixInt32,
                 tide_type: TideType = ...,
                 max_distance: float = ...,
                 bbox: tuple[float, float, float, float] | None = ...) -> None:
        ...

    def __getstate__(self) -> bytes:
        ...

    def __setstate__(self, state: bytes) -> None:
        ...

    def index(self) -> mesh.Index:
        ...

    def selected_indices(self) -> VectorInt64:
        ...
