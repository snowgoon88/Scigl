/*
 * Copyright (C) 2008 Nicolas P. Rougier
 * Modified 2011 Alain Dutech
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 2011/05/08 Tentative pour faire marcher en GLFW.
 */
#include "object.h" 
//#if defined(__APPLE__)
//    #include <Glut/glut.h>
//#else
//    #include <GL/glut.h>
//#endif
#include <GL/glfw.h>

#include <iostream>

#include <cstdlib>
#include "scene.h" 
#include "frame.h"
#include "cloud.h"
#include "line.h"


ScenePtr scene = ScenePtr (new Scene);

// void on_mouse_button (int button, int state, int x, int y) {
//     if (state == GLUT_DOWN) {
//         if (button == GLUT_LEFT_BUTTON) {
//             scene->mouse_action_start ("rotate",x,y);
//         } else if (button == GLUT_MIDDLE_BUTTON) {
//             scene->mouse_action_start ("zoom",x,y);
//         }
//     } else if  (state == GLUT_UP) {
//         scene->mouse_action_end (x,y);
//     }
//     glutPostRedisplay();
// }
void glfw_on_mouse_button( int button, int action )
{
  int x, y;
  glfwGetMousePos( &x, &y);

  //std::cout <<"Mouse Button at (" << x <<  ", " << y << ")\n";
  
  if( action == GLFW_PRESS ) {
    if( button == GLFW_MOUSE_BUTTON_LEFT ) {
      scene->mouse_action_start ("rotate",x,y);
    }
    else if( button == GLFW_MOUSE_BUTTON_MIDDLE ) {
      scene->mouse_action_start( "zoom", x, y);
    }
  }
  else if( action == GLFW_RELEASE ) {
    scene->mouse_action_end( x, y);
  }
}

void on_mouse_move (int x, int y) {
     scene->mouse_action (x,y);
//     glutPostRedisplay();
}

