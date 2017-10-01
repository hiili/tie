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
 * @file Spawner.hpp
 *
 * Base class for object spawners.
 */

/**
 * @class tie::Spawner
 *
 * @brief
 * Bese class for object spawners.
 * 
 * This class is base class for all object spawners. It contains all spawner
 * functionality except creating the objects to be spawned. Spawners are
 * represented as static platforms. When creating subclass-spawners you
 * must create all objects to be spawned and add them to spawner via calling
 * pushObject method.
 */

#ifndef TIE_SPAWNER_HPP
#define TIE_SPAWNER_HPP

#include <lifespace/lifespace.hpp>
#include <vector>

namespace tie
{

class Spawner: public lifespace::Object
{
private:
  /* Variables that contain information about the spawner. */  
  float size;
  lifespace::Vector location;
  lifespace::ODEWorld * spawnWorld;
  std::vector<lifespace::Object *> spawnObjects;
  std::vector< boost::shared_ptr< lifespace::Visual > > spawnVisuals;
  
  /* Variables that keep the state of the spawner. */
  float spawntime, passedTime;
  int spawnCount;
  lifespace::Object * lastSpawn;

  /**
   * Calculates the vertical (xz plane) distance of an object and the spawner .
   * 
   * @param targetObject The object whose distance is to be calculated.
   * @return The distance of object. 
   */
  float  verticalDistanceOf(lifespace::Object * targetObject);

protected:
  int spawnIndex;

public:
  /**
   * Keeps the total number of spawners in the world 
   */
  static int spawnerCount;

  /**
   * Spawner constructor. All subclasses must call this at their
   * constructor.
   * 
   * @param a_size The size of the spawner. Should be the maximum length
   * between two points of the object to be spawned, so that the object
   * always fits on the spawner platform.
   * @param a_spawntime The time between spawner checks. Think it as the
   * maximum time between spawns. Given in seconds.
   * @param a_location The location of the spawner.
   * @param a_spawnWorld The target ODEWorld the spawner is located in.
   * Note that spawners MUST be added directly to ODEWorld.
   */
	Spawner(float a_size, float a_spawntime, lifespace::Vector a_location, 
    lifespace::ODEWorld * a_spawnWorld);
	
  /**
   * Default destructor
   */
  virtual ~Spawner();
  
  /**
   * Spawns a new object or if out of objects does nothing. Useful if you
   * want to add additional functionality for sublclasses' spawn methods.
   * 
   * @param always_spawn Spawn when last spawned still present
   * @return The last object spawned, 0 if no object available
   */
  lifespace::Object * spawn(bool always_spawn);
  
  /**
   * Spawns a new object or if out of objects does nothing. Objects are
   * spawned at Lifespace's prepare method. Subclasses can override
   * this method and control spawned object
   * 
   * @return The last object spawned, 0 if no object available
   */
  virtual lifespace::Object * spawn();
  
  /**
   * Overrides Lifespace's prepare method for timing the spawnings.
   *
   * @param dt
   */
  virtual void prepare(float dt);
  
  /**
   * Adds a new object to this spawner's spawn stack. Spawner works in
   * first-in first-out manner.
   * 
   * @param newObject The new object to be added to the end of the spawning
   * stack.
   */
  void pushObject(lifespace::Object * newObject);
};

};

#endif //TIE_SPAWNER_HPP
