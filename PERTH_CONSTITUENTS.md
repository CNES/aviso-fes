# Tidal Constituents for Perth

This document provides a comprehensive reference of tidal constituents supported
by the Perth prediction engine. Each constituent represents a harmonic component
of the tidal signal and can be used to compute tidal predictions, characterized
by its angular speed and Doodson number representation.

For cross-validation, the table below is compared with the official list of
tidal constituents available at
[this URL](
https://iho.int/mtg_docs/com_wg/IHOTC/IHOTC_Misc/TWCWG_Constituent_list.pdf),
which contains the same information.
| Name | Speed (deg/h) | XDO |
| --- | --- | --- |
| Node | 0.0000015  | Z ZZZ AZB |
| Sa1 | 0.0000272  | Z ZAZ ZYZ |
| Sa | 0.0000272  | Z ZAZ ZZZ |
| Ssa | 0.0000545  | Z ZBZ ZZZ |
| Sta | 0.0000817  | Z ZCZ ZZZ |
| MSm | 0.0003127  | Z AXA ZZZ |
| Mm | 0.0003610  | Z AZY ZZZ |
| MSf | 0.0006737  | Z BXZ ZZZ |
| Mf | 0.0007282  | Z BZZ ZZZ |
| MStm | 0.0010408  | Z CXA ZZZ |
| Mtm | 0.0010892  | Z CZY ZZZ |
| MSqm | 0.0014018  | Z DXZ ZZZ |
| Mqm | 0.0014502  | Z DZX ZZZ |
| 2Q1 | 0.0085243  | A WZB ZZC |
| Sigma1 (${\sigma}1$) | 0.0085726  | A WBZ ZZC |
| Q1 | 0.0088853  | A XZA ZZC |
| Rho1 (${\rho}1$) | 0.0089336  | A XBY ZZC |
| O1 | 0.0092463  | A YZZ ZZC |
| Tau1 (${\tau}1$) | 0.0093007  | A YBZ ZZA |
| Beta1 (${\beta}1$) | 0.0095589  | A ZXA ZZA |
| M13 | 0.0096103  | A ZZZ ZZB |
| M1 | 0.0096134  | A ZZA ZZA |
| Chi1 (${\chi}1$) | 0.0096617  | A ZBY ZZA |
| Pi1 (${\pi}1$) | 0.0098927  | A AWZ ZAC |
| P1 | 0.0099199  | A AXZ ZZC |
| S1 | 0.0099472  | A AYZ ZZB |
| K1 | 0.0099744  | A AZZ ZZA |
| Psi1 (${\psi}1$) | 0.0100016  | A AAZ ZYA |
| Phi1 (${\phi}1$) | 0.0100289  | A ABZ ZZA |
| Theta1 (${\theta}1$) | 0.0102871  | A BXA ZZA |
| J1 | 0.0103354  | A BZY ZZA |
| SO1 | 0.0106481  | A CXZ ZZA |
| OO1 | 0.0107026  | A CZZ ZZA |
| Ups1 (${\upsilon}1$) | 0.0110636  | A DZY ZZA |
| Eps2 (${\epsilon}2$) | 0.0181860  | B WBA ZZZ |
| 2N2 | 0.0184987  | B XZB ZZZ |
| 2MS2 | 0.0185470  | B XBZ ZZZ |
| 2MS2 | 0.0185470  | B XBZ ZZZ |
| N2P | 0.0188566  | B YZZ ZZA |
| N2 | 0.0188597  | B YZA ZZZ |
| Nu2 (${\nu}2$) | 0.0189080  | B YBY ZZZ |
| Gamma2 (${\gamma}2$) | 0.0191724  | B ZXB ZZB |
| Alpa2 | 0.0191934  | B ZYZ ZAB |
| M2 | 0.0192207  | B ZZZ ZZZ |
| Beta2 (${\beta}2$) | 0.0192479  | B ZAZ ZYZ |
| Delta2 (${\delta}2$) | 0.0192751  | B ZBZ ZZZ |
| Delta2 (${\delta}2$) | 0.0192751  | B ZBZ ZZZ |
| Lambda2 (${\lambda}2$) | 0.0195333  | B AXA ZZB |
| 2MN2 | 0.0195817  | B AZY ZZZ |
| L2 | 0.0195817  | B AZY ZZB |
| L2P | 0.0195847  | B AZZ ZZC |
| T2 | 0.0198671  | B BWZ ZAZ |
| S2 | 0.0198943  | B BXZ ZZZ |
| R2 | 0.0199216  | B BYZ ZYB |
| K2 | 0.0199488  | B BZZ ZZZ |
| MSN2 | 0.0202553  | B CXY ZZZ |
| Eta2 (${\eta}2$) | 0.0203098  | B CZY ZZZ |
| 2SM2 | 0.0205680  | B DVZ ZZZ |
| 2MK3 | 0.0284669  | C YZZ ZZC |
| 2MK3 | 0.0284669  | C YZZ ZZC |
| M3 | 0.0288310  | C ZZZ ZZB |
| MK3 | 0.0291951  | C AZZ ZZA |
| N4 | 0.0377193  | D XZB ZZZ |
| MN4 | 0.0380803  | D YZA ZZZ |
| M4 | 0.0384413  | D ZZZ ZZZ |
| SN4 | 0.0387540  | D AXA ZZZ |
| MS4 | 0.0391150  | D BXZ ZZZ |
| MK4 | 0.0391695  | D BZZ ZZZ |
| S4 | 0.0397887  | D DVZ ZZZ |
| R4 | 0.0398159  | D DWZ ZZZ |
| SK4 | 0.0398432  | D DXZ ZZZ |
| 2MN6 | 0.0573010  | F YZA ZZZ |
| M6 | 0.0576620  | F ZZZ ZZZ |
| MSN6 | 0.0579747  | F AXA ZZZ |
| 2MS6 | 0.0583357  | F BXZ ZZZ |
| 2MK6 | 0.0583901  | F BZZ ZZZ |
| 2SM6 | 0.0590094  | F DVZ ZZZ |
| MSK6 | 0.0590638  | F DXZ ZZZ |
| S6 | 0.0596830  | F FTZ ZZZ |
| M8 | 0.0768827  | H ZZZ ZZZ |
