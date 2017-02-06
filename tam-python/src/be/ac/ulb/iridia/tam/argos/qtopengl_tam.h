/**
 * @file <argos3/plugins/robots/e-puck/simulator/qtopengl_tam.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_TAM_H
#define QTOPENGL_TAM_H

namespace argos {
   class CQTOpenGLTAM;
   class CTAMEntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

   class CQTOpenGLTAM {

   public:

      CQTOpenGLTAM();

      virtual ~CQTOpenGLTAM();

      virtual void Draw(CTAMEntity& c_entity);

   protected:

      void MakeBox(GLfloat f_size_x,
                   GLfloat f_size_y,
                   GLfloat f_size_z);
      void MakeBody();
      void MakeLED();

   private:

      GLuint m_unBaseList;
      GLuint m_unBodyList;
      GLuint m_unLEDList;
      GLuint m_unVertices;

   };

}

#endif
