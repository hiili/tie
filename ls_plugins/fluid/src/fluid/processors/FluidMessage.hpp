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
 * @file FluidMessage.hpp
 *
 * The Fluid Message implementation for Lifespaces Fluid plugin
 */

/**
 * @class lifespace::plugins::pfluid::FluidMessage
 * @ingroup fluid
 *
 * @brief
 * The Fluid message for passing data to Lifespaces Fluid plugin
 */

#ifndef LS_P_FLUIDMESSAGE_HPP
#define LS_P_FLUIDMESSAGE_HPP


#include <Fluid/Message.hpp>
#include <Fluid/Glove.hpp>
#include <Fluid/TimeStamp.hpp>


namespace lifespace { namespace plugins { namespace pfluid {

  template< class T >
  class FluidMessage :
    public Fluid::Message
  {
  
  private:
    T sample;
  

  public:
    /* ctor(s), dtor */
    FluidMessage( T sample_ ) :
      Message(),
      sample( sample_ )
    {}

    FluidMessage( const Fluid::TimeStamp &time, T sample_ ) :
      Message( time ),
      sample( sample_ )
    {}


    virtual ~FluidMessage()
    {
      delete &sample;
    }
  
    T * getContent() { return &sample; }

  };

}}} /* namespace lifespace::plugins::pfluid */






#endif  /* LS_P_VECTORMESSAGE_HPP */
