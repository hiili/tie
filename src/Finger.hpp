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
 * @file Finger.hpp
 *
 * Finger subspace part of Avatar's joint form.
 */

/**
 * @class tie::Finger
 *
 * @brief
 * Finger subspace part of Avatar's joint form.
 *
 * This class represents the fingers of Avatar. Fingers are made from two
 * parts connected with ball connector. It contains connectors for 
 * hand and bases of stub and tip (first and second part of finger).
 * Call Assemble-method to connect parts together.
 */
 
#ifndef TIE_FINGER_HPP
#define TIE_FINGER_HPP
#include <lifespace/lifespace.hpp>
#include "TIEMaterials.hpp"

namespace tie
{

  class Finger: public lifespace::Subspace
  {
  public:
    enum Sides {
      RIGHT,
      LEFT
    };
  
  private:
    lifespace::ODEAxleConnector * hand;
    lifespace::ODEAxleConnector * stub_to_tip;
    //lifespace::ODEBallConnector * stub_to_tip;
    lifespace::ODEAxleConnector * tip_to_stub;
    //lifespace::ODEBallConnector * tip_to_stub;
    lifespace::Connector * stubBase;
    lifespace::Connector * tipBase;
    lifespace::Object * tip;
  
    bool isThumb;
    enum Sides side;

  public:
    enum Connectors {
      CONN_HAND,
      CONN_STUB_BASE,
      CONN_TIP_BASE
    };


    /**
     * Default constructor, calls superclass constructor.
     * 
     * @param a_isThumb Is this finger a thumb. Needed for tip-stub axle
     * connector orientation.
     */	
    Finger(
            bool a_isThumb = false, enum Sides a_side = LEFT );
            
    /**
     * Default destructor.
     */
    virtual ~Finger();
  
    /**
     * Creates connectors, physical appearance and geometrical form.
     *
     * @param length The total length of finger
     * @param size The thickness of finger
     */
    void create(float length, float size);
  
    /**
     * Connect the parts of finger together.
     */
    void assemble();
  };
  
}

#endif //TIE_FINGER_HPP
