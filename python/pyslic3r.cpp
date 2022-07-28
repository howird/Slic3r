#include "pyslic3r.hpp"
#include "point.hpp"
#include "config.hpp"
#include "model.hpp"
#include "print.hpp"

PYBIND11_MODULE(pyslic3r, m) {
    m.doc() = "Slic3r Python Bindings";

    pybind_point(m);
    pybind_config(m);
    pybind_model(m);
    pybind_print(m);
    
}
