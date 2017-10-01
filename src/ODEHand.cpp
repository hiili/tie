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

#include <lifespace/lifespace.hpp>
using namespace lifespace;
#include "ODEHand.hpp"
#include "ODEFinger.hpp"
#include "TIEMaterials.hpp"
#include "GloveController.hpp"
#include "types.hpp"
using namespace tie;

#include <map>
#include <iostream>
using std::cout;



/** constants */
const float ODEHand::FIST_GRIP_PRETHRESHOLD = 0.1;
const float ODEHand::FIST_GRIP_POSTHRESHOLD = 0.5;




/* private methods */
void
ODEHand::initControls()
{
  controls[CTRL_FLEXION_THUMB]  = Control( Control::ContinuousMode );
  controls[CTRL_FLEXION_INDEX]  = Control( Control::ContinuousMode );
  controls[CTRL_FLEXION_MIDDLE] = Control( Control::ContinuousMode );
  controls[CTRL_FLEXION_RING]   = Control( Control::ContinuousMode );
  controls[CTRL_FLEXION_PINKY]  = Control( Control::ContinuousMode );
  controls[CTRL_IS_FISTED]      = Control( Control::ContinuousMode );
}

void
ODEHand::updateFingers()
{
  /* calculate and transmit up and fwd vectors for each
     fingers stub and tip */

  float hiVal = FLEXION_MIN_VAL; // defined in GloveController.hpp
  float loVal = FLEXION_MAX_VAL; // defined in GloveController.hpp

  // index finger
  float val = readControl( CTRL_FLEXION_INDEX );
  float angle =(M_PI/2.0) * ((hiVal - val) / (hiVal - loVal ));

  Vector fwdF = makeVector3d( 0.0, -sin(angle), -cos(angle) );
  Vector upF = makeVector3d( 0.0, cos(angle), -sin(angle) );
  
  index->getLocator()->setBasis( BasisMatrix( fwdF, upF ) );
  index->update( fwdF, upF );
  
  // middle finger
  val = readControl( CTRL_FLEXION_MIDDLE );
  angle =(M_PI/2.0) * ((hiVal - val) / (hiVal - loVal ));
  
  fwdF = makeVector3d( 0.0, -sin(angle), -cos(angle) );
  upF = makeVector3d( 0.0, cos(angle), -sin(angle) );
  
  middle->getLocator()->setBasis( BasisMatrix( fwdF, upF ) );
  middle->update( fwdF, upF );
  
  // ring finger
  val = readControl( CTRL_FLEXION_RING );
  angle =(M_PI/2.0) * ((hiVal - val) / (hiVal - loVal ));
  
  fwdF = makeVector3d( 0.0, -sin(angle), -cos(angle) );
  upF = makeVector3d( 0.0, cos(angle), -sin(angle) );
  
  ring->getLocator()->setBasis( BasisMatrix( fwdF, upF ) );
  ring->update( fwdF, upF );
  
  // pinky finger
  val = readControl( CTRL_FLEXION_PINKY );
  angle =(M_PI/2.0) * ((hiVal - val) / (hiVal - loVal ));
  
  fwdF = makeVector3d( 0.0, -sin(angle), -cos(angle) );
  upF = makeVector3d( 0.0, cos(angle), -sin(angle) );
  
  pinky->getLocator()->setBasis( BasisMatrix( fwdF, upF ) );
  pinky->update( fwdF, upF );
}

void
ODEHand::release( Object * obj )
{
  obj->getConnector( objectPickedConnId ).disconnect();
  const_cast<Object::connectors_t &>( obj->getConnectors() ).erase
    ( objectPickedConnId );
  
  // reset connector id
  objectPickedConnId = 0;
}
 
void
ODEHand::pickUpObject( Object * objToPick )
{
  unsigned int id = 0;

  // resolve new, unused connector id
  if( !objToPick->getConnectors().empty() ) {
    Object::connectors_t::iterator i =
      const_cast<Object::connectors_t &>( objToPick->getConnectors() ).end();
    id = (--i)->first;
    if( id < TIE_DYNAMIC_CONNECTORS_RANGE_START )
      id = TIE_DYNAMIC_CONNECTORS_RANGE_START; // first connector (+ offset)
    else
      id++;  // next connector
  } else {
    id = TIE_DYNAMIC_CONNECTORS_RANGE_START; // first connector (+ offset)
  }

  BasicLocator tmp( *getWorldLocator() );
  objToPick->getWorldLocator()->transform( tmp, Abs2Rel );

  // create new connector and insert it to objects list (hand owns))
  boost::shared_ptr<Connector> objConn
    ( new ODEFixedConnector
      ( Connector( *objToPick,
                   Connector::Master, // ode fixed connector bug: rotation does
                   // not affect to slave object
                   tmp )));
  
  const_cast<Object::connectors_t &>( objToPick->getConnectors() ).insert
    ( std::make_pair(id, objConn) );
  
  // connect with hand
  objToPick->getConnector( id ).connect( getConnector( CONN_GRIP ),
                                       Connector::DontAlign );
  
  // store created connector id
  objectPickedConnId = id;
}







/* public methods */
ODEHand::ODEHand() :
  Object(Object::Params(new ODELocator, 0, 0)),
  timer( 0.0 ),
  objectPicked( 0 ),
  pickCandidate( 0 ),
  isPicked( false )
{
  initControls();
}

ODEHand::~ODEHand()
{
}

