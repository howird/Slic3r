#include "model.hpp"

void pybind_model(py::module& m) {
    py::class_<TriangleMesh>(m, "TriangleMesh")
            .def(py::init())
            .def("read_stl", &TriangleMesh::ReadSTLFile)
            .def("volume", &TriangleMesh::volume)
            ; 
    
    py::class_<Model>(m, "Model")
            .def(py::init())
            .def("read_from_file", &Model::read_from_file)
            .def("translate", &Model::translate)
            ;
}
