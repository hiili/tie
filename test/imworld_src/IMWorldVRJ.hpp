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
 * @file IMWorldVRJ.hpp
 *
 * The Improbable Mission demo world object (for EVE environment).
 */

/**
 * @class tie::IMWorldVRJ
 *
 * @brief
 * The Improbable Mission demo world object (for EVE environment).
 *
 * Contains various interesting and exciting objects
 */
#ifndef _IMWORLD_HPP_
#define _IMWORLD_HPP_


#include <lifespace/lifespace.hpp>
#include <lifespace/plugins/fluid.hpp>

#include <tie/tie.hpp>

#include <boost/shared_ptr.hpp>
#include <vector>

#define GRAVITY 9.81
#define AVATAR_MASS 61.1


namespace tie {
  
  
  
  
  class IMWorld :
    public lifespace::ODEWorld
  {
  private:
    
    lifespace::ODECollisionRenderer * collisionRenderer;
    
    void makeSky();
    void makeField();
    
  public:
    
    /* constructors/destructors/etc */
    
    /**
     * Creates a new test world which contains a single light source and a
     * basis object.
     *
     * @param graphicsEventHost   A reference to the graphics device's
     *                            EventHost.
     */
    IMWorld( lifespace::EventHost<lifespace::GraphicsEvent> &
               graphicsEventHost, 
               lifespace::plugins::pfluid::FluidHost * inputHost,
               Avatar * avatar,
               UserInterface * ui );
    
    /** */
    virtual ~IMWorld();
  };
  
  
  
  
}   /* namespace tie */




#endif //_IMWORLD_HPP_