void 
ODEHand::create( float width, float thickness, enum Sides side_ )
{
  side = side_;
  
  float length = width * 0.89;
  // Set correct visual
  if(side == LEFT)
    setVisual
      ( sptr( new BasicVisual( sptr
                               ( new shapes::Cube
                                 ( makeVector3d(width, thickness, width))),
                               &TIEMat::whiteMat )));
  else
    setVisual
      ( sptr( new BasicVisual( sptr
                               ( new shapes::Cube
                                 ( makeVector3d(width, thickness, width))),
                               &TIEMat::groundMat )));
  
  setGeometry
    ( sptr( new BasicGeometry( sptr
                               ( new shapes::Cube
                                 ( makeVector3d(width, thickness, width))), 
                               sptr
                               ( new CollisionMaterial(3.0, 0.1, 0.01 ) ))));
  /*
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
  
  /* Base connector */
  base = new ODEBallConnector
    ( Connector
      (*this, Connector::Any,
       BasicLocator(makeVector3d(),
                    BasisMatrix( makeVector3d( 0.0, -1.0, 0.0 ),
                                 makeVector3d( 0.0, 0.0, 1.0 )))));
  
  connectors[CONN_BASE] = sptr( base );
  
  /* connector for picking up objects */
  gripConn = new ODEFixedConnector
    ( Connector
      (*this, Connector::Slave,
       BasicLocator( makeVector3d(),
                     BasisMatrix( makeVector3d( 0.0, 0.0, 1.0),
                                  makeVector3d( 0.0, 1.0, 0.0)))));

  connectors[CONN_GRIP] = sptr( gripConn );

  index = new ODEFinger
    ( Object::Params
      ( new BasicLocator( makeVector3d(-side*0.15, 0.0, -0.2 ), 
                          BasisMatrix(makeVector3d( 0.0, -1.0, 0.0),
                                      makeVector3d( 0.0, 0.0, 1.0))),
        new BasicVisual
        ( shapes::Located::create
          ( BasicLocator
            ( makeVector3d( 0.0, 0.0, -0.15 )),
            shapes::Cube::create
            ( makeVector3d( 0.095, 0.095, 0.2 ))),
          (side==LEFT? &TIEMat::whiteMat : &TIEMat::groundMat )),
        0));
  
  middle = new ODEFinger
    ( Object::Params
      ( new BasicLocator( makeVector3d(-side*0.15+(side*0.055*2.0),
                                       0.0, -0.2 ), 
                          BasisMatrix(makeVector3d( 0.0, -1.0, 0.0),
                                      makeVector3d( 0.0, 0.0, 1.0))),
        new BasicVisual
        ( shapes::Located::create
          ( BasicLocator
            ( makeVector3d( 0.0, 0.0, -0.15 )),
            shapes::Cube::create
            ( makeVector3d( 0.08, 0.1, 0.2 ))),
          (side==LEFT? &TIEMat::whiteMat : &TIEMat::groundMat )),
        0));
  
  ring = new ODEFinger
    ( Object::Params
      ( new BasicLocator( makeVector3d(-side*0.15+(side*0.065*3.0),
                                       0.0, -0.2 ), 
                          BasisMatrix(makeVector3d( 0.0, -1.0, 0.0),
                                      makeVector3d( 0.0, 0.0, 1.0))),
        new BasicVisual
        ( shapes::Located::create
          ( BasicLocator
            ( makeVector3d( 0.0, 0.0, -0.15 )),
            shapes::Cube::create
            ( makeVector3d( 0.08, 0.1, 0.2 ))),
          (side==LEFT? &TIEMat::whiteMat : &TIEMat::groundMat )),
        0));
  
  pinky = new ODEFinger
    ( Object::Params
      ( new BasicLocator( makeVector3d(-side*0.15+(side*0.075*4.0),
                                       0.0, -0.2 ), 
                          BasisMatrix(makeVector3d( 0.0, -1.0, 0.0),
                                      makeVector3d( 0.0, 0.0, 1.0))),
        new BasicVisual
        ( shapes::Located::create
          ( BasicLocator
            ( makeVector3d( 0.0, 0.0, -0.15 )),
            shapes::Cube::create
            ( makeVector3d( 0.095, 0.095, 0.2 ))),
          (side==LEFT? &TIEMat::whiteMat : &TIEMat::groundMat )),
        0));
  
  addObject(index);
  addObject(middle);
  addObject(ring);
  addObject(pinky);

    // finally, add hand as listener of its own collision events
  getGeometry()->events.addListener( this );
}

void 
ODEHand::prepare( lifespace::real dt )
{
  updateFingers();

  // check, whether object is already in hand
  if( isPicked ) {
    if( readControl( CTRL_IS_FISTED ) < 0.5 ) {
      release( objectPicked );
      isPicked = false;
      objectPicked = 0;
    }
  } else { // if not in hand, check pickup
    if( readControl( CTRL_IS_FISTED ) >= 0.5 &&
        pickCandidate &&
        timer < FIST_GRIP_POSTHRESHOLD ) {
      pickUpObject( pickCandidate );
      isPicked = true;
      objectPicked = pickCandidate;
    }
  }

  timer += dt;

  // methods inherited from base classes
  Actor::prepare( dt );
  Subspace::localPrepare( dt );
  Object::prepare( dt );
}


void
ODEHand::step()
{
  Actor::step();
  Subspace::localStep();
  Object::step();
}

void
ODEHand::processEvent( const Geometry::AddContactEvent & event,
                       Geometry & source )
{
  if( !isPicked && readControl( CTRL_IS_FISTED ) < 0.5 ) {
    pickCandidate = const_cast<Object *>( event.other->getHostObject() );
    timer = 0.0;
  }
}

