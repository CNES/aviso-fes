#include <chrono>
#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>
#include <list>
#include <netcdf>
#include <numbers>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "fes/tidal_model/cartesian.hpp"
#include "fes/tidal_model/lgp.hpp"
#include "fes/tide.hpp"

// Define the root directory for load tide files
#define LOAD_TIDE_ROOT_DIR "path/to/fes2022_loadtide/"

// Macro to construct full file paths
#define LOAD_TIDE(FILENAME) {LOAD_TIDE_ROOT_DIR FILENAME}

// Define the ocean tide file path (LPG2)
#define OCEAN_TIDE "path/to/fes2022_oceantide/fes2022b_lgp2.nc"

/// @brief Load tide paths for all constituents
/// @return A map of constituent identifiers to their file paths
static std::unordered_map<fes::Constituent, std::string> load_tide_paths() {
  return {{fes::Constituent::k2N2, LOAD_TIDE("2n2.nc")},
          {fes::Constituent::kEps2, LOAD_TIDE("eps2.nc")},
          {fes::Constituent::kJ1, LOAD_TIDE("j1.nc")},
          {fes::Constituent::kJ1, LOAD_TIDE("k1.nc")},
          {fes::Constituent::kK2, LOAD_TIDE("k2.nc")},
          {fes::Constituent::kL2, LOAD_TIDE("l2.nc")},
          {fes::Constituent::kLambda2, LOAD_TIDE("lambda2.nc")},
          {fes::Constituent::kM2, LOAD_TIDE("m2.nc")},
          {fes::Constituent::kM3, LOAD_TIDE("m3.nc")},
          {fes::Constituent::kM4, LOAD_TIDE("m4.nc")},
          {fes::Constituent::kM6, LOAD_TIDE("m6.nc")},
          {fes::Constituent::kM8, LOAD_TIDE("m8.nc")},
          {fes::Constituent::kMf, LOAD_TIDE("mf.nc")},
          {fes::Constituent::kMKS2, LOAD_TIDE("mks2.nc")},
          {fes::Constituent::kMm, LOAD_TIDE("mm.nc")},
          {fes::Constituent::kMN4, LOAD_TIDE("mn4.nc")},
          {fes::Constituent::kMS4, LOAD_TIDE("ms4.nc")},
          {fes::Constituent::kMSf, LOAD_TIDE("msf.nc")},
          {fes::Constituent::kMSqm, LOAD_TIDE("msqm.nc")},
          {fes::Constituent::kMtm, LOAD_TIDE("mtm.nc")},
          {fes::Constituent::kMu2, LOAD_TIDE("mu2.nc")},
          {fes::Constituent::kN2, LOAD_TIDE("n2.nc")},
          {fes::Constituent::kN4, LOAD_TIDE("n4.nc")},
          {fes::Constituent::kNu2, LOAD_TIDE("nu2.nc")},
          {fes::Constituent::kO1, LOAD_TIDE("o1.nc")},
          {fes::Constituent::kP1, LOAD_TIDE("p1.nc")},
          {fes::Constituent::kQ1, LOAD_TIDE("q1.nc")},
          {fes::Constituent::kR2, LOAD_TIDE("r2.nc")},
          {fes::Constituent::kS1, LOAD_TIDE("s1.nc")},
          {fes::Constituent::kS2, LOAD_TIDE("s2.nc")},
          {fes::Constituent::kS4, LOAD_TIDE("s4.nc")},
          {fes::Constituent::kSa, LOAD_TIDE("sa.nc")},
          {fes::Constituent::kSsa, LOAD_TIDE("ssa.nc")},
          {fes::Constituent::kT2, LOAD_TIDE("t2.nc")}};
}

