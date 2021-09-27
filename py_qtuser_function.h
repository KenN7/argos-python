#ifndef PY_QTUSER_FUNCTION_H
#define PY_QTUSER_FUNCTION_H

#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>
// #include <py_controller.h>
#include <boost/python.hpp>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>
#include <boost/make_shared.hpp>

using namespace argos;

class CPyQTUserFunction : public CQTOpenGLUserFunctions {

 public:

  CPyQTUserFunction();

  virtual ~CPyQTUserFunction () {}

  virtual void Init(TConfigurationNode& t_node);
  void DrawInWorld();
  void set_resources(std::string test_string);

 private:
  
    boost::python::object m_qtuser_main;
    boost::python::object m_qtuser_namesp;
    boost::python::object m_qtuser_script;
    PyThreadState* m_qtuser_interpreter;

  };
  #endif