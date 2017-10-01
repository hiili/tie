/**
 * @file SensorController.hpp
 */

#ifndef FLUID_BASICS_SENSORCONTROLLER_HPP
#define FLUID_BASICS_SENSORCONTROLLER_HPP


#include <lifespace/plugins/fluid.hpp>
#include <Fluid/Processors.hpp>
#include <Fluid/Message.hpp>
#include <Fluid/TrackerSensorSelector.hpp>

#include <string>


class SensorController :
  public lifespace::plugins::pfluid::FluidController {

private:
  Fluid::TrackerSensorSelector * source;
  lifespace::plugins::pfluid::LocationVectorProcessor * loc;
  lifespace::plugins::pfluid::RotationMatrixProcessor * rot;

public:
  enum SetControls {
    CTRL_SETLOC,
    CTRL_SETBASIS
  };
  
  SensorController( const std::string sensor );

  virtual bool msgProcess( const char * id, Fluid::Message * msg );

};


#endif  /* FLUID_BASICS_SENSORCONTROLLER */
