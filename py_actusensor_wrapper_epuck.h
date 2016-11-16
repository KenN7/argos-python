#ifndef PY_ACTUSENSOR_WRAPPER_EPUCK_H
#define PY_ACTUSENSOR_WRAPPER_EPUCK_H

#include <boost/python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

#include <argos3/core/control_interface/ci_controller.h>

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_wheels_actuator.h>
#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_proximity_sensor.h>

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/general.h>

#include <string>
#include <iostream>

namespace argos
{

// TODO: add m_pc_..._bool error checkin
// Wrapper for the Differential Steering Actuator.
class CEPuckWheelsWrapper
{
  public:
    CEPuckWheelsWrapper();
    ~CEPuckWheelsWrapper(){};
    argos::CCI_EPuckWheelsActuator *m_pcEPuckWheels;
    bool m_bEPuckWheels;
    // Set the speed of the two wheels.
    void SetSpeed(const Real f_left_wheel_speed, const Real f_right_wheel_speed);
};

// Wrapper for the Proximity Sensor.
class CEPuckProximitySensorWrapper
{
  public:
    CEPuckProximitySensorWrapper();
    ~CEPuckProximitySensorWrapper(){};
    argos::CCI_EPuckProximitySensor *m_pcEPuckProximity;
    bool m_bEPuckProximity;

    // Obtain the proximity readings at this control step.
    // The readings are exposed as a python list.
    // Each reading is exposed as a "proximity_reading", from which it is possible to obtain value and angle.
    boost::python::list GetReadings() const;
};



}
#endif

