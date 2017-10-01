#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include "Arm.hpp"
#include "TIEMaterials.hpp"
#include "CylinderVisual.hpp"
using namespace tie;


Arm::Arm():Object(),  Subspace()
{
}

Arm::~Arm()
{
}

void Arm::create(float length, float size) {
	// Set correct visual
	setVisual( sptr( new BasicVisual( sptr( new CylinderVisual (
    size / 2, size / 2, length, 8)), &TIEMat::whiteMat )));

  setGeometry( sptr( new BasicGeometry( sptr( new shapes::Cube(
    makeVector3d(size, length, size))), 
    sptr( new CollisionMaterial(1.0, 0.1, 0.0 ) ))));
  
	setLocator( sptr( new ODELocator() ));
	
	/**
	 * Create connectors.
	 * All connectors have "up" direction of negative z-axis.
	 */

	/* Wrist connector. Ball connector facing to negative y-axis. */		
	wrist = new ODEBallConnector (
  	Connector( *this, Connector::Any,
  		BasicLocator( makeVector3d( 0.0, length / -2.0, 0.0),
  			BasisMatrix(
  				makeVector3d( 0.0, -1.0, 0.0),
  				makeVector3d( 0.0, 0.0, -1.0)))));
	connectors[CONN_WRIST] = sptr( wrist );

	/* Shoulder connector. Ball connector facing to positive y-axis. */		
	shoulder = new ODEBallConnector (
  	Connector( *this, Connector::Any,
  		BasicLocator( makeVector3d( 0.0, (length + size) / 2.0, 0.0),
  			BasisMatrix(
  				makeVector3d( 0.0, 1.0, 0.0),
  				makeVector3d( 0.0, 0.0, -1.0)))));
	connectors[CONN_SHOULDER] = sptr( shoulder );
	
	/* Base connector */
	base = new Connector(*this, Connector::Slave, BasicLocator());
	connectors[CONN_BASE] = sptr( base );
}