/// @brief Get the ocean tide constituents to load
/// @return A list of constituent identifiers
static auto ocean_tide_constituents() -> std::list<fes::Constituent> {
  return {fes::k2N2,     fes::kEps2, fes::kJ1,  fes::kK1,  fes::kK2, fes::kL2,
          fes::kLambda2, fes::kM2,   fes::kM3,  fes::kM4,  fes::kM6, fes::kM8,
          fes::kMKS2,    fes::kMN4,  fes::kMS4, fes::kMSf, fes::kMf, fes::kMm,
          fes::kMSqm,    fes::kMtm,  fes::kMu2, fes::kN2,  fes::kN4, fes::kNu2,
          fes::kO1,      fes::kP1,   fes::kQ1,  fes::kR2,  fes::kS1, fes::kS2,
          fes::kS4,      fes::kSa,   fes::kSsa, fes::kT2};
}

/// @brief Convert degrees to radians
/// @tparam T Data type
/// @param degrees Value in degrees
/// @return Value in radians
template <typename T>
constexpr auto to_radians(const T& degrees) -> T {
  return degrees * std::numbers::pi / 180.0;
}

/// @brief Create a complex wave from amplitude and phase vectors
/// @param amp Amplitude vector
/// @param pha Phase vector in degrees
/// @return A complex vector representing the wave
auto polar(const std::vector<float>& amp, const std::vector<float>& pha)
    -> Eigen::VectorXcf {
  auto size = amp.size();
  if (size != pha.size()) {
    throw std::invalid_argument(
        "Amplitude and phase vectors must be of the same size.");
  }
  // Combine amplitude and phase into a complex wave
  Eigen::Map<const Eigen::VectorXf> amp_map(amp.data(), size);
  Eigen::Map<const Eigen::VectorXf> phase_map(pha.data(), size);

  return amp_map.binaryExpr(
      phase_map, [](float a, float p) { return std::polar(a, to_radians(p)); });
}

