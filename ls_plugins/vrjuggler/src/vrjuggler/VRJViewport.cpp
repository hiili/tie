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
 * @file VRJDevice.cpp
 */


#include <lifespace/types.hpp>
#include <lifespace/Structures/Event.hpp>
using namespace lifespace;

#include "VRJDevice.hpp"
#include "VRJViewport.hpp"
using namespace lifespace::plugins::pvrjuggler;

#include <Kernel/vjKernel.h>
#include <Kernel/GL/vjGlApp.h>

#include <iostream>




const Camera * VRJViewport::camera = 0;




/**
 * Place all extra low-level OpenGL context init stuff here. Currently this
 * only forwards the call to the underlying Viewport::initContext() method.
 */
void VRJViewport::initContext()
{
  Viewport::initContext();
}




VRJViewport::VRJViewport( VRJDevice & parentDevice ) :
  Viewport( parentDevice )
{}


VRJViewport::~VRJViewport()
{}




void VRJViewport::draw()
{
  // send the graphics events
  GraphicsEvent event1 = { GE_VSYNC, 0 };
  events.sendEvent( &event1 );
  GraphicsEvent event2 = { GE_REFRESH_BEGIN, 0 };
  events.sendEvent( &event2 );
  
  
  // init the graphics context
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // set the common VR Juggler camera to this viewport
  Viewport::setCamera( VRJViewport::GetCamera() );
  
  // render the actual contents
  applyCameraToGfx();
  
  
  // send a graphics event
  GraphicsEvent event3 = { GE_REFRESH_END, 0 };
  events.sendEvent( &event3 );
}
