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
 * @file BowlingPin.hpp
 *
 * Main header for the bowling pin class.
 *
 * Version: $Name: tie-1_2 $
 */
#ifndef _BOWLINGPIN_HPP_
#define _BOWLINGPIN_HPP_

#include <lifespace/lifespace.hpp>

//Materials
#include "TIEMaterials.hpp"


namespace tie
{
class BowlingPin: public lifespace::Object
{
public:
	BowlingPin(lifespace::Vector location);
	virtual ~BowlingPin();
};

} /* namespace tie */

#endif //_BOWLINGPIN_HPP_
