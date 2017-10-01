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
 * @file main_vrj.cpp
 *
 * Initialization stuff for running the test through GLOW. This version
 * includes functionality for switching between multiple world cameras
 * (keyboard keys 1-9) and an own debug camera (key 0).
 */


/* Include Lifespace and plugins. */

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <di_data_buffer.h>
#include <lifespace/plugins/fluid.hpp>
using namespace lifespace::plugins::pfluid;

#include <lifespace/plugins/vrjuggler.hpp>
using namespace lifespace::plugins::pvrjuggler;

/* Include test-specific files. */
#include <tie/tie.hpp>

#include "IMWorldVRJ.hpp"
using namespace tie;


/* Other includes. */

#include <ode/ode.h>
#include <ode/odecpp.h>

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

Avatar * makeVRJCamera( FluidHost * inputHost )
{
  // create a fluid controlled avatar and add a visual to it
  Avatar * fluidAvatar = new Avatar
    ( Object::Params
      ( new ODELocator( makeVector3d( 0.0, 2.0, 0.0 ), BasisMatrix(),
                        AVATAR_MASS,
                        2.5,
                        0.1, 0.1, 2.0,
                        0.1, 0.1, 2.0),
        new BasicVisual( shapes::Cube::create
                         ( makeVector3d( 3.0, 0.5, 3.0 )),
                         &TIEMat::whiteMat ),
        new BasicGeometry( shapes::Cube::create
                           ( makeVector3d( 3.0, 0.5, 3.0 )),
                           TIEMat::testCollMat1 )));
  

  // create the actual camera and make it follow the camObject
  Camera * camera1 = new Camera();
  camera1->setTargetObject( fluidAvatar );
  
  VRJViewport::SetCamera( camera1 );
  
  return fluidAvatar;
}

/* ---- main ---- */


int main( int argc, char * argv[] )
{
  if( argc != 3 ) {
    cout << "Usage: ./imworld_vrj <fluid config file> <juggler config file>\n";
    exit(1);
  }

  /* init fluid */
  FluidHost * inputHost = new FluidHost( argv[1] );
  cout << "Fluid initialized.\n";

  /* Init graphics. */
  VRJDevice::Init( string(argv[2]) );
  
  /* Enable automatic generation of display lists. */
  VRJDevice::instance()->setAutoDisplaylisting( true );
  
  
  Avatar * camObj = makeVRJCamera( inputHost );
  
  /* init user interface */
  UserInterface ui = UserInterface( camObj, inputHost, GRAVITY, AVATAR_MASS );

  /* Create the world. */
  IMWorld world( VRJDevice::instance()->events, inputHost, camObj, &ui );  
    
  /* Start the system. */
  world.setDefaultDt( 0.05 );
  VRJDevice::instance()->events.addListener( inputHost );
  VRJDevice::instance()->events.addListener( &world );
  cout << "Entering main loop.. \n ";
  VRJDevice::MainLoop();
  

  inputHost->terminate();
  delete inputHost;
  return 0;
}
