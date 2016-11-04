/*
 * AUTHOR: Ken Hasselmann <arg AT kenh DOT fr>
 *
 * Connects ARGoS to python
 *
 */

#ifndef PY_WRAPPER_H
#define PY_WRAPPER_H

#include <boost/python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

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
/* Definition of the foot-bot gripper actuator */
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_gripper_actuator.h>
/* Definition of the foot-bot light sensor */
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_light_sensor.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_base_ground_sensor.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_distance_scanner_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_distance_scanner_sensor.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_turret_encoder_sensor.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_turret_actuator.h>

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

    static void Logprint(const std::string str_message);

    //Sensors
    void CreateSensor(const std::string str_name, CCI_Sensor *pc_sensor, TConfigurationNode &t_node);

    //Actuators
    void CreateActu(const std::string str_name, CCI_Actuator *pc_actu, TConfigurationNode &t_node);
	
	/****************************************/
    /****************************************/

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
        bool m_bWheels;

        // Set the speed of the two wheels.
        void SetSpeed(const Real f_left_wheel_speed, const Real f_right_wheel_speed)
        {
            if (m_bWheels)
            {
                m_pcWheels->SetLinearVelocity(f_left_wheel_speed, f_right_wheel_speed);
            }
            else
            {
                ActusensorsWrapper::Logprint("Differential Steering Actuator not implemented or not stated in XML config.");
            }
        }
    };
	
	/****************************************/
    /****************************************/

    // Wrapper for the Gripper Actuator
    struct GripperWrapper
    {
        argos::CCI_FootBotGripperActuator *m_pcGripper;
        bool m_bGripper;

        void Lock() 
        {
            if (m_bGripper)
            {
                m_pcGripper->LockPositive();
            }
            else
            {
                ActusensorsWrapper::Logprint("Gripper Actuator not implemented or not stated in XML config.");
            }
        }
        void Unlock()
        {
            if (m_bGripper)
            {
                m_pcGripper->Unlock();
            }
            else
            {
                ActusensorsWrapper::Logprint("Gripper Actuator not implemented or not stated in XML config.");
            }
        }
    };
	
	/****************************************/
    /****************************************/	

    // Wrapper for the Omnidirectional Camera.
    // It is possible to enable/disable the camera, get the number of readings, and get the readings of the camera.
    struct OmnidirectionalCameraWrapper
    {
        argos::CCI_ColoredBlobOmnidirectionalCameraSensor *m_pcOmniCam;
        bool m_bOmniCam;

        // Get the readings from the camera, obtained at this control step.
        // Each reading is exposed as a "omnidirectional_camera_packet",
        // from which it is possible to obtain distance, angle and color of each reading.
        boost::python::list GetReadings() const
        {
            if (m_bOmniCam)
            {
                return ActusensorsWrapper::ToPythonList(m_pcOmniCam->GetReadings().BlobList);
            }
            else
            {
                ActusensorsWrapper::Logprint("Omnidirectional Camera Sensor not implemented or not stated in XML config.");
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
        const int GetCounter() const
        {
            return m_pcOmniCam->GetReadings().Counter;
        }
    };
	
	/****************************************/
    /****************************************/	

    // Wrapper for the Range and Bearing Sensor and Actuator.
    // Both of them are exposed as a single property of the robot, for simplicity.
    struct RangeAndBearingWrapper
    {
        argos::CCI_RangeAndBearingActuator *m_pcRABA;
        argos::CCI_RangeAndBearingSensor *m_pcRABS;
        bool m_bRABA;
        bool m_bRABS;

        // Erase the readings.
        void ClearData()
        {
            m_pcRABA->ClearData();
        }
        // Set the i-th bit of the data table.
        void SetData(const size_t un_idx, const UInt8 un_value)
        {
            m_pcRABA->SetData(un_idx, un_value);
        }
        // TODO: Set all bits at once

        // Return the readings obtained at this control step.
        // Each reading contains the range, the horizontal bearing, the vertical bearing and the data table.
        // The data table is exposed as a c_byte_array.
        boost::python::list GetReadings() const
        {
            return ToPythonList(m_pcRABS->GetReadings());
        }
    };
	
	/****************************************/
    /****************************************/	

    // Wrapper for the Leds Actuator.
    struct LedsActuatorWrapper
    {
        argos::CCI_LEDsActuator *m_pcLeds;
        bool m_bLeds;

        // Set the color of a given led, given its name.
        void SetSingleColorString(const UInt8 un_led_id, const std::string str_color_name)
        {
            m_pcLeds->SetSingleColor(un_led_id, ColorWrapper(str_color_name).m_cColor);
        }
        // Set the color of a given led, given its RGB values.
        void SetSingleColorRGB(const UInt8 un_led_id, const UInt8 un_red, const UInt8 un_green, const UInt8 un_blue)
        {
            m_pcLeds->SetSingleColor(un_led_id, ColorWrapper(un_red, un_green, un_blue).m_cColor);
        }
        // Set the color of every led, given its name.
        void SetAllColorsString(const std::string str_color_name)
        {
            m_pcLeds->SetAllColors(ColorWrapper(str_color_name).m_cColor);
        }
        // Set the color of every led, given its RGB values.
        void SetAllColorsRGB(const UInt8 un_red, const UInt8 un_green, const UInt8 un_blue)
        {
            m_pcLeds->SetAllColors(ColorWrapper(un_red, un_green, un_blue).m_cColor);
        }
    };
	
	/****************************************/
    /****************************************/	

    // Wrapper for the Proximity Sensor.
    struct FootBotProximitySensorWrapper
    {
        argos::CCI_FootBotProximitySensor *m_pcProximity;
        bool m_bProximity;

        // Obtain the proximity readings at this control step.
        // The readings are exposed as a python list.
        // Each reading is exposed as a "proximity_reading", from which it is possible to obtain value and angle.
        boost::python::list GetReadings() const
        {
            if (m_bProximity)
            {
                return ToPythonList(m_pcProximity->GetReadings());
            }
            else
            {
                ActusensorsWrapper::Logprint("Proximity sensor not implemented or not stated in XML config.");
                // TODO: add exception?
            }
        }
    };
	
	/****************************************/
    /****************************************/	

    // Wrapper for the Ground Motor Sensor.
    // Allows to get a list of readings from the ground.
    struct GroundSensorWrapper
    {
        argos::CCI_FootBotMotorGroundSensor *m_pcGround;
        bool m_bGround;

        boost::python::list GetReadings() const
        {
            if (m_bGround)
            {
                return ToPythonList(m_pcGround->GetReadings());
            }
            else
            {
                ActusensorsWrapper::Logprint("Motor Ground Sensor not implemented or not stated in XML config.");
                // TODO: add exception?
            }
        }
    };
	
	/****************************************/
    /****************************************/	

    // Wrapper for the Base Ground Sensor.
    // Allows to get a list of readings from the ground.
    struct BaseGroundSensorWrapper
    {
        argos::CCI_FootBotBaseGroundSensor *m_pcBaseGround;
        bool m_bBaseGround;

        boost::python::list GetReadings() const
        {
            if (m_bBaseGround)
            {
                return ToPythonList(m_pcBaseGround->GetReadings());
            }
            else
            {
                ActusensorsWrapper::Logprint("Base Ground Sensor not implemented or not stated in XML config.");
                // TODO: add exception?
            }
        }
    };
	
	/****************************************/
    /****************************************/	

    // Wrapper for the Light Sensor
    struct LightSensorWrapper
    {
        argos::CCI_FootBotLightSensor *m_pcLight;
        bool m_bLight;

        boost::python::list GetReadings() const
        {
            if (m_bLight)
            {
                return ToPythonList(m_pcLight->GetReadings());
            }
            else
            {
                ActusensorsWrapper::Logprint("Light Sensor not implemented or not stated in XML config.");
                // TODO: add exception?
            }
        }
    };
	
	/****************************************/
    /****************************************/	

    // Wrapper for the Distance Scanner Sensor and Actuator
    struct DistanceScannerWrapper
    {
        argos::CCI_FootBotDistanceScannerActuator *m_pcScannerActuator;
        argos::CCI_FootBotDistanceScannerSensor *m_pcScannerSensor;
        bool m_bScannerActuator;
        bool m_bScannerSensor;

        void Enable()
        {
            if (m_bScannerActuator)
            {
                m_pcScannerActuator->Enable();
            }
            else
            {
                ActusensorsWrapper::Logprint("Distance Scanner Actuator not implemented or not stated in XML config.");
            }
        }

        void Disable()
        {
            if (m_bScannerActuator)
            {
                m_pcScannerActuator->Disable();
            }
            else
            {
                ActusensorsWrapper::Logprint("Distance Scanner Actuator not implemented or not stated in XML config.");
            }
        }

        void SetRPM(const Real f_rpm)
        {
            if (m_bScannerActuator)
            {
                if (f_rpm < 0)
                    m_pcScannerActuator->SetRPM(0);
                m_pcScannerActuator->SetRPM(f_rpm);
            }
            else
            {
                ActusensorsWrapper::Logprint("Distance Scanner Actuator not implemented or not stated in XML config.");
            }
        }

        void SetAngle(const Real f_angle)
        {
            if (m_bScannerActuator)
            {
                m_pcScannerActuator->SetAngle(CRadians(f_angle));
            }
            else
            {
                ActusensorsWrapper::Logprint("Distance Scanner Actuator not implemented or not stated in XML config.");
            }
        }

        std::map<CRadians, Real> GetReadings() const
        {
            if (m_bScannerSensor)
            {
                return m_pcScannerSensor->GetReadingsMap();
            }
            else
            {
                ActusensorsWrapper::Logprint("Distance Scanner Sensor not implemented or not stated in XML config.");
            }
        }

        std::map<CRadians, Real> GetShortReadings() const
        {
            if (m_bScannerSensor)
            {
                return m_pcScannerSensor->GetShortReadingsMap();
            }
            else
            {
                ActusensorsWrapper::Logprint("Distance Scanner Sensor not implemented or not stated in XML config.");
            }
        }

        std::map<CRadians, Real> GetLongReadings() const
        {
            if (m_bScannerSensor)
            {
                return m_pcScannerSensor->GetLongReadingsMap();
            }
            else
            {
                ActusensorsWrapper::Logprint("Distance Scanner Sensor not implemented or not stated in XML config.");
            }
        }
    };
	
	/****************************************/
    /****************************************/	

    // Wrapper for the Turret Actuator and Sensor
    struct TurretWrapper
    {
        CCI_FootBotTurretEncoderSensor *m_pcTurretSensor;
        CCI_FootBotTurretActuator *m_pcTurretActuator;
        bool m_bTurretSensor;
        bool m_bTurretActuator;

        CRadians GetRotation() const
        {
            if (m_bTurretSensor)
            {
                return m_pcTurretSensor->GetRotation();
            }
            else
            {
                ActusensorsWrapper::Logprint("Turret Sensor not implemented or not stated in XML config.");
            }
        }
        void SetRotation(const Real f_angle)
        {
            if (m_bTurretActuator)
            {
                m_pcTurretActuator->SetRotation(argos::CRadians(f_angle));
            }
            else
            {
                ActusensorsWrapper::Logprint("Turret Actuator not implemented or not stated in XML config.");
            }
        }
        void SetRotationSpeed(const UInt32 n_speed_pulses)
        {
            if (m_bTurretActuator)
            {
                m_pcTurretActuator->SetRotationSpeed(n_speed_pulses);
            }
            else
            {
                ActusensorsWrapper::Logprint("Turret Actuator not implemented or not stated in XML config.");
            }
        }
        void SetMode(const std::string str_mode_name)
        {
            if (m_bTurretActuator)
            {
                if (str_mode_name == "off")
                {
                    m_pcTurretActuator->SetMode(argos::CCI_FootBotTurretActuator::ETurretModes::MODE_OFF);
                }
                if (str_mode_name == "passive")
                {
                    m_pcTurretActuator->SetMode(argos::CCI_FootBotTurretActuator::ETurretModes::MODE_PASSIVE);
                }
                if (str_mode_name == "speed_control")
                {
                    m_pcTurretActuator->SetMode(argos::CCI_FootBotTurretActuator::ETurretModes::MODE_SPEED_CONTROL);
                }
                if (str_mode_name == "position_control")
                {
                    m_pcTurretActuator->SetMode(argos::CCI_FootBotTurretActuator::ETurretModes::MODE_POSITION_CONTROL);
                }
            }
            else
            {
                ActusensorsWrapper::Logprint("Turret Actuator not implemented or not stated in XML config.");
            }
        }
        void SetActiveWithRotation(const Real f_angle)
        {
            if (m_bTurretActuator)
            {
                m_pcTurretActuator->SetActiveWithRotation(argos::CRadians(f_angle));
            }
            else
            {
                ActusensorsWrapper::Logprint("Turret Actuator not implemented or not stated in XML config.");
            }
        }
        void SetSpeedControlMode()
        {
            if (m_bTurretActuator)
            {
                m_pcTurretActuator->SetSpeedControlMode();
            }
            else
            {
                ActusensorsWrapper::Logprint("Turret Actuator not implemented or not stated in XML config.");
            }
        }
        void SetPositionControlMode()
        {
            if (m_bTurretActuator)
            {
                m_pcTurretActuator->SetPositionControlMode();
            }
            else
            {
                ActusensorsWrapper::Logprint("Turret Actuator not implemented or not stated in XML config.");
            }
        }
        void SetPassiveMode()
        {
            if (m_bTurretActuator)
            {
                m_pcTurretActuator->SetPassiveMode();
            }
            else
            {
                ActusensorsWrapper::Logprint("Turret Actuator not implemented or not stated in XML config.");
            }
        }
    };
	
	/****************************************/
    /****************************************/	

    // Wrapper for the CColor class.
    struct ColorWrapper
    {
        argos::CColor m_cColor;

        // Create a color by providing its name.
        ColorWrapper(const std::string str_color_name)
        {
            if (str_color_name == "red")
                m_cColor = argos::CColor::RED;
            else if (str_color_name == "black")
                m_cColor = argos::CColor::BLACK;
            // TODO: more colors
        }
        // Create a color by providing its RGB values.
        ColorWrapper(const UInt8 un_red, const UInt8 un_green, const UInt8 un_blue)
        {
            m_cColor = argos::CColor((const UInt8)un_red, (const UInt8)un_green, (const UInt8)un_blue, 0);
        }
    };
	
	/****************************************/
    /****************************************/

    // Define a wrapper for each actuator and sensor
    WheelsWrapper m_cWheelsWrapper = WheelsWrapper();
    GripperWrapper m_cGripperWrapper = GripperWrapper();
    OmnidirectionalCameraWrapper m_cOmnidirectionalCameraWrapper = OmnidirectionalCameraWrapper();
    FootBotProximitySensorWrapper m_cProximitySensorWrapper = FootBotProximitySensorWrapper();
    LedsActuatorWrapper m_cLedsWrapper = LedsActuatorWrapper();
    RangeAndBearingWrapper m_cRangeAndBearingWrapper = RangeAndBearingWrapper();
    GroundSensorWrapper m_cGroundSensorWrapper = GroundSensorWrapper();
    BaseGroundSensorWrapper m_cBaseGroundSensorWrapper = BaseGroundSensorWrapper();
    LightSensorWrapper m_cLightSensorWrapper = LightSensorWrapper();
    DistanceScannerWrapper m_cDistanceScannerWrapper = DistanceScannerWrapper();
    TurretWrapper m_cTurretWrapper = TurretWrapper();
	
	/****************************************/
    /****************************************/

    // Define functions to access the elements the argos::CByteArray class.
    // Original source: https://mail.python.org/pipermail/cplusplus-sig/2003-June/004024.html
    static void CByteArraySetItem(argos::CByteArray &c_vec, const UInt32 un_index, const UInt8 un_value)
    {
        if (un_index >= 0 && un_index < c_vec.Size())
        {
            c_vec[un_index] = un_value;
        }
        else
        {
            PyErr_SetString(PyExc_IndexError, "index out of range");
            boost::python::throw_error_already_set();
        }
    }

    static UInt8 CByteArrayGetItem(const argos::CByteArray &c_vec, const UInt32 un_index)
    {
        if (un_index >= 0 && un_index < c_vec.Size())
        {
            return c_vec[un_index];
        }
        else
        {
            PyErr_SetString(PyExc_IndexError, "index out of range");
            boost::python::throw_error_already_set();
        }
    }
	
	/****************************************/
    /****************************************/

  private:
    // Used to convert vectors to python lists.
    // Original source: http://www.boost.org/doc/libs/1_62_0/libs/python/pyste/doc/wrappers.html
    template <class T>
    static boost::python::list ToPythonList(std::vector<T> c_vec)
    {
        typename std::vector<T>::iterator iter;
        boost::python::list list;
        for (iter = c_vec.begin(); iter != c_vec.end(); ++iter)
        {
            list.append(*iter);
        }
        return list;
    }
};
}

#endif
