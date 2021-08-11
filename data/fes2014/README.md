# CONTENTS

## Configuration Files

This directory contains the different configuration files to compute:

* FES2014 elevations (ocean or geocentric)
  * Use [ocean_tide.ini](ocean_tide.ini) to compute the ocean tide
    with the native grids.
  * Use [ocean_tide_extrapolated.ini](ocean_tide_extrapolated.ini)
    to compute the ocean tide with the extrapolated grids.
  * Use [load_tide.ini](load_tide.ini) to compute the load tide.
* FES2014 currents
    * Use [northward_velocity.ini](northward_velocity.ini) to use the
      native grids.
    * Use [eastward_velocity.ini](eastward_velocity.ini) to use the
      native grids.

> **_NOTE_**
>
> The configuration files are supplied for use with the tree as described
> below.
>
> ```
> data/
> └── fes2014
>     ├── eastward_velocity
>     ├── load_tide
>     ├── northward_velocity
>     ├── ocean_tide
>     └── ocean_tide_extrapolated
>```

## Database Contents

The various sub directories located here are empty. Indeed, data necessary to
calculate the different tides are not distributed here, but on the
[AVISO](http://www.aviso.altimetry.fr/en/data/products/auxiliary-products/global-tide-fes/)
website. You will find on the AVISO FTP server authenticated the following
archives for filling empty directories:

- eastward_velocity.tar.xz
- load_tide.tar.xz
- northward_velocity.tar.xz
- ocean_tide.tar.xz
- ocean_tide_extrapolated.tar.xz

To allow you to download as small as possible archives, the archives have been
compressed using the [xz](http://tukaani.org/xz) program using the LZMA2
compression algorithms. To decompress the archive, you must use this command or
[7-zip](http://www.7-zip.org/) if you use Windows.

> **_NOTE_**
>
> The decompression of these archives will need 193 MiB of memory.

## Database description

The FES2014 tides database includes 3 components: Tide elevations, currents and
loading. Each component contains 34 tidal components:

    2N₂, J₁, K₁, K₂, L₂, MKS₂, MN₄, MS₄, Mf, Mm, Msf, Msqm, Mtm, M₂, M₃, M₄, M₆, M₈, N₂, N₄, O₁, P₁, P₁, R₂, Sa, Ssa, S₁, S₂, S₄, T₂, µ₂, ε₂, λ₂, ν₂

### Tide Elevations

FES2014b ocean tide model is distributed; this model has
been computed using the new FES2014 tidal loading grids. Two versions
are available:

  * Non extrapolated version contained in the `ocean_tide.tar.xz` archive: the
    model is defined on a 1/16° grid directly
    interpolated from the finite element native grid of the model.

  * Extrapolated version contained in the `ocean_tide_extrapolated.tar.xz`: the
    above version has been extrapolated on the coasts in order to cover nearly
    entirely the coastal regions. The procedure is a simple extrapolation
    applied on 10 pixels and using mean values when necessary, it cannot be
    accurate enough in some regions.

### Tide Currents

Tidal currents derived from FES2014a models are delivered on
the 1/16° grid, directly interpolated from the finite element native grid.
No extrapolation is done on the coasts of the currents. This database is
contained in two separate archives: `eastward_velocity.tar.xz` and
`northward_velocity.tar.xz`.

### Tide Loading

The new FES2014a tide loading is supplied on the same 1/16° grid. This
dataset is provided into the `load_tide.tar.xz` archive.
