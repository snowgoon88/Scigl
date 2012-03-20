/*
 * Copyright (C) 2008 Nicolas P. Rougier
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
 * 15 april 2009 : added/modified by Alain Dutech
 */
#include "scene-graph.h"
#include <iostream>
#include "trackball.h"

// ===========================================================================
SceneGraph::SceneGraph (void)
{

  axis_cube_ = BasisCubePtr ( new BasisCube());
  axis_cube_->set_position ( -.5, -.5, -.5);
  set_axis_size( 2.0, 2.0, 2.0 );

}
// ===========================================================================
SceneGraph::~SceneGraph (void)
{
}

// ===========================================================================  
void
SceneGraph::set_axis_size (float size_x, float size_y, float size_z)
{
  axis_cube_->set_size ( size_x, size_y, size_z );
}
void
SceneGraph::set_axis_size (Size s)
{
  set_axis_size (s.dx, s.dy, s.dz);
}

// ===========================================================================
BasisCubePtr
SceneGraph::get_basis_cube (void)
{
  return axis_cube_;
}
// ===========================================================================
bool
SceneGraph::remove (ObjectPtr object)
{
  std::vector<ObjectPtr>::iterator it;
  
  bool found = false;
  // look for the object
  //std::cout << "remove : search object " << object->get_id() << "\n";
  it = objects_.begin();
  while( found == false ) {
    //std::cout << "remove : look at object " << (*it)->get_id() << "\n";
    if( *it == object ) {
      //std::cout << "remove : found\n";
      objects_.erase(it);
      found = true;
    }
    it++;
  }
      
  return found;
}
// ===========================================================================
ObjectPtr
SceneGraph::get_object( unsigned int id)
{
  unsigned int i;

  for (i=0; i<objects_.size(); i++) {
    if ( objects_[i]->check_id( id ) == true ) {
      return objects_[i];
    }
  }
#ifdef HAVE_BOOST
  return ObjectPtr();
#else
  return NULL;
#endif
}
 // ===========================================================================
