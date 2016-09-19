#include "py_controller.h"

using namespace argos;

int add(int x)
{
  return x+1;
}

using namespace boost::python;

#if PY_MAJOR_VERSION >= 3
#   define INIT_MODULE PyInit_libpy_controller_interface
    extern "C" PyObject* INIT_MODULE();
#else
#   define INIT_MODULE initlibpy_controller_interface
    extern "C" void INIT_MODULE();
#endif

typedef boost::shared_ptr<CPyController> cpy_ptr;

BOOST_PYTHON_MODULE(libpy_controller_interface)
{
  def("add", add);
  class_<CPyController, cpy_ptr>("CPyController", no_init)
    .def("getid", &CPyController::GetId_2)
    //.def("controlstep", &CPyController::GetId)
  ;
  //class_<CCI_Actuator>("")

  //class_<CCI_Sensor>("")
}


CPyController::CPyController() :
  m_state(0),
  m_id(0),
  // m_pcWheels(NULL),
  // m_pcProximity(NULL),
  // m_pcOmniCam(NULL),
  m_main(NULL),
  m_namesp(NULL),
  m_script(NULL)
{
}

std::string CPyController::GetId_2()
{
  return GetId().c_str();
}


void CPyController::Destroy() {
  //launch python destroy function
  object destroy = m_main.attr("destroy");
  destroy();
  //Py_Finalize(); the documentation of boost says we should NOT use this ..
}

void CPyController::Reset() {
  //launch python reset function
  object reset_f = m_main.attr("reset");
  reset_f();
}


void CPyController::InitSensorsActuators()
{

  for(CCI_Actuator::TMap::iterator it = m_mapActuators.begin();
      it != m_mapActuators.end();
      ++it)
  {
     m_actuators_dict[it->first] = it->second;
  }
  m_main.attr("actuators") = m_actuators_dict;

  for(CCI_Sensor::TMap::iterator it = m_mapSensors.begin();
      it != m_mapSensors.end();
      ++it)
  {
     m_sensors_dict[it->first] = it->second;
  }
  m_main.attr("sensors") = m_sensors_dict;

}


void CPyController::Init(TConfigurationNode& t_node)
{

  //init of actuators/sensors
  //m_pcWheels = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
  //m_pcProximity = GetSensor<CCI_FootBotProximitySensor>("footbot_proximity");
  //m_pcOmniCam = GetSensor<CCI_ColoredBlobOmnidirectionalCameraSensor>("colored_blob_omnidirectional_camera");

  //init python
  PyImport_AppendInittab((char*)"libpy_controller_interface", INIT_MODULE);
  Py_Initialize();

  m_main = import("__main__");
  m_namesp = m_main.attr("__dict__");

  /* Load script */
  std::string strScriptFileName;
  GetNodeAttributeOrDefault(t_node, "script", strScriptFileName, strScriptFileName);
  if(strScriptFileName == "")
  {
    THROW_ARGOSEXCEPTION("Error loading python script \"" << strScriptFileName << "\"" << std::endl);
  }

  m_script = exec_file(strScriptFileName.c_str(), m_namesp);

  std::cout << "strScript:" << strScriptFileName << std::endl;
  printf("%s\n", GetId_2());

  try
  {

    //cpy_ptr aacpy_pt = (cpy_ptr)this;
    PyRun_SimpleString("import libpy_controller_interface as l");

    PyRun_SimpleString("print(l.add(41))");
    PyRun_SimpleString(
            "foo = []\n"
            "def setup(a_foo_from_cxx):\n"
            "    print('setup called with', a_foo_from_cxx)\n"
            "    global foo\n"
            "    foo.append(a_foo_from_cxx)\n"
        );

    // object main = object(handle<>(borrowed(
    //         PyImport_AddModule("__main__")
    //     )));

    object setup_func = m_main.attr("setup");
    setup_func(ptr(this));

    //m_main.attr("yama") = ptr(this);
    InitSensorsActuators();
    PyRun_SimpleString("print(actuators)");
    //PyRun_SimpleString("print(CPyController.getid())");

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

  // PyRun_SimpleString("print(foo)");

  // PyRun_SimpleString("import random\n"
  //                    "def rand():\n"
  //                    "    i = random.randint(1,10)\n"
  //                    "    print(i)\n"
  //                    "    return i"
  //                  );


  //exec("import random", m_namesp);
  //object rand_func = eval("random.randint(-10,10)", m_namesp);

  //std::cout << extract<double>(rand_func) << std::endl;


  //launch controlstep python function
  object controlstep = m_main.attr("controlstep");
  controlstep();

  //object rand_func2 = eval("random.randint(-5,10)", m_namesp);

  // int ih = extract<double>(rand_func);
  // int ih2 = extract<double>(rand_func2);
  //printf("%d\n", ih2);

  //PyRun_SimpleString("print(yama.getid())");

  //m_pcWheels->SetLinearVelocity(1, 1);
}

REGISTER_CONTROLLER(CPyController, "python_controller");
