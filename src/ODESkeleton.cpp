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
 * @file ODESkeleton.cpp
 */


#include "ODESkeleton.hpp"
#include "FluidSkeleton.hpp"
#include "Skeleton.hpp"
#include "Head.hpp"
#include "ODEHand.hpp"

using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <iostream>
using std::cout;
using std::endl;

/**
 * Internal method for connecting avatar's skeletal structure
 */
void
ODESkeleton::constructSkeleton()
{



}


ODEHand *
ODESkeleton::getODEHand( bool isLeft )
{ return ( isLeft ? leftHand : rightHand ); }




/**
 * Connects the tie avatar model to physical representation in lifespace.
 *
 * @param master Pointer to master skeletal hierachy according which
 *               ODESkeleton is aligned.
 *
 * @note Objects to connect are hardcoded for tie's particular avatar model.
 */
void
ODESkeleton::connect( Skeleton * master )
{
#ifdef DEBUG_AVATAR
  cout << "ODESkeleton::connect(): start\n";
#endif
  

  //connect head
  if( head ) {
    master->getBaseNode()->getConnector(FluidSkeleton::CONN_HEAD_BASE).connect
      ( head->getConnector( Head::CONN_BASE ), Connector::DontAlign );
    ((ODEFixedConnector &)master->getBaseNode()->getConnector
     (FluidSkeleton::CONN_HEAD_BASE)).setParam( dParamERP, 0.5 );
    ((ODEFixedConnector &)master->getBaseNode()->getConnector
     (FluidSkeleton::CONN_HEAD_BASE)).setParam( dParamCFM, 0.001 );
  }
  
  //connect left hand
  if( leftHand ) {
    master->getBaseNode()->getConnector(FluidSkeleton::CONN_LEFT_HAND).connect
      ( leftHand->getConnector( ODEHand::CONN_BASE ), Connector::DontAlign );
    ((ODEFixedConnector &)master->getBaseNode()->getConnector
     (FluidSkeleton::CONN_LEFT_HAND)).setParam( dParamERP, 0.9 );    // 0.5
    ((ODEFixedConnector &)master->getBaseNode()->getConnector
     (FluidSkeleton::CONN_LEFT_HAND)).setParam( dParamCFM, 0.1 );    // 0.001
  }
  
  //connect right hand
  if( rightHand ) {
    master->getBaseNode()->getConnector(FluidSkeleton::CONN_RIGHT_HAND).connect
      ( rightHand->getConnector( ODEHand::CONN_BASE ), Connector::DontAlign );
    ((ODEFixedConnector &)master->getBaseNode()->getConnector
     (FluidSkeleton::CONN_RIGHT_HAND)).setParam( dParamERP, 0.9 ); // 0.5
    ((ODEFixedConnector &)master->getBaseNode()->getConnector
     (FluidSkeleton::CONN_RIGHT_HAND)).setParam( dParamCFM, 0.1 ); // 0.001
  }  

#ifdef DEBUG_AVATAR
  cout << "ODESkeleton::connect(): finishing\n";
#endif
}
