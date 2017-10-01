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
 * @file ODEFinger.hpp
 *
 * Finger subspace part of Avatar's simplified form.
 */

/**
 * @class tie::ODEFinger
 *
 * @brief
 * Finger subspace part of Avatar's simplified form.
 *
 * This class represents the fingers of Avatar. Fingers are made from two
 * parts and are static i.e. have lifespace::BasicLocator as locator. 
 *
 * @see lifespace::BasicLocator 
 */
 
#ifndef TIE_ODEFINGER_HPP
#define TIE_ODEFINGER_HPP
#include <lifespace/lifespace.hpp>
#include "TIEMaterials.hpp"

namespace tie
{

  class ODEFinger: public lifespace::Subspace
  {
  public:
    enum Sides {
      LEFT = -1,
      RIGHT = 1
    };
    
    lifespace::Object * tip;
  
  private:
    enum Sides side;

  public:
    /**
     * Default constructor, calls superclass constructor.
     * 
     * @param a_isThumb Is this finger a thumb. Needed for tip-stub axle
     * connector orientation.
     */	
    ODEFinger( lifespace::Object::Params params );


    /**
     * Default destructor.
     */
    virtual ~ODEFinger()
    {
    }

    /**
     * Update method for updating finger configuration.
     *
     * @note _MUST_ only be called during corresponding hands prepare-pass
     *       for now
     */
    virtual void update( lifespace::Vector fwd,
                         lifespace::Vector up );

  };
  
}    /* namespace tie */

#endif //TIE_ODEFINGER_HPP
