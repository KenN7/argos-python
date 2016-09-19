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
    std::cout << "go for " << a << b << std::endl;

}
