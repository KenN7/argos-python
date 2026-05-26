#include "py_qtuser_function.h"
#include "py_python_runtime.h"
#include <cctype>
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

void CallIfDefined(boost::python::object& c_namespace, const std::string& str_function) {
  if (PyMapping_HasKeyString(c_namespace.ptr(), str_function.c_str())) {
    object cFunction = c_namespace[str_function];
    if (!cFunction.is_none()) {
      cFunction();
    }
  }
}

int GetTrailingNumber(const std::string& str_id) {
  std::string::size_type unFirstDigit = str_id.size();
  while (unFirstDigit > 0 && std::isdigit(str_id[unFirstDigit - 1])) {
    --unFirstDigit;
  }
  if (unFirstDigit == str_id.size()) {
    return 0;
  }
  return std::stoi(str_id.substr(unFirstDigit));
}

template <typename TRobotEntity>
void DrawRobot(CPyQTUserFunction& c_user_function,
               boost::python::object& c_namespace,
               TRobotEntity& c_entity) {
  CPyController& cController =
    dynamic_cast<CPyController&>(c_entity.GetControllableEntity().GetController());
  c_namespace["robot"] = cController.getActusensors();
  CallIfDefined(c_namespace, "draw_in_robot");

  // Draw the robot ID from here, because DrawText from Python will give segfault.
  c_user_function.DrawText(CVector3(0.0, 0.0, 0.13),
                           std::to_string(GetTrailingNumber(c_entity.GetId()) + 1),
                           CColor::BLUE);
}
}

CPyQTUserFunction::CPyQTUserFunction() {
  CPyGILGuard cGIL;
  m_qtuser_namesp = dict();
  m_qtuser_namesp["__builtins__"] = import("builtins");
  m_qtuser_namesp["__name__"] = "__argos_qtuser_function__";

  // This is just to draw the ID of the robot in the robot reference frame
  RegisterUserFunction<CPyQTUserFunction,CEPuckEntity>(&CPyQTUserFunction::Draw);
  RegisterUserFunction<CPyQTUserFunction,CFootBotEntity>(&CPyQTUserFunction::Draw);
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

  try {
    // Keep DrawInWorld temporarily for backwards compatibility.
    CallIfDefined(m_qtuser_namesp, "DrawInWorld");
    CallIfDefined(m_qtuser_namesp, "draw_in_world");
  } catch (error_already_set) {
    PyErr_Print();  
  }

}


void CPyQTUserFunction::Draw(CEPuckEntity& c_entity) {
  CPyGILGuard cGIL;
  try {
    DrawRobot(*this, m_qtuser_namesp, c_entity);
  } catch (error_already_set) {
    PyErr_Print();
  }
}

void CPyQTUserFunction::Draw(CFootBotEntity& c_entity) {
  CPyGILGuard cGIL;
  try {
    DrawRobot(*this, m_qtuser_namesp, c_entity);
  } catch (error_already_set) {
    PyErr_Print();
  }
}


boost::shared_ptr<EnvironmentWrapper>  CPyQTUserFunction::getEnvironment() {
    return m_environment;
}


REGISTER_QTOPENGL_USER_FUNCTIONS(CPyQTUserFunction, "py_qtuser_function")
