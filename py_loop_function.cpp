#include "py_loop_function.h"
#include "py_python_runtime.h"

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

BOOST_PYTHON_MODULE(libpy_loop_function_interface) {

}

namespace {
struct CLoopFunctionModuleRegistrar {
    CLoopFunctionModuleRegistrar() {
        if (!Py_IsInitialized()) {
            PyImport_AppendInittab("libpy_loop_function_interface", INIT_MODULE_LOOP_FUNCTION);
            PyImport_AppendInittab("libpy_controller_interface", INIT_MODULE_CONTROLLER);
        }
    }
};

CLoopFunctionModuleRegistrar g_cLoopFunctionModuleRegistrar;

template <typename TRobotEntity>
void AppendRobotControllers(CSpace& c_space,
                            const std::string& str_entity_type,
                            boost::python::list& c_all_robots) {
  try {
    CSpace::TMapPerType& cEntities = c_space.GetEntitiesByType(str_entity_type);
    for(CSpace::TMapPerType::iterator it = cEntities.begin(); it != cEntities.end(); ++it) {
      TRobotEntity& cRobot = *any_cast<TRobotEntity*>(it->second);
      CPyController& cController =
        dynamic_cast<CPyController&>(cRobot.GetControllableEntity().GetController());
      c_all_robots.append(cController.getActusensors());
    }
  } catch (...) {
    /*
     * ARGoS throws when no entity map exists for a type. That is fine here:
     * a loop function can be used with foot-bots, e-pucks, or a mix.
     */
  }
}
}

CPyLoopFunction::CPyLoopFunction() {
  CPyGILGuard cGIL;
  m_loop_namesp = dict();
  m_loop_namesp["__builtins__"] = import("builtins");
  m_loop_namesp["__name__"] = "__argos_loop_function__";
}

CPyLoopFunction::~CPyLoopFunction() {
  CPyGILGuard cGIL;
  m_loop_script = object();
  m_loop_namesp = object();
}


void CPyLoopFunction::Init(TConfigurationNode& t_node) {
  CPyGILGuard cGIL;

  TConfigurationNode& tParams = GetNode(t_node, "params");
  
  /* Load script */
  std::string strScriptFileName;
  GetNodeAttributeOrDefault(tParams, "script", strScriptFileName, strScriptFileName);
  if (strScriptFileName == "") {
    THROW_ARGOSEXCEPTION("Loop function: Error loading python script \"" << strScriptFileName << "\""
      << std::endl);
  }
  // exec user script
  try {
    m_loop_script = exec_file(strScriptFileName.c_str(), m_loop_namesp, m_loop_namesp);

    std::cout << "Loop function: strScript:" << strScriptFileName << std::endl;
  } catch (error_already_set) {
    PyErr_Print();
  }


  // Iterate over supported Python-controlled robots and add them to a boost list
  boost::python::list allRobots;
  AppendRobotControllers<CFootBotEntity>(GetSpace(), "foot-bot", allRobots);
  AppendRobotControllers<CEPuckEntity>(GetSpace(), "epuck", allRobots);
  m_loop_namesp["allrobots"]  = allRobots;


  // To-do: find way to access environment handle to access CPyQTUserFunction from loop function
  /* Get handle to environment */
  // cEnvironment = dynamic_cast<CPyQTUserFunction&>();
  // m_environment = cEnvironment.getEnvironment();
  // m_loop_namesp["environment"]  = m_environment;
  
  try {
    // Import the wrapper's lib
    object lib = import("libpy_loop_function_interface");
    m_loop_namesp["lib"] = lib;
    

    // Launch Python init function
    object init_f = m_loop_namesp["init"];
    init_f();
  } catch (error_already_set) {
    PyErr_Print();
  }

}

void CPyLoopFunction::Reset() {
  CPyGILGuard cGIL;
  // launch python reset function
  try {
    object reset_f = m_loop_namesp["reset"];
    reset_f();
  } catch (error_already_set) {
    PyErr_Print();
  }
}

void CPyLoopFunction::Destroy() {
  CPyGILGuard cGIL;
  
  // Launch Python destroy function
  try {
    object destroy_f = m_loop_namesp["destroy"];
    destroy_f();
  } catch (error_already_set) {
    PyErr_Print();
  }
}

void CPyLoopFunction::PreStep() {
  CPyGILGuard cGIL;

  // Launch Python pre_step function
  try {
    object pre_step_f = m_loop_namesp["pre_step"];
    pre_step_f();
  } catch (error_already_set) {
    PyErr_Print();
  }
}

void CPyLoopFunction::PostStep() {
  CPyGILGuard cGIL;
  // Launch Python post_step function
  try {
    object post_step_f = m_loop_namesp["post_step"];
    post_step_f();
  } catch (error_already_set) {
    PyErr_Print();
  }
}

bool CPyLoopFunction::IsExperimentFinished() {
  CPyGILGuard cGIL;

// Launch Python is_experiment_finished function
  try {
    object is_experiment_finished_f = m_loop_namesp["is_experiment_finished"];
    return is_experiment_finished_f();
  } catch (error_already_set) {
    PyErr_Print();
    return true;
  }

}

CColor CPyLoopFunction::GetFloorColor() {
  CPyGILGuard cGIL;

// Launch Python is_experiment_finished function
  try {
    object get_floor_color_f = m_loop_namesp["get_floor_color"];

    std::cout << "Testing GetFloorColor" << std::endl;
    return CColor::WHITE;
  } catch (error_already_set) {
    PyErr_Print();
    return CColor::WHITE;
  }

}

void CPyLoopFunction::PostExperiment() {
  CPyGILGuard cGIL;
  // Launch Python post_experiment function
  try {
    object post_experiment_f = m_loop_namesp["post_experiment"];
    post_experiment_f();
  } catch (error_already_set) {
    PyErr_Print();
  }
}


REGISTER_LOOP_FUNCTIONS(CPyLoopFunction, "py_loop_function")
