/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/models/qtopengl_tam.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_tam.h"
#include "tam_entity.h"
#include "tams_entity.h"
#include <argos3/core/utility/math/vector2.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const GLfloat BODY_COLOR[]     = { 1.0f, 1.0f, 1.0f, 1.0f };
   static const GLfloat BODY_SPECULAR[]  = { 0.6f, 0.6f, 0.6f, 1.0f };
   static const GLfloat BODY_SHININESS[] = { 0.5f                   };
   static const GLfloat BODY_EMISSION[]  = { 0.0f, 0.0f, 0.0f, 1.0f };

   static const GLfloat LED_RADIUS      = 0.01f;
   static const GLfloat LED_SPECULAR[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
   static const GLfloat LED_SHININESS[] = { 100.0f                   };
   static const GLfloat LED_EMISSION[]  = {   0.0f, 0.0f, 0.0f, 1.0f };

   /****************************************/
   /****************************************/

   CQTOpenGLTAM::CQTOpenGLTAM() :
      m_unVertices(20){
      /* Reserve the needed display lists */
      m_unBaseList = glGenLists(2);
      m_unBodyList = m_unBaseList;
      m_unLEDList = m_unBaseList + 1;
      /* Make body list */
      glNewList(m_unBodyList, GL_COMPILE);
      MakeBody();
      glEndList();
      /* Make LED list */
      glNewList(m_unLEDList, GL_COMPILE);
      MakeLED();
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLTAM::~CQTOpenGLTAM() {
      glDeleteLists(m_unBaseList, 2);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLTAM::Draw(CTAMEntity& c_entity) {
      /* Draw body */
      glPushMatrix();
      glCallList(m_unBodyList);
      glPopMatrix();
      /* Draw LEDs */
      /* Top */
      const CColor& cColor0 = c_entity.GetLEDEquippedEntity().GetLED(0).GetColor();
      GLfloat pfColor0[] = { static_cast<GLfloat>(cColor0.GetRed())   / 255.0f,
                            static_cast<GLfloat>(cColor0.GetGreen()) / 255.0f,
                            static_cast<GLfloat>(cColor0.GetBlue())  / 255.0f,
                            1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor0);
      const CVector3& cPosition0 = c_entity.GetLEDEquippedEntity().GetLEDOffset(0);
      glPushMatrix();
      glTranslatef(cPosition0.GetX(), cPosition0.GetY(), cPosition0.GetZ());
      glCallList(m_unLEDList);
      glPopMatrix();
      /* Body */
      /* left */
      const CColor& cColor1 = c_entity.GetLEDEquippedEntity().GetLED(1).GetColor();
      GLfloat pfColor1[] = { static_cast<GLfloat>(cColor1.GetRed())   / 255.0f,
                            static_cast<GLfloat>(cColor1.GetGreen()) / 255.0f,
                            static_cast<GLfloat>(cColor1.GetBlue())  / 255.0f,
                            1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor1);
      const CVector3& cPosition1 = c_entity.GetLEDEquippedEntity().GetLEDOffset(1);
      glPushMatrix();
      glTranslatef(cPosition1.GetX(), cPosition1.GetY(), cPosition1.GetZ());
      glCallList(m_unLEDList);
      glPopMatrix();
      /* middle */
      const CColor& cColor2 = c_entity.GetLEDEquippedEntity().GetLED(2).GetColor();
      GLfloat pfColor2[] = { static_cast<GLfloat>(cColor2.GetRed())   / 255.0f,
                            static_cast<GLfloat>(cColor2.GetGreen()) / 255.0f,
                            static_cast<GLfloat>(cColor2.GetBlue())  / 255.0f,
                            1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor2);
      const CVector3& cPosition2 = c_entity.GetLEDEquippedEntity().GetLEDOffset(2);
      glPushMatrix();
      glTranslatef(cPosition2.GetX(), cPosition2.GetY(), cPosition2.GetZ());
      glCallList(m_unLEDList);
      glPopMatrix();
      /* right */
      const CColor& cColor3 = c_entity.GetLEDEquippedEntity().GetLED(3).GetColor();
      GLfloat pfColor3[] = { static_cast<GLfloat>(cColor3.GetRed())   / 255.0f,
                            static_cast<GLfloat>(cColor3.GetGreen()) / 255.0f,
                            static_cast<GLfloat>(cColor3.GetBlue())  / 255.0f,
                            1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor3);
      const CVector3& cPosition3 = c_entity.GetLEDEquippedEntity().GetLEDOffset(3);
      glPushMatrix();
      glTranslatef(cPosition3.GetX(), cPosition3.GetY(), cPosition3.GetZ());
      glCallList(m_unLEDList);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLTAM::MakeBox(GLfloat f_size_x,
                              GLfloat f_size_y,
                              GLfloat f_size_z) {
      /* Set the material */
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, BODY_COLOR);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            BODY_SPECULAR);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           BODY_SHININESS);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            BODY_EMISSION);

      /* Calculate half sizes */
      GLfloat fHalfSizeX = f_size_x * 0.5f;
      GLfloat fHalfSizeY = f_size_y * 0.5f;

      /* Let's start the actual shape */

      /* This part covers the top and bottom faces (parallel to XY) */
      glBegin(GL_QUADS);
      /* Bottom face */
      glNormal3f(0.0f, 0.0f, -1.0f);
      glVertex3f( fHalfSizeX,  fHalfSizeY, 0.0f);
      glVertex3f( fHalfSizeX, -fHalfSizeY, 0.0f);
      glVertex3f(-fHalfSizeX, -fHalfSizeY, 0.0f);
      glVertex3f(-fHalfSizeX,  fHalfSizeY, 0.0f);
      /* Top face */
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(-fHalfSizeX, -fHalfSizeY, f_size_z);
      glVertex3f( fHalfSizeX, -fHalfSizeY, f_size_z);
      glVertex3f( fHalfSizeX,  fHalfSizeY, f_size_z);
      glVertex3f(-fHalfSizeX,  fHalfSizeY, f_size_z);
      glEnd();
      /* This part covers the faces (South, East, North, West) */
      glBegin(GL_QUADS);
      /* South face */
      glNormal3f(0.0f, -1.0f, 0.0f);
      glVertex3f(-fHalfSizeX, -fHalfSizeY, f_size_z);
      glVertex3f(-fHalfSizeX, -fHalfSizeY, 0.0f);
      glVertex3f( fHalfSizeX, -fHalfSizeY, 0.0f);
      glVertex3f( fHalfSizeX, -fHalfSizeY, f_size_z);
      /* East face */
      glNormal3f(1.0f, 0.0f, 0.0f);
      glVertex3f(fHalfSizeX, -fHalfSizeY, f_size_z);
      glVertex3f(fHalfSizeX, -fHalfSizeY, 0.0f);
      glVertex3f(fHalfSizeX,  fHalfSizeY, 0.0f);
      glVertex3f(fHalfSizeX,  fHalfSizeY, f_size_z);
      /* North face */
      glNormal3f(0.0f, 1.0f, 0.0f);
      glVertex3f( fHalfSizeX,  fHalfSizeY, f_size_z);
      glVertex3f( fHalfSizeX,  fHalfSizeY, 0.0f);
      glVertex3f(-fHalfSizeX,  fHalfSizeY, 0.0f);
      glVertex3f(-fHalfSizeX,  fHalfSizeY, f_size_z);
      /* West face */
      glNormal3f(-1.0f, 0.0f, 0.0f);
      glVertex3f(-fHalfSizeX,  fHalfSizeY, f_size_z);
      glVertex3f(-fHalfSizeX,  fHalfSizeY, 0.0f);
      glVertex3f(-fHalfSizeX, -fHalfSizeY, 0.0f);
      glVertex3f(-fHalfSizeX, -fHalfSizeY, f_size_z);
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLTAM::MakeBody() {
      /* Left side */
      glTranslatef(0.0f, 0.0525f, 0.0f);
      MakeBox(0.12f,  0.010f, 0.12f);
      /* Right side */
      glTranslatef(0.0f, -0.105f, 0.0f);
      MakeBox(0.12f,  0.010f, 0.12f);
      /* Back side */
      glTranslatef(0.048f, 0.0525f, 0.0f);
      MakeBox(0.024f, 0.10f, 0.12f);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLTAM::MakeLED() {
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            LED_SPECULAR);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           LED_SHININESS);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            LED_EMISSION);
      CVector3 cNormal, cPoint;
      CRadians cSlice(CRadians::TWO_PI / m_unVertices);
      glBegin(GL_TRIANGLE_STRIP);
      for(CRadians cInclination; cInclination <= CRadians::PI; cInclination += cSlice) {
         for(CRadians cAzimuth; cAzimuth <= CRadians::TWO_PI; cAzimuth += cSlice) {
            cNormal.FromSphericalCoords(1.0f, cInclination, cAzimuth);
            cPoint = LED_RADIUS * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());
            cNormal.FromSphericalCoords(1.0f, cInclination + cSlice, cAzimuth);
            cPoint = LED_RADIUS * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());
            cNormal.FromSphericalCoords(1.0f, cInclination, cAzimuth + cSlice);
            cPoint = LED_RADIUS * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());
            cNormal.FromSphericalCoords(1.0f, cInclination + cSlice, cAzimuth + cSlice);
            cPoint = LED_RADIUS * cNormal;
            glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
            glVertex3f(cPoint.GetX(), cPoint.GetY(), cPoint.GetZ());
         }
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawTAMNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CTAMsEntity& c_entity) {
         static CQTOpenGLTAM m_cModel;
         for(size_t i = 0; i < c_entity.GetTAMs().size(); ++i) {
            glPushMatrix();
            c_visualization.DrawRays(c_entity.GetControllableEntity());
            c_visualization.DrawEntity(c_entity.GetTAMs()[i]->GetEmbodiedEntity());
            m_cModel.Draw(*c_entity.GetTAMs()[i]);
            glPopMatrix();
         }
      }
   };

   class CQTOpenGLOperationDrawTAMSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CTAMsEntity& c_entity) {
         //c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
         //TODO
      }
   };

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawTAMNormal, CTAMsEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawTAMSelected, CTAMsEntity);

   /****************************************/
   /****************************************/

}
