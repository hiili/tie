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
 * @file Seesaw.hpp
 *
 * FluidSkeleton is the fluid assigned skeletal structure of Avatar in EVE.
 */

/**
 * @class tie::Seesaw
 *
 * @brief
 * Seesaw object for IM
 */

#ifndef TIE_SEESAW_HPP
#define TIE_SEESAW_HPP

#include "TIEMaterials.hpp"
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

namespace tie
{

class Seesaw: public Subspace
{
public:
	Seesaw(lifespace::Vector a_location, float a_length, 
    lifespace::Vector a_rotation = lifespace::makeVector3d(0.0, 0.0, 1.0));
	virtual ~Seesaw();
};

};

#endif //TIE_SESAW_HPP
