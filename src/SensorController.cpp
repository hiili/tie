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
 * @file SensorController.cpp
 */


#include "SensorController.hpp"


/* Fluid stuff */
#include <lifespace/plugins/fluid.hpp>
#include <Fluid/Message.hpp>
#include <Fluid/TrackerSensorSelector.hpp>
#include <Fluid/Processors.hpp>
#include <Fluid/Matrix3.hpp>
#include <lifespace/lifespace.hpp>
using namespace lifespace::plugins::pfluid;
using namespace Fluid;
using namespace tie;
using namespace lifespace;
using std::cout;
using std::endl;

#include <string>


SensorController::SensorController( const std::string sensor ) :
  FluidController( sensor, new Fluid::Processors() ),
  relTranslation( Vector3(0.0,0.0,0.0) ),
  isTranslated( false )
{
  /* Create processors */
  source =
    new TrackerSensorSelector( network,
                               sensor,
                               "tracker",
                               sensor );


  loc =
    new LocationVectorProcessor( network,
                                 sensor + "/loc",
                                 sensor + "/location" );
  
  rot =
    new RotationMatrixProcessor( network,
                                 sensor + "/rot",
                                 sensor + "/rotation" );
  
  
  
  /* Connect processors */
  linkActors( (sensor + "/loc").c_str(),
              (sensor + "/loc").c_str() );
  
  linkActors( (sensor + "/rot").c_str(),
              (sensor + "/rot").c_str() );
  
  start();
  
}

void
SensorController::setLocTranslationFluid( Vector trans )
{
  loc->setTranslationAbs( trans(DIM_X), trans(DIM_Y), trans(DIM_Z) );
}

void
SensorController::setLocTranslationRel( const Vector trans )
{
  relTranslation = Vector3( trans(DIM_X), trans(DIM_Y), trans(DIM_Z));
  isTranslated = true;
}


bool
SensorController::msgProcess( const char *id, Message * msg )
{
  
  if( NULL != strstr(id, "/loc") ) {
    
    Vector3 * locVec = ((FluidMessage<Vector3> *)msg)->getContent();
    
    if( isTranslated && currRot != 0 ) {
      // calculate scaled base vectors
      Vector3 transAbs = *currRot * relTranslation;
      
      // transform and pass coordinates forward
      useControl( CTRL_SETLOC, 1.0 );
      useControl( FluidController::CTRL_VECTOR_DIM0,
                  locVec->x + transAbs[0] );
      useControl( FluidController::CTRL_VECTOR_DIM1,
                  locVec->y + transAbs[1] );
      useControl( FluidController::CTRL_VECTOR_DIM2,
                  locVec->z + transAbs[2] );
    } else {
      useControl( CTRL_SETLOC, 1.0 );
      useControl( FluidController::CTRL_VECTOR_DIM0, locVec->x );
      useControl( FluidController::CTRL_VECTOR_DIM1, locVec->y );
      useControl( FluidController::CTRL_VECTOR_DIM2, locVec->z );
    }    

    return true;
  } else if( NULL != strstr(id, "/rot") ) {
    Matrix3 * orient = ((FluidMessage<Matrix3> *)msg)->getContent();
    currRot = orient;
    
    useControl( CTRL_SETBASIS, 1.0 );
    // Pass the matrix (column-major) forward.
    for( int i=0; i<3; i++ )
      for( int j=0; j<3; j++ )
        useControl( CTRL_MATRIX_0_0+(i*3+j), orient->get( j, i ) );
    
    return true;
  } else {
    // add new controls here..
  }
  return false; // we should never get here..
}
