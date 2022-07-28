#include "point.hpp"

void pybind_point(py::module& m) {
    py::class_<Point>(m, "Point")
            .def(py::init())
            .def(py::init<int, int>(),  py::arg("x"), py::arg("y"))
            .def(py::init<float, float>(),  py::arg("x"), py::arg("y"))
            .def_readwrite("x", &Point::x)
            .def_readwrite("y", &Point::y)
            .def("__repr__",
            [](const Point &p) {
                std::string r("<pyslic3r.Point (");
                r += boost::lexical_cast<std::string>(p.x);
                r += ", ";
                r += boost::lexical_cast<std::string>(p.y);
                r += ")>";
                return r;
            })
            ;

    py::class_<Pointf>(m, "Pointf")
            .def(py::init())
            .def(py::init<int, int>(),  py::arg("x"), py::arg("y"))
            .def(py::init<float, float>(),  py::arg("x"), py::arg("y"))
            .def_readwrite("x", &Pointf::x)
            .def_readwrite("y", &Pointf::y)
            .def("__repr__",
            [](const Pointf &p) {
                std::string r("<pyslic3r.Pointf (");
                r += boost::lexical_cast<std::string>(p.x);
                r += ", ";
                r += boost::lexical_cast<std::string>(p.y);
                r += ")>";
                return r;
            })
            ;

    py::class_<Pointf3>(m, "Pointf3")
            .def(py::init())
            .def(py::init<int, int, int>(),  py::arg("x"), py::arg("y"), py::arg("z"))
            .def(py::init<float, float, float>(),  py::arg("x"), py::arg("y"), py::arg("z"))
            .def_readwrite("x", &Pointf3::x)
            .def_readwrite("y", &Pointf3::y)
            .def_readwrite("z", &Pointf3::z)
            .def("__repr__",
            [](const Pointf3 &p) {
                std::string r("<pyslic3r.Pointf3 (");
                r += boost::lexical_cast<std::string>(p.x);
                r += ", ";
                r += boost::lexical_cast<std::string>(p.y);
                r += ", ";
                r += boost::lexical_cast<std::string>(p.z);
                r += ")>";
                return r;
            })
            ;

    py::class_<BoundingBox>(m, "BoundingBox")
            .def(py::init())
            .def(py::init<Point, Point>())
            ;

    py::class_<BoundingBoxf>(m, "BoundingBoxf")
            .def(py::init())
            .def(py::init<Pointf, Pointf>())
            ;

    py::class_<BoundingBoxf3>(m, "BoundingBoxf3")
            .def(py::init())
            .def(py::init<Pointf3, Pointf3>())
            ;
}
