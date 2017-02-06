/**
 * @file <argos3/plugins/robots/e-puck/simulator/tam_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef TAM_ENTITY_H
#define TAM_ENTITY_H

namespace argos {
   class CEmbodiedEntity;
   class CTAMEntity;
   class CTAMsEntity;
   class CLEDEquippedEntity;
   class CRABEquippedEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>

namespace argos {

   class CTAMEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CTAMEntity();
      CTAMEntity(CTAMsEntity* pc_parent);

      CTAMEntity(CTAMsEntity* pc_parent,
                 const std::string& str_id,
                 const CVector3& c_position = CVector3(),
                 const CQuaternion& c_orientation = CQuaternion());

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CLEDEquippedEntity& GetLEDEquippedEntity() {
         return *m_pcLEDEquippedEntity;
      }

      inline CRABEquippedEntity& GetRABEquippedEntity() {
         return *m_pcRABEquippedEntity;
      }

      virtual std::string GetTypeDescription() const {
         return "tam";
      }

      inline bool IsRobotPresent() const {
         return m_bRobotPresent;
      }

      inline void SetRobotPresent(bool b_present) {
         m_bRobotPresent = b_present;
      }

      inline bool ShowRays() {
         return m_bShowRays;
      }

   private:

      CEmbodiedEntity*    m_pcEmbodiedEntity;
      CLEDEquippedEntity* m_pcLEDEquippedEntity;
      CRABEquippedEntity* m_pcRABEquippedEntity;
      bool                m_bShowRays;
      bool                m_bRobotPresent;
   };

}

#endif
