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
 * @file VRJDevice.hpp
 *
 * The Graphics Device implementation of the VR Juggler plugin.
 */

/**
 * @class lifespace::plugins::pvrjuggler::VRJDevice
 * @ingroup vrjuggler
 *
 * @brief
 * The Graphics Device implementation of the VR Juggler plugin.
 *
 * \par Graphics Events
 * The following events are emitted by the VRJDevice:
 *   - GE_VSYNC: is sent from the intraFrame() callback. Execution does \em not
 *     block while the event is being processed (but the listeners are still
 *     handled sequentially, one a time).
 *   - GE_REFRESH_BEGIN: is sent from the preFrame() callback. Execution blocks
 *     while the event is being processed.
 *   - GE_REFRESH_END: is sent from the postFrame() callback. Execution blocks
 *     while the event is being processed.
 */
#ifndef LS_P_VRJUGGLER_VRJDEVICE_HPP
#define LS_P_VRJUGGLER_VRJDEVICE_HPP


#include <lifespace/types.hpp>
#include <lifespace/Graphics/Device.hpp>
#include <lifespace/Structures/Event.hpp>

#include <Kernel/vjKernel.h>
#include <Kernel/GL/vjGlApp.h>
#include <Kernel/GL/vjGlContextData.h>

#include <string>
#include <vector>
#include <cassert>


namespace lifespace { namespace plugins { namespace pvrjuggler {
  
  
  
  
  /* forwards */
  class VRJViewport;
  
  
  
  
  class VRJDevice :
    public Device,
    public vjGlApp
  {
    /** Singleton instance of this class. */
    static VRJDevice * theInstance;
    
    
    /** The VR Juggler kernel. */
    vjKernel * kernel;
    
    /** Viewports. */
    vjGlContextData<VRJViewport *> viewports;
    
    /** Should the new viewports use automatic generation of display lists? */
    bool autoDisplaylisting;
    
    
    /** Private constructor for the singleton instance. */
    VRJDevice( const std::string & configFile );
    
    /** Must not be deleted from outside. */
    virtual ~VRJDevice() {}
    
    
    
    
    
    
  public:
    
    /**
     * Returns the singleton instance of the class, or null if not yet
     * initialized with Init().
     */
    static VRJDevice * instance()
    { return theInstance; }
    
    /**
     * Initializes the VR Juggler kernel. This must be called exactly once
     * before using any other VR Juggler functionality. After this the VR
     * Juggler kernel is running and instance() method returns a valid instance
     * of this class.
     */
    static void Init( const std::string & configFile );
    
    /**
     * Enters the windowing system's mainloop. This will return when the user
     * closes the program (by closing the window or pressing ctrl-c).
     */
    static void MainLoop();
    
    /**
     * Returns all VRJViewports as an STL vector. Note that the viewports are
     * not available before entering the VRJDevice's mainloop.
     *
     * @warning
     * This uses the VR Juggler's vjGlContextData::getDataVector method, which
     * in turn should not be used during rendering!
     */
    std::vector<VRJViewport **> & getVRJViewports()
    { return *viewports.getDataVector(); }
    
    /**
     * This controls whether all encountered shapes should be automatically
     * wrapped into a display list. Note that this cannot be changed after
     * viewports have been created!
     *
     * @warning
     * If this is turned on, then the generated display lists cannot be
     * released or updated in any way! This implies also that dynamic
     * modifications to the shapes and visuals do not affect this renderer
     * anymore. Use the Precomputed -shape instead to manually control the
     * creation of display lists.
     */
    void setAutoDisplaylisting( bool newState )
    {
      // this cannot be used after the viewports have been created
      assert( viewports.getDataVector()->size() == 0 );
      
      autoDisplaylisting = newState;
    }
    
    bool getAutoDisplaylisting() const
    { return autoDisplaylisting; }
    
    
    /* VR Juggler callbacks */
    
    virtual void init();
    
    virtual void preFrame();
    
    virtual void intraFrame();
    
    virtual void postFrame();
    
    virtual void contextInit();
    
    virtual void contextClose();
    
    virtual void draw();
    
  };




 }}}   /* namespace lifespace::plugins::pvrjuggler */


#endif   /* LS_P_VRJUGGLER_VRJDEVICE_HPP */
