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
 * @file GloveController.cpp
 */


#include "GloveController.hpp"


/* Fluid stuff */
#include <lifespace/plugins/fluid.hpp>
#include <Fluid/Message.hpp>
#include <Fluid/Glove.hpp>
#include <Fluid/TrackerSensorSelector.hpp>
#include <Fluid/Processors.hpp>
using namespace lifespace::plugins::pfluid;
using namespace Fluid;
using namespace tie;
using std::cout;
using std::endl;

#include <string>


GloveController::GloveController( const std::string gloveName ) :
  FluidController( gloveName, new Fluid::Processors() )
{
  /* Create processors */

  glove =
    new GloveProcessor( network,
                        gloveName + "/flexions",
                        "../inputdevices/" + gloveName + "/output" );   
  
   /* Connect processors */
  linkActors( (gloveName + "/flexions").c_str(),
              (gloveName + "/flexions").c_str() );
  
  start();

  fist=false;
  fistOpenThreshold = 2.5; // default
  fistCloseThreshold = 3.5; // default
}


void
GloveController::setThresholds( float close, float open )
{
  if( close >= 0.0 && close <= 5.0 &&
      open >= 0.0 && open <= 5.0 ) {
    fistOpenThreshold = open;
    fistCloseThreshold = close;
  } else
    cout << "GloveController::setThresholds(): Illegal arguments.\n";
}


bool
GloveController::msgProcess( const char *id, Message * msg )
{
  if( NULL != strstr(id, "/flexions") ) {
    
    GloveSample * sample = ((FluidMessage<GloveSample> *)msg)->getContent();
    float cumSum = 0.0;
    
    // set finger flexions
    for( int i=Glove::FINGER_INDEX; i<Glove::FINGERS; i++ )
      useControl( CTRL_SET_THUMB+i, sample->getFingerJoint( i, Glove::FINGERJOINT_SECOND ) );

    // calculate fist
    for( int i=(int)Glove::FINGER_THUMB; i<Glove::FINGERS; i++ )
      cumSum += sample->getFingerJoint( i, Glove::FINGERJOINT_SECOND );    
    
    // set fist value
    if( cumSum < fistOpenThreshold && fist ) {
      useControl( CTRL_SET_FIST, 0.0 );
      fist = false;
    }
    if( cumSum > fistCloseThreshold && !fist ) {
      useControl( CTRL_SET_FIST, 1.0 );
      fist = true;
    }

    return true;
  } else {
    // add new controls here..
  }
  return false; // we should never get here..
}
