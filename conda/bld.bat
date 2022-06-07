mkdir build
cd build
cmake .. -DBUILD_PYTHON=on -G "Visual Studio 15 2017" -DPython3_EXECUTABLE=%PYTHON% -DCMAKE_INSTALL_PREFIX=%PREFIX% -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 exit 1

cmake --build . --config Release -- /m
if errorlevel 1 exit 1

ctest -C Release
if errorlevel 1 exit 1

cmake --build . --config Release --target install
if errorlevel 1 exit 1
