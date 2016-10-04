-------------
DESCRIPTION
-------------

This package is the fully revised version of the FES2014 distribution. 
This distribution includes the FES2014 tidal prediction software managed on this git page, and the FES2014 tides database available on the AVISO website: 
http://www.aviso.altimetry.fr/en/data/products/auxiliary-products/global-tide-fes/

The FES2014 tides database includes 3 components:
1. **tide elevations**: FES2014b ocean tide model is distributed; this model has been computed using the new FES2014 tidal loading grids. 2 versions are available:
  * non extrapolated version = the model is defined on a 1/16° grid directly interpolated from the finite element native grid of the model.
  Filenames are **WAVE**_FES2014b_elev.nc.
  * extrapolated version = the above version has been extrapolated on the coasts in order to cover nearly entirely the coastal regions. 
  The procedure is a simple extrapolation applied on 10 pixels and using mean values when necessary, it can be not enough accurate in some regions. 
  Filenames are **WAVE**_FES2014b_elev_extrapol.nc.

2. **tide currents**: tidal curents derived from FES2014a model are delivered on the 1/16° grid, directly interpolated from the finite element native grid.
  No extrapolation is done on the coasts for the currents. Filenames are **WAVE**\_FES2014a\_uv.nc
  
3. **tide loading**: the new FES2014 tide loading (noted FES2014_load) grids are supplied on the same 1/16° grid. 
FES2014_load must be used to compute the FES2014 geocentric (elastic) tide.  Filenames are **WAVE**_FES2014load.nc

|ocean tide | loading tide | geocentric tide |
|:---:|:---:|:---:|
|**FES2014b**   |**FES2014_load**  |**FES2014b + FES2014_load**|


  * 34 tidal components are available for each tide product: 2N2,E2,J1,K1,K2,L2,La2,M2,M3,M4,M6,M8,Mf,MKS2,Mm,MN4,MS4,MSf,MSqm,Mtm,Mu2,N2,N4,Nu2,O1,P1,Q1,R2,S1,S2,S4,Sa,Ssa,T2
  
  * Note that the files distributed are NetCdf4 (the NetCDF libraries must be up-to-date to read those data)

  * When using FES2014, please mention:  "FES2014 was produced by NOVELTIS, LEGOS and CLS Space Oceanography Division and it is distributed by AVISO, with support from CNES (http://www.aviso.altimetry.fr/)"


-----------------------------
HOW TO INSTALL THE SOFTWARE ?
-----------------------------
First one must be aware that the software shall be installed on a unix machine.

tar -xvf fes2014.tar
cd fes2014
- autoreconf (optional)
- ./configure
- make
- make install (careful, this command will not work if you cannot write in /usr/local/lib/. But the library libfes.a is still available in src/ directory)
- make test ( You must obtain the following message "All 5 tests passed")


-------------------------
HOW TO USE THE SOFTWARE ?
-------------------------

Careful, if you would like to use the routines in MEMORY ACCESS mode
(FES_MEM instead of FES_IO option), you should use a 64 bits machine with
enough memory (tests have been made on a machine with 16 Gb RAM).

Please refer to file examples/fes_slev.c to get an example of the routine to
use.

Please note that the old FES2004 database is included in this git repository as it is 
used to test the tide prediction sofware. FES2004 database is stored in data/ directory.

To produce FES2014 tidal prediction with the software, you need to change the INI variable to the the FES2014 version in the file examples/fes_slev.c :
* FES2014 elevations (ocean or geocentric) tide) => use fes2014b_elev.ini or fes2014_elevb_extrapol.ini
* FES2014 currents => use fes2014a_u.ini and fes2014a_v.ini

The weaving of these .ini files is available here but you will need to change the directory where the files are stored on your machine.

---------------------
BIBLIOGRAPHY
---------------------

Lyard F., L. Carrere, M. Cancet, A. Guillot, N. Picot: “FES2014, a new finite elements tidal model for global ocean”, in preparation, to be submitted to Ocean Dynamics in 2016.

Carrere L., F. Lyard, M. Cancet, A. Guillot, N. Picot: "FES 2014, a new tidal model - Validation results and perspectives for improvements", presentation to ESA Living Planet Conference, Prague 2016.


---------------------
CONTACT
---------------------
aviso@oceanobs.com


