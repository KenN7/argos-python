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
// m_pcProximity(NULL),
// m_pcOmniCam(NULL),
{
}

void ActusensorsWrapper::Init()
{

}

void ActusensorsWrapper::logprint(std::string message)
{
  std::cout << message << std::endl;
}

void ActusensorsWrapper::InitSteeringWheels()
{


}

void ActusensorsWrapper::wheels(Real fLeftWheelSpeed, Real fRightWheelSpeed)
{
    //std::cout << "go for " << a << b << std::endl;

    //mthis->mpc->SetLinearVelocity(fLeftWheelSpeed, fRightWheelSpeed);
    m_pcWheels->SetLinearVelocity(fLeftWheelSpeed, fRightWheelSpeed);
}


//those 2 following functions serve as wrappers for Actuators and Sensors
//the if are based on the options of the XML
//we could add for ex :
//actuators/sensors
//CCI_DifferentialSteeringActuator* m_pcWheels = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
//m_pcProximity = GetSensor<CCI_FootBotProximitySensor>("footbot_proximity");
//m_pcOmniCam = GetSensor<CCI_ColoredBlobOmnidirectionalCameraSensor>("colored_blob_omnidirectional_camera");

void ActusensorsWrapper::CreateActu(std::string name, CCI_Actuator* actu)
{
  if (name == "differential_steering")
  {
    m_pcWheels = (CCI_DifferentialSteeringActuator*)actu;
    //mthis = actu;
  }

}

void ActusensorsWrapper::CreateSensor(std::string name, CCI_Sensor* sensor)
{

}


BOOST_PYTHON_MODULE(libpy_controller_interface)
{
    //class_< ActusensorsWrapper >("robot", no_init)
  class_< ActusensorsWrapper, boost::shared_ptr<ActusensorsWrapper>, boost::noncopyable >("robot", no_init)
    .def("logprint", &ActusensorsWrapper::logprint)
    .def("wheels", &ActusensorsWrapper::wheels)
  ;
}
