/*
 * Copyright (C) 2004-2005 Eero Uusitalo, Ville Heikkil�, Mikko Ylikangas, Paul
 * Wagner, Juho Heiskari, Kati M��tt�, Mikko Purtonen.
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
static const GLfloat blue[4]    = { 0.0, 0.0, 1.0, 1.0 };
static const GLfloat smooth[1]   = { 1.0 };
static const GLfloat polished[1] = { 40.0 };
static const Material whiteMat( none, white, white,
                                none, smooth, GL_FRONT );
static const Material blueMat( none, blue, blue,
                                none, smooth, GL_FRONT );


TestWorld::TestWorld( EventHost<GraphicsEvent> & graphicsEventHost )
{
  
  /* -------- create world content -------- */
  
  // create the light object
  Object * lightObj = new Object
    ( Object::Params( new ODELocator( makeVector3d( 2.0, 4.0, -3.0 )),
                      new BasicVisual( sptr( new shapes::Sphere ),
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
                      new BasicVisual( sptr( new shapes::Basis ),
                                       &whiteMat ),
                      0 ));
  addObject( origin );

/*
  Object * testObj = new Object
    ( Object::Params( new ODELocator( makeVector3d( 5.0, 5.0, -3.0 )),
                      new BasicVisual( sptr( new shapes::Cube ),
                                       &blueMat ),
                      0 ));
  addObject( testObj );
*/

//  Cylinder * cylObj = new Cylinder
  Object * cylObj = new Object
    ( Object::Params( new ODELocator( makeVector3d( -2.0, 0.0, -3.0 )),
                      new BasicVisual( sptr( new CylinderVisual(2.0, 2.0, 2.0, 100)),
                                       &blueMat ),
//                      0 ));
                      new BasicGeometry (sptr (new shapes::Cube
                      							(makeVector3d(0.75, 3.0, 0.75))),
                      					 sptr (new CollisionMaterial 
                      					 (2.0, 0.9, 0.0 )))));
                      	
                      	
  addObject( cylObj );
	//Actor::Controlmap keymap; fillCylinderKeymap( keymap );
  //vp.addActor( cylObj, &keymap );
  
  
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
