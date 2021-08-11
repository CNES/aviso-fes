# Installation Instructions

## Requirements

* [cmake](https://cmake.org/): CMake is an open-source, cross-platform family of
  tools designed to build, test and package software. CMake is used to control
  the software compilation process using simple platform and compiler
  independent configuration files, and generate native makefiles and workspaces
  that can be used in the compiler environment of your choice.
* [netcdf](http://www.unidata.ucar.edu/software/netcdf/): A set of software
  libraries and self-describing, machine- independent data formats that support
  the creation, access, and sharing of array-oriented scientific data.

## Setup with conda

[Anaconda](https://anaconda.org/) is a free and open-source distribution of the
Python programming language for scientific computing, that aims to simplify
package management and deployment. The package management system `conda` manages
package versions.

The first step is to install the anaconda distribution. The installation manual
for this software is detailed
[here](https://docs.anaconda.com/anaconda/install/)

To install the software using `conda`, execute the following command:

```
conda install pyfes -c fbriol
```

This command will install the software and the necessary dependencies. More
information is available on the syntax of this command on the [related
documentation](https://conda.io/projects/conda/en/latest/commands/install.html)

## Building & Installing

One nice and highly recommended feature of CMake is the ability to do out of
source builds. In this way you can make all your `.o` files, various temporary
depend files, and even the binary executables without cluttering up your source
tree. To use out of source builds, first create a build directory, then change
into your build directory and run cmake pointing it to the source directory:

```
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/opt/local
```

The command gives feedback on what requirements are fulfilled and what
software is still required.

The library can be built and installed using these commands:

```
make -j
make install # sudo may be needed
```

> To see the full output from the compilation, you can use: `make VERBOSE=1`

If you have the dependencies installed in non-standard location (other than
`/usr`, e.g. `/home/xyz/projects`), set `CMAKE_PREFIX_PATH` to that directory to
help CMake find them. You can enter more different dirs if you separate them
with semicolons. You can also specify, for each dependency, the required library
and header search directories using options.

Also, if you plan to install the library to non-standard location, you might
want to set `CMAKE_INSTALL_PREFIX` to subdir of given prefix (e.g.
`/home/xyz/projects/fes`).

The following table gives an overview of the different options. The default
(without any options) will compile all binaries only and install it in
`/usr/local/`.

<table>
<caption>Construction options</caption>
<tr>
  <th>CMake options</th>
  <th>Documentation</th>
  <th>Default</th>
</tr>
<tr>
  <td>BUILD_DOC</td>
  <td>Build Doxygen documentation</td>
  <td>OFF</td>
</tr>
<tr>
  <td>BUILD_PYTHON</td>
  <td>Build python extension</td>
  <td>OFF</td>
</tr>
<tr>
  <td>BUILD_SHARED_LIBS</td>
  <td>Build shared libraries</td>
  <td>OFF</td>
</tr>
<tr>
  <td>CMAKE_BUILD_TYPE</td>
  <td>Type of compilation: Debug, RelWithDebInfo, Release</td>
  <td>RelWithDebInfo</td>
</tr>
<tr>
  <td>CMAKE_CXX_COMPILER</td>
  <td>C++ Compiler
  <td></td>
</tr>
<tr>
  <td>CMAKE_CXX_FLAGS</td>
  <td>Flags for the C++ Compiler
  <td></td>
</tr>
<tr>
  <td>CMAKE_INSTALL_PREFIX</td>
  <td>where you want to install</td>
  <td>/usr/local</td>
</tr>
<tr>
  <td>UTHASH_INCLUDE_DIR</td>
  <td>Path to the include directory of <code>uthash</code>
  <td></td>
</tr>
<tr>
  <td>NETCDF_INCLUDE_DIR</td>
  <td>Path to the include directory of NetCDF</td>
  <td></td>
</tr>
<tr>
  <td>PYTHON_EXECUTABLE</td>
  <td>Path to the Python interpreter</td>
  <td></td>
</tr>
</table>

The libraries are build as static by default. If you just want to build shared
libraries, enable `BUILD_SHARED_LIBS` to build the libraries as shared. If you
want to build with another compiler (e.g. Clang), pass
`-DCMAKE_CXX_COMPILER=clang++` to CMake.

Libraries built in Debug configuration (e.g. with `CMAKE_BUILD_TYPE` set to
`Debug`) have a `-d` suffix to make it possible to have both debug and release
libraries installed alongside each other. Headers are the same for both debug
and release configurations.

If you want to generate the Python library, enable `BUILD_PYTHON`. You can also
specify which python interpreter you want to use for generation by passing
`-DPYTHON_EXECUTABLE=/which/python/to/use` option to CMake.

## Running tests

In the building directory execute the `make test` command.

## Create package

### Sources

> When you create a distribution containing the source code, please extract
> the GIT tag to identify the distributed version:
> `git checkout <TAG> -b <tag>`

The "package_source" target of the makefile allows to generate a source
installer. To create this archive, in the build directory of the software
execute the following command :

```
make package_source
```

### Binaries and headers

The "package" target of the makefile allows to generate an installer of libraries and C headers in :
* `.deb` format if you generate the library under "Debian/Ubuntu" distribution,
* `.rpm` if the generation is done under "Red Hat/CentOS" or
* `.zip` if the generation is done under "Windows" or
* `.tar.gz` under the other platforms.

To create this installer, in the construction directory of the software execute the following command :

```
make package
```
