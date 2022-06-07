// This file is part of FES library.
//
// FES is free software: you can redistribute it and/or modify
// it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// FES is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU LESSER GENERAL PUBLIC LICENSE for more details.
//
// You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE
// along with FES.  If not, see <http://www.gnu.org/licenses/>.
#include <pybind11/pybind11.h>

#include "fes_handler.hpp"

namespace py = pybind11;

PYBIND11_MODULE(pyfes, m)
{
  py::class_<Handler>(m, "Handler")
    .def(py::init<const std::string&, const std::string&, const std::string&>())
    .def_static("dump_template", &Handler::dump_template, py::arg("path"))
    .def("set_buffer_size", &Handler::set_buffer_size, py::arg("size"))
    .def("calculate",
         [](Handler& self,
            py::array_t<double>& lon,
            py::array_t<double>& lat,
            py::array& date) -> py::tuple {
           return self.calculate(lon, lat, date);
         });

  m.attr("__version__") = "2.9.4.dev13";
}
