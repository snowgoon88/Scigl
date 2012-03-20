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
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 25 may 2011 : modified by Alain Dutech
 */
#include "curve.h"
//#include "shapes.h"
#include <iostream>

// _______________________________________________________________________ Curve
Curve::Curve (void) : Object ()
{
    set_size (1.0f, 1.0f, 1.0f);
    set_position (0.0f, 0.0f, 0.0f);
    set_fg_color (0.0f, 0.0f, 0.0f, 1.0f);
    set_bg_color (1.0f, 1.0f, 1.0f, 1.0f);
    set_thickness (1.01);
    cmap_ = Colormap::Hot();
    reset_data();

    std::ostringstream oss;
    oss << "Curve_" << id_;
    set_name (oss.str());
}


// ______________________________________________________________________ ~Curve
Curve::~Curve (void)
{}


// ______________________________________________________________________ render
void
Curve::render (void)
{
    compute_visibility();
    if (!get_visible()) {
        return;
    }
    Color c;
    double r, g, b, a;
    r = fg_color_.r;
    g = fg_color_.g;
    b = fg_color_.b;
    a = fg_color_.a;
    glColor4f(r,g,b,a*alpha_);
    // -------------------------------------------------------------------------
    //  Rendering using GL_LINE_STRIP
    // -------------------------------------------------------------------------
    if ((thickness_ == 0) or (thickness_ > 1.0)) {
        glEnable (GL_BLEND);
        glEnable (GL_LINE_SMOOTH);
        if (thickness_ == 0)
            glLineWidth (1.0);
        else
            glLineWidth (thickness_);

        // XYZ
        // ---------------------------------------------------------------------
	Position p = get_position();
	Size s = get_size();
	GLdouble xval = 0.0;
	glBegin(GL_LINE_STRIP);
	std::list<Position>::const_iterator _iter_data;
	for( _iter_data = _data.begin(); _iter_data != _data.end(); _iter_data++) {
	  //std::cout << "[" << xval << "] " <<  (GLdouble) (xval - _rgX.min) / (_rgX.max - _rgX.min) << " " << (GLdouble) ((*_iter_data).y - _rgY.min) / (_rgY.max - _rgY.min) << " " << (GLdouble) ((*_iter_data).z - _rgZ.min) / (_rgZ.max - _rgZ.min) << "\n";

	  glVertex3f( (GLdouble) ((xval - _rgX.min) / (_rgX.max - _rgX.min) +p.x) * s.x,
		      (GLdouble) (((*_iter_data).y - _rgY.min) / (_rgY.max - _rgY.min) + p.y) * s.y,
		      (GLdouble) (((*_iter_data).z - _rgZ.min) / (_rgZ.max - _rgZ.min) + p.z) * s.z);
	  xval += 1.0;
	}
	glEnd();
    }
}
// _____________________________________________________________________ add_xyz
void
Curve::add_xyz ( double x, double y, double z )
{
  Position point(x, y, z, 0.0);

  if( _data.size() > (unsigned int) (_cache_size)) {
    _data.pop_front();
    _data.push_back( point );
  }
  else {
    _data.push_back( point );
  }
}

// ___________________________________________________________________ add_xyz_c
void
Curve::add_xyz_c ( double x, double y, double z, DataPtr cdata)
{
  add_xyz( x, y, z);
}

// ______________________________________________________________________ add_yz
void
Curve::add_yz ( double y, double z )
{
  Position point(0.0, y, z, 0.0);

  if( _data.size() > (unsigned int) (_cache_size)) {
    _data.pop_front();
    _data.push_back( point );
  }
  else {
    _data.push_back( point );
  }
}

// ____________________________________________________________________ add_yz_c
void
Curve::add_yz_c ( double y, double z, DataPtr cdata)
{
  add_yz( y, z);
}
// __________________________________________________________________ reset_data
void
Curve::reset_data (void)
{
  _data.clear();
}


// _______________________________________________________________ get_thickness
float
Curve::get_thickness (void) const
{
    return thickness_;
}


// _______________________________________________________________ set_thickness
void
Curve::set_thickness (float thickness)
{
    thickness_ = thickness;
}


// ________________________________________________________________ get_colormap
ColormapPtr
Curve::get_colormap (void)
{
    return cmap_;
}


// ________________________________________________________________ set_colormap
void
Curve::set_colormap (ColormapPtr colormap)
{
    cmap_ = colormap;
}

// ________________________________________________________________ range_coordX
void
Curve::set_range_coordX (Range range)
{
  _rgX = range;
  _rgX.min = 0.0;
  _cache_size = (unsigned int ) abs(ceil(_rgX.max));
  
  while( _data.size() > _cache_size ) {
    _data.pop_front();
  }
}
Range
Curve::get_range_coordX (void)
{
  return _rgX;
}

// ________________________________________________________________ range_coordY
void
Curve::set_range_coordY (Range range)
{
  _rgY = range;
}
Range
Curve::get_range_coordY (void)
{
  return _rgY;
}
// ________________________________________________________________ range_coordZ
void
Curve::set_range_coordZ (Range range)
{
  _rgZ = range;
}
Range
Curve::get_range_coordZ (void)
{
  return _rgZ;
}
