from typing import ClassVar, Dict, Iterator, List, Optional, Tuple, overload
import datetime

from . import constituents, datemanip, mesh, tidal_model

__all__ = [
    "AbstractTidalModelComplex64",
    "AbstractTidalModelComplex128",
    "Accelerator",
    "AstronomicAngle",
    "Axis",
    "Constituent",
    "Formulae",
    "LongPeriodEquilibrium",
    "Quality",
    "Settings",
    "TideType",
    "Wave",
    "WaveTable",
    "constituents",
    "datemanip",
    "evaluate_tide",
    "mesh",
    "tidal_model",
]

from ..type_hints import (
    MatrixComplex128,
    MatrixFloat64,
    VectorComplex64,
    VectorComplex128,
    VectorDateTime64,
    VectorFloat64,
    VectorInt8,
    VectorUInt8,
    VectorUInt16,
)

k2MK3: Constituent
k2MK6: Constituent
k2MN2: Constituent
k2MN6: Constituent
k2MS2: Constituent
k2MS6: Constituent
k2N2: Constituent
k2Q1: Constituent
k2SM2: Constituent
k2SM6: Constituent
kA5: Constituent
kChi1: Constituent
kEps2: Constituent
kEta2: Constituent
kExtrapolated1: Quality
kExtrapolated2: Quality
kExtrapolated3: Quality
kInterpolated: Quality
kJ1: Constituent
kK1: Constituent
kK2: Constituent
kL2: Constituent
kLambda2: Constituent
kM0: Constituent
kM11: Constituent
kM12: Constituent
kM13: Constituent
kM2: Constituent
kM3: Constituent
kM4: Constituent
kM6: Constituent
kM8: Constituent
kMeeus: Formulae
kMf: Constituent
kMf1: Constituent
kMf2: Constituent
kMK3: Constituent
kMK4: Constituent
kMKS2: Constituent
kMm: Constituent
kMm1: Constituent
kMm2: Constituent
kMN4: Constituent
kMNS2: Constituent
kMO3: Constituent
kMP1: Constituent
kMS4: Constituent
kMSf: Constituent
kMSK6: Constituent
kMSN2: Constituent
kMSN6: Constituent
kMsqm: Constituent
kMtm: Constituent
kMu2: Constituent
kN2: Constituent
kN4: Constituent
kNu2: Constituent
kO1: Constituent
kOO1: Constituent
kP1: Constituent
kPhi1: Constituent
kPi1: Constituent
kPsi1: Constituent
kQ1: Constituent
kR2: Constituent
kR4: Constituent
kRadial: TideType
kRho1: Constituent
kS1: Constituent
kS2: Constituent
kS4: Constituent
kS6: Constituent
kSa: Constituent
kSa1: Constituent
kSchuremanOrder1: Formulae
kSchuremanOrder3: Formulae
kSigma1: Constituent
kSK4: Constituent
kSN4: Constituent
kSsa: Constituent
kSta: Constituent
kT2: Constituent
kTheta1: Constituent
kTide: TideType
kUndefined: Quality


class AbstractTidalModelComplex128:

    def __init__(self, *args, **kwargs) -> None:
        ...

    def accelerator(self,
                    formulae: Formulae = ...,
                    time_tolerance: float = ...) -> Accelerator:
        ...

    def add_constituent(self, name: str, wave: VectorComplex128) -> None:
        ...

    def clear(self) -> None:
        ...

    def identifiers(self) -> List[Constituent]:
        ...

    @overload
    def interpolate(
        self,
        lon: VectorFloat64,
        lat: VectorFloat64,
        num_threads: int = ...
    ) -> Tuple[Dict[Constituent, VectorComplex128], VectorInt8]:
        ...

    @overload
    def interpolate(self, lon: float, lat: float,
                    wave_table: WaveTable) -> Quality:
        ...

    def __bool__(self) -> bool:
        ...

    def __len__(self) -> int:
        ...

    @property
    def dynamic(self) -> List[Constituent]:
        ...

    @dynamic.setter
    def dynamic(self, value: List[Constituent]) -> None:
        ...

    @property
    def tide_type(self) -> TideType:
        ...


class AbstractTidalModelComplex64:

    def __init__(self, *args, **kwargs) -> None:
        ...

    def accelerator(self,
                    formulae: Formulae = ...,
                    time_tolerance: float = ...) -> Accelerator:
        ...

    def add_constituent(self, name: str, wave: VectorComplex64) -> None:
        ...

    def clear(self) -> None:
        ...

    def identifiers(self) -> List[Constituent]:
        ...

    @overload
    def interpolate(
        self,
        lon: VectorFloat64,
        lat: VectorFloat64,
        num_threads: int = ...
    ) -> Tuple[Dict[Constituent, VectorComplex128], VectorInt8]:
        ...

    @overload
    def interpolate(self, lon: float, lat: float,
                    wave_table: WaveTable) -> Quality:
        ...

    def __bool__(self) -> bool:
        ...

    def __len__(self) -> int:
        ...

    @property
    def dynamic(self) -> List[Constituent]:
        ...

    @dynamic.setter
    def dynamic(self, value: List[Constituent]) -> None:
        ...

    @property
    def tide_type(self) -> TideType:
        ...


