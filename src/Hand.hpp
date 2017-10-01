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
 * @file Hand.hpp
 *
 * Hand subspace part of Avatar's joint form
 */

/**
 * @class tie::Hand
 *
 * @brief
 * Hand subspace part of Avatar's joint form
 *
 * This class represents the hands of Avatar. It contains connectors for
 * wrist, thumb, base. Rest of fingers are yet to be implemented.
 */
 
#ifndef TIE_HAND_HPP
#define TIE_HAND_HPP
#include <lifespace/lifespace.hpp>
#include "TIEMaterials.hpp"
#include "Finger.hpp"

namespace tie {
  
  class Hand: public lifespace::Subspace
  {
  private:
    lifespace::ODEBallConnector * wrist; 
    lifespace::ODEAxleConnector * thumbConnector, * indexConnector;
    lifespace::ODEAxleConnector * middleConnector, * ringConnector;
    lifespace::ODEAxleConnector * pinkyConnector;
    lifespace::Connector * base;
    
    lifespace::shapes::Cube * geom;
    
      
    tie::Finger * thumb,* index,* middle, * ring, * pinky;
    
  public:
    enum Connectors {
      CONN_WRIST,
      CONN_THUMB,
      CONN_INDEX,
      CONN_MIDDLE,
      CONN_RING,
      CONN_PINKY,
      CONN_BASE
    };
    
    typedef enum Sides {
      RIGHT,
      LEFT
    } HandSides;
    
    /**
     * Default constructor, calls superclass constructor.
     */
    Hand();
    
    /**
     * Default destructor.
     */
    virtual ~Hand();
    
    /* operations */
    
    /**
     * Creates connectors, physical appearance and geometrical form.
     *

     * @param width The width of hand
     * @param thickness The width of hand
     * @param side The side of hand, enum Sides LEFT or RIGHT
     * @param superspace For some reason finger tip connectors won't work if
     *        we add fingers to hand-subspace.
     *        Because of this you must supply subspace 
     * that the hand is added to so fingers can be added to it too.
     */

    void create(float width, float thickness, enum Sides side, 
                Subspace * superspace);
    
    /**
     * Connect all connectors.
     */
    void assemble();

    void connect();
    void disconnect();

  };
  
} /* namespace tie */

#endif //TIE_HAND_HPP
