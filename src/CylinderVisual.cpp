/*
 * Copyright (C) 2004-2005 Eero Uusitalo, Ville Heikkil‰, Mikko Ylikangas, Paul
 * Wagner, Juho Heiskari, Kati M‰‰tt‰, Mikko Purtonen.
 * This file is part of The Incredible Environment.
 * 
 * The Incredible Environment is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 * 
 * The Incredible Environment is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with The Incredible Environment; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/**
 * @file CylinderVisual.cpp
 *
 * Implementation for the cylinder visual object.
 */


#include "CylinderVisual.hpp"
#include "OGLDispListContext.hpp"
#include "types.hpp"
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <GL/gl.h>
#include <GL/glu.h>

CylinderVisual::CylinderVisual( GLfloat base_, GLfloat top_,
			    				GLfloat height_, GLint slices_ ) :
		base( base_ ),
		top( top_ ),
		height( height_ ),
		slices( slices_ )
{
	GLint i;
	GLfloat angle;
	sin_val.resize(slices, 0);
	cos_val.resize(slices, 0);
	if(slices < 3) slices = 3;
	for( i = 0; i < slices; i++) {
		angle = ((GLfloat)i/(GLfloat)slices)*2*M_PI;
		sin_val.at(i) = sin(angle);
		cos_val.at(i) = cos(angle);
	}
	updateRimNormalValues();

}

// calculate values for rim normal
void CylinderVisual::updateRimNormalValues()
{
    // calculate normal values based on the larger disc;
    // this gives slightly greater accuracy and allows the use of a
    // zero-size disc on either end (i.e. the cylinder is in fact a cone)
	if(base > top) {
        rim_normal_y = base * (base - top) / height;
        rim_normal_xz = base;
		rim_normal_len = sqrt(pow(base,2)+pow(rim_normal_y,2));
	} else {
		rim_normal_y = top * (base - top) / height;
		rim_normal_xz = top;
		rim_normal_len = sqrt(pow(top,2)+pow(rim_normal_y,2));
	}

    // normalize y-component length
	rim_normal_y = rim_normal_y / rim_normal_len;

  	// normalize xz-component length
	rim_normal_xz = rim_normal_xz / rim_normal_len;
}

GLfloat CylinderVisual::getBase()
{
	return base;
}

void CylinderVisual::setBase(GLfloat new_base)
{
	base = new_base;
	updateRimNormalValues();
}

GLfloat CylinderVisual::getTop()
{
	return top;
}

void CylinderVisual::setTop(GLfloat new_top)
{
	top = new_top;
	updateRimNormalValues();
}

GLfloat CylinderVisual::getHeight()
{
	return height;
}

void CylinderVisual::setHeight(GLfloat new_height)
{
	height = new_height;
	updateRimNormalValues();
}

shapes::CustomOpenGLShape::Context * 
CylinderVisual::render( shapes::CustomOpenGLShape::Context * context ) const
{ 
  	if( context != NULL ) {
	    glCallList( ((OGLDispListContext *)context)->list_id );
	    return context;
    
  	} else {

	GLint i,j;

#ifdef GLOW_PLATFORM_BIGENDIAN
    OGLDispListContext * ret = 0;
#else
    OGLDispListContext * ret = new OGLDispListContext( glGenLists(1) );
//    glNewList(ret->list_id, GL_COMPILE);
#endif

	// Base disc
	if(base > 0) {
		glBegin(GL_TRIANGLE_STRIP);
			glNormal3f( 0.0f, -1.0f, 0.0f);
			i = 0;
			j = slices - 1;
			while(i <= j) {
				glVertex3f( -sin_val.at(i)*base,
							-height/2,
							-cos_val.at(i)*base);
				glVertex3f( -sin_val.at(j)*base,
							-height/2,
							-cos_val.at(j)*base);
				i++;
				j--;
			}
		glEnd();
    }

	// Top disc
	if(top > 0) {
		glBegin(GL_TRIANGLE_STRIP);
			glNormal3f( 0.0f, 1.0f, 0.0f);
			i = 0;
			j = slices - 1;
			while(i <= j) {
				glVertex3f( sin_val.at(i)*top,
							height/2,
							-cos_val.at(i)*top);
				glVertex3f( sin_val.at(j)*top,
							height/2,
							-cos_val.at(j)*top);
				i++;
				j--;
			}
		glEnd();
    }

	// Cylinder rim
	glBegin(GL_TRIANGLE_STRIP);
		glNormal3f( sin_val.at(slices-1)*rim_normal_xz,
					rim_normal_y,
					-cos_val.at(slices-1)*rim_normal_xz);
		glVertex3f( sin_val.at(slices-1)*base,
					-height/2,
					-cos_val.at(slices-1)*base);
		glVertex3f( sin_val.at(slices-1)*top,
					height/2,
					-cos_val.at(slices-1)*top);
	  	for(i=0; i < slices; i++) {
			glNormal3f( sin_val.at(i)*rim_normal_xz,
						rim_normal_y,
						-cos_val.at(i)*rim_normal_xz);
			glVertex3f( sin_val.at(i)*base,
						-height/2,
						-cos_val.at(i)*base);
			glVertex3f( sin_val.at(i)*top,
						height/2,
						-cos_val.at(i)*top);
		}
	glEnd();

#ifdef GLOW_PLATFORM_BIGENDIAN
#else
//    glEndList();
#endif
    return ret;
  	}
}
