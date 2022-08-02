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
        // .def(py::init<const ConfigDef*>())
        .def(py::init([](){
            Slic3r::DynamicConfig* config = new Slic3r::DynamicConfig();
            Slic3r::ConfigDef* config_def = new Slic3r::ConfigDef();

            config_def->merge(Slic3r::cli_actions_config_def);
            config_def->merge(Slic3r::cli_transform_config_def);
            config_def->merge(Slic3r::cli_misc_config_def);
            config_def->merge(Slic3r::print_config_def);

            config->def = config_def;
            return config;
        }))
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
