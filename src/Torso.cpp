#include "Torso.hpp"
#include "TIEMaterials.hpp"
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <boost/shared_ptr.hpp>

Torso::Torso():Object(), Subspace()
{
	
}

Torso::~Torso()
{
}


void 
Torso::create(float bodyHeight, float bodyWidth, float bodyThickness)
{
	// Set correct visual
	setVisual( sptr( new BasicVisual( sptr( new shapes::Cube (
		makeVector3d(bodyWidth, bodyHeight, bodyThickness))), &TIEMat::whiteMat )));
		
  setGeometry( sptr( new BasicGeometry( sptr( new shapes::Cube(
    makeVector3d(bodyWidth, bodyHeight, bodyThickness))), 
    sptr( new CollisionMaterial(1.0, 0.1, 0.0 ) ))));
  
	setLocator( sptr( new ODELocator() ));

	/**
	 * Create connectors.
	 * All connectors have "up" direction of negative z-axis.
	 */

	/* Neck connector. Ball connector facing to positive y-axis. */	 
	neck = new ODEBallConnector (
  	Connector( *this, Connector::Any,
  		BasicLocator( makeVector3d( 0.0, bodyHeight / 2.0, 0.0),
  			BasisMatrix(
  				makeVector3d( 0.0, 1.0, 0.0),
  				makeVector3d( 0.0, 0.0, -1.0)))));
	connectors[CONN_NECK] = sptr( neck );

	/* Right shoulder connector. Ball connector facing to positive x-axis. */
	rightShoulder = new ODEBallConnector (
  	Connector( *this, Connector::Any,
  		BasicLocator( makeVector3d( bodyWidth / 2.0, bodyHeight / 2.0, 0.0),
  			BasisMatrix(
  				makeVector3d( 1.0, 0.0, 0.0),
  				makeVector3d( 0.0, 0.0, -1.0)))));
	connectors[CONN_RIGHT_SHOULDER] = sptr( rightShoulder );

	/* Left shoulder connector. Ball connector facing to negative x-axis */
	leftShoulder = new ODEBallConnector (
  	Connector( *this, Connector::Any,
  		BasicLocator( makeVector3d( bodyWidth / -2.0, bodyHeight / 2.0, 0.0),
  			BasisMatrix(
  				makeVector3d( -1.0, 0.0, 0.0),
  				makeVector3d( 0.0, 0.0, -1.0)))));
	connectors[CONN_LEFT_SHOULDER] = sptr( leftShoulder );
	
	/* Base connector. */
	base = new Connector(*this, Connector::Slave, BasicLocator());
	connectors[CONN_BASE] = sptr( base );
}
