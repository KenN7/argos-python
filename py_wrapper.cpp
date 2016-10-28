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

void ActusensorsWrapper::logprint(std::string message)
{
  std::cout << message << std::endl;
}

// Initialize the actuator specified by the provided name.

// NOTE: the Init function gives problems for all the sensors/actuators that use the "media" parameter.
// When the argos file is loaded, the following error is given:
//    [FATAL] Error parsing attribute "medium"
//    Attribute does not exist <ticpp.h@1791>
// The sensors/actuators seem to work even without it, so for now it has been disabled.
void ActusensorsWrapper::CreateActu(std::string name, CCI_Actuator *actu, TConfigurationNode &t_node)
{
  if (name == "differential_steering")
  {
    wheels_wrapper.m_pcWheels = (CCI_DifferentialSteeringActuator *)actu;
    wheels_wrapper.m_pcWheels_bool = true;
    wheels_wrapper.m_pcWheels->Init(t_node);
  }
  if (name == "footbot_gripper")
  {
    gripper_wrapper.m_pcGripper = dynamic_cast<CCI_FootBotGripperActuator *>(actu);
    gripper_wrapper.m_pcGripper_bool = true;
    gripper_wrapper.m_pcGripper->Init(t_node);
  }
  if (name == "leds")
  {
    leds_wrapper.m_pcLeds = (CCI_LEDsActuator *)actu;
    leds_wrapper.m_pcLeds_bool = true;
    //leds_wrapper.m_pcLeds->Init(t_node);
  }
  if (name == "range_and_bearing")
  {
    range_and_bearing_wrapper.m_pcRABA = (CCI_RangeAndBearingActuator *)actu;
    range_and_bearing_wrapper.m_pcRABA_bool = true;
    //range_and_bearing_wrapper.m_pcRABA->Init(t_node);
  }
}

// Initialize the sensor specified by the provided name.
void ActusensorsWrapper::CreateSensor(std::string name, CCI_Sensor *sensor, TConfigurationNode &t_node)
{
  if (name == "footbot_proximity")
  {
    proximity_sensor_wrapper.m_pcProximity = (CCI_FootBotProximitySensor *)sensor;
    proximity_sensor_wrapper.m_pcProximity_bool = true;
    proximity_sensor_wrapper.m_pcProximity->Init(t_node);
  }

  if (name == "colored_blob_omnidirectional_camera")
  {
    omnidirectional_camera_wrapper.m_pcOmniCam = (CCI_ColoredBlobOmnidirectionalCameraSensor *)sensor;
    omnidirectional_camera_wrapper.m_pcOmniCam_bool = true;
    //omnidirectional_camera_wrapper.m_pcOmniCam->Init(t_node);
  }
  if (name == "range_and_bearing")
  {
    range_and_bearing_wrapper.m_pcRABS = (CCI_RangeAndBearingSensor *)sensor;
    range_and_bearing_wrapper.m_pcRABS_bool = true;
    //range_and_bearing_wrapper.m_pcRABS->Init(t_node);
  }

  if (name == "footbot_motor_ground")
  {
    ground_sensor_wrapper.m_pcGround = (CCI_FootBotMotorGroundSensor *)sensor;
    ground_sensor_wrapper.m_pcGround_bool = true;
    ground_sensor_wrapper.m_pcGround->Init(t_node);
  }

  if (name == "footbot_base_ground")
  {
    base_ground_sensor_wrapper.m_pcBaseGround = dynamic_cast<CCI_FootBotBaseGroundSensor *>(sensor);
    base_ground_sensor_wrapper.m_pcBaseGround_bool = true;
    base_ground_sensor_wrapper.m_pcBaseGround->Init(t_node);
  }

  if (name == "footbot_light")
  {
    light_sensor_wrapper.m_pcLight = (CCI_FootBotLightSensor *)sensor;
    light_sensor_wrapper.m_pcLight_bool = true;
    light_sensor_wrapper.m_pcLight->Init(t_node);
  }
}

