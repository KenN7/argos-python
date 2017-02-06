#include <boost/python.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "tams_entity.h"
#include "tam_entity.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/media/rab_medium.h>
#include <argos3/plugins/simulator/media/led_medium.h>


namespace argos {

    // Taken from http://thejosephturner.com/blog/post/embedding-python-in-c-applications-with-boostpython-part-2/
    std::string parse_python_exception()
    {
        PyObject *type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
        // Fetch the python error
        PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);
        // Set some fallback values
        std::string ret("Unfetchable Python error");

        // Try to obtain the type of error (e.g. SyntaxError)
        // check that there is actually a valid pointer to the type data.
        if(type_ptr != NULL){
            boost::python::handle<> h_type(type_ptr);
            boost::python::str type_pstr(h_type);
            boost::python::extract<std::string> e_type_pstr(type_pstr);
            if(e_type_pstr.check())
                ret = e_type_pstr();
            else
                ret = "Unknown exception type";
        }

        // Find the value that caused the error
        if(value_ptr != NULL){
            boost::python::handle<> h_val(value_ptr);
            boost::python::str a(h_val);
            boost::python::extract<std::string> returned(a);
            if(returned.check())
                ret +=  ": " + returned();
            else
                ret += std::string(": Unparseable Python error: ");
        }

