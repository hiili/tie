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
 * @file GloveProcessor.hpp
 *
 * The Fluid device implementation for Lifespaces Fluid plugin
 */

/**
 * @class lifespace::plugins::pfluid::GloveProcessor
 * @ingroup fluid
 *
 * @brief
 * Glove processor for Lifespaces Fluid plugin
 */

#ifndef LS_P_GLOVEPROCESSOR_HPP
#define LS_P_GLOVEPROCESSOR_HPP


#include <Fluid/TimeStamp.hpp>
#include <Fluid/MessageSource.hpp>
#include <Fluid/ProcessorNode.hpp>
#include <Fluid/Processors.hpp>
#include <Fluid/IONodeBuffer.hpp>
#include <Fluid/Glove.hpp>
#include <string>
#include <iostream>


namespace lifespace { namespace plugins { namespace pfluid {

  
  class GloveProcessor :
    public Fluid::MessageSource,
    public Fluid::ProcessorNodeContinuous
  {

  private:
    const std::string feeder;

    Fluid::TimeStamp lastUpdate;
    
    Fluid::IONodeBuffer< Fluid::GloveSample > * input;


  public:
    /* ctor, dtor */
    GloveProcessor(Fluid::Processors * host_, 
                   const std::string &id,
                   const std::string &feeder_ ) :
      ProcessorNodeContinuous( host_, id ),
      feeder( feeder_ ),
      input( 0 )
    {
      std::cout << "Creating processor: " << id.c_str();
    }
    virtual ~GloveProcessor() {}


    virtual bool start();
    virtual bool stop();
    
    virtual void update();
  };
    






}}} /* namespace lifespace::plugins::pfluid */







#endif /* LS_P_LOCATIONVECTORPROCESSOR_HPP */
