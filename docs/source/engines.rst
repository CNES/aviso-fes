.. _prediction_engines:

==================
Prediction Engines
==================

PyFES implements two prediction engines that differ in their constituent
notation and approach to :term:`nodal corrections <nodal correction>`. Both
engines implement the same fundamental
:doc:`harmonic method <theory/index>` and share the same
:ref:`inference modes <inference_types>`,
:ref:`astronomic formulae <astronomic_formulae>`, and
:ref:`prediction functions <prediction_functions>`.

The choice of engine is determined by the tidal atlas format.

Engine Comparison
=================

.. list-table::
   :header-rows: 1
   :widths: 30 35 35

   * - Feature
     - FES/Darwin
     - PERTH/Doodson
   * - **YAML key**
     - ``engine: darwin``
     - ``engine: perth``
   * - **Settings class**
     - :class:`~pyfes.FesRuntimeSettings`
     - :class:`~pyfes.PerthRuntimeSettings`
   * - **Notation**
     - :term:`Darwin notation`
     - :term:`Doodson numbers <Doodson number>`
   * - **Constituents**
     - 142
     - 123
   * - **Nodal corrections**
     - Individual Schureman factors
     - Group modulations (configurable)
   * - **Default inference**
     - ``SPLINE``
     - ``LINEAR``
   * - **Default formulae**
     - ``SCHUREMAN_ORDER_1``
     - ``MEEUS``
   * - **Compatible atlases**
     - FES2014, FES2022
     - GOT4.10, GOT5.5, GOT5.6

.. _darwin_notation:

FES/Darwin Engine
=================

The Darwin engine uses the harmonic notation system developed by Sir George
Darwin and refined by Paul Schureman. Each constituent is identified by a
traditional name (e.g., :math:`M_2`, :math:`S_2`, :math:`K_1`) derived from
the underlying astronomical forcing.

Constituent Notation
--------------------

In Darwin notation, each constituent's tidal argument is expressed as a linear
combination of the :term:`fundamental variables`:

.. math::

    V_k = n_1 \tau + n_2 s + n_3 h + n_4 p + n_5 N' + n_6 p_1

where the integer coefficients :math:`n_i` are specific to each constituent.
For the full derivation, see :doc:`theory/harmonic_development`.

Nodal Corrections
-----------------

The Darwin engine applies **individual Schureman nodal corrections** to each
constituent. The amplitude factor :math:`f_k` and phase correction :math:`u_k`
are computed from the auxiliary astronomical angles (obliquity :math:`I`,
longitude of the lunar node :math:`N`, etc.) using Schureman's formulas 73--80
and 215--235. For example:

.. math::

    f(M_2) = \frac{\cos^4(I/2)}{0.9154}, \qquad u(M_2) = 2\xi - 2\nu

For the complete set of nodal correction formulas, see
:doc:`theory/nodal_corrections`.

Default Settings
----------------

.. code-block:: python

    settings = pyfes.FesRuntimeSettings()
    # Defaults:
    #   inference_type = SPLINE
    #   astronomic_formulae = SCHUREMAN_ORDER_1
    #   group_modulations = False

Constituent List
----------------

The Darwin engine supports 142 tidal constituents. See
:doc:`constituents/darwin` for the complete list with frequencies and
:term:`XDO notation`.

.. _doodson_notation:

PERTH/Doodson Engine
=====================

The PERTH engine uses the systematic :term:`Doodson number` classification
developed by Arthur Doodson in 1921. It was implemented by Dr. Richard Ray at
NASA Goddard Space Flight Center for the GOT (Goddard Ocean Tide) model
series.

Doodson Number Classification
------------------------------

Each constituent is assigned a six-digit number encoding its
:term:`fundamental variables` coefficients with an offset of +5:

.. math::

    \text{Doodson number} = (n_1)(n_2+5)(n_3+5).(n_4+5)(n_5+5)(n_6+5)

For example, :math:`M_2` has coefficients (2, 0, 0, 0, 0, 0) and is encoded
as **255.555**. See :doc:`theory/harmonic_development` for the derivation
and worked examples.

The :term:`XDO notation` provides an equivalent alphabetical encoding used
internally by PyFES.

Group Modulations
-----------------

The PERTH engine supports **group modulations**, an alternative to individual
nodal corrections. In this approach, closely related constituents within a
tidal group are modulated together rather than individually. This can be
enabled or disabled through the settings:

.. code-block:: python

    settings = pyfes.PerthRuntimeSettings().with_group_modulations(True)

When group modulations are disabled, the engine falls back to individual
nodal corrections similar to the Darwin approach.

Default Settings
----------------

.. code-block:: python

    settings = pyfes.PerthRuntimeSettings()
    # Defaults:
    #   inference_type = LINEAR
    #   astronomic_formulae = MEEUS
    #   group_modulations = True

Constituent List
----------------

The PERTH/Doodson engine supports 123 tidal constituents. See
:doc:`constituents/perth` for the complete list with frequencies and
:term:`XDO notation`.

Choosing an Engine
==================

The engine choice depends on your tidal atlas:

- **FES atlases** (FES2014, FES2022): use ``engine: darwin``.
- **GOT atlases** (GOT4.10, GOT5.5, GOT5.6): use ``engine: perth``.

The engine is set in the YAML configuration file. If omitted, it defaults to
``darwin``. All other settings (inference mode, formulae, parallelism) are
independent of the engine choice and can be configured freely with either
engine. See :ref:`inference_types` and :ref:`astronomic_formulae`.
