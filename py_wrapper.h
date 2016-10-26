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
/* Definition of the foot-bot motor ground sensor */
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_motor_ground_sensor.h>

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

    static void logprint(std::string message);

    //Sensors
    void CreateSensor(std::string name, CCI_Sensor *sensor, TConfigurationNode &t_node);

    //Actuators
    void CreateActu(std::string name, CCI_Actuator *actu, TConfigurationNode &t_node);

    // ACTUATORS AND SENSOR WRAPPERS

    // For each actuator and sensor, it is provided a struct that contains a reference to the actual C++ actuator or sensor,
    // and a series of functions to interact with it.
    // Each wrapper is then exported in python as a property of the "robot" class,
    // and the functions of each wrapper can be used from python as methods or class properties.

    // TODO: add m_pc_..._bool error checking

    // Wrapper for the Differential Steering Actuator.
    struct WheelsWrapper
    {
        argos::CCI_DifferentialSteeringActuator *m_pcWheels;
        bool m_pcWheels_bool;

        // Set the speed of the two wheels.
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

    // Wrapper for the Omnidirectional Camera.
    // It is possible to enable/disable the camera, get the number of readings, and get the readings of the camera.
    struct OmnidirectionalCameraWrapper
    {
        argos::CCI_ColoredBlobOmnidirectionalCameraSensor *m_pcOmniCam;
        bool m_pcOmniCam_bool;

        // Get the readings from the camera, obtained at this control step.
        // Each reading is exposed as a "omnidirectional_camera_packet",
        // from which it is possible to obtain distance, angle and color of each reading.
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

        // Enable the camera.
        void Enable()
        {
            m_pcOmniCam->Enable();
        }
        // Disable the camera.
        void Disable()
        {
            m_pcOmniCam->Disable();
        }
        // Return the number of readings obtained so far, i.e. the number of control steps from which the recording started.
        int get_counter()
        {
            return m_pcOmniCam->GetReadings().Counter;
        }
    };

    // Wrapper for the Range and Bearing Sensor and Actuator. 
    // Both of them are exposed as a single property of the robot, for simplicity.
    struct RangeAndBearingWrapper
    {
        argos::CCI_RangeAndBearingActuator *m_pcRABA;
        argos::CCI_RangeAndBearingSensor *m_pcRABS;
        bool m_pcRABA_bool;
        bool m_pcRABS_bool;

        // Erase the readings.
        void ClearData()
        {
            m_pcRABA->ClearData();
        }
        // Set the i-th bit of the data table.
        void SetData(size_t un_idx, UInt8 un_value)
        {
            m_pcRABA->SetData(un_idx, un_value);
        }
        // TODO: Set all bits at once

        // Return the readings obtained at this control step. 
        // Each reading contains the range, the horizontal bearing, the vertical bearing and the data table. 
        // The data table is exposed as a c_byte_array. 
        boost::python::list GetReadings()
        {
            return ToPythonList(m_pcRABS->GetReadings());
        }
    };

    // Wrapper for the Leds Actuator.
    struct LedsActuatorWrapper
    {
        argos::CCI_LEDsActuator *m_pcLeds;
        bool m_pcLeds_bool;

        // Set the color of a given led, given its name.
        void SetSingleColorString(const UInt8 led_id, std::string color_name)
        {
            m_pcLeds->SetSingleColor(led_id, ColorWrapper(color_name).color);
        }
        // Set the color of a given led, given its RGB values.
        void SetSingleColorRGB(const UInt8 led_id, int red, int green, int blue)
        {
            m_pcLeds->SetSingleColor(led_id, ColorWrapper(red, green, blue).color);
        }
        // Set the color of every led, given its name.
        void SetAllColorsString(std::string color_name)
        {
            m_pcLeds->SetAllColors(ColorWrapper(color_name).color);
        }
        // Set the color of every led, given its RGB values.
        void SetAllColorsRGB(int red, int green, int blue)
        {
            m_pcLeds->SetAllColors(ColorWrapper(red, green, blue).color);
        }
    };

     // Wrapper for the Proximity Sensor.
    struct FootBotProximitySensorWrapper
    {
        argos::CCI_FootBotProximitySensor *m_pcProximity;
        bool m_pcProximity_bool;

        // Obtain the proximity readings at this control step.
        // The readings are exposed as a python list.
        // Each reading is exposed as a "proximity_reading", from which it is possible to obtain value and angle.
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

    struct GroundSensorWrapper
    {
        argos::CCI_FootBotMotorGroundSensor* m_pcGround;
        bool m_pcGround_bool;

        boost::python::list GetReadings()
        {
            if (m_pcGround_bool)
            {
                return ToPythonList(m_pcGround->GetReadings());
            }
            else
            {
                ActusensorsWrapper::logprint("Motor Ground Sensor not implemented or not stated in XML config.");
                // TODO: add exception?
            }
        }
    };

    // Wrapper for the CColor class. 
    struct ColorWrapper
    {
        argos::CColor color;

        // Create a color by providing its name.
        ColorWrapper(std::string color_name)
        {
            if (color_name == "red")
                color = argos::CColor::RED;
            else if (color_name == "black")
                color = argos::CColor::BLACK;
            // TODO: more colors
        }
        // Create a color by providing its RGB values.
        ColorWrapper(int red, int green, int blue)
        {
            color = argos::CColor((const UInt8)red, (const UInt8)green, (const UInt8)blue, 0);
        }
    };

    // Define a wrapper for each actuator and sensor
    WheelsWrapper wheels_wrapper = WheelsWrapper();
    OmnidirectionalCameraWrapper omnidirectional_camera_wrapper = OmnidirectionalCameraWrapper();
    FootBotProximitySensorWrapper proximity_sensor_wrapper = FootBotProximitySensorWrapper();
    LedsActuatorWrapper leds_wrapper = LedsActuatorWrapper();
    RangeAndBearingWrapper range_and_bearing_wrapper = RangeAndBearingWrapper();
    GroundSensorWrapper ground_sensor_wrapper = GroundSensorWrapper();

    // Define functions to access the elements the argos::CByteArray class.
    // Original source: https://mail.python.org/pipermail/cplusplus-sig/2003-June/004024.html
    static void c_byte_array_setitem(argos::CByteArray &v, int index, UInt8 value)
    {
        if (index >= 0 && index < v.Size())
        {
            v[index] = value;
        }
        else
        {
            PyErr_SetString(PyExc_IndexError, "index out of range");
            boost::python::throw_error_already_set();
        }
    }

    static UInt8 c_byte_array_getitem(argos::CByteArray &v, int index)
    {
        if (index >= 0 && index < v.Size())
        {
            return v[index];
        }
        else
        {
            PyErr_SetString(PyExc_IndexError, "index out of range");
            boost::python::throw_error_already_set();
        }
    }

  private:
    // Used to convert vectors to python lists.
    // Original source: http://www.boost.org/doc/libs/1_62_0/libs/python/pyste/doc/wrappers.html
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
