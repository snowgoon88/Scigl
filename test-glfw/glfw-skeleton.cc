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

/**
 * Skeleton for using GLFW with SciGL library.
 *
 */


ScenePtr scene = ScenePtr (new Scene);

/**
 * Called after every mouse button pressed.
 * see glfwSetMouseButtonCallback().
 */
void on_mouse_button( int button, int action )
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
/**
 * Called when the mouse moves.
 * see glfwSetMousePosCallback().
 */
void on_mouse_move (int x, int y) {
  scene->mouse_action (x,y);
  // TODO No equivalence for "glutPostRedisplay()"; ==> needed ??
}
/**
 * Called when a key is pressed.
 * see glfwSetKeyCallback().
 */
void on_key_pressed( int key, int action)
{
  // g or G : display orientation of scene
  if( ((key == 103) || (key == 71))  && (action == GLFW_PRESS)) {
    Orientation orien = scene->get_orientation();
    std::cout << "Orient = (" << orien.x << ", " << orien.y << ")\n";
  }
}

void display (void) {
    glClearColor (1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->render ();
    // Swap buffers
    glfwSwapBuffers();
}
// TODO ==> needed? Right now, done each time in the main loop.
// void reshape (int width, int height) {
//     glViewport (0,0,width, height);
//     glutPostRedisplay();
// }

int main (int argc, char **argv)
{
  int width, height;
  
  // Initialise GLFW
  if( !glfwInit() ) {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    exit( EXIT_FAILURE );
  }
  
  // TODO Check that same parameters are used than : glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  // Open a window and create its OpenGL context
  if( !glfwOpenWindow( 640, 480, 0,0,0,0, 0,0, GLFW_WINDOW ) ) {
    fprintf( stderr, "Failed to open GLFW window\n" );
    
    glfwTerminate();
    exit( EXIT_FAILURE );
  }
  glfwSetWindowTitle( "Frame" );
  
  
  //glutReshapeFunc (reshape);
  //glutDisplayFunc (display);
  glfwSetMouseButtonCallback( on_mouse_button );
  glfwSetMousePosCallback( on_mouse_move );
  glfwSetKeyCallback( on_key_pressed );
  //glutReshapeWindow (400,400);
  
std::cout << "Init GLEW\n";
  if( !glewInit() ) {
    fprintf( stderr, "Failed to initialize GLEW\n" );
  }
  if (glewIsSupported("GL_EXT_framebuffer_object"))
    std::cout<<"Old EXT FBO available"<<std::endl;
  else
    std::cout<<"Old EXT FBO NOT available"<<std::endl;
  if (glewIsSupported("GL_ARB_framebuffer_object"))
    std::cout<<"Newer ARB FBO available"<<std::endl;
  else
    std::cout<<"Newer ARB FBO NOT available"<<std::endl;

  // Background color for scene
  scene->set_bg_color (1,1,1,1);
  // Add axis or coordinate
  FramePtr frame = FramePtr (new Frame());
  scene->add (frame);

  scene->set_zoom(2.0);


  // Main loop of GLFW
  do {
    // Get window size (may be different than the requested size)
    glfwGetWindowSize( &width, &height );
    // Special case: avoid division by zero below
    height = height > 0 ? height : 1;
    
    glViewport( 0, 0, width, height );
    
    // display
    display();
    
  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
	 glfwGetWindowParam( GLFW_OPENED ) );
  
  return 0;
}


