/*
 * AUTHOR: Ken Hasselmann <arg AT kenh DOT fr>
 *
 * Connects ARGoS to python
 *
 */
#include "py_wrapper.h"

using namespace argos;
using namespace boost::python;

ActusensorsWrapper::ActusensorsWrapper()
{
}

/****************************************/
/****************************************/	

void ActusensorsWrapper::Logprint(const std::string str_message)
{
    std::cout << str_message << std::endl;
}

/****************************************/
/****************************************/	

// Initialize the actuator specified by the provided name.

// NOTE: the Init function gives problems for all the sensors/actuators that use the "media" parameter.
// When the argos file is loaded, the following error is given:
//    [FATAL] Error parsing attribute "medium"
//    Attribute does not exist <ticpp.h@1791>
// The sensors/actuators seem to work even without it, so for now it has been disabled.
void ActusensorsWrapper::CreateActu(const std::string str_name, CCI_Actuator *pc_actu, TConfigurationNode &t_node)
{
    if (str_name == "differential_steering")
    {
        m_cWheelsWrapper.m_pcWheels = (CCI_DifferentialSteeringActuator *)pc_actu;
        m_cWheelsWrapper.m_bWheels = true;
        m_cWheelsWrapper.m_pcWheels->Init(t_node);
    }
    if (str_name == "footbot_gripper")
    {
        m_cGripperWrapper.m_pcGripper = dynamic_cast<CCI_FootBotGripperActuator *>(pc_actu);
        m_cGripperWrapper.m_bGripper = true;
        m_cGripperWrapper.m_pcGripper->Init(t_node);
    }
    if (str_name == "leds")
    {
        m_cLedsWrapper.m_pcLeds = (CCI_LEDsActuator *)pc_actu;
        m_cLedsWrapper.m_bLeds = true;
        //m_cLedsWrapper.m_pcLeds->Init(t_node);
    }
    if (str_name == "range_and_bearing")
    {
        m_cRangeAndBearingWrapper.m_pcRABA = (CCI_RangeAndBearingActuator *)pc_actu;
        m_cRangeAndBearingWrapper.m_bRABA = true;
        //m_cRangeAndBearingWrapper.m_pcRABA->Init(t_node);
    }
    if (str_name == "footbot_distance_scanner")
    {
        m_cDistanceScannerWrapper.m_pcScannerActuator = dynamic_cast<CCI_FootBotDistanceScannerActuator *>(pc_actu);
        m_cDistanceScannerWrapper.m_bScannerActuator = true;
        m_cDistanceScannerWrapper.m_pcScannerActuator->Init(t_node);
    }
    if (str_name == "footbot_turret")
    {
        m_cTurretWrapper.m_pcTurretActuator = dynamic_cast<CCI_FootBotTurretActuator *>(pc_actu);
        m_cTurretWrapper.m_bTurretActuator = true;
        m_cTurretWrapper.m_pcTurretActuator->Init(t_node);
    }
}

/****************************************/
/****************************************/	

// Initialize the sensor specified by the provided name.
void ActusensorsWrapper::CreateSensor(const std::string str_name, CCI_Sensor *pc_sensor, TConfigurationNode &t_node)
{
    if (str_name == "footbot_proximity")
    {
        m_cProximitySensorWrapper.m_pcProximity = (CCI_FootBotProximitySensor *)pc_sensor;
        m_cProximitySensorWrapper.m_bProximity = true;
        m_cProximitySensorWrapper.m_pcProximity->Init(t_node);
    }

    if (str_name == "colored_blob_omnidirectional_camera")
    {
        m_cOmnidirectionalCameraWrapper.m_pcOmniCam = (CCI_ColoredBlobOmnidirectionalCameraSensor *)pc_sensor;
        m_cOmnidirectionalCameraWrapper.m_bOmniCam = true;
        //m_cOmnidirectionalCameraWrapper.m_pcOmniCam->Init(t_node);
    }
    if (str_name == "range_and_bearing")
    {
        m_cRangeAndBearingWrapper.m_pcRABS = (CCI_RangeAndBearingSensor *)pc_sensor;
        m_cRangeAndBearingWrapper.m_bRABS = true;
        //m_cRangeAndBearingWrapper.m_pcRABS->Init(t_node);
    }

    if (str_name == "footbot_motor_ground")
    {
        m_cGroundSensorWrapper.m_pcGround = (CCI_FootBotMotorGroundSensor *)pc_sensor;
        m_cGroundSensorWrapper.m_bGround = true;
        m_cGroundSensorWrapper.m_pcGround->Init(t_node);
    }

    if (str_name == "footbot_base_ground")
    {
        m_cBaseGroundSensorWrapper.m_pcBaseGround = dynamic_cast<CCI_FootBotBaseGroundSensor *>(pc_sensor);
        m_cBaseGroundSensorWrapper.m_bBaseGround = true;
        m_cBaseGroundSensorWrapper.m_pcBaseGround->Init(t_node);
    }

    if (str_name == "footbot_light")
    {
        m_cLightSensorWrapper.m_pcLight = (CCI_FootBotLightSensor *)pc_sensor;
        m_cLightSensorWrapper.m_bLight = true;
        m_cLightSensorWrapper.m_pcLight->Init(t_node);
    }
    if (str_name == "footbot_distance_scanner")
    {
        m_cDistanceScannerWrapper.m_pcScannerSensor = (CCI_FootBotDistanceScannerSensor *)pc_sensor;
        m_cDistanceScannerWrapper.m_bScannerSensor = true;
        m_cDistanceScannerWrapper.m_pcScannerSensor->Init(t_node);
    }
    if (str_name == "footbot_turret_encoder")
    {
        m_cTurretWrapper.m_pcTurretSensor = dynamic_cast<CCI_FootBotTurretEncoderSensor *>(pc_sensor);
        m_cTurretWrapper.m_bTurretSensor = true;
        m_cTurretWrapper.m_pcTurretSensor->Init(t_node);
    }
}

