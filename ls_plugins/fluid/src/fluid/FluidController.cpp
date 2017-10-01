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
 * @file FluidController.cpp
 */



#include "FluidController.hpp"
#include <lifespace/types.hpp>
#include <lifespace/Control/Controller.hpp>
#include <Fluid/Processors.hpp>
#include <Fluid/MessageSource.hpp>
#include <Fluid/InputDevices.hpp>
#include <iostream>
#include <list>
#include <cassert>
using namespace lifespace::plugins::pfluid;
using namespace lifespace;
using namespace Fluid;




FluidController::~FluidController()
{
  if( !network ) {
    std::cout << "Fluid processor network for controller: " << n_name.c_str()
              << " does not exist! (fluidController:dtor)." << std::endl;
  } else {
    network->clear( true );
    network = 0;
  }
}


/**
 * @note
 * defaultdt is not currently used with fluid..
 */
void
FluidController::timestep()
{
  if( !network ) {
    std::cout << "Fluid processor network for controller: " << n_name.c_str()
              << " does not exist! (fluidController:timestep())." << std::endl;
  } else {
    network->update();
  }
}


void 
FluidController::linkActors( const char * nodename, const char * sinkName)
{
  MessageSource * feeder;
  feeder = network->findMessageSource( nodename );
  if( feeder == 0 ) { 
    std::cout << "FluidController " << n_name.c_str() << ": cannot find node: "
              << nodename << ". (linkActors)." << std::endl;
  } else {
    sinkNames.push_back( sinkName );
    feeders.push_back( nodename );
    feeder->msgAddSink( sinkName, nodename, this );
  }
}
