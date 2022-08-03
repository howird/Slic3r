#include "pyslic3r.hpp"
#include <boost/lexical_cast.hpp>
#include <map>

#include "../src/libslic3r/ConfigBase.hpp"
#include "../src/libslic3r/Config.hpp"
#include "../src/libslic3r/PrintConfig.hpp"

namespace py = pybind11;

void pybind_config(py::module& m);
