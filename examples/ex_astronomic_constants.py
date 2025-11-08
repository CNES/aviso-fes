"""********************************
Astronomic Constants Calculation
********************************

This notebook verifies the calculations of key coefficients found in Paul
Schureman's "Manual of Harmonic Analysis and Prediction of Tides" (Special
Publication No. 98, 1940 Edition). The Python script provided has been used to
recompute these values based on the astronomical constants and formulas detailed
in the manual.
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


def round_to_4_decimal(value: float, enabled: bool = True) -> float:
    """Rounds a float to 4 decimal places.

    Args:
        value (float): The value to round.
        enabled (bool): Whether to round the value.

    Returns:
        float: The rounded value.

    """
    return round(value, 4) if enabled else value


# %%
@dataclasses.dataclass
class AstronomicConstants:
    """Astronomic constants used in the Manual Of Harmonic Analysis and
    Prediction of Tides.
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

    def __str__(self) -> str:
        return (
            'Astronomic Constants:\n'
            f'  Inclination (i): {round_to_4_decimal(self.i)} rad\n'
            f'  Obliquity (w): {round_to_4_decimal(self.w)} rad\n'
            "  Eccentricity of Earth's orbit (e1): "
            f'{round_to_4_decimal(self.e1)}\n'
            "  Eccentricity of Moon's orbit (e): "
            f'{round_to_4_decimal(self.e)}\n'
            f'  Solar factor (s): {round_to_4_decimal(self.s)}'
        )


# %%
def schureman_values() -> AstronomicConstants:
    """Returns the Schureman's values as written in the book."""
    return AstronomicConstants(
        i=math.radians(5.145),
        w=math.radians(23.452),
        e1=0.016_75,
        e=0.054_90,
        s=0.4602,
    )


