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
 */
#include "cloud.h"
#include "shapes.h"

#include <iostream>

// ________________________________________________________________________ Cloud
Cloud::Cloud (void) : Object ()
{
    set_size (1.0f, 1.0f, 1.0f);
    set_position (0.0f, 0.0f, 0.0f);
    set_fg_color (0.5f, 0.5f, 0.5f, 1.0f);
    set_bg_color (1.0f, 1.0f, 1.0f, 1.0f);
    set_thickness (1.01);
    cmap_ = Colormap::Hot();
    reset_data();

    std::ostringstream oss;
    oss << "Cloud_" << id_;
    set_name (oss.str());
}


// _______________________________________________________________________ ~Cloud
Cloud::~Cloud (void)
{}


// ______________________________________________________________________ render
void
Cloud::render (void)
{
    if ((not xdata_) or (not ydata_) or (not zdata_)) {
        return;
    }
    compute_visibility();
    if (!get_visible()) {
        return;
    }
    unsigned int width        = xdata_->get_width();
    unsigned int xdata_stride = xdata_->get_stride();
    unsigned int ydata_stride = ydata_->get_stride();
    unsigned int zdata_stride = zdata_->get_stride();
    Color c;
    double x, y, z, s, v, r, g, b, a;
    r = fg_color_.r;
    g = fg_color_.g;
    b = fg_color_.b;
    a = fg_color_.a;

    // -------------------------------------------------------------------------
    //  Rendering using GL_POINTS
    // -------------------------------------------------------------------------
    if ((thickness_ == 0) or (thickness_ > 1.0)) {
        glEnable (GL_BLEND);
        glEnable (GL_POINT_SMOOTH);
        //glColor4f (r,g,b,a*alpha_);
        if (thickness_ == 0)
            glPointSize (1.0);
        else
            glPointSize (thickness_);

        // XYZ
        // ---------------------------------------------------------------------
        if ((not cdata_) and (not sdata_)) {
            GLbyte *xdata = (GLbyte *) xdata_->get_data();
            GLbyte *ydata = (GLbyte *) ydata_->get_data();
            GLbyte *zdata = (GLbyte *) zdata_->get_data();
	    glColor4f (r,g,b,a*alpha_);
            glBegin(GL_POINTS);
            for (unsigned int i=0; i<width; i++) {
                x = * (GLdouble *)(xdata+i*xdata_stride);
                y = * (GLdouble *)(ydata+i*ydata_stride);
                z = * (GLdouble *)(zdata+i*zdata_stride);
                glVertex3f (x,y,z);
            }
            glEnd();
        }

        // XYZ + S
        // ---------------------------------------------------------------------
        else if ((not cdata_) and (sdata_)) {
            GLbyte *xdata = (GLbyte *) xdata_->get_data();
            GLbyte *ydata = (GLbyte *) ydata_->get_data();
            GLbyte *zdata = (GLbyte *) zdata_->get_data();
            GLbyte *sdata = (GLbyte *) sdata_->get_data();
            unsigned int sdata_stride = sdata_->get_stride();
	    glColor4f (r,g,b,a*alpha_);
            for (unsigned int i=0; i<width; i++) {
                x = * (GLdouble *)(xdata+i*xdata_stride);
                y = * (GLdouble *)(ydata+i*ydata_stride);
                z = * (GLdouble *)(zdata+i*zdata_stride);
                s = * (GLdouble *)(sdata+i*sdata_stride);
                glPointSize (thickness_*s);
                glBegin(GL_POINTS);
                glVertex3f (x,y,z);
                glEnd();
            }

        }
        else if ((cdata_) and (not sdata_)) {
            GLbyte *xdata = (GLbyte *) xdata_->get_data();
            GLbyte *ydata = (GLbyte *) ydata_->get_data();
            GLbyte *zdata = (GLbyte *) zdata_->get_data();
            GLbyte *cdata = (GLbyte *) cdata_->get_data();
            unsigned int cdata_stride = cdata_->get_stride();
            
            // XYZ + C
            // -----------------------------------------------------------------
            if (cdata_->get_depth() == 1) {
                glBegin(GL_POINTS);
                for (unsigned int i=0; i<width; i++) {
                    x = * (GLdouble *)(xdata+i*xdata_stride);
                    y = * (GLdouble *)(ydata+i*ydata_stride);
                    z = * (GLdouble *)(zdata+i*zdata_stride);
                    v = * (GLdouble *)(cdata+i*cdata_stride + 0*sizeof(GLdouble));
                    Color c = (*cmap_)(v);
                    r = c.r;
                    g = c.g;
                    b = c.b;
                    a = c.a;
                    glColor4f(r,g,b,a*alpha_);
		    glVertex3f (x,y,z);
                }
                glEnd();
            }
            // XYZ + RGB
            // -----------------------------------------------------------------
            else if (cdata_->get_depth() == 3) {
                glBegin(GL_POINTS);
                for (unsigned int i=0; i<width; i++) {
                    x = * (GLdouble *)(xdata+i*xdata_stride);
                    y = * (GLdouble *)(ydata+i*ydata_stride);
                    z = * (GLdouble *)(zdata+i*zdata_stride);
                    r = * (GLdouble *)(cdata+i*cdata_stride + 0*sizeof(GLdouble));
                    g = * (GLdouble *)(cdata+i*cdata_stride + 1*sizeof(GLdouble));
                    b = * (GLdouble *)(cdata+i*cdata_stride + 2*sizeof(GLdouble));
                    glColor4f(r,g,b,alpha_);
                    glVertex3f (x,y,z);
                }
                glEnd();
            }
            // XYZ + RGBA
            // -----------------------------------------------------------------
            else if (cdata_->get_depth() == 4) {
	      glBegin(GL_POINTS);
                for (unsigned int i=0; i<width; i++) {
                    x = * (GLdouble *)(xdata+i*xdata_stride);
                    y = * (GLdouble *)(ydata+i*ydata_stride);
                    z = * (GLdouble *)(zdata+i*zdata_stride);
                    r = * (GLdouble *)(cdata+i*cdata_stride + 0*sizeof(GLdouble));
                    g = * (GLdouble *)(cdata+i*cdata_stride + 1*sizeof(GLdouble));
                    b = * (GLdouble *)(cdata+i*cdata_stride + 2*sizeof(GLdouble));
                    a = * (GLdouble *)(cdata+i*cdata_stride + 3*sizeof(GLdouble));
                    glColor4f(r,g,b,a*alpha_);
                    glVertex3f (x,y,z);
                }
		glEnd();
            }
        }
        else if ((cdata_) and (sdata_)) {
            GLbyte *xdata = (GLbyte *) xdata_->get_data();
            GLbyte *ydata = (GLbyte *) ydata_->get_data();
            GLbyte *zdata = (GLbyte *) zdata_->get_data();
            GLbyte *cdata = (GLbyte *) cdata_->get_data();
            GLbyte *sdata = (GLbyte *) sdata_->get_data();
            unsigned int cdata_stride = cdata_->get_stride();
            unsigned int sdata_stride = sdata_->get_stride();
            
            // XYZ + S + C
            // -----------------------------------------------------------------
            if (cdata_->get_depth() == 1) {
                for (unsigned int i=0; i<width; i++) {
                    x = * (GLdouble *)(xdata+i*xdata_stride);
                    y = * (GLdouble *)(ydata+i*ydata_stride);
                    z = * (GLdouble *)(zdata+i*zdata_stride);
                    s = * (GLdouble *)(sdata+i*sdata_stride);
                    v = * (GLdouble *)(cdata+i*cdata_stride + 0*sizeof(GLdouble));
                    Color c = (*cmap_)(v);
                    r = c.r;
                    g = c.g;
                    b = c.b;
                    a = c.a;
                    glPointSize (thickness_*s);
                    glBegin(GL_POINTS);
                    glVertex3f (x,y,z);
                    glEnd();
                }
            }
            // XYZ + S + RGB
            // -----------------------------------------------------------------
            else if (cdata_->get_depth() == 3) {
                for (unsigned int i=0; i<width; i++) {
                    x = * (GLdouble *)(xdata+i*xdata_stride);
                    y = * (GLdouble *)(ydata+i*ydata_stride);
                    z = * (GLdouble *)(zdata+i*zdata_stride);
                    s = * (GLdouble *)(sdata+i*sdata_stride);
                    r = * (GLdouble *)(cdata+i*cdata_stride + 0*sizeof(GLdouble));
                    g = * (GLdouble *)(cdata+i*cdata_stride + 1*sizeof(GLdouble));
                    b = * (GLdouble *)(cdata+i*cdata_stride + 2*sizeof(GLdouble));
                    glColor4f(r,g,b,a*alpha_);
                    glPointSize (thickness_*s);
                    glBegin(GL_POINTS);
                    glVertex3f (x,y,z);
                    glEnd();
                }
            }
            // XYZ + S + RGBA
            // -----------------------------------------------------------------
            else if (cdata_->get_depth() == 4) {
                for (unsigned int i=0; i<width; i++) {
                    x = * (GLdouble *)(xdata+i*xdata_stride);
                    y = * (GLdouble *)(ydata+i*ydata_stride);
                    z = * (GLdouble *)(zdata+i*zdata_stride);
                    s = * (GLdouble *)(sdata+i*sdata_stride);
                    r = * (GLdouble *)(cdata+i*cdata_stride + 0*sizeof(GLdouble));
                    g = * (GLdouble *)(cdata+i*cdata_stride + 1*sizeof(GLdouble));
                    b = * (GLdouble *)(cdata+i*cdata_stride + 2*sizeof(GLdouble));
                    a = * (GLdouble *)(cdata+i*cdata_stride + 3*sizeof(GLdouble));
                    glColor4f(r,g,b,a*alpha_);
                    glPointSize (thickness_*s);
                    glBegin(GL_POINTS);
                    glVertex3f (x,y,z);
                    glEnd();
                }
            }
        }
	glDisable (GL_BLEND);
        glDisable (GL_POINT_SMOOTH);
    }


    // -------------------------------------------------------------------------
    //  Rendering using sphere
    // -------------------------------------------------------------------------
    else {
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    
        // XYZ
        // ---------------------------------------------------------------------
        if ((not cdata_) and (not sdata_)) {
            GLbyte *xdata = (GLbyte *) xdata_->get_data();
            GLbyte *ydata = (GLbyte *) ydata_->get_data();
            GLbyte *zdata = (GLbyte *) zdata_->get_data();
	    glColor4f (r,g,b,a*alpha_);
            for (unsigned int i=0; i<width; i++) {
                x = * (GLdouble *)(xdata+i*xdata_stride);
                y = * (GLdouble *)(ydata+i*ydata_stride);
                z = * (GLdouble *)(zdata+i*zdata_stride);
                sphere (x,y,z, thickness_);
            }
        }
        // XYZ + S
        // ---------------------------------------------------------------------
        else if ((not cdata_) and (sdata_)) {
            GLbyte *xdata = (GLbyte *) xdata_->get_data();
            GLbyte *ydata = (GLbyte *) ydata_->get_data();
            GLbyte *zdata = (GLbyte *) zdata_->get_data();
            GLbyte *sdata = (GLbyte *) sdata_->get_data();
	    glColor4f (r,g,b,a*alpha_);
            unsigned int sdata_stride = sdata_->get_stride();
            for (unsigned int i=0; i<width; i++) {
                x = * (GLdouble *)(xdata+i*xdata_stride);
                y = * (GLdouble *)(ydata+i*ydata_stride);
                z = * (GLdouble *)(zdata+i*zdata_stride);
                s = * (GLdouble *)(sdata+i*sdata_stride);
                sphere (x,y,z, thickness_*s);
            }
        }
        else if ((cdata_) and (not sdata_)) {
            GLbyte *xdata = (GLbyte *) xdata_->get_data();
            GLbyte *ydata = (GLbyte *) ydata_->get_data();
            GLbyte *zdata = (GLbyte *) zdata_->get_data();
            GLbyte *cdata = (GLbyte *) cdata_->get_data();
            unsigned int cdata_stride = cdata_->get_stride();
            
            // XYZ + C
            // -----------------------------------------------------------------
            if (cdata_->get_depth() == 1) {
                for (unsigned int i=0; i<width; i++) {
                    x = * (GLdouble *)(xdata+i*xdata_stride);
                    y = * (GLdouble *)(ydata+i*ydata_stride);
                    z = * (GLdouble *)(zdata+i*zdata_stride);
                    v = * (GLdouble *)(cdata+i*cdata_stride + 0*sizeof(GLdouble));
                    Color c = (*cmap_)(v);
                    r = c.r;
                    g = c.g;
                    b = c.b;
                    a = c.a;
                    glColor4f(r,g,b,a*alpha_);
                    sphere (x,y,z, thickness_);
                }
            }
            // XYZ + RGB
            // -----------------------------------------------------------------
            else if (cdata_->get_depth() == 3) {
                for (unsigned int i=0; i<width; i++) {
                    x = * (GLdouble *)(xdata+i*xdata_stride);
                    y = * (GLdouble *)(ydata+i*ydata_stride);
                    z = * (GLdouble *)(zdata+i*zdata_stride);
                    r = * (GLdouble *)(cdata+i*cdata_stride + 0*sizeof(GLdouble));
                    g = * (GLdouble *)(cdata+i*cdata_stride + 1*sizeof(GLdouble));
                    b = * (GLdouble *)(cdata+i*cdata_stride + 2*sizeof(GLdouble));
                    glColor4f(r,g,b,a*alpha_);
                    sphere (x,y,z, thickness_);
                }
            }
            // XYZ + RGBA
            // -----------------------------------------------------------------
            else if (cdata_->get_depth() == 4) {
	      for (unsigned int i=0; i<width; i++) {
                    x = * (GLdouble *)(xdata+i*xdata_stride);
                    y = * (GLdouble *)(ydata+i*ydata_stride);
                    z = * (GLdouble *)(zdata+i*zdata_stride);
                    r = * (GLdouble *)(cdata+i*cdata_stride + 0*sizeof(GLdouble));
                    g = * (GLdouble *)(cdata+i*cdata_stride + 1*sizeof(GLdouble));
                    b = * (GLdouble *)(cdata+i*cdata_stride + 2*sizeof(GLdouble));
                    a = * (GLdouble *)(cdata+i*cdata_stride + 3*sizeof(GLdouble));
                    glColor4f(r,g,b,a*alpha_);
                    sphere (x,y,z, thickness_);
                }
            }
        }

        else if ((cdata_) and (sdata_)) {
            GLbyte *xdata = (GLbyte *) xdata_->get_data();
            GLbyte *ydata = (GLbyte *) ydata_->get_data();
            GLbyte *zdata = (GLbyte *) zdata_->get_data();
            GLbyte *cdata = (GLbyte *) cdata_->get_data();
            GLbyte *sdata = (GLbyte *) sdata_->get_data();
            unsigned int cdata_stride = cdata_->get_stride();
            unsigned int sdata_stride = sdata_->get_stride();
            
            // XYZ + S + C
            // -----------------------------------------------------------------
            if (cdata_->get_depth() == 1) {
                for (unsigned int i=0; i<width; i++) {
                    x = * (GLdouble *)(xdata+i*xdata_stride);
                    y = * (GLdouble *)(ydata+i*ydata_stride);
                    z = * (GLdouble *)(zdata+i*zdata_stride);
                    s = * (GLdouble *)(sdata+i*sdata_stride);
                    v = * (GLdouble *)(cdata+i*cdata_stride + 0*sizeof(GLdouble));
                    Color c = (*cmap_)(v);
                    r = c.r;
                    g = c.g;
                    b = c.b;
                    a = c.a;
                    glColor4f(r,g,b,a*alpha_);
                    sphere (x,y,z, thickness_*s);
                }
            }
            // XYZ + S + RGB
            // -----------------------------------------------------------------
            else if (cdata_->get_depth() == 3) {
                for (unsigned int i=0; i<width; i++) {
                    x = * (GLdouble *)(xdata+i*xdata_stride);
                    y = * (GLdouble *)(ydata+i*ydata_stride);
                    z = * (GLdouble *)(zdata+i*zdata_stride);
                    s = * (GLdouble *)(sdata+i*sdata_stride);
                    r = * (GLdouble *)(cdata+i*cdata_stride + 0*sizeof(GLdouble));
                    g = * (GLdouble *)(cdata+i*cdata_stride + 1*sizeof(GLdouble));
                    b = * (GLdouble *)(cdata+i*cdata_stride + 2*sizeof(GLdouble));
                    glColor4f(r,g,b,a*alpha_);
                    sphere (x,y,z, thickness_*s);
                }
            }
            // XYZ + S + RGBA
            // -----------------------------------------------------------------
            else if (cdata_->get_depth() == 4) {
                for (unsigned int i=0; i<width; i++) {
                    x = * (GLdouble *)(xdata+i*xdata_stride);
                    y = * (GLdouble *)(ydata+i*ydata_stride);
                    z = * (GLdouble *)(zdata+i*zdata_stride);
                    s = * (GLdouble *)(sdata+i*sdata_stride);
                    r = * (GLdouble *)(cdata+i*cdata_stride + 0*sizeof(GLdouble));
                    g = * (GLdouble *)(cdata+i*cdata_stride + 1*sizeof(GLdouble));
                    b = * (GLdouble *)(cdata+i*cdata_stride + 2*sizeof(GLdouble));
                    a = * (GLdouble *)(cdata+i*cdata_stride + 3*sizeof(GLdouble));
                    glColor4f(r,g,b,a*alpha_);
                    sphere (x,y,z, thickness_*s);
                }
            }
        }
    }
}



