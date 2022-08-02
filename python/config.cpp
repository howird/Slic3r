#include "config.hpp"

class PyConfigBase : public Slic3r::ConfigBase {
public:
    using Slic3r::ConfigBase::ConfigBase;

    ConfigOption* optptr(const t_config_option_key &opt_key, bool create = false) override {
        PYBIND11_OVERRIDE_PURE(Slic3r::ConfigOption*, Slic3r::ConfigBase, optptr, opt_key, create);
    }
    std::vector<std::string> keys() const override {
        PYBIND11_OVERRIDE_PURE(std::vector<std::string>, Slic3r::ConfigBase, keys);
    }
};

class PyDynamicConfig : public Slic3r::DynamicConfig {
public:
    using Slic3r::DynamicConfig::DynamicConfig;

    ConfigOption* optptr(const t_config_option_key &opt_key, bool create = false) override {
        PYBIND11_OVERRIDE(Slic3r::ConfigOption*, Slic3r::DynamicConfig, optptr, opt_key, create);
    }
    std::vector<std::string> keys() const override {
        PYBIND11_OVERRIDE(std::vector<std::string>, Slic3r::DynamicConfig, keys);
    }
};

class PyDynamicPrintConfig : public Slic3r::DynamicPrintConfig {
public:
    using Slic3r::DynamicPrintConfig::DynamicPrintConfig;

    ConfigOption* optptr(const t_config_option_key &opt_key, bool create = false) override {
        PYBIND11_OVERRIDE(Slic3r::ConfigOption*, Slic3r::DynamicPrintConfig, optptr, opt_key, create);
    }
    std::vector<std::string> keys() const override {
        PYBIND11_OVERRIDE(std::vector<std::string>, Slic3r::DynamicPrintConfig, keys);
    }
};

void pybind_config(py::module& m) {
    py::class_<Slic3r::ConfigDef>(m, "ConfigDef")
        .def(py::init())
        .def("merge", &Slic3r::ConfigDef::merge)
        ;
    
    py::class_<Slic3r::ConfigOptionDef>(m, "ConfigOptionDef")
        .def_readwrite("type", &Slic3r::ConfigOptionDef::type)
        ;
    
    py::class_<Slic3r::ConfigBase, PyConfigBase>(m, "ConfigBase")
        // .def("load", &Slic3r::ConfigBase::load)
        // .def("save", &Slic3r::ConfigBase::save)
        ;
    
    py::class_<Slic3r::DynamicConfig, Slic3r::ConfigBase, PyDynamicConfig>(m, "DynamicConfig")
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
        .def("read_cli", [](
            Slic3r::DynamicConfig &dconf,
            const std::vector<std::string> &tokens,
            Slic3r::t_config_option_keys* extra,
            Slic3r::t_config_option_keys* keys)
        {
            dconf.read_cli(tokens, extra, keys);
        })
        .def("load", [](Slic3r::DynamicConfig &conf, const std::string &file) {
            conf.load(file);
        })
        .def("save", [](Slic3r::DynamicConfig &conf, const std::string &file) {
            conf.save(file);
        })
        ;

    py::class_<Slic3r::DynamicPrintConfig, Slic3r::DynamicConfig, PyDynamicPrintConfig>(m, "DynamicPrintConfig")
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
