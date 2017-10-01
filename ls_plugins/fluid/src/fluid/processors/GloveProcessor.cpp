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
 * @file GloveProcessor.cpp
 */




#include "GloveProcessor.hpp"
#include "../FluidController.hpp"
#include "FluidMessage.hpp"
#include <lifespace/types.hpp>
#include <Fluid/IONode.hpp>
#include <Fluid/Glove.hpp>
#include <Fluid/Vector3.hpp>
#include <iostream>
using namespace lifespace::plugins::pfluid;
using namespace lifespace;
using namespace Fluid;
using namespace std;



bool
GloveProcessor::start()
{
  std::cout << "Starting processor: " << id().c_str();
  IONode * node = host()->findIONode( feeder );
  input = dynamic_cast< IONodeBuffer < GloveSample > * >(node);
  
  if( !input ) {
    cout << "Sourcenode: " << feeder << " not found. \n"
         << "  LocationVectorProcessor::start(). ";
    return false;
  }

  lastUpdate = input->getLatestTimeStamp();

  return true;
}


bool
GloveProcessor::stop()
{
  std::cout << "Stop processing: " << id().c_str();
  input = 0;
  return true;
}


/**
 * Updates new coordinates
 *
 * @note
 * Currently uses precision floats, may be changed to double
 */
void
GloveProcessor::update()
{
  GloveSample currSample;
  assert( input );

  TimeStamp currTime = input->getLatestTimeStamp();
  if( currTime != lastUpdate ) {
    currSample = input->getIndex( input->getSampleCount() );

    msgSend( id().c_str(), new FluidMessage<GloveSample>( currTime,
                                                          currSample ) );
    
    lastUpdate = currTime;
  }
}

