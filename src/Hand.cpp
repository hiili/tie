#include <lifespace/lifespace.hpp>
using namespace lifespace;
#include "Hand.hpp"
#include "Finger.hpp"
#include "TIEMaterials.hpp"
using namespace tie;

Hand::Hand():Object(Object::Params(new ODELocator, 0, 0))
{
}

Hand::~Hand()
{
}

void 
Hand::create( float width, float thickness, enum Sides side,
              Subspace * superspace)
{
  float face;
  if(side == LEFT)
    face = 1.0;
  else
    face = -1.0;
		
  float length = width * 0.89;
  // Set correct visual
  if(side == LEFT)
    setVisual
      ( sptr( new BasicVisual( sptr
                               ( new shapes::Cube
                                 ( makeVector3d(width, length, thickness))),
                               &TIEMat::whiteMat )));
  else
    setVisual
      ( sptr( new BasicVisual( sptr
                               ( new shapes::Cube
                                 ( makeVector3d(width, length, thickness))),
                               &TIEMat::groundMat )));
  
  setGeometry
    ( sptr( new BasicGeometry( sptr
                               ( new shapes::Cube
                                 ( makeVector3d(width, length, thickness))), 
                               sptr
                               ( new CollisionMaterial(1.0, 0.1, 0.0 ) ))));
  /**
   * Create connectors.
   * All connectors have "up" direction of negative z-axis.
   */

  /* Wrist connector. Ball connector facing to positive y-axis. */
  wrist = new ODEBallConnector
    ( Connector( *this, Connector::Any,
                 BasicLocator( makeVector3d( 0.0, length / -2.0, 0.0),
                               BasisMatrix( makeVector3d( 0.0, -1.0, 0.0),
                                            makeVector3d( 0.0, 0.0, -1.0)))));
  connectors[CONN_WRIST] = sptr( wrist );
	
  /* 
   * Thumb connector. Axleconnector facing either positive x-axis (left hand)
   * or negative x-axis (right hand).
   */
  /*
  thumbConnector = new ODEAxleConnector
    ( Connector( *this, Connector::Any,
                 BasicLocator( makeVector3d( width / 2.0 * face,
                                             length / -2.0 + thickness / 2.0,
                                             0.0),
                               BasisMatrix( makeVector3d( -1.0, face, 0.0),
                                            makeVector3d( face, 0.5, -0.5)))),
      10.0, 0, M_PI / 2, 0.0 );
  
  connectors[CONN_THUMB] = sptr( thumbConnector );

  indexConnector = new ODEAxleConnector
    ( Connector( *this, Connector::Master,
                 BasicLocator( makeVector3d( 3.0 * width / 8.0 * face,
                                             length / 2.0,
                                             0.0),
                               BasisMatrix( makeVector3d( -1.0, 0.0, 0.0),
                                            makeVector3d( 0.0, 1.0, 0.0)))),
      10.0, 0, M_PI / 2, 0.0 );
	
  connectors[CONN_INDEX] = sptr( indexConnector );	

  middleConnector = new ODEAxleConnector
    ( Connector( *this, Connector::Master,
                 BasicLocator( makeVector3d( width / 8.0 * face,
                                             length / 2.0,
                                             0.0),
                               BasisMatrix( makeVector3d( -1.0, 0.0, 0.0),
                                            makeVector3d( 0.0, 1.0, 0.0)))),
      10.0, 0, M_PI / 2, 0.0 );
  
  connectors[CONN_MIDDLE] = sptr( middleConnector );

  ringConnector = new ODEAxleConnector
    ( Connector( *this, Connector::Master,
                 BasicLocator( makeVector3d( width / -8.0 * face,
                                             length / 2.0,
                                             0.0),
                               BasisMatrix( makeVector3d( -1.0, 0.0, 0.0),
                                            makeVector3d( 0.0, 1.0, 0.0)))),
      10.0, 0, M_PI / 2, 0.0 );
  
  connectors[CONN_RING] = sptr( ringConnector );		

  pinkyConnector = new ODEAxleConnector
    ( Connector( *this, Connector::Master,
                 BasicLocator( makeVector3d( 3.0 * width / -8.0 * face,
                                             length / 2.0,
                                             0.0),
                               BasisMatrix( makeVector3d( -1.0, 0.0, 0.0),
                                            makeVector3d( 0.0, 1.0, 0.0)))),
      10.0, 0, M_PI / 2, 0.0 );
  
  connectors[CONN_PINKY] = sptr( pinkyConnector );		
 */

  /* Base connector */
  base = new ODEBallConnector
    ( Connector
      (*this, Connector::Any,
       BasicLocator( makeVector3d( 0.0, 0.0, 0.0),
                     BasisMatrix( makeVector3d( 0.0, 0.0, -1.0),
                                  makeVector3d( 0.0, -face, 0.0 ) ))));

     connectors[CONN_BASE] = sptr( base );
     
  /*
   * Create fingers.
   */
     /*
       if(side == LEFT)  
    thumb = new Finger( new ODELocator( makeVector3d(), BasisMatrix(),
                                        0.1, 0.1 ), 
                        true, thumb->LEFT);
  else
    thumb = new Finger( new ODELocator( makeVector3d(), BasisMatrix(),
                                        0.1, 0.1 ),
                        true, thumb->RIGHT);
  index = new Finger( new ODELocator( makeVector3d(), BasisMatrix(),
                                      0.1, 0.1 ));
  middle = new Finger( new ODELocator( makeVector3d(), BasisMatrix(),
                                       0.1, 0.1 ));
  ring = new Finger( new ODELocator( makeVector3d(), BasisMatrix(),
                                     0.1, 0.1 ));
  pinky = new Finger( new ODELocator( makeVector3d(), BasisMatrix(),
                                      0.1, 0.1 ));
  
  thumb->create( 0.74 * width, 0.225 * width );
  middle->create( 1.05 * width, 0.225 * width );
  index->create( 0.89 * width, 0.225 * width );
  ring->create( 0.95 * width, 0.225 * width );
  pinky->create( 0.63 * width, 0.225 * width );
  
  //Using this tips won't be connected correctly
  this->addObject(thumb);
  this->addObject(index);
  this->addObject(middle);
  this->addObject(ring);
  this->addObject(pinky);
   
  //Using this tips will be connected correctly  
  superspace->addObject(thumb);
  superspace->addObject(index);
  superspace->addObject(middle);
  superspace->addObject(ring);
  superspace->addObject(pinky);
  */
}

