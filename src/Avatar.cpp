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
 * @file Avatar.cpp
 *
 * Avatar is the base class for user representation in TIE structure.
 */

#include "Avatar.hpp"
#include "Head.hpp"
#include "ODEHand.hpp"
#include "SensorController.hpp"
#include "GloveController.hpp"
#include "FluidSkeleton.hpp"
#include "ODESkeleton.hpp"
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <lifespace/plugins/fluid.hpp>
using namespace lifespace::plugins::pfluid;

#include <GL/gl.h>

#include <string>
#include <iostream>
using std::cout;
using std::endl;

void 
Avatar::fillFluidKeymap( Controller::ControlMap & keymap )
{
  keymap[FluidController::CTRL_VECTOR_DIM0] =
    Controller::MapTarget( FloatingActor::CTRL_LOC_X,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM1] =
    Controller::MapTarget( FloatingActor::CTRL_LOC_Y,   1.0 );
  keymap[FluidController::CTRL_VECTOR_DIM2] =
    Controller::MapTarget( FloatingActor::CTRL_LOC_Z,   1.0 );
  keymap[FluidController::CTRL_MATRIX_0_0] =
    Controller::MapTarget( FloatingActor::CTRL_BASIS_XX, 1.0 );
  keymap[FluidController::CTRL_MATRIX_0_1] =
    Controller::MapTarget( FloatingActor::CTRL_BASIS_XY, 1.0 );
  keymap[FluidController::CTRL_MATRIX_0_2] =
    Controller::MapTarget( FloatingActor::CTRL_BASIS_XZ, 1.0 );
  keymap[FluidController::CTRL_MATRIX_1_0] =
    Controller::MapTarget( FloatingActor::CTRL_BASIS_YX, 1.0 );
  keymap[FluidController::CTRL_MATRIX_1_1] =
    Controller::MapTarget( FloatingActor::CTRL_BASIS_YY, 1.0 );
  keymap[FluidController::CTRL_MATRIX_1_2] =
    Controller::MapTarget( FloatingActor::CTRL_BASIS_YZ, 1.0 );
  keymap[FluidController::CTRL_MATRIX_2_0] =
    Controller::MapTarget( FloatingActor::CTRL_BASIS_ZX, 1.0 );
  keymap[FluidController::CTRL_MATRIX_2_1] =
    Controller::MapTarget( FloatingActor::CTRL_BASIS_ZY, 1.0 );
  keymap[FluidController::CTRL_MATRIX_2_2] =
    Controller::MapTarget( FloatingActor::CTRL_BASIS_ZZ, 1.0 );
  
  keymap[SensorController::CTRL_SETLOC] =
    Controller::MapTarget( FloatingActor::CTRL_SETLOC, 1.0   );
  keymap[SensorController::CTRL_SETBASIS] =
    Controller::MapTarget( FloatingActor::CTRL_SETBASIS, 1.0   );
}

void 
Avatar::fillFlexionKeymap( Controller::ControlMap & keymap )
{  
  keymap[GloveController::CTRL_SET_FIST] =
    Controller::MapTarget( ODEHand::CTRL_IS_FISTED, 1.0   );
  keymap[GloveController::CTRL_SET_FORE] =
    Controller::MapTarget( ODEHand::CTRL_FLEXION_INDEX, 1.0   );
  keymap[GloveController::CTRL_SET_MIDDLE] =
    Controller::MapTarget( ODEHand::CTRL_FLEXION_MIDDLE, 1.0   );
  keymap[GloveController::CTRL_SET_INDEX] =
    Controller::MapTarget( ODEHand::CTRL_FLEXION_RING, 1.0   );
  keymap[GloveController::CTRL_SET_PINKY] =
    Controller::MapTarget( ODEHand::CTRL_FLEXION_PINKY, 1.0   );
}



