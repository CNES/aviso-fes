.. currentmodule:: pyfes.core

.. _core_constituents:

Tidal constituents
==================

List of tidal constituents handled by the library. Use :py:func:`pyfes.core.darwin.constituents` to retrieve the runtime mapping from constituent string keys to internal values. Keys correspond to the names below (for example, ``M2``).

.. _constituent-mm:

.. rubric:: :math:`Mm`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Mm``.

=============  =============  =============
V              u              Factor-f
=============  =============  =============
:math:`s - p`  :math:`0`      :math:`f(Mm)`
=============  =============  =============

.. note::

    Schureman: Table 2, Page 164, Ref. A2

.. _constituent-mf:

.. rubric:: :math:`Mf`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Mf``.

=============  =============  =============
V              u              Factor-f
=============  =============  =============
:math:`2s`     :math:`-2\xi`  :math:`f(Mf)`
=============  =============  =============

.. note::

    Schureman: Table 2, Page 164, Ref. A6

.. _constituent-mtm:

.. rubric:: :math:`Mtm`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Mtm``.

==============  ==============  ==============
V               u               Factor-f
==============  ==============  ==============
:math:`3s - p`  :math:`-2\xi`   :math:`f(Mf)`
==============  ==============  ==============

.. note::

    Schureman: Table 2, Page 164, Ref. A7

.. _constituent-msqm:

.. rubric:: :math:`MSqm`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MSqm``.

===============  ===============  ===============
V                u                Factor-f
===============  ===============  ===============
:math:`4s - 2h`  :math:`-2\xi`    :math:`f(Mf)`
===============  ===============  ===============

.. note::

    Schureman: Table 2, Page 164, Ref. A12

.. _constituent-ssa:

.. rubric:: :math:`Ssa`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Ssa``.

==========  ==========  ==========
V           u           Factor-f
==========  ==========  ==========
:math:`2h`  :math:`0`   :math:`1`
==========  ==========  ==========

.. note::

    Schureman: Table 2, Page 164, Ref. B6

.. _constituent-sa:

.. rubric:: :math:`Sa`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Sa``.

==========  ==========  ==========
V           u           Factor-f
==========  ==========  ==========
:math:`h`   :math:`0`   :math:`1`
==========  ==========  ==========

.. note::

    Schureman: Table 2, Page 164, Ref. B64

.. _constituent-2q1:

.. rubric:: :math:`2Q_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2Q1``.

====================================  ====================================  ====================================
V                                     u                                     Factor-f
====================================  ====================================  ====================================
:math:`T - 4s + h + 2p + 90^{\circ}`  :math:`+2\xi - \nu`                   :math:`f(O_1)`
====================================  ====================================  ====================================

.. note::

    Schureman: Table 2, Page 164, Ref. A17

.. _constituent-sigma1:

.. rubric:: :math:`\sigma_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Sigma1``.

================================  ================================  ================================
V                                 u                                 Factor-f
================================  ================================  ================================
:math:`T - 4s + 3h + 90^{\circ}`  :math:`+2\xi - \nu`               :math:`f(O_1)`
================================  ================================  ================================

.. note::

    Schureman: Table 2, Page 164, Ref. A20

.. _constituent-q1:

.. rubric:: :math:`Q_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Q1``.

===================================  ===================================  ===================================
V                                    u                                    Factor-f
===================================  ===================================  ===================================
:math:`T - 3s + h + p + 90^{\circ}`  :math:`+2\xi - \nu`                  :math:`f(O_1)`
===================================  ===================================  ===================================

.. note::

    Schureman: Table 2, Page 164, Ref. A15

.. _constituent-rho1:

.. rubric:: :math:`\rho_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Rho1``.

====================================  ====================================  ====================================
V                                     u                                     Factor-f
====================================  ====================================  ====================================
:math:`T - 3s + 3h - p + 90^{\circ}`  :math:`+2\xi - \nu`                   :math:`f(O_1)`
====================================  ====================================  ====================================

.. note::

    Schureman: Table 2, Page 164, Ref. A18

.. _constituent-o1:

.. rubric:: :math:`O_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``O1``.

===============================  ===============================  ===============================
V                                u                                Factor-f
===============================  ===============================  ===============================
:math:`T - 2s + h + 90^{\circ}`  :math:`+2\xi - \nu`              :math:`f(O_1)`
===============================  ===============================  ===============================

.. note::

    Schureman: Table 2, Page 164, Ref. A14

.. _constituent-mp1:

.. rubric:: :math:`MP_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MP1``.

