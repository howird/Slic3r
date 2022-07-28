#include "pyslic3r.hpp"
#include <boost/lexical_cast.hpp>

#include "../src/libslic3r/TriangleMesh.hpp"
#include "../src/libslic3r/Model.hpp"

namespace py = pybind11;

void pybind_model(py::module& m);
