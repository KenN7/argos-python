/*
 * AUTHOR: Ken Hasselmann <arg AT kenh DOT fr>
 *
 * Connects ARGoS to python
 *
 */

#ifndef PY_CONTROLLER_H
#define PY_CONTROLLER_H

#include <boost/python.hpp>

#include <argos3/core/control_interface/ci_controller.h>
// #include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
// #include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
// #include <argos3/plugins/robots/generic/control_interface/ci_colored_blob_omnidirectional_camera_sensor.h>
// #include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_gripper_actuator.h>

#include <argos3/core/utility/logging/argos_log.h>

#include <string>
#include <iostream>

namespace argos {

class CPyController : public CCI_Controller {

public:

  CPyController();
  virtual ~CPyController() {}

  /*
   * This function initializes the controller.
   * The 't_node' variable points to the <parameters> section in the XML
   * file in the <controllers><footbot_ccw_wander_controller> section.
   */
  virtual void Init(TConfigurationNode& t_node);

  /*
   * This function is called once every time step.
   * The length of the time step is set in the XML file.
   */
  virtual void ControlStep();

  /*
   * This function resets the controller to its state right after the
   * Init().
   * It is called when you press the reset button in the GUI.
   * In this example controller there is no need for resetting anything,
   * so the function could have been omitted. It's here just for
   * completeness.
   */
  virtual void Reset();

  /*
   * Called to cleanup what done by Init() when the experiment finishes.
   * In this example controller there is no need for clean anything up,
   * so the function could have been omitted. It's here just for
   * completeness.
   */
  virtual void Destroy();

  void InitSensorsActuators();

  std::string GetId_2();

private:

  //CCI_DifferentialSteeringActuator* m_pcWheels;
  //CCI_FootBotProximitySensor* m_pcProximity;
  //CCI_ColoredBlobOmnidirectionalCameraSensor* m_pcOmniCam;

  boost::python::object m_main;
  boost::python::object m_namesp;
  boost::python::object m_script;

  boost::python::dict m_sensors_dict;
  boost::python::dict m_actuators_dict;

  int m_state;
  int m_id;
};

}

#endif
