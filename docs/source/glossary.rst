.. _glossary:

========
Glossary
========

.. glossary::

    Admittance
        The complex ratio between a minor tidal constituent and a major
        constituent, used to infer amplitudes and phases of constituents not
        explicitly provided in a tidal atlas. Both prediction engines support
        admittance inference but use different algorithms. See also
        :term:`Inference`.

    Astronomical Argument
        The angle :math:`V_k(t) = n_1\tau + n_2 s + n_3 h + n_4 p + n_5 N'
        + n_6 p_1` that determines the phase of a :term:`tidal constituent`
        at time *t*. It is computed from the six :term:`fundamental variables`
        using integer coefficients encoded in the constituent's
        :term:`Doodson number`. See :doc:`theory/harmonic_development` for the
        full derivation.

    AVISO
        Archiving, Validation and Interpretation of Satellite Oceanographic data.

    CLS
        Collecte Localisation Satellites, a subsidiary of :term:`CNES`, is a
        company that provides satellite-based services for environmental
        monitoring, maritime surveillance, and sustainable resource management.

    CNES
        The Centre National d'Études Spatiales is the French space agency.

    Compound Wave
        A tidal constituent whose frequency is an integer linear combination of
        two or more astronomical constituent frequencies, arising from non-linear
        hydrodynamic interactions in shallow water. For example, the
        quarter-diurnal constituent :math:`M_4` has twice the frequency of
        :math:`M_2`. See :doc:`theory/compound_waves`.

    Darwin Notation
        A harmonic constituent notation system developed by Sir George Darwin
        and refined by Paul Schureman. Each constituent is expressed in terms
        of fundamental astronomical arguments (*s*, *h*, *p*, *N*, *p₁*) with
        time-varying nodal corrections. The FES/Darwin prediction engine uses
        this notation system for tidal constituents. See
        :ref:`darwin_notation` for details.

    Doodson Number
        A systematic numerical classification of tidal constituents developed
        by Arthur Doodson in 1921. Each constituent is assigned a six-digit
        number (format: :math:`D_1D_2D_3.D_4D_5D_6`) encoding the coefficients
        of fundamental astronomical arguments. For example, the M₂ tide is
        represented as 255.555. The PERTH/Doodson prediction engine uses this
        notation system. See :ref:`doodson_notation` for details and
        :doc:`theory/harmonic_development` for the mathematical derivation.

    Equilibrium Tide
        The hypothetical ocean surface shape that would exist if the ocean were
        in static gravitational equilibrium with the :term:`tide-generating
        potential`. For long-period tidal constituents, the ocean response
        closely approximates this equilibrium state. PyFES uses the
        Cartwright-Tayler-Edden spectral tables to compute the equilibrium
        long-period tide.

    FES
        Finite Element Solution, a series of global ocean tide atlases.

    Fundamental Variables
        The six time-varying astronomical angles that define all tidal
        constituent frequencies: :math:`\tau` (local mean lunar time),
        :math:`s` (mean longitude of the Moon), :math:`h` (mean longitude
        of the Sun), :math:`p` (longitude of lunar perigee), :math:`N'`
        (negative longitude of the Moon's ascending node), and :math:`p_1`
        (longitude of solar perihelion). Every tidal frequency is a linear
        combination of the rates of change of these six variables. See
        :doc:`theory/harmonic_development`.

    Group Modulation
        A nodal correction technique where related constituents are modulated
        together as groups, rather than applying individual corrections to each
        constituent. This approach provides computational efficiency while
        maintaining accuracy. Enabled via ``with_group_modulations(True)`` in
        the runtime settings.

    Inference
        The process of estimating minor tidal constituents from major
        constituents using admittance relationships. PyFES offers four generic
        inference types usable with any engine: ``ZERO`` (no inference),
        ``LINEAR`` (linear interpolation, recommended for GOT atlases),
        ``SPLINE`` (spline-based interpolation, recommended for FES atlases),
        and ``FOURIER`` (Fourier-based interpolation). See also
        :term:`Admittance` and :ref:`inference_types`.

    Laplace Species
        The three classes of tidal oscillation resulting from the spherical
        harmonic decomposition of the second-degree :term:`tide-generating
        potential`: long-period (zonal, :math:`m=0`), diurnal (tesseral,
        :math:`m=1`), and semidiurnal (sectoral, :math:`m=2`). The species
        number corresponds to the number of tidal cycles per day. See
        :doc:`theory/tide_generating_potential`.

    Legendre Polynomial
        Mathematical functions :math:`P_n(\cos z)` used in the expansion of
        the gravitational potential in spherical harmonics. The :math:`n=2`
        term dominates the :term:`tide-generating potential` and gives rise
        to the three :term:`Laplace species`.

    LEGOS
        Laboratoire d'Etudes en Géophysique et Océanographie Spatiales, a
        research laboratory involved in the development of FES.

    Lunar Node
        The point where the Moon's orbital plane crosses the ecliptic. The
        retrograde precession of this node, with a period of 18.61 years,
        produces the dominant long-term modulation of tidal amplitudes and
        phases, accounted for by :term:`nodal corrections <nodal correction>`.

    Nodal Correction
        Time-dependent factors :math:`f` (amplitude) and :math:`u` (phase)
        that account for the 18.61-year modulation of tidal constituents due
        to the precession of the :term:`lunar node`. In the prediction
        equation, the observed amplitude is :math:`f \cdot H` and the phase
        is adjusted by :math:`u`. See :doc:`theory/nodal_corrections`.

    Nodal Factor
        The amplitude modulation factor :math:`f` in the harmonic prediction
        equation. It varies slowly over the 18.61-year nodal cycle. For
        example, :math:`f(M_2) = \cos^4(I/2) / 0.9154` where :math:`I`
        is the obliquity of the lunar orbit. See :doc:`theory/nodal_corrections`
        for all formulae.

    NOVELTIS
        A French company specializing in environmental sciences and sustainable
        development, involved in the development of FES.

    Phase Lag
        The angular delay :math:`G` (or :math:`\kappa`) of a constituent's
        high water relative to the passage of the corresponding
        :term:`tide-generating potential` maximum. It is a location-specific
        value determined from tidal observations or numerical models, and
        forms one of the two harmonic constants (together with the amplitude)
        characterising a constituent at a given site.

    Rayleigh Criterion
        The minimum observation duration :math:`T` required to resolve two
        tidal constituents of frequencies :math:`f_1` and :math:`f_2`:
        :math:`T > 1/(f_1 - f_2)`. This criterion determines the number
        of independent constituents recoverable from harmonic analysis of a
        given record length. See :doc:`theory/harmonic_analysis`.

    Tidal Constituent
        A single harmonic component of the tide. Each constituent represents a
        specific astronomical cycle that influences the Earth's tides and is
        characterised by a unique frequency derived from the :term:`fundamental
        variables`, an amplitude, and a :term:`phase lag`.

    Tidal Species
        Classification of tidal constituents by their approximate frequency
        band: long-period (species 0), diurnal (species 1), semidiurnal
        (species 2), terdiurnal (species 3), quarter-diurnal (species 4),
        and so on. The species number equals the coefficient :math:`n_1` of
        the local mean lunar time :math:`\tau` in the :term:`Doodson number`.

    Tide-Generating Potential
        The gravitational potential arising from the differential attraction
        of the Moon and Sun between a point on the Earth's surface and the
        Earth's centre. It is the fundamental physical driver of ocean tides
        and can be expanded in :term:`Legendre polynomials <Legendre
        polynomial>` and decomposed into :term:`Laplace species`. See
        :doc:`theory/tide_generating_potential`.

    XDO Notation
        Extended Doodson Ordering, an alphabetical encoding of Doodson numbers.
        Each integer coefficient is mapped to a letter via the sequence
        R, S, T, U, V, W, X, Y, Z, A, B, C, D, E, F, G, H, I, J, ...
        where ``Z`` corresponds to coefficient 0. For example, :math:`M_2`
        with coefficients (2, 0, 0, 0, 0, 0, 0) is encoded as ``BZZZZZ``.
        See :doc:`theory/harmonic_development`.
