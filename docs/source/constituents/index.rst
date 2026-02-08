.. _constituents:

===================
Tidal Constituents
===================

PyFES provides two catalogues of tidal constituents, one for each prediction
engine. Every constituent is characterised by its name, angular speed (in
degrees per hour), and :term:`XDO notation` encoding.

- **Darwin catalogue** --- 99 constituents used by the FES/Darwin engine
  (``engine: darwin``). See :doc:`darwin`.
- **Doodson catalogue** --- 80 constituents used by the PERTH/Doodson engine
  (``engine: perth``). See :doc:`perth`.

For background on how constituent frequencies are derived from the six
:term:`fundamental variables`, see :doc:`../theory/harmonic_development`.
For a comparison of the two engines, see :doc:`../engines`.

.. toctree::
   :maxdepth: 1
   :hidden:

   darwin
   perth
