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


/* NOTE: contains still just the otacombat 2 functionality! */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


/* main_vrj.cpp */


/* Include Fluid stuff. */

/*
#include "BasicsConfig.hpp"
#include "LeftHand.hpp"
#include "RightHand.hpp"
*/
/*
#include "../minitest/BasicsConfig.hpp"
#include "../minitest/LeftHand.hpp"
#include "../minitest/RightHand.hpp"
*/


/* Include Lifespace. */

#include <lifespace/lifespace.hpp>
using namespace lifespace;


/* Include more Fluid stuff. */

//#include <lifespace/plugins/fluid.hpp>
//#include <Fluid/TrackerSensorSelector.hpp>
//using namespace lifespace::plugins::pfluid;
//using namespace Fluid;


/* Include VR Juggler. */

#include <lifespace/plugins/vrjuggler.hpp>
using namespace lifespace::plugins::pvrjuggler;


/* Include Otacombat 2 files. */

#include "OC2World.hpp"
/*
#include "Animation.hpp"
#include "MaxVisual.hpp"
#include "Meshes/AnimationData.hpp"
#include "Meshes/RightThighVisual.hpp"
#include "Meshes/RightShinVisual.hpp"
#include "Meshes/RightFootVisual.hpp"
#include "Meshes/LeftThighVisual.hpp"
#include "Meshes/LeftShinVisual.hpp"
#include "Meshes/LeftFootVisual.hpp"
#include "Meshes/PowerUnitVisual.hpp"
#include "Meshes/HullVisual.hpp"
#include "Meshes/CannonVisual.hpp"
*/
using namespace otacombat2;


/* Other includes. */

#include <ode/ode.h>
#include <ode/odecpp.h>

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


Object * makeVRJCamera()
{
  Object * camObject = new Object
    ( Object::Params( new BasicLocator( makeVector3d( 0.0, -2.0, 2.0 ))));
  
  // create the actual camera, make it follow the camObject and forward its
  // video feed into the viewport.
  Camera * camera = new Camera();
  camera->setTargetObject( camObject );
  VRJViewport::SetCamera( camera );
  
  return camObject;
}








/* ---- main ---- */


int main( int argc, char * argv[] )
{
  if( argc != 3 ) {
    cout << "Usage: " << argv[0] << " <VR Juggler conf> <FLUID conf>" << endl;
    return 1;
  }
  
  /* Create and initialize fluidhost. */
  //FluidHost * inputHost = new FluidHost( argv[2] );
  
  /* Init VR Juggler. */
  VRJDevice::Init( string(argv[1]) );
  
  /* Make a VR Juggler camera. */
  Object * camObject = makeVRJCamera();
  
  /* Create the world. */
  OC2World world( VRJDevice::instance()->events, camObject );
  
  /* Insert the camera into the world. */
  world.addObject( camObject );
  
  /* Add testprocessor network testcontroller. */
  //inputHost->start();

  
  /* Start the system. */
  world.setDefaultDt( 0.05 );
  VRJDevice::instance()->events.addListener( &world );
  VRJDevice::MainLoop();
  
  
  /* Shut down. */
  //inputHost->terminate();
  //delete inputHost;
  return 0;
}
