#include "py_actusensor_wrapper_epuck.h"
#include "py_wrapper.h"

using namespace argos;

/****************************************/
/****************************************/

CEPuckWheelsWrapper::CEPuckWheelsWrapper()
{
}

void CEPuckWheelsWrapper::SetSpeed(const Real f_left_wheel_speed, const Real f_right_wheel_speed)
{
    if (m_bEPuckWheels)
    {
        m_pcEPuckWheels->SetLinearVelocity(f_left_wheel_speed, f_right_wheel_speed);
    }
    else
    {
        ActusensorsWrapper::Logprint("Differential Steering Actuator not implemented or not stated in XML config.");
    }
}

/****************************************/
/****************************************/

CEPuckProximitySensorWrapper::CEPuckProximitySensorWrapper()
{
}

boost::python::list CEPuckProximitySensorWrapper::GetReadings() const
{
    if (m_bEPuckProximity)
    {
        return ActusensorsWrapper::ToPythonList(m_pcEPuckProximity->GetReadings());
    }
    else
    {
        ActusensorsWrapper::Logprint("Proximity sensor not implemented or not stated in XML config.");
        // TODO: add exception?
    }
}