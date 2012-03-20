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
 * 10 april 2009 : modified by Alain Dutech
 */
#ifndef __PLANE_COORD_H__
#define __PLANE_COORD_H__

#include "object.h"
#include "axis-ranged.h"

#ifdef HAVE_BOOST
typedef boost::shared_ptr<class PlaneCoord> PlaneCoordPtr;
#else
typedef class PlaneCoord *                  PlaneCoordPtr;
#endif

/**
 * Class for drawing one "plane" of a cube with coordinates.
 *
 * PlaneCoord is drawn in the Oxy plane, origin at (0,0,0), use set_position
 * and set_size for updating position and shape.
 *
 * coord1 is along sides '1' and '3', coord2 is along sides '2' and
 * '4', starting from Ox and going counterclockwise. One can set the
 * Range (set_range_coord) and label (set_label_coord).
 * For each side, one can
 * set what is drawn using a PlaneCoord::SideType among AXIS, LINE or NONE.
 * 
 * Default configuration is:
 * - set_sides_type(AXIS, LINE, LINE, AXIS );
 * - set_range_coord1 (Range( -1, 1, 4, 4*5));
 * - set_range_coord2 (Range( -1, 1, 4, 4*5));
 * - set_title_coord1 ("Coord_1");
 * - set_title_coord2 ("Coord_2");
 *
 * \image html plane-coord.png
 */
class PlaneCoord : public Object {
 public:
  // type of side
  /**
   * Specify what is drawn on side.
   */
  enum SideType { NONE = 0, LINE, AXIS };

  // Base
  /**
   * @name Creation/Destruction
   */
  /**
   * Default constructor
   */
  PlaneCoord (void);
  /**
   * Destructor
   */
  virtual ~PlaneCoord (void);
  //@}

  // Rendering
  /**
   *  @name Rendering
   */
  /**
   * Render the PlaneCoord
   */
  virtual void render (void);
  //@}

  /**
   * @name Visibility
   */
  /**
   * Hide.
   */
  virtual void hide (float timeout=0);
  /**
   * Show.
   */
  virtual void show (float timeout=0);
  //@}

  // Setter/Getters
  /**
   * @name SidesType
   *
   * Set the axis.
   */
  /**
   * Set type of side, for each side numbered in counter clockwise order.
   *
   * @param side_n SideType
   */
  virtual void set_sides_type (SideType side_1, SideType side_2,
			       SideType side_3, SideType side_4);
  /**
   * Set Range= (min, max, nb_major_ticks, nb_minor_ticks) on coord1.
   */
  virtual void set_range_coord1 (Range range);
  /**
   * Set Range= (min, max, nb_major_ticks, nb_minor_ticks) on coord2.
   */
  virtual void set_range_coord2 (Range range);

  /**
   * Get Range for coord1.
   */
  virtual Range get_range_coord1 (void);
  /**
   * Get Range for coord2.
   */
  virtual Range get_range_coord2 (void);

  /**
   * Set Coord1 so that values between (min+(max_side - max), max_slide) is
   * displayed.
   *
   * @param max_slide   Value of the max point of the sliding range
   */
  virtual void set_slide_max1( float max_slide );
  /**
   * Get max_slide of Coord1
   */
  virtual float get_slide_max1();
  /**
   * Set Coord2 so that values between (min+(max_side - max), max_slide) is
   * displayed.
   *
   * @param max_slide   Value of the max point of the sliding range
   */
  virtual void set_slide_max2( float max_slide );
  /**
   * Get max_slide of Coord2
   */
  virtual float get_slide_max2();
  /**
   * Switch between 'sliding' mode on/off. (default is on).
   */
  void set_sliding_coord1( bool flag );
  /**
   * Get the sliding mode flag.
   */
  bool get_sliding_coord1();
  /**
   * Switch between 'sliding' mode on/off. (default is on).
   */
  void set_sliding_coord2( bool flag );
  /**
   * Get the sliding mode flag.
   */
  bool get_sliding_coord2();
  
  /**
   * Project a coordinate on axe1, taking into account Position, Range, sliding value
   * and size.
   * ((value - (rg.min+(max_slide1_ - rg.max))) / (rg.max - rg.min) + pos.x ) * s.x
   */
  virtual float get_projection_coord1( float value );
  /**
   * Project a coordinate on axe2, taking into account Position, Range, sliding value
   * and size.
   * ((value - (rg.min+(max_slide1_ - rg.max))) / (rg.max - rg.min) + pos.y ) * s.y
   */
  virtual float get_projection_coord2( float value );
  /**
   * Project a coordinate on axe3, taking into account Position, Range, sliding value
   * and size.
   * (value + pos.z ) * sz.z
   */
  virtual float get_projection_coord3( float value );

  /**
   * Set label for coord1.
   */
  virtual void set_title_coord1 (std::string label);
   /**
   * Set label for coord2.
   */
  virtual void set_title_coord2 (std::string label);
  //@}
  
  /**
   * @name Flipped
   */
  /**
   * Set flipped state.
   *
   * @param flip
   */
  virtual void set_flipped (bool flip);
  /**
   * Get flipped state.
   *
   * @return flip
   */
  virtual bool get_flipped ();
  //@}
  
 protected:
  /**
   * SideType, in couterclockwise order.
   */
  SideType * side_type_;
  /**
   * Axis for all Sides.
   */
  AxisRanged * axis_;
  /**
   * Sliding value for coord1.
   */
  float max_slide1_;
  /**
   * Sliding value for coord2.
   */
  float max_slide2_;
  /**
   * Store flipped state.
   */
  bool flipped_;

};

#endif // __PLANE_COORD_H__
