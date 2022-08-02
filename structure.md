# Code Sturcture
# src/slic3r.cpp
- entrypoint of cpp command line program, entire process is run within `CLI::run()`
## `class CLI`
- __attributes__: 
    - `config_def: ConfigDef`
    - `config: DynamicConfig`
    - `print_config: DynamicPrintConfig`
    - `full_print_config: FullPrintConfig`
    - `input_files, actions, transforms: t_config_option_keys (vector<string>)`
    - `models: vector<Model>`
    - `last_outfile: string`
- `CLI::run()`:
    - reads all config files
    - loads all models from input files and puts them in models vector
    - applies all specified transforms to models
    - pseudocode:
        - for each model in the models vector:
            - create obj: SimplePrint `print`
            - sets `print.model` to model and applies `"dont_arrange"`, `"center"` and `"align_xy"` from `config`
            - runs `print.export_gcode()`

# src/ConfigBase.cpp
Houses `ConfigBase` class and all of its base Classes

### `ConfigOption` class
- this class is a base class with entirely virtual functions which define getters and setters for each data type
- child classes will only implement one data type getter/setter thus the others will return this classes' implementation which all throw exceptions
### `ConfigOptionSingle<T>` class inheriting from `ConfigOption`
- base class  which allows you to store and access a single option value
### `ConfigOptionVectorBase` class inheriting from `ConfigOption`
- entirely virual base class just defines a purely virtual function to serialize a vector of values
### `ConfigOptionVector<T>` class inheriting from `ConfigOptionVectorBase`
- base class which defines a setter for its values and a get_at(index) function
### `ConfigOptionTYPE` and `ConfigOptionTYPEs`
- child classes of `ConfigOptionSingle<TYPE>` and `ConfigOptionVector<T>` respecitvely
- define constructors for each type, TYPE
- define `serialize` and `deserialize` functions to generate and parse strings, respectively
    - `ConfigOptionTYPEs` will also have the `vdeserialize` function which instead of returning a single string for all values, returns a vector of strings for each value
### `ConfigOptionEnum<Enum>` class
- child class of `ConfigOptionSingle<T>`
- used in `DynamicConfig` objects when creating a config value object for ConfigOptionType == coEnum
- In the StaticConfig, it is better to use the specialized `ConfigOptionEnum<T>` containers
- stores a `std::map<str, int>`
- `serialize` returns the str value represented by the enum and `deserialize` the inverse

### `ConfigOptionType` enum
- enum for all the types `ConfigOptionDef` may have

### `ConfigOptionDef` class
- Definition of a configuration value for the purpose of GUI presentation, editing, value mapping and config file handling.
- has a `ConfigOptionType` and `default_value: ConfigOption`

### `ConfigDef` class
- configuration values for the purpose of GUI presentation, editing, value mapping and config file handling
- does not carry the actual configuration values, but it carries the defaults of the configuration values
- contains a `std::map<str, ConfigOptionDef>`
- has functions to add to that map and check values

### `ConfigBase` class
- an abstract configuation store
- main attribute is a `ConfigDef` map

### `DynamicConfig` class inherits from `ConfigBase`
- Configuration store with dynamic number of configuration values.
- has attributes: `def: *ConfigDef` from `ConfigBase` as well as `options: std::map<str, *ConfigOption>`

### `StaticConfig` class inherits from `ConfigBase`

- has attribute: `def: *ConfigDef` from `ConfigBase`, no others
- has method to set all values to their defaults

# src/Config.cpp
### `Config` class
- essentially a wrapper class for `DynamicPrintConfig`
- has attribute: `_config: DynamicPrintConfig`

# src/PrintConfig.cpp
### `ConfigOptionEnum<ENUM>` for the below enums
- `GCodeFlavor`, `HostType`, `InfillPattern`, `SupportMaterialPattern`, `SeamPosition`
- enums for various options
### `PrintConfigBase` inherits from `ConfigBase`
- Slic3r configuration storage with print_config_def assigned
### `DynamicPrintConfig` inherits from `PrintConfigBase` and `DynamicConfig`
- used to override the configuration per object, per modification volume or per printing material.
- used to store user modifications of the print global parameters, so the modified configuration values may be diffed against the active configuration to invalidate the proper slicing resp. g-code generation processing steps

### `StaticPrintConfig` inherits from `PrintConfigBase` and `PrintConfig`
- base class for `PrintObjectConfig` `GCodeConfig` to inherit from

### `PrintObjectConfig` `GCodeConfig` classes inherit from `StaticPrintConfig` 
- have attributes for each of the options and sets them to their defaults at construction

### `PrintConfig` inherits from `GCodeConfig`
- has attributes for options

# src/SimplePrint
#### `SimplePrint::set_model(const Model &model)`
- sets SimplePrint::_model to given model
- translates it to 0
#### `SimplePrint::export_gcode()`
- runs `Print::validate` then `Print::export_gcode(str)`
- check that all parts fit in bed shape and warn if they don't

# src/Print
### `Print::export_gcode(str)`
- function run by simple print
- calls `Print::export_gcode(ostream)` internally
- then runs post processing

### `Print::export_gcode(ostream)`
- runs `Print::process()`