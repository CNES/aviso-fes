.. _api_configuration:

=============
Configuration
=============

The configuration module loads a YAML file that describes which tidal atlas
files to use, the grid type, and the engine. This is the recommended way to set
up a prediction pipeline without hard-coding file paths.

.. autofunction:: pyfes.config.load

.. autoclass:: pyfes.config.Configuration
   :members:
   :undoc-members:

.. autoclass:: pyfes.config.Cartesian
   :members:
   :undoc-members:

.. autoclass:: pyfes.config.LGP
   :members:
   :undoc-members:
