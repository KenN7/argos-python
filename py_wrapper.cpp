/*
 * AUTHOR: Ken Hasselmann <arg AT kenh DOT fr>
 *
 * Connects ARGoS to python
 *
 */
#include "py_wrapper.h"

using namespace argos;
using namespace boost::python;

ActusensorsWrapper::ActusensorsWrapper() //: m_pcWheels(NULL),
                                         //m_pcWheels_bool(false),
                                         //m_pcProximity(NULL),
                                         //m_pcProximity_bool(false)
                                         //m_pcOmniCam(NULL),
                                         //m_pcOmniCam_bool(false)
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

// boost::python::list ActusensorsWrapper::proximity()
// {
//   if (m_pcProximity_bool)
//   {
//     const CCI_FootBotProximitySensor::TReadings &tProxReads = m_pcProximity->GetReadings();
//     return ToPythonList(tProxReads);
//   }
//   else
//   {
//     logprint("Proximity sensor not implemented or not stated in XML config.");
//     // TODO: add exception?
//   }
// }

// boost::python::list ActusensorsWrapper::get_blobs()
// {
//   return ToPythonList(m_pcOmniCam->GetReadings().BlobList);
// }

// argos::CCI_DifferentialSteeringActuator *ActusensorsWrapper::GetWheels()
// {
//   if (m_pcWheels_bool)
//   {
//     return m_pcWheels;
//   }
//   else
//   {
//     logprint("Differential Steering Actuator not implemented or not stated in XML config.");
//   }
// }

// argos::CCI_ColoredBlobOmnidirectionalCameraSensor *ActusensorsWrapper::GetOmniDirectionalCamera()
// {
//   if (m_pcOmniCam_bool)
//   {
//     return m_pcOmniCam;
//   }
//   else
//   {
//     logprint("Omnidirectional Camera Sensor not implemented or not stated in XML config.");
//   }
// }

//those 2 following functions serve as wrappers for Actuators and Sensors
//the if are based on the options of the XML
//we could add for ex :
//actuators/sensors
//CCI_DifferentialSteeringActuator* m_pcWheels = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
//m_pcProximity = GetSensor<CCI_FootBotProximitySensor>("footbot_proximity");
//m_pcOmniCam = GetSensor<CCI_ColoredBlobOmnidirectionalCameraSensor>("colored_blob_omnidirectional_camera");

void ActusensorsWrapper::CreateActu(std::string name, CCI_Actuator *actu, TConfigurationNode &t_node)
{
  if (name == "differential_steering")
  {
    wheels_wrapper.m_pcWheels = (CCI_DifferentialSteeringActuator *)actu;
    wheels_wrapper.m_pcWheels_bool = true;
    wheels_wrapper.m_pcWheels->Init(t_node);
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
}

void ActusensorsWrapper::vector_setitem(argos::CByteArray &v, int index, UInt8 value)
{
  if (index >= 0 && index < v.Size())
  {
    v[index] = value;
  }
  else
  {
    PyErr_SetString(PyExc_IndexError, "index out of range");
    throw_error_already_set();
  }
}

UInt8 ActusensorsWrapper::vector_getitem(argos::CByteArray &v, int index)
{
  if (index >= 0 && index < v.Size())
  {
    return v[index];
  }
  else
  {
    PyErr_SetString(PyExc_IndexError, "index out of range");
    throw_error_already_set();
  }
}

BOOST_PYTHON_MODULE(libpy_controller_interface)
{
  class_<ActusensorsWrapper, boost::shared_ptr<ActusensorsWrapper>, boost::noncopyable>("robot", no_init)
      .def("logprint", &ActusensorsWrapper::logprint)
      .staticmethod("logprint")
      //.def("proximity", &ActusensorsWrapper::proximity)
      //.def("wheels", &ActusensorsWrapper::GetWheels,
      //     return_internal_reference<>())
      //.def("colored_blob_omnidirectional_camera", &ActusensorsWrapper::GetOmniDirectionalCamera,
      //     return_internal_reference<>())
      //.def("get_blobs", &ActusensorsWrapper::get_blobs)
      .add_property("wheels", &ActusensorsWrapper::wheels_wrapper)
      .add_property("colored_blob_omnidirectional_camera", &ActusensorsWrapper::omnidirectional_camera_wrapper)
      .add_property("proximity", &ActusensorsWrapper::proximity_sensor_wrapper)
      .add_property("leds", &ActusensorsWrapper::leds_wrapper)
      .add_property("range_and_bearing", &ActusensorsWrapper::range_and_bearing_wrapper);

  class_<ActusensorsWrapper::WheelsWrapper, boost::noncopyable>("wheels_wrapper_wrapper", no_init)
      .def("set_speed", &ActusensorsWrapper::WheelsWrapper::set_speed);

  class_<ActusensorsWrapper::OmnidirectionalCameraWrapper, boost::noncopyable>("omnidirectional_camera_wrapper_wrapper", no_init)
      .def("enable", &ActusensorsWrapper::OmnidirectionalCameraWrapper::Enable)
      .def("disable", &ActusensorsWrapper::OmnidirectionalCameraWrapper::Disable)
      .def("get_readings", &ActusensorsWrapper::OmnidirectionalCameraWrapper::GetReadings)
      .def("get_counter", &ActusensorsWrapper::OmnidirectionalCameraWrapper::get_counter);

  class_<ActusensorsWrapper::FootBotProximitySensorWrapper, boost::noncopyable>("footbot_proximity_sensor_wrapper", no_init)
      .def("get_readings", &ActusensorsWrapper::FootBotProximitySensorWrapper::GetReadings);

  class_<ActusensorsWrapper::LedsActuatorWrapper, boost::noncopyable>("leds_actuator_wrapper", no_init)
      .def("set_single_color", &ActusensorsWrapper::LedsActuatorWrapper::SetSingleColorString)
      .def("set_single_color", &ActusensorsWrapper::LedsActuatorWrapper::SetSingleColorRGB)
      .def("set_all_colors", &ActusensorsWrapper::LedsActuatorWrapper::SetAllColorsString)
      .def("set_all_colors", &ActusensorsWrapper::LedsActuatorWrapper::SetAllColorsRGB);

  class_<ActusensorsWrapper::RangeAndBearingWrapper, boost::noncopyable>("range_and_bearing_wrapper", no_init)
      .def("clear_data", &ActusensorsWrapper::RangeAndBearingWrapper::ClearData)
      .def("set_data", &ActusensorsWrapper::RangeAndBearingWrapper::SetData)
      .def("get_readings", &ActusensorsWrapper::RangeAndBearingWrapper::GetReadings);

  class_<argos::CRadians>("radians", init<>())
      .def(init<double>())
      .def("value", &argos::CRadians::GetValue);

  class_<argos::CCI_FootBotProximitySensor::SReading>("proximity_reading", no_init)
      .def_readonly("value", &argos::CCI_FootBotProximitySensor::SReading::Value)
      .def_readonly("angle", &argos::CCI_FootBotProximitySensor::SReading::Angle);

  // class_<argos::CCI_DifferentialSteeringActuator, boost::noncopyable>("wheels_actuator", no_init)
  //     .def("set_speed", &argos::CCI_DifferentialSteeringActuator::SetLinearVelocity);

  // class_<argos::CCI_ColoredBlobOmnidirectionalCameraSensor, boost::noncopyable>("omnidirectional_camera_sensor", no_init)
  //     .def("enable", &argos::CCI_ColoredBlobOmnidirectionalCameraSensor::Enable)
  //     .def("disable", &argos::CCI_ColoredBlobOmnidirectionalCameraSensor::Disable)
  //     .def("get_readings", &argos::CCI_ColoredBlobOmnidirectionalCameraSensor::GetReadings,
  //          return_internal_reference<>());

  // class_<argos::CCI_ColoredBlobOmnidirectionalCameraSensor::SReadings>("omnidirectional_camera_sensor_readings", no_init)
  //     .def_readonly("counter", &argos::CCI_ColoredBlobOmnidirectionalCameraSensor::SReadings::Counter);

  class_<argos::CCI_ColoredBlobOmnidirectionalCameraSensor::SBlob>("blob", no_init)
      .def_readonly("distance", &argos::CCI_ColoredBlobOmnidirectionalCameraSensor::SBlob::Distance)
      .def_readonly("angle", &argos::CCI_ColoredBlobOmnidirectionalCameraSensor::SBlob::Angle)
      .def_readonly("color", &argos::CCI_ColoredBlobOmnidirectionalCameraSensor::SBlob::Color);

  class_<ActusensorsWrapper::ColorWrapper>("color", init<std::string>())
      .def(init<int, int, int>())
      .add_property("raw_color", &ActusensorsWrapper::ColorWrapper::color);
  // TODO: add other color stuff
  class_<argos::CColor>("raw_color", no_init);

  class_<argos::CCI_RangeAndBearingSensor::SPacket>("range_and_bearing_packet", no_init)
      .add_property("range", &argos::CCI_RangeAndBearingSensor::SPacket::Range)
      .add_property("horizontal_bearing", &argos::CCI_RangeAndBearingSensor::SPacket::HorizontalBearing)
      .add_property("vertical_bearing", &argos::CCI_RangeAndBearingSensor::SPacket::VerticalBearing)
      .add_property("data", &argos::CCI_RangeAndBearingSensor::SPacket::Data);

  // class_<argos::CByteArray>("c_byte_array", no_init)
  //   .def("__get_item__", &argos::CByteArray::operator[])
  //   .def("__set_item__", &argos::CByteArray::operator[]);

  class_<argos::CByteArray>("c_byte_array", no_init)
      .def("__getitem__", &ActusensorsWrapper::vector_getitem)
      .def("__setitem__", &ActusensorsWrapper::vector_setitem);
}
