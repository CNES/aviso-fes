.. currentmodule:: pyfes.core

.. class:: Quality

    Enum class for quality flags.

    .. method:: __init__(self, value)

        Constructor.

        :param value: The value of the quality flag.

    .. autoattribute:: kExtrapolated1

        Value extrapolated with one data point

    .. autoattribute:: kExtrapolated2

        Value extrapolated with two data points

    .. autoattribute:: kExtrapolated3

        Value extrapolated with three data points

    .. autoattribute:: kInterpolated

        Value correctly interpolated

    .. autoattribute:: kUndefined

        Value undefined

    .. property:: name

        Returns the name of the quality flag.

    .. property:: value

        Returns the value of the quality flag.
