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
def updated_astronomic_constants() -> AstronomicConstants:
    """
    Provides updated astronomic constants based on IERS Conventions (2010).

    The "solar factor" (s) is the ratio of the tide-generating forces of the
    Sun and Moon, calculated as: s = (M_sun / M_moon) * (a_moon / a_sun)^3
    This is equivalent to (S/E) / (M/E) * (a_moon / AU)^3.
    """
    # Mass of Sun / Mass of Earth (S/E) from G*M_sun and G*M_earth
    # IERS 2010 recommends using the geocentric gravitational constant (GE)
    # and the heliocentric gravitational constant (GS). GS/GE = (S/E).
    SE = 332946.0487  # This value is surprisingly stable and close to IERS.

    # Mass of Moon / Mass of Earth (M/E)
    # IERS Conventions (2010), Table 1.1
    ME = 0.0123000371  # This value is also very precise.

    # Instead of parallaxes, modern conventions use the semi-major axes.
    # Semi-major axis of lunar orbit (a_moon) in meters
    # IERS Conventions (2010), Table 1.1
    a_moon = 384402e3  # meters (Note: this is an average value)

    # Astronomical Unit (a_sun or AU) in meters
    # IERS Conventions (2010), a defining constant. IAU 2012 resolution.
    AU = 149597870700.0  # meters

    # The ratio of tide-generating forces (Solar Factor 's')
    # s = (SE / ME) * (a_moon / AU)**3
    s = (SE / ME) * (a_moon / AU)**3

    return AstronomicConstants(
        # Inclination of the mean lunar orbit to the mean ecliptic (I)
        # IERS Conventions (2010), Chapter 5, eq. 5.76
        i=math.radians(dms_to_deg(5, 8, 43.4)),  # 5.14539°

        # Obliquity of the ecliptic for J2000.0 (ε)
        # IERS Conventions (2010), Table 1.1
        w=math.radians(dms_to_deg(23, 26, 21.406)),

        # Eccentricity of the Earth's mean orbit for J2000.0
        # IERS Conventions (2010), Chapter 5
        e1=0.016708634,

        # Eccentricity of the Moon's mean orbit for J2000.0
        # IERS Conventions (2010), Chapter 5
        e=0.054900489,

        # The calculated solar factor
        s=s,
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
#   \left[\sin2I\cos\nu \right]_0
#   = sin(2\omega) \times \{1 - 3/2 \times sin^2(i)\} = 0.7214
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
#       sin(\overline{I}) -5/4 \times sin^3(\overline{I}) = 0.3192
#
# .. math::
#
#       f(141) = sin(\overline{I}) - 5/4 \times sin^3(\overline{I}) / 0.3192
a141 = math.sin(const.w) - 5 / 4 * math.sin(const.w)**3
print(f'a141 = {a141:.4f}')

# %%
# Formulae 144, 138
# -----------------
# `Page 35, 36`
#
# .. math::
#
#       (1 - 10 \sin^2(\frac{1}{2}\overline{I})
#       + 15 \sin^4(\frac{1}{2}\overline{I}))
#       \cos^2(\frac{1}{2}\overline{I}) = 0.5873
#
# .. math::
#
#       f(144) = (1 - 10 \sin^2(\frac{1}{2}I)
#       + 15 \sin^4(\frac{1}{2}I))
#       \cos^2(\frac{1}{2}I) / 0.5873
a144 = (1 - 10 * math.sin(0.5 * const.w)**2 +
        15 * math.sin(0.5 * const.w)**4) * math.cos(0.5 * const.w)**2
print(f'a144 = {a144:.4f}')

# %%
# Formulae 146, 139
# -----------------
# `Page 35, 36`
#
# .. math::
#
#      \sin \overline{I} \cos^4(\frac{1}{2}\overline{I}) = 0.5658
#
# .. math::
#
#     f(146) = \sin(I) \cos^4(\frac{1}{2}I) / 0.5658
a146 = math.sin(const.w) * math.cos(0.5 * const.w)**4
print(f'a146 = {a146:.4f}')

# %%
# Formulae 147, 139
# -----------------
# `Page 35, 36`
#
# .. math::
#
#       (\cos^2(\frac{1}{2}\overline{I}) - 2/3)
#       \sin(\overline{I})\cos^2(\frac{1}{2}\overline{I})
#       = 0.1114
#
# .. math::
#
#       f(147) = \left[(\cos^2(\frac{1}{2}I) - 2/3)
#       \sin(I)\cos^2(\frac{1}{2}I)\right] / 0.1114
a147 = ((math.cos(0.5 * const.w)**2 - 2 / 3) * math.sin(const.w) *
        math.cos(0.5 * const.w)**2)
print(f'a147 = {a147:.4f}')

# %%
# Formulae 149, 140
# -----------------
# `Page 35, 36`
#
# .. math::
#
#       cos^6(\frac{1}{2}I)
#       + cos^4(\frac{1}{2}I)
#       \times sin^2(\frac{1}{2}I) = 0.8758
#
# .. math::
#
#       f(M_3) = cos^6(\frac{1}{2}I) / 0.8758
a82 = a70**(3 / 2)
print(f'a82 = {a82:.4f}')

# %%
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
# Formulae 224
# ------------
# `Page 45`
#
# .. math::
#
#       \nu^{\prime}=tan^{-1}\frac{A \sin\nu}{A \cos \nu + B}
#       = tan^{-1} \frac{\sin 2I \sin \nu}{\sin 2I \cos \nu + 0.3347}
#
# where
#
# .. math::
#
#       A=0.5023*\sin(2I)
#
# .. math::
#
#       B=0.1681
#
print(f'{term_b22/term_a47:.4f}')
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
print(f'{a226:.4f}, {term_a22 ** 2:.4f}, '
      f'{term_a22 * term_b22 * 2:.4f}, {term_b22 ** 2:.4f}')
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
print(f'{a234:.4f}')
print(f'{term_a47**2 / a234**2:.4f}')
print(f'{(term_a47 * term_b47 * 2) / a234**2:.4f}')
print(f'{term_b47**2 / a234**2:.4f}')

# %%
# Fundamental astronomical data
# -----------------------------
# `Page 156`
#
# .. math::
#
#       cos(I) = cos(i) \times cos(\omega) - sin(i)
#       \times sin(\omega) \times cos(N)
print(f'cos(i) * cos(ω) = {math.cos(const.i) * math.cos(const.w):.5f}')
print(f'sin(i) * sin(ω) = {math.sin(const.i) * math.sin(const.w):.5f}')
# %%
# .. math::
#
#       tan \frac{1}{2}(N - \xi + \nu)
#       = \frac{cos \frac{1}{2}(\omega - i)}
#       {cos \frac{1}{2}(\omega + i)} tan \frac{1}{2}N
#       = 1.01883 tan \frac{1}{2}N

numerator = math.cos(0.5 * (const.w - const.i))
denominator = math.cos(0.5 * (const.w + const.i))
print(f'{numerator/denominator:.5f} * tan(1/2(N))')

# %%
# .. math::
#
#       tan \frac{1}{2}(N - \xi - \nu)
#       = \frac{sin \frac{1}{2}(\omega - i)}
#       {sin \frac{1}{2}(\omega + i)} tan \frac{1}{2}N
#       = 0.64412 * tan(1/2(N))
numerator = math.sin(0.5 * (const.w - const.i))
denominator = math.sin(0.5 * (const.w + const.i))
print(f'{numerator/denominator:.5f} * tan(1/2(N))')

# %%
