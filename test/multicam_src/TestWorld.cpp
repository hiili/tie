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
 * @file TestWorld.cpp
 *
 * Implementation for the the world object.
 */

#include "TestWorld.hpp"
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <lifespace/plugins/glow.hpp>
using namespace lifespace::plugins::pglow;

#include <ode/ode.h>
#include <ode/odecpp.h>

#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;




static const GLfloat none[4]     = { 0.0, 0.0, 0.0, 0.0 };
static const GLfloat white[4]    = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat smooth[1]   = { 1.0 };
static const GLfloat polished[1] = { 40.0 };
static const Material whiteMat( none, white, white,
                                none, smooth, GL_FRONT );




/* This function fills in the keymap of the camera actor object. Modify this to
   change the keyboard controls and mouse sensitivity. */
static void fillCameraKeymap( Controller::ControlMap & keymap )
{
  // define the keyboard controls (last argument is sensitivity)
  keymap['a'] = Controller::MapTarget( FloatingActor::CTRL_FORCEREL_X_NEG,
                                       1.0 );
  keymap['d'] = Controller::MapTarget( FloatingActor::CTRL_FORCEREL_X_POS,
                                       1.0 );
  keymap['s'] = Controller::MapTarget( FloatingActor::CTRL_FORCEREL_Z_POS,
                                       1.0 );
  keymap['w'] = Controller::MapTarget( FloatingActor::CTRL_FORCEREL_Z_NEG,
                                       1.0 );
  keymap[' '] = Controller::MapTarget( FloatingActor::CTRL_FORCEREL_Y_POS,
                                       1.0 );
  keymap['<'] = Controller::MapTarget( FloatingActor::CTRL_FORCEREL_Y_NEG,
                                       1.0 );
  
  // define the mouse controls. the last argument controls the sensitivity.
  keymap[GLOWViewport::MOUSE_DX] =
    Controller::MapTarget( FloatingActor::CTRL_TORQUEREL_Y_NEG, 0.02 );
  keymap[GLOWViewport::MOUSE_DY] =
    Controller::MapTarget( FloatingActor::CTRL_TORQUEREL_X_NEG, 0.02 );
}




TestWorld::TestWorld( EventHost<GraphicsEvent> & graphicsEventHost )
{
  
  
  /* -------- create some cameras -------- */
  
  
  // create an actor object which the user can move and add a visual to it
  FloatingActor * camObject1 = new FloatingActor();
  camObject1->getLocator()->setLoc( makeVector3d( 0.3, 0.5, -1.2 ) );
  camObject1->setVisual
    ( sptr( new BasicVisual( shapes::Cube::create
                             ( makeVector3d( 0.1, 0.2, 0.4 )),
                             &whiteMat )));
  //camObject->setAutoRoll( false );   // use this to disable camera auto-roll
  addObject( camObject1 );
  
  // create the actual camera and make it follow the camObject
  shared_ptr<Camera> camera1( new Camera() );
  camera1->setTargetObject( camObject1 );
  
  // create a keymap for the camera
  shared_ptr<Controller::ControlMap> camKeymap1
    ( new Controller::ControlMap );
  fillCameraKeymap( *camKeymap1 );
  
  // combine the camera, actor and the keymap and add the result to cameras
  CameraEntry cam1 = { camera1, camObject1, camKeymap1 };
  cameras.push_back( cam1 );
  
  
  
  
  /* -------- create world content -------- */
  
  
  // create the light object
  Object * lightObj = new Object
    ( Object::Params( new ODELocator( makeVector3d( 2.0, 2.0, -3.0 )),
                      new BasicVisual( shapes::Sphere::create(),
                                       &whiteMat ),
                      0 ));
  addObject( lightObj );
  
  // create the actual light
  GLfloat * lightAtt = new GLfloat[3];
  lightAtt[0] = 0.2; lightAtt[1] = 0.0; lightAtt[2] = 0.01;
  Light * light = new Light( &whiteMat, lightObj, lightAtt );
  getEnvironment()->addLight( light );
  
  // create the origin object
  Object * origin = 
    new Object
    ( Object::Params( new BasicLocator(),
                      new BasicVisual( shapes::Basis::create(),
                                       &whiteMat ),
                      0 ));
  addObject( origin );
  
  
  
  
  /* -------- finalize -------- */
  
  
  // activate the ODE world
  activate( true );
  
  // create and activate the collision detector
  collisionRenderer = new ODECollisionRenderer( this );
  collisionRenderer->connect();
  graphicsEventHost.addListener( collisionRenderer );
  
  // connect all connectors
  
}


TestWorld::~TestWorld()
{
}
