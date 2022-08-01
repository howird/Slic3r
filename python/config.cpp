#include "config.hpp"

void pybind_config(py::module& m) {
    py::class_<Slic3r::ConfigDef>(m, "ConfigDef")
        .def(py::init())
        .def("merge", &Slic3r::ConfigDef::merge)
        ;
    
    py::class_<Slic3r::ConfigOptionDef>(m, "ConfigOptionDef")
        .def_readwrite("type", &Slic3r::ConfigOptionDef::type)
        ;
    
    py::class_<Slic3r::DynamicConfig>(m, "DynamicConfig")
        .def(py::init())
        .def("load", [](Slic3r::DynamicConfig &dconf, const std::string &file) {
            dconf.load(file);
        })
        .def("save", [](Slic3r::DynamicConfig &dconf, const std::string &file) {
            dconf.save(file);
        })
        .def("read_cli", [](
            Slic3r::DynamicConfig &dconf,
            const std::vector<std::string> &tokens,
            Slic3r::t_config_option_keys* extra,
            Slic3r::t_config_option_keys* keys)
        {
            dconf.read_cli(tokens, extra, keys);
        })
        // .def_readonly("type", &Slic3r::ConfigOptionDef::type)
        // .def("options", [](Slic3r::DynamicConfig &dconf) {
        //     return dconf.def->options;
        // })
        // .def("options", Slic3r::DynamicConfig::def::options)
        ;

    py::class_<Slic3r::DynamicPrintConfig, Slic3r::DynamicConfig>(m, "DynamicPrintConfig")
        .def(py::init())
        .def("add_from_file", [](Slic3r::DynamicPrintConfig &dconf, const std::string &file) {
            Slic3r::DynamicPrintConfig c;
            c.load(file);
            c.normalize();
            dconf.apply(c);
        })
        ;

    py::class_<Slic3r::FullPrintConfig>(m, "FullPrintConfig")
        .def(py::init())
        ;
}
