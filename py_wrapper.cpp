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
    m_pcWheels = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");

}

void ActusensorsWrapper::wheels(Real fLeftWheelSpeed, Real fRightWheelSpeed)
{
    //std::cout << "go for " << a << b << std::endl;
    m_pcWheels->SetLinearVelocity(fLeftWheelSpeed, fRightWheelSpeed);
}
