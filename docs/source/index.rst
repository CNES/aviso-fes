PyFES documentation
===================

This package is the fully revised version of the FES2022 distribution. A full
description of the FES2022 tidal solution is given in the `handbook
<https://www.aviso.altimetry.fr/fileadmin/documents/data/tools/hdbk_FES2022.pdf>`_
and in the paper (Lyard et al. 2024).

The library uses a set of tidal components to predict the ocean tide at any
location on the Earth. The source code provides an older version of the FES99
model, because it is significantly smaller than newer versions. **Do not
use it for scientific purposes**. You can download the model from the `AVISO
<https://www.aviso.altimetry.fr/en/data/products/auxiliary-products/global-tide-fes.html>`_
website.

Theoretical Background: The Harmonic Method
--------------------------------------------

The prediction of tides is based on the principle of **harmonic analysis**, a
method developed into a practical application by Sir William Thomson
(Lord Kelvin) around 1867. The core idea is that the complex, periodic motion of
the tide at any location can be resolved into the sum of a series of simpler,
regular wave-like components known as **tidal constituents**. Each constituent
corresponds to a distinct astronomical forcing, such as the gravitational pull
of the Moon or Sun, or orbital variations like the evection and variation of the
moon.

As detailed in Schureman's manual [Schureman1940]_, the height of the tide, *h*,
at any time, *t*, can be expressed by the fundamental equation of harmonic
prediction:

.. math::

    h(t) = H_0 + A \cos(at + \alpha) + B \cos(bt + \beta) + C \cos(ct + \gamma) + \dots

Where:
    * :math:`H_0` is the mean height of the water level above the chart datum.
    * Each cosine term represents a single **tidal constituent** (e.g., the
      principal lunar semidiurnal tide, :math:`M_2`; the principal solar
      semidiurnal tide, :math:`S_2`; etc.).
    * Amplitude (:math:`A`, :math:`B`, :math:`C`...): This is the strength, or
      half the range, of each constituent. It is a location-specific value
      determined from the analysis of tidal observations.
    * Speed (:math:`a`, :math:`b`, :math:`c`...): This is the angular speed of
      the constituent, representing how quickly its phase changes. Speeds are
      constant for each constituent and are derived from universal astronomical
      data, such as the rotation of the Earth and the orbital periods of the
      Moon and Sun.
    * Phase Lag (:math:`\alpha`, :math:`\beta`, :math:`\gamma`...): Also known
      as the **epoch** (:math:`\kappa`), this value represents the timing of
      a constituent's high water relative to its theoretical astronomical
      forcing. Like the amplitude, it is a location-specific constant found
      through observation.

The :term:`FES` models, such as FES2022, are sophisticated global atlases that provide
the location-specific amplitudes (:math:`H`) and phase lags (:math:`\kappa`)
for a large number of tidal constituents. The **PyFES** library acts as the
harmonic prediction engine.

PyFES Dual-Engine Architecture
-------------------------------

PyFES implements **two distinct prediction engines** that differ in their
mathematical formulation and constituent notation:

* **FES/Darwin Engine** (``engine: darwin``): Uses :term:`Darwin notation` with
  Schureman's nodal corrections. This is the classical approach developed for
  FES tidal atlases (FES2014, FES2022). It supports 142 tidal constituents and
  follows traditional oceanographic conventions.

* **PERTH5/Doodson Engine** (``engine: perth``): Uses :term:`Doodson number`
  classification with group modulations. Developed by Dr. Richard Ray at NASA
  Goddard Space Flight Center, this engine is designed for GOT (Goddard Ocean
  Tide) models. It supports 123 tidal constituents.

Both engines support configurable inference modes (``ZERO``, ``LINEAR``,
``SPLINE``, ``FOURIER``) for handling minor constituents.

Both engines implement the same fundamental harmonic method but differ in their
constituent representation and approach to nodal corrections. The choice of
engine depends on your tidal atlas format. For a detailed comparison, see
:ref:`prediction_engines`.

Prediction Workflow
-------------------

When a user requests a tide prediction for a specific location and time, PyFES:

1.  Retrieves the amplitude and phase for each constituent from the tidal model
    maps at the desired location.
2.  Calculates the astronomical argument (the angle inside the cosine function)
    for the specified time using the known astronomical speeds of each constituent.
3.  Applies nodal corrections appropriate to the selected engine:

    * **Darwin engine**: Individual Schureman nodal factors (*f*) and phase
      corrections (*u*)
    * **PERTH5 engine**: Group modulation corrections or individual corrections
      based on configuration

4.  Applies the fundamental prediction equation shown above, summing the
    contributions of all constituents.
5.  Infers minor constituents not in the atlas using admittance relationships.
6.  Adds the local mean sea level (:math:`H_0`) to produce the final predicted
    tide height relative to the datum.

References
~~~~~~~~~~

.. [Schureman1940] Schureman, P. (1940). *Manual of Harmonic Analysis and
    Prediction of Tides*. U.S. Coast and Geodetic Survey, Special
    Publication No. 98.

Bibliography
------------

* Lyard, F., Carrere, L., Fouchet, E., Cancet, M., Greenberg, D.,
  Dibarboure, G., and Picot, N.: FES2022 a step towards a SWOTcompliant tidal
  correction, Submitted to J. Geophy. Res., in review, 2025

* Lyard, F. H., Allain, D. J., Cancet, M., Carrère, L., and Picot, N.: FES2014
  global ocean tide atlas: design and performance, Ocean Sci., 17, 615-649,
  https://doi.org/10.5194/os-17-615-2021, 2021.

* Carrere L., F. Lyard, M. Cancet, A. Guillot, N. Picot: FES 2014, a new tidal
  model - Validation results and perspectives for improvements, presentation to
  ESA Living Planet Conference, Prague 2016.

Credits
-------
When using FES2022, please mention: ``FES2022 was produced by LEGOS, NOVELTIS
and CLS Ocean and Climate Division; the project was funded by CNES. It is
distributed by AVISO, with support from CNES (http://www.aviso.altimetry.fr/)``

Contact
-------

`aviso@altimetry.fr <mailto:aviso@altimetry.fr>`_

.. toctree::
   :hidden:
   :maxdepth: 1
   :caption: Contents:

   changelog
   setup
   conda
   auto_examples/index.rst
   prediction_engines
   pyfes
   api
   core
   cxx
   glossary