class Accelerator:

    def __init__(self, *args, **kwargs) -> None:
        ...


class AstronomicAngle:

    def __init__(self, formulae: Formulae = ...) -> None:
        ...

    def update(self, date: datetime.datetime, leap_seconds: int) -> None:
        ...

    @property
    def h(self) -> float:
        ...

    @property
    def i(self) -> float:
        ...

    @property
    def n(self) -> float:
        ...

    @property
    def nu(self) -> float:
        ...

    @property
    def nuprim(self) -> float:
        ...

    @property
    def nusec(self) -> float:
        ...

    @property
    def p(self) -> float:
        ...

    @property
    def p1(self) -> float:
        ...

    @property
    def r(self) -> float:
        ...

    @property
    def s(self) -> float:
        ...

    @property
    def t(self) -> float:
        ...

    @property
    def x1ra(self) -> float:
        ...

    @property
    def xi(self) -> float:
        ...


class Axis:
    __hash__: ClassVar[None] = ...  # type: ignore[assignment]

    def __init__(self,
                 points: VectorFloat64,
                 epsilon: float = ...,
                 is_circular: bool = ...) -> None:
        ...

    def end(self) -> float:
        ...

    def find_index(self, value: float, bounded: bool = ...) -> int:
        ...

    def find_indices(self, values: float) -> Optional[Tuple[int, int]]:
        ...

    def max_value(self) -> float:
        ...

    def min_value(self) -> float:
        ...

    def __eq__(self, other: Axis) -> bool:  # type: ignore[override]
        ...

    def __getitem__(self, index: int) -> float:
        ...

    def __len__(self) -> int:
        ...

    @property
    def is_ascending(self) -> bool:
        ...

    @property
    def is_circular(self) -> bool:
        ...

    @property
    def start(self) -> float:
        ...

    @property
    def step(self) -> float:
        ...


class Constituent:
    __members__: ClassVar[dict] = ...  # read-only
    __entries: ClassVar[dict] = ...
    k2MK3: ClassVar[Constituent] = ...
    k2MK6: ClassVar[Constituent] = ...
    k2MN2: ClassVar[Constituent] = ...
    k2MN6: ClassVar[Constituent] = ...
    k2MS2: ClassVar[Constituent] = ...
    k2MS6: ClassVar[Constituent] = ...
    k2N2: ClassVar[Constituent] = ...
    k2Q1: ClassVar[Constituent] = ...
    k2SM2: ClassVar[Constituent] = ...
    k2SM6: ClassVar[Constituent] = ...
    kA5: ClassVar[Constituent] = ...
    kChi1: ClassVar[Constituent] = ...
    kEps2: ClassVar[Constituent] = ...
    kEta2: ClassVar[Constituent] = ...
    kJ1: ClassVar[Constituent] = ...
    kK1: ClassVar[Constituent] = ...
    kK2: ClassVar[Constituent] = ...
    kL2: ClassVar[Constituent] = ...
    kLambda2: ClassVar[Constituent] = ...
    kM0: ClassVar[Constituent] = ...
    kM11: ClassVar[Constituent] = ...
    kM12: ClassVar[Constituent] = ...
    kM13: ClassVar[Constituent] = ...
    kM2: ClassVar[Constituent] = ...
    kM3: ClassVar[Constituent] = ...
    kM4: ClassVar[Constituent] = ...
    kM6: ClassVar[Constituent] = ...
    kM8: ClassVar[Constituent] = ...
    kMK3: ClassVar[Constituent] = ...
    kMK4: ClassVar[Constituent] = ...
    kMKS2: ClassVar[Constituent] = ...
    kMN4: ClassVar[Constituent] = ...
    kMNS2: ClassVar[Constituent] = ...
    kMO3: ClassVar[Constituent] = ...
    kMP1: ClassVar[Constituent] = ...
    kMS4: ClassVar[Constituent] = ...
    kMSK6: ClassVar[Constituent] = ...
    kMSN2: ClassVar[Constituent] = ...
    kMSN6: ClassVar[Constituent] = ...
    kMSf: ClassVar[Constituent] = ...
    kMf: ClassVar[Constituent] = ...
    kMf1: ClassVar[Constituent] = ...
    kMf2: ClassVar[Constituent] = ...
    kMm: ClassVar[Constituent] = ...
    kMm1: ClassVar[Constituent] = ...
    kMm2: ClassVar[Constituent] = ...
    kMsqm: ClassVar[Constituent] = ...
    kMtm: ClassVar[Constituent] = ...
    kMu2: ClassVar[Constituent] = ...
    kN2: ClassVar[Constituent] = ...
    kN4: ClassVar[Constituent] = ...
    kNu2: ClassVar[Constituent] = ...
    kO1: ClassVar[Constituent] = ...
    kOO1: ClassVar[Constituent] = ...
    kP1: ClassVar[Constituent] = ...
    kPhi1: ClassVar[Constituent] = ...
    kPi1: ClassVar[Constituent] = ...
    kPsi1: ClassVar[Constituent] = ...
    kQ1: ClassVar[Constituent] = ...
    kR2: ClassVar[Constituent] = ...
    kR4: ClassVar[Constituent] = ...
    kRho1: ClassVar[Constituent] = ...
    kS1: ClassVar[Constituent] = ...
    kS2: ClassVar[Constituent] = ...
    kS4: ClassVar[Constituent] = ...
    kS6: ClassVar[Constituent] = ...
    kSK4: ClassVar[Constituent] = ...
    kSN4: ClassVar[Constituent] = ...
    kSa: ClassVar[Constituent] = ...
    kSa1: ClassVar[Constituent] = ...
    kSigma1: ClassVar[Constituent] = ...
    kSsa: ClassVar[Constituent] = ...
    kSta: ClassVar[Constituent] = ...
    kT2: ClassVar[Constituent] = ...
    kTheta1: ClassVar[Constituent] = ...

    def __init__(self, value: int) -> None:
        ...

    def __eq__(self, other: object) -> bool:
        ...

    def __getstate__(self) -> int:
        ...

    def __hash__(self) -> int:
        ...

    def __index__(self) -> int:
        ...

    def __int__(self) -> int:
        ...

    def __ne__(self, other: object) -> bool:
        ...

    def __setstate__(self, state: int) -> None:
        ...

    @property
    def name(self) -> str:
        ...

    @property
    def value(self) -> int:
        ...


