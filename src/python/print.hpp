#include "pyslic3r.hpp"
#include <boost/lexical_cast.hpp>

#include "../../xs/src/libslic3r/Print.hpp"
#include "../../xs/src/libslic3r/SimplePrint.hpp"
#include "../../xs/src/libslic3r/PrintGCode.hpp"

namespace py = pybind11;

void pybind_print(py::module& m);
