#include "print.hpp"

void pybind_print(py::module& m) {
    py::class_<Slic3r::SimplePrint>(m, "SimplePrint")
        .def(py::init())
        .def("set_model", &Slic3r::SimplePrint::set_model)
        .def("export_gcode", &Slic3r::SimplePrint::export_gcode)
        ;
    
    py::class_<Slic3r::PrintRegion>(m, "PrintRegion")
        .def(py::init())
        ;

    py::class_<Slic3r::PrintObject>(m, "PrintObject")
        .def(py::init())
        ;
    
    py::class_<Slic3r::Print>(m, "Print")
        .def(py::init())
        ;
}