/**
 * @file SensorController.cpp
 */


#include "SensorController.hpp"


/* Fluid stuff */
#include <lifespace/plugins/fluid.hpp>
#include <Fluid/Message.hpp>
#include <Fluid/TrackerSensorSelector.hpp>
#include <Fluid/Processors.hpp>
using namespace lifespace::plugins::pfluid;
using namespace Fluid;
using std::cout;
using std::endl;

#include <string>


SensorController::SensorController( const std::string sensor ) :
  FluidController( sensor, new Fluid::Processors() )
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


bool
SensorController::msgProcess( const char *id, Message * msg )
{
  
  if( NULL != strstr(id, "/loc") ) {
    

    Vector3 * locVec = ((FluidMessage<Vector3> *)msg)->getContent();
    useControl( 90, 1.0 );
    useControl( FluidController::CTRL_VECTOR_DIM0, locVec->x );
    useControl( FluidController::CTRL_VECTOR_DIM1, locVec->y );
    useControl( FluidController::CTRL_VECTOR_DIM2, locVec->z );
    
    return true;
  } else if( NULL != strstr(id, "/rot") ) {
    Matrix3 * orient = ((FluidMessage<Matrix3> *)msg)->getContent();
    useControl( 100, 1.0 );

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
