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
 * @file SensorController.hpp
 */

/**
 * @class tie::SensorController
 * 
 * @brief Sensorcontroller contains functionality to read MotionStar Tracker
 * sensor input through fluid and pass it on to all Actors registered to
 * listening it.
 *
 * @see lifespace::Controller,
 *      lifespace::Actor,
 *      lifespace::plugins::pfluid::FluidController
 */

#ifndef TIE_SENSORCONTROLLER_HPP
#define TIE_SENSORCONTROLLER_HPP


#include <lifespace/plugins/fluid.hpp>
#include <Fluid/Processors.hpp>
#include <Fluid/Message.hpp>
#include <Fluid/TrackerSensorSelector.hpp>
#include <Fluid/Matrix3.hpp>

#include <lifespace/lifespace.hpp>

#include <string>


namespace tie {

  class SensorController :
    public lifespace::plugins::pfluid::FluidController {
    
  private:
    Fluid::TrackerSensorSelector * source;
    lifespace::plugins::pfluid::LocationVectorProcessor * loc;
    lifespace::plugins::pfluid::RotationMatrixProcessor * rot;
    Fluid::Matrix3 * currRot;
    
    Fluid::Vector3 relTranslation;
    bool isTranslated;
    
  public:

    enum SetControls {
      CTRL_SETLOC = ControlCount,
      CTRL_SETBASIS = ControlCount + 1,
      
      SC_ControlCount = ControlCount + 2
    };
    
    /**
     * Constructs a new sensor controller.
     * 
     * @param sensor a fluid name of the sensor (defined in fluid config file)
     */
    SensorController( const std::string sensor );
    
    /**
     * sets location translation for sensor location in fluid coordinates
     *
     * @param trans translation vector in absolute world coordinates
     */
    void setLocTranslationFluid( lifespace::Vector trans );

    /**
     * sets location translation for sensor location relative to sensors
     * coordinates 
     *
     * @param trans translation vector in relative sensor coordinates
     */
    void setLocTranslationRel( lifespace::Vector trans );
    
    /**
     * Method inherited from lifespace::plugins::pfluid::FluidController
     *
     * @see lifespace::plugins::pfluid::FluidController
     */
    virtual bool msgProcess( const char * id, Fluid::Message * msg );
    
  };

} /* namespace tie */


#endif  /* TIE_SENSORCONTROLLER */
