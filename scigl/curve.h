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
 * Modified 25/05/2011 by Alain Dutech.
 */
#ifndef __CURVE_H__
#define __CURVE_H__
#include "object.h"
#include "colormap.h"
#include "data.h"
#include <list>

#if defined(HAVE_BOOST)
#   include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class Curve> CurvePtr;
#else
    typedef class Curve *                  CurvePtr;
#endif


/**
 * Represententation of a l(dynamic) Curve
 *
 */
class Curve : public Object {
public:

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Curve (void);

    /**
     * Destructor.
     */
    virtual ~Curve (void);
    //@}


    /**
     *  @name Rendering
     */
    /**
     * 
     */
    void render	(void);
    //@}


    // _________________________________________________________________________

    /**
     *  @name Data 
     */
    /**
     * Add a new y,z point.
     *
     * @param y point y coordinates
     * @param z point z coordinates
     */
    virtual void add_yz ( double y, double z );

    /**
     * Add a new y,z point + color.
     * WARNING : color is not used.
     *
     * @param y point y coordinates
     * @param z point z coordinates
     * @param cdata data color values
     */
    virtual void add_yz_c ( double y, double z, DataPtr cdata);
    /**
     * Add a new x,y,z point.
     *
     * @param x point x coordinates
     * @param y point y coordinates
     * @param z point z coordinates
     */
    virtual void add_xyz ( double x, double y, double z );

    /**
     * Add a new x,y,z point + color.
     * WARNING : color is not used.
     *
     * @param x point x coordinates
     * @param y point y coordinates
     * @param z point z coordinates
     * @param cdata data color values
     */
    virtual void add_xyz_c ( double x, double y, double z, DataPtr cdata);
    //@}

    // _________________________________________________________________________

    /**
     *  @name Config
     */
    /**
     * Set Range= (min, max, nb_major_ticks, nb_minor_ticks) on coordX.
     * WARNING : min is ignored, max sets also size of cache.
     */
    virtual void set_range_coordX (Range range);
    /**
     * Set Range= (min, max, nb_major_ticks, nb_minor_ticks) on coordY.
     */
    virtual void set_range_coordY (Range range);
    /**
     * Set Range= (min, max, nb_major_ticks, nb_minor_ticks) on coordZ.
     */
    virtual void set_range_coordZ (Range range);
    
    /**
     * Get Range for coordX.
     */
    virtual Range get_range_coordX (void);
    /**
     * Get Range for coordY.
     */
    virtual Range get_range_coordY (void);
    /**
     * Get Range for coordZ.
     */
    virtual Range get_range_coordZ (void);
    
    //@}


    // _________________________________________________________________________

    /**
     * @name Line thickness
     */
    /**
     * Get line thickness
     *
     * @return line thickness
     */
    virtual float get_thickness (void) const;

    /**
     * Set line thickness
     *
     * @param thickness lien thicnkness
     */
    virtual void set_thickness (float thickness);
    //@}


    // _________________________________________________________________________

    /**
     *  @name Colormap
     */
    /**
     * Get colormap
     */
    virtual ColormapPtr get_colormap (void);

    /**
     * Set colormap
     */
    virtual void set_colormap (ColormapPtr colormap);
    //@}

protected:
    
    // _________________________________________________________________________

    /**
     * Reset all data
     */
    void reset_data (void);    


protected:

    // _________________________________________________________________________


    /**
     * Line thickness
     */
    float thickness_;

    /**
     * Colormap
     */
    ColormapPtr cmap_;

    /**
     * Data coordinates
     */
    std::list<Position> _data;
    /**
     * Size of Cache.
     */
    unsigned int _cache_size;

    /**
     * X Range.
     */
    Range _rgX;
    /**
     * Y Range.
     */
    Range _rgY;
    /**
     * Z Range.
     */
    Range _rgZ;
};

#endif
