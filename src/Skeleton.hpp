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
 * @file Skeleton.hpp
 *
 * Skeleton is the base class for user representation in TIE structure.
 */

/**
 * @class tie::Skeleton
 *
 * @brief
 * Skeleton is the base class for any modeled skeletal structure with clearly
 * defined base.
 */

#ifndef TIE_SKELETON_HPP
#define TIE_SKELETON_HPP

#include <lifespace/lifespace.hpp>

#include <list>
#include <iostream>


namespace tie {


  class Skeleton
  {
  protected:
    lifespace::Subspace * baseNode;

  public:
    /** ctor */
    Skeleton() {}
    
    /**
     * ctor
     * 
     * @param baseNode_ basenode for skeleton
     */
    Skeleton( lifespace::Subspace * baseNode_ ):
      baseNode( baseNode_ )
    {}

    /** dtor */
    virtual ~Skeleton()
    {
      baseNode = 0; 
    }

    
    /**
     * Accessor method for Skeletons SG
     *
     * @return pointer to SG
     */
    lifespace::Object * getBaseNode() { return baseNode; }

    /**
     * Override this, if you want to connect physically controlled skeleton to
     * a static, input driven skeleton.
     * 
     * @param master master skeleton with whom current skeleton is aligned.
     */
    virtual void connect( Skeleton * master ) {};

    /**
     * Functionality for connecting skeleton to lifespace's prepare -cycle.
     * 
     * @param dt Timestep length to be prepared.
     */
    virtual void prepare( lifespace::real dt ) {};

    /**
     * Functionality for connecting skeleton to lifespace's step -cycle.
     */
    virtual void step() {}
  };


}  /* namespace tie */


#endif /* TIE_SKELETON_HPP */