================================  ================================  ================================
V                                 u                                 Factor-f
================================  ================================  ================================
:math:`T - 2s + 3h - 90^{\circ}`  :math:`-\nu`                      :math:`f(J_1)`
================================  ================================  ================================

.. note::

    Schureman: Table 2, Page 164, Ref. A29

.. _constituent-m1:

.. rubric:: :math:`M1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``M1``.

==========================  ==========================  ==========================
V                           u                           Factor-f
==========================  ==========================  ==========================
:math:`T - s + h + p - 90`  :math:`-\nu`                :math:`f(M_1)`
==========================  ==========================  ==========================

.. _constituent-m11:

.. rubric:: :math:`M1_{1}`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``M11``.

==================================  ==================================  ==================================
V                                   u                                   Factor-f
==================================  ==================================  ==================================
:math:`T - s + h - p - 90^{\circ}`  :math:`+2\xi - \nu`                 :math:`f(O_1)`
==================================  ==================================  ==================================

.. note::

    Schureman: Table 2, Page 164, Ref. A16

.. _constituent-m12:

.. rubric:: :math:`M1_{2}`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``M12``.

==================================  ==================================  ==================================
V                                   u                                   Factor-f
==================================  ==================================  ==================================
:math:`T - s + h + p - 90^{\circ}`  :math:`-\nu`                        :math:`f(J_1)`
==================================  ==================================  ==================================

.. note::

    Schureman: Table 2, Page 164, Ref. A23

.. _constituent-m13:

.. rubric:: :math:`M1_{3}`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``M13``.

=================  =================  =================
V                  u                  Factor-f
=================  =================  =================
:math:`T - s + h`  :math:`\xi -\nu`   :math:`f(144)`
=================  =================  =================

.. note::

    Schureman: Table 2, Page 165, Ref. A71

.. _constituent-chi1:

.. rubric:: :math:`\chi_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Chi1``.

===================================  ===================================  ===================================
V                                    u                                    Factor-f
===================================  ===================================  ===================================
:math:`T - s + 3h - p - 90^{\circ}`  :math:`-\nu`                         :math:`f(J_1)`
===================================  ===================================  ===================================

.. note::

    Schureman: Table 2, Page 164, Ref. A27

.. _constituent-pi1:

.. rubric:: :math:`\pi_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Pi1``.

================================  ================================  ================================
V                                 u                                 Factor-f
================================  ================================  ================================
:math:`T - 2h + p1 + 90^{\circ}`  :math:`0`                         :math:`1`
================================  ================================  ================================

.. note::

    Schureman: Table 2, Page 165, Ref. B15

.. _constituent-p1:

.. rubric:: :math:`P_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``P1``.

==========================  ==========================  ==========================
V                           u                           Factor-f
==========================  ==========================  ==========================
:math:`T - h + 90^{\circ}`  :math:`0`                   :math:`1`
==========================  ==========================  ==========================

.. note::

    Schureman: Table 2, Page 165, Ref. B14

.. _constituent-s1:

.. rubric:: :math:`S_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``S1``.

==========  ==========  ==========
V           u           Factor-f
==========  ==========  ==========
:math:`T`   :math:`0`   :math:`1`
==========  ==========  ==========

.. note::

    Schureman: Table 2, Page 165, Ref. B71

.. _constituent-k1:

.. rubric:: :math:`K_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``K1``.

==========================  ==========================  ==========================
V                           u                           Factor-f
==========================  ==========================  ==========================
:math:`T + h - 90^{\circ}`  :math:`- \nu^{\prime}`      :math:`f(K_1)`
==========================  ==========================  ==========================

.. note::

    Schureman: Table 2, Page 165, Ref. Note 2

.. _constituent-psi1:

.. rubric:: :math:`\psi_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Psi1``.

================================  ================================  ================================
V                                 u                                 Factor-f
================================  ================================  ================================
:math:`T + 2h - p1 - 90^{\circ}`  :math:`0`                         :math:`1`
================================  ================================  ================================

.. note::

    Schureman: Table 2, Page 165, Ref. B24

.. _constituent-phi1:

.. rubric:: :math:`\phi_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Phi1``.

===========================  ===========================  ===========================
V                            u                            Factor-f
===========================  ===========================  ===========================
:math:`T + 3h - 90^{\circ}`  :math:`0`                    :math:`1`
===========================  ===========================  ===========================

.. note::

    Schureman: Table 2, Page 165, Ref. B31

