"""Recompute the IERS-based constants in ``include/fes/numbers.hpp``.

The constants are obtained by evaluating the formulae documented in
``examples/ex_astronomic_constants.py`` with the IERS Conventions (2010) /
IAU 2009 / IAU 2012 astronomic constants returned by
``updated_astronomic_constants()``.

The script only updates the IERS branch of each ``kUseSchuremanConstants
? schureman : iers`` ternary; Schureman's published values are left
untouched.
"""

from __future__ import annotations

import math
import pathlib
import re


HERE = pathlib.Path(__file__).resolve().parent
HPP = HERE.parent / 'include' / 'fes' / 'numbers.hpp'

# Format used for every IERS value in numbers.hpp (six fractional digits).
FMT = '{:.6f}'


def dms_to_deg(d: float, m: float, s: float) -> float:
    """Convert degrees, minutes, seconds to decimal degrees."""
    return d + m / 60 + s / 3600


def iers_constants() -> dict[str, float]:
    """Astronomic constants from ``updated_astronomic_constants()``.

    Kept inline so this script does not import the example notebook
    (which executes top-level code on import).
    """
    SE = 332946.0487
    ME = 0.0123000371
    a_moon = 384399e3
    AU = 149597870700.0
    return {
        'i': math.radians(dms_to_deg(5, 8, 43.4)),
        'w': math.radians(dms_to_deg(23, 26, 21.406)),
        'e1': 0.016708634,
        'e': 0.0549006,
        's': (SE / ME) * (a_moon / AU) ** 3,
    }


def compute_values() -> dict[str, float]:
    """Compute the IERS-based constants from the formulae."""
    c = iers_constants()
    i, w, e, e1, s = c['i'], c['w'], c['e'], c['e1'], c['s']

    sin_w = math.sin(w)
    cos_w = math.cos(w)
    sin_i = math.sin(i)
    sin_half_w = math.sin(0.5 * w)
    cos_half_w = math.cos(0.5 * w)
    cos_half_i = math.cos(0.5 * i)

    bracket = 1 - 1.5 * sin_i**2

    k65 = (2 / 3 - sin_w**2) * bracket
    k66 = sin_w**2 * cos_half_i**4
    k67 = sin_w * cos_half_w**2 * cos_half_i**4
    k68 = math.sin(2 * w) * bracket
    k69 = sin_w * math.sin(0.5 * w) ** 2 * cos_half_i**4
    k70 = cos_half_w**4 * cos_half_i**4
    k71 = sin_w**2 * bracket
    k141 = sin_w - 1.25 * sin_w**3
    k144 = (1 - 10 * sin_half_w**2 + 15 * sin_half_w**4) * cos_half_w**2
    k146 = sin_w * cos_half_w**4
    k147 = (cos_half_w**2 - 2 / 3) * sin_w * cos_half_w**2
    k149 = cos_half_w**6 * cos_half_i**6

    k197_1 = 0.25 + 2.25 * cos_w**2 / cos_half_w**4
    k197_2 = 1.5 * cos_w / cos_half_w**2

    k216 = 0.5 + 0.75 * e**2
    k217 = (0.5 + 0.75 * e1**2) * s * math.sin(2 * w)
    k218 = 0.5 + 0.75 * e**2
    k219 = (0.5 + 0.75 * e1**2) * s * sin_w**2

    k224 = k217 / k216
    k226 = k216 * k68 + k217
    denom_226 = k226**2
    k227_1 = k216**2 / denom_226
    k227_2 = 2 * k216 * k217 / denom_226
    k227_3 = k217**2 / denom_226

    k232 = k219 / k218
    k234 = k218 * k71 + k219
    denom_234 = k234**2
    k235_1 = k218**2 / denom_234
    k235_2 = 2 * k218 * k219 / denom_234
    k235_3 = k219**2 / denom_234

    cos_i_cos_w = math.cos(i) * cos_w
    sin_i_sin_w = sin_i * sin_w
    tan1 = math.cos(0.5 * (w - i)) / math.cos(0.5 * (w + i))
    tan2 = math.sin(0.5 * (w - i)) / math.sin(0.5 * (w + i))

    return {
        'k65': k65,
        'k66': k66,
        'k67': k67,
        'k68': k68,
        'k69': k69,
        'k70': k70,
        'k71': k71,
        'k141': k141,
        'k144': k144,
        'k146': k146,
        'k147': k147,
        'k149': k149,
        'k197_1': k197_1,
        'k197_2': k197_2,
        'k216': k216,
        'k217': k217,
        'k218': k218,
        'k219': k219,
        'k224': k224,
        'k226': k226,
        'k227_1': k227_1,
        'k227_2': k227_2,
        'k227_3': k227_3,
        'k232': k232,
        'k234': k234,
        'k235_1': k235_1,
        'k235_2': k235_2,
        'k235_3': k235_3,
        'kCosICosW': cos_i_cos_w,
        'kSinISinW': sin_i_sin_w,
        'kTan1': tan1,
        'kTan2': tan2,
    }


# Pattern matches:
#   constexpr auto <name> = kUseSchuremanConstants ? <schureman> : <iers>;
LINE_RE = re.compile(
    r'(constexpr\s+auto\s+(\w+)\s*=\s*kUseSchuremanConstants\s*\?\s*'
    r'[-+0-9.eE]+\s*:\s*)([-+0-9.eE]+)(\s*;)'
)


def patch(text: str, values: dict[str, float]) -> tuple[str, list[str]]:
    """Replace the IERS values in the given text with the provided values."""
    missing: list[str] = []

    def repl(match: re.Match[str]) -> str:
        name = match.group(2)
        if name not in values:
            return match.group(0)
        return f'{match.group(1)}{FMT.format(values[name])}{match.group(4)}'

    new_text = LINE_RE.sub(repl, text)

    found = {m.group(2) for m in LINE_RE.finditer(text)}
    missing = [name for name in values if name not in found]
    return new_text, missing


def main() -> None:
    """Update the IERS-based constants in numbers.hpp."""
    values = compute_values()
    original = HPP.read_text()
    updated, missing = patch(original, values)

    if missing:
        raise SystemExit(
            'Computed values not found in numbers.hpp: '
            + ', '.join(sorted(missing))
        )

    if updated == original:
        print(f'{HPP}: no changes')
        return

    HPP.write_text(updated)
    print(f'{HPP}: updated')
    for name in sorted(values):
        print(f'  {name:<10s} = {FMT.format(values[name])}')


if __name__ == '__main__':
    main()
