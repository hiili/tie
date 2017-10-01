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
 * @file MotionHandler.cpp
 *
 * The MotionHandler object implementation
 */

#include "MotionHandler.hpp"

#include "Avatar.hpp"
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <iostream>
using std::cout;


// initialize necessary controls
void MotionHandler::init()
{
  controls[CTRL_HEAD_LOC_X]           = Control( Control::ContinuousMode );
  controls[CTRL_HEAD_LOC_Y]           = Control( Control::ContinuousMode );
  controls[CTRL_HEAD_LOC_Z]           = Control( Control::ContinuousMode );
  controls[CTRL_HEAD_BASIS_XX]        = Control( Control::ContinuousMode );
  controls[CTRL_HEAD_BASIS_XZ]        = Control( Control::ContinuousMode );
  controls[CTRL_HEAD_BASIS_ZX]        = Control( Control::ContinuousMode );
  controls[CTRL_HEAD_BASIS_ZZ]        = Control( Control::ContinuousMode );
  controls[CTRL_RHAND_LOC_X]          = Control( Control::ContinuousMode );
  controls[CTRL_RHAND_LOC_Y]          = Control( Control::ContinuousMode );
  controls[CTRL_RHAND_LOC_Z]          = Control( Control::ContinuousMode );
  controls[CTRL_LHAND_LOC_X]          = Control( Control::ContinuousMode );
  controls[CTRL_LHAND_LOC_Y]          = Control( Control::ContinuousMode );
  controls[CTRL_LHAND_LOC_Z]          = Control( Control::ContinuousMode );
}

MotionHandler::MotionHandler( Avatar * avatar_,
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
                              float avatarMass_ ) :
  Object(),
  avatar( avatar_ ),
  cubeXmin( cubeXmin_ ),
  cubeXmax( cubeXmax_ ),
  cubeZmin( cubeZmin_ ),
  cubeZmax( cubeZmax_ ),
  moveZoneWidth( moveZoneWidth_ ),
  moveForceFactor( moveForceFactor_ ),
  rotTorqueFactor( rotTorqueFactor_ ),
  shoulderWidth( shoulderWidth_ ),
  gestureTolerance( gestureTolerance_ ),
  gravity ( gravity_ ),
  avatarMass ( avatarMass_ ),
  otherMovement( false )
{
  init();
}

