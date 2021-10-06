#ifndef PY_ENVIRONMENT_WRAPPER_H
#define PY_ENVIRONMENT_WRAPPER_H

#include <boost/python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/general.h>

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>

#include <iostream>
#include <string>
#include <map>

namespace argos {

/****************************************/
/****************************************/

// Wrapper for loop functions
class CQTOpenGLUserFunctionsWrapper {
  public:
    CQTOpenGLUserFunctionsWrapper();
    ~CQTOpenGLUserFunctionsWrapper(){};

    argos::CQTOpenGLUserFunctions* m_pcCQTOpenGLUserFunctions;

//const boost::python::list c_position_list, const boost::python::list c_orientation_list, const Real f_radius
	void DrawCircle();
};



}

#endif