        // Find where the error occured
        if(traceback_ptr != NULL){
            boost::python::handle<> h_tb(traceback_ptr);
            boost::python::object tb(boost::python::import("traceback"));
            boost::python::object fmt_tb(tb.attr("format_tb"));
            boost::python::object tb_list(fmt_tb(h_tb));
            boost::python::object tb_str(boost::python::str("\n").join(tb_list));
            boost::python::extract<std::string> returned(tb_str);
            if(returned.check())
                ret += ": " + returned();
            else
                ret += std::string(": Unparseable Python traceback");
        }
        return ret;
    }


   /****************************************/
   /****************************************/

   static const CVector3 FRONT_BARRIER_LEFT_ENDPOINT (-0.052f,  0.05f, 0.005f);
   static const CVector3 FRONT_BARRIER_RIGHT_ENDPOINT(-0.052f, -0.05f, 0.005f);
   static const CVector3 BACK_BARRIER_LEFT_ENDPOINT  (-0.012f,  0.05f, 0.005f);
   static const CVector3 BACK_BARRIER_RIGHT_ENDPOINT (-0.012f, -0.05f, 0.005f);

   /****************************************/
   /****************************************/

    class CTAMsControllableEntityImpl : public CTAMsControllableEntity {

    public:

        CTAMsControllableEntityImpl(CTAMsEntity* pc_parent,
                                  const std::string& str_id,
                                  const std::string& str_experiment_module_path,
                                  CRABMedium& c_rab_medium,
                                  UInt32 un_rand_seed) :
        CTAMsControllableEntity(pc_parent, str_id),
        m_cRABMedium(c_rab_medium),
        m_unRandSeed(un_rand_seed){
            try {
                Py_Initialize();
                main_module = boost::python::import("__main__");
                main_namespace = main_module.attr("__dict__");

                boost::python::exec("import sys", main_namespace);
                boost::python::exec("import os", main_namespace);
                boost::python::exec("sys.path.append(os.path.abspath('../common'))", main_namespace);
                boost::python::exec("sys.path.append(os.path.abspath('./'))", main_namespace);
                boost::python::exec(str_experiment_module_path, main_namespace);


                // Import the module "TAM_argos"
                tam_module = boost::python::import("TAM_argos");

                /*
                * Import the module relative to the experiment being performed.
                * The Experiment class name must be the same as the file name:
                * E.g. /usr/.../MyExperiment.py -> MyExperiment
                */
                experiment_module = boost::python::import(os.path.basename(str_experiment_module_path));
                // Create an instance of the experiment class, by passing the given random seed to it.
                m_tPythonExperimentObject = experiment_module.attr(os.path.basename(str_experiment_module_path))(un_rand_seed);

            } catch(boost::python::error_already_set const &) {
                std::string p_error_str = parse_python_exception();
                std::cout << "Error in Python: " << p_error_str << std::endl;
            }
        }

        virtual ~CTAMsControllableEntityImpl() {}

        virtual void Sense() {
            /*
            * Clear intersection data
            */
            m_vecCheckedRays.clear();
            m_vecIntersectionPoints.clear();
            /*
            * Update TAM state in Python
            */
            try {
                for(size_t i = 0; i < m_vecTAMEntities.size(); ++i) {
                    /* Update robot presence flag */
                    UpdateTAMEntityRobotPresence(*m_vecTAMEntities[i]);

                    m_vecPythonTAMObjects[i].attr("update_robot_present")(m_vecTAMEntities[i]->IsRobotPresent());

                    /*
                    * TODO: int robotData;
                    */
                    if (m_vecTAMEntities[i]->IsRobotPresent()) {
                        const CSet<CRABEquippedEntity*>& setRABs =
                            m_cRABMedium.GetRABsCommunicatingWith(
                        m_vecTAMEntities[i]->GetRABEquippedEntity());

                        for(CSet<CRABEquippedEntity*>::iterator it = setRABs.begin(); it != setRABs.end(); ++it) {
                            CRABEquippedEntity& cRABEntity = **it;

                            m_vecJavaTAMObjects[i].attr("get_robot_data_received")(cRABEntity.GetData()[0]);
                        }
                    }
                }
            } catch(boost::python::error_already_set const &) {
                std::string p_error_str = parse_python_exception();
                std::cout << "Error in Python: " << p_error_str << std::endl;
            }
        }

        virtual void ControlStep() {}

        virtual void Act() {}

        virtual void Destroy() {}

        virtual void AddTAM(CTAMEntity& c_tam) {
            try {
                /* Create a new java TAM */
                boost::python::object tTAM = CreateTAM(c_tam);
                /* Add TAM to the lists */
                m_vecPythonTAMObjects.push_back(tTAM);
                m_vecTAMEntities.push_back(&c_tam);

                // Attach the python TAM object to the python Experiment object.
                m_tPythonExperimentObject.attr("attach_TAM_controller")(tTam);
            } catch(boost::python::error_already_set const &) {
                std::string p_error_str = parse_python_exception();
                std::cout << "Error in Python: " << p_error_str << std::endl;
            }
        }


    private:

        boost::python::object CreateTAM(CTAMEntity& c_tam) 
        {
            return tam_module.attr("TAM")(c_tam.GetId());
        }

        // Contains the python module of the TAM.
        boost::python::object tam_module;
        // Contains the python module of the experiment that is being performed.
        boost::python::object experiment_class;
        // Instance of the experiment class defined in the experiment module.
        boost::python::object m_tPythonExperimentObject;

        std::vector<CTAMEntity*> m_vecTAMEntities;

        std::vector<boost::python::object> m_vecPythonTAMObjects;

        CRABMedium& m_cRABMedium;

        UInt32 m_unRandSeed;


        void UpdateTAMEntityRobotPresence(CTAMEntity& c_tam) {
            /* Buffer to contain data about the intersection */
            SEmbodiedEntityIntersectionItem sIntersection;
            /* Get the TAM body */
            CEmbodiedEntity& cBody = c_tam.GetEmbodiedEntity();
            /* Calculate the endpoints of the front barrier in world coordinates */
            CVector3 cBarrierLeft  = FRONT_BARRIER_LEFT_ENDPOINT;
            cBarrierLeft.Rotate(cBody.GetOriginAnchor().Orientation);
            cBarrierLeft += cBody.GetOriginAnchor().Position;
            CVector3 cBarrierRight = FRONT_BARRIER_RIGHT_ENDPOINT;
            cBarrierRight.Rotate(cBody.GetOriginAnchor().Orientation);
            cBarrierRight += cBody.GetOriginAnchor().Position;
            CRay3 cBarrier(cBarrierLeft, cBarrierRight);
            bool bFrontBarrierOccupied = GetClosestEmbodiedEntityIntersectedByRay(sIntersection,
                                                                                  cBarrier,
                                                                                  cBody);
            if (c_tam.ShowRays())
               AddCheckedRay(bFrontBarrierOccupied, cBarrier);
            if(bFrontBarrierOccupied) {
               if (c_tam.ShowRays())
                  AddIntersectionPoint(cBarrier, sIntersection.TOnRay);
            }
            /* Calculate the endpoints of the back barrier in world coordinates */
            cBarrierLeft  = BACK_BARRIER_LEFT_ENDPOINT;
            cBarrierLeft.Rotate(cBody.GetOriginAnchor().Orientation);
            cBarrierLeft += cBody.GetOriginAnchor().Position;
            cBarrierRight = BACK_BARRIER_RIGHT_ENDPOINT;
            cBarrierRight.Rotate(cBody.GetOriginAnchor().Orientation);
            cBarrierRight += cBody.GetOriginAnchor().Position;
            cBarrier.Set(cBarrierLeft, cBarrierRight);
            bool bBackBarrierOccupied = GetClosestEmbodiedEntityIntersectedByRay(sIntersection,
                                                                                 cBarrier,
                                                                                 cBody);
            if (c_tam.ShowRays())
               AddCheckedRay(bBackBarrierOccupied, cBarrier);
            if(bBackBarrierOccupied) {
               if (c_tam.ShowRays())
                  AddIntersectionPoint(cBarrier, sIntersection.TOnRay);
            }
            /*
             * Set the flag for the presence of a robot in the TAM entity
             * This logic mirrors what is calculated by the real TAM firmware
             */
            if (!c_tam.IsRobotPresent() && !bFrontBarrierOccupied && bBackBarrierOccupied)
            {
              c_tam.SetRobotPresent(true);
            }
            else if (c_tam.IsRobotPresent() && !bFrontBarrierOccupied && !bBackBarrierOccupied)
            {
              c_tam.SetRobotPresent(false);
            }
            //c_tam.SetRobotPresent((!bFrontBarrierOccupied) & bBackBarrierOccupied);
         }
    };


    /****************************************/
    /****************************************/

    CTAMsEntity::CTAMsEntity() :
       CComposableEntity(NULL),
       m_pcTAMControllable(NULL),
       m_pcRABMedium(NULL),
       m_pcLEDMedium(NULL) {}
 
    /****************************************/
    /****************************************/
 
    CTAMsEntity::CTAMsEntity(const std::string& str_id,
                             const std::string& str_experiment_module_path,
                             const std::string& str_rab_medium,
                             const std::string& str_led_medium,
                             const UInt32 un_rand_seed) :
       CComposableEntity(NULL) {
 
       m_pcRABMedium = &(CSimulator::GetInstance().GetMedium<CRABMedium>(str_rab_medium));
       m_pcLEDMedium = &(CSimulator::GetInstance().GetMedium<CLEDMedium>(str_led_medium));
       m_unRandSeed = un_rand_seed;
       m_pcTAMControllable = new CTAMsControllableEntityImpl(this,
                                                             "controller_0",
                                                             str_experiment_module_path,
                                                             *m_pcRABMedium,
                                                             m_unRandSeed);
       /* Add controllable entity */
       AddComponent(*m_pcTAMControllable);
    }
 
    /****************************************/
    /****************************************/
 
    void CTAMsEntity::Init(TConfigurationNode& t_tree) {
        try {
            /* Parse media info */
            std::string strRABMedium;
            GetNodeAttribute(t_tree, "rab_medium", strRABMedium);
            m_pcRABMedium = &(CSimulator::GetInstance().GetMedium<CRABMedium>(strRABMedium));
            std::string strLEDMedium;
            GetNodeAttribute(t_tree, "led_medium", strLEDMedium);
            m_pcLEDMedium = &(CSimulator::GetInstance().GetMedium<CLEDMedium>(strLEDMedium));
            std::string str_experiment_module_path;
            GetNodeAttribute(t_tree, "str_experiment_module_path", str_experiment_module_path);
            /* Random seed. Default is 0 */
            GetNodeAttributeOrDefault(t_tree, "random_seed", m_unRandSeed, m_unRandSeed);
            m_pcTAMControllable = new CTAMsControllableEntityImpl(this,
                                                                  "controller_0",
                                                                  str_experiment_module_path,
                                                                  *m_pcRABMedium,
                                                                  m_unRandSeed);
            AddComponent(*m_pcTAMControllable);
            /* Go through TAMs */
            TConfigurationNodeIterator itTAM("tam");
            for(itTAM = itTAM.begin(&t_tree); itTAM != itTAM.end(); ++itTAM) {
               CTAMEntity* pcTAM = new CTAMEntity(this);
               pcTAM->Init(*itTAM);
               AddTAM(*pcTAM);
            }
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Error initializing the TAMs entity", ex);
         }
    }
 
    /****************************************/
    /****************************************/
 
    void CTAMsEntity::Reset() {
       // Destroy and recreate TAM entities
    }
 
    /****************************************/
    /****************************************/
 
    void CTAMsEntity::Destroy() {
    }
 
    /****************************************/
    /****************************************/
 
    void CTAMsEntity::AddTAM(CTAMEntity& c_tam) {
       c_tam.GetLEDEquippedEntity().AddToMedium(*m_pcLEDMedium);
       m_pcRABMedium->AddEntity(c_tam.GetRABEquippedEntity());
       AddComponent(c_tam);
       m_vecTAMs.push_back(&c_tam);
       m_pcTAMControllable->AddTAM(c_tam);
       c_tam.Update();
    }
 
    /****************************************/
    /****************************************/
 
    REGISTER_ENTITY(CTAMsEntity,
                    "tams",
                    "Carlo Pinciroli [ilpincy@gmail.com]",
                    "1.0",
                    "A container for the Task Abstraction Modules developed at IRIDIA.",
                    "TODO\n\n"
                    ,
                    "Under development"
       );
 
    /****************************************/
    /****************************************/
 
    REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CTAMsEntity);
 
    /****************************************/
    /****************************************/
 
}