/// @brief Load tide data from a NetCDF file
/// @param ident The constituent identifier
/// @param filename The path to the NetCDF file
/// @param model The tidal model to update
static auto load_tide_data_from_file(
    const fes::Constituent ident, const std::string& filename,
    std::unique_ptr<fes::tidal_model::Cartesian<float>>& model) -> void {
  try {
    // Open the NetCDF file for reading
    netCDF::NcFile ds(filename, netCDF::NcFile::read);

    // Get the dimensions
    auto lon_dim = ds.getDim("lon").getSize();
    auto lat_dim = ds.getDim("lat").getSize();

    // Get the variables
    netCDF::NcVar lon_var = ds.getVar("lon");
    netCDF::NcVar lat_var = ds.getVar("lat");
    netCDF::NcVar amp_var = ds.getVar("amplitude");
    netCDF::NcVar phase_var = ds.getVar("phase");

    // Ensure variables are valid
    assert(!lon_var.isNull());
    assert(!lat_var.isNull());
    assert(!amp_var.isNull());
    assert(!phase_var.isNull());

    // Read the data
    std::vector<float> amp(lon_dim * lat_dim);
    std::vector<float> phase(lon_dim * lat_dim);

    amp_var.getVar(amp.data());
    phase_var.getVar(phase.data());

    // Ensure amplitude and phase data are in column-major order to match
    // the cartesian model's expected layout.
    assert(amp_var.getDim(0).getSize() == lat_dim);
    assert(amp_var.getDim(1).getSize() == lon_dim);
    assert(phase_var.getDim(0).getSize() == lat_dim);
    assert(phase_var.getDim(1).getSize() == lon_dim);

    // Initialize the model if it hasn't been already
    if (model == nullptr) {
      std::vector<double> lon(lon_dim);
      std::vector<double> lat(lat_dim);

      // Assumption: longitude and latitude grids are consistent across all
      // tidal constituents. Production code should validate this invariant.
      lon_var.getVar(lon.data());
      lat_var.getVar(lat.data());

      auto x_axis =
          fes::Axis(Eigen::Map<const Eigen::VectorXd>(lon.data(), lon.size()),
                    1e-6, true);
      auto y_axis =
          fes::Axis(Eigen::Map<const Eigen::VectorXd>(lat.data(), lat.size()),
                    1e-6, false);
      model = std::move(std::make_unique<fes::tidal_model::Cartesian<float>>(
          std::move(x_axis), std::move(y_axis), fes::TideType::kRadial, false));
    }

    // Combine amplitude and phase into a complex wave
    auto wave = polar(amp, phase);

    // Finally, add the constituent to the model
    model->add_constituent(ident, std::move(wave));
  } catch (netCDF::exceptions::NcException& e) {
    std::cerr << "Error opening file: " << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}

/// @brief Load the tide model
/// @return A unique pointer to the tide model
static auto load_load_tide_model()
    -> std::unique_ptr<fes::tidal_model::Cartesian<float>> {
  auto tide_paths = load_tide_paths();
  std::unique_ptr<fes::tidal_model::Cartesian<float>> model = nullptr;

  for (const auto& [ident, path] : tide_paths) {
    load_tide_data_from_file(ident, path, model);
  }
  return model;
}

/// @brief Load the ocean tide model from an LPG2 NetCDF file
/// @return A unique pointer to the ocean tide model
static auto load_ocean_tide_model()
    -> std::unique_ptr<fes::tidal_model::LGP2<float>> {
  try {
    auto lpg2_model = std::make_unique<fes::tidal_model::LGP2<float>>();
    // Open the NetCDF file for reading
    netCDF::NcFile ds(OCEAN_TIDE, netCDF::NcFile::read);

    // Get the dimensions
    auto coordinates = ds.getDim("coordinates").getSize();
    auto triangles = ds.getDim("triangles").getSize();
    auto lgp2_nodes = ds.getDim("lgp2_nodes").getSize();

    // Read the coordinates
    std::vector<double> lon(coordinates);
    std::vector<double> lat(coordinates);
    ds.getVar("lon").getVar(lon.data());
    ds.getVar("lat").getVar(lat.data());

    // Read the triangles
    std::vector<int32_t> triangles_data(triangles * 3);
    ds.getVar("triangle").getVar(triangles_data.data());

    // Read the LGP2 codes
    std::vector<int32_t> lgp2_codes(triangles * 6);
    ds.getVar("lgp2").getVar(lgp2_codes.data());

    // Create the mesh index
    auto index = std::make_shared<fes::mesh::Index>(
        Eigen::Map<const Eigen::VectorXd>(lon.data(), lon.size()),
        Eigen::Map<const Eigen::VectorXd>(lat.data(), lat.size()),
        Eigen::Map<const Eigen::Matrix<int32_t, -1, 3, Eigen::RowMajor>>(
            triangles_data.data(), triangles, 3));

    // Initialize the LGP2 model with the mesh index and LGP2 codes
    lpg2_model = std::make_unique<fes::tidal_model::LGP2<float>>(
        std::move(index),
        Eigen::Map<
            const Eigen::Matrix<int32_t, Eigen::Dynamic, 6, Eigen::RowMajor>>(
            lgp2_codes.data(), triangles, 6),
        fes::TideType::kTide, 100'000);

    // Read and add each constituent to the model
    for (const auto& ident : ocean_tide_constituents()) {
      std::string prefix = fes::constituents::name(ident);
      std::string var_name = prefix + "_amplitude";

      // Get the amplitude variable
      auto amp = ds.getVar(var_name);
      if (amp.isNull()) {
        throw std::runtime_error("Variable " + var_name +
                                 " not found in the dataset.");
      }
      // Read the amplitude data
      std::vector<float> amplitude(lgp2_nodes);
      amp.getVar(amplitude.data());

      // Get the phase variable
      var_name = prefix + "_phase";
      auto phase = ds.getVar(var_name);
      if (phase.isNull()) {
        throw std::runtime_error("Variable " + var_name +
                                 " not found in the dataset.");
      }
      // Read the phase data
      std::vector<float> phase_data(lgp2_nodes);
      phase.getVar(phase_data.data());
      // Combine amplitude and phase into a complex wave
      auto wave = polar(amplitude, phase_data);
      // Add the constituent to the model
      lpg2_model->add_constituent(ident, std::move(wave));
    }
    return lpg2_model;

  } catch (netCDF::exceptions::NcException& e) {
    std::cerr << "Error opening file: " << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}

int main() {
  // Load the load tide model
  auto radial_handler = load_load_tide_model();
  // Load the ocean tide model
  auto tide_handler = load_ocean_tide_model();
  // Create the FES settings
  auto settings = fes::Settings{fes::angle::Formulae::kIERS, 0.0};
  // Create a timeseries of dates starting from the first january 1983
  Eigen::VectorXd times(24);
  auto start_date = std::chrono::year{1983} / std::chrono::January / 1;
  auto start = std::chrono::sys_days{start_date};
  for (int64_t i = 0; i < times.size(); ++i) {
    auto current = start + std::chrono::days(i);
    times[i] =
        std::chrono::duration<double>(current.time_since_epoch()).count();
  }

  // Set the location (longitude, latitude) in degrees
  auto lon = Eigen::VectorXd::Constant(times.size(), -7.688);
  auto lat = Eigen::VectorXd::Constant(times.size(), 59.195);

  // Evaluate the radial load tide (long period and interpolation quality are
  // ignored)
  Eigen::VectorXd load = std::get<0>(
      fes::evaluate_tide(radial_handler.get(), times, lon, lat, settings));

  // Evaluate the ocean tide (interpolation quality is ignored here, but in
  // production you should check this to differentiate between interpolated and
  // extrapolated points)
  Eigen::VectorXd tide;
  Eigen::VectorXd lp;
  std::tie(tide, lp, std::ignore) =
      fes::evaluate_tide(tide_handler.get(), times, lon, lat, settings);

  // Print header with better formatting
  std::cout << "\n" << std::string(110, '=') << std::endl;
  std::cout << "                           FES2022 TIDE PREDICTION RESULTS"
            << std::endl;
  std::cout << std::string(110, '=') << std::endl;
  std::cout << std::left << std::setw(12) << "Date" << std::setw(6) << "Hour"
            << std::right << std::setw(12) << "Latitude" << std::setw(12)
            << "Longitude" << std::setw(12) << "Short_tide" << std::setw(12)
            << "LP_tide" << std::setw(12) << "Pure_Tide" << std::setw(12)
            << "Geo_Tide" << std::setw(12) << "Rad_Tide" << std::endl;
  std::cout << std::string(110, '-') << std::endl;

  // Print results for each time point
  for (int64_t i = 0; i < times.size(); ++i) {
    // Convert Unix timestamp back to date
    auto time_point =
        std::chrono::system_clock::from_time_t(static_cast<time_t>(times[i]));
    auto date = std::chrono::floor<std::chrono::days>(time_point);
    auto time_of_day = time_point - date;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(time_of_day);

    // Get year/month/day
    auto ymd = std::chrono::year_month_day{date};

    // Format date as YYYY-MM-DD
    std::cout << std::left << std::setw(4) << static_cast<int>(ymd.year())
              << "-" << std::setw(2) << std::setfill('0')
              << static_cast<unsigned>(ymd.month()) << "-" << std::setw(2)
              << std::setfill('0') << static_cast<unsigned>(ymd.day()) << " "
              << std::setfill(' ') << std::setw(5) << hours.count() << "h";

    // Format the tide data with proper alignment and precision
    std::cout << std::right << std::fixed << std::setprecision(3)
              << std::setw(12) << lat(i) << std::setw(12) << lon(i)
              << std::setw(12) << tide(i) << std::setw(12) << lp(i)
              << std::setw(12) << (tide(i) + lp(i)) << std::setw(12)
              << (tide(i) + lp(i) + load(i)) << std::setw(12) << load(i)
              << std::endl;
  }
  std::cout << std::string(110, '=') << std::endl;
  return 0;
}
