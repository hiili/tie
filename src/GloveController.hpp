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
 * @file GloveController.hpp
 */

/**
 * @class tie::GloveController
 * 
 * @brief GloveController contains functionality to read glove input through
 * fluid and pass it on to all Actors registered to listening it.
 *
 * @see lifespace::Controller,
 *      lifespace::Actor,
 *      lifespace::plugins::pfluid::FluidController
 */



#ifndef TIE_GLOVECONTROLLER_HPP
#define TIE_GLOVECONTROLLER_HPP


#include <lifespace/plugins/fluid.hpp>
#include <Fluid/Processors.hpp>
#include <Fluid/Message.hpp>
#include <Fluid/TrackerSensorSelector.hpp>

#include <string>


namespace tie {
  
  /** Fluid's glove input, when finger is totally closed */
#define FLEXION_MIN_VAL 0.0
  /** Fluid's glove input, when finger is totally opened */
#define FLEXION_MAX_VAL 1.0
  
  class GloveController :
    public lifespace::plugins::pfluid::FluidController {
    
  private:
    lifespace::plugins::pfluid::GloveProcessor * glove;
    bool fist;
    float fistCloseThreshold;
    float fistOpenThreshold;

  public:
    enum SetControls {
      CTRL_SET_THUMB  = ControlCount + 1,
      CTRL_SET_FORE   = ControlCount + 2,
      CTRL_SET_MIDDLE = ControlCount + 3,
      CTRL_SET_INDEX  = ControlCount + 4,
      CTRL_SET_PINKY  = ControlCount + 5,
      CTRL_SET_FIST   = ControlCount + 6,
      GC_ControlCount = ControlCount + 7
    };
    
    /**
     * Constructs a new glove controller.
     * 
     * @param sensor a fluid name of the glove (defined in fluid config file)
     */
    GloveController( const std::string gloveName );
    
    /** 
     * sets the fist lower and min thresholds. If either one of the thresholds
     * is illegal (<0.0 or >5.0), preserves the old ones.
     *
     * @param close close threshold (from cumulative sum of flexions)
     * @param open open threshold (from cumulative sum of flexions)
     **/
    void setThresholds( float close, float open );

    /**
     * Method inherited from lifespace::plugins::pfluid::FluidController
     *
     * @see lifespace::plugins::pfluid::FluidController
     */
    virtual bool msgProcess( const char * id, Fluid::Message * msg );
        
  };

} /* namespace tie */


#endif  /* TIE_SENSORCONTROLLER */
