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
 * @file UserInterface.hpp
 *
 * <yhden lauseen kuvaus, sama kuin alla>
 */

/**
 * @class tie::UserInterface
 *
 * @brief
 * UserInterface subspace that handles special user input
 *
 * The UserInterface handles special user gestures for movement 
 * (forward, backward, strafe), rotation etc.
 */
#ifndef TIE_USERINTERFACE_HPP
#define TIE_USERINTERFACE_HPP

#include "MotionHandler.hpp"
#include "GestureHandler.hpp"
#include "SensorController.hpp"

#include <lifespace/lifespace.hpp>

#include <lifespace/plugins/fluid.hpp>

namespace tie {
    
  class UserInterface:
  	public lifespace::Subspace
  {
  	tie::Avatar * avatar;
    
    void fillFluidHeadKeymap( lifespace::Controller::ControlMap & keymap );
    
    void fillFluidHeadKeymapGest( lifespace::Controller::ControlMap & keymap );
    
    void fillFluidRightHandKeymap( lifespace::Controller::ControlMap & keymap );

    void fillFluidRightHandKeymapGest( lifespace::Controller::ControlMap & keymap );
    
    void fillFluidLeftHandKeymap( lifespace::Controller::ControlMap & keymap );

    void fillFluidLeftHandKeymapGest( lifespace::Controller::ControlMap & keymap );
    
    void init( lifespace::plugins::pfluid::FluidHost * inputHost_,
               float gravity_,
               float avatarMass_ );
    
  public:
    
    /* constructors/destructors/etc */
    
    UserInterface( tie::Avatar * avatar_,
                   lifespace::plugins::pfluid::FluidHost * inputHost_,
                   float gravity_,
                   float avatarMass_ );
    
    
    
  };
  
}   /* namespace tie */

#endif   /* TIE_USERINTERFACE_HPP */
