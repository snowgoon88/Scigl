/*
 * Copyright (C) 2008 Nicolas P. Rougier, Alain Dutech
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
 */
#include "basis-cube.h"

// ============================================================================
BasisCube::BasisCube (void)
{
  //plans_ = new PlaneCoord[3];
  plans_[0] = PlaneCoordPtr ( new PlaneCoord() );
  plans_[1] = PlaneCoordPtr ( new PlaneCoord() );
  plans_[2] = PlaneCoordPtr ( new PlaneCoord() );
  
  set_title_coord_x ("X axis");
  set_range_coord_x (Range( -1, 1, 4, 4*5));

  set_title_coord_y ("Y axis");
  set_range_coord_y (Range( -1, 1, 4, 4*5));

  set_title_coord_z ("Z axis");
  set_range_coord_z (Range( -1, 1, 4, 4*5));
  
  
  set_position_Oxy ( 0 );
  set_sides_type_Oxy ( PlaneCoord::AXIS, PlaneCoord::AXIS, 
		       PlaneCoord::LINE, PlaneCoord::LINE );
  set_position_Oyz ( 0 );
  set_sides_type_Oyz ( PlaneCoord::LINE, PlaneCoord::LINE,
		       PlaneCoord::AXIS, PlaneCoord::AXIS );
  set_position_Ozx ( 1 );
  set_sides_type_Ozx ( PlaneCoord::LINE, PlaneCoord::LINE,
		       PlaneCoord::AXIS, PlaneCoord::AXIS );
  get_plane (PLAN_OZX)->set_flipped (true);
}
// ============================================================================
BasisCube::~BasisCube (void)
{
  //delete [] plans_;
}
// ============================================================================
void
BasisCube::render()
{
  compute_visibility();
  if (not get_visible()) {
    return;
  }
  else {
  }

  glPushMatrix();

  // set global position and size
  glScalef (get_size().x, get_size().y, get_size().z);
  glTranslatef (get_position().x, get_position().y, get_position().z);

  // need to decide which plane to draw first (farther Z from point of view)

  /* z sorting is done on faces center */
  PlanIndex first, second;
  float z[3];
  float m[16];
  glGetFloatv (GL_MODELVIEW_MATRIX, m);
  // Oxy center is (0.5, 0.5, plan_pos_.z)
  z[PLAN_OXY] = 0.5 * m[2]
                    + 0.5 * m[6]
                    +  plan_pos_.z * m[10]
                    + m[14];
  // Oyz center is (plan_pos_.x, 0.5, 0.5)
  z[PLAN_OYZ] = plan_pos_.x * m[2]
                    + 0.5 * m[6]
	            + 0.5 * m[10]
                    + m[14];
  if( z[PLAN_OXY] < z[PLAN_OYZ] ) {
    first = PLAN_OXY;
    second = PLAN_OYZ;
  }
  else {
    first = PLAN_OYZ;
    second = PLAN_OXY;
  }
  // 0zx center is ( 0.5, plan_pos_.y, 0.5)
  z[PLAN_OZX] = 0.5 * m[2]
                    + plan_pos_.y * m[6]
	            + 0.5 * m[10]
                    + m[14];
  if( z[PLAN_OZX] < z[first] ) {
    render_plane (PLAN_OZX);
    render_plane (first);
    render_plane (second);
  }
  else {
    render_plane (first);
    if(  z[PLAN_OZX] < z[second] ) {
      render_plane (PLAN_OZX);
      render_plane (second);
    }
    else {
      render_plane (second);
      render_plane (PLAN_OZX);
    }
  }
  glPopMatrix();
}
void
BasisCube::render_plane (PlanIndex plan_name)
{
  switch (plan_name) 
    {
    case PLAN_OXY:  
      // Oxy
      glPushMatrix();
      glTranslatef ( 0, 0, plan_pos_.z );
      get_plane (PLAN_OXY)->render();
      glPopMatrix();
      break;
    case PLAN_OYZ:
      // Oyz
      glPushMatrix();
      glRotatef (90, 0, 0, 1);
      glRotatef (90, 1, 0, 0);
      glTranslatef ( 0, 0, plan_pos_.x );
      get_plane (PLAN_OYZ)->render();
      glPopMatrix();
      break;
    case PLAN_OZX:
      // Ozx
      glPushMatrix();
      glRotatef (90, 0, 0, 1);
      glRotatef (90, 1, 0, 0);
      glRotatef (90, 0, 0, 1);
      glRotatef (90, 1, 0, 0);
      glTranslatef ( 0, 0, plan_pos_.y );
      get_plane (PLAN_OZX)->render();
      glPopMatrix();
    }
}
// ============================================================================
void
BasisCube::set_range_coord_x (Range range)
{
  get_plane (PLAN_OXY)->set_range_coord1 (range);
  get_plane (PLAN_OZX)->set_range_coord2 (range);
}
void
BasisCube::set_range_coord_y (Range range)
{
  get_plane (PLAN_OXY)->set_range_coord2 (range);
  get_plane (PLAN_OYZ)->set_range_coord1 (range);
}
void
BasisCube::set_range_coord_z (Range range)
{
  get_plane (PLAN_OYZ)->set_range_coord2 (range);
  get_plane (PLAN_OZX)->set_range_coord1 (range);
}
// ============================================================================
Range
BasisCube::get_range_coord_x (void)
{
  return  get_plane (PLAN_OXY)->get_range_coord1 ();
}
Range
BasisCube::get_range_coord_y (void)
{
  return  get_plane (PLAN_OYZ)->get_range_coord1 ();
}
Range
BasisCube::get_range_coord_z (void)
{
  return  get_plane (PLAN_OZX)->get_range_coord1 ();
}
// ============================================================================
void
BasisCube::set_title_coord_x (std::string label)
{
  get_plane (PLAN_OXY)->set_title_coord1 (label);
  get_plane (PLAN_OZX)->set_title_coord2 (label);
}
void
BasisCube::set_title_coord_y (std::string label)
{
  get_plane (PLAN_OXY)->set_title_coord2 (label);
  get_plane (PLAN_OYZ)->set_title_coord1 (label);
}
void
BasisCube::set_title_coord_z (std::string label)
{
  get_plane (PLAN_OYZ)->set_title_coord2 (label);
  get_plane (PLAN_OZX)->set_title_coord1 (label);
}
// ============================================================================
void
BasisCube::set_sides_type_Oxy ( PlaneCoord::SideType side_1,
				PlaneCoord::SideType side_2,
				PlaneCoord::SideType side_3,
				PlaneCoord::SideType side_4)
{
  get_plane (PLAN_OXY)->set_sides_type (side_1, side_2, side_3, side_4);
}
void
BasisCube::set_sides_type_Oyz ( PlaneCoord::SideType side_1,
				PlaneCoord::SideType side_2,
				PlaneCoord::SideType side_3,
				PlaneCoord::SideType side_4)
{
  get_plane (PLAN_OYZ)->set_sides_type (side_1, side_2, side_3, side_4);
}
void
BasisCube::set_sides_type_Ozx ( PlaneCoord::SideType side_1,
				PlaneCoord::SideType side_2,
				PlaneCoord::SideType side_3,
				PlaneCoord::SideType side_4)
{
  get_plane (PLAN_OZX)->set_sides_type (side_1, side_2, side_3, side_4);
}
// ============================================================================
void
BasisCube::set_position_Oxy (float z)
{
  plan_pos_.z = z;
}
void
BasisCube::set_position_Oyz (float x)
{
  plan_pos_.x = x;
}
void
BasisCube::set_position_Ozx (float y)
{
  plan_pos_.y = y;
}
// ============================================================================
PlaneCoordPtr
BasisCube::get_plane ( PlanIndex plane_name )
{
  return plans_[plane_name];
}
// ============================================================================
void
BasisCube::hide (float timeout)
{
   Object::hide (timeout);
    get_plane (PLAN_OXY)->hide (timeout);
    get_plane (PLAN_OYZ)->hide (timeout);
    get_plane (PLAN_OZX)->hide (timeout);
}
void
BasisCube::show (float timeout)
{
   Object::show (timeout);
    get_plane (PLAN_OXY)->show (timeout);
    get_plane (PLAN_OYZ)->show (timeout);
    get_plane (PLAN_OZX)->show (timeout);
}