.. _constituent-theta1:

.. rubric:: :math:`\theta_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Theta1``.

==================================  ==================================  ==================================
V                                   u                                   Factor-f
==================================  ==================================  ==================================
:math:`T + s - h + p - 90^{\circ}`  :math:`-\nu`                        :math:`f(J_1)`
==================================  ==================================  ==================================

.. note::

    Schureman: Table 2, Page 164, Ref. A28

.. _constituent-j1:

.. rubric:: :math:`J_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``J1``.

==================================  ==================================  ==================================
V                                   u                                   Factor-f
==================================  ==================================  ==================================
:math:`T + s + h - p - 90^{\circ}`  :math:`-\nu`                        :math:`f(J_1)`
==================================  ==================================  ==================================

.. note::

    Schureman: Table 2, Page 164, Ref. A24

.. _constituent-oo1:

.. rubric:: :math:`OO_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``OO1``.

===============================  ===============================  ===============================
V                                u                                Factor-f
===============================  ===============================  ===============================
:math:`T + 2s + h - 90^{\circ}`  :math:`-2\xi - \nu`              :math:`f(OO_1)`
===============================  ===============================  ===============================

.. note::

    Schureman: Table 2, Page 164, Ref. A31

.. _constituent-mns2:

.. rubric:: :math:`2MNS_2 = M_2 + N_2 + S_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MNS2``.

========================  ========================  ========================
V                         u                         Factor-f
========================  ========================  ========================
:math:`2T - 5s + 4h + p`  :math:`+4\xi - 4\nu`      :math:`f(M_2)^2`
========================  ========================  ========================

.. note::

    Schureman: Table 2, Page 167, Ref. MNS2

.. _constituent-eps2:

.. rubric:: :math:`\varepsilon_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Eps2``.

========================  ========================  ========================
V                         u                         Factor-f
========================  ========================  ========================
:math:`2T - 5s + 4h + p`  :math:`+2\xi - 2\nu`      :math:`f(M_2)`
========================  ========================  ========================

.. _constituent-2n2:

.. rubric:: :math:`2N_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2N2``.

=========================  =========================  =========================
V                          u                          Factor-f
=========================  =========================  =========================
:math:`2T - 4s + 2h + 2p`  :math:`+2\xi - 2\nu`       :math:`f(M_2)`
=========================  =========================  =========================

.. note::

    Schureman: Table 2, Page 165, Ref. A42

.. _constituent-mu2:

.. rubric:: :math:`\mu_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Mu2``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`2T - 4s + 4h`  :math:`+2\xi - 2\nu`  :math:`f(M_2)`
====================  ====================  ====================

.. note::

    Schureman: Table 2, Page 165, Ref. A45

.. _constituent-2ms2:

.. rubric:: :math:`2MS_2 = 2M_2 - S_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2MS2``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`2T - 4s + 4h`  :math:`+4\xi - 4\nu`  :math:`f(M_2)^2`
====================  ====================  ====================

.. _constituent-n2:

.. rubric:: :math:`N_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``N2``.

========================  ========================  ========================
V                         u                         Factor-f
========================  ========================  ========================
:math:`2T - 3s + 2h + p`  :math:`+2\xi - 2\nu`      :math:`f(M_2)`
========================  ========================  ========================

.. note::

    Schureman: Table 2, Page 165, Ref. A40

.. _constituent-nu2:

.. rubric:: :math:`\nu_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Nu2``.

========================  ========================  ========================
V                         u                         Factor-f
========================  ========================  ========================
:math:`2T - 3s + 4h - p`  :math:`+2\xi - 2\nu`      :math:`f(M_2)`
========================  ========================  ========================

.. note::

    Schureman: Table 2, Page 165, Ref. A43

.. _constituent-m2:

.. rubric:: :math:`M_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``M2``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`2T - 2s + 2h`  :math:`+2\xi - 2\nu`  :math:`f(M_2)`
====================  ====================  ====================

.. note::

    Schureman: Table 2, Page 165, Ref. A39

.. _constituent-mks2:

.. rubric:: :math:`MKS_2 = M_2 + K_2 - S_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MKS2``.

==========================================  ==========================================  ==========================================
V                                           u                                           Factor-f
==========================================  ==========================================  ==========================================
:math:`2T - 2s + 4h`                        :math:`+2\xi - 2\nu -2\nu^{\prime \prime}`  :math:`f(M_2) \times f(K_2)`
==========================================  ==========================================  ==========================================

.. _constituent-lambda2:

