/**
 * @file <argos3/plugins/robots/e-puck/simulator/dynamics2d_tam_model.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "dynamics2d_tam_model.h"
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.h>
#include <argos3/plugins/robots/tam/simulator/tams_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real TAM_HEIGHT = 0.12f;

   /****************************************/
   /****************************************/

   CDynamics2DTAMModel::CDynamics2DTAMModel(CDynamics2DEngine& c_engine,
                                            CTAMEntity& c_entity) :
      CDynamics2DSingleBodyObjectModel(c_engine, c_entity),
      m_cTAMEntity(c_entity) {
      /* Create a polygonal object in the physics space */
      /* Start defining the vertices
         NOTE: points must be defined in a clockwise winding
      */
       cpVect tVerticesSideL[] = {
          cpv(-0.060f, 0.060),
          cpv( 0.060f, 0.060),
          cpv( 0.060f, 0.05),
          cpv(-0.060f, 0.05),
       };
       cpVect tVerticesSideR[] = {
          cpv(-0.060f, -0.05),
          cpv( 0.060f, -0.05),
          cpv( 0.060f, -0.060),
          cpv(-0.060f, -0.060),
       };
       cpVect tVerticesBack[] = {
          cpv(0.036f,  0.05),
          cpv(0.060f,  0.05),
          cpv(0.060f, -0.05),
          cpv(0.036f, -0.05),
       };
      /* Get body position and orientation */
      const CVector3& cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      CRadians cXAngle, cYAngle, cZAngle;
      GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      /* Roto-translate the vertices */
      cpVect tRot = cpvforangle(cZAngle.GetValue());
      cpVect tTrans = cpv(cPosition.GetX(), cPosition.GetY());
      tVerticesSideL[0] = cpvadd(cpvrotate(tVerticesSideL[0], tRot), tTrans);
      tVerticesSideL[1] = cpvadd(cpvrotate(tVerticesSideL[1], tRot), tTrans);
      tVerticesSideL[2] = cpvadd(cpvrotate(tVerticesSideL[2], tRot), tTrans);
      tVerticesSideL[3] = cpvadd(cpvrotate(tVerticesSideL[3], tRot), tTrans);
      tVerticesSideR[0] = cpvadd(cpvrotate(tVerticesSideR[0], tRot), tTrans);
      tVerticesSideR[1] = cpvadd(cpvrotate(tVerticesSideR[1], tRot), tTrans);
      tVerticesSideR[2] = cpvadd(cpvrotate(tVerticesSideR[2], tRot), tTrans);
      tVerticesSideR[3] = cpvadd(cpvrotate(tVerticesSideR[3], tRot), tTrans);
      tVerticesBack[0] = cpvadd(cpvrotate(tVerticesBack[0], tRot), tTrans);
      tVerticesBack[1] = cpvadd(cpvrotate(tVerticesBack[1], tRot), tTrans);
      tVerticesBack[2] = cpvadd(cpvrotate(tVerticesBack[2], tRot), tTrans);
      tVerticesBack[3] = cpvadd(cpvrotate(tVerticesBack[3], tRot), tTrans);
      /* Create a static body */
      m_ptBody = cpBodyNewStatic();
      m_ptBody->data = this;
      /* Create the shapes */
      m_ptShapeSideL =
         cpSpaceAddShape(GetDynamics2DEngine().GetPhysicsSpace(),
                         cpPolyShapeNew(m_ptBody,
                                        4,
                                        tVerticesSideL,
                                        cpvzero));
      m_ptShapeSideR =
         cpSpaceAddShape(GetDynamics2DEngine().GetPhysicsSpace(),
                         cpPolyShapeNew(m_ptBody,
                                        4,
                                        tVerticesSideR,
                                        cpvzero));
      m_ptShapeBack =
         cpSpaceAddShape(GetDynamics2DEngine().GetPhysicsSpace(),
                         cpPolyShapeNew(m_ptBody,
                                        4,
                                        tVerticesBack,
                                        cpvzero));
      /* Configure the shapes */
      m_ptShapeSideL->e = 0.0; // No elasticity
      m_ptShapeSideL->u = 0.1; // Little contact friction to help sliding away
      m_ptShapeSideL->collision_type = CDynamics2DEngine::SHAPE_NORMAL;
      m_ptShapeSideR->e = 0.0; // No elasticity
      m_ptShapeSideR->u = 0.1; // Little contact friction to help sliding away
      m_ptShapeSideR->collision_type = CDynamics2DEngine::SHAPE_NORMAL;
      m_ptShapeBack->e = 0.0; // No elasticity
      m_ptShapeBack->u = 0.1; // Little contact friction to help sliding away
      m_ptShapeBack->collision_type = CDynamics2DEngine::SHAPE_NORMAL;
      /* Associate this model to the body data for ray queries */
//      m_ptShapeSideL->data = this;
//      m_ptShapeSideR->data = this;
//      m_ptShapeBack->data  = this;
      SetBody(m_ptBody,TAM_HEIGHT);
      //CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   CDynamics2DTAMModel::~CDynamics2DTAMModel() {
//      cpSpaceRemoveShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptShapeSideL);
//      cpSpaceRemoveShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptShapeSideR);
//      cpSpaceRemoveShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptShapeBack);
//      cpShapeFree(m_ptShapeSideL);
//      cpShapeFree(m_ptShapeSideR);
//      cpShapeFree(m_ptShapeBack);
//      cpBodyFree(m_ptBody);
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DTAMModel::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                      const CRay3& c_ray) const {
      cpSegmentQueryInfo tInfo;
      cpVect tRayStart = cpv(c_ray.GetStart().GetX(), c_ray.GetStart().GetY());
      cpVect tRayEnd   = cpv(c_ray.GetEnd().GetX()  , c_ray.GetEnd().GetY()  );
      f_t_on_ray = 2.0;
      if(cpShapeSegmentQuery(m_ptShapeSideL, tRayStart, tRayEnd, &tInfo)) {
         CVector3 cIntersectionPoint;
         c_ray.GetPoint(cIntersectionPoint, tInfo.t);
         if((cIntersectionPoint.GetZ() >= GetEmbodiedEntity().GetOriginAnchor().Position.GetZ()               ) &&
            (cIntersectionPoint.GetZ() <= GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + TAM_HEIGHT) ) {
            f_t_on_ray = tInfo.t;
         }
      }
      if(cpShapeSegmentQuery(m_ptShapeSideR, tRayStart, tRayEnd, &tInfo)) {
         CVector3 cIntersectionPoint;
         c_ray.GetPoint(cIntersectionPoint, tInfo.t);
         if((cIntersectionPoint.GetZ() >= GetEmbodiedEntity().GetOriginAnchor().Position.GetZ()               ) &&
            (cIntersectionPoint.GetZ() <= GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + TAM_HEIGHT) ) {
            f_t_on_ray = Min(f_t_on_ray, tInfo.t);
         }
      }
      if(cpShapeSegmentQuery(m_ptShapeBack, tRayStart, tRayEnd, &tInfo)) {
         CVector3 cIntersectionPoint;
         c_ray.GetPoint(cIntersectionPoint, tInfo.t);
         if((cIntersectionPoint.GetZ() >= GetEmbodiedEntity().GetOriginAnchor().Position.GetZ()               ) &&
            (cIntersectionPoint.GetZ() <= GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + TAM_HEIGHT) ) {
            f_t_on_ray = Min(f_t_on_ray, tInfo.t);
         }
      }
      return f_t_on_ray < 2.0f;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DTAMModel::CalculateBoundingBox() {
      GetBoundingBox().MinCorner.SetX(Min(Min(m_ptShapeSideL->bb.l, m_ptShapeSideR->bb.l), m_ptShapeBack->bb.l));
      GetBoundingBox().MinCorner.SetY(Min(Min(m_ptShapeSideL->bb.b, m_ptShapeSideR->bb.b), m_ptShapeBack->bb.b));
      GetBoundingBox().MinCorner.SetZ(0.0f);
      GetBoundingBox().MaxCorner.SetX(Max(Max(m_ptShapeSideL->bb.r, m_ptShapeSideR->bb.r), m_ptShapeBack->bb.r));
      GetBoundingBox().MaxCorner.SetY(Max(Max(m_ptShapeSideL->bb.t, m_ptShapeSideR->bb.t), m_ptShapeBack->bb.t));
      GetBoundingBox().MaxCorner.SetZ(TAM_HEIGHT);
   }

   /****************************************/
   /****************************************/

//   bool CDynamics2DTAMModel::IsCollidingWithSomething() const {
//      return
//         (cpSpaceShapeQuery(m_cDyn2DEngine.GetPhysicsSpace(), m_ptShapeSideL, NULL, NULL) > 0) ||
//         (cpSpaceShapeQuery(m_cDyn2DEngine.GetPhysicsSpace(), m_ptShapeSideR, NULL, NULL) > 0) ||
//         (cpSpaceShapeQuery(m_cDyn2DEngine.GetPhysicsSpace(), m_ptShapeBack, NULL, NULL) > 0);
//   }

   /****************************************/
   /****************************************/

   class CDynamics2DOperationAddCTAMsEntity : public CDynamics2DOperationAddEntity {
   public:
      CDynamics2DOperationAddCTAMsEntity() {}
      virtual ~CDynamics2DOperationAddCTAMsEntity() {}
      SOperationOutcome ApplyTo(CDynamics2DEngine& c_engine,
                   CTAMsEntity& c_entity) {
         const std::vector<CTAMEntity*>& cTAMs = c_entity.GetTAMs();
         for(size_t i = 0; i < cTAMs.size(); ++i) {
            c_engine.AddEntity(*cTAMs[i]);
         }
         return SOperationOutcome(true);
      }
   };
   REGISTER_DYNAMICS2D_OPERATION(CDynamics2DOperationAddEntity,
                                 CDynamics2DOperationAddCTAMsEntity,
                                 CTAMsEntity);

   class CDynamics2DOperationRemoveCTAMsEntity : public CDynamics2DOperationRemoveEntity {
   public:
      CDynamics2DOperationRemoveCTAMsEntity() {}
      virtual ~CDynamics2DOperationRemoveCTAMsEntity() {}
      SOperationOutcome ApplyTo(CDynamics2DEngine& c_engine,
                   CTAMsEntity& c_entity) {
         const std::vector<CTAMEntity*>& cTAMs = c_entity.GetTAMs();
         for(size_t i = 0; i < cTAMs.size(); ++i) {
            c_engine.RemoveEntity(*cTAMs[i]);
         }
         return SOperationOutcome(true);
      }
   };
   REGISTER_DYNAMICS2D_OPERATION(CDynamics2DOperationRemoveEntity,
                                 CDynamics2DOperationRemoveCTAMsEntity,
                                 CTAMsEntity);

   REGISTER_STANDARD_DYNAMICS2D_OPERATIONS_ON_ENTITY(CTAMEntity, CDynamics2DTAMModel);

   /****************************************/
   /****************************************/

}