void
Avatar::init( FluidHost * inputHost, ODEWorld * world )
{
  // create connectors for this avatar
  connectors[FluidSkeleton::CONN_HEAD_BASE] =
    sptr( new ODEFixedConnector
          ( Connector
            ( (Subspace &)*this,  Connector::Any, BasicLocator() )));
  connectors[FluidSkeleton::CONN_LEFT_HAND] =
    sptr( new ODEFixedConnector
          ( Connector
            ( (Subspace &)*this,  Connector::Any, BasicLocator() )));
  connectors[FluidSkeleton::CONN_RIGHT_HAND] =
    sptr( new ODEFixedConnector
          ( Connector
            ( (Subspace &)*this, Connector::Any, BasicLocator() )));

  // create fluid controlled objects
  head =
    new SensorController( std::string( "head" ) );
  leftHand =
    new SensorController( std::string( "lefthand" ) );
  rightHand =
    new SensorController( std::string( "righthand" ) );
  leftGlove =
    new GloveController( std::string( "leftHandGlove" ) );
  rightGlove =
    new GloveController( std::string( "rightHandGlove" ) );
  inputHost->addController( rightGlove );

  cout << "Processors created - creating Actors\n";

  // create actors and fill their controls
  FloatingActor * headActor = 
    new FloatingActor
    ( Object::Params
      ( new InertiaLocator(makeVector3d( 0.0, 0.0, 0.0), BasisMatrix()),
        0, 0 ) );
  headActor->setAutoRoll( false );

  FloatingActor * leftHandActor =
    new FloatingActor( Object::Params( new InertiaLocator(), 0, 0 ));
  leftHandActor->setAutoRoll( false );
  
  FloatingActor * rightHandActor =
    new FloatingActor( Object::Params( new InertiaLocator(), 0, 0 ));
  rightHandActor->setAutoRoll( false );

  cout << "Actors Created - filling controls\n";

  // fill the controls
  Controller::ControlMap * sensorControlmap = new Controller::ControlMap();
  fillFluidKeymap( *sensorControlmap );
  
  cout << "Filled controls - adding actors\n";

  // add actors to inputhost
  head->addActor( headActor, sensorControlmap );
  inputHost->addController( head );

  leftHand->addActor( leftHandActor, sensorControlmap );
  inputHost->addController( leftHand );

  rightHand->addActor( rightHandActor, sensorControlmap );
  inputHost->addController( rightHand );

  cout << "Actors added - creating fluidSkeleton\n";

  // create new skeleton model for fluid input avatar
  fluidSkeleton = new FluidSkeleton( (Subspace *)this, headActor,
                                     leftHandActor, rightHandActor );
  
  cout << "FluidSkeleton created - creating ode objects\n";

  // create ode objects
  Head * odeHead = new Head(); odeHead->create( 0.4 );

  // set correct translation for head processor
  head->setLocTranslationRel( makeVector3d( -0.25, -0.35, 0.25 ));

  // create hands
  ODEHand * odeLeftHand = new ODEHand();
  odeLeftHand->create( 0.4, 0.15, ODEHand::LEFT );

  // create the light for left hand
  GLfloat * leftHandLightAtt = new GLfloat[3];
  leftHandLightAtt[0] = 2.0;
  leftHandLightAtt[1] = 0.0;
  leftHandLightAtt[2] = 0.03;
  Light * lightLH = new Light( &TIEMat::whiteMat,
                               odeLeftHand,
                               leftHandLightAtt );
  world->getEnvironment()->addLight( lightLH );

  // set correct translation for left hand
  leftHand->setLocTranslationRel( makeVector3d( 0.0, 0.0, 0.3 ) );
  leftGlove->setThresholds( 3.5, 2.5 );

  ODEHand * odeRightHand = new ODEHand();
  odeRightHand->create( 0.4, 0.2, ODEHand::RIGHT );

  // create the light for right hand
  GLfloat * rightHandLightAtt = new GLfloat[3];
  rightHandLightAtt[0] = 2.0;
  rightHandLightAtt[1] = 0.0;
  rightHandLightAtt[2] = 0.03;
  Light * lightRH = new Light( &TIEMat::whiteMat,
                               odeRightHand,
                               rightHandLightAtt );
  world->getEnvironment()->addLight( lightRH );
  
  // set correct translation for left hand
  rightHand->setLocTranslationRel( makeVector3d( 0.0, 0.0, 0.3 ) );
  rightGlove->setThresholds( 2.5, 1.5 );

  // controlmap for left and right hand glove actors
  Controller::ControlMap * flexionControlmap = new Controller::ControlMap();
  fillFlexionKeymap( *flexionControlmap );

  // add actors for created hands
  leftGlove->addActor( odeLeftHand, flexionControlmap );
  inputHost->addController( leftGlove );

  rightGlove->addActor( odeRightHand, flexionControlmap );
  inputHost->addController( rightGlove );


  cout << "Ode objects created - creating ode skeleton\n";

  // create new physical skeleton model for ODE and lifespace
  odeSkeleton = new ODESkeleton( this,
                                 0,
                                 odeHead,
                                 0,
                                 0,
                                 odeLeftHand,
                                 odeRightHand );

  cout << "Ode skeleton created - initializing fluidskeleton\n";
  
  fluidSkeleton->init( odeSkeleton );
  
  cout << "Avatar constructor finished\n";

}




void
Avatar::prepare( real dt )
{
#ifdef DEBUG_AVATAR
  cout << "Avatar: prepare(): start\n";
#endif

  fluidSkeleton->prepare( dt );
  
#ifdef DEBUG_AVATAR
  cout << "Avatar: prepare(): "
       << "called fluidskeleton::prepare, calling subspace::localprepare\n";
#endif
  // call the both underlying bases' prepare (for subspace: local version)
  Subspace::localPrepare( dt );

#ifdef DEBUG_AVATAR
  cout << "Avatar: prepare(): "
       << "called subspace::localprepare, calling object::localprepare\n";
#endif
  Object::prepare( dt );

#ifdef DEBUG_AVATAR
  cout << "Avatar: prepare(): finished\n";
#endif  
}


void
Avatar::step()
{
#ifdef DEBUG_AVATAR
  cout << "Avatar: step(): start\n";
#endif
  fluidSkeleton->step();
  
  // call the both underlying bases' step (for subspace: local version)
  Subspace::localStep();
  Object::step();

#ifdef DEBUG_AVATAR
  cout << "Avatar: step(): finished\n";
#endif
}
