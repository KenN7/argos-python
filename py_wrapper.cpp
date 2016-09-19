/*
 * AUTHOR: Ken Hasselmann <arg AT kenh DOT fr>
 *
 * Connects ARGoS to python
 *
 */
#include "py_wrapper.h"

using namespace argos;
using namespace boost::python;

ActusensorsWrapper::ActusensorsWrapper() :
m_pcWheels(NULL)
{
}

void ActusensorsWrapper::Init()
{

}

void ActusensorsWrapper::InitSteeringWheels()
{

}

void ActusensorsWrapper::wheels(float a, float b)
{

}


BOOST_PYTHON_MODULE(robot)
{
  class_<ActusensorsWrapper>("robot", no_init)
    .def("wheels", &ActusensorsWrapper::wheels)
  ;

}
