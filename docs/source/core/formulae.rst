.. currentmodule:: pyfes.core

.. class:: Formulae

    This Enum class encapsulates the various astronomical formulae required for
    computing astronomical angles.

    .. method:: __init__(self, value)

        Constructor.

        :param value: The value of the formula.

    .. autoattribute:: kSchuremanOrder1

        Schureman's formula of order 1.

    .. autoattribute:: kSchuremanOrder3

        Schureman's formula of order 3.

    .. autoattribute:: kMeeus

        Meeus' formula.

    .. autoattribute:: kIERS

        `IERS' formula <https://hpiers.obspm.fr/eop-pc/models/nutations/nut_oceanic.html>`_.

    .. property:: name

        Returns the name of the formula.

    .. property:: value

        Returns the value of the formula.
