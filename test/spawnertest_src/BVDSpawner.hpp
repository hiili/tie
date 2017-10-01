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
 * @file BallSpawner.hpp
 *
 * Example spawner class.
 */

/**
 * @class tie::BallSpawner
 *
 * @brief
 * Example spawner class.
 * 
 * Example use of TIE:s spawner class. Spawns balls of given size.
 */
 
#ifndef TIE_BVDSPAWNER_HPP
#define TIE_BVDSPAWNER_HPP

#include "Spawner.hpp"
#include <lifespace/lifespace.hpp>

namespace tie
{

class BVDSpawner: public Spawner
{
private:
  lifespace::Vector force;
  lifespace::Vector torque;
  lifespace::Object * middle, * end1, * end2;
public:
	BVDSpawner(float a_size, float a_spawntime, lifespace::Vector a_location, 
    lifespace::ODEWorld * a_spawnWorld, int a_spawnCount = 0,
    lifespace::Vector a_force = lifespace::makeVector3d(), 
    lifespace::Vector a_torque = lifespace::makeVector3d());
    
	virtual ~BVDSpawner();
  
  virtual Object * spawn();
};

/*
 * The middle object of the rotating basis vector system. This object has two
 * connectors: one at the tip of the X axis (CONN_X_AXIS_TIP) and one at the
 * tip of the Y axis (CONN_Y_AXIS_TIP). It is also a subspace and can be used
 * to test the hierarchy conversion between Lifespace and ODE (just insert the
 * other objects into this subspace, or build some hierarchies with this,
 * etc..).
 */
class TestObject :
  public lifespace::Object
{
 public:
  
  enum Connectors {
    CONN_X_AXIS_TIP,
    CONN_Y_AXIS_TIP
  };
  
  /**
   * Constructs a new test object with connectors at the tip of the x and y
   * axis.
   */
  TestObject( const lifespace::Object::Params & objectParams, float size ) :
    lifespace::Object( objectParams )
  {
    // create and insert the x axis connector
    connectors[CONN_X_AXIS_TIP] = boost::shared_ptr<lifespace::Connector>
      ( new lifespace::ODEAxleConnector
        ( lifespace::Connector( *this, lifespace::Connector::Any,
                     lifespace::BasicLocator
                     ( lifespace::makeVector3d( size , 0.0, 0.0 ),
                       lifespace::BasisMatrix
                       ( lifespace::makeVector3d( 1.0, 0.0, 0.0 ),
                         lifespace::makeVector3d( 0.0, 1.0, 0.0 ) ))),
          1.0, -INFINITY, INFINITY, 0.5 ) );
    // create and insert the y axis connector
    connectors[CONN_Y_AXIS_TIP] = boost::shared_ptr<lifespace::Connector>
      ( new lifespace::ODEAxleConnector
        ( lifespace::Connector( *this, lifespace::Connector::Any,
                     lifespace::BasicLocator
                     ( lifespace::makeVector3d( 0.0, size , 0.0 ),
                       lifespace::BasisMatrix
                       ( lifespace::makeVector3d( 0.0, 1.0, 0.0 ),
                         lifespace::makeVector3d( 0.0, 0.0, 1.0 ) ))),
          1.0, -INFINITY, INFINITY, 0.5 ) );
  }
};

/*
 * The non-middle objects of the rotating basis vector system. This object has
 * only one connector (at the tip of the x axis) and is inherited directly from
 * Object (instead of a Subspace).
 */
class TestObject2 :
  public lifespace::Object
{
 public:
  
  enum Connectors {
    CONN_X_AXIS_TIP
  };
  
  /**
   * Constructs a new test object with a connector at the tip of the x axis
   */
  TestObject2( const lifespace::Object::Params & objectParams, float size ) :
    lifespace::Object( objectParams )
  {
    // create and insert the x axis connector
    connectors[CONN_X_AXIS_TIP] = boost::shared_ptr<lifespace::Connector>
      ( new lifespace::ODEAxleConnector
        ( lifespace::Connector( *this, lifespace::Connector::Any,
                     lifespace::BasicLocator
                     ( lifespace::makeVector3d( size , 0.0, 0.0 ),
                       lifespace::BasisMatrix
                       ( lifespace::makeVector3d( 1.0, 0.0, 0.0 ),
                         lifespace::makeVector3d( 0.0, 1.0, 0.0 ) ))),
          1.0, -INFINITY, INFINITY, 0.5 ) );
  }
};

}; //namespace tie

#endif //TIE_BVDSPAWNER_HPP
