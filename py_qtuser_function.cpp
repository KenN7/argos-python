#include "py_qtuser_function.h"
#include "py_python_runtime.h"
#include <string>

using namespace argos;
using namespace boost::python;

#define INIT_MODULE_LOOP_FUNCTION PyInit_libpy_loop_function_interface
extern "C" PyObject* INIT_MODULE_LOOP_FUNCTION();

// TODO: I had to add these lines and the line PyImport_AppendInittab("libpy_controller_interface", INIT_MODULE_CONTROLLER)
// in this file, otherwise I god an error that libpy_controller_interface is not a built-in module
#define INIT_MODULE_CONTROLLER PyInit_libpy_controller_interface
extern "C" PyObject* INIT_MODULE_CONTROLLER();

// TODO: I had to add these lines and the line PyImport_AppendInittab("libpy_qtuser_function_interface", INIT_MODULE_QTUSER_FUNCTION)
// in this file, otherwise I god an error that libpy_qtuser_function_interface is not a built-in module
#define INIT_MODULE_QTUSER_FUNCTION PyInit_libpy_qtuser_function_interface
extern "C" PyObject* INIT_MODULE_QTUSER_FUNCTION();

BOOST_PYTHON_MODULE(libpy_qtuser_function_interface) {

}

namespace {
struct CQTUserFunctionModuleRegistrar {
    CQTUserFunctionModuleRegistrar() {
        if (!Py_IsInitialized()) {
            PyImport_AppendInittab("libpy_qtuser_function_interface", INIT_MODULE_QTUSER_FUNCTION);
        }
    }
};

CQTUserFunctionModuleRegistrar g_cQTUserFunctionModuleRegistrar;
}

CPyQTUserFunction::CPyQTUserFunction() {
  CPyGILGuard cGIL;
  m_qtuser_namesp = dict();
  m_qtuser_namesp["__builtins__"] = import("builtins");
  m_qtuser_namesp["__name__"] = "__argos_qtuser_function__";

  // This is just to draw the ID of the robot in the robot reference frame
  RegisterUserFunction<CPyQTUserFunction,CEPuckEntity>(&CPyQTUserFunction::Draw);
}

CPyQTUserFunction::~CPyQTUserFunction() {
  CPyGILGuard cGIL;
  m_qtuser_script = object();
  m_qtuser_namesp = object();
}

void CPyQTUserFunction::Init(TConfigurationNode& t_node) {
  CPyGILGuard cGIL;
  
  TConfigurationNode& tParams = GetNode(t_node, "params");

  /* Load script */
  std::string strScriptFileName;
  GetNodeAttributeOrDefault(tParams, "script", strScriptFileName, strScriptFileName);
  if (strScriptFileName == "") {
    THROW_ARGOSEXCEPTION("QTUSER function: Error loading python script \"" << strScriptFileName << "\""
      << std::endl);
  }
  // exec user script
  try {
    m_qtuser_script = exec_file(strScriptFileName.c_str(), m_qtuser_namesp, m_qtuser_namesp);

    std::cout << "QTUSER function: strScript:" << strScriptFileName << std::endl;
  } catch (error_already_set) {
    PyErr_Print();
  }

  m_environment = boost::make_shared<EnvironmentWrapper>();
  m_qtuser_namesp["environment"] = m_environment;

  try {
    // Import the wrapper's lib
    // PyRun_SimpleString("import libpy_qtuser_function_interface as lib");
    // object lib = import("libpy_qtuser_function_interface");

    // Launch Python init function
    object init_f = m_qtuser_namesp["init"];
    init_f();
  } catch (error_already_set) {
    PyErr_Print();
  }

}

void CPyQTUserFunction::Destroy() {
  CPyGILGuard cGIL;
  
  // Launch Python destroy function
  try {
    object destroy_f = m_qtuser_namesp["destroy"];
    destroy_f();
  } catch (error_already_set) {
    PyErr_Print();
  }
}


void CPyQTUserFunction::DrawInWorld() {
  CPyGILGuard cGIL;

  // keeping this one temporarily for backwards compatibility
  try {
    object draw_in_world_f = m_qtuser_namesp["DrawInWorld"];
    draw_in_world_f();

  } catch (error_already_set) {
    // std::cout << "please rename DrawInWorld to draw_in_world in loop_functiopqtuser_function.py" << std::endl;
  }

  // launch python draw function
  try {
    object draw_in_world_f = m_qtuser_namesp["draw_in_world"];
    draw_in_world_f();

  } catch (error_already_set) {
    PyErr_Print();
  }

}


void CPyQTUserFunction::Draw(CEPuckEntity& c_entity) {
  CPyGILGuard cGIL;
  /* The position of the drawings is expressed wrt the reference point of the epuck
  */

  // Get robot actusensors and export to Python
  CPyController& cController = dynamic_cast<CPyController&>(c_entity.GetControllableEntity().GetController());
  m_qtuser_namesp["robot"]  = cController.getActusensors();

  // Launch Python draw function
  try {
    object destroy_f = m_qtuser_namesp["draw_in_robot"];
    destroy_f();
  } catch (error_already_set) {
    PyErr_Print();
  }

  // Draw the robot ID from here, because DrawText from Python will give segfault
  DrawText(CVector3(0.0, 0.0, 0.13),   // position
        std::to_string(stoi(c_entity.GetId().substr(2)) + 1),
        CColor::BLUE); // text
}


boost::shared_ptr<EnvironmentWrapper>  CPyQTUserFunction::getEnvironment() {
    return m_environment;
}


REGISTER_QTOPENGL_USER_FUNCTIONS(CPyQTUserFunction, "py_qtuser_function")
