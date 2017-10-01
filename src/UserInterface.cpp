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
 * @file UserInterface.cpp
 *
 * Implementation for the UserInterface object
 */

#include "UserInterface.hpp"
#include "MotionHandler.hpp"
#include "GestureHandler.hpp"
#include "SensorController.hpp"
#include "types.hpp"
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <lifespace/plugins/fluid.hpp>
using namespace lifespace::plugins::pfluid;

#include <string>

// Fill head keymap for MotionHandler
void UserInterface::fillFluidHeadKeymap( Controller::ControlMap & keymap )
{
  keymap[FluidController::CTRL_VECTOR_DIM0] =
    Controller::MapTarget( MotionHandler::CTRL_HEAD_LOC_X,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM1] =
    Controller::MapTarget( MotionHandler::CTRL_HEAD_LOC_Y,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM2] =
    Controller::MapTarget( MotionHandler::CTRL_HEAD_LOC_Z,   1.0 );
  keymap[FluidController::CTRL_MATRIX_0_0] =
    Controller::MapTarget( MotionHandler::CTRL_HEAD_BASIS_XX,   1.0 );
  keymap[FluidController::CTRL_MATRIX_0_2] =
    Controller::MapTarget( MotionHandler::CTRL_HEAD_BASIS_XZ,   1.0 );
  keymap[FluidController::CTRL_MATRIX_2_0] =
    Controller::MapTarget( MotionHandler::CTRL_HEAD_BASIS_ZX,   1.0 );
  keymap[FluidController::CTRL_MATRIX_2_2] =
    Controller::MapTarget( MotionHandler::CTRL_HEAD_BASIS_ZZ,   1.0 );
}

// Fill head keymap for GestureHandler
void UserInterface::fillFluidHeadKeymapGest( Controller::ControlMap & keymap )
{
  keymap[FluidController::CTRL_VECTOR_DIM0] =
    Controller::MapTarget( GestureHandler::CTRL_HEAD_LOC_X,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM1] =
    Controller::MapTarget( GestureHandler::CTRL_HEAD_LOC_Y,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM2] =
    Controller::MapTarget( GestureHandler::CTRL_HEAD_LOC_Z,   1.0 );
}

// Fill right hand keymap for MotionHandler
void UserInterface::fillFluidRightHandKeymap( Controller::ControlMap & keymap )
{
  keymap[FluidController::CTRL_VECTOR_DIM0] =
    Controller::MapTarget( MotionHandler::CTRL_RHAND_LOC_X,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM1] =
    Controller::MapTarget( MotionHandler::CTRL_RHAND_LOC_Y,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM2] =
    Controller::MapTarget( MotionHandler::CTRL_RHAND_LOC_Z,   1.0 );
}

// Fill right hand keymap for GestureHandler
void UserInterface::fillFluidRightHandKeymapGest( Controller::ControlMap & keymap )
{
  keymap[FluidController::CTRL_VECTOR_DIM0] =
    Controller::MapTarget( GestureHandler::CTRL_RHAND_LOC_X,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM1] =
    Controller::MapTarget( GestureHandler::CTRL_RHAND_LOC_Y,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM2] =
    Controller::MapTarget( GestureHandler::CTRL_RHAND_LOC_Z,   1.0 );
}

// Fill left hand keymap for MotionHandler
void UserInterface::fillFluidLeftHandKeymap( Controller::ControlMap & keymap )
{
  keymap[FluidController::CTRL_VECTOR_DIM0] =
    Controller::MapTarget( MotionHandler::CTRL_LHAND_LOC_X,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM1] =
    Controller::MapTarget( MotionHandler::CTRL_LHAND_LOC_Y,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM2] =
    Controller::MapTarget( MotionHandler::CTRL_LHAND_LOC_Z,   1.0 );
}

