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
 * @file TIEMaterials.hpp
 *
 * Static material definitions for tie.
 */

/**
 * @class tie::TIEMaterials
 *
 * @brief
 * Static material definitions for tie.
 *
 * To use predefined materials, include TIEMaterials.hpp -file. Then usage:
 * TIEMat::<material name>.
 */
#ifndef TIE_TIEMATERIALS_HPP
#define TIE_TIEMATERIALS_HPP

#include <lifespace/lifespace.hpp>
#include <boost/shared_ptr.hpp>
#include <GL/gl.h>




namespace tie {
  
  
  
  
  struct TIEMat {
    static const GLfloat none[4];
    static const GLfloat white[4];
    static const GLfloat gray6[4];
    static const GLfloat gray3[4];
    static const GLfloat red[4];
    static const GLfloat green2[4];
    static const GLfloat green4[4];
    static const GLfloat green6[4];
    static const GLfloat green8[4];
    static const GLfloat green[4];
    static const GLfloat smooth[1];
    static const GLfloat polished[1];
    static const lifespace::Material blackMat;
    static const lifespace::Material redMat;
    static const lifespace::Material whiteMat;
    static const lifespace::Material redEmissionMat;
    static const lifespace::Material emissionMat;

    static const lifespace::Material groundMat;
    
    static const boost::shared_ptr
    <const lifespace::CollisionMaterial> testCollMat;
  };
  
  
  
  
}   /* namespace tie */

#endif   /* TIE_TIEMATERIALS_HPP */
