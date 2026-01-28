.. _glossary:

========
Glossary
========

.. glossary::

    AVISO
        Archiving, Validation and Interpretation of Satellite Oceanographic data.

    CLS
        Collecte Localisation Satellites, a subsidiary of :term:`CNES`, is a
        company that provides satellite-based services for environmental
        monitoring, maritime surveillance, and sustainable resource management.

    CNES
        The Centre National d'Études Spatiales is the French space agency.

    Admittance
        The complex ratio between a minor tidal constituent and a major
        constituent, used to infer amplitudes and phases of constituents not
        explicitly provided in a tidal atlas. Both prediction engines support
        admittance inference but use different algorithms. See also
        :term:`Inference`.

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
        represented as 255.555. The PERTH5/Doodson prediction engine uses this
        notation system. See :ref:`doodson_notation` for details.

    FES
        Finite Element Solution, a series of global ocean tide atlases.

    Group Modulation
        A nodal correction technique used in the PERTH5/Doodson prediction
        engine where related constituents are modulated together as groups,
        rather than applying individual corrections to each constituent. This
        approach provides computational efficiency while maintaining accuracy.
        Enabled via ``with_group_modulations(True)`` in
        :class:`pyfes.PerthRuntimeSettings`.

    Inference
        The process of estimating minor tidal constituents from major
        constituents using admittance relationships. The PERTH5 engine offers
        three inference types: ``ZERO_ADMITTANCE`` (no inference),
        ``LINEAR_ADMITTANCE`` (linear interpolation), and
        ``FOURIER_ADMITTANCE`` (Fourier-based interpolation). The FES/Darwin
        engine uses traditional admittance methods. See also
        :term:`Admittance`.

    LEGOS
        Laboratoire d'Etudes en Géophysique et Océanographie Spatiales, a
        research laboratory involved in the development of FES.

    NOVELTIS
        A French company specializing in environmental sciences and sustainable
        development, involved in the development of FES.

    Tidal Constituent
        A single harmonic component of the tide. Each constituent represents a
        specific astronomical cycle that influences the Earth's tides.