// Fill left hand keymap for GestureHandler
void UserInterface::fillFluidLeftHandKeymapGest( Controller::ControlMap & keymap )
{
  keymap[FluidController::CTRL_VECTOR_DIM0] =
    Controller::MapTarget( GestureHandler::CTRL_LHAND_LOC_X,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM1] =
    Controller::MapTarget( GestureHandler::CTRL_LHAND_LOC_Y,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM2] =
    Controller::MapTarget( GestureHandler::CTRL_LHAND_LOC_Z,   1.0 );
}


void UserInterface::init( lifespace::plugins::pfluid::FluidHost * inputHost,
                          float gravity,
                          float avatarMass )
{
  // create fluid controlled objects
  SensorController * head =
    new SensorController( std::string( "head" ) );
  SensorController * leftHand =
    new SensorController( std::string( "lefthand" ) );
  SensorController * rightHand =
    new SensorController( std::string( "righthand" ) );

  // set correct translation for head processor
  head->setLocTranslationRel( makeVector3d( -0.25, -0.35, 0.25 ));
  // set correct translations and gesture thresholds for both hands
  leftHand->setLocTranslationRel( makeVector3d( 0.0, 0.0, 0.15 ) );
  rightHand->setLocTranslationRel( makeVector3d( 0.0, 0.0, 0.15 ) );


  // create actors
  MotionHandler * motionHandler = 
    new MotionHandler(
		      avatar,            // pointer to the user avatar
		      EVE_CUBE_X_MIN,    // cubeXmin
		      EVE_CUBE_X_MAX,    // cubeXmax
		      EVE_CUBE_Z_MIN,    // cubeZmin
		      EVE_CUBE_Z_MAX,    // cubeZmax
		      0.37 * (EVE_CUBE_Z_MAX - EVE_CUBE_Z_MIN),  // moveZoneWidth
		      0.9 * gravity * avatarMass,             // moveForceFactor
                      3.0 * gravity * avatarMass,            // rotation max torque  
		      1.5,               // user shoulder width
		      0.75,              // gesture error tolerance
                      gravity,           // world gravity
                      avatarMass );      // avatar object mass

  GestureHandler * gestureHandler = 
    new GestureHandler( avatar );

  // fill the actors' controls
  Controller::ControlMap * headSensorControlmap = new Controller::ControlMap();
  fillFluidHeadKeymap( *headSensorControlmap );

  Controller::ControlMap * headSensorControlmapGest = new Controller::ControlMap();
  fillFluidHeadKeymapGest( *headSensorControlmapGest );

  Controller::ControlMap * rightHandSensorControlmap = new Controller::ControlMap();
  fillFluidRightHandKeymap( *rightHandSensorControlmap );

  Controller::ControlMap * rightHandSensorControlmapGest = new Controller::ControlMap();
  fillFluidRightHandKeymapGest( *rightHandSensorControlmapGest );

  Controller::ControlMap * leftHandSensorControlmap = new Controller::ControlMap();
  fillFluidLeftHandKeymap( *leftHandSensorControlmap );

  Controller::ControlMap * leftHandSensorControlmapGest = new Controller::ControlMap();
  fillFluidLeftHandKeymapGest( *leftHandSensorControlmapGest );

  // add actors to inputhost
  head->addActor( motionHandler, headSensorControlmap );
  head->addActor( gestureHandler, headSensorControlmapGest );
  inputHost->addController( head );

  rightHand->addActor( motionHandler, rightHandSensorControlmap );
  rightHand->addActor( gestureHandler, rightHandSensorControlmapGest );
  inputHost->addController( rightHand );
  
  leftHand->addActor( motionHandler, leftHandSensorControlmap );
  leftHand->addActor( gestureHandler, leftHandSensorControlmapGest );
  inputHost->addController( leftHand );
  
  // add actors to the UserInterface subspace
  addObject( motionHandler );
  addObject( gestureHandler );
}

UserInterface::UserInterface( tie::Avatar * avatar_,
                              lifespace::plugins::pfluid::FluidHost * inputHost_,
                              float gravity_,
                              float avatarMass_ ):
  lifespace::Subspace(),
  avatar( avatar_ )
{
  init( inputHost_, gravity_, avatarMass_ );
}