.. rubric:: :math:`\lambda_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Lambda2``.

================================  ================================  ================================
V                                 u                                 Factor-f
================================  ================================  ================================
:math:`2T - s + p + 180^{\circ}`  :math:`+2\xi - 2\nu`              :math:`f(M_2)`
================================  ================================  ================================

.. note::

    Schureman: Table 2, Page 165, Ref. A44

.. _constituent-l2:

.. rubric:: :math:`L_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``L2``.

=====================================  =====================================  =====================================
V                                      u                                      Factor-f
=====================================  =====================================  =====================================
:math:`2T - s + 2h - p + 180^{\circ}`  :math:`+2\xi - 2\nu - R`               :math:`f(L_2)`
=====================================  =====================================  =====================================

.. note::

    Schureman: Table 2, Page 166, Ref. Note 3

.. _constituent-2mn2:

.. rubric:: :math:`2MN_2 = 2M_2 - N_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2MN2``.

=====================================  =====================================  =====================================
V                                      u                                      Factor-f
=====================================  =====================================  =====================================
:math:`2T - s + 2h - p + 180^{\circ}`  :math:`+2\xi - 2\nu`                   :math:`f(M_2)^3`
=====================================  =====================================  =====================================

.. _constituent-t2:

.. rubric:: :math:`T_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``T2``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`2T - h + p_1`  :math:`0`             :math:`1`
====================  ====================  ====================

.. note::

    Schureman: Table 2, Page 166, Ref. B40

.. _constituent-s2:

.. rubric:: :math:`S_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``S2``.

==========  ==========  ==========
V           u           Factor-f
==========  ==========  ==========
:math:`2T`  :math:`0`   :math:`1`
==========  ==========  ==========

.. note::

    Schureman: Table 2, Page 166, Ref. B39

.. _constituent-r2:

.. rubric:: :math:`R_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``R2``.

=================================  =================================  =================================
V                                  u                                  Factor-f
=================================  =================================  =================================
:math:`2T + h - p1 + 180^{\circ}`  :math:`0`                          :math:`1`
=================================  =================================  =================================

.. note::

    Schureman: Table 2, Page 166, Ref. B41

.. _constituent-k2:

.. rubric:: :math:`K_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``K2``.

=============================  =============================  =============================
V                              u                              Factor-f
=============================  =============================  =============================
:math:`2T + 2h`                :math:`-2\nu^{\prime \prime}`  :math:`f(K_2)`
=============================  =============================  =============================

.. note::

    Schureman: Table 2, Page 166, Ref. Note 4

.. _constituent-msn2:

.. rubric:: :math:`MSN_2 = M_2 + S_2 - N_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MSN2``.

=================  =================  =================
V                  u                  Factor-f
=================  =================  =================
:math:`2T + s -p`  :math:`0`          :math:`f(M_2)^2`
=================  =================  =================

.. _constituent-eta2:

.. rubric:: :math:`\eta_2 = KJ_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Eta2``.

=======================  =======================  =======================
V                        u                        Factor-f
=======================  =======================  =======================
:math:`2T + s + 2h - p`  :math:`-2\nu`            :math:`f(KJ_2)`
=======================  =======================  =======================

.. note::

    Schureman: Table 2, Page 165, Ref. A49

.. _constituent-2sm2:

.. rubric:: :math:`2SM_2 = 2S_2 - M_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2SM2``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`2T + 2s - 2h`  :math:`-2\xi + 2\nu`  :math:`f(M_2)`
====================  ====================  ====================

.. _constituent-mo3:

.. rubric:: :math:`MO_3 = M_2 + O_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MO3``.

=================================  =================================  =================================
V                                  u                                  Factor-f
=================================  =================================  =================================
:math:`3T - 4s + 3h + 90^{\circ}`  :math:`4\xi - 3\nu`                :math:`f(M_2) \times f(O_1)`
=================================  =================================  =================================

.. _constituent-2mk3:

.. rubric:: :math:`2MK_3 = 2M_2 - K_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2MK3``.

==================================  ==================================  ==================================
V                                   u                                   Factor-f
==================================  ==================================  ==================================
:math:`3T - 4s + 3h + 90^{\circ}`   :math:`4\xi - 4\nu + \nu^{\prime}`  :math:`f(M_2)^2 \times f(K_1)`
==================================  ==================================  ==================================

.. _constituent-m3:

.. rubric:: :math:`M_3`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``M3``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`3T - 3s + 3h`  :math:`+3\xi - 3\nu`  :math:`f(M_3)`
====================  ====================  ====================

