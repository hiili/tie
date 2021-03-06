/*
 * Copyright (C) 2004-2005 Eero Uusitalo, Ville Heikkil�, Mikko Ylikangas, Paul
 * Wagner, Juho Heiskari, Kati M��tt�, Mikko Purtonen.
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
 
#include "BVDSpawner.hpp"
#include <lifespace/lifespace.hpp>
#include <tie/tie.hpp>

using namespace lifespace;

namespace tie
{

BVDSpawner::BVDSpawner(float a_size, float a_spawntime, Vector a_location, 
    ODEWorld * a_spawnWorld, int a_spawnCount, Vector a_force, 
    Vector a_torque):
    Spawner(a_size, a_spawntime, a_location, a_spawnWorld),
    force(a_force), torque(a_torque)
{
  for(int i = 0; i < a_spawnCount; i++) {
    pushObject(new TestObject
      ( Object::Params
        ( new ODELocator( makeVector3d() ),
          new BasicVisual
          ( shapes::Scaled::create
            ( makeVector3d( a_size / 3.0, a_size / 3.0, a_size / 3.0 ),
                                    shapes::Basis::create() ),
            &TIEMat::redMat ),
          new BasicGeometry
          ( shapes::Scaled::create( makeVector3d
            ( a_size / 3.0, a_size / 3.0, a_size / 3.0 ),
                                    shapes::Basis::create() ),
            (sptr (new CollisionMaterial (1.0, 0.1, 0) )))       
         ),
         a_size / 3.0
       )
     );
    
    pushObject( new TestObject2
        ( Object::Params
          ( new ODELocator( makeVector3d() ),
            new BasicVisual
            ( shapes::Scaled::create( makeVector3d
              ( a_size / 3.0, a_size / 3.0, a_size / 3.0 ),
                                      shapes::Basis::create() ),
              &TIEMat::whiteMat ),
            new BasicGeometry
            ( shapes::Scaled::create( makeVector3d
              ( a_size / 3.0, a_size / 3.0, a_size / 3.0 ),
                                      shapes::Basis::create() ),
              (sptr (new CollisionMaterial (1.0, 0.1, 0) )))
            ),
          a_size / 3.0
        )
     );
     
     pushObject( new TestObject2
        ( Object::Params
          ( new ODELocator( makeVector3d() ),
            new BasicVisual
            ( shapes::Scaled::create( makeVector3d
              ( a_size / 3.0, a_size / 3.0, a_size / 3.0 ),
                                      shapes::Basis::create() ),
              &TIEMat::whiteMat ),
            new BasicGeometry
            ( shapes::Scaled::create( makeVector3d
              ( a_size / 3.0, a_size / 3.0, a_size / 3.0 ),
                                      shapes::Basis::create() ),
              (sptr (new CollisionMaterial (1.0, 0.1, 0) )))
            ),
          a_size / 3.0
        )
     );
  }
}

BVDSpawner::~BVDSpawner()
{
}

Object * BVDSpawner::spawn()
{
  Object * middle = Spawner::spawn();
  if(middle != 0) {
    Object * end1 = Spawner::spawn(true);
    Object * end2 = Spawner::spawn(true);
    middle->getConnector(TestObject::CONN_X_AXIS_TIP).connect
      ( end1->getConnector(TestObject2::CONN_X_AXIS_TIP), 
        Connector::AlignSlave );
    middle->getConnector(TestObject::CONN_Y_AXIS_TIP).connect
      ( end2->getConnector(TestObject2::CONN_X_AXIS_TIP),
        Connector::AlignSlave );
    middle->getLocator()->addForceAbs( force );
    middle->getLocator()->addForceAbs( torque );
  }
  return middle;
}

};
