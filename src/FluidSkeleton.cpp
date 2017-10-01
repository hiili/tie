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
 * @file FluidSkeleton.hpp
 */


#include "FluidSkeleton.hpp"
#include "ODESkeleton.hpp"
#include "Skeleton.hpp"

using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <iostream>
using std::cout;
using std::endl;





void
FluidSkeleton::init( ODESkeleton * odeSkel )
{
#ifdef DEBUG_AVATAR
  std::cout << "FluidSkeleton init start\n";
#endif

  odeSkeleton = odeSkel;

  assert( baseNode != 0 );  // basenode must exist!!
    
  // add controlnodes to base (if they exist)
  
  if( headControl != 0 )
    baseNode->addObject( headControl );
  
  if( leftHandControl != 0 )
    baseNode->addObject( leftHandControl );
  
  if( rightHandControl != 0 )
    baseNode->addObject( rightHandControl );
  

#ifdef DEBUG_AVATAR
  std::cout << "Fluidskeleton init finishing\n";
#endif
}


void
FluidSkeleton::prepare( real dt )
{
#ifdef DEBUG_AVATAR
  cout << "FluidSkeleton: prepare() starting\n";
#endif  

  assert( baseNode != 0 );  // no point continuing, if we don't have this


  // if target exists and is connected, disconnect connector before moving
  if( headControl != 0 ) {
    if( baseNode->getConnector( CONN_HEAD_BASE ).isConnected() )
      baseNode->getConnector( CONN_HEAD_BASE ).disconnect();
    
    //move and rotate connector according to fluidcontrolled skeletal object..
    baseNode->getConnector( CONN_HEAD_BASE ).setLoc
      ( headControl->getLocator()->getLoc() );
    baseNode->getConnector( CONN_HEAD_BASE ).setBasis
      ( headControl->getLocator()->getBasis() );
  }
  
  // if target exists, disconnect connector before moving
  if( leftHandControl != 0 ) {
    if( baseNode->getConnector( CONN_LEFT_HAND ).isConnected() )
      baseNode->getConnector( CONN_LEFT_HAND ).disconnect();
    
    //move and rotate connector according to fluidcontrolled skeletal object..
    baseNode->getConnector( CONN_LEFT_HAND ).setLoc
      ( leftHandControl->getLocator()->getLoc() );
    baseNode->getConnector( CONN_LEFT_HAND ).setBasis
      ( leftHandControl->getLocator()->getBasis() );  
  }

  // if target exists, disconnect connector before moving
  if( rightHandControl != 0 ) {
    if( baseNode->getConnector( CONN_RIGHT_HAND ).isConnected() )
      baseNode->getConnector( CONN_RIGHT_HAND ).disconnect();
    
    //move and rotate connector according to fluidcontrolled skeletal object..
    baseNode->getConnector( CONN_RIGHT_HAND ).setLoc
      ( rightHandControl->getLocator()->getLoc() );
    baseNode->getConnector( CONN_RIGHT_HAND ).setBasis
      ( rightHandControl->getLocator()->getBasis() );
  }

#ifdef DEBUG_AVATAR  
  cout << "FluidSkeleton: prepare() finished calculations - calling connect\n";
#endif

  // reconnect all broken joints
  odeSkeleton->connect( this );
  
#ifdef DEBUG_AVATAR
  cout << "FluidSkeleton: prepare() finishing\n";
#endif
}


// empty metod now
void
FluidSkeleton::step()
{}
