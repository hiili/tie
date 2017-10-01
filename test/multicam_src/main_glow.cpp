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
 * Initialization stuff for running the test through GLOW. This version
 * includes functionality for switching between multiple world cameras
 * (keyboard keys 1-9) and an own debug camera (key 0).
 */


/* Include Lifespace and plugins. */

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <lifespace/plugins/glow.hpp>
using namespace lifespace::plugins::pglow;


/* Include test-specific files. */

#include "TestWorld.hpp"
using namespace tie;


/* Other includes. */

#include <ode/ode.h>
#include <ode/odecpp.h>

#include <glow/glow.h>

#include <GL/gl.h>

#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

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




static const GLfloat none[4]     = { 0.0, 0.0, 0.0, 0.0 };
static const GLfloat white[4]    = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat smooth[1]   = { 1.0 };
static const GLfloat polished[1] = { 40.0 };
static const Material whiteMat( white, white, white,
                                none, smooth, GL_FRONT );
static const GLfloat camLightAtt[3] = { 1.0, 0.0, 0.1 };


static TestWorld::CameraEntry createGLOWCamera( World & world )
{
  // create an actor object which the user can move
  FloatingActor * camObject = new FloatingActor();
  //camObject->setAutoRoll( false );   // use this to disable camera auto-roll
  world.addObject( camObject );
  
  // attach a light source to the camera object
  Light * light = new Light( &whiteMat, camObject, camLightAtt );
  world.getEnvironment()->addLight( light );
  
  
  // create the actual camera, make it follow the camObject and forward its
  // video feed into the viewport.
  boost::shared_ptr<Camera> camera( new Camera() );
  camera->setTargetObject( camObject );
  
  // create a keymap for the camera
  boost::shared_ptr<Controller::ControlMap> camKeymap
    ( new Controller::ControlMap );
  fillCameraKeymap( *camKeymap );
  
  
  // combine the camera, actor and the keymap and return the result
  TestWorld::CameraEntry glowCam = { camera, camObject, camKeymap };
  return glowCam;
}








/* ---- classes ---- */


/**
 * Extended to allow switching between multiple cameras. Deleting the active
 * world camera will most probably cause a crash!
 */
class GLOWViewportEx :
  public GLOWViewport
{
  TestWorld::cameras_t * worldCameras;
  TestWorld::CameraEntry glowCamera;
  TestWorld::CameraEntry * currentCamera;
  
  void activateCamera( unsigned int num )
  {
    // deactivate the old camera
    if( currentCamera ) {
      assert( currentCamera->camera && currentCamera->actor );
      GLOWViewport::removeActor( currentCamera->actor );
      GLOWViewport::setCamera( 0 );
      currentCamera = 0;
    }
    
    // select the new current camera
    if( num == 0 ) {
      // glow camera
      currentCamera = &glowCamera;
    } else {
      // one of the world cameras
      if( num - 1 < worldCameras->size() ) {
        currentCamera = &worldCameras->at( num - 1 );
      }
    }
    
    // activate the new current camera
    if( currentCamera ) {
      assert( currentCamera->camera && currentCamera->actor );
      GLOWViewport::addActor( currentCamera->actor,
                              currentCamera->controlMap.get() );
      GLOWViewport::setCamera( currentCamera->camera.get() );
    }
  }
  
public:
  GLOWViewportEx( GLOWDevice & parentDevice ) :
    GLOWViewport( parentDevice ),
    currentCamera( 0 )
  {}
  
  virtual void OnKeyboard( glow::Glow::KeyCode key, int x, int y,
                           glow::Glow::Modifiers modifiers )
  {
    if( key >= '0' && key <= '9' ) {
      activateCamera( key - '0' );
    } else GLOWViewport::OnKeyboard( key, x, y, modifiers );
  }
  
  void setWorldCameras( TestWorld::cameras_t * cameras )
  { worldCameras = cameras; }
  
  void setGlowCamera( const TestWorld::CameraEntry & camera )
  {
    glowCamera = camera;
    activateCamera( 0 );
  }
};








/* ---- main ---- */


int main( int argc, char * argv[] )
{
  /* Init graphics. */
  GLOWDevice::Init( argc, argv );
  GLOWDevice window;
  GLOWViewportEx viewport( window );
  
  
  /* Create the world. */
  TestWorld world( window.events );
  
  /* Add the world cameras and add a GLOW-controlled camera (with a light
     source) into the world and to the camera switching viewport. */
  viewport.setWorldCameras( world.getCameras() );
  viewport.setGlowCamera( createGLOWCamera( world ) );
  
  
  /* Start the system. */
  world.setDefaultDt( 0.05 );
  window.events.addListener( &world );
  GLOWDevice::MainLoop();
  
  /* This is never reached when using GLOW. */
  return 0;
}
