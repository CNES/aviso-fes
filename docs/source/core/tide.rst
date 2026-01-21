.. py:function:: pyfes.core.evaluate_tide(\
        tidal_model: Union[pyfes.core.AbstractTidalModelComplex64, \
                            pyfes.core.AbstractTidalModelComplex128], \
        date: VectorDateTime64, \
        longitude: VectorFloat64, \
        latitude: VectorFloat64, \
        settings: Optional[pyfes.core.Settings] = None, \
        num_threads: int = 0) -> Tuple[VectorFloat64, VectorFloat64, VectorFloat8]

   Ocean tide calculation.

   :param tidal_model: Tidal model used to interpolate the modelized waves
   :param date: Date of the tide calculation
   :param longitude: Longitude in degrees for the position at which the tide is
        calculated
   :param latitude: Latitude in degrees for the position at which the tide is
        calculated
   :param settings: Settings for the tide computation.
   :param num_threads: Number of threads to use for the computation. If 0, the
        number of threads is automatically determined.
   :return: A tuple of three elements that contains:
     * The height of the the diurnal and semi-diurnal constituents of the
       tidal spectrum (cm)
     * The height of the long period wave constituents of the tidal
       spectrum (cm)
     * The quality flag indicating the reliability of the tide
       calculation at the given position:

       * **0**: the tide is undefined (no model data available at the given
         position).
       * **Positive values**: the tide is interpolated at the given
         position using ``N`` data points (where ``N`` is the
         number of data points used for the interpolation).
       * **Negative values**: the tide is extrapolated at the given
         position using ``-N`` data points (where ``N`` is the
         number of data points used for the extrapolation).

    .. note::

      Computed height of the diurnal and semi-diurnal constituents is set
      to nan if no data is available at the given position. The long period wave
      constituents is always computed because this value does not depend on
      model data.

.. py:function:: pyfes.core.evaluate_equilibrium_long_period(\
        dates: VectorDateTime64, \
        latitudes: VectorFloat64, \
        settings: Optional[pyfes.core.Settings] = None, \
        num_threads: int = 0) -> VectorFloat64

    Compute the long-period equilibrium ocean tides.

    :param dates: Dates of the tide calculation
    :param latitudes: Latitude in degrees for the position at which the long-period
        tide is calculated
    :param settings: Settings for the tide computation.
    :param num_threads: Number of threads to use for the computation. If 0, the
        number of threads is automatically determined.
    :return: The computed long-period tide, in centimeters.
