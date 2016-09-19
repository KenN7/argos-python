/*
 * AUTHOR: Ken Hasselmann <arg AT kenh DOT fr>
 *
 * Connects ARGoS to python
 *
 */

#ifndef PY_WRAPPER_H
#define PY_WRAPPER_H


#include <boost/python.hpp>

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_colored_blob_omnidirectional_camera_sensor.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_gripper_actuator.h>

#include <string>
#include <iostream>

#   define INIT_WRAPPER PyInit_libpy_wrapper
extern "C" PyObject* INIT_WRAPPER();

namespace argos {

class ActusensorsWrapper {

    public:

        ActusensorsWrapper();
        ~ActusensorsWrapper() {};

        void Init();

        //Sensors

        //Actuators

        void InitSteeringWheels();
        void wheels(float a, float b);

    private:

        CCI_DifferentialSteeringActuator* m_pcWheels;

};

}

#endif
