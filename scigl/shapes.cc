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
#include "shapes.h"

void sphere (GLfloat x, GLfloat y, GLfloat z, GLfloat r)
{
    static GLUquadricObj *quadric = 0;
    static GLuint list = 0;
    if (not quadric) {
        quadric=gluNewQuadric();
        gluQuadricNormals(quadric, GLU_SMOOTH);
        list = glGenLists(1);
        glNewList(list, GL_COMPILE);
        gluSphere(quadric, 1.0, 12, 6);
        glEndList();
    }

    glPushMatrix();
    glScalef (r,r,r);
    glTranslatef(x/r,y/r,z/r);
    glCallList (list);
    glPopMatrix();    
}

void cylinder(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat r1,
              GLfloat x2, GLfloat y2, GLfloat z2, GLfloat r2)
{
    static GLUquadricObj *quadric = 0;
    if (not quadric) {
        quadric=gluNewQuadric();
        gluQuadricNormals(quadric, GLU_SMOOTH);
    }
    float vx = x2-x1;
    float vy = y2-y1;
    float vz = z2-z1;
    // handle the degenerate case with an approximation
    if(vz == 0)
        vz = .00000001;
    float v = sqrt(vx*vx + vy*vy + vz*vz);
    float ax = 57.2957795*acos(vz/v);
    if ( vz < 0.0 )
        ax = -ax;
    float rx = -vy*vz;
    float ry = vx*vz;
    glPushMatrix();
    glTranslatef(x1,y1,z1);
    glRotatef (ax, rx, ry, 0);
    gluCylinder (quadric, r1, r2, v, 6, 1);
    glPopMatrix ();    
}


// _______________________________________________________________________ plane
void
plane (Color fg, Color bg, int nx, int ny)
{
    glColor4fv (bg.data);
    glPolygonOffset (1, 1);
    glDisable (GL_TEXTURE_2D);
    glEnable (GL_POLYGON_OFFSET_FILL);
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glBegin (GL_QUADS);
    glVertex3f (-0.5f, -0.5f, 0);
    glVertex3f (-0.5f,  0.5f, 0);
    glVertex3f ( 0.5f,  0.5f, 0);
    glVertex3f ( 0.5f, -0.5f, 0);
    glEnd();

    glLineWidth (1.0f);
    glDisable (GL_POLYGON_OFFSET_FILL);
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    glEnable (GL_LINE_SMOOTH);
    glColor4fv (fg.data);
    glBegin (GL_QUADS);
    glVertex3f (-0.5f, -0.5f, 0);
    glVertex3f (-0.5f,  0.5f, 0);
    glVertex3f ( 0.5f,  0.5f, 0);
    glVertex3f ( 0.5f, -0.5f, 0);
    glEnd();

    //glEnable (GL_LINE_STIPPLE);
    //glLineStipple (1, 0xf0f0);
    glColor4f (fg.r, fg.g, fg.b, fg.a*.25);
    glLineWidth (0.25f);
    if (nx > 0) {
        glBegin (GL_LINES);
        for (int i=0; i< nx; i++) {
            glVertex3f (-0.5f + (i+1)/float(nx+1), -0.5f, 0.0f);
            glVertex3f (-0.5f + (i+1)/float(nx+1),  0.5f, 0.0f);
        }
        glEnd();
    }
    if (ny > 0) {
        glBegin (GL_LINES);
        for (int i=0; i< ny; i++) {
            glVertex3f (-0.5f, -0.5 + (i+1)/float(ny+1), 0.0f);
            glVertex3f ( 0.5f, -0.5 + (i+1)/float(ny+1), 0.0f);
        }
        glEnd();
    }
    //glDisable (GL_LINE_STIPPLE);
}