// ________________________________________________________________ set_xyz_data
void
Cloud::set_xyz_data (DataPtr xdata, DataPtr ydata, DataPtr zdata)
{
    if ((not xdata) or (not ydata) or (not zdata)) {
        throw std::invalid_argument
            ("cloud: X,Y,Z data must be valid data");
    }
    if ((not (xdata->get_type() == xdata->get_type())) or
        (not (xdata->get_type() == zdata->get_type()))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z data must be of same type");
    }
    if ((not (xdata->get_type() == GL_DOUBLE))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z data must be of type GL_DOUBLE");
    }
    if ((not (xdata->get_width() == xdata->get_width())) or
        (not (xdata->get_width() == zdata->get_width()))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z data must be of same size");
    }
    if ((not (xdata->get_height() == 1)) or
        (not (ydata->get_height() == 1)) or
        (not (zdata->get_height() == 1))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z data must be linear (height=1)");
    }
    reset_data();
    xdata_ = xdata;
    ydata_ = ydata;
    zdata_ = zdata;
    id_ = id_counter_;
    id_ += xdata->get_width();
}


// ______________________________________________________________ set_xyz_s_data
void
Cloud::set_xyz_s_data (DataPtr xdata, DataPtr ydata, DataPtr zdata,
                       DataPtr sdata)
{
    if ((not xdata) or (not ydata) or (not zdata) or
        (not sdata) ) {
        throw std::invalid_argument
            ("cloud: X,Y,Z,S data must be valid data");
    }
    if ((not (xdata->get_type() == xdata->get_type())) or
        (not (xdata->get_type() == zdata->get_type()))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z data must be of same type");
    }
    if ((not (xdata->get_type() == GL_DOUBLE))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z data must be of type GL_DOUBLE");
    }
    if ((not (xdata->get_width() == xdata->get_width())) or
        (not (xdata->get_width() == zdata->get_width())) or
        (not (xdata->get_width() == sdata->get_width()))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z,S data must be of same size");
    }
    if ((not (xdata->get_height() == 1)) or
        (not (ydata->get_height() == 1)) or
        (not (zdata->get_height() == 1)) or
        (not (sdata->get_height() == 1))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z,S data must be linear (height=1)");
    }
    if ((not (sdata->get_type() == GL_DOUBLE))) {
        throw std::invalid_argument
            ("cloud: S data must be of type GL_DOUBLE");
    }
    reset_data();
    xdata_ = xdata;
    ydata_ = ydata;
    zdata_ = zdata;
    sdata_ = sdata;
    id_ = id_counter_;
    id_ += xdata->get_width();
}


