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
 * @file Arm.hpp
 *
 * Torso subspace part of Avatar's joint form
 */

/**
 * @class tie::Arm
 *
 * @brief Arm subspace part of Avatar's joint form
 *
 * This class represents the arms of Avatar. It contains connectors for 
 * shoulders, wrists and base.
 */
 
#ifndef TIE_ARM_HPP
#define TIE_ARM_HPP
#include <lifespace/lifespace.hpp>
#include "TIEMaterials.hpp"

namespace tie
{

class Arm: public lifespace::Subspace
{
private:
	lifespace::ODEBallConnector * shoulder;
	lifespace::ODEBallConnector * wrist;
	lifespace::Connector * base;

public:
	enum Connectors {
		CONN_SHOULDER,
		CONN_WRIST,
		CONN_BASE
	};
	
  /**
   * Default constructor, calls superclass constructor.
   */	
	Arm();
	
  /**
   * Default destructor.
   */
	virtual ~Arm();
	
	/* operations */
	
	/**
	* Creates connectors, physical appearance and geometrical form.
	*
	* @param length The length of arm
	* @param size The thickness of arm
	*/
	void create(float length, float size);
};

} /* namespace tie */

#endif //TIE_ARM_HPP
