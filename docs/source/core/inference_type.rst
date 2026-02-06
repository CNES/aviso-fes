.. currentmodule:: pyfes.core

.. class:: InferenceType

    This Enum class represents the different inference types available in PyFES
    for handling minor tidal constituents not explicitly provided in the tidal
    atlas. Each inference type corresponds to a specific method of estimating
    the contributions of these minor constituents based on the provided data
    and settings.

    .. method:: __init__(self, value)

        Constructor.

        :param value: The value of the inference type.

    .. autoattribute:: FOURIER

        Munk-Cartwright Fourier series interpolation of admittances.

    .. autoattribute:: LINEAR

        Piecewise linear interpolation of admittances, default method for the
        DOODSON engine.

    .. autoattribute:: SPLINE

        Spline interpolation of admittance, default method for the DARWIN
        engine.

    .. autoattribute:: ZERO

        No inference performed; use only explicitly provided constituents.

    .. property:: name

        Returns the name of the inference type.

    .. property:: value

        Returns the value associated with the inference type.
