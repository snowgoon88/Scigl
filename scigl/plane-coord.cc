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
#include "plane-coord.h"
#include <iostream>

// ============================================================================
PlaneCoord::PlaneCoord (void) : Object () 
{
  side_type_ = new SideType [4];
  axis_ = new AxisRanged [4];

  set_size      (1, 1);
  set_position  (0,0);
  set_fg_color  (0,0,0,1);
  set_bg_color  (1,1,1,.2);
  set_br_color  (0,0,0,1);
  
  //axis_[0].set_label ("X axis");
  axis_[0].set_position (0);
  axis_[0].set_size (1);
  

  //axis_[1].set_label ("Y axis");
  axis_[1].set_position (0);
  axis_[1].set_size (1);

  //axis_[2].set_label ("X axis");
  axis_[2].set_position (0);
  axis_[2].set_size (1);

  //axis_[3].set_label ("Y axis");
  axis_[3].set_position (0);
  axis_[3].set_size (1);

  set_sides_type(AXIS, LINE, LINE, AXIS ); // LINE
  set_range_coord1 (Range( -1, 1, 4, 4*5));
  set_range_coord2 (Range( -1, 1, 4, 4*5));
  set_title_coord1 ("Coord_1");
  set_title_coord2 ("Coord_2");
  set_flipped (false);

}
// ============================================================================
PlaneCoord::~PlaneCoord (void)
{
  delete [] axis_;
  delete [] side_type_;
}


// ============================================================================
void
PlaneCoord::render (void)
{
  compute_visibility();
  if (not get_visible()) return;

  glPushMatrix();

  // set global position and size
  glScalef (get_size().x, get_size().y, get_size().z);
  glTranslatef (get_position().x, get_position().y, get_position().z);

  if (get_flipped()) {
    glTranslatef ( 1, 0, 0);
    glRotatef (180, 0, 1, 0);
  }

  glEnable (GL_BLEND);
  glDisable (GL_TEXTURE_2D);

  // render plane as a filled GL_QUADS
  // (Alain) disabled to allow transparency ??
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  glColor4f (get_bg_color().r, get_bg_color().g, get_bg_color().b, get_bg_color().a*alpha_);
  glPolygonOffset (1, 1);
  glEnable (GL_POLYGON_OFFSET_FILL);
  glBegin (GL_QUADS); {
    glVertex3f (0,  0, 0);
    glVertex3f (1,  0, 0);
    glVertex3f (1, 1, 0);
    glVertex3f (0, 1, 0);
  }
  glEnd();

  glLineWidth (0.5f);
  glColor4f (get_br_color().r, get_br_color().g, get_br_color().b, get_br_color().a*alpha_);
  glDisable (GL_POLYGON_OFFSET_FILL);
  glEnable (GL_LINE_SMOOTH);

  glPushMatrix();
  // draw sides (AXIS, LINE or NONE)
  for (int i=0; i<4; i++ )
    {
      switch (side_type_[i]) 
	{
	case AXIS:
	  axis_[i].render();
	  break;
	case LINE:
	  glBegin (GL_LINES); {
	    glVertex3f (0,  0, 0);
	    glVertex3f (1,  0, 0);
	  }
	  glEnd();
	  break;
	default:
	  {}
	}
      glTranslatef( 1, 0, 0);
      glRotatef( 90, 0, 0, 1);
    }
  glPopMatrix();

  // Grid lines
  glEnable (GL_LINE_STIPPLE);
  glLineStipple (1, 0xf0f0);
  glBegin (GL_LINES);
  for( unsigned int i=0; i<axis_[0]._x_ratio_major.size(); i++ ) {
    glVertex2f( axis_[0]._x_ratio_major[i], 0);
    glVertex2f( axis_[0]._x_ratio_major[i], 1);
  }
  glEnd();

  glBegin (GL_LINES);
  // BEWARE : axis[3] range is "inverted" (from min to max)...
  for( unsigned int i=0; i<axis_[3]._x_ratio_major.size(); i++ ) {
    glVertex2f( 0, 1.0 - axis_[3]._x_ratio_major[i]);
    glVertex2f( 1, 1.0 - axis_[3]._x_ratio_major[i]);
  }
  glEnd();

//   for (float x=1/axis_[0].get_range().major;
//        x < 1.0;
//        x += 1/(float)axis_[0].get_range().major)
//     {
//       glBegin (GL_LINES); {
// 	glVertex2f (x, 0);
// 	glVertex2f (x, 1); 
//       }
//       glEnd();
//     }
  // for(float y=1/(float)axis_[1].get_range().major;
//        y < 1.0;
//        y += 1/(float)axis_[1].get_range().major)
//     {
//       glBegin (GL_LINES); {
// 	glVertex2f (0, y);
// 	glVertex2f (1, y);
//       } 
//       glEnd();
//     }
  glDisable (GL_LINE_STIPPLE);

  glPopMatrix();
  
}
// ============================================================================
void
PlaneCoord::hide (float timeout)
{
    Object::hide (timeout);
    for (int i=0; i<4; i++) {
      axis_[i].hide (timeout);
    }
}
void
PlaneCoord::show (float timeout)
{
  Object::show (timeout);
  for (int i=0; i<4; i++) {
    axis_[i].show (timeout);
  }
}