.. note::

    Schureman: Table 2, Page 166, Ref. A82

.. _constituent-mk3:

.. rubric:: :math:`MK_3 = M_2 + K_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MK3``.

==================================  ==================================  ==================================
V                                   u                                   Factor-f
==================================  ==================================  ==================================
:math:`3T - 2s + 3h - 90^{\circ}`   :math:`2\xi - 2\nu - \nu^{\prime}`  :math:`f(M_2) \times f(K_1)`
==================================  ==================================  ==================================

.. _constituent-n4:

.. rubric:: :math:`N_4 = N_2 + N_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``N4``.

=========================  =========================  =========================
V                          u                          Factor-f
=========================  =========================  =========================
:math:`4T - 6s + 4h + 2p`  :math:`+4\xi - 4\nu`       :math:`f(M_2)^2`
=========================  =========================  =========================

.. _constituent-mn4:

.. rubric:: :math:`MN_4 = M_2 + N_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MN4``.

========================  ========================  ========================
V                         u                         Factor-f
========================  ========================  ========================
:math:`4T - 5s + 4h + p`  :math:`+4\xi - 4\nu`      :math:`f(M_2)^2`
========================  ========================  ========================

.. _constituent-m4:

.. rubric:: :math:`M_4 = 2M_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``M4``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`4T - 4s + 4h`  :math:`+4\xi - 4\nu`  :math:`f(M_2)^2`
====================  ====================  ====================

.. note::

    Schureman: Table 2a, Page 167

.. _constituent-sn4:

.. rubric:: :math:`SN_4 = S_2 + N_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``SN4``.

========================  ========================  ========================
V                         u                         Factor-f
========================  ========================  ========================
:math:`4T - 3s + 2h + p`  :math:`2\xi - 2\nu`       :math:`f(M_2)`
========================  ========================  ========================

.. _constituent-ms4:

.. rubric:: :math:`MS_4 = M_2 + S_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MS4``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`4T - 2s + 2h`  :math:`+2\xi - 2\nu`  :math:`f(M_2)`
====================  ====================  ====================

.. note::

    Schureman: Table 2a, Page 167.

.. _constituent-mk4:

.. rubric:: :math:`MK_4 = M_2 + K_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MK4``.

=========================================  =========================================  =========================================
V                                          u                                          Factor-f
=========================================  =========================================  =========================================
:math:`4T - 2s + 4h`                       :math:`2\xi - 2\nu - 2\nu^{\prime\prime}`  :math:`f(MK_4)`
=========================================  =========================================  =========================================

.. note::

    Schureman: Table 2a, Page 167

.. _constituent-s4:

.. rubric:: :math:`S_4 = S_2 + S_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``S4``.

==========  ==========  ==========
V           u           Factor-f
==========  ==========  ==========
:math:`4T`  :math:`0`   :math:`1`
==========  ==========  ==========

.. _constituent-sk4:

.. rubric:: :math:`SK_4 = S_2 + K_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``SK4``.

=============================  =============================  =============================
V                              u                              Factor-f
=============================  =============================  =============================
:math:`4T + 2h`                :math:`-2\nu^{\prime \prime}`  :math:`f(K_2)`
=============================  =============================  =============================

.. _constituent-r4:

.. rubric:: :math:`R_4 = R_2 + R_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``R4``.

======================  ======================  ======================
V                       u                       Factor-f
======================  ======================  ======================
:math:`4T + 2h - 2p_1`  :math:`0`               :math:`1`
======================  ======================  ======================

.. _constituent-2mn6:

.. rubric:: :math:`2MN_6 = 2M_2 + N_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2MN6``.

========================  ========================  ========================
V                         u                         Factor-f
========================  ========================  ========================
:math:`6T - 7s + 6h + p`  :math:`6\xi - 6\nu`       :math:`f(M_2)^3`
========================  ========================  ========================

.. _constituent-m6:

.. rubric:: :math:`M_6 = 3M_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``M6``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`6T - 6s + 6h`  :math:`+6\xi - 6\nu`  :math:`f(M_2)^3`
====================  ====================  ====================

.. _constituent-msn6:

.. rubric:: :math:`MSN_6 = M_2 + S_2 + N_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MSN6``.

========================  ========================  ========================
V                         u                         Factor-f
========================  ========================  ========================
:math:`6T - 5s + 4h + p`  :math:`4\xi - 4\nu`       :math:`f(M_2)^2`
========================  ========================  ========================

.. _constituent-2ms6:

.. rubric:: :math:`2MS_6 = 2M_2 + S_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2MS6``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`6T - 4s + 4h`  :math:`4\xi - 4\nu`   :math:`f(M_2)^2`
====================  ====================  ====================

