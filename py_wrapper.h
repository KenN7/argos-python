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
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
/* Definition of the range and bearing actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.h>
/* Definition of the range and bearing sensor */
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/general.h>

#include <string>
#include <iostream>

namespace argos
{

class ActusensorsWrapper
{
  public:
    ActusensorsWrapper();
    ~ActusensorsWrapper(){};

    void Init();
    static void logprint(std::string message);

    //Sensors
    void CreateSensor(std::string name, CCI_Sensor *sensor, TConfigurationNode &t_node);

    //Actuators
    void CreateActu(std::string name, CCI_Actuator *actu, TConfigurationNode &t_node);

    void InitSteeringWheels();
    boost::python::list proximity();
    //boost::python::list get_blobs();
    //argos::CCI_ColoredBlobOmnidirectionalCameraSensor *GetOmniDirectionalCamera();
    //argos::CCI_DifferentialSteeringActuator *GetWheels();

    struct WheelsWrapper
    {
        argos::CCI_DifferentialSteeringActuator *m_pcWheels;
        bool m_pcWheels_bool;
        void set_speed(float fLeftWheelSpeed, float fRightWheelSpeed)
        {
            if (m_pcWheels_bool)
            {
                m_pcWheels->SetLinearVelocity(fLeftWheelSpeed, fRightWheelSpeed);
            }
            else
            {
                ActusensorsWrapper::logprint("Differential Steering Actuator not implemented or not stated in XML config.");
            }
        }
    };

    struct OmnidirectionalCameraWrapper
    {
        argos::CCI_ColoredBlobOmnidirectionalCameraSensor *m_pcOmniCam;
        bool m_pcOmniCam_bool;
        boost::python::list GetReadings()
        {
            if (m_pcOmniCam_bool)
            {
                return ActusensorsWrapper::ToPythonList(m_pcOmniCam->GetReadings().BlobList);
            }
            else
            {
                ActusensorsWrapper::logprint("Omnidirectional Camera Sensor not implemented or not stated in XML config.");
            }
        }

        void Enable()
        {
            m_pcOmniCam->Enable();
        }
        void Disable()
        {
            m_pcOmniCam->Disable();
        }
        int get_counter()
        {
            return m_pcOmniCam->GetReadings().Counter;
        }
    };

    struct FootBotProximitySensorWrapper
    {
        argos::CCI_FootBotProximitySensor *m_pcProximity;
        bool m_pcProximity_bool;

        boost::python::list GetReadings()
        {
            if (m_pcProximity_bool)
            {
                return ToPythonList(m_pcProximity->GetReadings());
            }
            else
            {
                ActusensorsWrapper::logprint("Proximity sensor not implemented or not stated in XML config.");
                // TODO: add exception?
            }
        }
    };

    struct LedsActuatorWrapper
    {
        argos::CCI_LEDsActuator *m_pcLeds;
        bool m_pcLeds_bool;

        void SetSingleColorString(const UInt8 led_id, std::string color_name)
        {
            m_pcLeds->SetSingleColor(led_id, ColorWrapper(color_name).color);
        }
        void SetSingleColorRGB(const UInt8 led_id, int red, int green, int blue)
        {
            m_pcLeds->SetSingleColor(led_id, ColorWrapper(red, green, blue).color);
        }
        void SetAllColorsString(std::string color_name)
        {
            m_pcLeds->SetAllColors(ColorWrapper(color_name).color);
        }
        void SetAllColorsRGB(int red, int green, int blue)
        {
            m_pcLeds->SetAllColors(ColorWrapper(red, green, blue).color);
        }
    };

    struct ColorWrapper
    {
        argos::CColor color;

        ColorWrapper(std::string color_name)
        {
            if (color_name == "red")
                color = argos::CColor::RED;
            else if (color_name == "black")
                color = argos::CColor::BLACK;
        }
        ColorWrapper(int red, int green, int blue)
        {
            color = argos::CColor((const UInt8)red, (const UInt8)green, (const UInt8)blue, 0);
        }
    };

    struct RangeAndBearingWrapper
    {
        argos::CCI_RangeAndBearingActuator *m_pcRABA;
        argos::CCI_RangeAndBearingSensor *m_pcRABS;
        bool m_pcRABA_bool;
        bool m_pcRABS_bool;

        void ClearData()
        {
            m_pcRABA->ClearData();
        }
        void SetData(size_t un_idx, UInt8 un_value)
        {
            m_pcRABA->SetData(un_idx, un_value);
        }
        boost::python::list GetReadings()
        {
            return ToPythonList(m_pcRABS->GetReadings());
        }
    };

    static void vector_setitem(argos::CByteArray &v, int index, UInt8 value);

    static UInt8 vector_getitem(argos::CByteArray &v, int index);

    WheelsWrapper wheels_wrapper = WheelsWrapper();
    OmnidirectionalCameraWrapper omnidirectional_camera_wrapper = OmnidirectionalCameraWrapper();
    FootBotProximitySensorWrapper proximity_sensor_wrapper = FootBotProximitySensorWrapper();
    LedsActuatorWrapper leds_wrapper = LedsActuatorWrapper();
    RangeAndBearingWrapper range_and_bearing_wrapper = RangeAndBearingWrapper();

  private:
    //CCI_DifferentialSteeringActuator *m_pcWheels;
    //CCI_FootBotProximitySensor *m_pcProximity;
    //CCI_ColoredBlobOmnidirectionalCameraSensor *m_pcOmniCam;

    //variables to activate a actuator/sensors
    //bool m_pcWheels_bool;
    //bool m_pcProximity_bool;
    //bool m_pcOmniCam_bool;

    template <class T>
    static boost::python::list ToPythonList(std::vector<T> vector)
    {
        typename std::vector<T>::iterator iter;
        boost::python::list list;
        for (iter = vector.begin(); iter != vector.end(); ++iter)
        {
            list.append(*iter);
        }
        return list;
    }
};
}

#endif
