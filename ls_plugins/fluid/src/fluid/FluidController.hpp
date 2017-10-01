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
 * @file FluidController.hpp
 *
 * The Fluid controller implementation for Lifespaces Fluid plugin
 */

/**
 * @class lifespace::plugins::pfluid::FluidController
 * @ingroup fluid
 *
 * @brief
 * The Fluid device implementation for Lifespaces Fluid plugin
 *
 *
 * @todo
 * Remember to put copyrights correctly, when all the team starts developing
 */
#ifndef LS_P_FLUIDCONTROLLER_HPP
#define LS_P_FLUIDCONTROLLER_HPP



#include <cassert>
#include <string>
#include <iostream>
#include <Fluid/Config.hpp>
#include <Fluid/Processors.hpp>
#include <Fluid/MessageSink.hpp>
#include <Fluid/IONode.hpp>
#include <lifespace/Control/Controller.hpp>




namespace lifespace { namespace plugins { namespace pfluid {



  class FluidController :
    public lifespace::Controller,
    public Fluid::MessageSink
  {

  public:
    
    /** Basic control signals */
    enum Controls {
      /**
       * @name These controls are for supplying 3-D vectoral data.
       * @{ */
      CTRL_VECTOR_DIM0,
      CTRL_VECTOR_DIM1,
      CTRL_VECTOR_DIM2,
      /** @} */

      /** 
       * @name These controls are for supplying 3x3 matrix data.
       * @{ */
      CTRL_MATRIX_0_0,
      CTRL_MATRIX_0_1,
      CTRL_MATRIX_0_2,
      CTRL_MATRIX_1_0,
      CTRL_MATRIX_1_1,
      CTRL_MATRIX_1_2,
      CTRL_MATRIX_2_0,
      CTRL_MATRIX_2_1,
      CTRL_MATRIX_2_2,
      /** @} */

      ControlCount
    };


  protected:
    Fluid::Processors * network;
    std::string n_name;
    std::list< const char * > sinkNames;
    std::list< const char * > feeders;

    
  public:    
    /* ctor, dtor */
    FluidController( const std::string id,
                     Fluid::Processors * a_network ) :
      network( a_network ),
      n_name( id )
    {
      assert( network );

      std::cout << "Fluidcontroller, constructor: Starting up network: "
                << id << std::endl;

    }
    
    ~FluidController();


    void start() { network->start(); }

    /**
     * method for updating processor states last processor assigning correct
     * commands to attached actors.
     *
     * @param dt    Length of the timestep taken
     */
    void timestep();
    
    /**
     * method for processing fluid messages
     *
     * @param id     Id of the message processed (actually processors name,
     *               where mesage is coming from)
     * @param msg    Actual message sent.
     * @return    Boolean value, whether processing was successful or failure.
     */
    virtual bool msgProcess( const char *id, Fluid::Message * msg ) = 0;


    /** method for adding listener to the feeder nodes for actors */
    void linkActors( const char * nodename, const char * sinkName );


    /** method for removing listeners from feeders */
    void unlinkActors( const char * nodename, const char * sinkName );

    /** Accessor for controllers name */
    std::string * getName() { return &n_name; }


  };
    
  



}}}  /* namespace lifespace::plugins::pfluid */



#endif  /* LS_P_FLUIDCONTROLLER_HPP */
