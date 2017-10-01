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
 * @file CylinderVisual.hpp
 *
 * The cylinder visual object.
 */

/**
 * @class tie::CylinderVisual
 *
 * @brief
 * Visual representation of the cylinder object.
 *
 * The cylinder is located at (0, 0, 0) in current coordinates and
 * has a default base and top radius of 0.5 and height of 1.0. The
 * number of slices around the y-axis has a default value of 20.
 * 
 */  

#ifndef TIE_CYLINDERVISUAL_HPP
#define TIE_CYLINDERVISUAL_HPP

#include <GL/gl.h>
#include <lifespace/lifespace.hpp>

namespace tie {

  class CylinderVisual :
    public lifespace::shapes::CustomOpenGLShape
  {
  	GLfloat base;		// base disc radius
  	GLfloat top;		// top disc radius
  	GLfloat height;		// cylinder height
  	GLint slices;		// number of slices around y-axis
  	std::vector<float> sin_val;
  	std::vector<float> cos_val;
  	GLfloat rim_normal_y, rim_normal_xz, rim_normal_len;

  	void updateRimNormalValues();

  public:

    /* constructors/destructors/etc */

	CylinderVisual()
	{
		CylinderVisual( 0.5, 0.5, 1.0, 20 );
	}

	CylinderVisual( GLfloat base, GLfloat top, GLfloat height )
	{
		CylinderVisual( base, top, height, 20 );
	}

    /**
     * Creates a new CylinderVisual using the given values.
     * 
     * @param base   	Base disc radius.
     * @param top   	Top disc radius.
     * @param height	Cylinder height.
     * @param slices	Number of slices around the y-axis
     *
     */
    CylinderVisual( GLfloat base_, GLfloat top_,
    				GLfloat height_, GLint slices_ );

    ~CylinderVisual() {}


    /* accessors */

	GLfloat getBase();
	
	void setBase(GLfloat new_base);
	
	GLfloat getTop();
	
	void setTop(GLfloat new_top);
	
	GLfloat getHeight();
	
	void setHeight(GLfloat new_height);


    /* operations */

    virtual lifespace::shapes::CustomOpenGLShape::Context * render
    ( lifespace::shapes::CustomOpenGLShape::Context * context ) const;
    
  };



} /* namespace */
  
#endif /* TIE_CYLINDERVISUAL_HPP */
