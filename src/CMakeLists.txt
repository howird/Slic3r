cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project (pyslic3r)

option(Enable_GUI "Use the wxWidgets code in slic3r.cpp" OFF)
option(GUI_BUILD_TESTS "Build tests for Slic3r GUI." OFF)
option(SLIC3R_BUILD_TESTS "Build tests for libslic3r." OFF)
option(SLIC3R_STATIC "Build and link Slic3r statically." OFF)
option(BUILD_EXTRUDE_TIN "Build and link the extrude-tin application." OFF)
option(PROFILE "Build with gprof profiling output." OFF)
option(COVERAGE "Build with gcov code coverage profiling." OFF)
option(SLIC3R_DEBUG "Build with Slic3r's debug output" OFF)

# only on newer GCCs: -ftemplate-backtrace-limit=0
add_compile_options(-ftemplate-backtrace-limit=0)
add_compile_options(-DNO_PERL -DM_PI=3.14159265358979323846 -DHAS_BOOL -DNOGDI -DBOOST_ASIO_DISABLE_KQUEUE)
if(SLIC3R_DEBUG)
    add_compile_options(-DSLIC3R_DEBUG)
endif()

if (MSVC)
    add_compile_options(-W3)
    add_compile_options(-bigobj) 
else()
    add_compile_options(-Wall)
endif()

if(DEFINED ENV{SLIC3R_VAR_REL})
    add_compile_options(-DVAR_REL=$ENV{SLIC3R_VAR_REL})
endif(DEFINED ENV{SLIC3R_VAR_REL})

if(DEFINED ENV{SLIC3R_VAR_ABS})
    add_compile_options(-DVAR_ABS)
endif(DEFINED ENV{SLIC3R_VAR_ABS})

if(DEFINED ENV{SLIC3R_VAR_ABS_PATH})
    add_compile_options(-DVAR_ABS_PATH=$ENV{SLIC3R_VAR_ABS_PATH})
endif(DEFINED ENV{SLIC3R_VAR_ABS_PATH})

# mingw needs to be told to deal with large objects
if(MINGW)
    add_compile_options(-Wa,-mbig-obj) 
endif()

if(Enable_GUI) 
    add_compile_options(-DUSE_WX)
endif(Enable_GUI)


execute_process(COMMAND git rev-parse --short HEAD OUTPUT_VARIABLE GIT_VERSION ERROR_QUIET)
if (MSVC)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(-DEBUG -Od)
        if (SLIC3R_STATIC)
            add_compile_options(-MTd)
        else(SLIC3R_STATIC)
            add_compile_options(-MDd)
        endif(SLIC3R_STATIC)
    elseif (CMAKE_BUILD_TYPE MATCHES Release)
        if (SLIC3R_STATIC)
            add_compile_options(-MTd)
        else(SLIC3R_STATIC)
            add_compile_options(-MD)
        endif(SLIC3R_STATIC)
    elseif (CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
        add_compile_options(-DEBUG -O2)
    endif()
else()    
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(-g -O0)
    elseif (CMAKE_BUILD_TYPE MATCHES Release)
        add_compile_options(-O3)
    elseif (CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
        add_compile_options(-g -O3)
    endif()
endif()

if(PROFILE)
    add_compile_options(-g -pg -DBUILD_PROFILE)
endif(PROFILE)

if(COVERAGE)
    add_compile_options(-g -ftest-coverage)
endif(COVERAGE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

IF(CMAKE_HOST_APPLE)
    add_compile_options(-stdlib=libc++ -DBOOST_THREAD_DONT_USE_CHRONO -DBOOST_NO_CXX11_RVALUE_REFERENCES -DBOOST_THREAD_USES_MOVE)
    set(CMAKE_EXE_LINKER_FLAGS "-framework IOKit -framework CoreFoundation -lc++")
ELSE(CMAKE_HOST_APPLE)
#    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -L.")
ENDIF(CMAKE_HOST_APPLE)


if(SLIC3R_STATIC)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME ON)
else(SLIC3R_STATIC)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_STATIC_RUNTIME OFF)
endif(SLIC3R_STATIC)


if (NOT GIT_VERSION STREQUAL "")
    if (MSVC)
        add_compile_options(/DSLIC3R_BUILD_COMMIT=${GIT_VERSION})
    else(MSVC)
        execute_process(COMMAND git rev-parse --short HEAD OUTPUT_VARIABLE GIT_VERSION_1)
        string(REGEX REPLACE "\n$" "" GIT_VERSION "${GIT_VERSION_1}")
        add_compile_options(-DSLIC3R_BUILD_COMMIT=${GIT_VERSION})
    endif(MSVC)
endif(NOT GIT_VERSION STREQUAL "")

find_package(Threads REQUIRED)

set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost REQUIRED COMPONENTS system thread filesystem)

