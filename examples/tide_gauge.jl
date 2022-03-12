using Base
using Dates
using Printf

"""Possible type of access."""
@enum AccessType begin
    # Direct access (Grids are not loaded into memory).
    fes_io = 0
    # Memory access (Grids are loaded into memory).
    fes_mem = 1
end

"""Possible type of tide."""
@enum TideType begin
    # Ocean tide.
    fes_tide = 0
    # Radial tide.
    fes_radial = 1
end

"""The result codes that can be returned by functions in the FES C SDK."""
@enum Status begin
    # No error reporting.
    fes_success = 0
    # Not enough memory.
    fes_no_memory = 1
    # netCDF error.
    fes_netcdf_error = 2
    # IO error.
    fes_io_error = 3
    # Invalid configuration file.
    fes_ini_error = 4
    # No data available in grids for the location asked.
    fes_no_data = 5
    # Function receives an argument that has the right type but an
    # inappropriate value.
    fes_value_error = 6
end

"""
    FesHandler(tide::String, mode::String, path::String)

Handle the FES handler.

# Arguments
* tide::String: Specifies the tide computation mode : ``tide`` to compute the
  short tide or ``radial`` to compute the radial tide.
* mode::String: Drives the memory management of the library : ``io`` no data are
  loaded into memory (data for the calculation will be read from the netCDF
  grids if necessary) or ``memory`` to load all NetCDF grids into memory.
* path::String: The path to the configuration file to use.
"""
mutable struct FesHandler
    handler::Ptr{Cvoid}
    function FesHandler(tide::TideType,
        mode::AccessType,
        path::String)
        tide_type = Int32(-1)
        access_mode = Int32(-1)

        # Create the handler
        handler = C_NULL
        ref = Ref(handler)

        status = ccall(
            (:fes_new, :libfes),
            Int32,
            (Ptr{Ptr{Cvoid}}, Int32, Int32, Cstring),
            Base.cconvert(Ptr{Ptr{Cvoid}}, ref), Int32(tide), Int32(mode), path
        )
        handler = ref[]

        # Check for errors
        if handler == C_NULL
            throw(OutOfMemoryError())
        end
        self = new(handler)
        f(self) = ccall((:fes_delete, :libfes), Cvoid, (Ptr{Cvoid},), self.handler)
        finalizer(f, self)
        if status == 1
            throw(error(self))
        end
        return self
    end
end

"""
    error(fes::FesHandler)

Get the error message from the handler.
"""
function error(fes::FesHandler)
    status = ccall(
        (:fes_error, :libfes),
        Cstring,
        (Ptr{Cvoid},),
        fes.handler
    )
    return unsafe_string(status)
end

"""
    errno(fes::FesHandler)

Return the errno from the handler.
"""
function errno(fes::FesHandler)
    return Status(
        ccall(
            (:fes_errno, :libfes),
            Int32,
            (Ptr{Cvoid},),
            fes.handler))
end

"""
    min_number(fes::FesHandler)

Return the minimum number of points used to interpolate the tidal waves for the
asked position.
"""
function min_number(fes::FesHandler)
    return ccall(
        (:fes_min_number, :libfes),
        Int32,
        (Ptr{Cvoid},),
        fes.handler
    )
end

"""
    set_buffer_size(fes::FesHandler, size::UInt64)

Sets the size of the buffer
"""
function set_buffer_size(fes::FesHandler, size::UInt64)
    status = ccall(
        (:fes_set_buffer_size, :libfes),
        Int32,
        (Ptr{Cvoid}, UInt64),
        fes.handler, size
    )
    if status == 1
        throw(error(fes))
    end
end


"""
    cnes_julian_day(date)

Return the number of days since the first January 1950 at 00:00:00.
"""
function cnes_julian_day(date::DateTime)
    return (datetime2unix(date) / 86400.0) + 7305.0
end


"""
    calculate(fes::FesHandler, lat::Float64, lon::Float64, date::Date)

Calculate the tide for the given position and date.

# Arguments
* lat::Float64: Latitude in degrees (positive north) for the position to
  calculate the tide for.
* lon::Float64: Longitude in degrees (positive east) for the position to
  calculate the tide for.
* date::Date: Date for the position to calculate the tide for.

# Returns
* Computed height of the diurnal and semi-diurnal constituents of the tidal
  spectrum (in centimeters).
* Computed height of the long period wave constituents of the tidal spectrum
  (in centimeters).
* The number of points used to interpolate the tidal waves for the asked
  positions.
"""
function calculate(fes::FesHandler, lat::Float64, lon::Float64, date::DateTime)
    h = Ref{Float64}(0.0)
    h_long_period = Ref{Float64}(0.0)

    status = ccall(
        (:fes_core, :libfes),
        Int32,
        (Ptr{Cvoid}, Float64, Float64, Float64, Ref{Float64}, Ref{Float64}),
        fes.handler, lat, lon, cnes_julian_day(date), h, h_long_period
    )
    if status == 1
        if errno(fes) == fes_no_data::Status
            return NaN, NaN, 0
        end
        throw(error(fes))
    end
    return h[], h_long_period[], min_number(fes)
end

"""
    gauge(short_tide::FesHandler, radial_tide::FesHandler, lat::Float64, lon::Float64, date::Date)

Calculate the tide for the given position and date.
"""
function gauge(
    short_tide::FesHandler, radial_tide::FesHandler, first_date::DateTime,
    last_date::DateTime, lat::Float64, lon::Float64)

    @printf("%-19s %9s %9s %9s %9s %9s %9s %9s\n",
        "Date", "Latitude", "Longitude", "Short_tid", "LP_tid", "Pure_Tide",
        "Geo_Tide", "Rad_Tide")
    while first_date < last_date
        tide, lp, _ = calculate(short_tide, lat, lon, first_date)
        load = calculate(radial_tide, lat, lon, first_date)[1]

        @printf("%-19s %9.4f %9.4f %9.4f %9.4f %9.4f %9.4f %9.4f\n",
            first_date, lat, lon, tide, lp, tide + lp, tide + lp + load, load)

        first_date += Dates.Hour(1)
    end
end

short_tide = FesHandler(fes_tide::TideType, fes_io::AccessType, "../test/fes.ini")
radial_tide = FesHandler(fes_radial::TideType, fes_io::AccessType, "../test/fes.ini")
gauge(short_tide, radial_tide, DateTime(1983, 1, 1), DateTime(1983, 1, 2), 59.195, -7.688)
