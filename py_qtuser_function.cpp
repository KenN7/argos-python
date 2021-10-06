#include "py_qtuser_function.h"

using namespace argos;
using namespace boost::python;

#define INIT_MODULE_QTUSER_FUNCTION PyInit_libpy_qtuser_function_interface
extern "C" PyObject* INIT_MODULE_QTUSER_FUNCTION();

BOOST_PYTHON_MODULE(libpy_qtuser_function_interface) {

  // // Export "LoopFunctionsWrapper" that contains loop functions
  //   class_<CPyQTUserFunction, boost::shared_ptr<CPyQTUserFunction>, boost::noncopyable>("environment", no_init)
  //     .def("set_resources", &CPyQTUserFunction::set_resources);   

}

CPyQTUserFunction::CPyQTUserFunction() {
    // init python
  PyImport_AppendInittab("libpy_qtuser_function_interface", INIT_MODULE_QTUSER_FUNCTION);
  if (!Py_IsInitialized()) {
    Py_Initialize();
  }
  m_qtuser_interpreter = Py_NewInterpreter();
    // init main module and namespace
  m_qtuser_main = import("__main__");
  m_qtuser_namesp = m_qtuser_main.attr("__dict__");
}

void CPyQTUserFunction::Init(TConfigurationNode& t_node) {


  m_environment = boost::make_shared<ActusensorsWrapper>();

  // TConfigurationNode& tParams = GetNode(t_node, "params");
    
  /* Load script */
  std::string strScriptFileName;
  // GetNodeAttributeOrDefault(tParams, "script", strScriptFileName, strScriptFileName);
  // if (strScriptFileName == "") {
  //   THROW_ARGOSEXCEPTION("QTUSER function: Error loading python script \"" << strScriptFileName << "\""
  //     << std::endl);
  // }
  strScriptFileName = "/home/eksander/geth-argos/MarketForaging/loop_functions/qtuser_function.py";
  // exec user script
  try {
    m_qtuser_script = exec_file(strScriptFileName.c_str(), m_qtuser_namesp, m_qtuser_namesp);

    std::cout << "QTUSER function: strScript:" << strScriptFileName << std::endl;
  } catch (error_already_set) {
    PyErr_Print();
  }

  try {
    // Import the wrapper's lib
    // PyRun_SimpleString("import libpy_qtuser_function_interface as lib");
    // object lib = import("libpy_qtuser_function_interface");
    m_qtuser_namesp["environment"] = m_environment;

    // Launch Python init function
    object init_f = m_qtuser_main.attr("init");
    init_f();
  } catch (error_already_set) {
    PyErr_Print();
  }

}

void CPyQTUserFunction::set_resources(std::string test_string) {
std::cout << "test string: set_resources" << std::endl;
}



void CPyQTUserFunction::DrawInWorld() {
  // launch python reset function
  try {
    object draw_in_world_f = m_qtuser_main.attr("DrawInWorld");
    draw_in_world_f();

    // object color_f = m_qtuser_main.attr("color");
    // std::cout << color_f << std::endl;
//
  } catch (error_already_set) {
    PyErr_Print();
  }
}

// void CPyQTUserFunction::DrawInWorld() {
// 	DrawCircle(
// 	CVector3(0.0f, 0.0f, 0.01f), 
// 	CQuaternion(),
// 	0.1,
// 	CColor::RED);
// }


REGISTER_QTOPENGL_USER_FUNCTIONS(CPyQTUserFunction, "py_qtuser_function")