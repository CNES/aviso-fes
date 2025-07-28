PyFES documentation
===================

This package is the fully revised version of the FES2022 distribution. A full
description of the FES2022 tidal solution is given in the `handbook
<https://www.aviso.altimetry.fr/fileadmin/documents/data/tools/hdbk_FES2022.pdf>`_
and in the paper (Lyard et al. 2024).

The library uses a set of tidal components to predict the ocean tide at any
location on the Earth. The source code provides an older version of the FES
model (99), because it is significantly smaller than newer versions. **Do not
use it for scientific purposes**. You can download the model from the `AVISO
<https://www.aviso.altimetry.fr/en/data/products/auxiliary-products/global-tide-fes.html>`_
website.

.. note::

   This new code is under development and it is designed to handle the new finite
   element grids of FES2022. It will become official at the end of this year. For
   now, we do not provide support for this new version.

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
   pyfes
   api
   core
