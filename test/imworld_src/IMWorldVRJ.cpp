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
 * @file IMWorldVRJ.cpp
 *
 * The Improbable Mission demo world for TIE (for EVE environment).
 */

#include "IMWorldVRJ.hpp"

#include<tie/tie.hpp>
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;


#include <lifespace/plugins/fluid.hpp>
using namespace lifespace::plugins::pfluid;

#include <lifespace/plugins/vrjuggler.hpp>
using namespace lifespace::plugins::pvrjuggler;


#include "Chain.hpp"


#include <ode/ode.h>
#include <ode/odecpp.h>

#include <string>
#include <vector>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <GL/gl.h>


static float WorldFriction = 1.3;
static float WingMass = 3.0;

static float StarDist = 500.0;
static unsigned int StarCount = 50;
static float StarSize = 1.5;

static int FloorDivisions = 16;  //_MUST_ be sqrt:able value!
static float FloorSize = 200.0;




/** Dumps the current framerate to cout. */ 
class DumpFPS :
  public EventListener<GraphicsEvent>
{
  double prev;
  float avgFPS;

  double getTime()
  {
    timeval tv; gettimeofday( &tv, 0 );
    return ((double)tv.tv_sec) + ((double)tv.tv_usec) * 1e-6;
  }
  
public:
  
  DumpFPS() :
    prev( getTime() ),
    avgFPS( 0.0 )
  {}
  
  void processEvent( const GraphicsEvent * event )
  {
    double time = getTime();
    
    if( event->id == GE_REFRESH_BEGIN ) {
      float fps = time - prev;
      prev = time;
      avgFPS = 0.75 * avgFPS + 0.25 * fps;
      
      cout << "current fps: " << std::fixed << std::setprecision(1)
           << (1.0 / fps) << ", avg fps: " << (1.0 / avgFPS) << endl;
    }
  }
  
};




/* ----------private methods---------- */
void 
IMWorld::makeSky()
{
  //Make some light
 
  // create a moon
  Object * lightObj5 = new Object
    ( Object::Params( new BasicLocator( makeVector3d( -50.0, 44.0, 13.0 )),
                      new BasicVisual( shapes::Located::create
                                       ( BasicLocator( makeVector3d( -500.0, 440.0, 130.0 )),
                                         shapes::Sphere::create( 20.0 ) ),
                                       &TIEMat::whiteEmissionMat ),
                      0));
  addObject( lightObj5 );
  
  // create the actual light
  GLfloat * lightAtt5 = new GLfloat[3];
  lightAtt5[0] = 0.85; lightAtt5[1] = 0.0; lightAtt5[2] = 0.0;
  Light * light5 = new Light( &TIEMat::whiteMat, lightObj5, lightAtt5 );
  getEnvironment()->addLight( light5 );


  // create a new light object
  Object * lightObj3 = new Object
    ( Object::Params( new BasicLocator( makeVector3d( 50.0, 44.0, -13.0 )),
                      0, 0 ));
  addObject( lightObj3 );
  
  // create the actual light

  Light * light3 = new Light( &TIEMat::whiteMat, lightObj3, lightAtt5 );
  //getEnvironment()->addLight( light3 );

  // create stars on the sky
  float theta = 0.0; // xy -plane angle
  float phi = 0.0;   // zx -plane angle
  
  shared_ptr<Shape> star( shapes::Cube::create
                          ( makeVector3d( StarSize, StarSize, StarSize)));
  shared_ptr<shapes::Union> stars( shapes::Union::create() );
  for( int i = 0; i < StarCount; i++ ) {
    theta = FRAND01() * M_PI;
    phi = FRAND01() * M_PI;
    
    
    stars->targets.push_back
      ( shapes::Located::create
        ( BasicLocator( makeVector3d
                        ( StarDist * cos( phi ) * cos( theta ),
                          StarDist * sin( theta ),
                          StarDist * sin( phi ) * cos( theta )),
                        BasisMatrix( makeVector3d( FRAND01(),
                                                   FRAND01(),
                                                   FRAND01() ),
                                     makeVector3d( FRAND01(),
                                                   FRAND01(),
                                                   FRAND01() ))),
          star ));
    
  }
  
  addObject( new Object
             ( Object::Params( new BasicLocator
                               ( makeVector3d( 0.0, 0.0, 0.0 )),
                               new BasicVisual( stars,
                                                &TIEMat::whiteEmissionMat ),
                               0 )));
               
}

