"""
********************************
Astronomic Constants Calculation
********************************
"""
# %%
import dataclasses
import math


def dms_to_deg(degrees: float, minutes: float, seconds: float) -> float:
    """Converts degrees, minutes, and seconds to decimal degrees.

    Args:
        degrees (float): The degrees value.
        minutes (float): The minutes value.
        seconds (float): The seconds value.

    Returns:
        float: The decimal degrees value.
    """
    return degrees + minutes / 60 + seconds / 3600


# %%
@dataclasses.dataclass
class AstronomicConstants:
    """Astronomic constants used in the Manual Of Harmonic Analysis and
    Prediction of Tides.

    Special Publication No. 98 Revised (1940) Edition.
    """

    #: Inclination of moon's orbit to plane of ecliptic
    i: float

    #: Obliquity of ecliptic
    w: float

    #: Eccentricity of earth's orbit
    e1: float

    #: Eccentricity of moon's orbit
    e: float

    #: Solar factor
    s: float


# %%
def schureman_values() -> AstronomicConstants:
    """Returns the Schureman's values as written in the book."""
    return AstronomicConstants(i=math.radians(5.145),
                               w=math.radians(23.452),
                               e1=0.016_75,
                               e=0.054_90,
                               s=0.4602)


# %%
def schureman_recomputed_values() -> AstronomicConstants:
    """Returns the Schureman's values recomputed with the given values written
    in the book."""
    # Distance of the center of the earth to the center of the moon
    # (in miles)
    # R = 239_000

    # Mass (of sun/mass) of earthy (S/E)
    SE = 331_954

    # Mass of moon/mass of earth (M/E)
    ME = 0.012_27

    # Solar parallex
    c1: float = math.radians(dms_to_deg(0, 0, 8.80))

    # Lunar equatorial parallex
    c: float = math.radians(dms_to_deg(0, 57, 2.70))

    # Mean earth radius in miles
    mean_earth_radius = 3_958.9

    # Equatorial radius of earth in miles
    earth_equatorial_radius = 3_963.37

    # Mean solar parallax in respect to mean radius
    ac1 = (mean_earth_radius / earth_equatorial_radius) * c1

    # Mean lunar parallax in respect to mean radius
    ac = (mean_earth_radius / earth_equatorial_radius) * c

    # Solar coefficient U1 (SE) * ac1^3
    u1: float = SE * ac1**3

    # Basic factor U (ME) * ac^3
    u: float = ME * ac**3

    return AstronomicConstants(
        i=math.radians(dms_to_deg(5, 8, 43.3546)),
        w=math.radians(dms_to_deg(23, 27, 8.26)),
        e1=0.016_75,
        e=0.054_90,
        s=u1 / u,
    )


# %%
def astronomic_constants() -> AstronomicConstants:
    """Astronomic constants."""
    # Mass (of sun/mass) of earthy (S/E)
    SE = 332946.0487

    # Mass of moon/mass of earth (M/E)
    ME = 0.0123000371

    # Solar parallex
    c1 = dms_to_deg(0, 0, 8.794_143)

    # Lunar equatorial parallex
    c = dms_to_deg(0, 57, 2.70)
    mean_earth_radius = 6_371_008.771415059
    earth_equatorial_radius = 6_378_137.0

    # Mean solar parallax in respect to mean radius
    ac1 = (mean_earth_radius / earth_equatorial_radius) * c1

    # Mean lunar parallax in respect to mean radius
    ac = (mean_earth_radius / earth_equatorial_radius) * c

    # Solar coefficient U1 (SE) * ac1^3
    u1: float = SE * ac1**3

    # Basic factor U (ME) * ac^3
    u: float = ME * ac**3

    return AstronomicConstants(
        i=math.radians(dms_to_deg(5, 8, 43.3546)),
        w=math.radians(dms_to_deg(23, 26, 21.45)),
        e1=0.016_71,
        e=0.054_9006,
        s=u1 / u,
    )


# %%
# Schureman's values
# ------------------
const = schureman_values()
print(const)

