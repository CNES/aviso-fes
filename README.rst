DESCRIPTION
###########

This package is the fully revised version of the FES2014 distribution.
This distribution includes the FES2014 tidal prediction software managed on this
page and the FES2014 tides databases available on
`AVISO website <http://www.aviso.altimetry.fr/en/data/products/auxiliary
-products/global-tide-fes/>`_

The FES2014 tides database includes 3 components:

1. **Tide elevations**: FES2014b ocean tide model is distributed; this model has
been computed using the new FES2014 tidal loading grids. Two versions
are available:

* Non extrapolated version: the model is defined on a 1/16° grid directly
  interpolated from the finite element native grid of the model. Filenames
  are ``WAVE_FES2014b_elev.nc``.

* Extrapolated version: the above version has been extrapolated on the
  coasts in order to cover nearly entirely the coastal regions. The
  procedure is a simple extrapolation applied on 10 pixels and using mean
  values when necessary, it can be not enough accurate in some regions.
  Filenames are ``WAVE_FES2014b_elev_extrapol.nc``.

2. **Tide currents**: tidal curents derived from FES2014a model are delivered on
the 1/16° grid, directly interpolated from the finite element native grid.
No extrapolation is done on the coasts for the currents. Filenames are
``WAVE_FES2014a_uv.nc``.

3. **Tide loading**: the new FES2014b tide loading (noted FES2014_load) grids
are supplied on the same 1/16° grid.

FES2014_load must be used to compute the FES2014 geocentric (elastic) tide.
Filenames are ``WAVE_FES2014_load.nc``.

The tides database contains 34 tidal components for each tide product: 2N2,
Eps2, J1, K1, K2, L2, La2, M2 , M3, M4, M6, M8, Mf, MKS2, Mm, MN4, MS4, MSf,
MSqm, Mtm, Mu2, N2, N4, Nu2, O1, P1, Q1, R2, S1, S2, S4, Sa, Ssa, T2.

.. note:: To produce FES2014 tidal prediction with the software, you need to use
  the following parameter files:

  * FES2014 elevations (ocean or geocentric) tide: use `fes2014b_elev.ini`
    or `fes2014b_elev_extrapol.ini`

  * FES2014 currents: use `fes2014a_u.ini` and `fes2014a_v.ini`

CREDITS
#######

When using FES2014, please mention: *FES2014 was produced by NOVELTIS, LEGOS,
CLS Space Oceanography Division and CNES. It is distributed by AVISO, with
support from CNES (http://www.aviso.altimetry.fr/)*

HOW TO INSTALL THE SOFTWARE ?
#############################

The complete description of the build method is described in the `INSTALL
<INSTALL>`_ file located in the root of this software.

HOW TO USE THE SOFTWARE ?
#########################

This distribution contains a C API to be used to calculate the tide. The
`API <API.rst>`_ documentation describes the functions to be used to
perform the calculation. The directory `examples <examples>`_ contains an
example of using this API.

BIBLIOGRAPHY
############

* Lyard F., L. Carrere, M. Cancet, A. Guillot, N. Picot: *FES2014, a new finite
  elements tidal model for global ocean*, in preparation, to be submitted to
  Ocean Dynamics in 2016.

* Carrere L., F. Lyard, M. Cancet, A. Guillot, N. Picot: *FES 2014, a new tidal
  model - Validation results and perspectives for improvements*, presentation to
  ESA Living Planet Conference, Prague 2016.

CONTACT
#######

aviso@altimetry.fr