# %%
def schureman_recomputed_values() -> AstronomicConstants:
    """Returns the Schureman's values recomputed with the given values written
    in the book.
    """
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
    """Provides updated astronomic constants based on IERS Conventions (2010).

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
    s = (SE / ME) * (a_moon / AU) ** 3

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
# Astronomical Constants Used
# ===========================
#
# The following astronomic constants, as defined by Schureman, are used for the
# calculations. The values are converted to radians for use in Python's
# trigonometric functions.
const = schureman_values()
print(const)

# %%
# Calculation Checks
# ==================
#
# Below, each formula from the manual is presented, followed by the result from
# the Python script and a comparison with Schureman's published value.
#
# Formulae 65 & 73 (P. 24, 25)
# ----------------------------
# This formula calculates the mean value of the obliquity factor for the lunar
# long-period constituent :math:`Mm`
#
# .. math::
#
#   \left[ \frac{2}{3} - \sin^2 I \right]_0
#   = \left(\frac{2}{3} - \sin^2 \omega\right)
#       \left(1-\frac{3}{2} \sin^2 i\right)
#   = 0.5021
#
# .. math::
#
#   f(Mm) = \{2/3 - sin^2(I)\} / 0.5021
f65 = round_to_4_decimal(
    (2 / 3 - math.sin(const.w) ** 2) * (1 - 3 / 2 * math.sin(const.i) ** 2)
)
print(f'f65 = {f65:.4f}')

# %%
# Formulae 66 & 74 (P. 24, 25)
# ----------------------------
# This calculates the mean obliquity factor for the lunar long-period
# constituent :math:`Mf`.
#
# .. math::
#
#   \left[ \sin^2 I \cos 2\xi \right]_0
#   = \sin^2 \omega \cos^4\frac{i}{2}
#   = 0.1578
#
# .. math::
#
#       f(Mf) = sin^2(I) / 0.1578
f66 = round_to_4_decimal(math.sin(const.w) ** 2 * math.cos(0.5 * const.i) ** 4)
print(f'f66 = {f66:.4f}')

# %%
# Formulae 67 & 75 (P. 25)
# ------------------------
# This calculates the mean obliquity factor for the lunar diurnal constituent
# :math:`O_1`.
#
# .. math::
#
#   \left[ \sin I \cos^2 \frac{1}{2}I \cos(2\xi - \nu) \right]_0
#   = \sin\omega \cos^2\frac{\omega}{2} \cos^4\frac{i}{2}
#   = 0.3800
#
# .. math::
#
#   f(O_1) = sin(I) \times cos^2(\frac{1}{2}I) / 0.3800
f67 = round_to_4_decimal(
    math.sin(const.w)
    * math.cos(0.5 * const.w) ** 2
    * math.cos(0.5 * const.i) ** 4
)
print(f'f67 = {f67:.4f}')

# %%
# Formulae 68 & 76 (P. 25)
# ------------------------
# This calculates the mean obliquity factor for the lunar diurnal constituent
# :math:`J_1`.
#
# .. math::
#
#   \left[\sin2I\cos\nu \right]_0
#   = \sin(2\omega) \left(1 - \frac{3}{2} \sin^2i\right)
#   = 0.7214
#
# .. math::
#
#   f(J_1) = sin(2I) / 0.7214
f68 = round_to_4_decimal(
    math.sin(2 * const.w) * (1 - 3 / 2 * math.sin(const.i) ** 2)
)
print(f'f68 = {f68:.4f}')

# %%
# Formulae 69 & 77 (P. 25)
# ------------------------
# This calculates the mean obliquity factor for the lunar diurnal constituent
# :math:`OO_1`.
#
# .. math::
#
#   \left[ \sin{I}\sin^2{\frac{1}{2}I}\cos(2\xi + \nu) \right]_0
#   = \sin(\omega) \sin^2(\frac{\omega}{2}) \cos^4(\frac{i}{2})
#   = 0.0164
#
# .. math::
#
#   f(OO_1) = sin(I) \times sin^2(\frac{1}{2}I) / 0.0164
f69 = round_to_4_decimal(
    math.sin(const.w)
    * math.sin(0.5 * const.w) ** 2
    * math.cos(0.5 * const.i) ** 4
)
print(f'f69 = {f69:.4f}')

# %%
# Formulae 70 & 78 (P. 25)
# ------------------------
# This calculates the mean obliquity factor for the principal lunar semidiurnal
# constituent :math:`M_2`.
#
# .. math::
#
#   \left[ \cos^4 \frac{1}{2} I \times \cos(2\xi - 2\nu) \right]_0
#   = cos^4(\frac{1}{2}\omega) \times cos^4(\frac{1}{2}i)
#   = 0.9154
#
# .. math::
#
#   f(M_2) = cos^4(\frac{1}{2}I) / 0.9154
f70 = round_to_4_decimal(
    math.cos(0.5 * const.w) ** 4 * math.cos(0.5 * const.i) ** 4
)
print(f'f70 = {f70:.4f}')

# %%
# Formulae 71 & 79 (P. 25)
# ------------------------
# This calculates a mean obliquity factor used for several semidiurnal
# constituents.
#
# .. math::
#
#   \left[\sin^2I \cos{2\nu} \right]_0
#   = \sin^2(\omega) \left(1 - \frac{3}{2} \sin^2(i)\right)
#   = 0.1565
#
# .. math::
#
#   f(79) = \sin^2I / 0.1565
f71 = round_to_4_decimal(
    math.sin(const.w) ** 2 * (1 - 3 / 2 * math.sin(const.i) ** 2)
)
print(f'f71 = {f71:.4f}')

# %%
# Formulae 141 & 137 (P. 35, 36)
# ------------------------------
# This calculates a mean obliquity factor for certain long-period constituents
# derived from the 4th power of the moon's parallax.
#
# .. math::
#
#   \sin \overline{I} - \frac{5}{4} \sin^3\overline{I}
#   = 0.3192
#
# (where :math:`\overline{I} = \omega`)
#
# .. math::
#
#   f(141) = \sin(I) - \frac{5}{4} \times \sin^3(I) / 0.3192
f141 = round_to_4_decimal(math.sin(const.w) - 5 / 4 * math.sin(const.w) ** 3)
print(f'f141 = {f141:.4f}')

# %%
# Formulae 144 & 138 (P. 35, 36)
# ------------------------------
# This calculates a mean obliquity factor for certain diurnal constituents (like
# :math:`M_1`) derived from the 4th power of the moon's parallax.
#
# .. math::
#
#   \left(1 - 10 \sin^2\frac{\overline{I}}{2}
#       + 15 \sin^4\frac{\overline{I}}{2}\right)
#           \cos^2\frac{\overline{I}}{2}
#   = 0.5873
#
# .. math::
#
#   f(144) = \left(1 - 10 \sin^2(\frac{1}{2}I) + 15 \sin^4(\frac{1}{2}I)\right)
#   \cos^2(\frac{1}{2}I) / 0.5873
f144 = round_to_4_decimal(
    (1 - 10 * math.sin(0.5 * const.w) ** 2 + 15 * math.sin(0.5 * const.w) ** 4)
    * math.cos(0.5 * const.w) ** 2
)
print(f'f144 = {f144:.4f}')

# %%
# Formulae 146 & 139 (P. 35, 36)
# ------------------------------
# This calculates a mean obliquity factor for certain semidiurnal constituents
# derived from the 4th power of the moon's parallax.
#
# .. math::
#
#   \sin \overline{I} \cos^4\frac{\overline{I}}{2}
#
# (where :math:`\overline{I} = \omega`)
#
# .. math::
#
#     f(146) = \sin(I) \cos^4(\frac{1}{2}I) / 0.3658
f146 = round_to_4_decimal(math.sin(const.w) * math.cos(0.5 * const.w) ** 4)
print(f'f146 = {f146:.4f}')

# %%
# Formulae 147 & 139 (P. 35, 36)
# ------------------------------
# This calculates another mean obliquity factor for semidiurnal constituents.
#
# .. math::
#
#   \left(\cos^2\frac{\overline{I}}{2} - \frac{2}{3}\right)
#       \sin\overline{I}\cos^2\frac{\overline{I}}{2}
#   = 0.1114
#
# .. math::
#
#       f(147) = \left[\left(\cos^2(\frac{1}{2}I) - 2/3\right)
#       \sin(I)\cos^2(\frac{1}{2}I)\right] / 0.1114
f147 = round_to_4_decimal(
    (math.cos(0.5 * const.w) ** 2 - 2 / 3)
    * math.sin(const.w)
    * math.cos(0.5 * const.w) ** 2
)
print(f'f147 = {f147:.4f}')

# %%
# Formula 149 (P. 36)
# -------------------
# This formula provides the node factor for the terdiurnal constituent
# :math:`M_3`.
#
# .. math::
#
#   \left[\cos^6\frac{I}{2} \right]_0 = \cos^6\frac{\omega}{2}\cos^6\frac{i}{2}
#
# .. math::
#
#       f(M_3) = cos^6(\frac{1}{2}I) / 0.8758
f149 = round_to_4_decimal(
    math.cos(0.5 * const.w) ** 6 * math.cos(0.5 * const.i) ** 6
)
print(f'f149 = {f149:.4f}')

# %%
# Formula 197 (P. 41)
# -------------------
# This formula calculates the components for the amplitude factor :math:`1/Q_a`
# of the :math:`M_1` tide.
#
# .. math::
#
#   1/Q_a = \sqrt{\left(\frac{1}{4} + \frac{9}{4}\frac{\cos^2 I}
#   {\cos^4 \frac{1}{2} I}\right) + \left(\frac{3}{2}\frac{\cos I}
#   {\cos^2 \frac{1}{2}I}\right) \cos 2P}
#
# (where :math:`I = \omega`)
#
# This simplifies to:
#
# .. math::
#
#     1/Q_a = \sqrt{2.310 + 1.435 \cos 2P}
#
# .. math::
#
#   \cos(2P) = \cos(2 \times (p - \xi))
#
# Formulae 207 (P. 43)
# --------------------
#
# .. math::
#
#   f(M_ 1) = f(O_ 1) \times 1 / Q_a
f197_1 = round_to_4_decimal(
    (1 / 4) + (9 / 4) * (math.cos(const.w) ** 2 / math.cos(0.5 * const.w) ** 4)
)
f197_2 = round_to_4_decimal(
    (3 / 2) * (math.cos(const.w) / math.cos(0.5 * const.w) ** 2)
)
print(f'f197_1 = {f197_1:.3f}')
print(f'f197_2 = {f197_2:.3f}')

# %%
# Formulae 216-219 (P. 45)
# ------------------------
# These formulas compute the coefficients for the terms that combine to form the
# lunisolar constituents :math:`K_1` and :math:`K_2`.
#
# .. math::
#
#       term A_{22}
#       = (\frac{1}{2}+\frac{3}{4}e^2)\sin 2I
#           \cos(T+h-90^{\circ}-\nu)
#       = 0.5023 \sin 2I \cos (T+h-90^{\circ}-\nu)
#
# .. math::
#
#       term B_{22}
#       = (\frac{1}{2}+\frac{3}{4}e_1^2)S^{\prime}
#           \sin 2\omega \cos(T+h-90^{\circ})
#       = 0.1681 \cos (T+h-90^{\circ})
#
# .. math::
#
#       term A_{47}
#       = (\frac{1}{2}+\frac{3}{4}e^2)
#           \sin^2I \cos (2T+2h-2\nu)
#       = 0.5023 \sin^2I \cos (2T+2h-2\nu)
#
# .. math::
#
#       term B_{47}
#       = (\frac{1}{2}+\frac{3}{4}e_1^2)S^{\prime}
#           \sin^2\omega \cos (2T+2h)
#       = 0.0365 \cos (2T+2h)
f216 = round_to_4_decimal(1 / 2 + 3 / 4 * const.e**2)
f217 = round_to_4_decimal(
    (1 / 2 + 3 / 4 * const.e1**2) * const.s * math.sin(2 * const.w)
)
f218 = round_to_4_decimal(1 / 2 + 3 / 4 * const.e**2)
f219 = round_to_4_decimal(
    (1 / 2 + 3 / 4 * const.e1**2) * const.s * math.sin(const.w) ** 2
)
print(f'f216 = {f216:.4f}')
print(f'f217 = {f217:.4f}')
print(f'f218 = {f218:.4f}')
print(f'f219 = {f219:.4f}')

# %%
# Formulae 224 (P. 45)
# --------------------
# This provides the ratio of the solar coefficient to the lunar coefficient
# for the K1 tide.
#
# .. math::
#
#   \nu^{\prime}=tan^{-1}\frac{A \sin\nu}{A \cos\nu + B}
#   = tan^{-1} \frac{\sin 2I \sin \nu}{\sin 2I \cos\nu + 0.3347}
#
# where :math:`A = 0.5023 \sin(2I)` and :math:`B = 0.1681`.
f224 = round_to_4_decimal(f217 / f216)
print(f'f224 = {f224:.4f}')

# %%
# Formula 226 (P. 45)
# -------------------
# This calculates the mean value of the :math:`K_1` coefficient.
#
# .. math::
#
#   \left[\left(A^2 + 2AB \times cos(\upsilon) + B^2\right)^\frac{1}{2}
#       \times cos(\upsilon')\right]_0 = [A \cos\nu + B]_0
#
# .. math::
#
#   [A \cos\nu + B]_0
#   = \left[0.5023 \sin(2I) \cos\nu + 0.1681\right]_0
#   = 0.5305
f226 = round_to_4_decimal(f216 * f68 + f217)
print(f'f226 = {f226:.4f}')

# %%
# Formula 227 (P. 45)
# -------------------
# This gives the components for the node factor :math:`f(K_1)`.
#
# .. math::
#
#   f(K_1)^2 = \frac{A^2 + 2AB\cos\nu + B^2}{[A\cos\nu + B]_0^2}
#
# where :math:`A` and :math:`B` are the coefficients for :math:`term A_{22}`
# and :math:`term B_{22}`
#
# * :math:`A^2=0.2523`
# * :math:`2AB=0.1689`
# * :math:`B^2=0.0283`
#
# .. math::
#
#       f(K_1) = (0.2523 \times sin^2(2I) + 0.1689 \times sin(2I) \times
#       cos(\upsilon) + 0.0283)^\frac{1}{2}/0.5305
#
# * :math:`A^2/0.5305=0.8665`
# * :math:`2AB/0.5305=0.6001`
# * :math:`B^2/0.5305=0.1006`
#
# .. math::
#
#       f(K_1) = (0.8965 \times sin^2(2I) + 0.6001 \times sin(2I) \times
#       cos(\upsilon) + 0.1006)^\frac{1}{2}
denominator = f226**2
f227_1 = round_to_4_decimal(f216**2) / denominator
f227_2 = round_to_4_decimal(f216 * f217 * 2) / denominator
f227_3 = round_to_4_decimal(f217**2) / denominator
print(f'{f227_1:.4f}')
print(f'{f227_2:.4f}')
print(f'{f227_3:.4f}')

# %%
# Formulae 232 (P. 45)
# --------------------
# This provides the ratio of the solar coefficient to the lunar coefficient
# for the K2 tide.
#
# .. math::
#
#   \nu^{\prime\prime}=tan^{-1}\frac{A \sin 2\nu}{A \cos 2\nu + B}
#   = tan^{-1} \frac{\sin 2I \sin 2\nu}{\sin 2I \cos 2\nu + 0.0727}
#
# where :math:`A = 0.5023 \sin(2I)` and :math:`B = 0.0365`.
f232 = round_to_4_decimal(f219 / f218)
print(f'f232 = {f232:.4f}')

# %%
# Formula 234 (P. 46)
# -------------------
# This calculates the mean value of the :math:`K_2` coefficient.
#
# .. math::
#
#   \left[\left(A^2 + 2AB \cos(2\upsilon) + B^2\right)^\frac{1}{2}
#       \cos(\nu')\right]_0
#   = \left[A \cos2\nu + B\right]_0
#
# .. math::
#
#   \left[0.5023 \sin^2I \cos(2\nu) + 0.0365\right]_0 = 0.1151
f234 = round_to_4_decimal(f218 * f71 + f219)
print(f'f234 = {f234:.4f}')

# %%
# Formula 235 (P. 46)
# -------------------
# This gives the components for the node factor :math:`f(K_2)`.
#
# .. math::
#
#   f(K_2)^2 = \frac{A^2 + 2AB\cos(2\nu) + B^2}{[A\cos(2\nu) + B]_0^2}
#
# where :math:`A` and :math:`B` are the coefficients for :math:`term A_{47}`
# and :math:`term B_{47}`
#
# * :math:`A^2=0.2523`
# * :math:`2AB=0.0367`
# * :math:`B^2=0.0013`
#
# .. math::
#
#   f(K_2) = \left(0.2523 \sin^4(I) + 0.0367 \sin^2(I)
#       \cos(2\nu) + 0.0013\right)^\frac{1}{2}/0.1151
#
# * :math:`A^2/0.1151=19.0444`
# * :math:`2AB/0.1151=2.7702`
# * :math:`B^2/0.1151=0.0981`
#
# .. math::
#
#   f(K_2) = \left(19.0444 \sin^4(I) + 2.7702 \sin^2(I)
#       \cos(2\nu) + 0.0981\right)^\frac{1}{2}
denominator = f234**2
f235_1 = round_to_4_decimal(f218**2) / denominator
f235_2 = round_to_4_decimal(f218 * f219 * 2) / denominator
f235_3 = round_to_4_decimal(f219**2) / denominator
print(f'{f235_1:.4f}')
print(f'{f235_2:.4f}')
print(f'{f235_3:.4f}')

# %%
# Formulae from Page 156
# ----------------------
# These formulas from the "Explanation of Tables" section are fundamental for
# calculating the values of I, :math:`\nu`, and :math:`\xi`.
#
# .. math::
#
#   \cos(I) = \cos(i)\cos(\omega) - \sin(i)\sin(\omega)\cos(N)
#   = 0.91370 - 0.03569\cos(N)
cos_i_cos_w = math.cos(const.i) * math.cos(const.w)
sin_i_sin_w = math.sin(const.i) * math.sin(const.w)
print(f'cos(i) * cos(ω) = {cos_i_cos_w:.5f}')
print(f'sin(i) * sin(ω) = {sin_i_sin_w:.5f}')
# %%
# .. math::
#
#   \tan \frac{1}{2}(N - \xi + \nu)
#   = \frac{\cos \frac{1}{2}(\omega - i)}{\cos \frac{1}{2}(\omega + i)}
#       \tan \frac{1}{2}N
#   = 1.01883 \tan \frac{1}{2}N
numerator = math.cos(0.5 * (const.w - const.i))
denominator = math.cos(0.5 * (const.w + const.i))
tan_coeff_1 = numerator / denominator
print(f'{tan_coeff_1:.5f}')

# %%
# .. math::
#
#   \tan \frac{1}{2}(N - \xi - \nu)
#   = \frac{\sin \frac{1}{2}(\omega - i)}{\sin \frac{1}{2}(\omega + i)}
#       \tan \frac{1}{2}N
#   = 0.64412 \tan(\frac{1}{2}N)
numerator = math.sin(0.5 * (const.w - const.i))
denominator = math.sin(0.5 * (const.w + const.i))
tan_coeff_2 = numerator / denominator
print(f'{tan_coeff_2:.5f}')
