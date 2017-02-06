/**
 * @file <argos3/plugins/robots/e-puck/simulator/tams_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef TAMS_ENTITY_H
#define TAMS_ENTITY_H

namespace argos {
   class CTAMsEntity;
   class CTAMEntity;
   class CTAMsControllableEntity;
   class CLEDMedium;
   class CRABMedium;
}

#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   class CTAMsControllableEntity : public CControllableEntity {
   public:
      CTAMsControllableEntity(CComposableEntity* pc_parent,
                              const std::string& str_id) :
         CControllableEntity(pc_parent, str_id) {}
      virtual ~CTAMsControllableEntity() {}
      virtual void Init(TConfigurationNode&) { /* Nothing to do */ }
      virtual void Reset() { /* Nothing to do */ }
      virtual void AddTAM(CTAMEntity& c_tam) = 0;
   };

   /****************************************/
   /****************************************/

   class CTAMsEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CTAMsEntity();
      CTAMsEntity(const std::string& str_id,
                  const std::string& str_experiment_module_path,
                  const std::string& str_rab_medium,
                  const std::string& str_led_medium,
                  const UInt32 un_rand_seed);
      virtual ~CTAMsEntity() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      virtual std::string GetTypeDescription() const {
         return "tams";
      }

      void AddTAM(CTAMEntity& c_tam);

      inline const std::vector<CTAMEntity*>& GetTAMs() {
         return m_vecTAMs;
      }

      inline CTAMsControllableEntity& GetControllableEntity() {
         return *m_pcTAMControllable;
      }

   private:

      CTAMsControllableEntity* m_pcTAMControllable;
      std::vector<CTAMEntity*> m_vecTAMs;
      CRABMedium* m_pcRABMedium;
      CLEDMedium* m_pcLEDMedium;
      UInt32 m_unRandSeed;
   };

   /****************************************/
   /****************************************/

}

#endif
