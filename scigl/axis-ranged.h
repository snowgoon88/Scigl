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
 */
#ifndef __AXIS_RANGED_H__
#define __AXIS_RANGED_H__
#include <string>
#include <vector>
#include "font.h"
#include "object.h"

#ifdef HAVE_BOOST
typedef boost::shared_ptr<class AxisRanged> AxisRangedPtr;
#else
typedef class AxisRanged *                  AxisRangedPtr;
#endif

/**
 * Axis with ticks and label, dimension memorized in a range.
 *
 * Axis is drawn along the Ox axis in the local coordinates.
 *
 */

class AxisRanged : public Object {
// ----------------------------------------------------------------------------
public:
  
  /**
   * @name Creation/Destruction
   */
  /**
   * Default constructor
   */
  AxisRanged (void);
  /**
   * Destructor.
   */
  virtual ~AxisRanged (void);
  //@}
  
  /**
   *  @name Rendering
   */
  /**
   * Rendering
   */
  virtual void render (void);
  //@}
  
  /**
   *  @name Positionning
   */
  /**
   * Along its 'line', where does the axis starts
   */
  virtual void set_position_x (float x=-.5);
  /**
   * Along its 'line', size of the axis.
   */
  virtual void set_size (float x=1);
  //@}
  
  /**
   * @name Title
   */
  /**
   * Set title
   *
   * @param label new label
   */
  virtual void        set_title (std::string label);
  /**
   * Get title
   *
   * @return title
   */
  virtual std::string get_title (void);
  //@}
  
  /**
   * @name Range
   */
  /**
   * Ticks range and location as 
   * @param range.min           Value of first major ticks (x=0) (default -1)
   * @param range.max           Value of last major ticks (x=1) (default +1)
   * @param range.major         Number of major ticks (default 3)
   * @param range.minor         Number of minor ticks (default 11)
   */
  virtual void  set_range (Range range);
  /**
   * Ticks range and location as 
   * @param min           Value of first major ticks (x=0) (default -1)
   * @param max           Value of last major ticks (x=1) (default +1)
   * @param major         Number of major ticks (default 3)
   * @param minor         Number of minor ticks (default 11)
   */
  virtual void  set_range (float min, float max, float major=5, float minor=10);
  /**
   * Get Ticks range and location
   */
  virtual Range get_range (void);
  //@}
  /**
   * Set Axis so that values between (min+(max_slide - max), max_slide) is
   * displayed. Depend on the value of _fg_sliding (see set_slinding).
   *
   * @param max_slide   Value of the max point of the sliding range
   */
  virtual void set_slide_max( float max_slide );
  /**
   * Switch between 'sliding' mode on/off. (default is on).
   * on : values between (min+(max_slide - max), max_slide)
   * off : values between (min, max_slide)
   */
  void set_sliding( bool flag );
  /**
   * Get the sliding mode flag.
   */
  bool get_sliding();


  // ----------------------------------------------------------------------------
 protected:
  std::string          label_;            // Axis label
  Range                range_;            // X axis range
  float                max_slide_;        // Max of range if sliding
  bool                 _fg_sliding;       // Sliding or Expand
 public:
  std::vector< float > _x_ratio_major;    // Local position of major ticks
};

#endif