.. _constituent-2mk6:

.. rubric:: :math:`2MK_6 = 2M_2 + K_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2MK6``.

==========================================  ==========================================  ==========================================
V                                           u                                           Factor-f
==========================================  ==========================================  ==========================================
:math:`6T - 4s + 6h`                        :math:`4\xi - 4\nu - 2\nu^{\prime \prime}`  :math:`f(M_2)^2 \times f(K_2)`
==========================================  ==========================================  ==========================================

.. _constituent-2sm6:

.. rubric:: :math:`2SM_6 = 2S_2 + M_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2SM6``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`6T - 2s + 2h`  :math:`2\xi - 2\nu`   :math:`f(M_2)`
====================  ====================  ====================

.. _constituent-msk6:

.. rubric:: :math:`MSK_6 = M_2 + K_2 + S_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MSK6``.

===================================  ===================================  ===================================
V                                    u                                    Factor-f
===================================  ===================================  ===================================
:math:`6T - 2s + 4h`                 :math:`2\xi - 2\nu - 2\nu^{\prime}`  :math:`f(M_2) \times f(K_2)`
===================================  ===================================  ===================================

.. _constituent-s6:

.. rubric:: :math:`S_6 = 3S_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``S6``.

==========  ==========  ==========
V           u           Factor-f
==========  ==========  ==========
:math:`6T`  :math:`0`   :math:`1`
==========  ==========  ==========

.. _constituent-m8:

.. rubric:: :math:`M_8 = 4M_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``M8``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`8T - 8s + 8h`  :math:`8\xi - 8\nu`   :math:`f(M_2)^4`
====================  ====================  ====================

.. _constituent-msf:

.. rubric:: :math:`MSf = M_2 - S_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MSf``.

================================  ================================  ================================
V                                 u                                 Factor-f
================================  ================================  ================================
:math:`2s - 2h`                   :math:`2\xi - 2\nu`               :math:`f(M_2) * f(S_2) = f(M_2)`
================================  ================================  ================================

.. _constituent-a5:

.. rubric:: :math:`A5`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``A5``.

===============  ===============  ===============
V                u                Factor-f
===============  ===============  ===============
:math:`2s - 2h`  :math:`0`        :math:`f(Mm)`
===============  ===============  ===============

.. note::

    Schureman: Table 2, Page 164, Ref. A5

.. _constituent-sa1:

.. rubric:: :math:`Sa_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Sa1``.

===============  ===============  ===============
V                u                Factor-f
===============  ===============  ===============
:math:`h - p_1`  :math:`0`        :math:`1`
===============  ===============  ===============

.. note::

    Schureman: Table 2, Page 164, Ref. B2

.. _constituent-sta:

.. rubric:: :math:`Sta`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Sta``.

================  ================  ================
V                 u                 Factor-f
================  ================  ================
:math:`3h - p_1`  :math:`0`         :math:`1`
================  ================  ================

.. note::

    Schureman: Table 2, Page 164, Ref. B7

.. _constituent-mm2:

.. rubric:: :math:`Mm_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Mm2``.

==============  ==============  ==============
V               u               Factor-f
==============  ==============  ==============
:math:`s - 90`  :math:`-\xi`    :math:`f(141)`
==============  ==============  ==============

.. note::

    Schureman: Table 2, Page 164, Ref. A64

.. _constituent-mm1:

.. rubric:: :math:`Mm_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Mm1``.

===================  ===================  ===================
V                    u                    Factor-f
===================  ===================  ===================
:math:`s + p + 180`  :math:`-2\xi`        :math:`f(Mf)`
===================  ===================  ===================

.. note::

    Schureman: Table 2, Page 164, Ref. A8

.. _constituent-mf1:

.. rubric:: :math:`Mf_1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Mf1``.

===============  ===============  ===============
V                u                Factor-f
===============  ===============  ===============
:math:`2s - 2p`  :math:`0`        :math:`f(Mm)`
===============  ===============  ===============

.. note::

    Schureman: Table 2, Page 164, Ref. A3

.. _constituent-mf2:

