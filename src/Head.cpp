#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include "Head.hpp"
#include "TIEMaterials.hpp"
using namespace tie;



Head::Head():Object(), Subspace()
{
}

Head::~Head()
{
}

void Head::create(float headSize) 
{
  //set heads geometry
  geom = new shapes::Sphere( headSize );
  
  //Set correct visual needed when standalone mode
/*
  setVisual( sptr( new BasicVisual( sptr( geom ), 
                                    &TIEMat::whiteMat )));
*/
  setLocator( sptr( new ODELocator( makeVector3d( 0.0, 1.0, 0.0)) ));
  
  setGeometry( sptr( new BasicGeometry( sptr( geom ),
                                        TIEMat::testCollMat ) ));
  
  /**
   * Create connectors.
   * All connectors have "up" direction of negative z-axis.
   */
  
  /* Neck connector. Ball connector facing to negative y-axis. */	
  neck = new ODEBallConnector
    ( Connector( *this, Connector::Any,
                 ODELocator
                 ( makeVector3d( 0.0, headSize * -1.1, 0.0),
                   BasisMatrix( makeVector3d( 0.0, -1.0, 0.0),
                                makeVector3d( 0.0, 0.0, -1.0)))));
  
  connectors[CONN_NECK] = sptr( neck );
  
  /* Base connector */
  base = new ODEBallConnector
    ( Connector(*this, Connector::Any, ODELocator(makeVector3d( 0.0, 0.0, 0.0))));
  connectors[CONN_BASE] = sptr( base );
}

