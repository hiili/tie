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
 * @file AccelVectorProcessor.cpp
 */




#include "AccelVectorProcessor.hpp"
#include "../FluidController.hpp"
#include "FluidMessage.hpp"
#include <lifespace/types.hpp>
#include <Fluid/IONode.hpp>
#include <Fluid/Vector3.hpp>
#include <iostream>
using namespace lifespace::plugins::pfluid;
using namespace lifespace;
using namespace Fluid;
using namespace std;



bool
AccelVectorProcessor::start()
{
  std::cout << "Starting processor: " << id().c_str();
  IONode * node = host()->findIONode( feeder );
  input = dynamic_cast< IONodeBuffer < Vector3 > * >(node);
  
  if( !input ) {
    cout << "Sourcenode: " << feeder << " not found. \n"
         << "  AccelVectorProcessor::start(). ";
    return false;
  }

  lastDt = 0.0;
  lastUpdate = input->getLatestTimeStamp();
  lastPos = (Vector3 &)input->getIndex(input->timeStampToIndex( lastUpdate ) );
  lastVel = Vector3( 0.0, 0.0, 0.0 );

  return true;
}


bool
AccelVectorProcessor::stop()
{
  std::cout << "Stop processing: " << id().c_str();
  input = 0;
  return true;
}


/**
 * Updates the acceleration vector.
 *
 * @note
 * Currently uses precision floats, may be changed to double
 */
void
AccelVectorProcessor::update()
{
  Vector3 currPos;
  float currDt;
  Vector3 accVector;
  Vector3 velVector;
  assert( input );

  TimeStamp currTime = input->getLatestTimeStamp();
  if( currTime != lastUpdate ) {
    currDt = (currTime - lastUpdate).secondsF();
    currPos = input->getIndex( input->getSampleCount() );
    
    velVector = (currPos - lastPos) / currDt;
    accVector = (velVector - lastVel) / (0.5 * lastDt + 0.5 * currDt);
    
    
    cout << "Update(), dt: " << currDt << " inputdata: ( "
         << currPos.x << " , " << currPos.y << " , " << currPos.z << " ), "
         << "accelVec: ( "
         << accVector.x << " , " << accVector.y << " , " << accVector.z
         << " )." << endl;
    msgSend( id().c_str(), new FluidMessage<Vector3>( currTime, accVector ) );
    
    lastDt = currDt;  // note order.
    lastUpdate = currTime;                      // note order.
    lastPos = currPos;
    lastVel = velVector;
  }
}