// ______________________________________________________________ set_xyz_c_data
void
Cloud::set_xyz_c_data (DataPtr xdata, DataPtr ydata, DataPtr zdata,
                       DataPtr cdata)
{
    if ((not xdata) or (not ydata) or (not zdata) or (not cdata)) {
        throw std::invalid_argument
            ("cloud: X,Y,Z,C data must be valid data");
    }
    if ((not (xdata->get_type() == xdata->get_type())) or
        (not (xdata->get_type() == zdata->get_type()))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z data must be of same type");
    }
    if ((not (xdata->get_type() == GL_DOUBLE))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z data must be of type GL_DOUBLE");
    }
    if ((not (xdata->get_width() == xdata->get_width())) or
        (not (xdata->get_width() == zdata->get_width())) or
        (not (xdata->get_width() == cdata->get_width()))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z,C data must be of same size");
    }
    if ((not (xdata->get_height() == 1)) or
        (not (ydata->get_height() == 1)) or
        (not (zdata->get_height() == 1)) or
        (not (cdata->get_height() == 1))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z,C data must be linear (height=1)");
    }
    if ((not (cdata->get_type() == GL_DOUBLE))) {
        throw std::invalid_argument
            ("cloud: C data must be of type GL_DOUBLE");
    }
    if ((not (cdata->get_depth() == 1)) and
        (not (cdata->get_depth() == 3)) and
        (not (cdata->get_depth() == 4))) {
        throw std::invalid_argument
            ("cloud: C data must have 1 (cmap value), 3 (rgb) or 4 (rgba) components");
    }
    reset_data();
    xdata_ = xdata;
    ydata_ = ydata;
    zdata_ = zdata;
    cdata_ = cdata;
    id_ = id_counter_;
    id_ += xdata->get_width();

}

