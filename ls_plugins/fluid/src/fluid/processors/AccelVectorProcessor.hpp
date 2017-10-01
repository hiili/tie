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
 * @file AccelVectorProcessor.hpp
 *
 * The Fluid device implementation for Lifespaces Fluid plugin
 */

/**
 * @class lifespace::plugins::pfluid::AccelVectorProcessor
 * @ingroup fluid
 *
 * @brief
 * Acceleration vector calculating processor for Lifespaces Fluid plugin
 */

#ifndef LS_P_ACCELVECTORPROCESSOR_HPP
#define LS_P_ACCELVECTORPROCESSOR_HPP


#include <Fluid/TimeStamp.hpp>
#include <Fluid/MessageSource.hpp>
#include <Fluid/ProcessorNode.hpp>
#include <Fluid/Processors.hpp>
#include <Fluid/IONodeBuffer.hpp>
#include <Fluid/Vector3.hpp>
#include <string>
#include <iostream>


namespace lifespace { namespace plugins { namespace pfluid {

  
  class AccelVectorProcessor :
    public Fluid::MessageSource,
    public Fluid::ProcessorNodeContinuous
  {

  private:
    const std::string feeder;

    float lastDt;
    Fluid::TimeStamp lastUpdate;
    Fluid::Vector3 lastPos;
    Fluid::Vector3 lastVel;
    
    Fluid::IONodeBuffer< Fluid::Vector3 > * input;


  public:
    /* ctor, dtor */
    AccelVectorProcessor(Fluid::Processors * host_, 
                         const std::string &id,
                         const std::string &feeder_ ) :
      ProcessorNodeContinuous( host_, id ),
      feeder( feeder_ ),
      input( 0 )
    {
      std::cout << "Creatingting processor: " << id.c_str();
    }
    virtual ~AccelVectorProcessor() {}


    virtual bool start();
    virtual bool stop();
    
    virtual void update();
  };
    






}}} /* namespace lifespace::plugins::pfluid */







#endif /* LS_P_ACCELVECTORPROCESSOR_HPP */