.. rubric:: :math:`Mf_2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``Mf2``.

===================  ===================  ===================
V                    u                    Factor-f
===================  ===================  ===================
:math:`2s - p - 90`  :math:`-\xi`         :math:`f(141)`
===================  ===================  ===================

.. note::

    Schureman: Table 2, Page 164, Ref. A65

.. _constituent-m0:

.. rubric:: :math:`M_0`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``M0``.

=============  =============  =============
V              u              Factor-f
=============  =============  =============
:math:`0`      :math:`0`      :math:`f(Mm)`
=============  =============  =============

.. note::

    Schureman: Table 2, Page 164, Ref. A1

.. _constituent-n2p:

.. rubric:: :math:`N_2P`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``N2P``.

=================================  =================================  =================================
V                                  u                                  Factor-f
=================================  =================================  =================================
:math:`2T - 3s + 2h + 90^{\circ}`  :math:`+3\xi - 2\nu`               :math:`f(146)`
=================================  =================================  =================================

.. note::

    Schureman: Table 2, Page 166, Ref. A76

.. _constituent-l2p:

.. rubric:: :math:`L_2P`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``L2P``.

================================  ================================  ================================
V                                 u                                 Factor-f
================================  ================================  ================================
:math:`2T - s + 2h - 90^{\circ}`  :math:`+\xi - 2\nu`               :math:`f(147)`
================================  ================================  ================================

.. note::

    Schureman: Table 2, Page 166, Ref. A79

.. _constituent-msk2:

.. rubric:: :math:`MSK2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MSK2``.

========================================  ========================================  ========================================
V                                         u                                         Factor-f
========================================  ========================================  ========================================
:math:`22T - 2s`                          :math:`2\xi -2\nu + 2\nu^{\prime\prime}`  :math:`f(M_2) \times f(K_2)`
========================================  ========================================  ========================================

.. _constituent-skm2:

.. rubric:: :math:`SKM2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``SKM2``.

==========================================  ==========================================  ==========================================
V                                           u                                           Factor-f
==========================================  ==========================================  ==========================================
:math:`2T + 2s`                             :math:`-2\xi + 2\nu - 2\nu^{\prime\prime}`  :math:`f(M_2) \times f(K_2)`
==========================================  ==========================================  ==========================================

.. _constituent-oq2:

.. rubric:: :math:`OQ2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``OQ2``.

=======================================  =======================================  =======================================
V                                        u                                        Factor-f
=======================================  =======================================  =======================================
:math:`2T - 5s + 2h + 1p + 180^{\circ}`  :math:`0`                                :math:`f(O_1) \times f(Q_1)`
=======================================  =======================================  =======================================

.. _constituent-3ms4:

.. rubric:: :math:`3MS4`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``3MS4``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`4T - 6s + 6h`  :math:`6\xi - 6\nu`   :math:`f(M_2)^3`
====================  ====================  ====================

.. _constituent-mnu4:

.. rubric:: :math:`MNu4 = M2 + Nu2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MNu4``.

======================  ======================  ======================
V                       u                       Factor-f
======================  ======================  ======================
:math:`4T -5s + 6h -p`  :math:`4\xi - 4\nu`     :math:`f(M_2)^2`
======================  ======================  ======================

.. _constituent-2msn4:

.. rubric:: :math:`2MSN4 = M2 + S2 - N2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2MSN4``.

=======================  =======================  =======================
V                        u                        Factor-f
=======================  =======================  =======================
:math:`4T - s + 2h - p`  :math:`2\xi - 2\nu`      :math:`f(M_2)^3`
=======================  =======================  =======================

.. _constituent-2ns2:

.. rubric:: :math:`2NS2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2NS2``.

=========================  =========================  =========================
V                          u                          Factor-f
=========================  =========================  =========================
:math:`2T - 6s + 4h + 2p`  :math:`4\xi - 4\nu`        :math:`f(M_2)^2`
=========================  =========================  =========================

.. _constituent-mnus2:

.. rubric:: :math:`MNuS2 = M2 + Nu2 - S2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MNuS2``.

========================  ========================  ========================
V                         u                         Factor-f
========================  ========================  ========================
:math:`2T - 5s + 6h - p`  :math:`4\xi - 4\nu`       :math:`f(M_2)^2`
========================  ========================  ========================

.. _constituent-2mk2:

.. rubric:: :math:`2MK2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2MK2``.

=========================================  =========================================  =========================================
V                                          u                                          Factor-f
=========================================  =========================================  =========================================
:math:`2T - 4s + 2h`                       :math:`4\xi - 4\nu + 2\nu^{\prime\prime}`  :math:`f(M_2)^2 \times f(K_2)`
=========================================  =========================================  =========================================

.. _constituent-nkm2:

.. rubric:: :math:`NKM2 = N2 + K2 - M2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``NKM2``.