void display (void) {
    glClearColor (1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->render ();
    //glutSwapBuffers();
    // Swap buffers
    glfwSwapBuffers();
}
// void reshape (int width, int height) {
//     glViewport (0,0,width, height);
//     glutPostRedisplay();
// }

int main (int argc, char **argv)
{
    int width, height;

  //glutInit (&argc, argv);
  // Initialise GLFW
  if( !glfwInit() ) {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    exit( EXIT_FAILURE );
  }

  //glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  //glutCreateWindow ("Frame");
  // Open a window and create its OpenGL context
  if( !glfwOpenWindow( 640, 480, 0,0,0,0, 0,0, GLFW_WINDOW ) ) {
    fprintf( stderr, "Failed to open GLFW window\n" );
    
    glfwTerminate();
    exit( EXIT_FAILURE );
  }
  glfwSetWindowTitle( "Frame" );


  //glutReshapeFunc (reshape);
  //glutDisplayFunc (display);
  //glutMouseFunc (on_mouse_button);
  glfwSetMouseButtonCallback( glfw_on_mouse_button );
  //glutMotionFunc (on_mouse_move);
  glfwSetMousePosCallback( on_mouse_move );
  //glutReshapeWindow (400,400);
    
  scene->set_bg_color (1,1,1,1);

  FramePtr frame = FramePtr (new Frame());
  scene->add (frame);

  // Quelques points
  GLdouble data[4*(3+1+4+1+4)];
  for( int i=0; i<4; i++) {
    // Divide by two to be at "frame" scale
    data[i*13+0] = (-0.8 + (i%2) * 1.6) / 2.0;  // x
    data[i*13+1] = (-0.8 + (i%2) * 1) / 2.0;    // y
    data[i*13+2] = (-0.8 + i * 0.8) / 2.0;  // z
    data[i*13+3] = 10.0;    // scale cloud
    data[i*13+4] = 1.0;      // red cloud
    data[i*13+5] = 0.0;                   // green
    data[i*13+6] = 0.0;                   // blue
    data[i*13+7] = 0.8;                             // alpha
    data[i*13+8] = 0.1;    // scale line
    data[i*13+9] = 0.0;      // red line
    data[i*13+10] = 0.0;                   // green
    data[i*13+11] = 0.0;                   // blue
    data[i*13+12] = 1.0;                             // alpha

  }
  // last point
  data[3*13+0] = (-0.8) / 2.0;  // x
  data[3*13+1] = (-0.8) / 2.0;    // y
  data[3*13+2] = (-0.8) / 2.0;  // z

  int n=3;
  DataPtr xl = DataPtr (new Data());
  xl->set (&data[0], n+1, 1, 1, GL_DOUBLE, 13*sizeof(GLdouble));
  
  DataPtr yl = DataPtr (new Data());
  yl->set (&data[1], n+1, 1, 1, GL_DOUBLE, 13*sizeof(GLdouble));
  
  DataPtr zl = DataPtr (new Data());
  zl->set (&data[2], n+1, 1, 1, GL_DOUBLE, 13*sizeof(GLdouble));

  DataPtr sl = DataPtr (new Data());
  sl->set (&data[8], n+1, 1, 1, GL_DOUBLE, 13*sizeof(GLdouble));
  
  DataPtr cl = DataPtr (new Data());
  cl->set (&data[9], n+1, 1, 4, GL_DOUBLE, 13*sizeof(GLdouble));


  LinePtr line = LinePtr(new Line ());
  //line->set_thickness (2.0);
  line->set_thickness (0.01);
  line->set_xyz_s_c_data(xl,yl,zl,sl,cl);
  scene->add (line);

  DataPtr x = DataPtr (new Data());
  x->set (&data[0], n, 1, 1, GL_DOUBLE, 13*sizeof(GLdouble));
  
  DataPtr y = DataPtr (new Data());
  y->set (&data[1], n, 1, 1, GL_DOUBLE, 13*sizeof(GLdouble));
  
  DataPtr z = DataPtr (new Data());
  z->set (&data[2], n, 1, 1, GL_DOUBLE, 13*sizeof(GLdouble));
  
  DataPtr sc = DataPtr (new Data());
  sc->set (&data[3], n, 1, 1, GL_DOUBLE, 13*sizeof(GLdouble));
  
  DataPtr cc = DataPtr (new Data());
  cc->set (&data[4], n, 1, 4, GL_DOUBLE, 13*sizeof(GLdouble));


  CloudPtr cloud = CloudPtr(new Cloud ());
  //cloud->set_thickness (0.0025);
  cloud->set_thickness (1.01);
  cloud->set_xyz_s_c_data(x,y,z,sc,cc);
  scene->add (cloud);  

  
  scene->set_zoom(2.0);


  //glutMainLoop();
  do {
    //t = glfwGetTime();
    //glfwGetMousePos( &x, NULL );
    
    // Get window size (may be different than the requested size)
    glfwGetWindowSize( &width, &height );
    
    // Special case: avoid division by zero below
    height = height > 0 ? height : 1;
    
    glViewport( 0, 0, width, height );
    
//         // Clear color buffer to black
//         glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
//         glClear( GL_COLOR_BUFFER_BIT );

//         // Select and setup the projection matrix
//         glMatrixMode( GL_PROJECTION );
//         glLoadIdentity();
//         gluPerspective( 65.0f, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f );

//         // Select and setup the modelview matrix
//         glMatrixMode( GL_MODELVIEW );
//         glLoadIdentity();
//         gluLookAt( 0.0f, 1.0f, 0.0f,    // Eye-position
//                    0.0f, 20.0f, 0.0f,   // View-point
//                    0.0f, 0.0f, 1.0f );  // Up-vector

//         // Draw a rotating colorful triangle
//         glTranslatef( 0.0f, 14.0f, 0.0f );
//         glRotatef( 0.3f*(GLfloat)x + (GLfloat)t*100.0f, 0.0f, 0.0f, 1.0f );
//         glBegin( GL_TRIANGLES );
//           glColor3f( 1.0f, 0.0f, 0.0f );
//           glVertex3f( -5.0f, 0.0f, -4.0f );
//           glColor3f( 0.0f, 1.0f, 0.0f );
//           glVertex3f( 5.0f, 0.0f, -4.0f );
//           glColor3f( 0.0f, 0.0f, 1.0f );
//           glVertex3f( 0.0f, 0.0f, 6.0f );
//         glEnd();
    display();
    
  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
	 glfwGetWindowParam( GLFW_OPENED ) );
  
  return 0;}
  