void MotionHandler::localPrepare( real dt_ )
{
  // handle movement towards +Z
  if( readControl(CTRL_HEAD_LOC_Z) > cubeZmax - moveZoneWidth) {
    avatar->getLocator()->addForceRel(
      makeVector3d( 0,
                    ( otherMovement? 0.0:0.5 * gravity * avatarMass),
                    (readControl(CTRL_HEAD_LOC_Z)
                    - (cubeZmax - moveZoneWidth))*moveForceFactor));
    otherMovement = true;
  }

  // handle movement towards -Z
  if( readControl(CTRL_HEAD_LOC_Z) < cubeZmin + moveZoneWidth) {
    avatar->getLocator()->addForceRel(
      makeVector3d( 0,
                    ( otherMovement? 0.0:0.5 * gravity * avatarMass),
                    (readControl(CTRL_HEAD_LOC_Z)
                    - (cubeZmin + moveZoneWidth))*moveForceFactor));
    otherMovement = true;
  }

  // handle movement towards +X
  if( readControl(CTRL_HEAD_LOC_X) > cubeXmax - moveZoneWidth) {
    avatar->getLocator()->addForceRel(
      makeVector3d( (readControl(CTRL_HEAD_LOC_X)
                    - (cubeXmax - moveZoneWidth))*moveForceFactor,
                    ( otherMovement? 0.0:0.5 * gravity * avatarMass),
                    0));
    otherMovement = true;
  }

  // handle movement towards -X
  if( readControl(CTRL_HEAD_LOC_X) < cubeXmin + moveZoneWidth) {
    avatar->getLocator()->addForceRel(
      makeVector3d( (readControl(CTRL_HEAD_LOC_X)
                     - (cubeXmin + moveZoneWidth))*moveForceFactor,
                    ( otherMovement? 0.0:0.5 * gravity * avatarMass),
                    0));
    otherMovement = true;
  }

  // handle avatar rotation
  if(
     // check that the head and hands are aligned on the XZ-plane
     readControl(CTRL_HEAD_LOC_X) - readControl(CTRL_LHAND_LOC_X)
     < readControl(CTRL_RHAND_LOC_X) - readControl(CTRL_HEAD_LOC_X) + gestureTolerance
     && readControl(CTRL_HEAD_LOC_X) - readControl(CTRL_LHAND_LOC_X)
     > readControl(CTRL_RHAND_LOC_X) - readControl(CTRL_HEAD_LOC_X) - gestureTolerance

     && readControl(CTRL_HEAD_LOC_Z) - readControl(CTRL_LHAND_LOC_Z)
     < readControl(CTRL_RHAND_LOC_Z) - readControl(CTRL_HEAD_LOC_Z) + gestureTolerance
     && readControl(CTRL_HEAD_LOC_Z) - readControl(CTRL_LHAND_LOC_Z)
     > readControl(CTRL_RHAND_LOC_Z) - readControl(CTRL_HEAD_LOC_Z) - gestureTolerance

     // check that the head is on the same height as the hands
     && readControl(CTRL_HEAD_LOC_Y) > readControl(CTRL_LHAND_LOC_Y) - gestureTolerance
     && readControl(CTRL_HEAD_LOC_Y) > readControl(CTRL_RHAND_LOC_Y) - gestureTolerance
     && readControl(CTRL_HEAD_LOC_Y) < readControl(CTRL_LHAND_LOC_Y) + gestureTolerance
     && readControl(CTRL_HEAD_LOC_Y) < readControl(CTRL_RHAND_LOC_Y) + gestureTolerance

     // check that hands are far enough from each other
     && sqrt(pow(readControl(CTRL_RHAND_LOC_X) - readControl(CTRL_LHAND_LOC_X), 2)
	     + pow(readControl(CTRL_RHAND_LOC_Z) - readControl(CTRL_LHAND_LOC_Z), 2))
     > shoulderWidth
     )
    {
#ifdef DEBUG_GESTURES      
      cout << "MotionHandler::prepare(): turning conditions met, head angle calc begin.\n";
#endif
      
      // calculate ahead vector and vector from left hand to right hand
      // and inner product
      Vector unitLeftToRight =
        normalized( makeVector3d( readControl(CTRL_RHAND_LOC_X),
                                  0.0, readControl(CTRL_RHAND_LOC_Z)) -
                    makeVector3d( readControl(CTRL_LHAND_LOC_X),
                                  0.0, readControl(CTRL_LHAND_LOC_Z )) );

      Vector unitHeadFwd =
        normalized( makeVector3d( readControl(CTRL_HEAD_BASIS_XX),
                                  0.0,
                                  readControl(CTRL_HEAD_BASIS_XZ )));
      
      
      float innerP = inner_prod( unitLeftToRight,unitHeadFwd);

#ifdef DEBUG_GESTURES
      cout << "MotionHandler::prepare(): head angle calc complete, innerP: "
           << innerP << "\n";
#endif    

      // rotate avatar, if head orientation differs enough from hand alignment;
      // inner product of 0.3 corresponds an angle of 18 degrees
      if( innerP < -0.3 || innerP > 0.3 )
        avatar->getLocator()->addForceRel
          ( makeVector3d( 0,
                          ( otherMovement? 0.0:0.5 * gravity * avatarMass),
                          0));
      avatar->getLocator()->addTorqueRel
        (makeVector3d(0.0,-innerP * rotTorqueFactor,0.0));
    }
  
  otherMovement = false;
}

void MotionHandler::localStep()
{}

void MotionHandler::prepare( real dt_ )
{
  localPrepare( dt_ );
  Actor::prepare( dt_ );
  Object::prepare( dt_ );
}

void MotionHandler::step()
{
  localStep();
  Actor::step();
  Object::step();
}
