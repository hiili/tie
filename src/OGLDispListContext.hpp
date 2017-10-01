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
 * @file OGLDispListContext.hpp
 *
 * The OpenGL Display List Context object.
 */

#ifndef TIE_OGLDISPLISTCONTEXT_HPP
#define TIE_OGLDISPLISTCONTEXT_HPP


#include <GL/gl.h>
#include <GL/glu.h>
#include <lifespace/lifespace.hpp>


namespace tie {
  
    class OGLDispListContext :
      public lifespace::shapes::CustomOpenGLShape::Context {
      
    public:
      GLint list_id;
      
      OGLDispListContext( GLint list_id_ )
      {
        assert( list_id_ != 0 );
        list_id = list_id_; 
      }
      
      ~OGLDispListContext() {}
      
    };

}  /* namespace */

#endif  /* TIE_OGLDISPLISTCONTEXT_HPP */