if (NOT (${Boost_VERSION_STRING} VERSION_LESS "1.74.0"))
    MESSAGE("Adding in boost::nowide")
    find_package(Boost REQUIRED COMPONENTS system thread filesystem OPTIONAL_COMPONENTS nowide)
endif()



set(LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/../xs/src/)

# directory that contains the dependent non-source files, like models and configurations
set(TESTFILE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test/inputs/)

set(EXPAT_INCLUDES
    ${LIBDIR}/expat
)
set(ADMESH_INCLUDES
    ${LIBDIR}/admesh
)

set(BSPLINE_INCLUDES
    ${LIBDIR}/BSpline/
)

set(POLY2TRI_INCLUDES
    ${LIBDIR}/poly2tri
    ${LIBDIR}/poly2tri/sweep
    ${LIBDIR}/poly2tri/common
)
set(COMMON_INCLUDES
    ${LIBDIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/standalone/
)
set(SLIC3R_INCLUDES
    ${COMMON_INCLUDES}
    ${EXPAT_INCLUDES}
    ${ADMESH_INCLUDES}
    ${POLY2TRI_INCLUDES}
    ${BSPLINE_INCLUDES}
)
set(LIBSLIC3R_INCLUDES
    ${LIBDIR}/libslic3r
    ${LIBDIR}/libslic3r/IO
)


add_library(ZipArchive SHARED
    ${LIBDIR}/Zip/ZipArchive.cpp
)

add_library(miniz SHARED
    ${LIBDIR}/miniz/miniz.c
)
target_compile_features(ZipArchive PUBLIC cxx_std_11)
target_include_directories(ZipArchive PUBLIC ${COMMON_INCLUDES})
target_compile_options(ZipArchive PUBLIC -w)

add_library(libslic3r SHARED
    ${LIBDIR}/libslic3r/BoundingBox.cpp
    ${LIBDIR}/libslic3r/BridgeDetector.cpp
    ${LIBDIR}/libslic3r/ClipperUtils.cpp
    ${LIBDIR}/libslic3r/ConfigBase.cpp
    ${LIBDIR}/libslic3r/Config.cpp
    ${LIBDIR}/libslic3r/ConditionalGCode.cpp
    ${LIBDIR}/libslic3r/ExPolygon.cpp
    ${LIBDIR}/libslic3r/ExPolygonCollection.cpp
    ${LIBDIR}/libslic3r/Extruder.cpp
    ${LIBDIR}/libslic3r/ExtrusionEntity.cpp
    ${LIBDIR}/libslic3r/ExtrusionEntityCollection.cpp
    ${LIBDIR}/libslic3r/Fill/Fill.cpp
    ${LIBDIR}/libslic3r/Fill/Fill3DHoneycomb.cpp
    ${LIBDIR}/libslic3r/Fill/FillConcentric.cpp
    ${LIBDIR}/libslic3r/Fill/FillHoneycomb.cpp
    ${LIBDIR}/libslic3r/Fill/FillPlanePath.cpp
    ${LIBDIR}/libslic3r/Fill/FillRectilinear.cpp
    ${LIBDIR}/libslic3r/Fill/FillGyroid.cpp
    ${LIBDIR}/libslic3r/Flow.cpp
    ${LIBDIR}/libslic3r/GCode.cpp
    ${LIBDIR}/libslic3r/PrintGCode.cpp
    ${LIBDIR}/libslic3r/GCode/CoolingBuffer.cpp
    ${LIBDIR}/libslic3r/GCode/SpiralVase.cpp
    ${LIBDIR}/libslic3r/GCodeReader.cpp
    ${LIBDIR}/libslic3r/GCodeSender.cpp
    ${LIBDIR}/libslic3r/GCodeTimeEstimator.cpp
    ${LIBDIR}/libslic3r/GCodeWriter.cpp
    ${LIBDIR}/libslic3r/Geometry.cpp
    ${LIBDIR}/libslic3r/IO.cpp
    ${LIBDIR}/libslic3r/IO/AMF.cpp
    ${LIBDIR}/libslic3r/IO/TMF.cpp
    ${LIBDIR}/libslic3r/Layer.cpp
    ${LIBDIR}/libslic3r/LayerRegion.cpp
    ${LIBDIR}/libslic3r/LayerRegionFill.cpp
    ${LIBDIR}/libslic3r/LayerHeightSpline.cpp
    ${LIBDIR}/libslic3r/Line.cpp
    ${LIBDIR}/libslic3r/Log.cpp
    ${LIBDIR}/libslic3r/Model.cpp
    ${LIBDIR}/libslic3r/MotionPlanner.cpp
    ${LIBDIR}/libslic3r/MultiPoint.cpp
    ${LIBDIR}/libslic3r/PerimeterGenerator.cpp
    ${LIBDIR}/libslic3r/PlaceholderParser.cpp
    ${LIBDIR}/libslic3r/Point.cpp
    ${LIBDIR}/libslic3r/Polygon.cpp
    ${LIBDIR}/libslic3r/Polyline.cpp
    ${LIBDIR}/libslic3r/PolylineCollection.cpp
    ${LIBDIR}/libslic3r/Print.cpp
    ${LIBDIR}/libslic3r/PrintConfig.cpp
    ${LIBDIR}/libslic3r/PrintObject.cpp
    ${LIBDIR}/libslic3r/PrintRegion.cpp
    ${LIBDIR}/libslic3r/SimplePrint.cpp
    ${LIBDIR}/libslic3r/SLAPrint.cpp
    ${LIBDIR}/libslic3r/SlicingAdaptive.cpp
    ${LIBDIR}/libslic3r/Surface.cpp
    ${LIBDIR}/libslic3r/SurfaceCollection.cpp
    ${LIBDIR}/libslic3r/SVG.cpp
    ${LIBDIR}/libslic3r/TriangleMesh.cpp
    ${LIBDIR}/libslic3r/TransformationMatrix.cpp
    ${LIBDIR}/libslic3r/SupportMaterial.cpp
    ${LIBDIR}/libslic3r/utils.cpp
    ${LIBDIR}/libslic3r/miniz_extension.cpp
)
target_compile_features(libslic3r PUBLIC cxx_std_11)
target_include_directories(libslic3r SYSTEM PUBLIC ${SLIC3R_INCLUDES})
target_include_directories(libslic3r PUBLIC ${LIBSLIC3R_INCLUDES})
if (BOOST_NOWIDE_FOUND)
    target_compile_options(libslic3r -DBOOST_INCLUDE_NOWIDE)
endif()

add_library(BSpline SHARED
    ${LIBDIR}/BSpline/BSpline.cpp
)
target_include_directories(BSpline PUBLIC ${BSPLINE_INCLUDES})
target_compile_options(BSpline PUBLIC -w)

add_library(admesh SHARED
    ${LIBDIR}/admesh/connect.c
    ${LIBDIR}/admesh/normals.c
    ${LIBDIR}/admesh/shared.c
    ${LIBDIR}/admesh/stl_io.c
    ${LIBDIR}/admesh/stlinit.c
    ${LIBDIR}/admesh/util.c
)
target_include_directories(admesh PUBLIC ${ADMESH_INCLUDES} ${COMMON_INCLUDES})
set_property(TARGET admesh PROPERTY C_STANDARD 99)
target_compile_options(admesh PUBLIC -w)


add_library(clipper SHARED ${LIBDIR}/clipper.cpp)
target_compile_features(clipper PUBLIC cxx_std_11)
target_include_directories(clipper PUBLIC ${COMMON_INCLUDES})
target_compile_options(clipper PUBLIC -w)

add_library(expat SHARED
    ${LIBDIR}/expat/xmlparse.c
    ${LIBDIR}/expat/xmlrole.c
    ${LIBDIR}/expat/xmltok.c
)
target_compile_features(expat PUBLIC cxx_std_11)
target_include_directories(expat PUBLIC ${EXPAT_INCLUDES})
target_compile_options(expat PUBLIC -w)

add_library(polypartition SHARED ${LIBDIR}/polypartition.cpp)
target_include_directories(polypartition PUBLIC ${COMMON_INCLUDES})
target_compile_options(polypartition PUBLIC -w)

add_library(poly2tri SHARED
    ${LIBDIR}/poly2tri/common/shapes.cc
    ${LIBDIR}/poly2tri/sweep/advancing_front.cc
    ${LIBDIR}/poly2tri/sweep/cdt.cc
    ${LIBDIR}/poly2tri/sweep/sweep_context.cc
    ${LIBDIR}/poly2tri/sweep/sweep.cc
)
target_include_directories(poly2tri PUBLIC ${COMMON_INCLUDES})
target_compile_options(poly2tri PUBLIC -w)


# add_executable(slic3r slic3r.cpp)
# target_compile_features(slic3r PUBLIC cxx_std_14)

if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER 3.10)
cmake_policy(SET CMP0072 NEW)
endif (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER 3.10)



