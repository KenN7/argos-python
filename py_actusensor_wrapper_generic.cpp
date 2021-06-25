#include "py_actusensor_wrapper_generic.h"
#include "py_wrapper.h"

using namespace argos;

/****************************************/
/****************************************/

CWheelsWrapper::CWheelsWrapper()
{
}

void CWheelsWrapper::SetSpeed(const Real f_left_wheel_speed, const Real f_right_wheel_speed)
{
    if (m_pcWheels == nullptr)
    {
        ActusensorsWrapper::Logprint("Differential Steering Actuator not implemented or not stated in XML config.");
        return;
    }
    m_pcWheels->SetLinearVelocity(f_left_wheel_speed, f_right_wheel_speed);
}

/****************************************/
/****************************************/

COmnidirectionalCameraWrapper::COmnidirectionalCameraWrapper()
{
}

boost::python::list COmnidirectionalCameraWrapper::GetReadings() const
{
    if (m_pcOmniCam == nullptr)
    {
        ActusensorsWrapper::Logprint("Omnidirectional Camera Sensor not implemented or not stated in XML config.");
        return {};
    }
    return ActusensorsWrapper::ToPythonList(m_pcOmniCam->GetReadings().BlobList);
}
// Enable the camera.
void COmnidirectionalCameraWrapper::Enable()
{
    if (m_pcOmniCam == nullptr)
    {
        ActusensorsWrapper::Logprint("Omnidirectional Camera Sensor not implemented or not stated in XML config.");
        return ;
    }
    m_pcOmniCam->Enable();
}
// Disable the camera.
void COmnidirectionalCameraWrapper::Disable()
{
    if (m_pcOmniCam == nullptr)
    {
        ActusensorsWrapper::Logprint("Omnidirectional Camera Sensor not implemented or not stated in XML config.");
        return;
    }
    m_pcOmniCam->Disable();
}
// Return the number of readings obtained so far, i.e. the number of control steps from which the recording started.
const int COmnidirectionalCameraWrapper::GetCounter() const
{
    if (m_pcOmniCam == nullptr)
    {
        ActusensorsWrapper::Logprint("Omnidirectional Camera Sensor not implemented or not stated in XML config.");
        return {};
    }
    return m_pcOmniCam->GetReadings().Counter;
}

/****************************************/
/****************************************/

CRangeAndBearingWrapper::CRangeAndBearingWrapper()
{
}

void CRangeAndBearingWrapper::ClearData()
{
    if (m_pcRABA == nullptr)
    {
        ActusensorsWrapper::Logprint("RABA not implemented or not stated in XML config.");
        return;
    }
    m_pcRABA->ClearData();
}
// Set the i-th bit of the data table.
void CRangeAndBearingWrapper::SetData(const size_t un_idx, const UInt8 un_value)
{
    if (m_pcRABA == nullptr)
    {
        ActusensorsWrapper::Logprint("RABA not implemented or not stated in XML config.");
        return;
    }
    m_pcRABA->SetData(un_idx, un_value);
}
// TODO: Set all bits at once
// Return the readings obtained at this control step.
// Each reading contains the range, the horizontal bearing, the vertical bearing and the data table.
// The data table is exposed as a c_byte_array.
boost::python::list CRangeAndBearingWrapper::GetReadings() const
{
    if (m_pcRABS == nullptr)
    {
        ActusensorsWrapper::Logprint("RABA not implemented or not stated in XML config.");
        return {};
    }
    return ActusensorsWrapper::ToPythonList(m_pcRABS->GetReadings());
}

/****************************************/
/****************************************/

CLedsActuatorWrapper::CLedsActuatorWrapper()
{
}

// Set the color of a given led, given its name.
void CLedsActuatorWrapper::SetSingleColorString(const UInt8 un_led_id, const std::string str_color_name)
{
    if (m_pcLeds == nullptr) {
        ActusensorsWrapper::Logprint("Leds not implemented or not stated in XML config.");
        return;
    }
    m_pcLeds->SetSingleColor(un_led_id, ActusensorsWrapper::CColorWrapper(str_color_name).m_cColor);
}
// Set the color of a given led, given its RGB values.
void CLedsActuatorWrapper::SetSingleColorRGB(const UInt8 un_led_id, const UInt8 un_red, const UInt8 un_green, const UInt8 un_blue)
{
    if (m_pcLeds == nullptr) {
        ActusensorsWrapper::Logprint("Leds not implemented or not stated in XML config.");
        return;
    }
    m_pcLeds->SetSingleColor(un_led_id, ActusensorsWrapper::CColorWrapper(un_red, un_green, un_blue).m_cColor);
}
// Set the color of every led, given its name.
void CLedsActuatorWrapper::SetAllColorsString(const std::string str_color_name)
{
    if (m_pcLeds == nullptr) {
        ActusensorsWrapper::Logprint("Leds not implemented or not stated in XML config.");
        return;
    }
    m_pcLeds->SetAllColors(ActusensorsWrapper::CColorWrapper(str_color_name).m_cColor);
}
// Set the color of every led, given its RGB values.
void CLedsActuatorWrapper::SetAllColorsRGB(const UInt8 un_red, const UInt8 un_green, const UInt8 un_blue)
{
    if (m_pcLeds == nullptr) {
        ActusensorsWrapper::Logprint("Leds not implemented or not stated in XML config.");
        return;
    }
    m_pcLeds->SetAllColors(ActusensorsWrapper::CColorWrapper(un_red, un_green, un_blue).m_cColor);
}
