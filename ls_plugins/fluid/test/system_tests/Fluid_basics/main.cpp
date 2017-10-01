/**
 * Test application for fluid plugin to lifespace
 */

/**
 * @file main.cpp
 *
 */

#include "SensorController.hpp"

#include <lifespace/lifespace.hpp>
using namespace lifespace;

/* Fluid stuff */
#include <lifespace/plugins/fluid.hpp>
#include <Fluid/TrackerSensorSelector.hpp>
using namespace lifespace::plugins::pfluid;
using namespace Fluid;

#include <lifespace/plugins/glow.hpp>
using namespace lifespace::plugins::pglow;

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <GL/gl.h>




/* Define some colors. */
static const GLfloat none[4]        = { 0.0, 0.0, 0.0, 0.0 };
static const GLfloat white[4]       = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat gray6[4]       = { 0.6, 0.6, 0.6, 1.0 };
static const GLfloat gray3[4]       = { 0.3, 0.3, 0.3, 1.0 };
static const GLfloat red[4]         = { 1.0, 0.0, 0.0, 1.0 };

static const GLfloat camLightAtt[3] = { 1.0, 0.0, 0.1 };

/* Define some values for the OpenGL specular parameter. */
static const GLfloat polished[1]    = { 40.0 };

/* Define some basic materials. */
static const Material redMat( none, red, white,
			      gray3, polished, GL_FRONT );
static const Material whiteMat( none, gray6, white,
                                gray3, polished, GL_FRONT );
static const Material redEmissionMat( none, red, white,
			              red, polished, GL_FRONT );
static const Material emissionMat( gray6, gray6, white,
                                   gray6, polished, GL_FRONT );


static void drawEveCube( ODEWorld * world )
{
  Object * lw = new Object
    (Object::Params ( new BasicLocator
                      ( makeVector3d( -10.0, 5.0, 0.0 ),
                        BasisMatrix()),
                      new BasicVisual
                      (sptr( new shapes::Cube
                             ( makeVector3d( 0.2, 20.0, 20.0))),
                       &whiteMat ),
                      0 ));
  Object * rw = new Object
    (Object::Params ( new BasicLocator( makeVector3d( 10.0, 5.0, 0.0 ),
                                        BasisMatrix()),
                      new BasicVisual
                      ( sptr( new shapes::Cube
                              ( makeVector3d( 0.2, 20.0, 20.0))),
                        &whiteMat ),
                      0 ));
  Object * fw = new Object
    (Object::Params ( new BasicLocator( makeVector3d( 0.0, 5.0, -10.0 ),
                                        BasisMatrix()),
                      new BasicVisual
                      ( sptr( new shapes::Cube
                              ( makeVector3d( 20.0, 20.0, 0.2))),
                        &whiteMat ),
                      0 ));
  Object * fl = new Object
    (Object::Params ( new BasicLocator( makeVector3d( 0.0, -5.0, 0.0 ),
                                        BasisMatrix()),
                      new BasicVisual
                      ( sptr( new shapes::Cube
                              ( makeVector3d( 20.0, 0.2, 20.0))),
                        &whiteMat ),
                      0 ));
                              
  world->addObject( lw );
  world->addObject( rw );
  world->addObject( fw );
  world->addObject( fl );
  
}



static void fillFluidKeymap( Controller::ControlMap & keymap )
{
  // define the keyboard controls
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
  
  keymap[90] = Controller::MapTarget( FloatingActor::CTRL_SETLOC, 1.0   );
  keymap[100] = Controller::MapTarget( FloatingActor::CTRL_SETBASIS, 1.0   );
}


/* This function fills in the keymap of the camera actor object. Modify this to
   change the keyboard controls and mouse sensitivity. */
static void fillCameraKeymap( Controller::ControlMap & keymap )
{
  // define the keyboard controls (last argument is sensitivity)
  keymap['a'] =
    Controller::MapTarget( FloatingActor::CTRL_FORCEREL_X_NEG, 2.0 );
  keymap['d'] =
    Controller::MapTarget( FloatingActor::CTRL_FORCEREL_X_POS, 2.0 );
  keymap['s'] =
    Controller::MapTarget( FloatingActor::CTRL_FORCEREL_Z_POS, 2.0 );
  keymap['w'] =
    Controller::MapTarget( FloatingActor::CTRL_FORCEREL_Z_NEG, 2.0 );
  keymap[' '] =
    Controller::MapTarget( FloatingActor::CTRL_FORCEREL_Y_POS, 2.0 );
  keymap['<'] =
    Controller::MapTarget( FloatingActor::CTRL_FORCEREL_Y_NEG, 2.0 );
  
  // define the mouse controls. the last argument controls the sensitivity.
  keymap[GLOWViewport::MOUSE_DX] =
    Controller::MapTarget( FloatingActor::CTRL_TORQUEREL_Y_NEG, 0.02 );
  keymap[GLOWViewport::MOUSE_DY] =
    Controller::MapTarget( FloatingActor::CTRL_TORQUEREL_X_NEG, 0.02 );
}


