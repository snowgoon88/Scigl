// -*- coding: utf-8 -*-

#include <iostream>

#include <GL/glew.h>
#include <GL/glfw.h>

//#include <cstdlib>
#include <boost/thread.hpp>
#include "scene.h"
#include "plane-coord.h"

#include "logged_vector.h"
#include "logged_vector_scigl.h"
#include "logged_vector_control.h"

// GRAPHIC
ScenePtr _scene = ScenePtr (new Scene);
PlaneCoordPtr _coord_rec;

// CONTROL
LoggedVectorControlPtr _vec_control;
// MODEL
LoggedVectorPtr _vec_model = LoggedVectorPtr( new LoggedVector );
// VIEW
LoggedVectorSciglPtr _vec_obs1;
LoggedVectorSciglPtr _vec_obs2;

// Slow down things
boost::posix_time::millisec td(1);

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------
/**
 * Called after every mouse button pressed.
 * see glfwSetMouseButtonCallback().
 *
 * @todo Un controler pourrait offrir une methode on_mouse_button, 
 * ou on pourait essayer de le detacher de Object
 */
void on_mouse_button( int button, int action )
{
  int x, y;
  glfwGetMousePos( &x, &y);

  //std::cout <<"Mouse Button at (" << x <<  ", " << y << ")\n";
  
  if( action == GLFW_PRESS ) {
    if( button == GLFW_MOUSE_BUTTON_LEFT ) {
      // With SHIFT ??
      if( glfwGetKey( GLFW_KEY_LSHIFT ) || glfwGetKey( GLFW_KEY_RSHIFT)) {
	_scene->mouse_action_start ("move-resize",x,y);
      }
      else {
	_scene->mouse_action_start ("rotate",x,y);
      }
    }
    else if( button == GLFW_MOUSE_BUTTON_RIGHT ) {
      _scene->mouse_action_start( "zoom", x, y);
    }
  }
  else if( action == GLFW_RELEASE ) {
    _scene->mouse_action_end( x, y);
  }
}
/**
 * Called when the mouse moves.
 * see glfwSetMousePosCallback().
 */
void on_mouse_move (int x, int y) {
  _scene->mouse_action (x,y);
  // TODO No equivalence for "glutPostRedisplay()"; ==> needed ??
}
/**
 * Called when a key is pressed.
 * see glfwSetKeyCallback().
 */
void on_key_pressed( int key, int action)
{
  // o or O : display orientation of scene
  if( ((key == 111) || (key == 79))  && (action == GLFW_PRESS)) {
    Orientation orien = _scene->get_orientation();
    std::cout << "Orient = (" << orien.x << ", " << orien.y << ")\n";
  }
  // p or P : plan 0xy
  else if( ((key == 112) || (key == 80))  && (action == GLFW_PRESS)) {
    _scene->set_orientation( 0, 0);
  }
  // t or T : toggle full/window mode
  else if( ((key == 116) || (key == 84)) && (action == GLFW_PRESS)) {
    if( _vec_control->is_window_mode() == true ) {
      _vec_control->set_full_mode();
    }
    else {
      _vec_control->set_window_mode();
    }
  }
}
/**
 * Compute what is to be drawn.
 */
void display (void) {
    glClearColor (1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _scene->render ();
    // Swap buffers
    glfwSwapBuffers();
}
// ------------------------------------------------------------------------------------
/**
 * Initialize the Graphical Objects.
 */
void init_graphic()
{

  // Initialise GLFW
  if( !glfwInit() ) {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    exit( EXIT_FAILURE );
  }

  // Open a window and create its OpenGL context
  if( !glfwOpenWindow( 600, 480, 0,0,0,0, 0,0, GLFW_WINDOW ) ) {
    fprintf( stderr, "Failed to open GLFW window\n" );
    
    glfwTerminate();
    exit( EXIT_FAILURE );
  }
  glfwSetWindowTitle( "Skel GLFW" );
  
  glfwSetMouseButtonCallback( on_mouse_button );
  glfwSetMousePosCallback( on_mouse_move );
  glfwSetKeyCallback( on_key_pressed );

  // Background color for scene
  _scene->set_bg_color (1,1,1,1);

  // Add a system coordinate
  Position pos_rep( -0.5, -0.5, -0.5, 0);
  Range rg_x( 0, 1, 4, 4*5);
  Range rg_y( -0.5, 0.5, 4, 4*5);
  Range rg_z( 0, 1, 4, 4*5);
  
  // Add a system coordinate
  _coord_rec = PlaneCoordPtr (new PlaneCoord());
  _coord_rec->set_title_coord1( "LastIter" );
  Range rg_x_rec( 0, 200, 5, 5*5);
  _coord_rec->set_range_coord1( rg_x_rec );
  _coord_rec->set_title_coord2( "Avg. Reward" );
  Range rg_y_rec( -2.0, 2.0, 5, 5*5);
  _coord_rec->set_range_coord2( rg_y_rec );
  Position pos_rep_rec( -0.45, -0.45, 0, 0);
  _coord_rec->set_position( pos_rep_rec );
  Size size_rep_rec( 1.5, 1.5, 1, 0.0);
  _coord_rec->set_size( size_rep_rec );
  _scene->add ( _coord_rec );

  // Initialise Controler
  _vec_control = LoggedVectorControlPtr( new LoggedVectorControl( _coord_rec ));
  // Initialise Model and Observer
  _vec_obs1 = _vec_control->add_viewer( _vec_model, 1 );
  _vec_obs2 = _vec_control->add_viewer( _vec_model, 2 );
  _vec_obs2->set_thickness( 2.5 );
  _vec_obs2->set_fg_color( 1.0, 0.0, 0.0 );

  _scene->add( _vec_obs1 );
  _scene->add( _vec_obs2 );

  _scene->set_orientation( 0, 0);
  _scene->setup();
  _scene->update();

}
/**
 * The Graphical Loop.
 */
void run_graphic()
{
  // Some graphic parameters
  int width, height;

  // Main loop of GLFW
  int nb_iter = 0;
  do {
    // Get window size (may be different than the requested size)
    glfwGetWindowSize( &width, &height );
    // Special case: avoid division by zero below
    height = height > 0 ? height : 1;
    
    glViewport( 0, 0, width, height );

    // add to _vec_model
    if( nb_iter % 10 == 0 ) {
      LoggedVector::T_Vect new_v(3); 
      new_v << (float) nb_iter / 100.0 ,
	2.0 * sinf((float) nb_iter / 100.0),
	1.0 + cosf((float) nb_iter / 100.0);
      _vec_model->add_vector( nb_iter, new_v );
    }
  
    // display
    display();
    boost::this_thread::sleep(td);
    
    nb_iter++;

  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
	 glfwGetWindowParam( GLFW_OPENED ) );
  
}
/**
 * Callable for boost::thread
 */
void graphic_thread()
{
  std::cout << "graphic_thread init\n";
  init_graphic();
  std::cout << "graphic_thread run\n";
  run_graphic();
}
// ------------------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
  graphic_thread();

  return 0;
}
