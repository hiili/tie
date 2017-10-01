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
 * @file BVDSpawner.hpp
 *
 * Example spawner class.
 */

/**
 * @class tie::BVDSpawner
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

}; //namespace tie

#endif //TIE_BVDSPAWNER_HPP
