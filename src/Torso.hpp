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
 * @file Torso.hpp
 *
 * Torso subspace part of Avatar's joint form
 */

/**
 * @class tie::Torso
 *
 * @brief
 * Torso subspace part of Avatar's joint form
 *
 * This class is the torso of Avatar. It contains connectors for neck,
 * two arms and base.
 */

#ifndef TIE_TORSO_HPP
#define TIE_TORSO_HPP


#include <lifespace/lifespace.hpp>
#include "TIEMaterials.hpp"


namespace tie
{

class Torso:public lifespace::Subspace
{
private:
	lifespace::ODEBallConnector * neck;
	lifespace::ODEBallConnector * leftShoulder;
	lifespace::ODEBallConnector * rightShoulder;
	lifespace::Connector * base;
	
public:
	enum Connectors {
		CONN_NECK,
		CONN_LEFT_SHOULDER,
		CONN_RIGHT_SHOULDER,
		CONN_BASE
	};

  /**
   * Default constructor, calls superclass constructor.
   */
	Torso();

  /**
   * Default destructor.
   */
	virtual ~Torso();

	/* operations */

	/**
	* Creates connectors, physical appearance and geometrical form.
	*
	* @param bodyHeight The height of the torso
	* @param bodyWidth The width of the torso
	* @param bodyThickness The thickness of the torso
	*/
	void create(float bodyHeight, float bodyWidth, float bodyThickness);	

};

} /* namespace tie */

#endif //TIE_TORSO_HPP
