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

#include "ODEFinger.hpp"
#include "TIEMaterials.hpp"
#include "CylinderVisual.hpp"
#include "types.hpp"
using namespace tie;

#include <lifespace/lifespace.hpp>

using namespace lifespace;


ODEFinger::ODEFinger( lifespace::Object::Params params ):
  Object( params )
{   
  //Create the finger tip part
  tip = new Object
    ( Object::Params( new BasicLocator
                      ( makeVector3d( 0.0, 0.0, -0.25 ),
                        BasisMatrix() ),
                      new BasicVisual   
                      ( shapes::Located::create
                        ( BasicLocator
                          ( makeVector3d( 0.0, 0.0, -0.15)),
                          shapes::Cube::create   
                          ( makeVector3d( 0.08, 0.1, 0.2 ))),
                        &TIEMat::whiteMat ),
                      0 ));
  addObject( tip );
}


void 
ODEFinger::update( Vector fwd, Vector up )
{
  tip->getLocator()->setBasis( BasisMatrix( fwd, up ) );
}