void
IMWorld::makeField()
{
  shared_ptr<CollisionMaterial> defaultCM( new CollisionMaterial(WorldFriction, 0.7, 0.0));
  
  //First, generate a floor ( collision form ).
  Object * collFloor = new Object
    (Object::Params
     (new BasicLocator(makeVector3d(0.0, -1.0,0.0)),
      0,
      new BasicGeometry
      (sptr (new shapes::Cube(makeVector3d(FloorSize,3.0,FloorSize))),
       defaultCM)));
  addObject(collFloor);
  
  
  // Then, make visuals for it
  int divSqrt = (int)sqrt( (double)FloorDivisions );
  
  shared_ptr<Shape> floorPiece
    ( shapes::Cube::create
      ( makeVector3d(FloorSize/divSqrt,3.0,FloorSize/divSqrt ) ));
  shared_ptr<shapes::Union> floorShape( shapes::Union::create() );
  
  for( int i=0; i<divSqrt; i++ ) {
    for( int j=0; j<divSqrt; j++ ) {
      
      floorShape->targets.push_back
        ( shapes::Located::create
          ( BasicLocator( makeVector3d
                          ((-FloorSize/2.0+0.5*FloorSize/divSqrt)+
                           FloorSize*j/divSqrt,
                           -1.0,
                           (-FloorSize/2.0+0.5*FloorSize/divSqrt)+
                           FloorSize*i/divSqrt) ),
            floorPiece ));
      
    }
    
  }
  
  addObject
    ( new Object( Object::Params( new BasicLocator(),
                                  new BasicVisual( floorShape,
                                                   &TIEMat::groundMat ),
                                  0 )));
  
  //Walls, each in 2 parts
  
  Object * wall1left = new Object
    (Object::Params
     (new BasicLocator(makeVector3d(50.0, 7.0, -100.0)),
      new BasicVisual
      (sptr (new shapes::Cube(makeVector3d(100.0,15.0,1.0))),
       &TIEMat::wall1Mat),
      new BasicGeometry
      (sptr (new shapes::Cube(makeVector3d(100.0,15.0,1.0))),
       defaultCM)));
  addObject(wall1left);
  
  Object * wall1right = new Object
  	(Object::Params
         (new BasicLocator(makeVector3d(-50.0, 7.0, -100.0)),
          new BasicVisual
          (sptr (new shapes::Cube(makeVector3d(100.0,15.0,1.0))),
           &TIEMat::wall1Mat),
          new BasicGeometry
          (sptr (new shapes::Cube(makeVector3d(100.0,15.0,1.0))),
           defaultCM)));
  addObject(wall1right);
  
  Object * wall2left = new Object
    (Object::Params
     (new BasicLocator(makeVector3d(100.0,7.0,-50.0)),
      new BasicVisual
      (sptr (new shapes::Cube(makeVector3d(1.0,15.0,100.0))),
       &TIEMat::wall2Mat),
      new BasicGeometry
      (sptr (new shapes::Cube(makeVector3d(1.0,15.0,100.0))),
       defaultCM)));
  
  addObject(wall2left);
  
  Object * wall2right = new Object
  	(Object::Params
         (new BasicLocator(makeVector3d(100.0, 7.0, 50.0)),
          new BasicVisual
          (sptr (new shapes::Cube(makeVector3d(1.0,15.0,100.0))),
           &TIEMat::wall2Mat),
          new BasicGeometry
          (sptr (new shapes::Cube(makeVector3d(1.0,15.0,100.0))),
           defaultCM)));
  
  addObject(wall2right);
  
  Object * wall3left = new Object
    (Object::Params
     (new BasicLocator(makeVector3d(-100.0,7.0,50.0)),
      new BasicVisual
      (sptr (new shapes::Cube(makeVector3d(1.0,15.0,100.0))),
       &TIEMat::wall3Mat),
      new BasicGeometry
      (sptr (new shapes::Cube(makeVector3d(10.0, 15.0, 100.0))),
  		 	defaultCM)));
  
  addObject(wall3left);
  
  Object * wall3right = new Object
    (Object::Params
     (new BasicLocator(makeVector3d(-100.0, 7.0, -50.0)),
      new BasicVisual
      (sptr (new shapes::Cube(makeVector3d(1.0,15.0,100.0))),
       &TIEMat::wall3Mat),
      new BasicGeometry
      (sptr (new shapes::Cube(makeVector3d(10.0, 15.0, 100.0))),
       defaultCM)));
  
  addObject(wall3right);
}







