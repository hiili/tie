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
 * @file ODESkeleton.hpp
 *
 * ODESkeleton is the base class for physical user representation in virtual
 * world.
 */

/**
 * @class tie::ODESkeleton
 *
 * @brief
 * ODESkeleton contains functionality for managing ODE skeletal structures
 * with staticly updated controlnodes (in case of TIE: FluidSkeleton)
 * 
 * @see tie::Skeleton, tie::Avatar
 */


namespace tie {
  class ODESkeleton;
};

#ifndef TIE_ODESKELETON_HPP
#define TIE_ODESKELETON_HPP

#include "FluidSkeleton.hpp"
#include "Skeleton.hpp"
#include "Head.hpp"
#include "ODEHand.hpp"

#include <lifespace/lifespace.hpp>

#include <list>
#include <cassert>
#include <iostream>


namespace tie {


  class ODESkeleton :
    public Skeleton
  {
  private:
    bool notConnected;

    lifespace::Object * torso;
    Head * head;
    lifespace::Object * leftArm;
    lifespace::Object * rightArm;
    ODEHand * leftHand;
    ODEHand * rightHand;

    /** internal method, which connects skeleton connectors */
    void constructSkeleton();

  public:
    /** ctor */
    ODESkeleton() {}

    ODESkeleton( lifespace::Subspace * baseNode_,
                 lifespace::Object * torso_ = 0,
                 Head * head_= 0,
                 lifespace::Object * leftArm_ = 0,
                 lifespace::Object * rightArm_ = 0,
                 ODEHand * leftHand_ = 0,
                 ODEHand * rightHand_= 0 ):
      Skeleton( baseNode_ ),
      torso( torso_ ),
      head( head_ ),
      leftArm( leftArm_ ),
      rightArm( rightArm_ ),
      leftHand( leftHand_ ),
      rightHand( rightHand_ )
    {
      assert( baseNode != 0 ); // basenode must exist!!

      // add body parts to base node(if they exist)
      if( torso != 0 )
        baseNode->addObject( torso );
      if( head != 0 )
        baseNode->addObject( head );
      if( leftArm != 0 )
        baseNode->addObject( leftArm );
      if( rightArm != 0 )
        baseNode->addObject( rightArm );
      if( leftHand != 0 )
        baseNode->addObject( leftHand );
      if( rightHand != 0 )
        baseNode->addObject( rightHand );
      
      notConnected = true;
    }

    /** dtor */
    virtual ~ODESkeleton()
    { baseNode = 0; }
    
    ODEHand * getODEHand( bool isLeft );

    virtual void connect( Skeleton * master );
    
  };


}  /* namespace tie */


#endif /* TIE_ODESKELETON_HPP */