BOOST_PYTHON_MODULE(libpy_controller_interface)
{
  // Export the main "robot" class, and define the various actuators and sensors as property of the class.
  class_<ActusensorsWrapper, boost::shared_ptr<ActusensorsWrapper>, boost::noncopyable>("robot", no_init)
      .def("logprint", &ActusensorsWrapper::logprint)
      .staticmethod("logprint")
      .add_property("wheels", &ActusensorsWrapper::wheels_wrapper)
      .add_property("colored_blob_omnidirectional_camera", &ActusensorsWrapper::omnidirectional_camera_wrapper)
      .add_property("proximity", &ActusensorsWrapper::proximity_sensor_wrapper)
      .add_property("leds", &ActusensorsWrapper::leds_wrapper)
      .add_property("range_and_bearing", &ActusensorsWrapper::range_and_bearing_wrapper)
      .add_property("motor_ground", &ActusensorsWrapper::ground_sensor_wrapper)
      .add_property("base_ground", &ActusensorsWrapper::base_ground_sensor_wrapper)
      .add_property("gripper", &ActusensorsWrapper::gripper_wrapper)
      .add_property("light_sensor", &ActusensorsWrapper::light_sensor_wrapper);

  // Export "WheelsWrapper", wrapper of CCI_DifferentialSteeringActuator.
  class_<ActusensorsWrapper::WheelsWrapper, boost::noncopyable>("wheels_wrapper_wrapper", no_init)
      .def("set_speed", &ActusensorsWrapper::WheelsWrapper::set_speed);

  // Export "GripperWrapper", wrapper of CFootBotGripping.
  class_<ActusensorsWrapper::GripperWrapper, boost::noncopyable>("gripper_wrapper", no_init)
      .def("lock", &ActusensorsWrapper::GripperWrapper::Lock)
      .def("unlock", &ActusensorsWrapper::GripperWrapper::Unlock);

  // Export "OmnidirectionalCameraWrapper" , wrapper of CCI_ColoredBlobOmnidirectionalCameraSensor.
  class_<ActusensorsWrapper::OmnidirectionalCameraWrapper, boost::noncopyable>("omnidirectional_camera_wrapper_wrapper", no_init)
      .def("enable", &ActusensorsWrapper::OmnidirectionalCameraWrapper::Enable)
      .def("disable", &ActusensorsWrapper::OmnidirectionalCameraWrapper::Disable)
      .def("get_readings", &ActusensorsWrapper::OmnidirectionalCameraWrapper::GetReadings)
      .def("get_counter", &ActusensorsWrapper::OmnidirectionalCameraWrapper::get_counter);

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
  class_<ActusensorsWrapper::RangeAndBearingWrapper, boost::noncopyable>("range_and_bearing_wrapper", no_init)
      .def("clear_data", &ActusensorsWrapper::RangeAndBearingWrapper::ClearData)
      .def("set_data", &ActusensorsWrapper::RangeAndBearingWrapper::SetData)
      .def("get_readings", &ActusensorsWrapper::RangeAndBearingWrapper::GetReadings);

  // Export GroundSensorWrapper, wrapper of CCI_FootBotMotorGroundSensor.
  class_<ActusensorsWrapper::GroundSensorWrapper, boost::noncopyable>("ground_sensor_wrapper", no_init)
      .def("get_readings", &ActusensorsWrapper::GroundSensorWrapper::GetReadings);

  // Export BaseGroundSensorWrapper, wrapper of CCI_FootBotBaseGroundSensor .
  class_<ActusensorsWrapper::BaseGroundSensorWrapper, boost::noncopyable>("ground_sensor_wrapper", no_init)
      .def("get_readings", &ActusensorsWrapper::BaseGroundSensorWrapper::GetReadings);

  // Export LightSensorWrapper, wrapper of CCI_FootBotLightSensor.
  class_<ActusensorsWrapper::LightSensorWrapper, boost::noncopyable>("light_sensor_wrapper", no_init)
      .def("get_readings", &ActusensorsWrapper::LightSensorWrapper::GetReadings);

  // Export the CRadians class, used by the readings of various sensors.
  // TODO: add more properties
  class_<argos::CRadians>("radians", init<>())
      .def(init<double>())
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
      .def(init<double, double>())
      .add_property("x", &argos::CVector2::GetX, &argos::CVector2::SetX)
      .add_property("y", &argos::CVector2::GetY, &argos::CVector2::SetY)
      .def_readonly("length", &argos::CVector2::Length)
      .def_readonly("angle", &argos::CVector2::Angle)
      .def_readonly("square_length", &argos::CVector2::SquareLength);

  // Export the ColorWrapper class.
  class_<ActusensorsWrapper::ColorWrapper>("color", init<std::string>())
      .def(init<int, int, int>())
      .add_property("raw_color", &ActusensorsWrapper::ColorWrapper::color);
  // TODO: add other color stuff

  // Export the CColor class. This class is not directly usable in python,
  // but it is exported to simplify the usage of colors in python.
  class_<argos::CColor>("raw_color", no_init)
      .def("__eq__", &argos::CColor::operator==);

  // Export the CByteArray class, and define new functions on it to access its data.
  class_<argos::CByteArray>("c_byte_array", no_init)
      .def("__getitem__", &ActusensorsWrapper::c_byte_array_getitem)
      .def("__setitem__", &ActusensorsWrapper::c_byte_array_setitem);
}
