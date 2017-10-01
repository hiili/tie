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
 * @file BowlingAlley.cpp
 *
 * This is the bowling alley subspace for The Incredible Environment.
 *
 * Version: $Name: tie-1_2 $
 */
#include "BowlingAlley.hpp"
#include <lifespace/lifespace.hpp>
#include "BowlingPin.hpp"
using namespace lifespace;
using namespace tie;

#include <iostream>

static float PinOffset = 0.40;


//Ball removed
/*
static float BallSize = 0.4;
static float BallMass = 5.0;
static float BallInertia = 1.0;
static float BallFriction = 0.01;
static float BallBounciness = 0.05;
*/

//Constructor
BowlingAlley::BowlingAlley(lifespace::Vector location) :
	Subspace(Subspace::Params(Object::Params
		(new ODELocator(location,BasisMatrix()),0,0)))
{
setLocator( sptr( new ODELocator(location)));
BowlingPin * pin1 = new BowlingPin 
						(makeVector3d());
BowlingPin * pin2 = new BowlingPin 
						(makeVector3d(PinOffset, 0.0, PinOffset));
BowlingPin * pin3 = new BowlingPin 
						(makeVector3d(-PinOffset, 0.0, PinOffset));
BowlingPin * pin4 = new BowlingPin 
						(makeVector3d(0.0, 0.0, PinOffset * 2.0));
BowlingPin * pin5 = new BowlingPin 
						(makeVector3d(-PinOffset * 2.0, 0.0, PinOffset * 2.0));
BowlingPin * pin6 = new BowlingPin 
						(makeVector3d(PinOffset * 2.0, 0.0, PinOffset * 2.0));
BowlingPin * pin7 = new BowlingPin 
						(makeVector3d(-PinOffset, 0.0, PinOffset * 3.0));
BowlingPin * pin8 = new BowlingPin 
						(makeVector3d(PinOffset, 0.0, PinOffset * 3.0));
BowlingPin * pin9 = new BowlingPin 
						(makeVector3d(-PinOffset * 3.0, 0.0, PinOffset * 3.0));
BowlingPin * pin10 = new BowlingPin 
						(makeVector3d(PinOffset * 3.0, 0.0, PinOffset *3.0));


// Removed

/*
Object * bowlingBall = new Object 
	(Object::Params 
		(new ODELocator (makeVector3d(0.0, 0.0, -PinOffset * 20.0),BasisMatrix(),
                                 BallMass, BallInertia),
                 new BasicVisual (sptr(new shapes::Sphere(BallSize)),&TIEMat::blackMat),
                 new BasicGeometry (sptr(new shapes::Sphere(BallSize)),
                                    (sptr (new CollisionMaterial (BallFriction,BallBounciness,0.0))))));

*/
addObject(pin1);
addObject(pin2);
addObject(pin3);
addObject(pin4);
addObject(pin5);
addObject(pin6);
addObject(pin7);
addObject(pin8);
addObject(pin9);
addObject(pin10);
//addObject(bowlingBall);
}
//destructor
BowlingAlley::~BowlingAlley()
{
}

void 
BowlingAlley::prepare( lifespace::real dt )
{
  //std::cout << "BowlingAlley::prepare(): start - calling Subspace::localPrepare\n";
  Subspace::localPrepare( dt );
  //std::cout << "BowlingAlley::prepare(): calling Object::prepare()\n";
  Object::prepare( dt );
  //std::cout << "BowlingAlley::prepare(): finished\n";
}

void
BowlingAlley::step()
{
  //std::cout << "BowlingAlley::prepare(): start - calling Subspace::localStep()\n";
  Subspace::localStep();
  //std::cout << "BowlingAlley::prepare(): calling Object::step()\n";
  Object::step();
  //std::cout << "BowlingAlley::prepare(): finished\n";
}
