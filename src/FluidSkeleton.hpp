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
 * @file FluidSkeleton.hpp
 *
 * FluidSkeleton is the fluid assigned skeletal structure of Avatar in EVE.
 */

/**
 * @class tie::FluidSkeleton
 *
 * @brief
 * FluidSkeleton contains functionality for managing staticly updated
 * controlnodes, that can be defined as skeleton with clearly definable base
 * object.
 * 
 * @see tie::Skeleton, tie::Avatar
 */


namespace tie {
  class FluidSkelton;
};

#ifndef TIE_FLUIDSKELETON_HPP
#define TIE_FLUIDSKELETON_HPP

#include "ODESkeleton.hpp"
#include "Skeleton.hpp"

#include <lifespace/lifespace.hpp>

#include <list>
#include <cassert>
#include <iostream>

namespace tie {


  class FluidSkeleton :
    public Skeleton
  {
  private:
    lifespace::FloatingActor * headControl;
    lifespace::FloatingActor * leftHandControl;
    lifespace::FloatingActor * rightHandControl;

    ODESkeleton * odeSkeleton;

  public:

    enum Connectors {
      CONN_HEAD_BASE,
      CONN_LEFT_HAND,
      CONN_RIGHT_HAND
    };

    /** 
     * ctor
     *
     * @param baseNode_ base object for this skeleton.
     * @param headControl_ control object for TIE::Avatar's head
     * @param leftHandControl_ control object for tie::Avatar's left hand
     * @param rightHandControl_ control object for tie::Avatar's right hand
     **/
    FluidSkeleton( lifespace::Subspace * baseNode_, 
                   lifespace::FloatingActor * headControl_ = 0,
                   lifespace::FloatingActor * leftHandControl_ = 0,
                   lifespace::FloatingActor * rightHandControl_ = 0 ) :
      Skeleton( baseNode_ ),
      headControl( headControl_ ),
      leftHandControl( leftHandControl_ ),
      rightHandControl( rightHandControl_ )
    {}



    /** dtor */
    ~FluidSkeleton()
    {
      baseNode = 0;;
    }
    
    /**
     * Initializes FluidSkeleton, adds control objects given in constructor to
     * base object, stores pointer to ODESkeleton.
     *
     * @param odeSkel pointer to ODESkeleton created in Avatar
     *
     * @note currently hardcoded to tie's avatar model
     */
    void init( ODESkeleton * odeSkel );

    virtual void prepare( lifespace::real dt );

    virtual void step();



  };


}  /* namespace tie */


#endif /* TIE_FLUIDSKELETON_HPP */
