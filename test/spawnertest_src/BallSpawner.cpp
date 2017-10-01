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
 
#include "BallSpawner.hpp"
#include <lifespace/lifespace.hpp>
#include <tie/tie.hpp>

using namespace lifespace;

namespace tie
{

BallSpawner::BallSpawner(float a_size, float a_spawntime, Vector a_location, 
    ODEWorld * a_spawnWorld, int a_spawnCount, Vector a_force, 
    Vector a_torque):
    Spawner(a_size, a_spawntime, a_location, a_spawnWorld),
    force(a_force), torque(a_torque)
{
  /**
   * Add total spawnCount balls to the spawntstack of Spawner superclass
   */
  for(int i = 0; i < a_spawnCount; i++) {
    pushObject(new Object
      ( Object::Params        
        /* Must give ODELocator as locator, location does not matter */
        ( new ODELocator(),
        /* Create visual for the balls */
        new BasicVisual( shapes::Sphere::create(a_size / 2),
          &TIEMat::groundMat ),
        /* Create geometry for the balls */
        new BasicGeometry( shapes::Sphere::create(a_size / 2),
          (sptr (new CollisionMaterial (1.0, 0.1, 0) )))
        )
      )
    );
  }
}

BallSpawner::~BallSpawner()
{
}

/**
 * Apply some force to the object that is spawned.
 */
Object * BallSpawner::spawn()
{
  Object * spawnObject = Spawner::spawn();
  if(spawnObject != 0) {
    spawnObject->getLocator()->addForceAbs(force);
    spawnObject->getLocator()->addTorqueAbs(torque);
    return spawnObject;
  }
  else
    return 0;
}

};