class Formulae:
    __members__: ClassVar[dict] = ...  # read-only
    __entries: ClassVar[dict] = ...
    kMeeus: ClassVar[Formulae] = ...
    kSchuremanOrder1: ClassVar[Formulae] = ...
    kSchuremanOrder3: ClassVar[Formulae] = ...

    def __init__(self, value: int) -> None:
        ...

    def __eq__(self, other: object) -> bool:
        ...

    def __getstate__(self) -> int:
        ...

    def __hash__(self) -> int:
        ...

    def __index__(self) -> int:
        ...

    def __int__(self) -> int:
        ...

    def __ne__(self, other: object) -> bool:
        ...

    def __setstate__(self, state: int) -> None:
        ...

    @property
    def name(self) -> str:
        ...

    @property
    def value(self) -> int:
        ...


class LongPeriodEquilibrium:

    def __init__(self) -> None:
        ...

    def disable_dynamic_wave(self, wave_table: WaveTable) -> None:
        ...

    def lpe_minus_n_waves(self, angles: AstronomicAngle, lat: float) -> float:
        ...


class Quality:
    __members__: ClassVar[dict] = ...  # read-only
    __entries: ClassVar[dict] = ...
    kExtrapolated1: ClassVar[Quality] = ...
    kExtrapolated2: ClassVar[Quality] = ...
    kExtrapolated3: ClassVar[Quality] = ...
    kInterpolated: ClassVar[Quality] = ...
    kUndefined: ClassVar[Quality] = ...

    def __init__(self, value: int) -> None:
        ...

    def __eq__(self, other: object) -> bool:
        ...

    def __getstate__(self) -> int:
        ...

    def __hash__(self) -> int:
        ...

    def __index__(self) -> int:
        ...

    def __int__(self) -> int:
        ...

    def __ne__(self, other: object) -> bool:
        ...

    def __setstate__(self, state: int) -> None:
        ...

    @property
    def name(self) -> str:
        ...

    @property
    def value(self) -> int:
        ...


class Settings:

    def __init__(self,
                 astronomic_formulae: Formulae = ...,
                 time_tolerance: float = ...) -> None:
        ...

    @property
    def astronomic_formulae(self) -> Formulae:
        ...

    @property
    def time_tolerance(self) -> float:
        ...


