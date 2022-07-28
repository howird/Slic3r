![](var/Slic3r_128px.png) Slic3r [![Build Status](https://travis-ci.org/slic3r/Slic3r.svg?branch=master)](https://travis-ci.org/slic3r/Slic3r) [![Build status](https://ci.appveyor.com/api/projects/status/8iqmeat6cj158vo6?svg=true)](https://ci.appveyor.com/project/lordofhyphens/slic3r) [![Build Status](http://osx-build.slic3r.org:8080/buildStatus/icon?job=Slic3r)](http://osx-build.slic3r.org:8080/job/Slic3r)![Coverity Status](https://scan.coverity.com/projects/17257/badge.svg)
======

We have automated builds for Windows (64-bit) and OSX (>= 10.7). [Get a fresh build now](http://dl.slic3r.org/dev/) and stay up-to-date with the development!

The MacOS X build server is kindly sponsored by: <img width=150 src=https://cloud.githubusercontent.com/assets/31754/22719818/09998c92-ed6d-11e6-9fa0-09de638f3a36.png />

### So, what's this Slic3r?

Slic3r is mainly a **toolpath generator** for 3D printers: it reads 3D models (STL, OBJ, AMF, 3MF) and it converts them into **G-code** instructions for 3D printers. But it does much more than that, see the [features list](#features) below.

Slic3r was born in **2011** within the RepRap community and thanks to its high configurability became the swiss-army knife for 3D printing. It served as a platform for implementing several **new (experimental) ideas that later became technology standards**, such as multiple extruders, brim, variable-height layers, per-object settings, modifiers, post-processing scripts, G-code macros and more. Despite being based on volunteer efforts, Slic3r is still pushing the boundaries of 3D printing.

Slic3r is:

* **Open:** it is totally **open source** and it's **independent from any commercial company** or printer manufacturer. We want to keep 3D printing open and free.
* **Compatible:** it supports all the known G-code dialects (Marlin, Repetier, Mach3, LinuxCNC, Machinekit, Smoothie, Makerware, Sailfish).
* **Advanced:** many configuration options allow for fine-tuning and full control. While novice users often need just few options, Slic3r is mostly used by advanced users.
* **Community-driven:** new features or issues are discussed in the [GitHub repository](https://github.com/slic3r/Slic3r/issues). Join our collaborative effort and help improve it!
* **Robust:** the codebase includes more than 1,000 unit and regression tests, collected in 6 years of development.
* **Modular:** the core of Slic3r is libslic3r, a C++ library that provides a granular API and reusable components.
* **Embeddable:** a complete and powerful command line interface allows Slic3r to be used from the shell or integrated with server-side applications.
* **Powerful:** see the list below!

See the [project homepage](https://slic3r.org/) at slic3r.org for more information.

### <a name="features"></a>Features

(Most of these are also available in the command line interface.)

* **G-code generation** for FFF/FDM printers;
* **conversion** between STL, OBJ, AMF, 3MF and POV formats;
* **auto-repair** of non-manifold meshes (and ability to re-export them);
* **SVG export** of slices;
* built-in **USB/serial** host controller, supporting **multiple simultaneous printers** each one with a spool queue;
* **OctoPrint integration** (send to printer);
* built-in **projector and host for DLP printers**;
* tool for **cutting meshes** in multiple solid parts with visual preview (also in batch using a grid);
* tool for **extruding 2.5D TIN meshes**.

### What language is it written in?

The core parts of Slic3r are written in C++11, with multithreading. The graphical interface is in the process of being ported to C++14.

### How to install?

You can download a precompiled package from [slic3r.org](https://slic3r.org/) (releases) or from [dl.slicr3r.org](https://dl.slic3r.org/dev/) (automated builds).

If you want to compile the source yourself follow the instructions on one of these wiki pages:
* [Linux](https://github.com/slic3r/Slic3r/wiki/Running-Slic3r-from-git-on-GNU-Linux)
* [Windows](https://github.com/slic3r/Slic3r/wiki/Running-Slic3r-from-git-on-Windows)
* [Mac OSX](https://github.com/slic3r/Slic3r/wiki/Running-Slic3r-from-git-on-OS-X)

### Can I help?

Sure, but please read the
[CONTRIBUTING](https://github.com/slic3r/Slic3r/blob/master/.github/CONTRIBUTING.md)
document first!

### Directory structure

* `package/`: the scripts used for packaging the executables
* `src/`: the C++ source of the `slic3r` executable and the CMake definition file for compiling it
* `src/GUI`: The C++ GUI.
* `src/test`: New test suite for libslic3r and the GUI. Implemented with [Catch2](https://github.com/catchorg/Catch2)
* `t/`: the test suite (deprecated)
* `utils/`: various useful scripts
* `xs/src/libslic3r/`: C++ sources for libslic3r
* `xs/t/`: test suite for libslic3r (deprecated)
* `xs/xsp/`: bindings for calling libslic3r from Perl (XS) (deprecated)

### Acknowledgements

The main author of Slic3r is Alessandro Ranellucci (@alranel, *Sound* in IRC, [@alranel](http://twitter.com/alranel) on Twitter), who started the project in 2011.

Joseph Lenox (@lordofhyphens, *LoH* in IRC, [@LenoxPlay](http://twitter.com/LenoxPlay) on Twitter) is the current co-maintainer.

Contributions by Henrik Brix Andersen, Vojtech Bubnik, Nicolas Dandrimont, Mark Hindess, Petr Ledvina, Y. Sapir, Mike Sheldrake, Kliment Yanev and numerous others. Original manual by Gary Hodgson. Slic3r logo designed by Corey Daniels, <a href="http://www.famfamfam.com/lab/icons/silk/">Silk Icon Set</a> designed by Mark James, stl and gcode file icons designed by Akira Yasuda.

### How can I invoke Slic3r using the command line?

The command line is documented in the relevant [manual page](https://manual.slic3r.org/advanced/command-line).


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
    - 

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



# src/