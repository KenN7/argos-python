#include "py_environment_wrapper.h"
#include "py_wrapper.h"

using namespace argos;

/****************************************/
/****************************************/

CQTOpenGLUserFunctionsWrapper::CQTOpenGLUserFunctionsWrapper() {}

// Add bool fill, color, number of vertices inputs
// const boost::python::list c_position_list, const boost::python::list c_orientation_list, const Real f_radius
void CQTOpenGLUserFunctionsWrapper::DrawCircle() {

//     extract python list for position

//     // Create Cvector3 with position
//     // c_position = convert c_position_list
//     // Create Cvector3 with orientation
//     // c_orientation = convert c_orientation_list
//     // Do arrow thingy
 m_pcCQTOpenGLUserFunctions->DrawCircle(CVector3(0.0f, 0.0f, 0.01f), CQuaternion(), 0.1, CColor::RED);

    // std::cout << "test:" << CVector3(boost::python::extract<UInt8>(boost::python::object(c_position_list[0])), boost::python::extract<UInt8>(boost::python::object(c_position_list[1])), boost::python::extract<UInt8>(boost::python::object(c_position_list[2]))) << std::endl;
    // std::cout << "test2:" << f_radius << std::endl;
    // m_pcCLoopFunctions->DrawCylinder(CVector3(0.0f, 0.0f, 0.3f), CQuaternion(), 0.1f,  0.05f, CColor::BLACK);
//     m_pcCLoopFunctions->DrawInWorld() {
//         DrawCircle(CVector3(0.5, 0.5, 0.5), CQuaternion(), 0.5);
     // std::cout << "test2:" << std::endl;

// };
}

/****************************************/
/****************************************/