// ____________________________________________________________ set_xyz_s_c_data
void
Cloud::set_xyz_s_c_data (DataPtr xdata, DataPtr ydata, DataPtr zdata,
                        DataPtr sdata, DataPtr cdata)
{
    if ((not xdata) or (not ydata) or (not zdata) or
        (not cdata) or (not sdata)) {
        throw std::invalid_argument
            ("cloud: X,Y,Z,S,C data must be valid data");
    }
    if ((not (xdata->get_type() == xdata->get_type())) or
        (not (xdata->get_type() == zdata->get_type()))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z data must be of same type");
    }
    if ((not (xdata->get_type() == GL_DOUBLE))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z data must be of type GL_DOUBLE");
    }
    if ((not (xdata->get_width() == xdata->get_width())) or
        (not (xdata->get_width() == zdata->get_width())) or
        (not (xdata->get_width() == sdata->get_width())) or
        (not (xdata->get_width() == cdata->get_width()))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z,S,C data must be of same size");
    }
    if ((not (xdata->get_height() == 1)) or
        (not (ydata->get_height() == 1)) or
        (not (zdata->get_height() == 1)) or
        (not (sdata->get_height() == 1)) or
        (not (cdata->get_height() == 1))) {
        throw std::invalid_argument
            ("cloud: X,Y,Z,C data must be cloudar (height=1)");
    }
    if ((not (sdata->get_type() == GL_DOUBLE)) or
        (not (cdata->get_type() == GL_DOUBLE))) {
        throw std::invalid_argument
            ("cloud: S,C data must be of type GL_DOUBLE");
    }
    if ((not (cdata->get_depth() == 1)) and
        (not (cdata->get_depth() == 3)) and
        (not (cdata->get_depth() == 4))) {
        throw std::invalid_argument
            ("cloud: C data must have 1 (cmap value), 3 (rgb) or 4 (rgba) components");
    }
    reset_data();
    xdata_ = xdata;
    ydata_ = ydata;
    zdata_ = zdata;
    cdata_ = cdata;
    sdata_ = sdata;
    id_ = id_counter_;
    id_ += xdata->get_width();
}

// __________________________________________________________________ reset_data
void
Cloud::reset_data (void)
{
#if defined(HAVE_BOOST)
    xdata_ = DataPtr();
    ydata_ = DataPtr();
    zdata_ = DataPtr();
    sdata_ = DataPtr();
    cdata_ = DataPtr();
#else
    xdata_ = 0;
    ydata_ = 0;
    zdata_ = 0;
    sdata_ = 0;
    cdata_ = 0;
#endif
}


// _______________________________________________________________ get_thickness
float
Cloud::get_thickness (void) const
{
    return thickness_;
}


// _______________________________________________________________ set_thickness
void
Cloud::set_thickness (float thickness)
{
    thickness_ = thickness;
}


// ________________________________________________________________ get_colormap
ColormapPtr
Cloud::get_colormap (void)
{
    return cmap_;
}


// ________________________________________________________________ set_colormap
void
Cloud::set_colormap (ColormapPtr colormap)
{
    cmap_ = colormap;
}
