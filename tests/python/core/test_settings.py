# Copyright (c) 2026 CNES
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.
import pytest

from pyfes import core


@pytest.mark.parametrize(
    'settings_type', [core.FESSettings, core.PerthSettings]
)
def test_with_methods_return_self(settings_type) -> None:
    """The ``with_*`` methods return the settings object itself."""
    settings = settings_type()
    chained = (
        settings.with_astronomic_formulae(core.Formulae.IERS)
        .with_time_tolerance(60.0)
        .with_group_modulations(True)
        .with_compute_long_period_equilibrium(False)
        .with_inference_type(core.InferenceType.ZERO)
        .with_num_threads(2)
        .with_engine_type(settings.engine_type)
    )
    assert chained is settings
    assert type(chained) is settings_type
    assert settings.astronomic_formulae == core.Formulae.IERS
    assert settings.time_tolerance == 60.0
    assert settings.group_modulations
    assert not settings.compute_long_period_equilibrium
    assert settings.inference_type == core.InferenceType.ZERO
    assert settings.num_threads == 2


def test_with_methods_on_a_temporary() -> None:
    """Chaining on a temporary object keeps its type and its values."""
    settings = core.FESSettings().with_num_threads(3)
    assert isinstance(settings, core.FESSettings)
    assert settings.num_threads == 3
