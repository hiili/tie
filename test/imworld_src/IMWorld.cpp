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
 * @file IMWorld.cpp
 *
 * The Improbable Mission demo world for TIE.
 */

#include "IMWorld.hpp"

#include<tie/tie.hpp>
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include "TIEMaterials.hpp"
#include <ode/ode.h>
#include <ode/odecpp.h>

#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

static float WorldFriction = 1.3;
static float WingMass = 3.0;

IMWorld::IMWorld( EventHost<GraphicsEvent> & graphicsEventHost )
{
  
  /* -------- create world content -------- */
  
//Create default collisionmaterial

  CollisionMaterial * defaultCM = 
  		new CollisionMaterial(WorldFriction, 1.0, 0.0);

  //First, generate a floor.
  
  Object * floor1 = new Object
  	(Object::Params
  		(new BasicLocator(makeVector3d(50.0,0.0,50.0)),
  		 new BasicVisual
  		 	(sptr (new shapes::Cube(makeVector3d(100.0,1.0,100.0))),
  		 	&TIEMat::groundMat),
  		 new BasicGeometry
  		 	(sptr (new shapes::Cube(makeVector3d(100.0,1.0,100.0))),
  		 	sptr (defaultCM))));
  addObject(floor1);
  
  
  Object * floor2 = new Object
  	(Object::Params
  		(new BasicLocator(makeVector3d(-50.0,0.0,-50.0)),
  		 new BasicVisual
  		 	(sptr (new shapes::Cube(makeVector3d(100.0,1.0,100.0))),
  		 	&TIEMat::groundMat),
  		 new BasicGeometry
  		 	(sptr (new shapes::Cube(makeVector3d(100.0,1.0,100.0))),
  		 	sptr (defaultCM))));
  addObject(floor2);
  
  Object * floor3 = new Object
  	(Object::Params
  		(new BasicLocator(makeVector3d(50.0,0.0,-50.0)),
  		 new BasicVisual
  		 	(sptr (new shapes::Cube(makeVector3d(100.0,1.0,100.0))),
  		 	&TIEMat::groundMat),
  		 new BasicGeometry
  		 	(sptr (new shapes::Cube(makeVector3d(100.0,1.0,100.0))),
  		 	sptr (defaultCM))));
  addObject(floor3);
  
  Object * floor4 = new Object
  	(Object::Params
  		(new BasicLocator(makeVector3d(-50.0,0.0,50.0)),
  		 new BasicVisual
  		 	(sptr (new shapes::Cube(makeVector3d(100.0,1.0,100.0))),
  		 	&TIEMat::groundMat),
  		 new BasicGeometry
  		 	(sptr (new shapes::Cube(makeVector3d(100.0,1.0,100.0))),
  		 	sptr (defaultCM))));
  addObject(floor4);
  
  //Walls, each in 2 parts
  
  Object * wall1left = new Object
  	(Object::Params
  		(new BasicLocator(makeVector3d(50.0,5.0,-100.0)),
  		 new BasicVisual
  		 	(sptr (new shapes::Cube(makeVector3d(100.0,10.0,1.0))),
  		 	&TIEMat::groundMat),
  		 new BasicGeometry
  		 	(sptr (new shapes::Cube(makeVector3d(100.0,10.0,1.0))),
  		 	sptr (defaultCM))));
  addObject(wall1left);
  
   Object * wall1right = new Object
  	(Object::Params
  		(new BasicLocator(makeVector3d(-50.0,5.0,-100.0)),
  		 new BasicVisual
  		 	(sptr (new shapes::Cube(makeVector3d(100.0,10.0,1.0))),
  		 	&TIEMat::groundMat),
  		 new BasicGeometry
  		 	(sptr (new shapes::Cube(makeVector3d(100.0,10.0,1.0))),
  		 	sptr (defaultCM))));
  addObject(wall1right);
  
  Object * wall2left = new Object
  	(Object::Params
  		(new BasicLocator(makeVector3d(100.0,5.0,-50.0)),
  		 new BasicVisual
  		 	(sptr (new shapes::Cube(makeVector3d(1.0,10.0,100.0))),
  		 	&TIEMat::groundMat),
  		 new BasicGeometry
  		 	(sptr (new shapes::Cube(makeVector3d(1.0,10.0,100.0))),
  		 	sptr (defaultCM))));

  addObject(wall2left);
  
   Object * wall2right = new Object
  	(Object::Params
  		(new BasicLocator(makeVector3d(100.0, 5.0, 50.0)),
  		 new BasicVisual
  		 	(sptr (new shapes::Cube(makeVector3d(1.0,10.0,100.0))),
  		 	&TIEMat::groundMat),
  		 new BasicGeometry
  		 	(sptr (new shapes::Cube(makeVector3d(1.0,10.0,100.0))),
  		 	sptr (defaultCM))));

  addObject(wall2right);
  
  Object * wall3left = new Object
  	(Object::Params
  		(new BasicLocator(makeVector3d(-100.0,5.0,50.0)),
  		 new BasicVisual
  		 	(sptr (new shapes::Cube(makeVector3d(1.0,10.0,100.0))),
  		 	&TIEMat::groundMat),
  		 new BasicGeometry
  		 	(sptr (new shapes::Cube(makeVector3d(10.0, 10.0, 100.0))),
  		 	sptr (defaultCM))));

  addObject(wall3left);
  
   Object * wall3right = new Object
  	(Object::Params
  		(new BasicLocator(makeVector3d(-100.0, 5.0, -50.0)),
  		 new BasicVisual
  		 	(sptr (new shapes::Cube(makeVector3d(1.0,10.0,100.0))),
  		 	&TIEMat::groundMat),
  		 new BasicGeometry
  		 	(sptr (new shapes::Cube(makeVector3d(10.0, 10.0, 100.0))),
  		 	sptr (defaultCM))));

  addObject(wall3right);
  
 //Make a high platform
 
  Object * platform1 = new Object
  	(Object::Params
  		(new BasicLocator(makeVector3d(-35.0, 11.0, -25.0)),
  		new BasicVisual
  			(sptr (new shapes::Cube(makeVector3d(60.0, 15.0, 10.0))),
  			&TIEMat::blackMat),
  		new BasicGeometry
  			(sptr (new shapes::Cube(makeVector3d(60.0, 15.0, 10.0))),
  			sptr (defaultCM))));

   addObject(platform1);

 //(Sort of)seesaw on the platform
 
  Object * sCube = new Object
  	(Object::Params
  		(new ODELocator(makeVector3d(-35.0, 25.0, -25.0)),
  		new BasicVisual
  			(sptr (new shapes::Cube(makeVector3d(8.0, 10.0, 1.0))),
  			&TIEMat::whiteMat),
  		new BasicGeometry
  			(sptr (new shapes::Cube(makeVector3d(8.0, 10.0, 1.0))),
  			sptr (new CollisionMaterial(1.0, 0.1, 0.0)))));
  
  addObject(sCube);
  
  Object * sBoard = new Object
  	(Object::Params
  		(new ODELocator(makeVector3d(-35.0, 37.0, -25.0)),
  		new BasicVisual
  			(sptr (new shapes::Cube(makeVector3d(3.0, 1.0, 24.0))),
  			&TIEMat::blackMat),
  		new BasicGeometry
  			(sptr (new shapes::Cube(makeVector3d(3.0, 1.0, 24.0))),
  			sptr(new CollisionMaterial(1.0, 0.1, 0.0)))));
  			
  addObject(sBoard);

/*
	Left for posterity: VTT's original, exploding seesaw

  Seesaw * seesaw1 = new Seesaw (makeVector3d(-35.0, 23.0, -24.5), 30);
  
  //Remember to add it
  
  addObject(seesaw1);
*/
 // Put 2 balls, one in either end of seesaw
 
  Object * seesawBall1 = new Object
 	(Object::Params
 		(new ODELocator(makeVector3d(-35.0, 42.0, -36.5)),
 		new BasicVisual
 			(sptr (new shapes::Sphere(3.0)),
 			&TIEMat::redMat),
 		new BasicGeometry
 			(sptr (new shapes::Sphere(3.0)),
 			sptr (new CollisionMaterial(0.1, 1.0, 0.0)))));
 
  addObject(seesawBall1);
  
  Object * seesawBall2 = new Object
 	(Object::Params
 		(new ODELocator(makeVector3d(-35.0, 42.0, -13.5)),
 		new BasicVisual
 			(sptr (new shapes::Sphere(3.0)),
 			&TIEMat::redMat),
 		new BasicGeometry
 			(sptr (new shapes::Sphere(3.0)),
 			sptr (new CollisionMaterial(0.1, 1.0, 0.0)))));
 
  addObject(seesawBall2);			
  

 //A slippery ramp
 
  Object * ramp1 = new Object
  	(Object::Params
  		(new BasicLocator 
  			(makeVector3d (45.0, 8.0, 15.0),
  			BasisMatrix
  				(makeVector3d (0.0, 0.0, 1.0),
  				makeVector3d (0.0, 1.0, -0.24))),
  		new BasicVisual
  			(sptr (new shapes::Cube(makeVector3d(6.0, 0.5, 67.0))), 
  			&TIEMat::blackMat),
  		new BasicGeometry
  			(sptr (new shapes::Cube(makeVector3d(6.0, 0.5, 67.0))),
  			sptr (new CollisionMaterial(0.03, 0.3, 0.0)))));
  		
   addObject (ramp1);
  				
 //A plateau to the end of ramp
  
  Object * plateau = new Object
  	(Object::Params
  		(new BasicLocator
  			(makeVector3d ( 45.0, 15.0, 55.0)),
  		new BasicVisual
  			(sptr (new shapes::Cube(makeVector3d(15.0, 1.0, 15.0))),
  			&TIEMat::whiteMat),
  		new BasicGeometry
  			(sptr (new shapes::Cube(makeVector3d(15.0, 1.0, 15.0))),
  			sptr (new CollisionMaterial(1.0, 0.1, 0.0)))));
  			
  	addObject (plateau);
  	
  //Cube Spawner next to the Plateau
  
  addObject(new CubeSpawner (3.0, 50.0, makeVector3d(56.0, 18.0, 55.0),
  			this,5,true,true,false));
  			
  				
 //Ball spawner
 
  BallSpawner * ballSpawner = new BallSpawner
  	(1.5, 100.0, makeVector3d (-65.0, 3.0, 50.0), this, 6);
  
  addObject(ballSpawner);  				
  
  //Bowling alley
  
  addObject (new BowlingAlley(makeVector3d( -60.0, 3.0, 70.0)));
 
 //Seesaw somewhere
 
 addObject (new Seesaw(makeVector3d(-50.0, 4.0, 70.0), 9.5));
 

 addObject (new CubeSpawner(0.9, 0.9, makeVector3d(53.0, 3.5, 70.0), this,
 	1, false, false, true));

//base for hack-seesaw
  Object * sStopper = new Object
  	(Object::Params
  		(new BasicLocator(makeVector3d(48.0, 1.0, 70.0)),
  		new BasicVisual
  			(sptr (new shapes::Cube(makeVector3d(0.1, 2.1, 3.0))),
  			&TIEMat::blackMat),
  		new BasicGeometry
  			(sptr (new shapes::Cube(makeVector3d(0.1, 2.1, 3.0))),
  			sptr (new CollisionMaterial(1.0, 0.1, 0.0)))));
  			
  addObject (sStopper);  
  
  Object *sPlank = new Object
  	(Object::Params
  		(new ODELocator(makeVector3d(48.0, 3.0, 70.0)),
  		new BasicVisual
  			(sptr (new shapes::Cube(makeVector3d(16.0, 0.3, 2.0))),
  			&TIEMat::redMat),
  		new BasicGeometry
  			(sptr (new shapes::Cube(makeVector3d(16.0, 0.3, 2.0))),
  			sptr (new CollisionMaterial(3.0, 0.1, 0.0)))));
  			
  addObject(sPlank);
  
  
 //BVD (don't ask) Spawner on the other side
 
  BVDSpawner * bvdSpawner = new BVDSpawner
  	(4.0, 15.0, makeVector3d (-65.0, 0.6, -50.0), this, 10);
  	
  addObject(bvdSpawner);
 
 
 
 //Lots of cubes with torque
  
 addObject(new CubeSpawner (1.0, 35.0, makeVector3d (-75.0, 35.00, -60.0),
 	this, 10, false, false, true, makeVector3d(),
 		 makeVector3d(0.0, 50.0, 0.0)));
 
 //Lots of spheres with torque
 
 addObject(new BallSpawner (1.0, 35.0, makeVector3d (-75.0, 40.00, -60.0),
 	this, 8, false, false, true, makeVector3d(),
 		 makeVector3d(0.0, 000.0, 0.0)));						
 
 
  
  
  
  // create one light object
  Object * lightObj = new Object
    ( Object::Params( new BasicLocator( makeVector3d( 50.0, 48.0, -3.0 )),
                      new BasicVisual( shapes::Sphere::create(),
                                       &TIEMat::redMat ),
                      new BasicGeometry
                      	(sptr (new shapes::Sphere(1.0)),
                      	sptr (new CollisionMaterial (2.0, 0.9, 0.0 )))));
  addObject( lightObj );
  
  // create the actual light
  GLfloat * lightAtt = new GLfloat[3];
  lightAtt[0] = 1.0; lightAtt[1] = 0.0; lightAtt[2] = 0.0;
  Light * light = new Light( &TIEMat::redEmissionMat, lightObj, lightAtt );
  getEnvironment()->addLight( light );
  
  //Another light
  
    // create a new light object
  Object * lightObj5 = new Object
    ( Object::Params( new BasicLocator( makeVector3d( -50.0, 44.0, 3.0 )),
                      new BasicVisual( sptr( new shapes::Sphere ),
                                       &TIEMat::redEmissionMat ),
                      new BasicGeometry
                      	(sptr (new shapes::Sphere(1.0)),
                      	sptr (new CollisionMaterial (2.0, 0.9, 0.0 )))));
  addObject( lightObj5 );
  
  // create the actual light
  GLfloat * lightAtt5 = new GLfloat[3];
  lightAtt5[0] = 1.0; lightAtt5[1] = 0.0; lightAtt5[2] = 0.0;
  Light * light5 = new Light( &TIEMat::whiteMat, lightObj5, lightAtt );
  getEnvironment()->addLight( light5 );
 
 // Create a windmill
 
 //First, a solid (non-moveable) base for the windmill
 
  Object * windmillBase = new Object
  	( Object::Params (new BasicLocator (makeVector3d(0.0, 10.1, -20.0)),
  	
  	//For ODELocator
//  							,BasisMatrix(), (WingMass * 3.0), 1.0),
  					new BasicVisual( sptr(new shapes::Cube
  						(makeVector3d(2.0, 21.0, 1.0))),
  						&TIEMat::redMat),
  					new BasicGeometry
  						(sptr (new shapes::Cube
  							(makeVector3d(1.0,21.0, 1.0))),
  						sptr (new CollisionMaterial (0.1, 0.9, 0.0)))));
  addObject (windmillBase);
  
 //Make the propellors
 
  shapes::Union * wingShape = new shapes::Union (
  		sptr (new shapes::Cube(makeVector3d(33.0, 3.0, 0.501))),
  		sptr (new shapes::Cube(makeVector3d(3.0, 33.0, 0.5))));
 
  Object * windmillWings = new Object
  	( Object::Params (new ODELocator (makeVector3d(0.0, 20.0, -15.0),
  							BasisMatrix(), WingMass, 1.0),
  					new BasicVisual(sptr (wingShape), &TIEMat::whiteMat),
  					new BasicGeometry (
  						sptr (wingShape),
  						sptr (new CollisionMaterial (1.0, 0.9, 0.0)))));
  						
  addObject (windmillWings);
  
  //Make some connectors
  
  wingConnector = new ODEAxleConnector
      ( Connector( *windmillWings, Connector::Any,
                   BasicLocator( makeVector3d( 0.0, 0.0, -0.5 ),
                                 BasisMatrix(
                                             makeVector3d( 0.0, 0.0, -1.0),
                                             makeVector3d( 0.0, 1.0, 0.0)))),
        500.0, -INFINITY, INFINITY, 0.0);
        
  //Connectors in Object is protected, so need to go around        
        
  const_cast<Object::connectors_t &>( windmillWings->getConnectors() )[0] =
    sptr( wingConnector );
        
  windmillConnector = new ODEAxleConnector
  	( Connector( *windmillBase, Connector::Any,
  				BasicLocator ( makeVector3d(0.0, 9.0, 0.5),
  							BasisMatrix(
  								makeVector3d(0.0, 0.0, 1.0),
  								makeVector3d(0.0, 1.0, 0.0)))),
  				500.0, -INFINITY, INFINITY, 0.0);
 
  const_cast<Object::connectors_t &>( windmillBase->getConnectors() )[0] =
    sptr( windmillConnector );

  //Batter thing
  
  Object * batterBase = new Object
  	(Object::Params
  		(new BasicLocator(makeVector3d(80.0, 4.0, -80.0)),
  		new BasicVisual(sptr (new shapes::Cube(makeVector3d(0.5, 8.0, 0.5))),
  						&TIEMat::whiteMat),
  		new BasicGeometry(sptr(new shapes::Cube(makeVector3d(0.5, 8.0, 0.5))),
  						sptr(new CollisionMaterial(1.0, 1.0, 0.0)))));
  						
  addObject(batterBase);
  
  Object * batterBat = new Object
  	(Object::Params
  		(new ODELocator(makeVector3d(80.0, 4.0, -85.0), BasisMatrix(),5.0, 2.0),
  		new BasicVisual(sptr (new shapes::CappedCylinder(7.0, 0.8)),
  						&TIEMat::redMat),
  		new BasicGeometry(sptr (new shapes::CappedCylinder(6.0, 0.8)),
  						sptr(new CollisionMaterial(0.2, 1.0, 0.0)))));
  			
  addObject (batterBat);			
  
   batConnector = new ODEAxleConnector
      ( Connector( *batterBat, Connector::Any,
                   BasicLocator( makeVector3d( 0.0, 0.0, 4.1 ),
                                 BasisMatrix(
                                             makeVector3d( 0.0, -1.0, 0.0),
                                             makeVector3d( 1.0, 0.0, 0.0)))),
        5000.0, -INFINITY, INFINITY, 0.0);
        
  //Connectors in Object is protected, so need to go around        
        
  const_cast<Object::connectors_t &>( batterBat->getConnectors() )[0] =
    sptr( batConnector ); 
      
  batterBaseConnector = new ODEAxleConnector
  	( Connector( *batterBase, Connector::Any,
  				BasicLocator ( makeVector3d(0.0, 4.5, 0.0),
  							BasisMatrix(
  								makeVector3d(0.0, 1.0, 0.0),
  								makeVector3d(1.0, 0.0, 0.0)))),
  				5000.0, -INFINITY, INFINITY, 0.0);
 
  const_cast<Object::connectors_t &>( batterBase->getConnectors() )[0] =
    sptr( batterBaseConnector );

//Spawner to fire at Batter, with torque of course
  
 addObject(new CubeSpawner (1.0, 35.0, makeVector3d (55.0, 7.00, -86.0),
 	this, 12, false, false, true, makeVector3d(140.0, 65.0, 3.0),
 		 makeVector3d(0.0, 45.0, 45.0)));


  /* -------- finalize -------- */
  
 //These all for now in main_glow.cpp
 
 /*
  // activate the ODE world
  activate( true );
  
  // create and activate the collision detector
  collisionRenderer = new ODECollisionRenderer( this );
  collisionRenderer->connect();
  graphicsEventHost.addListener( collisionRenderer );
  
  // connect all connectors
  
  
  */
}


IMWorld::~IMWorld()
{
}
