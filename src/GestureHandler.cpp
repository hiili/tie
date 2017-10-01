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
 * @file GestureHandler.cpp
 *
 * The GestureHandler object implementation
 */

#include "GestureHandler.hpp"

#include "Avatar.hpp"
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

// initialize necessary controls
void GestureHandler::init()
{
  controls[CTRL_HEAD_LOC_X]  = Control( Control::ContinuousMode );
  controls[CTRL_HEAD_LOC_Y]  = Control( Control::ContinuousMode );
  controls[CTRL_HEAD_LOC_Z]  = Control( Control::ContinuousMode );
  controls[CTRL_RHAND_LOC_X] = Control( Control::ContinuousMode );
  controls[CTRL_RHAND_LOC_Y] = Control( Control::ContinuousMode );
  controls[CTRL_RHAND_LOC_Z] = Control( Control::ContinuousMode );
  controls[CTRL_LHAND_LOC_X] = Control( Control::ContinuousMode );
  controls[CTRL_LHAND_LOC_Y] = Control( Control::ContinuousMode );
  controls[CTRL_LHAND_LOC_Z] = Control( Control::ContinuousMode );
}

GestureHandler::GestureHandler ( tie::Avatar * avatar_ ):
  Object(),
  avatar( avatar_ )
{
  init();
}

void GestureHandler::localPrepare( real dt_ )
{}

void GestureHandler::localStep()
{}

void GestureHandler::prepare( real dt_ )
{
  localPrepare( dt_ );
  Actor::prepare( dt_ );
  Object::prepare( dt_ );
}

void GestureHandler::step()
{
  localStep();
  Actor::step();
  Object::step();
}