if(SLIC3R_STATIC)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME ON)
else(SLIC3R_STATIC)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_STATIC_RUNTIME OFF)
endif(SLIC3R_STATIC)

find_library(bsystem_l boost_system log)
if(SLIC3R_STATIC)
    add_library(bsystem STATIC IMPORTED)
else(SLIC3R_STATIC)
    add_library(bsystem SHARED IMPORTED)
endif(SLIC3R_STATIC)
set_target_properties(bsystem PROPERTIES IMPORTED_LOCATION ${bsystem_l})
find_library(bthread_l boost_thread)
if(SLIC3R_STATIC)
    add_library(bthread STATIC IMPORTED)
else(SLIC3R_STATIC)
add_library(bthread SHARED IMPORTED)
endif(SLIC3R_STATIC)
set_target_properties(bthread PROPERTIES IMPORTED_LOCATION ${bthread_l})
include_directories(${Boost_INCLUDE_DIRS})



IF(CMAKE_HOST_UNIX)
    #set(Boost_LIBRARIES bsystem bthread bfilesystem)
ENDIF(CMAKE_HOST_UNIX)

set(LIBSLCER_DEP
    admesh
    BSpline
    clipper
    expat
    polypartition
    poly2tri
    ZipArchive
    miniz
)

# Libraries that Libslic3r itself depends on.
set(LIBSLIC3R_DEPENDS
    admesh
    BSpline
    clipper
    expat
    polypartition
    poly2tri
    ZipArchive
    miniz
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT} 
)
# set_target_properties(libslic3r ${LIBSLCER_DEP} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

add_subdirectory(pybind11)
pybind11_add_module(pyslic3r python/pyslic3r.cpp python/point.cpp python/config.cpp python/model.cpp)
target_link_libraries(pyslic3r PUBLIC libslic3r ${LIBSLIC3R_DEPENDS})


# if (BUILD_EXTRUDE_TIN)
#     add_executable(extrude-tin utils/extrude-tin.cpp)
#     set_target_properties(extrude-tin PROPERTIES LINK_SEARCH_START_STATIC 1)
#     set_target_properties(extrude-tin PROPERTIES LINK_SEARCH_END_STATIC 1)
# endif() 

# if (BUILD_EXTRUDE_TIN)
#     target_link_libraries (extrude-tin libslic3r ${LIBSLIC3R_DEPENDS})
# endif()


