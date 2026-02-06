.. currentmodule:: pyfes.core

.. class:: EngineType

    This Enum class represents the different tidal engines available in PyFES
    for performing tidal computations. Each engine type corresponds to a
    specific method of calculating tidal predictions based on the underlying
    mathematical models.

    .. method:: __init__(self, value)

        Constructor.

        :param value: The value of the engine type.

    .. autoattribute:: DARWIN

        DARWIN engine, used by FES models.

    .. autoattribute:: DOODSON

        DOODSON engine, used by GOT models.

    .. property:: name

        Returns the name of the engine type.

    .. property:: value

        Returns the value associated with the engine type.
