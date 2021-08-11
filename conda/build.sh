#!/bin/bash
set -e
mkdir build
cd build
cmake .. -DBUILD_PYTHON=on -DPYTHON_EXECUTABLE=$PYTHON -DBUILD_SHARED_LIBS=on -DCMAKE_INSTALL_PREFIX=$PREFIX
make -j
make test
make install
