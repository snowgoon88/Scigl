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

#include <vector>

#include <cstdlib>
#include "scene.h" 
#include "basis-cube.h"
#include "curve.h"
#include "data.h"

/**
 * Test of Curve.
 * The idea is to have points of the Oxy plane, taking into account the range 
 * of coordinates on the plane.
 *
 * Warning with PlaneCoord : scaling is done according to range, but not translation
 * of points...
 */


ScenePtr scene = ScenePtr (new Scene);
GLdouble *data;

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

/**
 * Project one coordinate on a Range.
 */
float project_on_range( float x, Range &r )
{
  return (x - r.min) / (r.max - r.min );
}
/**
 *
 */
void set_data( std::vector<Vec4f> &v_vec, const Color &col,
	       Range &range_x, Range &range_y,
	       Position &pos)
{
  if( data != NULL ) delete [] data;

  data = new GLdouble[v_vec.size() * (3+4)]; // x,y,z,col
  for( unsigned int i=0; i < v_vec.size(); i++ ) {
    // x,y,z
    data[i*7+0] = (GLdouble) project_on_range( v_vec[i].x, range_x) + pos.x;
    data[i*7+1] = (GLdouble) project_on_range( v_vec[i].y, range_y) + pos.y;
    data[i*7+2] = (GLdouble) v_vec[i].z + pos.z;
    // color
    data[i*7+3] = (GLdouble) col.red;
    data[i*7+4] = (GLdouble) col.green;
    data[i*7+5] = (GLdouble) col.blue;
    data[i*7+6] = (GLdouble) col.alpha;

    std::cout << "data[" << i << "]=";
    for( unsigned int j=0; j<7; j++) {
      std::cout << " " << data[i*7+j];
    }
    std::cout << std::endl;
  }
}

int main (int argc, char **argv)
{
  int width, height;
  data = NULL;
  
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
  
  // Background color for scene
  scene->set_bg_color (1,1,1,1);

  // Add a system coordinate
  Position pos_rep( -0.5, -0.5, -0.5, 0);
  Range rg_x( 0, 200, 4, 4*5);
  Range rg_y( -4, 4, 4, 4*5);
  Range rg_z( -1, 1, 4, 4*5);
  
  BasisCubePtr coord = BasisCubePtr (new BasisCube());
  coord->set_range_coord_x( rg_x );
  coord->set_range_coord_y( rg_y );
  coord->set_range_coord_z( rg_z );
  coord->set_title_coord_x("X");
  coord->set_title_coord_y("Y");
  coord->set_title_coord_z("Z");
  coord->set_position( pos_rep );
  scene->add ( coord );


  CurvePtr curve = CurvePtr (new Curve());
  Range rg_curve( 0.0, 200.0, 4, 4*5 );
  curve->set_range_coordX( rg_curve );
  curve->set_range_coordY( rg_y );
  curve->set_range_coordZ( rg_z );
  curve->set_thickness( 1.5 );
  curve->set_position( pos_rep );

  scene->add( curve );
 
  CurvePtr curve2 = CurvePtr (new Curve());
  curve2->set_range_coordX( rg_curve );
  curve2->set_range_coordY( rg_y );
  curve2->set_range_coordZ( rg_z );
  curve2->set_fg_color( 1.0, 0.0, 0.0, 1.0);
  curve2->set_thickness( 1.5 );
  curve2->set_position( pos_rep );

  scene->add( curve2 );

  // Add points to be displayed
//   std::vector<Vec4f> v_vec;
//   v_vec.push_back( Vec4f(0.5, 0.5, 0, 0));
//   v_vec.push_back( Vec4f(0, 0, 0, 0));
//   v_vec.push_back( Vec4f(0.75, -0.5, 0, 0));
//   v_vec.push_back( Vec4f(0.25, 1.1, 0, 0));
//   for(unsigned int i=0; i < v_vec.size(); i++) {
//     std::cout << "v[" << i << "] = " << v_vec[i].x << " " << v_vec[i].y << " " << v_vec[i].z << "\n"; 
//   }
//   set_data( v_vec, Color( 1.0, 0, 0, 1), rg_x, rg_y, pos_rep);

//   DataPtr xx = DataPtr (new Data());
//   xx->set (&data[0], v_vec.size(), 1, 1, GL_DOUBLE, 7*sizeof(GLdouble));
//   DataPtr yy = DataPtr (new Data());
//   yy->set (&data[1], v_vec.size(), 1, 1, GL_DOUBLE, 7*sizeof(GLdouble));
//   DataPtr zz = DataPtr (new Data());
//   zz->set (&data[2], v_vec.size(), 1, 1, GL_DOUBLE, 7*sizeof(GLdouble));

//   DataPtr cc = DataPtr (new Data());
//   cc->set (&data[3], v_vec.size(), 1, 4, GL_DOUBLE, 7*sizeof(GLdouble));

//   CloudPtr cloud = CloudPtr(new Cloud ());
//   //cloud->set_thickness (0.1);
//   cloud->set_thickness (5.01); // using GL_POINTS
//   cloud->set_xyz_c_data(xx,yy,zz,cc);
//   //cloud->set_scale( 5 );
//   std::cout << "data added\n";
//   scene->add (cloud);    
//   cloud->set_position( Position( -0.5, -0.5, 0, 0));

  scene->set_zoom(1.8);
  scene->set_orientation( 35.0, 0 );


  // Main loop of GLFW
  int count = 0;
  do {
    // Get window size (may be different than the requested size)
    glfwGetWindowSize( &width, &height );
    // Special case: avoid division by zero below
    height = height > 0 ? height : 1;
    
    glViewport( 0, 0, width, height );
    if( count % 10  == 0) {
      curve->add_yz( 1.0 + sin( (double) (count/10) * 3 / M_PI ), 0.0);
      curve2->add_yz( cos( (double) (count/10) * 3 / M_PI ), -0.5);
    }
    count ++;

    // display
    display();
    
  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
	 glfwGetWindowParam( GLFW_OPENED ) );
  
  return 0;
}


