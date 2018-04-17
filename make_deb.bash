#!/bin/bash
./configure
make -j 8
LIB=$(ls src/.libs/libfes.so.?.?.?)
[[ $LIB =~ ([0-9]+\.[0-9]+\.[0-9]+) ]]
VERSION=${BASH_REMATCH[1]}
make install  DESTDIR=/tmp/installdir
fpm -s dir -t deb -n libfes -v $VERSION -d libnetcdf11 --license LGPL-3.0 --vendor LEGOS/CNES/CLS/NOVELTIS --category science --deb-priority optional --maintainer aviso@altimetry.fr  --url https://bitbucket.org/cnes_aviso/fes  --description "Finite Element Solution 2014 - Global Tide\nFES2014 is the last version of the FES (Finite Element Solution) tide model\ndeveloped in 2014-2016. It is an improved version of the FES2012 model. This\nnew FES2014 model has been developed, implemented and validated by the LEGOS,\nNOVELTIS and CLS, within a CNES funded project." -C /tmp/installdir usr