// ============================================================================
void
PlaneCoord::set_sides_type (SideType side1, SideType side2,
			    SideType side3, SideType side4)
{
  side_type_[0] = side1;
  side_type_[1] = side2;
  side_type_[2] = side3;
  side_type_[3] = side4;
}
// ============================================================================
void
PlaneCoord::set_range_coord1 (Range range)
{
  axis_[0].set_range (range);
  axis_[2].set_range ( Range( range.max, range.min, range.major, range.minor));
  max_slide1_ = range.max;
}
void
PlaneCoord::set_range_coord2 (Range range)
{
  axis_[1].set_range (range);
  axis_[3].set_range ( Range( range.max, range.min, range.major, range.minor));
  max_slide2_ = range.max;
}
// ============================================================================
Range
PlaneCoord::get_range_coord1 (void)
{
  return axis_[0].get_range ();
}
Range
PlaneCoord::get_range_coord2 (void)
{
  return axis_[1].get_range ();
}
// ============================================================================
void
PlaneCoord::set_slide_max1 (float max_slide )
{
  max_slide1_ = max_slide;
  axis_[0].set_slide_max( max_slide1_ );
  axis_[2].set_slide_max( max_slide1_ );
}
float
PlaneCoord::get_slide_max1()
{
  return max_slide1_;
}
void
PlaneCoord::set_slide_max2 (float max_slide )
{
  max_slide2_ = max_slide;
  axis_[1].set_slide_max( max_slide2_ );
  axis_[3].set_slide_max( max_slide2_ );
}
float
PlaneCoord::get_slide_max2()
{
  return max_slide2_;
}
// ============================================================================
void
PlaneCoord::set_sliding_coord1( bool flag )
{
  axis_[0].set_sliding( flag );
  axis_[2].set_sliding( flag );
}
bool
PlaneCoord::get_sliding_coord1()
{
  return axis_[0].get_sliding();
}
void
PlaneCoord::set_sliding_coord2( bool flag )
{
  axis_[1].set_sliding( flag );
  axis_[3].set_sliding( flag );
}
bool
PlaneCoord::get_sliding_coord2()
{
  return axis_[1].get_sliding();
}
// ============================================================================
float
PlaneCoord::get_projection_coord1( float value )
{
  Position pos = get_position();
  Range rg = get_range_coord1();
  Size sz = get_size();

  float proj;
  if( get_sliding_coord1() ) {
    proj = ((value - (rg.min+(max_slide1_ - rg.max))) / (rg.max - rg.min) + pos.x ) * sz.x;
  }
  else {
    proj = ((value - rg.min) / (max_slide1_ - rg.min) + pos.x ) * sz.x;
  }
  return proj;
}
float
PlaneCoord::get_projection_coord2( float value )
{
  Position pos = get_position();
  Range rg = get_range_coord2();
  Size sz = get_size();

  float proj = ((value - (rg.min+(max_slide2_ - rg.max))) / (rg.max - rg.min) + pos.y ) * sz.y;
  return proj;
}
float
PlaneCoord::get_projection_coord3( float value )
{
  Position pos = get_position();
  Size sz = get_size();

  float proj = (value + pos.z ) * sz.z;
  return proj;
}
// ============================================================================
void
PlaneCoord::set_title_coord1 (std::string label)
{
  axis_[0].set_title (label);
  axis_[2].set_title (label);
}
void
PlaneCoord::set_title_coord2 (std::string label)
{
  axis_[1].set_title (label);
  axis_[3].set_title (label);
}
// ============================================================================
void
PlaneCoord::set_flipped (bool flip)
{
  flipped_ = flip;
}
bool
PlaneCoord::get_flipped ()
{
  return flipped_;
}
// ============================================================================
