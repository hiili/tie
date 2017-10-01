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
 * @file VRJViewport.hpp
 *
 * The Graphics Viewport implementation of the VR Juggler plugin.
 */

/**
 * @class lifespace::plugins::pvrjuggler::VRJViewport
 * @ingroup vrjuggler
 *
 * @brief
 * The Graphics Viewport implementation of the VR Juggler plugin.
 *
 * \par Graphics Events
 * The following events are emitted by the VRJViewport:
 *   - GE_VSYNC: is sent immediately after entering this viewport's draw()
 *     callback. Execution blocks while the event is being processed.
 *   - GE_REFRESH_BEGIN: is sent from the draw() callback before
 *     drawing. Execution blocks while the event is being processed.
 *   - GE_REFRESH_END: is sent from the draw() callback after
 *     drawing. Execution blocks while the event is being processed.
 */
#ifndef LS_P_VRJUGGLER_VRJVIEWPORT_HPP
#define LS_P_VRJUGGLER_VRJVIEWPORT_HPP


#include <lifespace/types.hpp>
#include <lifespace/Graphics/Viewport.hpp>

#include "VRJDevice.hpp"

#include <Kernel/vjKernel.h>
#include <Kernel/GL/vjGlApp.h>

#include <list>
#include <cassert>




namespace lifespace {

  
  /* forwards */
  class Camera;
  
  
namespace plugins { namespace pvrjuggler {
  
  
  
  
  class VRJViewport :
    public Viewport
  {
    
    /** The current camera (the VR Juggler plugin allows only one camera to be
        used for all viewports). */
    static const Camera * camera;
    
    
    /** */
    virtual void initContext();
    
    
  public:
    
    /**
     * Sets the camera whose OpenGL stream is rendered to all VR Juggler
     * viewports. If a camera is already set, then the new one will replace the
     * old one. Giving a null pointer disables rendering.
     */
    static void SetCamera( const Camera * newCamera )
    { camera = newCamera; }

    /**
     * Returns the current camera, or null if no camera is attached.
     */
    static const Camera * GetCamera()
    { return camera; }
    
    
    /** */
    VRJViewport( VRJDevice & parentDevice );
    
    /** */
    virtual ~VRJViewport();
    
    /** Not available with VR Juggler (will assert fail). */
    virtual void close() { assert(false); }
    
    
    /** */
    virtual void draw();
    
  };
  
  
  
  
 }}}   /* namespace lifespace::plugins::pvrjuggler */


#endif   /* LS_P_VRJUGGLER_VRJVIEWPORT_HPP */
