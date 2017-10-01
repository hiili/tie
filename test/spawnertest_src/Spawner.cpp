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

#include "Spawner.hpp"
#include <lifespace/lifespace.hpp>
#include <tie/tie.hpp>
#include <math.h>
using namespace lifespace;

/* Initialise static counter of total number of spawners */
int Spawner::spawnerCount = 0;

Spawner::Spawner(float a_size, float a_spawntime, Vector a_location, 
    ODEWorld * a_spawnWorld):
  Object(Object::Params(new BasicLocator(a_location))),  
  size(a_size), location(a_location), 
  spawnWorld(a_spawnWorld), spawntime(a_spawntime)
{
  /* Initialise counters */
  spawnCount = 0;
  passedTime = 0.0;
  spawnIndex = 0;
  lastSpawn = 0;
  
  /* Add one to the total number of spawners */
  spawnerCount++;
  
  /* Create spawning platform */
  Object * platform = new Object
    (Object::Params 
      (new BasicLocator(a_location),
      new BasicVisual
        (sptr (new shapes::Cube
          (makeVector3d(size, size / 100, size))), &TIEMat::redMat),
      new BasicGeometry 
        (sptr (new shapes::Cube(makeVector3d(size, size / 100, size))),
        (sptr (new CollisionMaterial(1.0, 0.1, 0.0))))
      )
    );
  
  /* Add the object to the ODEWorld */
  spawnWorld->addObject(platform);
}

Spawner::~Spawner()
{
}

void Spawner::prepare(float dt)
{
  /* Increase passed time counter with given dt */
  passedTime += (float)dt;
  
  /* If passed time is greater than spawntime try to spawn object */
  if(passedTime > spawntime) {
    spawn();
    passedTime = 0.0;
  }
  
  /* Call object's prepare method */
  Object::prepare(dt);
}

void Spawner::pushObject(Object * newObject)
{
  /* Increase spawnable object count */
  spawnCount++;
  
  /* Add the object to the stack */
  spawnObjects.push_back(newObject);
  
  /* Add the object's visual to the stack */
  spawnVisuals.push_back(newObject->getVisual());
  
  /* Set object's locator to suitable location */
  newObject->setLocator( sptr ( new ODELocator
    ( makeVector3d( (size + 0.1) * spawnCount, 
    spawnerCount * -10, 0.0 ))));

  /* Set object's visual to null */
  newObject->setVisual( boost::shared_ptr<Visual>() );  
  /* Add the object to the world */
  spawnWorld->addObject(newObject);

}

Object * Spawner::spawn(bool always_spawn)
{
  /* Calculate the vector from spawner to last spawned object */
  Vector location = makeVector3d
    ( getLocator()->getLoc()[0], 
    getLocator()->getLoc()[1] + size,
    getLocator()->getLoc()[2]);
  
  /* If spawner's and last spawned object's distance is greater than
   * the size of the object spawn new object if available */
  if(spawnIndex < spawnCount && 
      ( always_spawn || verticalDistanceOf( lastSpawn) > size ))
  {
    lastSpawn = spawnObjects[spawnIndex];
    
    /* Set correct locator and visual */
    lastSpawn->getLocator()->setLoc(location);
    lastSpawn->setVisual(spawnVisuals[spawnIndex]);
    
    /* Set velocity and rotation to zero */
    lastSpawn->getLocator()->setVel(makeVector3d());
    lastSpawn->getLocator()->setRotation(makeVector3d());
    
    spawnIndex++;
    return lastSpawn;
  }  
  else
    return 0;
}

Object * Spawner::spawn()
{
  return spawn(false);
}

float Spawner::verticalDistanceOf(lifespace::Object * targetObject)
{
  if(targetObject != 0) {
    Vector dVector = 
      getLocator()->getLoc() - targetObject->getLocator()->getLoc();
    float distance =
      sqrt(dVector[0] * dVector[0] + dVector[2] * dVector[2]);
    return distance;
  }
  else /* If no last object, return INFINITY as distance */
    return INFINITY;
}

