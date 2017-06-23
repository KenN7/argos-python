#
# Set minimum required version
#
cmake_minimum_required(VERSION 2.8)
#
# Set the path additional cmake files must be searched for
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 11)

#
# Make sure we are under Unix
#
if(NOT UNIX)
  message(FATAL_ERROR "ARGoS compiles only under UNIX, sorry!")
endif(NOT UNIX)

#
# Set build options
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildOptions.cmake)

#
# Set compiler flags
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildFlags.cmake)

#
# Check for libraries
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildChecks.cmake)

#
# Set up CPack for later use
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSPackaging.cmake)

# Set up ARGoS compilation information
include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIRS})

set(Python_ADDITIONAL_VERSIONS 3.5)
find_package(PythonLibs)
include_directories (${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHON_LIBRARIES})

find_package(Boost 1.61.0 COMPONENTS python3)
include_directories (${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})

add_library (
    py_controller_interface SHARED py_controller.cpp py_wrapper.cpp py_actusensor_wrapper_generic.cpp py_actusensor_wrapper_footbot.cpp py_actusensor_wrapper_epuck.cpp
)

target_link_libraries (py_controller_interface
    argos3core_simulator
    argos3plugin_simulator_footbot
    argos3plugin_simulator_genericrobot
    ${PYTHON_LIBRARIES}
    ${Boost_LIBRARIES}
)
