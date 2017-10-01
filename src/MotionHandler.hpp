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
 * @file MotionHandler.hpp
 *
 * MotionHandler handles user movement in the virtual environment.
 *
 */

/**
 * @class tie::MotionHandler
 *
 * @brief
 * The MotionHandler class for handling user movement in the virtual environment.
 *
 */
#ifndef TIE_MOTIONHANDLER_HPP
#define TIE_MOTIONHANDLER_HPP

#include "Avatar.hpp"

#include <lifespace/lifespace.hpp>

namespace tie {
    
  class MotionHandler :
    public lifespace::Actor,
    public virtual lifespace::Object
  {
    tie::Avatar * avatar;
    float cubeXmin;
    float cubeXmax;
    float cubeZmin;
    float cubeZmax;
    float moveZoneWidth;
    float moveForceFactor;
    float rotTorqueFactor;
    float shoulderWidth;
    float gestureTolerance;
    float gravity;
    float avatarMass;

    void init();

    bool otherMovement;
    
  public:

    enum Controls {
      CTRL_HEAD_LOC_X,
      CTRL_HEAD_LOC_Y,
      CTRL_HEAD_LOC_Z,
      CTRL_HEAD_BASIS_XX,
      CTRL_HEAD_BASIS_XZ,
      CTRL_HEAD_BASIS_ZX,
      CTRL_HEAD_BASIS_ZZ,
      CTRL_RHAND_LOC_X,
      CTRL_RHAND_LOC_Y,
      CTRL_RHAND_LOC_Z,
      CTRL_LHAND_LOC_X,
      CTRL_LHAND_LOC_Y,
      CTRL_LHAND_LOC_Z,
      ControlCount
    };
    
    /* constructors/destructors/etc */
    
    MotionHandler( tie::Avatar * avatar_,
		   float cubeXmin_,
		   float cubeXmax_,
		   float cubeZmin_,
		   float cubeZmax_,
		   float moveZoneWidth_,
		   float moveForceFactor_,
                   float rotTorqueFactor_,
		   float shoulderWidth_,
		   float gestureTolerance_,
                   float gravity_,
                   float avatarMass_ );
    
    
    /* operations */
    
    void localPrepare( lifespace::real dt_ );
    
    void localStep();
    
    virtual void prepare( lifespace::real dt_ );
    
    virtual void step();
    
  };
  
  
}   /* namespace tie */




#endif   /* TIE_MOTIONHANDLER_HPP */
