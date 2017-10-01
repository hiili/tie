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
 * @file types.hpp
 *
 * General type and utility classes and macros.
 */
#ifndef TIE_TYPES_HPP
#define TIE_TYPES_HPP




/* Some constants and macros. */
//#define ...

/** EVE CAVE dimension left */
#define EVE_CUBE_X_MIN -4.5

/** EVE CAVE dimension right */
#define EVE_CUBE_X_MAX  4.0

/** EVE CAVE dimension bottom */
#define EVE_CUBE_Y_MIN  1.6

/** EVE CAVE dimension up */
#define EVE_CUBE_Y_MAX  5.9

/** EVE CAVE dimension forward */
#define EVE_CUBE_Z_MIN -4.6

/** EVE CAVE dimension back */
#define EVE_CUBE_Z_MAX  5.0

/** connector id range start (when defined outside of lifespace's object
    inheritance) */
#define TIE_DYNAMIC_CONNECTORS_RANGE_START 0x71300000

/** connector id range end (when defined outside of lifespace's object
    inheritance) */
#define TIE_DYNAMIC_CONNECTORS_RANGE_END   0x71310000


namespace tie {
  
  
  
  
  
  
  
  
}   /* namespace tie */




#endif   /* TIE_TYPES_HPP */
