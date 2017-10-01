/*
 * Copyright (C) 2004-2005 Eero Uusitalo, Ville Heikkilä, Mikko Ylikangas, Paul
 * Wagner, Juho Heiskari, Kati Määttä, Mikko Purtonen.
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
 * @file vrjuggler.hpp
 *
 * The VR Juggler plugin for the Lifespace Simulator.
 */

/**
 * @defgroup vrjuggler vrjuggler
 * @ingroup plugins
 *
 * The VR Juggler plugin for the Lifespace Simulator.
 *
 * The plugin uses VR Juggler to implement the following Lifespace
 * Simulator classes:
 *   - Device (VRJDevice): VRJDevice represents the whole VR Juggler system. It
 *     can contain multiple VRJViewport objects.
 *   - Viewport (VRJViewport): This represents a single rendering area (usually
 *     a wall in a virtual reality environment).
 *
 * \par Usage
 * The VRJDevice class is implemented as a singleton class and it represents
 * the whole VR Juggler system. VRJViewports represent individual rendering
 * targets (walls in a virtual reality environment, for example) and they are
 * instantiated automatically by the VRJDevice after VR Juggler has read its
 * configuration file.
 *
 * \par Automatic generation of display lists
 * Display lists can be generated automatically for all visuals. This can be
 * done with the VRJDevice's setAutoDisplaylisting() method. Note, however,
 * that enabling this will cause all dynamic changes to visuals to have no
 * effect!
 *
 * \par Example:
 * @code
 *   // init the VR Juggler kernel from the given configuration file
 *   VRJDevice::Init( string("vrjuggler.config") );
 *
 *   // Enable automatic generation of display lists.
 *   VRJDevice::instance()->setAutoDisplaylisting( true );
 *
 *   // create and fill a world
 *   ODEWorld world;
 *   ...
 *
 *   // create a camera and connect it to VR Juggler
 *   // (only a single common camera is allowed for VR Juggler viewports,
 *   // so it is set through the VRJDevice)
 *   Camera camera;
 *   camera.setTargetObject( &camObject );
 *   VRJViewport::SetCamera( &camera );
 *
 *   // set a default timestep length and make the world timestep automatically
 *   // on each display refresh
 *   world.setDefaultDt( 0.05 );
 *   VRJDevice::instance()->events.addListener( &world );
 *   VRJDevice::MainLoop();
 * @endcode
 */
#ifndef LS_P_VRJUGGLER_HPP
#define LS_P_VRJUGGLER_HPP


#include "vrjuggler/VRJDevice.hpp"
#include "vrjuggler/VRJViewport.hpp"


namespace lifespace { namespace plugins { namespace pvrjuggler {
  
  
  
  
  
  
  
  
 }}}   /* lifespace::plugins::pvrjuggler */




#endif   /* LS_P_VRJUGGLER_HPP */