void Hand::assemble()
{
  /*
  thumbConnector->
    connect( thumb->getConnector( thumb->CONN_HAND ), Connector::AlignSlave);
  ((ODEAxleConnector *)thumbConnector)->setParam( dParamERP, 0.3 );
  ((ODEAxleConnector *)thumbConnector)->setParam( dParamCFM, 0.1 );

  indexConnector->
    connect( index->getConnector( index->CONN_HAND ), Connector::AlignSlave);
  ((ODEAxleConnector *)indexConnector)->setParam( dParamERP, 0.3 );
  ((ODEAxleConnector *)indexConnector)->setParam( dParamCFM, 0.1 );

  middleConnector->
    connect( middle->getConnector( middle->CONN_HAND ), Connector::AlignSlave);
  ((ODEAxleConnector *)middleConnector)->setParam( dParamERP, 0.3 );
  ((ODEAxleConnector *)middleConnector)->setParam( dParamCFM, 0.1 );

  ringConnector->
    connect( ring->getConnector( ring->CONN_HAND ), Connector::AlignSlave);
  ((ODEAxleConnector *)ringConnector)->setParam( dParamERP, 0.3 );
  ((ODEAxleConnector *)ringConnector)->setParam( dParamCFM, 0.1 );

  pinkyConnector->
    connect( pinky->getConnector( pinky->CONN_HAND ), Connector::AlignSlave);
  ((ODEAxleConnector *)pinkyConnector)->setParam( dParamERP, 0.3 );
  ((ODEAxleConnector *)pinkyConnector)->setParam( dParamCFM, 0.1 );
    
  index->assemble();
  middle->assemble();
  ring->assemble(); 
  thumb->assemble();
  pinky->assemble();
  */
}


void
Hand::connect()
{
  /*
  thumbConnector->
    connect( thumb->getConnector( thumb->CONN_HAND ), Connector::AlignSlave);
  ((ODEAxleConnector *)thumbConnector)->setParam( dParamERP, 0.8 );

  indexConnector->
    connect( index->getConnector( index->CONN_HAND ), Connector::AlignSlave);
  ((ODEAxleConnector *)indexConnector)->setParam( dParamERP, 0.8 );

  middleConnector->
    connect( middle->getConnector( middle->CONN_HAND ), Connector::AlignSlave);
  ((ODEAxleConnector *)middleConnector)->setParam( dParamERP, 0.8 );

  ringConnector->
    connect( ring->getConnector( ring->CONN_HAND ), Connector::AlignSlave);
  ((ODEAxleConnector *)ringConnector)->setParam( dParamERP, 0.8 );

  pinkyConnector->
    connect( pinky->getConnector( pinky->CONN_HAND ), Connector::AlignSlave);
  ((ODEAxleConnector *)pinkyConnector)->setParam( dParamERP, 0.8 );
  */
}

void
Hand::disconnect()
{
  /*
  if( thumbConnector->isConnected() )
    thumbConnector->disconnect();

  if( indexConnector->isConnected() )
    indexConnector->disconnect();

  if( middleConnector->isConnected() )
    middleConnector->disconnect();

  if( ringConnector->isConnected() )
    ringConnector->disconnect();

  if( pinkyConnector->isConnected() )
    pinkyConnector->disconnect();
  */
}