std::string
SceneGraph::dump_objects (void)
{
  unsigned int i;

  std::ostringstream oss;
  for (i=0; i<objects_.size(); i++)
    oss << "dump :  " << objects_[i]->get_id() << "\n";

  return oss.str();
}
// ===========================================================================
void
SceneGraph::render ()
{
  //std::cout << "SceneGraph::render() \n";
  
  // glClearColor (get_bg_color().r, get_bg_color().g, get_bg_color().b, get_bg_color().a);
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
//   glMatrixMode (GL_MODELVIEW);
//   glLoadIdentity ();
//   glTranslatef (0.0, 0.0, -5.0f);
//   glScalef (zoom_, zoom_, zoom_);
//   float m[4][4];
//   build_rotmatrix (m, view_.data);
//   glMultMatrixf (&m[0][0]);
  
  GLint viewport[4]; 
  glGetIntegerv (GL_VIEWPORT, viewport);
  GLint scissor[4]; 
  glGetIntegerv (GL_SCISSOR_BOX, scissor);
  GLint scissor_active;
  GLint mode;
  glGetIntegerv (GL_RENDER_MODE, &mode);
  glGetIntegerv (GL_SCISSOR_TEST, &scissor_active);
  float height = viewport[3];
  
  render_start ();
  if (not get_visible()) {
    render_finish ();
    return;
  }    
  if (mode == GL_RENDER) {
    Widget::render();
  }
  render_finish ();
  
  glPushAttrib (GL_VIEWPORT_BIT | GL_SCISSOR_BIT);
  
  int border = 2;
  if ((get_br_color().alpha * alpha_) == 0) {
    border = 0;
  }
  
  // Set scissor test
  int x = viewport[0] + int(get_position().x + border/2);
  int y = viewport[1] + int(height-get_position().y-get_size().y + border/2);
  int w = int(get_size().x) - border;
  int h = int(get_size().y) - border;
  if (scissor_active) {
    if (x < scissor[0])
      x = scissor[0];
    if (y < scissor[1])
      y = scissor[1];
    if ((x+w) > (viewport[0]+scissor[2]))
      w += (viewport[0]+scissor[2]) - (x+w);
    if ((y+h) > (viewport[1]+scissor[3]))
      h += (viewport[1]+scissor[3]) - (y+h);
  }
  glEnable (GL_SCISSOR_TEST);
  glScissor (x,y,w,h);
  // Set viewport
  glViewport (viewport[0]+int(get_position().x) + border/2,
	      viewport[1]+int(height-get_position().y-get_size().y+border/2),
	      int(get_size().x)-border,
	      int(get_size().y)-border);
  
  
  // Set modelview 
  glClear(GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity ();
  
  if (mode == GL_SELECT) {
    GLint v[4];
    glGetIntegerv (GL_VIEWPORT, v);
    gluPickMatrix (pointer_.x, pointer_.y, 5, 5, v);
  }
  
  float aspect = 1.0f;
  aspect = get_size().x / get_size().y;
  float aperture = 25.0f;
  float near = 1.0f;
  float far = 100.0f;
  float top = tan(aperture*3.14159/360.0) * near;
  float bottom = -top;
  float left = aspect * bottom;
  float right = aspect * top;
  if (get_ortho_mode())
    glOrtho (left, right, bottom, top, near, far);
  else
    glFrustum (left, right, bottom, top, near, far);
  
  glMatrixMode (GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity ();
  glTranslatef (0.0, 0, -8.0f);
  glScalef (zoom_, zoom_, zoom_);
  float m[4][4];
  build_rotmatrix (m, view_.data);
  glMultMatrixf (&m[0][0]);
  
  // 
  setup();
  
  // Back widgets
  glDisable (GL_DEPTH_TEST);
  for (unsigned int i=0; i<widgets_.size(); i++)
    if (widgets_.at(i)->get_position().z < 0)
      widgets_.at(i)->render();
  
  // Objects
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);
  for (unsigned int i=0; i<objects_.size(); i++)
    objects_.at(i)->render();

  // BasisCube, rendered "after" to allow for transparency.
  glDisable (GL_DEPTH_TEST);
  axis_cube_->render();
  glDisable (GL_LIGHTING);

  
  // Front widgets
  glDisable (GL_DEPTH_TEST);
  for (unsigned int i=0; i<widgets_.size(); i++)
    if (widgets_.at(i)->get_position().z >= 0)
      widgets_.at(i)->render();

  glMatrixMode (GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode (GL_PROJECTION);
  glPopMatrix();
  
  //glDisable (GL_SCISSOR_TEST);
  glPopAttrib ();
}
// ============================================================================
void
SceneGraph::set_range_coord_x (Range range)
{
  axis_cube_->set_range_coord_x (range);

  // for (unsigned int i=0; i<objects_.size(); i++) {
//     SmoothSurface *surf_handle = ((SmoothSurface *) (objects_.at(i)));
//     Data *surf_data = surf_handle->get_data();
//     surf_data->set_range_coord_x( range );
//     surf_handle->update();
  //}
  
}
void
SceneGraph::set_range_coord_y (Range range)
{
  axis_cube_->set_range_coord_y (range);

  // for (unsigned int i=0; i<objects_.size(); i++) {
//     SmoothSurface *surf_handle = ((SmoothSurface *) (objects_.at(i)));
//     Data *surf_data = surf_handle->get_data();
//     surf_data->set_range_coord_y( range );
//     surf_handle->update();
  //}
}
void
SceneGraph::set_range_coord_z (Range range)
{
  axis_cube_->set_range_coord_z (range);
  
  // colorbar_->get_cmap()->scale( range.min, range.max);

//   for (unsigned int i=0; i<objects_.size(); i++) {
//     SmoothSurface *surf_handle = ((SmoothSurface *) (objects_.at(i)));
//     Data *surf_data = surf_handle->get_data();
//     surf_data->set_range_coord_z( range );
//     surf_handle->update();
  //}
}
