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
 * @file Chain.cpp
 *
 * Implementations for the Chain class.
 */

#include "Chain.hpp"

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <tie/tie.hpp>
using namespace tie;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;




Chain::Chain( const LinkDimensions & link, int linkCount,
              const lifespace::Material * visualMaterial,
              shared_ptr<const CollisionMaterial> collisionMaterial,
              lifespace::Subspace & hostSpace,
              Chain * firstLink ) :
  Object( Object::Params() ),
  tail( 0 )
{
  assert( linkCount >= 1 );
  assert( visualMaterial && collisionMaterial );
  
  float sideCenterX = link.width / 2.0 - link.thickness / 2.0;
  float endCenterY = link.length / 2.0 - link.thickness / 2.0;
  float endInnerY = link.length / 2.0 - link.thickness;
  float sideCylLength = link.length - 2.0 * link.thickness;
  float endCylLength = link.width - 2.0 * link.thickness;
  assert( sideCylLength > 0.0 && endCylLength > 0.0 );
  
  // create the collision shape
  /*
  visualShape = firstLink ? firstLink->visualShape :
    (shared_ptr<Shape>)shapes::Union::create
    ( shapes::Located::create
      ( BasicLocator( makeVector3d( -sideCenterX, 0.0, 0.0 ),
                      BasisMatrix( makeVector3d( 0.0, 1.0, 0.0 ),
                                   makeVector3d( 0.0, 0.0, 1.0 ))),
        shapes::CappedCylinder::create( sideCylLength, link.thickness / 2.0 )),
      shapes::Located::create
      ( BasicLocator( makeVector3d( sideCenterX, 0.0, 0.0 ),
                      BasisMatrix( makeVector3d( 0.0, 1.0, 0.0 ),
                                   makeVector3d( 0.0, 0.0, 1.0 ))),
        shapes::CappedCylinder::create( sideCylLength, link.thickness / 2.0 )),
      shapes::Located::create
      ( BasicLocator( makeVector3d( 0.0, -endCenterY, 0.0 ),
                      BasisMatrix( makeVector3d( 1.0, 0.0, 0.0 ),
                                   makeVector3d( 0.0, 0.0, 1.0 ))),
        shapes::CappedCylinder::create( endCylLength, link.thickness / 2.0 )),
      shapes::Located::create
      ( BasicLocator( makeVector3d( 0.0, endCenterY, 0.0 ),
                      BasisMatrix( makeVector3d( 1.0, 0.0, 0.0 ),
                                   makeVector3d( 0.0, 0.0, 1.0 ))),
        shapes::CappedCylinder::create( endCylLength, link.thickness / 2.0 )));
  */
  
  visualShape = firstLink ? firstLink->visualShape :
    (shared_ptr<Shape>)shapes::Union::create
    ( shapes::Located::create
      ( BasicLocator( makeVector3d( -sideCenterX, 0.0, 0.0 ) ),
        shapes::Cube::create
        ( makeVector3d( link.thickness*1.05, link.length, link.thickness*1.05 ))),
      shapes::Located::create
      ( BasicLocator( makeVector3d( sideCenterX, 0.0, 0.0 ) ),
        shapes::Cube::create
        ( makeVector3d( link.thickness*1.05, link.length, link.thickness*1.05 ))),
      shapes::Located::create
      ( BasicLocator( makeVector3d( 0.0, -endCenterY, 0.0 ) ),
        shapes::Cube::create
        ( makeVector3d( link.width, link.thickness, link.thickness ))),
      shapes::Located::create
      ( BasicLocator( makeVector3d( 0.0, endCenterY, 0.0 ) ),
        shapes::Cube::create
        ( makeVector3d( link.width, link.thickness, link.thickness ))));
  
  // create the visual shape
  collisionShape = firstLink ? firstLink->collisionShape :
    (shared_ptr<Shape>)shapes::Cube::create
    ( makeVector3d( link.width, link.length, link.thickness ) );
  
  // create the Visual
  visual = sptr( new BasicVisual( visualShape, visualMaterial ));
  
  // create the Geometry
  geometry = sptr( new BasicGeometry( collisionShape, collisionMaterial ));
  
  // create the Locator
  shared_ptr<Locator> locator
            ( new ODELocator( makeVector3d( 10.0 * FRAND01() - 5.0,
                                            10.0 * FRAND01() + 5.0,
                                            -40.0 * FRAND01() ),
                              BasisMatrix(),
                              1.0, 2.0,
                              0.1, 0.1, 0.1,
                              0.1, 0.1, 0.11 ) );
  
  // init the Object
  setLocator( locator );
  setVisual( visual );
  setGeometry( geometry );
  
  
  // create the connectors
  connectors[CONN_BASE] =
    sptr( new ODEBallConnector
          ( Connector( *this, Connector::Any,
                       BasicLocator( makeVector3d( 0.0, -endInnerY, 0.0 ),
                                     BasisMatrix
                                     ( makeVector3d( 0.0, -1.0, 0.0 ),
                                       makeVector3d( 0.0, 0.0, 1.0 ))))));
  connectors[CONN_TIP] =
    sptr( new ODEBallConnector
          ( Connector( *this, Connector::Any,
                       BasicLocator( makeVector3d( 0.0, endInnerY, 0.0 ),
                                     BasisMatrix
                                     ( makeVector3d( 0.0, 1.0, 0.0 ),
                                       makeVector3d( 1.0, 0.0, 0.0 ))))));
  
  
  // insert this into the firstLink subspace (if we are not the first one)
  //if( firstLink ) firstLink->addObject( this );
  hostSpace.addObject( this );
  
  // create the rest of the chain
  if( linkCount > 1 ) {
    tail = new Chain( link, linkCount - 1,
                      visualMaterial, collisionMaterial, hostSpace,
                      firstLink ? firstLink : this );
  }
  
}




void Chain::connect( Connector * baseConnector )
{
  // connect this to the given base connector (if given)
  if( baseConnector ) baseConnector->connect( getConnector(CONN_BASE),
                                              Connector::AlignSlave );
  
  // HUOM: oletus: palloniveli‰!
  /*
  if( baseConnector ) {
    ODEBallConnector & conn = *(ODEBallConnector *)baseConnector;
    //conn.setParam( dParamERP, 0.2 );
    //conn.setParam( dParamCFM, 0.1 );
  }
  */
  
  // connect the tail
  if( tail ) tail->connect( &getConnector(CONN_TIP) );
}
