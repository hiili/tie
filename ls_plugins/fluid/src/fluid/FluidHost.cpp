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
 * @file FluidHost.cpp
 */


#include "FluidHost.hpp"
#include "FluidController.hpp"
#include <Fluid/Config.hpp>
#include <Fluid/InputDevices.hpp>
#include <Fluid/VrjTrackerExport.hpp>
#include <lifespace/Structures/Event.hpp>
#include <lifespace/Graphics/types.hpp>
#include <list>
#include <cassert>
#include <iostream>
using namespace lifespace::plugins::pfluid;



FluidHost::~FluidHost()
{
  assert( config_file.empty() );
  terminate();
}



void
FluidHost::init()
{
#ifdef DEBUG_FLUID
  std::cout << "FludHost; Initializing fluid application from file: "
       << config_file << std::endl;
#endif
  Fluid::DeviceConfig::init( config_file.c_str() );
  Fluid::InputDevices::init();
  Fluid::VrjTrackerExport::configInit();

}


/* Terminates controllers when application wants to quit. This method is called
   also from destructor if user hasn't manually called it earlier */
void
FluidHost::terminate()
{
#ifdef DEBUG_FLUID
  std::cout << "FluidHost: Terminating fluid application" << std::endl;
#endif
  std::list< FluidController * >::iterator iter;
  for( iter = controllers.begin() ; iter != controllers.end() ; iter ++ ) {
    delete (*iter);
  }
  controllers.clear();
  assert( controllers.empty() );   // Just to be on the safe side.

  /* Stop fluid input processing. note order, as processors need to stop first
     and controllers cleared in order to avoid mistakes in multi-threaded
     mode */

  Fluid::InputDevices::stop();   

  Fluid::InputDevices::cleanup();
  Fluid::DeviceConfig::cleanup();
}

void
FluidHost::start()
{
  Fluid::InputDevices::start();
}


void
FluidHost::addController( FluidController * contr )
{

  if( contr != 0 ) {

#ifdef DEBUG_FLUID
    std::cout << "FluidHost: adding controller " << (contr->getName())->c_str()
              << " in host." << std::endl;
#endif

    controllers.push_back( contr );
  } else {
    std::cout << "FluidHost: Controller empty, when tried to add."
              << std::endl;
  }
}

/* Process the graphics events */
void
FluidHost::processEvent( const lifespace::GraphicsEvent * event )
{
#ifdef DEBUG_FLUID
  std::cout << "FluidHost: Handling graphics event.." << std::endl;
#endif

  std::list< FluidController * >::iterator iter = controllers.begin();
  
  switch( event->id ) {
  case GE_VSYNC:

#ifdef DEBUG_FLUID
    std::cout << "Timestep command received, calling update: "
              << (*iter) << std::endl;
#endif

    Fluid::InputDevices::update();

    for( ; iter != controllers.end() ; iter ++ ) {
      (*iter)->timestep();
    }
    break;
  }

}
