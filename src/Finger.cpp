#include "Finger.hpp"
#include "TIEMaterials.hpp"
#include "CylinderVisual.hpp"
#include "types.hpp"
using namespace tie;

#include <lifespace/lifespace.hpp>

using namespace lifespace;


Finger::Finger(bool a_isThumb, enum Sides a_side): 
  Object(Object::Params( new ODELocator(), 0, 0)), 
  Subspace(), isThumb(a_isThumb), side(a_side)
{	
}

Finger::~Finger()
{	
}

void Finger::create(float length, float size)
{
  //Length of stub part
  float stubLength = length * 0.5;
  if(stubLength < size)
    stubLength = size;
	
  //Length of tip part
  float tipLength = length * 0.5;
  if(tipLength < size)
    tipLength = size;
		
	/* 
	 * Create the stub part of the finger.
	 */
	
	// Set correct visual for stub part
	/*setVisual( sptr( new BasicVisual( sptr( new CylinderVisual (
		size / 2, size / 2, stubLength, 8)), &TIEMat::redMat )));*/
    
  setVisual( sptr( new BasicVisual
    ( shapes::Cube::create( makeVector3d( size, stubLength, size )), 
    &TIEMat::redMat )));    

  setGeometry( sptr( new BasicGeometry( shapes::Cube::create(
    makeVector3d(size , stubLength, size )), 
    sptr( new CollisionMaterial(1.0, 0.1, 0.0 ) ))));


	/**
	 * Create connectors for stub part.
	 * All connectors have "up" direction of negative z-axis.
	 */

  /* Thumb hand connector axle points to different direction from the rest */
  
  if( !isThumb ) {
    /* Normal hand connector. Axle connector facing to positive x-axis. */		
    hand = new ODEAxleConnector
      ( Connector( *this, Connector::Any,
                   BasicLocator( makeVector3d( 0.0, stubLength / 2.0, 0.0),
                                 BasisMatrix(
                                             makeVector3d( 1.0, 0.0, 0.0),
                                             makeVector3d( 0.0, -1.0, 0.0)))),
        10, -INFINITY, INFINITY, 0.0);
  }
  else if( side == LEFT) {
    /* Left thumb hand connector. Axle connector facing to negative z-axis. */    
    hand = new ODEAxleConnector
      ( Connector( *this, Connector::Any,
                   BasicLocator( makeVector3d( 0.0, stubLength / 2.0, 0.0),
                                 BasisMatrix(
                                             makeVector3d( 0.0, 0.0, -1.0),
                                             makeVector3d( 0.0, -1.0, 0.0)))),
        10, -INFINITY, INFINITY, 0.0);
  }
  else {
    /* Right thumb hand connector. Axle connector facing to positive z-axis. */    
    hand = new ODEAxleConnector
      ( Connector( *this, Connector::Any,
                   BasicLocator( makeVector3d( 0.0, stubLength / 2.0, 0.0),
                                 BasisMatrix(
                                             makeVector3d( 0.0, 0.0, 1.0),
                                             makeVector3d( 0.0, -1.0, 0.0)))),
        10, -INFINITY, INFINITY, 0.0);
  }
  
  connectors[CONN_HAND] = sptr( hand );
	
	/* Tip connector. Ball connector facing to negative y-axis. */		
	/*stub_to_tip = new ODEBallConnector (
  	Connector( *this, Connector::Any,
  		BasicLocator( makeVector3d( 0.0, stubLength / 2.0, 0.0),
  			BasisMatrix(
  				makeVector3d( 0.0, 1.0, 0.0),
  				makeVector3d( 0.0, 0.0, -1.0)))));*/

	/* Tip connector. Axle connector facing to positive x-axis. */		
	stub_to_tip = new ODEAxleConnector (
  	Connector( *this, Connector::Any,
  		BasicLocator( makeVector3d( 0.0, stubLength / -2.0, 0.0),
  			BasisMatrix(
  				makeVector3d( -1.0, 0.0, 0.0),
  				makeVector3d( 0.0, -1.0, 0.0)))),
  	10, 0.0, M_PI / 2, 0.0);

	/* Base connector */
	stubBase = new Connector(*this, Connector::Slave, BasicLocator());
	connectors[CONN_STUB_BASE] = sptr( stubBase );
	
	/*
	 * End of stub part of the finger.
	 */
	 
	 
  /*
   * Create the tip part of the finger.
   */
	 
	tip = new Object(Object::Params( new ODELocator(makeVector3d(
    0.0, length, 0.0 )), 0, 0));
	
  tip->setVisual( sptr( new BasicVisual
    ( shapes::Cube::create( makeVector3d( size, tipLength, size )), 
    &TIEMat::groundMat )));    

  tip->setGeometry( sptr( new BasicGeometry( shapes::Cube::create(
    makeVector3d(size , tipLength, size )), 
    sptr( new CollisionMaterial(1.0, 0.1, 0.0 ) ))));
	
  // Add tip to finger subspace
  addObject(tip);
  
  /**
   * Create connectors for tip part.
   * All connectors have "up" direction of negative z-axis.
   */

  /* Stub connector. Axle connector facing to positive x-axis. */		
  tip_to_stub =
    new ODEAxleConnector
    ( Connector( *tip, Connector::Any,
                 BasicLocator( makeVector3d( 0.0, tipLength / 2.0, 0.0 ),
                               BasisMatrix(
                                           makeVector3d( 1.0, 0.0, 0.0 ),
                                           makeVector3d( 0.0, -1.0, 0.0 )))),
      10.0, -INFINITY, INFINITY, 0.0 );
  
  /*tip_to_stub = new ODEBallConnector (
    Connector( *tip, Connector::Any,
    BasicLocator( makeVector3d( 0.0, tipLength / -2.0, 0.0),
    BasisMatrix(
    makeVector3d( 0.0, -1.0, 0.0),
    makeVector3d( 0.0, 0.0, -1.0)))));*/
	
  /* Base connector */
  tipBase = new Connector(*tip, Connector::Slave, BasicLocator());
  connectors[CONN_TIP_BASE] = sptr( tipBase );
	
  /*
   * End of tip part of the finger.
   */
}

void Finger::assemble()
{
  stub_to_tip->connect(*tip_to_stub, Connector::AlignSlave);
  stub_to_tip->setParam( dParamERP, 0.3 );
  stub_to_tip->setParam( dParamCFM, 0.1 );

}
