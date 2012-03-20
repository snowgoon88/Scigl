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
#include <stdio.h>

#include <cstdlib>
#include "terminal.h"

/**
 * Terminal in a window
 *
 */
TerminalPtr terminal = TerminalPtr (new Terminal);

// /**
//  * Called after every mouse button pressed.
//  * see glfwSetMouseButtonCallback().
//  */
// void on_mouse_button( int button, int action )
// {
//   int x, y;
//   glfwGetMousePos( &x, &y);

//   //std::cout <<"Mouse Button at (" << x <<  ", " << y << ")\n";
  
//   if( action == GLFW_PRESS ) {
//     if( button == GLFW_MOUSE_BUTTON_LEFT ) {
//       scene->mouse_action_start ("rotate",x,y);
//     }
//     else if( button == GLFW_MOUSE_BUTTON_MIDDLE ) {
//       scene->mouse_action_start( "zoom", x, y);
//     }
//   }
//   else if( action == GLFW_RELEASE ) {
//     scene->mouse_action_end( x, y);
//   }
// }
// /**
//  * Called when the mouse moves.
//  * see glfwSetMousePosCallback().
//  */
// void on_mouse_move (int x, int y) {
//   scene->mouse_action (x,y);
//   // TODO No equivalence for "glutPostRedisplay()"; ==> needed ??
// }
/**
 * Called when a key is pressed.
 * see glfwSetKeyCallback().
 */
void on_key_pressed( int key, int action)
{
  //std::cout << "KP key=" << key << ", act=" << action << "\n";
  std::string stract = "type", keyname = "";
  if( action == GLFW_PRESS ) {
    //if ((key > 31) and (key < 127)) { stract = "type"; keyname = char(key);
    //} else 
    if (key == 1) {            stract = "home";
    } else if (key == 5) {            stract = "end";
    } else if (key == GLFW_KEY_BACKSPACE) {            stract = "backspace";
    } else if (key == 9) {            stract = "complete";
    } else if (key == 11) {           stract = "kill";
    } else if (key == 12) {           stract = "clear";
    } else if (key == GLFW_KEY_ENTER) {           stract = "enter";
    } else if (key == 25) {           stract = "yank";
    } else if (key == 27) {           stract = "escape";
    } else if (key == 127) {          stract = "delete";
    } else if (key == GLFW_KEY_UP) {  stract ="history-prev";
    } else if (key == GLFW_KEY_DOWN) {  stract ="history-next";
    } else if (key == GLFW_KEY_LEFT) {  stract ="left";
    } else if (key == GLFW_KEY_RIGHT) {  stract ="right";
    } else if (key == GLFW_KEY_HOME) {  stract ="home";
    } else if (key == GLFW_KEY_END) {  stract ="end";
    } else {stract = "";
    }
    if (stract.size() or keyname.size())
        terminal->keyboard_action (stract, keyname);
  }
}
void on_char_key_pressed( int ch, int action )
{
  //std::cout << "CP ch=" << ch << ", act=" << action << "\n";
  std::string stract = "type", keyname = "";
  if( action == GLFW_PRESS ) {
    if ((ch > 31) and (ch < 127)) { stract = "type"; keyname = char(ch);
    }
    if (stract.size() or keyname.size())
      terminal->keyboard_action (stract, keyname);
  }
}
void terminal_activate (Terminal *terminal, std::string input)
{
    terminal->print (std::string("Activate callback: \"") + input + "\"\n");
}
void terminal_complete (Terminal *terminal, std::string input)
{
    terminal->print (terminal->get_prompt() + input + "\n");
    terminal->print (std::string("Complete callback: \"") + input + "\"\n");
}
void terminal_history_prev (Terminal *terminal, std::string input)
{
    terminal->print (terminal->get_prompt() + input + "\n");
    terminal->print (std::string("History prev callback: \"") + input + "\"\n");
}
void terminal_history_next (Terminal *terminal, std::string input)
{
    terminal->print (terminal->get_prompt() + input + "\n");
    terminal->print (std::string("History next callback: \"") + input + "\"\n");
}
void display (void) {
    glClearColor (1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    terminal->render ();
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
  //glfwSetMouseButtonCallback( on_mouse_button );
  //glfwSetMousePosCallback( on_mouse_move );
  glfwSetKeyCallback( on_key_pressed );
  glfwSetCharCallback( on_char_key_pressed );
  //glutReshapeWindow (400,400);
  
  // Background color for scene
  terminal->set_margin    (Margin(5,5,5,5));
  terminal->set_radius    (0);
  terminal->set_size      (1, 1);
  terminal->set_gravity   (1,1);
  terminal->set_position  (1,1);
  terminal->set_fg_color  (0,0,0,1);
  terminal->set_bg_color  (1,1,.9,1);
  terminal->set_br_color  (1,1,.9,1);
  
  terminal->print ("\033[01mTerminal widget\033[00m\n");
  terminal->print ("\033[01mStyle:\033[00m normal \033[01mbold\033[00m \033[04munderline\033[00m\n");
  terminal->print ("\033[01mForeground color:\033[00m "	\
		   "\033[30mblack\033[00m "		\
		   "\033[31mred\033[00m "		\
		   "\033[32mgreen\033[00m "		\
		   "\033[33myellow\033[00m "		\
		   "\033[34mblue\033[00m "		\
		   "\033[35mmagenta\033[00m "		\
		   "\033[36mcyan\033[00m "		\
		   "\033[37mwhite\033[00m "		\
		   "\033[39mdefault\033[00m\n");
  terminal->print ("\033[01mBackground color:\033[00m "	\
		   "\033[40mblack\033[00m "		\
		   "\033[41mred\033[00m "		\
		   "\033[42mgreen\033[00m "		\
		   "\033[43myellow\033[00m "		\
		   "\033[44mblue\033[00m "		\
		   "\033[45mmagenta\033[00m "		\
		   "\033[46mcyan\033[00m "		\
		   "\033[47mwhite\033[00m "		\
		   "\033[49mdefault\033[00m\n");
  
  terminal->set_prompt ("\033[01m>\033[0m ");
  terminal->connect ("activate", terminal_activate);
  terminal->connect ("complete", terminal_complete);
  terminal->connect ("history-prev", terminal_history_prev);
  terminal->connect ("history-next", terminal_history_next);

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


