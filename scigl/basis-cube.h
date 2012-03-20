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
#ifndef __BASIS_CUBE_H__
#define __BASIS_CUBE_H__

#include "object.h"
#include "plane-coord.h"

/**
 * Class for drawing a "coordinate cube" made of 3 PlaneCoord.
 *
 * The image below shows the basis configuration of the cube, which is
 * described below. Note that plane Ozx has to be flipped so as to
 * render correctly because it is set to a positive y position.
 *
 * Default configuration:
 * - set_title_coord_x ("X axis");
 * - set_range_coord_x (Range( -1, 1, 4, 4*5));
 * - set_title_coord_y ("Y axis");
 * - set_range_coord_y (Range( -1, 1, 4, 4*5));
 * - set_title_coord_z ("Z axis");
 * - set_range_coord_z (Range( -1, 1, 4, 4*5));
 * - set_position_Oxy ( 0 );
 * - set_sides_type_Oxy ( PlaneCoord::AXIS, PlaneCoord::AXIS, 
 PlaneCoord::LINE, PlaneCoord::LINE );
 * - set_position_Oyz ( 0 );
 * - set_sides_type_Oyz ( PlaneCoord::LINE, PlaneCoord::LINE,
 PlaneCoord::AXIS, PlaneCoord::AXIS );
 * - set_position_Ozx ( 1 );
 * - set_sides_type_Ozx ( PlaneCoord::LINE, PlaneCoord::LINE,
 PlaneCoord::AXIS, PlaneCoord::AXIS );
 * - get_plane (PLAN_OZX)->set_flipped (true);
 *
 * \image html basis-cube.png
 */

#ifdef HAVE_BOOST
typedef boost::shared_ptr<class BasisCube> BasisCubePtr;
#else
typedef class BasisCube *                  BasisCubePtr;
#endif

class BasisCube : public Object {
 public:
  /**
   * Index for the 3 planes.
   */
  enum PlanIndex { PLAN_OXY = 0, PLAN_OYZ, PLAN_OZX };

 public:
  /**
   * @name Creation/Destruction
   */
  /**
   * Default constructor
   */
  BasisCube (void);
  /**
   * Destructor
   */
  virtual ~BasisCube (void);
  //@}
  /**
   *  @name Rendering
   */
  /**
   * Render the PlaneCoord
   */
  virtual void render();
  //@}
  
  /**
   * @name SetRange
   *
   * Set Range = (min, max, nb_major_ticks, nb_minor_ticks)
   * for each coordinate.
   */
  virtual void set_range_coord_x (Range range);
  virtual void set_range_coord_y (Range range);
  virtual void set_range_coord_z (Range range);
  //@}
  /**
   * @name GetRange
   *
   * Get Range for each coordinate.
   */
  virtual Range get_range_coord_x (void);
  virtual Range get_range_coord_y (void);
  virtual Range get_range_coord_z (void);
  //@}
  /**
   * @name SetLabel
   *
   * Set label for each coordinate.
   */
  virtual void set_title_coord_x (std::string label);
  virtual void set_title_coord_y (std::string label);
  virtual void set_title_coord_z (std::string label);
  //@}
  
  /**
   * @name SetSideType
   *
   * Set sides for each plane of the cube. See PlaneCoord::set_sides_type.
   * 
   */
  virtual void set_sides_type_Oxy (PlaneCoord::SideType side_1,
				   PlaneCoord::SideType side_2,
				   PlaneCoord::SideType side_3,
				   PlaneCoord::SideType side_4);
  virtual void set_sides_type_Oyz (PlaneCoord::SideType side_1,
				   PlaneCoord::SideType side_2,
				   PlaneCoord::SideType side_3,
				   PlaneCoord::SideType side_4);
  virtual void set_sides_type_Ozx (PlaneCoord::SideType side_1,
				   PlaneCoord::SideType side_2,
				   PlaneCoord::SideType side_3,
				   PlaneCoord::SideType side_4);
  //@}
  
  /**
   * @name SetPosition
   *
   * Set the position of each plane along its normal axis.
   */ 
  virtual void set_position_Oxy (float z);
  virtual void set_position_Oyz (float x);
  virtual void set_position_Ozx (float y);
  //@}
  /**
   * Get one of the different PlaneCoord.
   *
   * @param plane_name must be a valid BasisCube::PlanIndex
   */
  virtual PlaneCoordPtr get_plane (PlanIndex plane_name);

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

 protected:
  /**
   * A set of three PlaneCoord.
   */
  PlaneCoordPtr plans_[3];
  /**
   * Position of the three planes along their normal axis.
   *
   * plan_pos.x is the position of Oyz, etc.
   */
  Position plan_pos_;
  /**
   * Render a specific plane.
   */
  void render_plane (PlanIndex plan_name);
};

#endif //__BASIS_CUBE_H__
