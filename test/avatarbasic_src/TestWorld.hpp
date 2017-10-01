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
 * @file TestWorld.hpp
 *
 * The world object.
 */

/**
 * @class tie::TestWorld
 *
 * @brief
 * The world object.
 *
 * Contains some objects and an own camera which can be activated, if using
 * GLOW, by pressing '1' (this is handled in main_glow.cpp).
 */
#ifndef TIE_STARTUP_TESTWORLD_HPP
#define TIE_STARTUP_TESTWORLD_HPP


#include <lifespace/lifespace.hpp>
#include <lifespace/plugins/fluid.hpp>

#include <boost/shared_ptr.hpp>
#include <vector>




namespace tie {
  
  
  
  
  class TestWorld :
    public lifespace::ODEWorld
  {
    
  public:
    
    typedef enum cameraModes {
      CAM_MODE_FOLLOW,
      CAM_MODE_ACTOR
    } CamMode;
    
    struct CameraEntry {
      boost::shared_ptr<lifespace::Camera> camera;
      lifespace::Actor * actor;
      lifespace::Object * object;
      boost::shared_ptr<lifespace::Controller::ControlMap> controlMap;
      CamMode mode;
    };
    typedef std::vector<CameraEntry> cameras_t;
    
    
  private:
    
    lifespace::ODECollisionRenderer * collisionRenderer;
    cameras_t cameras;
    
    
  public:
    
    /* constructors/destructors/etc */
    
    /**
     * Creates a new test world which contains a single light source and a
     * basis object.
     *
     * @param graphicsEventHost   A reference to the graphics device's
     *                            EventHost.
     */
    TestWorld( lifespace::EventHost<lifespace::GraphicsEvent> &
               graphicsEventHost, 
               lifespace::plugins::pfluid::FluidHost * inputHost );
    
    /** */
    virtual ~TestWorld();
    
    /* accessors */
    
    cameras_t * getCameras()
    { return &cameras; }
    
    
    /* operations */
    
  };
  
  
  
  
}   /* namespace tie */




#endif   /* TIE_STARTUP_TESTWORLD_HPP */
