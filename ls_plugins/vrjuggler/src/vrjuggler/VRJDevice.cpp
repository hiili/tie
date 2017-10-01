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
#include <lifespace/Renderers/OpenGLRenderer/OpenGLRenderer.hpp>
using namespace lifespace;

#include "VRJDevice.hpp"
#include "VRJViewport.hpp"
using namespace lifespace::plugins::pvrjuggler;

#include <Kernel/vjKernel.h>
#include <Kernel/GL/vjGlApp.h>

#include <string>




VRJDevice * VRJDevice::theInstance = 0;




VRJDevice::VRJDevice( const std::string & configFile ) :
  kernel( vjKernel::instance() ),
  autoDisplaylisting( false )
{
  // load a single config file
  kernel->loadConfigFile( configFile.c_str() );
  
  // start the kernel
  kernel->start();
}


void VRJDevice::Init( const std::string & configFile )
{
  // create and init the singleton instance
  assert( theInstance == 0 );
  theInstance = new VRJDevice( configFile );
}


void VRJDevice::MainLoop()
{
  assert( instance() );
  
  // activate this application into VR Juggler kernel
  instance()->kernel->setApplication( instance() );
  
  // sleep until user aborts
  while(true) usleep( 100000 );
}




void VRJDevice::init()
{
}


void VRJDevice::preFrame()
{
  // send a graphics event
  GraphicsEvent event = { GE_REFRESH_BEGIN, 0 };
  events.sendEvent( &event );
}


void VRJDevice::intraFrame()
{
  // send a graphics event
  GraphicsEvent event = { GE_VSYNC, 0 };
  events.sendEvent( &event );
}


void VRJDevice::postFrame()
{
  // send a graphics event
  GraphicsEvent event = { GE_REFRESH_END, 0 };
  events.sendEvent( &event );
}




void VRJDevice::contextInit()
{
  *viewports = new VRJViewport( *this );
  
  ((OpenGLRenderer *)(*viewports)->getRenderer())->setAutoDisplaylisting
    ( autoDisplaylisting );
}


void VRJDevice::contextClose()
{
  delete *viewports; *viewports = 0;
}


void VRJDevice::draw()
{ (*viewports)->draw(); }
