#include "pyslic3r.hpp"
#include <boost/lexical_cast.hpp>

#include "../src/libslic3r/Point.hpp"
#include "../src/libslic3r/BoundingBox.hpp"

namespace py = pybind11;

void pybind_point(py::module& m);
