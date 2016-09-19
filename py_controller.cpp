/*
 * AUTHOR: Ken Hasselmann <arg AT kenh DOT fr>
 *
 * Connects ARGoS to python
 *
 */
#include "py_controller.h"

using namespace argos;
using namespace boost::python;

#if PY_MAJOR_VERSION >= 3
#   define INIT_MODULE PyInit_libpy_controller_interface
    extern "C" PyObject* INIT_MODULE();
#else
#   define INIT_MODULE initlibpy_controller_interface
    extern "C" void INIT_MODULE();
#endif


CPyController::CPyController() :
  m_actusensors(NULL),
  m_main(NULL),
  m_namesp(NULL),
  m_script(NULL)
{
  //init python
  PyImport_AppendInittab((char*)"libpy_controller_interface", INIT_MODULE);
  Py_Initialize();

  //init main module and namespace
  m_main = import("__main__");
  m_namesp = m_main.attr("__dict__");

}

void CPyController::Destroy() {
  //launch python destroy function
  try
  {
    object destroy = m_main.attr("destroy");
    destroy();
  }
    catch (error_already_set)
  {
    PyErr_Print();
  }
  //Py_Finalize(); the documentation of boost says we should NOT use this ..
}

void CPyController::Reset() {
  //launch python reset function
  try
  {
    object reset_f = m_main.attr("reset");
    reset_f();
  }
  catch (error_already_set)
  {
    PyErr_Print();
  }
}


void CPyController::InitSensorsActuators()
{
  for(CCI_Actuator::TMap::iterator it = m_mapActuators.begin();
      it != m_mapActuators.end();
      ++it)
  {
    m_actusensors->CreateActu(it->first, it->second); //this);
  }

  for(CCI_Sensor::TMap::iterator it = m_mapSensors.begin();
      it != m_mapSensors.end();
      ++it)
  {
     m_actusensors->CreateSensor(it->first, it->second);
  }

}


void CPyController::Init(TConfigurationNode& t_node)
{
  //get instances of actuators and sensors and pass them to the wrapper
  //m_actusensors = boost::make_shared< ActusensorsWrapper >();
  m_actusensors = new ActusensorsWrapper();
  InitSensorsActuators();

  printf("%s\n", this);

  /* Load script */
  std::string strScriptFileName;
  GetNodeAttributeOrDefault(t_node, "script", strScriptFileName, strScriptFileName);
  if(strScriptFileName == "")
  {
    THROW_ARGOSEXCEPTION("Error loading python script \"" << strScriptFileName << "\"" << std::endl);
  }
  //exec user script
  m_script = exec_file(strScriptFileName.c_str(), m_namesp);
  //std::cout << "strScript:" << strScriptFileName << std::endl;
  std::cout << GetId().c_str() << std::endl;

  try
  {
    //import the wrappers's lib
    //PyRun_SimpleString("import libpy_controller_interface as lib");
    object lib = import("libpy_controller_interface");
    m_namesp["foo"] = ptr(m_actusensors);
    // PyRun_SimpleString(
    //         "foo = None\n"
    //         "def setup(a_foo_from_cxx):\n"
    //         "    print('setup called with', a_foo_from_cxx)\n"
    //         "    global foo\n"
    //         "    foo = a_foo_from_cxx\n"
    //     );
    //
    // object setup_func = m_main.attr("setup");
    // //setup_func(ptr(m_actusensors));


    //launch python init function
    object init_f = m_main.attr("init");
    init_f();
  }
  catch (error_already_set)
  {
    PyErr_Print();
  }

}

void CPyController::ControlStep()
{
  //here the sensors should be updated every step
  //m_actusensors->wheels(1,1);
  //exec("import random", m_namesp);
  //object rand_func = eval("random.randint(-10,10)", m_namesp);

  //launch controlstep python function*
  try
  {
    object controlstep = m_main.attr("controlstep");
    controlstep();
  }
    catch (error_already_set)
  {
    PyErr_Print();
  }
}

REGISTER_CONTROLLER(CPyController, "python_controller");
