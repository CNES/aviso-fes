.. py:function:: pyfes.core.evaluate_tide(\
        tidal_model: Union[pyfes.core.AbstractTidalModelComplex64, \
                            pyfes.core.AbstractTidalModelComplex128], \
        date: VectorDateTime64, \
        leap_seconds: VectorUInt16, \
        longitude: VectorFloat64, \
        latitude: VectorFloat64, \
        settings: Optional[pyfes.core.Settings] = None, \
        num_threads: int = 0) -> Tuple[VectorFloat64, VectorFloat64, VectorFloat8]

   Ocean tide calculation.

   :param tidal_model: Tidal model used to interpolate the modelized waves
   :param date: Date of the tide calculation
   :param leap_seconds: Leap seconds at the date of the tide calculation
   :param longitude: Longitude in degrees for the position at which the tide is
        calculated
   :param latitude: Latitude in degrees for the position at which the tide is
        calculated
   :param settings: Settings for the tide computation.
   :param num_threads: Number of threads to use for the computation. If 0, the
        number of threads is automatically determined.
   :return: A tuple of three elements that contains:

        * The height of the diurnal and semi-diurnal constituents of the
          tidal spectrum (cm)
        * The height of the long period wave constituents of the tidal
          spectrum (cm)
        * A flag indicating if the tide is correctly estimated or not. Possible
          values are

            ======  ==========================================================
            0       The tide is undefined because the position is outside the
                    domain of the tidal model or the numerical model is
                    undefined for the requested spatial position.
            1       The tide is extrapolated with the nearest mesh point.
            2       The tide is extrapolated using a linear interpolation
                    with two surrounding grid points.
            3       The tide is extrapolated using a linear interpolation
                    with three surrounding grid points.
            4       The tide is correctly estimated.
            ======  ==========================================================

        .. note::

            The flag value ``2``, ``4`` or ``8`` are only possible if the
            tidal model used is a Cartesian grid.

.. py:function:: pyfes.core.evaluate_equilibrium_long_period(\
        dates: VectorDateTime64, \
        leap_seconds: VectorUInt16, \
        latitudes: VectorFloat64, \
        settings: Optional[pyfes.core.Settings] = None, \
        num_threads: int = 0) -> VectorFloat64

    Compute the long-period equilibrium ocean tides.

    :param dates: Dates of the tide calculation
    :param leap_seconds: Leap seconds at the date of the tide calculation
    :param latitudes: Latitude in degrees for the position at which the long-period
        tide is calculated
    :param settings: Settings for the tide computation.
    :param num_threads: Number of threads to use for the computation. If 0, the
        number of threads is automatically determined.
    :return: The computed long-period tide, in centimeters.
