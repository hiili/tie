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

#include <ode/ode.h>
#include <ode/odecpp.h>

#include <tie/tie.hpp>

#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

static const GLfloat none[4]     = { 0.0, 0.0, 0.0, 0.0 };
static const GLfloat white[4]    = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat red[4]	= { 1.0, 0.0, 0.0, 1.0};
static const GLfloat blue[4]	= { 0.0, 0.0, 1.0, 1.0};
static const GLfloat red05[4]	= { 0.5, 0.0, 0.0, 1.0};
static const GLfloat green2[4]      = { 0.0, 0.2, 0.0, 1.0 };
static const GLfloat green4[4]      = { 0.0, 0.4, 0.0, 1.0 };
static const GLfloat green6[4]      = { 0.0, 0.6, 0.0, 1.0 };
static const GLfloat green8[4]      = { 0.0, 0.8, 0.0, 1.0 };
static const GLfloat green[4]      = { 0.0, 1.0, 0.0, 1.0 };

static const GLfloat smooth[1]   = { 1.0 };
static const GLfloat polished[1] = { 40.0 };
static const Material blueMat( red05, blue, blue, red05, smooth, GL_FRONT);
static const Material whiteMat( none, white, white,
                                none, smooth, GL_FRONT );
static const Material redMat( none, red, red05,
                                none, smooth, GL_FRONT );
static const Material groundMat( green2, green8, none,
                                  none, smooth, GL_FRONT );                                