# %%
# Formulae 73, 65
# ---------------
# `Page 24, 25`
#
# .. math::
#
#       \{2/3 - sin^2(\omega)\}\{1 - 3/2 \times sin^2(i)\} = 0.5021
#
# .. math::
#
#       f(Mm) = \{2/3 - sin^2(I)\} / 0.5021
a65 = (2 / 3 - math.sin(const.w)**2) * (1 - 3 / 2 * math.sin(const.i)**2)
print(f'{a65:.4f}')

# %%
# Formulae 74, 66
# ---------------
# `Page 24, 25`
#
# .. math::
#
#       sin^2(\omega) \times \cos^4(\frac{1}{2}i) = 0.1578
#
# .. math::
#
#       f(Mf) = sin^2(I) / 0.1578
a66 = math.sin(const.w)**2 * math.cos(0.5 * const.i)**4
print(f'a66 = {a66:.4f}')

# %%
# Formulae 75, 67
# ---------------
# `Page 25`
#
# .. math::
#
#   sin(\omega)cos^2(\frac{1}{2}\omega)cos^4(\frac{1}{2}i) = 0.3800
#
# .. math::
#
#   f(O_1) = sin(I) \times cos^2(\frac{1}{2}I) / 0.3800
a67 = math.sin(const.w) * math.cos(0.5 * const.w)**2 * math.cos(
    0.5 * const.i)**4
print(f'a67 = {a67:.4f}')

# %%
# Formulae 76, 68
# ---------------
# `Page 25`
#
# .. math::
#
#   sin(2\omega) \times \{1 - 3/2 \times sin^2(i)\} = 0.7214
#
# .. math::
#
#   f(J_1) = sin(2I) / 0.7214
a68 = math.sin(2 * const.w) * (1 - 3 / 2 * math.sin(const.i)**2)
print(f'a68 = {a68:.4f}')

# %%
# Formulae 77, 69
# ---------------
# `Page 25`
#
# .. math::
#
#       sin(\omega) \times sin^2(\frac{1}{2}\omega)
#       \times cos^4(\frac{1}{2}i) = 0.0164
#
# .. math::
#
#   f(OO_1) = sin(I) \times sin^2(\frac{1}{2}I) / 0.0164
a69 = math.sin(const.w) * math.sin(0.5 * const.w)**2 * math.cos(
    0.5 * const.i)**4
print(f'a69 = {a69:.4f}')

# %%
# Formulae 78, 70
# ---------------
# `Page 25`
#
# .. math::
#
#       cos^4(\frac{1}{2}\omega) \times cos^4(\frac{1}{2}i) = 0.9154
#
# .. math::
#
#       f(M_2) = cos^4(\frac{1}{2}I) / 0.9154
a70 = math.cos(0.5 * const.w)**4 * math.cos(0.5 * const.i)**4
print(f'a70 = {a70:.4f}')

# %%
# Formulae 79, 71
# ---------------
# `Page 25`
#
# .. math::
#
#       sin^2(\omega) \times (1 -3/2 \times sin^2(i))
#
# .. math::
#
#       f(79) = sin^2I / 0.1565
a71 = math.sin(const.w)**2 * (1 - 3 / 2 * math.sin(const.i)**2)
print(f'a71 = {a71:.4f}')

# %%
# Formulae 141, 137
# -----------------
# `Page 35, 36`
#
# .. math::
#
#       sin(I) -5/4 \times sin^3(I) = 0.3192
#
# .. math::
#
#       f(141) = sin(I) - 5/4 \times sin^3(i) / 0.3192
a141 = math.sin(const.w) - 5 / 4 * math.sin(const.w)**3
print(f'a141 = {a141:.4f}')

# %%
# Formulae 149, 140
# -----------------
# `Page 35, 36`
#
# .. math::
#
#       cos^6(\frac{1}{2}I) + cos^4(\frac{1}{2}I) \times sin^2(\frac{1}{2}I)
#
# .. math::
#
#       f(M_3) = cos^6(\frac{1}{2}I) / 0.8758
#
# Formulae 207, 195
# -----------------
# `Page 41`
#
# .. math::
#
#       1/Q_a = \left[1/4 + 3/2\frac{cos I}{cos^2 \frac{1}{2}I}\times cos 2P +
#       9/4\frac{cos^2 I}{cos^4 \frac{1}{2} I}\right]^\frac{1}{2}
#
# .. math::
#
#   1/Q_a = \left(2.310 + 1.435 \times cos(2P)\right)^\frac{1}{2}
#
# .. math::
#
#   cos(2P) = cos(2 \times (p - \xi))
#
# .. math::
#
#   f(M_ 1) = f(O_ 1) \times 1 / Q_a
a195_1 = (1 /
          4) + (9 / 4) * (math.cos(const.w)**2 / math.cos(0.5 * const.w)**4)
