/*
 * Copyright (C) 2008 Nicolas P. Rougier
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.

 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <string.h>
#include "font.h"
#include "axis-ranged.h"
#include <stdio.h>

/**
 * 12 juin 2011
 * Sliding axis.
 * But, in order to get the right effect, and because there is a
 * glRotate( 180, 0, 0, 1), one need to draw ticks of increasing
 * value from the endpoint of the axis get_position().x + get_size().x
 * to the starting point get_position().x.
 *
 * TODO : Check if compatible with usage of AxisRanged (ie. PlaneCoord)
 * TODO : Check with Axis if not something else (smarter) to do.
 */
// ============================================================================
AxisRanged::AxisRanged (void) : Object()
{
    set_title ("Axis");
    set_range (Range(-.5, .5, 5, 25));
    set_sliding( true );
    set_visible (true);
    set_position (-.5);
    set_size (1);
    set_fg_color (0,0,0,1);
    set_bg_color (1,1,1,.5);
    set_br_color (0,0,0,1);
    alpha_ = 1.0f;

    _x_ratio_major.clear();
}

// ============================================================================
AxisRanged::~AxisRanged (void)
{}

// ============================================================================
void
AxisRanged::render (void)
{
    if (!get_visible())
        return;
    Object::compute_visibility();

    FontPtr font = Font::Font24();
    float font_scale = 1/(get_range().major*font->get_glyph_size().x*8);
    float d1 = 0.065;  // major tick size
    float d2 = 0.025; // minor tick size

    if (get_size().x < 0) {
        d1 = -d1;
        d2 = -d2;
    }

    glColor4f (get_fg_color().r, get_fg_color().g, get_fg_color().b, get_fg_color().a*alpha_);

    // Position
    glPushMatrix(); // POS
    glTranslatef( get_position().x, get_position().y, get_position().z );
    
    // Axis
    glLineWidth (2.0f);
    glBegin (GL_LINES);
    glVertex3f (0, 0, 0);
    glVertex3f (get_size().x, 0, 0);
    glEnd();

    
      // Major Ticks
      // If sliding, Majors are at i such that min+(max-min)/major*i belongs
      // to (min+(max_slide-max), max_slide)
      // ie : i_min > (max_slide-max)/(max-min)*major
      //      i_max < (max_slide-min)/(max-min)*major;
      // If expand, the range is (min, max_slide) and Majors are calculated
      // accordingly : delta_major = (max_slide-min)/major
      // and i_min = 0, i_max = major

    float delta_major;
    int i_min;
    int i_max;

    if( _fg_sliding == true ) {
      delta_major = (get_range().max - get_range().min) / get_range().major;
      i_min = rint( ceil( (max_slide_ - get_range().max) / delta_major )); 
      i_max = rint( floor( (max_slide_ - get_range().min)/ delta_major ));
    }
    else {
      delta_major = (max_slide_ - get_range().min) / get_range().major;
      i_min = 0;
      i_max = (int) get_range().major;
    }

//       glPushMatrix();
//       if (get_size().x > 0)
// 	glRotatef (180,0,0,1);
      
      glLineWidth (1.0f);
      glBegin (GL_LINES);
      
//       // DEBUG ZERO
//       glVertex3f( get_position().x, -d1, 0);
//       glVertex3f( get_position().x, d1, 0);
//       // DEBUG ONE
//       glVertex3f( get_position().x+get_size().x, -d2, 0);
//       glVertex3f( get_position().x+get_size().x, d2, 0);

      _x_ratio_major.clear();
      for( int i=i_min; i <= i_max; i++ ) {
	float x_ratio;
	if( _fg_sliding ) {
	  x_ratio = (get_range().min + i * delta_major - (get_range().min + (max_slide_ - get_range().max))) / (get_range().max - get_range().min);
	  _x_ratio_major.push_back( x_ratio );
	}
	else {
	  x_ratio = (i * delta_major) / (max_slide_ - get_range().min);
	  _x_ratio_major.push_back( x_ratio );
	}
	//std::cout << "i=" << i << ", x=" << get_range().min + i * delta_major - (get_range().min + (max_slide_ - get_range().max)) << "\n";
	glVertex3f ( x_ratio * get_size().x, 0, 0);
        glVertex3f ( x_ratio * get_size().x, -d1, 0);
      }
      glEnd();
      
      //glPopMatrix();

      // Ticks Labels
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glEnable (GL_TEXTURE_2D);
      char text[16];
      for (int i=i_min; i <= i_max; i++) {
        snprintf (text, 15, "%+.2f",
		  get_range().min + i * delta_major );
                  //get_range().min + (get_range().max-get_range().min)*(1-i/get_range().major));
		  // get_range().min + (get_range().max-get_range().min)*i/get_range().major);
        // old_version Size s = font->size (text);
	size_t size_text = strlen(text);
        glPushMatrix(); //TICKSLAB
//         if (get_size().x > 0)
// 	  glRotatef (180,0,0,1);
        // old_version glTranslatef (get_position().x + i/get_range().major*get_size().x - s.x*font_scale/2,
        // old_version              -s.y*font_scale - fabs(d1), 0);
	float x_ratio;
	if( _fg_sliding ) {
	  x_ratio = (get_range().min + i * delta_major - (get_range().min + (max_slide_ - get_range().max))) / (get_range().max - get_range().min);
	}
	else {
	  x_ratio = (i * delta_major) / (max_slide_ - get_range().min);
	}
	glTranslatef (x_ratio*get_size().x - size_text*font->get_glyph_size().x *font_scale/2,
		      -font->get_glyph_size().y*font_scale - fabs(d1), 0);
        glScalef (font_scale,font_scale,font_scale);
        font->render (text);
        glPopMatrix(); //TICKSLAB
      }
    glDisable (GL_TEXTURE_2D);
    //}

//       // Minor ticks
//       glLineWidth (0.5f);

    float delta_minor;
    if( _fg_sliding ) {
      delta_minor = (get_range().max - get_range().min) / get_range().minor;
      i_min = rint( ceil( (max_slide_ - get_range().max) / delta_minor )); 
      i_max = rint( floor( (max_slide_ - get_range().min)/ delta_minor ));
    }
    else {
      delta_minor = (max_slide_ - get_range().min) / get_range().minor;
      i_min = 0;
      i_max = (int) get_range().minor;
    }
    //    glPushMatrix();
//     if (get_size().x > 0)
//       glRotatef (180,0,0,1);
    
    glLineWidth (1.0f);
    glBegin (GL_LINES);
    
//       // DEBUG ZERO
//       glVertex3f( get_position().x, -d1, 0);
//       glVertex3f( get_position().x, d1, 0);
//       // DEBUG ONE
//       glVertex3f( get_position().x+get_size().x, -d2, 0);
//       glVertex3f( get_position().x+get_size().x, d2, 0);


      for( int i=i_min; i <= i_max; i++ ) {
	float x_ratio;
	if( _fg_sliding ) {
	  x_ratio = (get_range().min + i * delta_minor - (get_range().min + (max_slide_ - get_range().max))) / (get_range().max - get_range().min);
	}
	else {
	  x_ratio = (i * delta_minor) / (max_slide_ - get_range().min);
	}
	// std::cout << "i=" << i << ", x=" << get_range().min + i * delta_minor - (get_range().min + (max_slide_ - get_range().max)) << "\n";
	glVertex3f (x_ratio * get_size().x, 0, 0);
        glVertex3f (x_ratio * get_size().x, -d2, 0);
      }
      glEnd();
      
//       glPopMatrix();


    // Label
    glEnable (GL_TEXTURE_2D);
    font_scale = .0025;
    glPushMatrix(); // LABEL
    // old_version Size s = font->size (label_);
    size_t size_label = label_.size();
//     if (get_size().x > 0)
//         glRotatef (180,0,0,1);
    // old_version glTranslatef (get_position().x + get_size().x/2 - s.x*font_scale/2,
    // old_version               -2*s.y*font_scale - fabs(d1), 0);
    glTranslatef (get_size().x/2 - size_label*font->get_glyph_size().x *font_scale/2,
		  -2*font->get_glyph_size().y*font_scale - fabs(d1), 0);
    glScalef (font_scale,font_scale,font_scale);
    font->render (label_);
    glPopMatrix(); //LABEL

    glDisable (GL_TEXTURE_2D);
    
    glPopMatrix(); // POS
}

// ============================================================================
void
AxisRanged::set_position_x (float x)
{
    position_ = Position (x, 0, 0, 0);
}


// ============================================================================
void
AxisRanged::set_size (float x)
{
    size_ = Size (x, 0, 0, 0);
}

// ============================================================================
void
AxisRanged::set_title (std::string label)
{
    label_ = label;
}

std::string
AxisRanged::get_title (void)
{
    return label_;
}

// ============================================================================
void
AxisRanged::set_range (Range range)
{
    range_ = range;
    max_slide_ = range_.max;
}

void
AxisRanged::set_range (float min, float max, float major, float minor)
{
    set_range (Range (min, max, major, minor));
}

Range
AxisRanged:: get_range (void)
{
    return range_;
}
// ============================================================================
void
AxisRanged::set_slide_max( float max_slide )
{
  max_slide_ = max_slide;
}
void
AxisRanged::set_sliding( bool flag )
{
  _fg_sliding = flag;
}
bool
AxisRanged::get_sliding()
{
  return _fg_sliding;
}
// ============================================================================
