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

#include <boost/filesystem.hpp>

namespace argos {

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
                boost::python::exec(boost::python::str(str_experiment_module_path), main_namespace);


                // Import the module "TAM_argos"
                tam_module = boost::python::import("TAM_argos");

                /*
                * Import the module relative to the experiment being performed.
                * The Experiment class name must be the same as the file name:
                * E.g. /usr/.../MyExperiment.py -> MyExperiment
                */

                const boost::filesystem::path p(str_experiment_module_path);
                experiment_module = boost::python::import(boost::python::str(p.filename()));
                // Create an instance of the experiment class, by passing the given random seed to it.
                m_tPythonExperimentObject = experiment_module.attr(boost::python::str(p.filename()))(un_rand_seed);

            } catch(boost::python::error_already_set const &) {
                PyErr_Print();
            }
        }

        virtual ~CTAMsControllableEntityImpl() {}

        virtual void Sense() 
        {
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

                            m_vecPythonTAMObjects[i].attr("get_robot_data_received")(cRABEntity.GetData()[0]);
                        }
                    }
                }
            } catch(boost::python::error_already_set const &) {
                PyErr_Print();
            }
        }

        virtual void ControlStep()
        {
            try {
                /* Call python experiment step */
                m_tPythonExperimentObject.attr("step")();
                /* Call all the python controller steps */
                boost::python::object tPythonControllerObject;
                for(size_t i = 0; i < m_vecPythonTAMObjects.size(); ++i) 
                {
                    /* Get reference of this TAM's controller */
                    tPythonControllerObject = m_vecPythonTAMObjects[i].attr("get_controller")();
                    /* If the reference is not NULL, call the controller step() */
                    if(tPythonControllerObject != NULL)
                    {
                    tPythonControllerObject.attr("step")();
                    }
                }
            } catch(boost::python::error_already_set const &) {
                PyErr_Print();
            }
        }

        virtual void Act()
        {
            try {
                /*
                * Update all TAM entities' LED colors from Python states
                */
                for(size_t i = 0; i < m_vecTAMEntities.size(); ++i) {
                    /* Get reference of this TAM's LED color */
                    boost::python::object tPythonLEDColor = m_vecPythonTAMObjects[i].attr("get_led_color")();
                    /* Create a CColor with the Python colors */
                    CColor cLEDColor(
                        boost::python::extract<argos::UInt8>(tPythonLEDColor.attr("get_red_channel_value")()),
                        boost::python::extract<argos::UInt8>(tPythonLEDColor.attr("get_green_channel_value")()),
                        boost::python::extract<argos::UInt8>(tPythonLEDColor.attr("get_blue_channel_value")()));
                    /* Set the TAM entity LED color */
                    m_vecTAMEntities[i]->GetLEDEquippedEntity().GetLED(0).SetColor(cLEDColor);
                    m_vecTAMEntities[i]->GetLEDEquippedEntity().GetLED(1).SetColor(cLEDColor);
                    m_vecTAMEntities[i]->GetLEDEquippedEntity().GetLED(2).SetColor(cLEDColor);
                    m_vecTAMEntities[i]->GetLEDEquippedEntity().GetLED(3).SetColor(cLEDColor);

                    UInt8 unData = boost::python::extract<UInt8>(m_vecPythonTAMObjects[i].attr("get_robot_data_to_send")());
                    if (m_vecTAMEntities[i]->IsRobotPresent()) {
                        m_vecTAMEntities[i]->GetRABEquippedEntity().GetData()[0] = unData;
                    }
                    m_vecTAMEntities[i]->Update();
                }
            } catch(boost::python::error_already_set const &) {
                PyErr_Print();
            }      
        }

        virtual void Destroy()
        {
            // No need to implement it?
            return;  
        }

        virtual void AddTAM(CTAMEntity& c_tam) 
        {
            try {
                /* Create a new java TAM */
                boost::python::object tTAM = CreateTAM(c_tam);
                /* Add TAM to the lists */
                m_vecPythonTAMObjects.push_back(tTAM);
                m_vecTAMEntities.push_back(&c_tam);

                // Attach the python TAM object to the python Experiment object.
                m_tPythonExperimentObject.attr("attach_TAM_controller")(tTAM);
            } catch(boost::python::error_already_set const &) {
                PyErr_Print();
            }
        }


    private:

        boost::python::object CreateTAM(CTAMEntity& c_tam) 
        {
            return tam_module.attr("TAM")(c_tam.GetId());
        }

        //  Needed to hold the python dictionary and the python "main" function;
        boost::python::object main_module;
        boost::python::object main_namespace;

        // Contains the python module of the TAM.
        boost::python::object tam_module;
        // Contains the python module of the experiment that is being performed.
        boost::python::object experiment_module;
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


