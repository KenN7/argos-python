/**
 * @file <argos3/plugins/robots/e-puck/simulator/tam_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "tam_entity.h"
#include "tams_entity.h"

#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CTAMEntity::CTAMEntity() :
      CComposableEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_pcLEDEquippedEntity(NULL),
      m_pcRABEquippedEntity(NULL),
      m_bShowRays(false),
      m_bRobotPresent(false) {}

   /****************************************/
   /****************************************/

   CTAMEntity::CTAMEntity(CTAMsEntity* pc_parent) :
      CComposableEntity(pc_parent),
      m_pcEmbodiedEntity(NULL),
      m_pcLEDEquippedEntity(NULL),
      m_pcRABEquippedEntity(NULL),
      m_bShowRays(false),
      m_bRobotPresent(false) {}

   /****************************************/
   /****************************************/

   CTAMEntity::CTAMEntity(CTAMsEntity* pc_parent,
                          const std::string& str_id,
                          const CVector3& c_position,
                          const CQuaternion& c_orientation) :
      CComposableEntity(this, str_id),
      m_pcEmbodiedEntity(NULL),
      m_pcLEDEquippedEntity(NULL),
      m_pcRABEquippedEntity(NULL),
      m_bShowRays(false),
      m_bRobotPresent(false) {
      try {
         /*
          * Create and init components
          */
         /* Embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this, "body_0", c_position, c_orientation);
         m_pcEmbodiedEntity->SetMovable(false);
         AddComponent(*m_pcEmbodiedEntity);
         /* LED equipped entity */
         m_pcLEDEquippedEntity = new CLEDEquippedEntity(this,
                                                        "leds_0");

         AddComponent(*m_pcLEDEquippedEntity);

         /* Top LED */
         m_pcLEDEquippedEntity->AddLED(CVector3(0.05f, 0.0f, 0.113f),m_pcEmbodiedEntity->GetOriginAnchor());
         /* body LEDs */
         m_pcLEDEquippedEntity->AddLED(CVector3(0.035f, -0.025f, 0.06f),m_pcEmbodiedEntity->GetOriginAnchor());
         m_pcLEDEquippedEntity->AddLED(CVector3(0.035f, 0.0f, 0.06f),m_pcEmbodiedEntity->GetOriginAnchor());
         m_pcLEDEquippedEntity->AddLED(CVector3(0.035f, 0.025f, 0.06f),m_pcEmbodiedEntity->GetOriginAnchor());

         /* RAB equipped entity */
         m_pcRABEquippedEntity = new CRABEquippedEntity(this,
                                                        "rab_0",
                                                        1,
                                                        0.05f,
                                                        m_pcEmbodiedEntity->GetOriginAnchor(),
                                                        *m_pcEmbodiedEntity,
                                                        //CVector3(0.034f, 0.0f, 0.052f));
                                                        CVector3(0.0f, 0.0f, 0.052f));
         AddComponent(*m_pcRABEquippedEntity);
         m_pcRABEquippedEntity->Enable();
         /* Update components */
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CTAMEntity::Init(TConfigurationNode& t_tree) {
      try {
         /*
          * Init parent
          */
         CComposableEntity::Init(t_tree);
         /*
          * Create and init components
          */
         /* Parse xml show_rays */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         /* Embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this);
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));

         m_pcEmbodiedEntity->SetMovable(false);
         /* LED equipped entity */
         m_pcLEDEquippedEntity = new CLEDEquippedEntity(this,
                                                        "leds_0");
         AddComponent(*m_pcLEDEquippedEntity);
         /* Top LED */
         m_pcLEDEquippedEntity->AddLED(CVector3(0.05f, 0.0f, 0.113f),m_pcEmbodiedEntity->GetOriginAnchor());
         /* body LEDs */
         m_pcLEDEquippedEntity->AddLED(CVector3(0.035f, -0.025f, 0.06f),m_pcEmbodiedEntity->GetOriginAnchor());
         m_pcLEDEquippedEntity->AddLED(CVector3(0.035f, 0.0f, 0.06f),m_pcEmbodiedEntity->GetOriginAnchor());
         m_pcLEDEquippedEntity->AddLED(CVector3(0.035f, 0.025f, 0.06f),m_pcEmbodiedEntity->GetOriginAnchor());
         /* RAB equipped entity */
         m_pcRABEquippedEntity = new CRABEquippedEntity(this,
                                                        "rab_0",
                                                        1,
                                                        0.05f,
                                                        m_pcEmbodiedEntity->GetOriginAnchor(),
                                                        *m_pcEmbodiedEntity,
                                                        //CVector3(0.034f, 0.0f, 0.052f));
                                                        CVector3(0.0f, 0.0f, 0.052f));

         AddComponent(*m_pcRABEquippedEntity);

         m_pcRABEquippedEntity->Enable();

         /* Update components */
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CTAMEntity::Reset() {
      /* Reset all components */
      CComposableEntity::Reset();
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CTAMEntity);

   /****************************************/
   /****************************************/

}