a195_2 = (3 / 2) * (math.cos(const.w) / math.cos(0.5 * const.w)**2)
print(f'a195_1 = {a195_1:.3f}')
print(f'a195_2 = {a195_2:.3f}')

# %%
# Formulae 216, 218, 217, 219
# ---------------------------
# `Page 45`
#
# .. math::
#
#       1/2 + 3/4 \times e^2 = 0.5023
#
# .. math::
#
#       (1/2 + 3/4 \times e_1^2) \times S' \times sin(2\omega) = 0.1681
#
# .. math::
#
#       (1/2 + 3/4 \times e_1^2) \times S'  \times sin^2(\omega) = 0.0365
term_a22 = 1 / 2 + 3 / 4 * const.e**2
term_b22 = (1 / 2 + 3 / 4 * const.e1**2) * const.s * math.sin(2 * const.w)
term_a47 = 1 / 2 + 3 / 4 * const.e**2
term_b47 = (1 / 2 + 3 / 4 * const.e1**2) * const.s * math.sin(const.w)**2

print(f'term A22 = {term_a22:.4f}')
print(f'term B22 = {term_b22:.4f}')
print(f'term A47 = {term_a47:.4f}')
print(f'term B47 = {term_b47:.4f}')

# %%
# Formulae 226
# ------------
# `Page 45`
#
# .. math::
#
#       [(A^2 + 2AB \times cos(\upsilon) + B^2)^\frac{1}{2} \times
#       cos(\upsilon')]_0
#
# .. math::
#
#   \left[0.5023 \times sin(2I) cos(\upsilon) + 0.1681\right]_0 = 0.5305
a226 = (term_a22 * a68 + term_b22)
print(f'a226 = {a226:.4f}')

# %%
# Formulae 227
# ------------
# `Page 45`
#
# .. math::
#
#       f(K_1) = (0.2523 \times sin^2(2I) + 0.1689 \times sin(2I) \times
#       cos(\upsilon) + 0.0283)^\frac{1}{2}/0.5305
#
# .. math::
#
#       f(K_1) = (0.8965 \times sin^2(2I) + 0.6001 \times sin(2I) \times
#       cos(\upsilon) + 0.1006)^\frac{1}{2}
print(f'{(term_a22 ** 2) / a226 ** 2:.4f}')
print(f'{(term_a22 * term_b22 * 2) / a226 ** 2:.4f}')
print(f'{term_b22 ** 2 / a226 ** 2:.4f}')

# %%
# Formulae 234
# ------------
# `Page 46`
#
# .. math::
#
#       [(A^2 + 2AB \times cos(2\upsilon) + B^2)^\frac{1}{2} \times
#       cos(\upsilon')]_0
#
# .. math::
#
#       \left[0.5023 \times sin^2(2I) cos(\upsilon) + 0.0365\right]_0 = 0.1151
a234: float = (term_a47 * a71 + term_b47)
print(f'a226 = {a234:.4f}')

# %%
# Formulae 235
# ------------
# `Page 46`
#
# .. math::
#
#       f(K_2) = (0.2523 \times sin^4(I) + 0.0367 \times sin^2(I) \times
#       cos(2\upsilon) + 0.0013)^\frac{1}{2}/0.1151
#
# .. math::
#
#       f(K_2) = (19.0444 \times sin^4(I) + 2.7702 \times sin^2(I) \times
#       cos(2\upsilon) + 0.0981)^\frac{1}{2}
print(f'{term_a47**2 / a234**2:.4f}')
print(f'{(term_a47 * term_b47 * 2) / a234**2:.4f}')
print(f'{term_b47**2 / a234**2:.4f}')
