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
 * @file Chain.hpp
 *
 * Chain object.
 */

/**
 * @class tie::Chain
 *
 * @brief
 * Chain object.
 */
#ifndef TIE_CHAIN_HPP
#define TIE_CHAIN_HPP


#include <lifespace/lifespace.hpp>
#include <boost/shared_ptr.hpp>




namespace tie {
  
  
  
  
  class Chain :
    public lifespace::Subspace
  {
    boost::shared_ptr<lifespace::Shape> visualShape;
    boost::shared_ptr<lifespace::Shape> collisionShape;
    boost::shared_ptr<lifespace::Visual> visual;
    boost::shared_ptr<lifespace::Geometry> geometry;
    Chain * tail;
    
    
  public:
    
    struct LinkDimensions {
      float length, width, thickness;
    };
    
    enum Connectors {
      CONN_BASE,
      CONN_TIP
    };
    
    /**
     * Creates a new chain and leaves it in unconnected state.
     */
    Chain( const LinkDimensions & link, int linkCount,
           const lifespace::Material * visualMaterial,
           boost::shared_ptr<const lifespace::CollisionMaterial>
           collisionMaterial, lifespace::Subspace & hostSpace,
           Chain * firstLink = 0 );
    
    
    /**
     * Builds the chain and connects it to the baseConnector, if given. 
     */
    void connect( lifespace::Connector * baseConnector );
    
  };
  
  
  
  
}   /* namespace tie */



#endif   /* TIE_CHAIN_HPP */