==============================  ==============================  ==============================
V                               u                               Factor-f
==============================  ==============================  ==============================
:math:`2T -s + 2h + p`          :math:`-2\nu^{\prime\prime}`    :math:`f(M_2)^2 \times f(K_2)`
==============================  ==============================  ==============================

.. _constituent-ml4:

.. rubric:: :math:`ML4 = M2 + L2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``ML4``.

============================  ============================  ============================
V                             u                             Factor-f
============================  ============================  ============================
:math:`4T - 3s + 4h - p`      :math:`4\xi - 4\nu`           :math:`f(M_2) \times f(L_2)`
============================  ============================  ============================

.. _constituent-so1:

.. rubric:: :math:`SO1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``SO1``.

===============================  ===============================  ===============================
V                                u                                Factor-f
===============================  ===============================  ===============================
:math:`T + 2s - h - 90^{\circ}`  :math:`-\nu`                     :math:`f(O_1)`
===============================  ===============================  ===============================

.. _constituent-so3:

.. rubric:: :math:`SO3`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``SO3``.

================================  ================================  ================================
V                                 u                                 Factor-f
================================  ================================  ================================
:math:`3T -2s + 1h + 90^{\circ}`  :math:`2\xi - \nu`                :math:`f(O_1)`
================================  ================================  ================================

.. _constituent-nk4:

.. rubric:: :math:`NK4 = N2 + K2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``NK4``.

=========================================  =========================================  =========================================
V                                          u                                          Factor-f
=========================================  =========================================  =========================================
:math:`4T - 3s + 4h + p`                   :math:`2\xi - 2\nu - 2\nu^{\prime\prime}`  :math:`f(M_2) \times f(K_2)`
=========================================  =========================================  =========================================

.. _constituent-mnk6:

.. rubric:: :math:`MNK6 = M2 + N2 + K2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``MNK6``.

=========================================  =========================================  =========================================
V                                          u                                          Factor-f
=========================================  =========================================  =========================================
:math:`6T - 5s + 6h + p`                   :math:`4\xi - 4\nu - 2\nu^{\prime\prime}`  :math:`f(M_2)^2 \times f(K_2)`
=========================================  =========================================  =========================================

.. _constituent-2nm6:

.. rubric:: :math:`3MNL6 = 3M2 + N2 - L2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2NM6``.

==============================  ==============================  ==============================
V                               u                               Factor-f
==============================  ==============================  ==============================
:math:`6T - 8s + 6h + 2p`       :math:`6\xi - 6\nu`             :math:`f(M_2)^4 \times f(L_2)`
==============================  ==============================  ==============================

.. _constituent-3ms8:

.. rubric:: :math:`3MS8 = M2 + S2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``3MS8``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`8T - 6s + 6h`  :math:`6\xi - 6\nu`   :math:`f(M_2)^3`
====================  ====================  ====================

.. _constituent-sk3:

.. rubric:: :math:`SK3`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``SK3``.

===========================  ===========================  ===========================
V                            u                            Factor-f
===========================  ===========================  ===========================
:math:`3T + h - 90^{\circ}`  :math:`-1\nu^{\prime}`       :math:`f(K_1)`
===========================  ===========================  ===========================

.. _constituent-2mns4:

.. rubric:: :math:`2MNS4 = 2M2 + N2 - S2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2MNS4``.

========================  ========================  ========================
V                         u                         Factor-f
========================  ========================  ========================
:math:`4T - 7s + 6h + p`  :math:`-6\xi - 6\nu`      :math:`f(M_2)^3`
========================  ========================  ========================

.. _constituent-2smu2:

.. rubric:: :math:`2SMu2 = S2 - Mu2`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2SMu2``.

====================  ====================  ====================
V                     u                     Factor-f
====================  ====================  ====================
:math:`2T + 4s - 4h`  :math:`-2\xi + 2\nu`  :math:`f(M_2)`
====================  ====================  ====================

.. _constituent-2mp5:

.. rubric:: :math:`2MP5 = 2M2 + P1`

Identifier returned by :py:func:`pyfes.core.darwin.constituents`: ``2MP5``.

===================  ===================  ===================
V                    u                    Factor-f
===================  ===================  ===================
:math:`5T -4s + 3h`  :math:`4\xi - 4\nu`  :math:`f(M_2)^2`
===================  ===================  ===================