/*
 * The actual world will be created and started here.
 */
int main( int argc, char * argv[] )
{  
  /* create and initialize fluidhost */
  FluidHost * inputHost = new FluidHost( argv[1] );
  
  inputHost->start();
  cout << "Fluid initalized.." << endl;

  /* Init graphics. */
  GLOWDevice::Init( argc, argv );
  GLOWDevice window;
  GLOWViewport viewport( window );

  /* Create hand controllers */
  SensorController leftHand( std::string("lefthand") );
  SensorController rightHand( std::string("righthand") );
  SensorController head( std::string( "head" ) );

  
  /* Create the world. Use ODEWorld instead of World to allow ODELocators to be
     used. */
  ODEWorld world;
  
  
  FloatingActor fluidActorR
    ( Object::Params( new InertiaLocator
                      ( makeVector3d( 0.0, 0.0, 1.0 ),
                        BasisMatrix(), 1.0, 1.0,
                        0.0, 0.2, 0.0, 0.2 ),
                      new BasicVisual
                      ( sptr ( new shapes::Cube 
                               ( makeVector3d( 0.2, 0.2, 0.2 ))),
                        &redMat ),
                      0 ) );

  fluidActorR.setAutoRoll( false );
  world.addObject( &fluidActorR );
    
  FloatingActor fluidActorL
    ( Object::Params( new InertiaLocator
                      ( makeVector3d( 0.0, 0.0, -1.0 ),
                        BasisMatrix(), 1.0, 1.0,
                        0.0, 0.2, 0.0, 0.2 ),
                      new BasicVisual
                      ( sptr ( new shapes::Cube 
                               ( makeVector3d( 0.2, 0.2, 0.2 ))),
                        &redMat ),
                      0 ) );

  fluidActorL.setAutoRoll( false );
  world.addObject( &fluidActorL );

  FloatingActor headActor
    ( Object::Params( new InertiaLocator
                      ( makeVector3d( 0.0, 0.0, -1.0 ),
                        BasisMatrix(), 1.0, 1.0,
                        0.0, 0.2, 0.0, 0.2 ),
                      new BasicVisual
                      ( sptr ( new shapes::Basis),
                        &redMat ),
                      0 ) );

  headActor.setAutoRoll( false );
  world.addObject( &headActor );
  
  /* Set defined controlmap */
  Controller::ControlMap fluidKeymap; fillFluidKeymap( fluidKeymap );

  head.addActor( &headActor, &fluidKeymap );
  inputHost->addController( &head );
  rightHand.addActor( &fluidActorR, &fluidKeymap );
  inputHost->addController( &rightHand );
  leftHand.addActor( &fluidActorL, &fluidKeymap );
  inputHost->addController( &leftHand );
  
  

  /** draw eve cube */
  drawEveCube( &world );


  // create the actual camera, make it follow the camObject and forward its
  // video feed into the viewport.
  // create an actor object which the user can move
  FloatingActor * camObject = new FloatingActor();
  //camObject->setAutoRoll( false );   // use this to disable camera auto-roll
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
  Light * camLight = new Light( &whiteMat, camObject, camLightAtt );
  world.getEnvironment()->addLight( camLight );
  
  
  /* Create a light source into the world. */

  // create an object which defines the location and visual of the light source
  Object lightObject
    ( Object::Params( new InertiaLocator( makeVector3d( 1.5, 2.0, -2.0 ) ),
                      new BasicVisual( sptr( new shapes::Sphere),
                                       &redEmissionMat ),
                      0 ));
  world.addObject( &lightObject );
  
  // create the actual light and make it follow the lightObject
  GLfloat lightatt[3] = { 1.0, 0.0, 0.02 };
  Light light( &whiteMat, &lightObject, lightatt );
  
  // activate the light into the global environment
  world.getEnvironment()->addLight( &light );
  
  
  
  
  // activate the world (ODEWorld requires this before use and currently
  // _after_ construction, but this limitation is going to change)
  world.activate( true );
  
  
  /* Finally, set the timestep length and connect the world to the window's
     ticker to receive a step command on every screen refresh. */

  /* Start the system. */
  world.setDefaultDt( 0.05 );
  window.events.addListener( inputHost );
  window.events.addListener( &world );
  cout << "Entering main loop.." << endl;
  GLOWDevice::MainLoop();
  

  inputHost->terminate();

  delete inputHost;

  return 0;

}