BOOST_PYTHON_MODULE(libpy_controller_interface)
{
    // Export the main "robot" class, and define the various actuators and sensors as property of the class.
    class_<ActusensorsWrapper, boost::shared_ptr<ActusensorsWrapper>, boost::noncopyable>("robot", no_init)
         .def("logprint", &ActusensorsWrapper::Logprint)
        .staticmethod("logprint")
        .add_property("wheels", &ActusensorsWrapper::m_cWheelsWrapper)
        .add_property("colored_blob_omnidirectional_camera", &ActusensorsWrapper::m_cOmnidirectionalCameraWrapper)
        .add_property("proximity", &ActusensorsWrapper::m_cProximitySensorWrapper)
        .add_property("leds", &ActusensorsWrapper::m_cLedsWrapper)
        .add_property("range_and_bearing", &ActusensorsWrapper::m_cRangeAndBearingWrapper)
        .add_property("motor_ground", &ActusensorsWrapper::m_cGroundSensorWrapper)
        .add_property("base_ground", &ActusensorsWrapper::m_cBaseGroundSensorWrapper)
        .add_property("gripper", &ActusensorsWrapper::m_cGripperWrapper)
        .add_property("light_sensor", &ActusensorsWrapper::m_cLightSensorWrapper)
        .add_property("distance_scanner", &ActusensorsWrapper::m_cDistanceScannerWrapper)
        .add_property("turret", &ActusensorsWrapper::m_cTurretWrapper);

    // Export "WheelsWrapper", wrapper of CCI_DifferentialSteeringActuator.
    class_<ActusensorsWrapper::WheelsWrapper, boost::noncopyable>("wheels_wrapper_wrapper", no_init)
        .def("set_speed", &ActusensorsWrapper::WheelsWrapper::SetSpeed);

    // Export "GripperWrapper", wrapper of CFootBotGripping.
    class_<ActusensorsWrapper::GripperWrapper, boost::noncopyable>("m_cGripperWrapper", no_init)
        .def("lock", &ActusensorsWrapper::GripperWrapper::Lock)
        .def("unlock", &ActusensorsWrapper::GripperWrapper::Unlock);

    // Export "OmnidirectionalCameraWrapper" , wrapper of CCI_ColoredBlobOmnidirectionalCameraSensor.
    class_<ActusensorsWrapper::OmnidirectionalCameraWrapper, boost::noncopyable>("omnidirectional_camera_wrapper_wrapper", no_init)
        .def("enable", &ActusensorsWrapper::OmnidirectionalCameraWrapper::Enable)
        .def("disable", &ActusensorsWrapper::OmnidirectionalCameraWrapper::Disable)
        .def("get_readings", &ActusensorsWrapper::OmnidirectionalCameraWrapper::GetReadings)
        .def("get_counter", &ActusensorsWrapper::OmnidirectionalCameraWrapper::GetCounter);

    // Export "FootBotProximitySensorWrapper", wrapper of CCI_FootBotProximitySensor.
    class_<ActusensorsWrapper::FootBotProximitySensorWrapper, boost::noncopyable>("footbot_proximity_sensor_wrapper", no_init)
        .def("get_readings", &ActusensorsWrapper::FootBotProximitySensorWrapper::GetReadings);

    // Export "LedsActuatorWrapper", wrapper of CCI_LEDsActuator.
    class_<ActusensorsWrapper::LedsActuatorWrapper, boost::noncopyable>("leds_actuator_wrapper", no_init)
        .def("set_single_color", &ActusensorsWrapper::LedsActuatorWrapper::SetSingleColorString)
        .def("set_single_color", &ActusensorsWrapper::LedsActuatorWrapper::SetSingleColorRGB)
        .def("set_all_colors", &ActusensorsWrapper::LedsActuatorWrapper::SetAllColorsString)
        .def("set_all_colors", &ActusensorsWrapper::LedsActuatorWrapper::SetAllColorsRGB);

    // Export RangeAndBearingWrapper, wrapper of CCI_RangeAndBearingActuator and CCI_RangeAndBearingSensor.
    class_<ActusensorsWrapper::RangeAndBearingWrapper, boost::noncopyable>("m_cRangeAndBearingWrapper", no_init)
        .def("clear_data", &ActusensorsWrapper::RangeAndBearingWrapper::ClearData)
        .def("set_data", &ActusensorsWrapper::RangeAndBearingWrapper::SetData)
        .def("get_readings", &ActusensorsWrapper::RangeAndBearingWrapper::GetReadings);

    // Export GroundSensorWrapper, wrapper of CCI_FootBotMotorGroundSensor.
    class_<ActusensorsWrapper::GroundSensorWrapper, boost::noncopyable>("m_cGroundSensorWrapper", no_init)
        .def("get_readings", &ActusensorsWrapper::GroundSensorWrapper::GetReadings);

    // Export BaseGroundSensorWrapper, wrapper of CCI_FootBotBaseGroundSensor .
    class_<ActusensorsWrapper::BaseGroundSensorWrapper, boost::noncopyable>("m_cGroundSensorWrapper", no_init)
        .def("get_readings", &ActusensorsWrapper::BaseGroundSensorWrapper::GetReadings);

    // Export LightSensorWrapper, wrapper of CCI_FootBotLightSensor.
    class_<ActusensorsWrapper::LightSensorWrapper, boost::noncopyable>("m_cLightSensorWrapper", no_init)
        .def("get_readings", &ActusensorsWrapper::LightSensorWrapper::GetReadings);

    // Export DistanceScannerWrapper, wrapper of CCI_FootBotDistanceScannerActuator and CCI_FootBotDistanceScannerSensor.
    class_<ActusensorsWrapper::DistanceScannerWrapper, boost::noncopyable>("m_cDistanceScannerWrapper", no_init)
        .def("enable", &ActusensorsWrapper::DistanceScannerWrapper::Enable)
        .def("disable", &ActusensorsWrapper::DistanceScannerWrapper::Disable)
        .def("set_rpm", &ActusensorsWrapper::DistanceScannerWrapper::SetRPM)
        .def("set_angle", &ActusensorsWrapper::DistanceScannerWrapper::SetAngle)
        .def("get_readings", &ActusensorsWrapper::DistanceScannerWrapper::GetReadings)
        .def("get_short_readings", &ActusensorsWrapper::DistanceScannerWrapper::GetShortReadings)
        .def("get_long_readings", &ActusensorsWrapper::DistanceScannerWrapper::GetLongReadings);

    // Export TurretWrapper, wrapper of CCI_FootBotTurretEncoderSensor and CCI_FootBotTurretActuator. 
    class_<ActusensorsWrapper::TurretWrapper, boost::noncopyable>("m_cTurretWrapper", no_init)
        .def("get_rotation", &ActusensorsWrapper::TurretWrapper::GetRotation)
        .def("set_rotation", &ActusensorsWrapper::TurretWrapper::SetRotation)
        .def("set_rotation_speed", &ActusensorsWrapper::TurretWrapper::SetRotationSpeed)
        .def("set_mode", &ActusensorsWrapper::TurretWrapper::SetMode)
        .def("set_active_with_rotation", &ActusensorsWrapper::TurretWrapper::SetActiveWithRotation)
        .def("set_speed_control_mode", &ActusensorsWrapper::TurretWrapper::SetSpeedControlMode)
        .def("set_position_control_mode", &ActusensorsWrapper::TurretWrapper::SetPositionControlMode)
        .def("set_passive_mode", &ActusensorsWrapper::TurretWrapper::SetPassiveMode);

    // Export a CRadians - float map, used for the readings of the Distance Scanner Sensor.
    // The map can be iterated. For a given map entry, get the key with map_entry.key()
    // (map_entry.key().value() to get the actual angle value),
    // and the value with map_entry.data()
    class_<std::map<argos::CRadians, Real>>("distance_sensor_readings_map")
        .def(boost::python::map_indexing_suite<std::map<argos::CRadians, Real>>());

    // Export the CRadians class, used by the readings of various sensors.
    // TODO: add more properties
    class_<argos::CRadians>("radians", init<>())
        .def(init<Real>())
        .def("value", &argos::CRadians::GetValue);

    // Export the SReading class, used to store the readings of the proximity sensor.
    class_<argos::CCI_FootBotProximitySensor::SReading>("proximity_reading", no_init)
        .def_readonly("value", &argos::CCI_FootBotProximitySensor::SReading::Value)
        .def_readonly("angle", &argos::CCI_FootBotProximitySensor::SReading::Angle);

    // Export the SReading class, used to store the readings of the light sensor.
    class_<argos::CCI_FootBotLightSensor::SReading>("proximity_reading", no_init)
        .def_readonly("value", &argos::CCI_FootBotLightSensor::SReading::Value)
        .def_readonly("angle", &argos::CCI_FootBotLightSensor::SReading::Angle);

    // Export the SBlob class, used to store the readings of the omnidirectiona camera.
    class_<argos::CCI_ColoredBlobOmnidirectionalCameraSensor::SBlob>("omnidirectional_camera_packet", no_init)
        .def_readonly("distance", &argos::CCI_ColoredBlobOmnidirectionalCameraSensor::SBlob::Distance)
        .def_readonly("angle", &argos::CCI_ColoredBlobOmnidirectionalCameraSensor::SBlob::Angle)
        .def_readonly("color", &argos::CCI_ColoredBlobOmnidirectionalCameraSensor::SBlob::Color);

    // Export the SPacket class, used to store the readings of the range and bearing sensor.
    class_<argos::CCI_RangeAndBearingSensor::SPacket>("range_and_bearing_packet", no_init)
        .add_property("range", &argos::CCI_RangeAndBearingSensor::SPacket::Range)
        .add_property("horizontal_bearing", &argos::CCI_RangeAndBearingSensor::SPacket::HorizontalBearing)
        .add_property("vertical_bearing", &argos::CCI_RangeAndBearingSensor::SPacket::VerticalBearing)
        .add_property("data", &argos::CCI_RangeAndBearingSensor::SPacket::Data);

    // Export the SReading class, used to store the readings of the motor ground sensor.
    // Each reading contains a value and the offset, expressed in x/y coordinates.
    class_<argos::CCI_FootBotMotorGroundSensor::SReading>("motor_ground_sensor_wrapper", no_init)
        .def_readonly("value", &argos::CCI_FootBotMotorGroundSensor::SReading::Value)
        .def_readonly("offset", &argos::CCI_FootBotMotorGroundSensor::SReading::Offset);

    // Export the SReading class, used to store the readings of the base ground sensor.
    // Each reading contains a value and the offset, expressed in x/y coordinates.
    class_<argos::CCI_FootBotBaseGroundSensor::SReading>("motor_ground_sensor_wrapper", no_init)
        .def_readonly("value", &argos::CCI_FootBotBaseGroundSensor::SReading::Value)
        .def_readonly("offset", &argos::CCI_FootBotBaseGroundSensor::SReading::Offset);

    class_<argos::CVector2>("vector2", init<>())
        .def(init<Real, Real>())
        .add_property("x", &argos::CVector2::GetX, &argos::CVector2::SetX)
        .add_property("y", &argos::CVector2::GetY, &argos::CVector2::SetY)
        .def_readonly("length", &argos::CVector2::Length)
        .def_readonly("angle", &argos::CVector2::Angle)
        .def_readonly("square_length", &argos::CVector2::SquareLength);

    // Export the ColorWrapper class.
    class_<ActusensorsWrapper::ColorWrapper>("color", init<std::string>())
        .def(init<UInt8, UInt8, UInt8>())
        .add_property("raw_color", &ActusensorsWrapper::ColorWrapper::m_cColor);
    // TODO: add other color stuff

    // Export the CColor class. This class is not directly usable in python,
    // but it is exported to simplify the usage of colors in python.
    class_<argos::CColor>("raw_color", no_init)
        .def("__eq__", &argos::CColor::operator==);

    // Export the CByteArray class, and define new functions on it to access its data.
    class_<argos::CByteArray>("c_byte_array", no_init)
        .def("__getitem__", &ActusensorsWrapper::CByteArrayGetItem)
        .def("__setitem__", &ActusensorsWrapper::CByteArraySetItem);
}
