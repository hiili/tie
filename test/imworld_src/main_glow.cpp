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
 * @file main_glow.cpp
 *
 * Initialization stuff for running the test through GLOW.
 */


/* Include Lifespace and plugins. */

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <lifespace/plugins/glow.hpp>
using namespace lifespace::plugins::pglow;

#include <tie/tie.hpp>

/* Include test-specific files. */

#include "IMWorld.hpp"
using namespace tie;


/* Other includes. */

#include <ode/ode.h>
#include <ode/odecpp.h>

#include <GL/gl.h>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;


/* Check that the current ODE is erp-patched and the patch version is ok. */
#ifndef ODE_VERSION_ERPPATCH
#error This program requires an erp-patched version of ODE!
#else
#if ODE_VERSION_ERPPATCH < 1
#error The ODEs erp-patch version must be >= 1!
#endif
#endif








/* ---- global stuff ---- */


/* This function fills in the keymap of the camera actor object. Modify this to
   change the keyboard controls and mouse sensitivity. */
static void fillCameraKeymap( Controller::ControlMap & keymap )
{
  // define the keyboard controls (last argument is sensitivity)
  keymap['a'] = Controller::MapTarget( FloatingActor::CTRL_FORCEREL_X_NEG,
                                       2.7 );
  keymap['d'] = Controller::MapTarget( FloatingActor::CTRL_FORCEREL_X_POS,
                                       2.7 );
  keymap['s'] = Controller::MapTarget( FloatingActor::CTRL_FORCEREL_Z_POS,
                                       2.7 );
  keymap['w'] = Controller::MapTarget( FloatingActor::CTRL_FORCEREL_Z_NEG,
                                       3.2 );
  keymap[' '] = Controller::MapTarget( FloatingActor::CTRL_FORCEREL_Y_POS,
                                       8.0 );
  keymap['<'] = Controller::MapTarget( FloatingActor::CTRL_FORCEREL_Y_NEG,
                                       2.7 );
  
  // define the mouse controls. the last argument controls the sensitivity.
  keymap[GLOWViewport::MOUSE_DX] =
    Controller::MapTarget( FloatingActor::CTRL_TORQUEREL_Y_NEG, 0.03 );
  keymap[GLOWViewport::MOUSE_DY] =
    Controller::MapTarget( FloatingActor::CTRL_TORQUEREL_X_NEG, 0.03 );
}




static const GLfloat none[4]     = { 0.0, 0.0, 0.0, 0.0 };
static const GLfloat white[4]    = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat smooth[1]   = { 1.0 };
static const GLfloat polished[1] = { 40.0 };
static const Material whiteMat( white, white, white,
                                none, smooth, GL_FRONT );
static const GLfloat camLightAtt[3] = { 1.0, 0.0, 0.001 };


static void addGLOWCamera( GLOWViewport & viewport, World & world )
{
  // create an actor object which the user can move
  FloatingActor * camObject = new FloatingActor(
	Object::Params( new ODELocator( makeVector3d(0.0, 1.5,9.0), BasisMatrix(), 4.0, 3.0),
                   	0,
                  	new BasicGeometry
                      	(sptr (new shapes::Cube),
                      	sptr (new CollisionMaterial (0.01, 0.9, 0.0 )))));
  camObject->setAutoRoll( false );   // use this to disable camera auto-roll
  world.addObject( camObject );
  
  // connect the input feed (keyboard, mouse, ...) from the viewport to the
  // object
  Controller::ControlMap * camKeymap = new Controller::ControlMap;
  fillCameraKeymap( *camKeymap );
  viewport.addActor( camObject, camKeymap );
  
  // create the actual camera, make it follow the camObject and forward its
  // video feed into the viewport.
  Camera * camera = new Camera();
  camera->setTargetObject( camObject );
  viewport.setCamera( camera );
  
  // attach a light source to the camera
  Light * light = new Light( &whiteMat, camObject, camLightAtt );
  world.getEnvironment()->addLight( light );
}


/* ---- main ---- */


int main( int argc, char * argv[] )
{
  /* Init graphics. */
  GLOWDevice::Init( argc, argv );
  GLOWDevice window;
  GLOWViewport viewport( window );
  
    
  /* Create the world. */
  IMWorld world( window.events );


  //Gravity
  world.setGravityVector( makeVector3d( 0.0, -1.5, 0.0 ) );
  
  /* Add a GLOW-controlled camera (with a light source) into the world. */
  addGLOWCamera( viewport, world );

  // activate the ODE world
  world.activate( true );
  
  

    // create and activate the collision detector
  world.collisionRenderer = new ODECollisionRenderer( &world );
  world.collisionRenderer->connect();
  window.events.addListener( world.collisionRenderer );
  world.setERP(0.5);
  world.setCFM(0.01);
  
  //Connect stuff
  
  world.wingConnector->connect(*world.windmillConnector, Connector::AlignMaster);
  
  world.batConnector->connect(*world.batterBaseConnector, Connector::AlignMaster);
  
   //Make it spin

  world.wingConnector->useControl(ODEAxleConnector::CTRL_MOTOR_FORCEFACTOR, 1.0);
  world.wingConnector->useControl(ODEAxleConnector::CTRL_MOTOR_TARGETVEL, 0.2);
  
  world.batConnector->useControl(ODEAxleConnector::CTRL_MOTOR_FORCEFACTOR, 1.0);
  world.batConnector->useControl(ODEAxleConnector::CTRL_MOTOR_TARGETVEL, 2.7);
 
  /* Start the system. */
  world.setDefaultDt( 0.05 );
  window.events.addListener( &world );
  
  GLOWDevice::MainLoop();
  
  /* This is never reached when using GLOW. */
  return 0;
}