class TideType:
    __members__: ClassVar[dict] = ...  # read-only
    __entries: ClassVar[dict] = ...
    kRadial: ClassVar[TideType] = ...
    kTide: ClassVar[TideType] = ...

    def __init__(self, value: int) -> None:
        ...

    def __eq__(self, other: object) -> bool:
        ...

    def __getstate__(self) -> int:
        ...

    def __hash__(self) -> int:
        ...

    def __index__(self) -> int:
        ...

    def __int__(self) -> int:
        ...

    def __ne__(self, other: object) -> bool:
        ...

    def __setstate__(self, state: int) -> None:
        ...

    @property
    def name(self) -> str:
        ...

    @property
    def value(self) -> int:
        ...


class Wave:

    class TidalType:
        __members__: ClassVar[dict] = ...  # read-only
        __entries: ClassVar[dict] = ...
        kLongPeriod: ClassVar[Wave.TidalType] = ...
        kShortPeriod: ClassVar[Wave.TidalType] = ...

        def __init__(self, value: int) -> None:
            ...

        def __eq__(self, other: object) -> bool:
            ...

        def __getstate__(self) -> int:
            ...

        def __hash__(self) -> int:
            ...

        def __index__(self) -> int:
            ...

        def __int__(self) -> int:
            ...

        def __ne__(self, other: object) -> bool:
            ...

        def __setstate__(self, state: int) -> None:
            ...

        @property
        def name(self) -> str:
            ...

        @property
        def value(self) -> int:
            ...

    admittance: bool
    dynamic: bool
    tide: complex

    def __init__(self, *args, **kwargs) -> None:
        ...

    def doodson_number(self) -> str:
        ...

    def name(self) -> str:
        ...

    def nodal_a(self, a: AstronomicAngle) -> None:
        ...

    def nodal_g(self, a: AstronomicAngle) -> None:
        ...

    def period(self) -> float:
        ...

    def v(self) -> float:
        ...

    def vu(self) -> float:
        ...

    @property
    def f(self) -> float:
        ...

    @property
    def freq(self) -> float:
        ...

    @property
    def ident(self) -> Constituent:
        ...

    @property
    def type(self) -> Wave.TidalType:
        ...

    @property
    def u(self) -> float:
        ...


class WaveTable:

    def __init__(self, waves: List[str] = ...) -> None:
        ...

    def admittance(self) -> None:
        ...

    def compute_nodal_corrections(self, angle: AstronomicAngle) -> None:
        ...

    def compute_nodal_modulations(
            self,
            dates: VectorDateTime64,
            leap_seconds: VectorUInt16,
            formulae: Formulae = ...) -> Tuple[MatrixFloat64, MatrixFloat64]:
        ...

    def find(self, ident: str) -> Wave:
        ...

    @staticmethod
    def harmonic_analysis(h: VectorFloat64, f: MatrixFloat64,
                          vu: MatrixFloat64) -> VectorComplex128:
        ...

    def keys(self) -> List[str]:
        ...

    @staticmethod
    def select_waves_for_analysis(duration: float,
                                  f: float = ...) -> List[str]:
        ...

    def tide_from_mapping(self,
                          date: datetime.datetime,
                          leap_seconds: int,
                          mapping: MatrixComplex128,
                          formulae: Formulae = ...,
                          num_threads: int = ...) -> MatrixFloat64:
        ...

    def tide_from_tide_series(self,
                              dates: VectorDateTime64,
                              leap_seconds: VectorUInt16,
                              wave: VectorComplex128,
                              formulae: Formulae = ...) -> VectorFloat64:
        ...

    def values(self) -> List[Wave]:
        ...

    def __getitem__(self, index: Constituent) -> Wave:
        ...

    def __iter__(self) -> Iterator:
        ...

    def __len__(self) -> int:
        ...


def evaluate_equilibrium_long_period(dates: VectorDateTime64,
                                     leap_seconds: VectorUInt16,
                                     latitudes: VectorFloat64,
                                     settings: Settings | None = ...,
                                     num_threads: int = ...) -> VectorFloat64:
    ...


@overload
def evaluate_tide(
    tidal_model: AbstractTidalModelComplex128,
    date: VectorDateTime64,
    leap_seconds: VectorUInt16,
    longitude: VectorFloat64,
    latitude: VectorFloat64,
    settings: Optional[Settings] = ...,
    num_threads: int = ...
) -> Tuple[VectorFloat64, VectorFloat64, VectorUInt8]:
    ...


@overload
def evaluate_tide(
    tidal_model: AbstractTidalModelComplex64,
    date: VectorDateTime64,
    leap_seconds: VectorUInt16,
    longitude: VectorFloat64,
    latitude: VectorFloat64,
    settings: Optional[Settings] = ...,
    num_threads: int = ...
) -> Tuple[VectorFloat64, VectorFloat64, VectorUInt8]:
    ...