/* ----------constructor---------- */

IMWorld::IMWorld( EventHost<GraphicsEvent> & graphicsEventHost,
                  FluidHost * inputHost,
                  Avatar * avatar,
                  UserInterface * ui )
{
  // attach a framerate counter
  graphicsEventHost.addListener( new DumpFPS() );
  
  
  addObject( ui );
  
  // init and add avatar object to the world
  avatar->init( inputHost, this );



  addObject( avatar );




  
  /* -------- create world content -------- */
  
  //Create default collisionmaterial

  shared_ptr<CollisionMaterial> defaultCM( new CollisionMaterial
                                           (WorldFriction, 0.7, 0.0));

  // make floor and walls
  makeField();

  // make sky with stars
  makeSky();

  //Make a high platform
  
  Object * platform1 = new Object
    (Object::Params
     (new BasicLocator(makeVector3d(-35.0, 9.0, -25.0)),
      new BasicVisual
      (sptr (new shapes::Cube(makeVector3d(60.0, 18.0, 10.0))),
       &TIEMat::blackMat),
      new BasicGeometry
      ( shapes::Cube::create
        ( makeVector3d(60.0, 18.0, 10.0)),
        defaultCM)));

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
      (sptr (new shapes::Cube(makeVector3d(12.0, 0.5, 67.0))), 
       &TIEMat::blackMat),
      new BasicGeometry
      (sptr (new shapes::Cube(makeVector3d(12.0, 0.5, 67.0))),
       sptr (new CollisionMaterial(0.15, 0.3, 0.0)))));
  		
  addObject (ramp1);
  				
  //A plateau to the end of ramp
  
  Object * plateau = new Object
    (Object::Params
     (new BasicLocator
      (makeVector3d ( 45.0, 15.0, 55.0)),
      new BasicVisual
      (sptr (new shapes::Cube(makeVector3d(18.0, 1.0, 15.0))),
       &TIEMat::whiteMat),
      new BasicGeometry
      (sptr (new shapes::Cube(makeVector3d(18.0, 1.0, 15.0))),
       sptr (new CollisionMaterial(1.0, 0.1, 0.0)))));
  			
  addObject (plateau);
  	
  //Cube Spawner next to the Plateau
  
        
  addObject(new CubeSpawner (2.0, 5.0, makeVector3d(52.0, 18.0, 52.0),
                             this,3,true,true,false));
  			
  				
  //Ball spawner
 
  BallSpawner * ballSpawner = new BallSpawner
    (1.5, 5.0, makeVector3d (-65.0, 3.0, 50.0), this, 5);
  
  addObject(ballSpawner);  				
  
        
  //Bowling alley
  
  addObject (new BowlingAlley(makeVector3d( -60.0, 3.0, 70.0)));
 
  //Cube on the seesaw

  addObject (new CubeSpawner(1.2, 1.0, makeVector3d(53.0, 2.8, 70.0), this,
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
    (4.0, 15.0, makeVector3d(-65.0, 2.6, -50.0), this, 3);
  	
  addObject(bvdSpawner);
 
 
  //Lots of cubes with torque
  
  addObject(new CubeSpawner (2.0, 35.0, makeVector3d (-75.0, 35.00, -60.0),
                             this, 8, false, false, true, makeVector3d(),
                             makeVector3d(0.0, 50.0, 0.0)));
 
  //Lots of spheres with torque
 
  addObject(new BallSpawner (2.0, 35.0, makeVector3d (-75.0, 40.00, -60.0),
                             this, 7, false, false, true, makeVector3d(),
                             makeVector3d(0.0, 30.0, 0.0)));						
  
  //Put cube with light
  
  Object * hillCube = new Object
    (Object::Params (new ODELocator (makeVector3d(-20.0, 15.6, -80.0)),
                     new BasicVisual (sptr (new shapes::Cube
                                            (makeVector3d(2.0, 2.0, 2.0))),
                                      &TIEMat::whiteMat),
                     new BasicGeometry (sptr (new shapes::Cube
                                              (makeVector3d(2.0, 2.0, 2.0))),
                                        defaultCM)));
  
  addObject (hillCube);

  //Put light in cube

  // create the actual light
  GLfloat * cubeLightAtt = new GLfloat[3];
  cubeLightAtt[0] = 1.0; cubeLightAtt[1] = 0.0; cubeLightAtt[2] = 0.02;
  Light * cubeLight = new Light( &TIEMat::brightWhiteMat,
                                 hillCube,
                                 cubeLightAtt );
  getEnvironment()->addLight( cubeLight );


 // Create a windmill
 
 //First, a solid (non-moveable) base for the windmill
 
  Object * windmillBase = new Object
    ( Object::Params (new BasicLocator (makeVector3d(0.0, 10.1, -20.0)),
                      new BasicVisual( sptr(new shapes::Cube
                                            (makeVector3d(2.0, 21.0, 1.0))),
                                       &TIEMat::redMat),
                      new BasicGeometry
                      (sptr (new shapes::Cube
                             (makeVector3d(1.0,21.0, 1.0))),
                       sptr (new CollisionMaterial (0.1, 0.9, 0.0)))));
  addObject (windmillBase);
  
  //Make the propellors
  
  enum PropellorConnectors {
    PROPELLOR_CONN_BASE,
    PROPELLOR_CONN_WINGTIP_0,   // keep these four adjacent and in this order!
    PROPELLOR_CONN_WINGTIP_1,
    PROPELLOR_CONN_WINGTIP_2,
    PROPELLOR_CONN_WINGTIP_3
  };
  float propellorLength = 33.0;
  float wingtipConnectorDist = propellorLength / 2.0 - 2.0;
  float wingtipConnectorZOffset = 1.0;
  
  shapes::Union * wingShape = new shapes::Union
    ( sptr (new shapes::Cube(makeVector3d(propellorLength, 3.0, 0.501))),
      sptr (new shapes::Cube(makeVector3d(3.0, propellorLength, 0.5))));
  
  Object * windmillWings = new Object
    ( Object::Params (new ODELocator (makeVector3d(0.0, 20.0, -15.0),
                                      BasisMatrix(), WingMass, 1.0),
                      new BasicVisual(sptr (wingShape), &TIEMat::whiteMat),
                      new BasicGeometry (
                                         sptr (wingShape),
                                         sptr (new CollisionMaterial
                                               (2.0, 0.1, 0.1)))));
  
  addObject (windmillWings);
  
  //Make some connectors
  
  ODEAxleConnector * wingConnector = new ODEAxleConnector
    ( Connector( *windmillWings, Connector::Any,
                 BasicLocator( makeVector3d( 0.0, 0.0, -0.5 ),
                               BasisMatrix(
                                           makeVector3d( 0.0, 0.0, -1.0),
                                           makeVector3d( 0.0, 1.0, 0.0)))),
      20000.0, -INFINITY, INFINITY, 0.0);
  
  //Connectors in Object is protected, so need to go around        
  Object::connectors_t & wingConnectors =
    const_cast<Object::connectors_t &>( windmillWings->getConnectors() );
  
  // add the base connector
  wingConnectors[PROPELLOR_CONN_BASE] = sptr( wingConnector );
  
  // add the wingtip connectors
  wingConnectors[PROPELLOR_CONN_WINGTIP_0] =
    sptr( new ODEBallConnector
          ( Connector( *windmillWings, Connector::Any,
                       BasicLocator
                       ( makeVector3d( -wingtipConnectorDist,
                                       0.0,
                                       wingtipConnectorZOffset ),
                         BasisMatrix( makeVector3d( -1.0, 0.0, 0.0 ),
                                      makeVector3d( 0.0, 0.0, 1.0 ) )))));
  wingConnectors[PROPELLOR_CONN_WINGTIP_1] =
    sptr( new ODEBallConnector
          ( Connector( *windmillWings, Connector::Any,
                       BasicLocator
                       ( makeVector3d( wingtipConnectorDist,
                                       0.0,
                                       wingtipConnectorZOffset ),
                         BasisMatrix( makeVector3d( 1.0, 0.0, 0.0 ),
                                      makeVector3d( 0.0, 0.0, 1.0 ) )))));
  wingConnectors[PROPELLOR_CONN_WINGTIP_2] =
    sptr( new ODEBallConnector
          ( Connector( *windmillWings, Connector::Any,
                       BasicLocator
                       ( makeVector3d( 0.0,
                                       -wingtipConnectorDist,
                                       wingtipConnectorZOffset ),
                         BasisMatrix( makeVector3d( 0.0, -1.0, 0.0 ),
                                      makeVector3d( 0.0, 0.0, 1.0 ) )))));
  wingConnectors[PROPELLOR_CONN_WINGTIP_3] =
    sptr( new ODEBallConnector
          ( Connector( *windmillWings, Connector::Any,
                       BasicLocator
                       ( makeVector3d( 0.0,
                                       wingtipConnectorDist,
                                       wingtipConnectorZOffset ),
                         BasisMatrix( makeVector3d( 0.0, 1.0, 0.0 ),
                                      makeVector3d( 0.0, 0.0, 1.0 ) )))));
  
  // create the pole connector
  ODEAxleConnector * windmillConnector = new ODEAxleConnector
    ( Connector( *windmillBase, Connector::Any,
                 BasicLocator ( makeVector3d(0.0, 9.0, 0.5),
                                BasisMatrix(
                                            makeVector3d(0.0, 0.0, 1.0),
                                            makeVector3d(0.0, 1.0, 0.0)))),
      20000.0, -INFINITY, INFINITY, 0.0);
  
  // insert the pole connector
  const_cast<Object::connectors_t &>( windmillBase->getConnectors() )[0] =
    sptr( windmillConnector );
  
  // create the windmill chains
  Chain * windmillChains[4];
  {
    Chain::LinkDimensions chainLinkDims = { 1.5, 0.6, 0.2 };
    shared_ptr<const CollisionMaterial> chainCollisionMaterial
      ( new CollisionMaterial( 0.2, 0.1, 0.01 ) );
    // tmp uplim!!!
    for( int i = 0 ; i < 1 ; i++ ) {
      windmillChains[i] =
        new Chain( chainLinkDims, 6,
                   &TIEMat::polishedMetalMat, chainCollisionMaterial,
                   *this );
      //addObject( windmillChains[i] );
    }
  }
  
  
  //Batter thing
  
  Object * batterBase = new Object
    (Object::Params
     (new BasicLocator(makeVector3d(80.0, 4.0, -80.0)),
      new BasicVisual(sptr (new shapes::Cube(makeVector3d(0.5, 8.0, 0.5))),
                      &TIEMat::whiteMat),
      new BasicGeometry(sptr(new shapes::Cube(makeVector3d(0.5, 8.0, 0.5))),
                        sptr(new CollisionMaterial(1.0, 0.9, 0.0)))));
  
  addObject(batterBase);
  
  Object * batterBat = new Object
    (Object::Params
     (new ODELocator(makeVector3d(80.0, 4.0, -85.0), BasisMatrix(),5.0, 2.0),
      new BasicVisual(sptr (new shapes::CappedCylinder(7.0, 0.8)),
                      &TIEMat::redMat),
      new BasicGeometry(sptr (new shapes::CappedCylinder(6.0, 0.8)),
                        sptr(new CollisionMaterial(0.2, 1.0, 0.0)))));
  
  addObject (batterBat);			
  
  ODEAxleConnector * batConnector = new ODEAxleConnector
    ( Connector( *batterBat, Connector::Any,
                 BasicLocator( makeVector3d( 0.0, 0.0, 4.1 ),
                               BasisMatrix(
                                           makeVector3d( 0.0, -1.0, 0.0),
                                           makeVector3d( 1.0, 0.0, 0.0)))),
      5000.0, -INFINITY, INFINITY, 0.0);
  
  //Connectors in Object is protected, so need to go around        
  
  const_cast<Object::connectors_t &>( batterBat->getConnectors() )[0] =
    sptr( batConnector ); 
  
  ODEAxleConnector * batterBaseConnector = new ODEAxleConnector
    ( Connector( *batterBase, Connector::Any,
                 BasicLocator ( makeVector3d(0.0, 4.5, 0.0),
                                BasisMatrix(
                                            makeVector3d(0.0, 1.0, 0.0),
                                            makeVector3d(1.0, 0.0, 0.0)))),
      5000.0, -INFINITY, INFINITY, 0.0);
  
  const_cast<Object::connectors_t &>( batterBase->getConnectors() )[0] =
    sptr( batterBaseConnector );
  
  //Spawner to fire at Batter, with torque of course
  
  addObject(new CubeSpawner (1.7, 35.0, makeVector3d (55.0, 7.00, -86.0),
                             this, 8, false, false, true,
                             makeVector3d(230.0, 220.0, 3.0),
                             makeVector3d(0.0, 45.0, 50.0)));
  
  
  /* -------- finalize -------- */
  
  setGravityVector( makeVector3d( 0.0, -GRAVITY, 0.0 ));
  dWorldSetContactMaxCorrectingVel( *this, 100.0 );
  
  // activate the ODE world
  activate( true );
  
  // create and activate the collision detector
  collisionRenderer = new ODECollisionRenderer( this );
  collisionRenderer->connect();
  graphicsEventHost.addListener( collisionRenderer );
  
  // connect all connectors
  
  avatar->connect();
  
  
  wingConnector->connect(*windmillConnector, Connector::AlignMaster);  
  batConnector->connect(*batterBaseConnector, Connector::AlignMaster);
  
  // tmp uplim!!!
  for( int i = 0 ; i < 1 ; i++ ) {
    windmillChains[i]->connect
      ( &windmillWings->getConnector( PROPELLOR_CONN_WINGTIP_0 + i ) );
  }
  
  //Make it spin
  
  wingConnector->useControl(ODEAxleConnector::CTRL_MOTOR_FORCEFACTOR, 1.0);
  wingConnector->useControl(ODEAxleConnector::CTRL_MOTOR_TARGETVEL, 0.2);
  
  batConnector->useControl(ODEAxleConnector::CTRL_MOTOR_FORCEFACTOR, 1.0);
  batConnector->useControl(ODEAxleConnector::CTRL_MOTOR_TARGETVEL, 2.7);
  
  
  setERP(0.5);
  setCFM(0.001);
  
  
}


IMWorld::~IMWorld()
{
}
