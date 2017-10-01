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
 * @file FluidHost.hpp
 *
 * The Fluid device implementation for Lifespaces Fluid plugin
 */

/**
 * @class lifespace::plugins::pfluid::FluidHost
 * @ingroup fluid
 *
 * @brief
 * The Fluid Controllers Host implementation for Lifespaces Fluid plugin
 *
 * \par Graphics events
 * FluidHost is able to listen and process graphics events hosted by GLOW or
 * VRJUggler plugin of the lifespace.
 */
#ifndef LS_P_FLUIDHOST_HPP
#define LS_P_FLUIDHOST_HPP




#include <lifespace/types.hpp>
#include <lifespace/Graphics/types.hpp>
#include <lifespace/Structures/Event.hpp>
#include "FluidController.hpp"
#include <list>




namespace lifespace { namespace plugins { namespace pfluid {


  class FluidHost :
    public lifespace::EventListener<GraphicsEvent>
  {
    
  private:
    std::list< FluidController * > controllers;
    std::string config_file;

    /**
     * initialization method for the whole fluid input system. Bails out, if
     * initialization fails.
     */
    void init();

  public:


    /* ctor, dtor */
    FluidHost( const std::string conf_file ) :
      config_file( conf_file )
    { init(); }

    virtual ~FluidHost();


    /**
     * method for terminating fluid processing
     *
     * @note
     * Call this only once when quitting the application.
     */
    void terminate();
    
    void start();   
    
    /** 
     * method for adding already created. Note after invoking this,
     * responsibility of the controller is transferred to FluidHost
     * instance. If adding fails, a message is printed to screen.
     */
    void addController( FluidController * contr );


    /**
     * Callback method for handling refresh events coming from display
     * routines.
     * 
     * @param event    GraphicsEvent casted by graphics device
     */
    virtual void processEvent( const GraphicsEvent * event );

  };



}}} /* namespace lifespace::plugins::pfluid */


#endif /* LS_P_FLUIDHOST_HPP */