TestWorld::TestWorld( EventHost<GraphicsEvent> & graphicsEventHost )
{
  
  /* -------- create world content -------- */
  
  // create the light object
  Object * lightObj = new Object
    ( Object::Params( new ODELocator( makeVector3d( 2.0, 2.0, -3.0 )),
                      new BasicVisual( sptr( new shapes::Sphere ),
                                       &whiteMat ),
                      new BasicGeometry
                      	(sptr (new shapes::Sphere(1.0)),
                      	sptr (new CollisionMaterial (2.0, 0.9, 0.0 )))));
  addObject( lightObj );
  
  // create the actual light
  GLfloat * lightAtt = new GLfloat[3];
  lightAtt[0] = 1.0; lightAtt[1] = 0.0; lightAtt[2] = 0.01;
  Light * light = new Light( &whiteMat, lightObj, lightAtt );
  getEnvironment()->addLight( light );
  
   // create a new light object
  Object * lightObj5 = new Object
    ( Object::Params( new BasicLocator( makeVector3d( 2.0, 15.0, -3.0 )),
                      new BasicVisual( sptr( new shapes::Sphere ),
                                       &blueMat ),
                      new BasicGeometry
                      	(sptr (new shapes::Sphere(1.0)),
                      	sptr (new CollisionMaterial (2.0, 0.9, 0.0 )))));
  addObject( lightObj5 );
  
  // create the actual light
  GLfloat * lightAtt5 = new GLfloat[3];
  lightAtt5[0] = 3.0; lightAtt5[1] = 0.0; lightAtt5[2] = 0.01;
  Light * light5 = new Light( &whiteMat, lightObj5, lightAtt );
  getEnvironment()->addLight( light5 );
 
 
  // create the origin object
  Object * origin = 
    new Object
    ( Object::Params( new ODELocator(),
                      new BasicVisual( sptr( new shapes::Basis ),
                                       &whiteMat ),
                      new BasicGeometry
                      	(sptr (new shapes::Basis),
                      	sptr (new CollisionMaterial (2.0,0.9,0.0 )))));
  addObject( origin );
  
  Object * redBall = new Object
    ( Object::Params( new ODELocator( makeVector3d( -1.5, 2.0, -3.0 )),
                      new BasicVisual( sptr( new shapes::Sphere( 1.0 ) ),
                                       &redMat ),
                      new BasicGeometry
                      	(sptr (new shapes::Sphere),
                      	 sptr (new CollisionMaterial (2.0, 0.9, 0.0 )))));
  addObject( redBall );
  
    // create another light
  Light * light2 = new Light( &blueMat, redBall, lightAtt );
  getEnvironment()->addLight( light2 );
  
  // Interesting avatar
  Object * avatarCube = new Object
    ( Object::Params( new ODELocator( makeVector3d( -3.5, 2.0, 3.0 )),
                      new BasicVisual
                      (sptr(new shapes::Union
                      (sptr(new shapes::Union
                      	  	(sptr( new shapes::Located
                      	  		( BasicLocator( makeVector3d( 0.0, 0.0, 0.0 ) ),
                      			sptr (new shapes::Scaled
                      				(makeVector3d(4.0, 3.0, 2.0), 
                      				sptr (new shapes::Cube
                      					(makeVector3d(1.0, 2.9, 0.5))))))),
                          	sptr (new shapes::Union 
                          		(sptr (new shapes::Located
                          			(BasicLocator(makeVector3d(-0.92, 2.6, 1.0)),
                          			 sptr (new shapes::Sphere))),
                         		(sptr (new shapes::Located
                         			(BasicLocator(makeVector3d(-0.92, 2.6, 2.0)),
                         			sptr (new shapes::Sphere(0.2))))))),
                          	sptr (new shapes::Union
                          		(sptr (new shapes::Located
                          			(BasicLocator(makeVector3d(0.92, 2.6, 1.0)),
                          			sptr (new shapes::Sphere))),
                          		sptr (new shapes::Located
                          			(BasicLocator(makeVector3d(0.92, 2.6, 2.0)),
                          			sptr (new shapes::Sphere(0.2))))))
                          )),
                       sptr (new shapes::Located
                       	   (BasicLocator(makeVector3d(0.0, 5.5, 0.0)),
                         		sptr (new shapes::Sphere(1.6))))
                       )),
                     &blueMat),
                       new BasicGeometry
                      	 (sptr(new shapes::Union
                      (sptr(new shapes::Union
                      	  	(sptr( new shapes::Located
                      	  		( BasicLocator( makeVector3d( 0.0, 0.0, 0.0 ) ),
                      			sptr (new shapes::Scaled
                      				(makeVector3d(4.0, 3.0, 2.0), 
                      				sptr (new shapes::Cube
                      					(makeVector3d(1.0, 2.9, 0.5))))))),
                          	sptr (new shapes::Union 
                          		(sptr (new shapes::Located
                          			(BasicLocator(makeVector3d(-0.92, 2.6, 1.0)),
                          			 sptr (new shapes::Sphere))),
                         		(sptr (new shapes::Located
                         			(BasicLocator(makeVector3d(-0.92, 2.6, 2.0)),
                         			sptr (new shapes::Sphere(0.2))))))),
                          	sptr (new shapes::Union
                          		(sptr (new shapes::Located
                          			(BasicLocator(makeVector3d(0.92, 2.6, 1.0)),
                          			sptr (new shapes::Sphere))),
                          		sptr (new shapes::Located
                          			(BasicLocator(makeVector3d(0.92, 2.6, 2.0)),
                          			sptr (new shapes::Sphere(0.2))))))
                          )),
                       sptr (new shapes::Located
                       	   (BasicLocator(makeVector3d(0.0, 5.5, 0.0)),
                         		sptr (new shapes::Sphere(1.6))))
                       )),
                       sptr (new CollisionMaterial (2.0, 0.9, 0.0 )))
              ));
  
  
  addObject (avatarCube);
   
  //Add a bowling alley
  
  tie::BowlingAlley * alley = new tie::BowlingAlley (makeVector3d(7.0, 0.0, 0.0));
  addObject(alley);
  
  //Create the ground
  
  Object * ground = new Object
    ( Object::Params( new BasicLocator( makeVector3d( 10.0, -3.0, 10.0 )),
                      new BasicVisual( sptr( new shapes::Cube
                      				(makeVector3d(40.0,0.5,40.0) ) ),
                                       &groundMat ),
                      new BasicGeometry
                      	(sptr (new shapes::Cube
                      		(makeVector3d(40.0,0.5,40.0))),
                      	 sptr (new CollisionMaterial (3.0, 0.9, 0.0 )))));
  addObject(ground);
  
  //One wall please
  
  Object * wall1 = new Object
    ( Object::Params( new BasicLocator( makeVector3d( 10.0, 2.0, -10.0 )),
                      new BasicVisual( sptr( new shapes::Cube
                      				(makeVector3d(40.0,10.0,0.5) ) ),
                                       &groundMat ),
                      new BasicGeometry
                      	(sptr (new shapes::Cube
                      		(makeVector3d(40.0,10.0,0.5))),
                      	 sptr (new CollisionMaterial (2.0, 0.9, 0.0 )))));
  addObject(wall1);
  
    // Create a spawner
  addObject( new CubeSpawner
      ( 1.0, 10.0, makeVector3d( 8.0, 0.0, -3.0 ), this, 1, false ));
  
  // Create another spawner
   
  addObject( new CubeSpawner
    ( 1.0, 10.0, makeVector3d( 4.0, 0.0, -3.0 ), this, 1, false ));

  // Create another spawner
   
  addObject( new CubeSpawner
    ( 2.0, 10.0, makeVector3d( 4.0, -2.5, 6.0 ), this, 10 ));  
  
  // Create a BVD spawner
  addObject( new BVDSpawner
    ( 2.0, 10.0, makeVector3d( -2.0, -2.5, -5.0 ), this, 20 ));
  
  // Create a seesaw
  addObject( new Seesaw
    ( makeVector3d( 6.0, -2.0, -3.0 ), 5.0,
      makeVector3d( 0.0, 0.0, 1.0 ) ));
 
  
  /* -------- finalize -------- */

  // connect all connectors

}


TestWorld::~TestWorld()
{
}
