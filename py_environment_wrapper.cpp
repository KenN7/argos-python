#include "py_environment_wrapper.h"
#include "py_wrapper.h"

using namespace argos;

/****************************************/
/****************************************/

CQTOpenGLUserFunctionsWrapper::CQTOpenGLUserFunctionsWrapper() {}

void CQTOpenGLUserFunctionsWrapper::SetDrawList(const std::string& key, const std::string& value) {
  m_cAttributes[key] = value;
}

const std::string CQTOpenGLUserFunctionsWrapper::GetDrawList(const std::string& key) {
  return m_cAttributes[key];
}

// Add bool fill, color, number of vertices inputs
// const boost::python::list c_position_list, const boost::python::list c_orientation_list, const Real f_radius
void CQTOpenGLUserFunctionsWrapper::DrawCircle(const boost::python::list c_position_list, 
                                               const boost::python::list c_orientation_list, 
                                               const Real f_radius, 
                                               const std::string str_color_name,
                                               const bool  b_fill) {
    m_pcCQTOpenGLUserFunctions->DrawCircle(
        CVector3(boost::python::extract<Real>(boost::python::object(c_position_list[0])), 
                 boost::python::extract<Real>(boost::python::object(c_position_list[1])), 
                 boost::python::extract<Real>(boost::python::object(c_position_list[2]))),
        CQuaternion(), // Implement orientation
        f_radius, 
        ActusensorsWrapper::CColorWrapper(str_color_name).m_cColor,
        b_fill);
}

void CQTOpenGLUserFunctionsWrapper::DrawPolygon(const boost::python::list c_position_list, 
                                                const boost::python::list c_orientation_list, 
                                                const boost::python::list vec_points_list, 
                                                const std::string str_color_name,
                                                const bool  b_fill) {
  
    std::vector<CVector2> vec_point_vector;
    boost::python::list vec_point;

    for (size_t i = 0; i < boost::python::len(vec_points_list); ++i) {

            vec_point = boost::python::extract<boost::python::list>(boost::python::object(vec_points_list[i]));

            vec_point_vector.push_back(CVector2(boost::python::extract<Real>(boost::python::object(vec_point[0])),
                                                boost::python::extract<Real>(boost::python::object(vec_point[1]))));
        }

    m_pcCQTOpenGLUserFunctions->DrawPolygon(
        CVector3(boost::python::extract<Real>(boost::python::object(c_position_list[0])), 
                 boost::python::extract<Real>(boost::python::object(c_position_list[1])), 
                 boost::python::extract<Real>(boost::python::object(c_position_list[2]))),
        CQuaternion(), // Implement orientation
        vec_point_vector, 
        ActusensorsWrapper::CColorWrapper(str_color_name).m_cColor,
        b_fill);
}

void CQTOpenGLUserFunctionsWrapper::DrawRay(const boost::python::list c_start,
                                            const boost::python::list c_end,
                                            const std::string str_color_name,
                                            const Real f_width) {
    CVector3 c_stt_vec;
    CVector3 c_end_vec;

    c_stt_vec = CVector3(boost::python::extract<Real>(boost::python::object(c_start[0])), 
                         boost::python::extract<Real>(boost::python::object(c_start[1])), 
                         boost::python::extract<Real>(boost::python::object(c_start[2])));

    c_end_vec = CVector3(boost::python::extract<Real>(boost::python::object(c_end[0])), 
                         boost::python::extract<Real>(boost::python::object(c_end[1])), 
                         boost::python::extract<Real>(boost::python::object(c_end[2])));

    m_pcCQTOpenGLUserFunctions->DrawRay(CRay3(c_stt_vec, c_end_vec), 
                                        ActusensorsWrapper::CColorWrapper(str_color_name).m_cColor,
                                        f_width);
}

void CQTOpenGLUserFunctionsWrapper::DrawBox(const boost::python::list c_position_list, 
                                            const boost::python::list c_orientation_list, 
                                            const boost::python::list c_size_list, 
                                            const std::string str_color_name) {
    m_pcCQTOpenGLUserFunctions->DrawBox(
        CVector3(boost::python::extract<Real>(boost::python::object(c_position_list[0])), 
                 boost::python::extract<Real>(boost::python::object(c_position_list[1])), 
                 boost::python::extract<Real>(boost::python::object(c_position_list[2]))),
        CQuaternion(), // Implement orientation
        CVector3(boost::python::extract<Real>(boost::python::object(c_size_list[0])), 
                 boost::python::extract<Real>(boost::python::object(c_size_list[1])), 
                 boost::python::extract<Real>(boost::python::object(c_size_list[2]))), 
        ActusensorsWrapper::CColorWrapper(str_color_name).m_cColor);
}

void CQTOpenGLUserFunctionsWrapper::DrawCylinder(const boost::python::list c_position_list, 
                                                 const boost::python::list c_orientation_list, 
                                                 const Real f_radius, 
                                                 const Real f_height,
                                                 const std::string str_color_name) {
    m_pcCQTOpenGLUserFunctions->DrawCylinder(
        CVector3(boost::python::extract<Real>(boost::python::object(c_position_list[0])), 
                 boost::python::extract<Real>(boost::python::object(c_position_list[1])), 
                 boost::python::extract<Real>(boost::python::object(c_position_list[2]))),
        CQuaternion(), // Implement orientation
        f_radius,
        f_height, 
        ActusensorsWrapper::CColorWrapper(str_color_name).m_cColor);
}


void CQTOpenGLUserFunctionsWrapper::DrawText(const boost::python::list c_position,
                                             const std::string str_text,
                                             const std::string str_color_name) {

    CVector3 c_pos_vec;
    c_pos_vec = CVector3(boost::python::extract<Real>(boost::python::object(c_position[0])), 
                         boost::python::extract<Real>(boost::python::object(c_position[1])), 
                         boost::python::extract<Real>(boost::python::object(c_position[2])));

}

void CQTOpenGLUserFunctionsWrapper::CloseWindow() {
  m_pcCQTOpenGLUserFunctions->GetMainWindow().close();

    // NOT WORKING; CORE DUMP
    // m_pcCQTOpenGLUserFunctions->DrawText(CVector3(0,0,1),
    //                                      "sdsd");
}
