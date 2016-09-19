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

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/general.h>

#include <string>
#include <iostream>

namespace argos {

class ActusensorsWrapper
{
    public:

        ActusensorsWrapper();
        ~ActusensorsWrapper() {};

        void Init();
        void logprint(std::string message);

        //Sensors
        void CreateSensor(std::string name, CCI_Sensor* sensor);

        //Actuators
        void CreateActu(std::string name, CCI_Actuator* actu);

        void InitSteeringWheels();
        void wheels(Real a, Real b);

    private:

        CCI_DifferentialSteeringActuator* m_pcWheels;
        //maybe those if you want :
        //CCI_DifferentialSteeringActuator* m_pcWheels;
        //CCI_FootBotProximitySensor* m_pcProximity;
        //CCI_ColoredBlobOmnidirectionalCameraSensor* m_pcOmniCam;
};

}

#endif
