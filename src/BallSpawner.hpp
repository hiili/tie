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
 
#ifndef _BALLSPAWNER_HPP_
#define _BALLSPAWNER_HPP_

#include "Spawner.hpp"
#include <lifespace/lifespace.hpp>

namespace tie
{

class BallSpawner: public Spawner
{
private:
  lifespace::Vector force;
  lifespace::Vector torque;
  bool alwaysSpawn;
public:

/**
 * Constructs a new ballspawner of given size.
 * 
 * @param a_size The size of the ballspawner. Equals two times the radius
 * of the spawned ball.
 * @param a_spawntime The minimum time between spawns in seconds.
 * @param a_location The location of the ballspawner.
 * @param a_spawnWorld The ODEWorld of the spawner.
 * @param a_spawnCount The number of balls to spawn.
 * @param a_visual Does the spawner have visual.
 * @param a_geometry Does the spawner have geometry.
 * @param a_force Force applied to the spawned ball after spawning.
 * @param a_torque Torque applied to the spawned ball after spawning.
 */
	BallSpawner(float a_size, float a_spawntime, lifespace::Vector a_location, 
    lifespace::ODEWorld * a_spawnWorld, int a_spawnCount = 0,
    bool a_visual = true, bool a_geometry = true,
    bool a_alwaysSpawn = false,
    lifespace::Vector a_force = lifespace::makeVector3d(), 
    lifespace::Vector a_torque = lifespace::makeVector3d());
	
  /**
   * Default destructor
   */  
  virtual ~BallSpawner();
  
  /**
   * Override superclass spawn method for adding some force and torque to
   * the spawned object
   * @return The spawned object
   */
  lifespace::Object * spawn();
};

};

#endif //_BALLSPAWNER_HPP_
