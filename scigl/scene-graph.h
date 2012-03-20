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
 *
 */
#ifndef __SCENE_GRAPH_H__
#define __SCENE_GRAPH_H__

#include "scene.h"
#include "basis-cube.h"
#include <string.h>

#ifdef HAVE_BOOST
    typedef boost::shared_ptr<class SceneGraph> SceneGraphPtr;
#else
    typedef class SceneGraph * SceneGraphPtr;
#endif

/**
 * Class for displaying several graphical objects (Surface, Ballon, etc).
 *
 * The scene is made of a BasisCube and several graphical objects, the
 * range of all axis can be dynamically changed.
 *
 * The image below shows a basic configuration with 2 Surfaces
 * (z=exp(x*x+y*y) and z=x+y).
 *
 * Default configuration based on default BasisCube except:
 * - axis_cube_->set_position ( -.5, -.5, -.5);
 * - set_axis_size( 1.0, 1.0, 1.0 );
 *
 * @todo choose consistent API
 * - get/set all objects (colorbar, surfaces, axis_cube), possibly
 * with iterator
 * - only predifined function, like "set_visible_axis/colorbar/surfaces'...
 *
 * \image html graph.png
 */
class SceneGraph : public Scene {

 public:
  /**
   * @name Creation/Destruction
   */
  /**
   * Default constructor
   */
  SceneGraph (void);
  /**
   * Destructor
   */
  virtual ~SceneGraph (void);
  //@}

  /**
   * Render the scene.
   *
   * Rendering is done in the following order:
   * -# Widgets with a negative z
   * -# graphical objects
   * -# BasisCube, so as to allow for transparency
   * -# Widgets with a positive z
   */
  virtual void render (void);

  /**
   * @name SetAxisSize
   *
   * Sets the size of all axis.
   */
  virtual void set_axis_size (Size s);
  virtual void set_axis_size (float size_x=1, float size_y=1, float size_z=1);
  //@}
  
  /**
   * @name SetRange
   *
   * Set Range = (min, max, nb_major_ticks, nb_minor_ticks)
   * for each coordinate.
   * 
   * @todo Update graphical objects accordingly.
   */
  virtual void set_range_coord_x (Range range);
  virtual void set_range_coord_y (Range range);
  virtual void set_range_coord_z (Range range);
  //@}

  

  /**
   * Remove a graphical object
   *
   * @return true if object removed
   */
  virtual bool remove (ObjectPtr object);
  /**
   * Get a graphical object by its id.
   *
   * @return ObjectPtr of empty sharedPtr/NULL
   */
  virtual ObjectPtr get_object( unsigned int id);

  /**
   * Get BasisCube.
   */
  BasisCubePtr get_basis_cube (void);

 protected:
  /**
   * A BasisCube to display coordinates.
   */
  BasisCubePtr axis_cube_;

 public:
  /**
   * For Debug : dump list of graphical objects as a string.
   */
  std::string dump_objects (void);
};

#endif //__GRAPH_H__
