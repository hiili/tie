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
#include <tie/tie.hpp>
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <lifespace/plugins/glow.hpp>
using namespace lifespace::plugins::pglow;

#include <lifespace/plugins/fluid.hpp>
using namespace lifespace::plugins::pfluid;

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


TestWorld::TestWorld( EventHost<GraphicsEvent> & graphicsEventHost,
                      FluidHost * inputHost )
{
  // create a fluid controlled avatar and add a visual to it
  Avatar * fluidAvatar = new Avatar
    ( Object::Params
      ( new ODELocator( makeVector3d(), BasisMatrix(), 100, 10),
        new BasicVisual( shapes::Cube::create
                         ( makeVector3d( 3.0, 0.1, 3.0 )),
                         &whiteMat ),
        new BasicGeometry( shapes::Cube::create
                           ( makeVector3d( 3.0, 0.1, 3.0 )),
                           TIEMat::testCollMat1 )));
  
  fluidAvatar->init( inputHost, this );
  
  addObject( fluidAvatar );
  
  /* init user interface */
  UserInterface * ui = new UserInterface( fluidAvatar, inputHost );
  addObject( ui );


  // create the actual camera and make it follow the camObject
  shared_ptr<Camera> camera1( new Camera() );
  camera1->setTargetObject( fluidAvatar );
  
  // create a keymap for the camera
  shared_ptr<Controller::ControlMap> camKeymap1
    ( new Controller::ControlMap );
  
  // combine the camera, actor and the keymap and add the result to cameras
  CameraEntry cam1 = { camera1, 0, fluidAvatar, camKeymap1, CAM_MODE_FOLLOW };
  cameras.push_back( cam1 );


  /* -------- create some cameras -------- */

  
  
  
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
  
  // create the collision test sphere
  Object * collSphere = new Object
    ( Object::Params( new ODELocator( makeVector3d( 0.0, 3.2, 3.0 )),
                      new BasicVisual( shapes::Sphere::create(1.5),
                                       &TIEMat::whiteMat ),
                      new BasicGeometry( shapes::Sphere::create( 1.5 ),
                                         TIEMat::ballCollMat ) ));
  addObject( collSphere );

  // create the collision test sphere
  Object * collSphere1 = new Object
    ( Object::Params( new ODELocator( makeVector3d( 2.0, 3.2, -1.99 )),
                      new BasicVisual( shapes::Sphere::create(1.5),
                                       &TIEMat::whiteMat ),
                      new BasicGeometry( shapes::Sphere::create( 1.5 ),
                                         TIEMat::ballCollMat ) ));
  addObject( collSphere1 );

  // create the collision test sphere
  Object * collSphere11 = new Object
    ( Object::Params( new ODELocator( makeVector3d( -2.0, 3.2, -1.99 )),
                      new BasicVisual( shapes::Sphere::create(1.5),
                                       &TIEMat::whiteMat ),
                      new BasicGeometry( shapes::Sphere::create( 1.5 ),
                                         TIEMat::ballCollMat ) ));

  addObject( collSphere11 );
  // create the collision test cube
  Object * collSphere2 = new Object
    ( Object::Params( new ODELocator( makeVector3d( -2.0, 0.99, -1.99 )),
                      new BasicVisual( shapes::Cube::create(makeVector3d
                                                            (1.0, 1.0, 1.0)),
                                       &TIEMat::blackMat ),
                      new BasicGeometry( shapes::Cube::create(),
                                         TIEMat::ballCollMat ) ));
  addObject( collSphere2 );

  // create the collision test sphere
  Object * collSphere3 = new Object
    ( Object::Params( new ODELocator( makeVector3d( -1.99, 0.99, 0.0 )),
                      new BasicVisual( shapes::Cube::create(makeVector3d
                                                            (1.0, 1.0, 1.0)),
                                       &TIEMat::blackMat ),
                      new BasicGeometry( shapes::Cube::create(makeVector3d
                                                              (1.0, 1.0, 1.0)),
                                         TIEMat::ballCollMat ) ));
  addObject( collSphere3 );

  
  // create the collision test sphere
  Object * collSphere31 = new Object
    ( Object::Params( new ODELocator( makeVector3d( 0.0, 1.05, -2.0 )),
                      new BasicVisual( shapes::Cube::create(),
                                       &TIEMat::blackMat ),
                      new BasicGeometry( shapes::Cube::create(),
                                         TIEMat::ballCollMat ) ));
  addObject( collSphere31 );

  // ground object
  Object * ground =
    new Object( Object::Params
                ( new BasicLocator( makeVector3d( 0.0, -0.25, 0.0 ), 
                                    BasisMatrix()),
                  new BasicVisual( shapes::Cube::create
                                   ( makeVector3d( 30.0, 0.5, 30.0 ) ),
                                   &TIEMat::whiteMat ),
                  new BasicGeometry( shapes::Cube::create
                                     ( makeVector3d( 30.0, 0.5, 30.0 ) ),
                                     TIEMat::testCollMat1 )));
  addObject( ground );
  

  // wall objects
  Object * wallR =
    new Object( Object::Params
                ( new BasicLocator( makeVector3d( 2.0, 2.0, 0.0 )),
                  new BasicVisual( shapes::Cube::create
                                   ( makeVector3d( 0.2, 8.0, 8.0 )),
                                   &TIEMat::blackMat ),
                  new BasicGeometry( shapes::Cube::create
                                     ( makeVector3d( 0.2, 8.0, 8.0 )),
                                     TIEMat::ballCollMat )));

  Object * wallL =
    new Object( Object::Params
                ( new BasicLocator( makeVector3d( -2.0, 2.0, 0.0 )),
                  new BasicVisual( shapes::Cube::create
                                   ( makeVector3d( 0.2, 8.0, 8.0 )),
                                   &TIEMat::blackMat ),
                  new BasicGeometry( shapes::Cube::create
                                     ( makeVector3d( 0.2, 8.0, 8.0 )),
                                     TIEMat::ballCollMat )));

  addObject( wallL );
  addObject( wallR );

  setGravityVector( makeVector3d( 0.0, -9.81, 0.0 ) );
  
  
  
  /* -------- finalize -------- */
  
  
  // activate the ODE world
  activate( true );
  
  // create and activate the collision detector
  collisionRenderer = new ODECollisionRenderer( this );
  collisionRenderer->connect();
  graphicsEventHost.addListener( collisionRenderer );
  
  // connect all connectors
  fluidAvatar->connect();

  setERP( 0.5 );
  setCFM( 0.00001 );
}


TestWorld::~TestWorld()
{
}