// int main()
// {
//     namespace py = boost::python;
    
//     try{
//         Py_Initialize();
//         py::object main_module = py::import("__main__");
//         py::object main_namespace = main_module.attr("__dict__");

//         py::exec("import sys", main_namespace);
//         py::exec("import os", main_namespace);
//         py::exec("sys.path.append(os.path.abspath('../common'))", main_namespace);
//         py::exec("sys.path.append(os.path.abspath('./'))", main_namespace);

//         // Import the module "TAM_argos"
//         py::object tam_module = py::import("TAM_argos");
//         // Instantiate an object of class "Test", and store it into "test_obj""
//         py::object tam = tam_module.attr("TAM")("TAM01");


        
//         // Save a class method as variable 
//         // NOTE: the method will access the class attribute of the object to which it refers, so be careful!
//         py::object print_tam = tam.attr("__str__");
//         std::cout << std::string(py::extract<std::string>(print_tam())) << std::endl;



//     } catch(boost::python::error_already_set const &){
//         std::string p_error_str = parse_python_exception();
//         std::cout << "Error in Python: " << p_error_str << std::endl;
//     }
// }

// boost::python::object CreateTAM(CTAMEntity& c_tam) 
// {
//     return tam = tam_module.attr("TAM")(c_tam.GetId());
// }
