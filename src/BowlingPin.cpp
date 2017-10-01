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
 * @file BowlingPin.cpp
 *
 * The bowling pin class.
 *
 * Version: $Name: tie-1_2 $
 */

#include "BowlingPin.hpp"
using namespace tie;
using namespace lifespace;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

static float PinRadius = 0.15;
static float PinHeight = 1.4;
static float PinFriction = 0.3;
static float PinBounciness = 0.8;
static float PinMass = 0.25;
static float PinInertia = 1.0;

BowlingPin::BowlingPin(lifespace::Vector location) 
  :Object (Object::Params (new ODELocator(location,BasisMatrix(),
                                          PinMass,PinInertia),
                           new BasicVisual(sptr
                                           (new shapes::Cube
                                            (makeVector3d(PinRadius,
                                                          PinHeight,
                                                          PinRadius))),
                                           &TIEMat::whiteMat),
                           new BasicGeometry
                           (sptr 
                            (new shapes::Cube
                             (makeVector3d
                              (PinRadius, PinHeight, PinRadius))),
                            (sptr 
                             (new CollisionMaterial
                              (PinFriction, PinBounciness,0.0)
                              )
                             )
                            )
                           )
           )
{
}


BowlingPin::~BowlingPin()
{
}


