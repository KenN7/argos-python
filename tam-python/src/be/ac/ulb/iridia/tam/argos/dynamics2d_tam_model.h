/**
 * @file <argos3/plugins/robots/tam/simulator/dynamics2d_tam_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_TAM_MODEL_H
#define DYNAMICS2D_TAM_MODEL_H

namespace argos {
   class CDynamics2DDifferentialSteeringControl;
   class CDynamics2DGripper;
   class CDynamics2DGrippable;
}


#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_single_body_object_model.h>
#include <argos3/plugins/robots/tam/simulator/tam_entity.h>

namespace argos {

   class CDynamics2DTAMModel : public CDynamics2DSingleBodyObjectModel {

   public:

      CDynamics2DTAMModel(CDynamics2DEngine& c_engine,
                          CTAMEntity& c_entity);
      virtual ~CDynamics2DTAMModel();

      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const;

      virtual void Reset() {}

      virtual void CalculateBoundingBox();

      //virtual void UpdateEntityStatus() {}
      virtual void UpdateFromEntityStatus() {}

      //virtual bool IsCollidingWithSomething() const;

   private:

      CTAMEntity& m_cTAMEntity;
      cpBody*  m_ptBody;
      cpShape* m_ptShapeSideL;
      cpShape* m_ptShapeSideR;
      cpShape* m_ptShapeBack;
   };

}

#endif
