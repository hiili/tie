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
 * @file fluid.hpp
 *
 * The fluid plugin for the Lifespace Simulator.
 */

/**
 * @defgroup fluid fluid
 * @ingroup plugins
 *
 * The fluid plugin for the Lifespace Simulator.
 *
 * Plugin uses Fluid user input processing mechanisms to implement EVE virtual
 * environment user control over lifespace simulation. There are two main
 * classes and three helper processors currently implemented in plugin.
 *
 * \par Modules and usage
 *
 * FluidHost is the main class of this plugin. It hendles initialization when
 * application starts and termination when it stops. After instatiating
 * FluidHost application, its init() method must be called only once. Init
 * starts input devices and prepares fluid for processing. When application
 * wants to quit, method terminate() must be called also exactly once.
 *
 * FluidController is a class which inheritance from lifaspaces Controller
 * allows it to manipulate multiple Actors. There can be multiple
 * FluidControllers functioning in one FluidHost, and each controller may
 * process several processor outputs. FluidController is also inherited from
 * Fluids class MessageSink. This allows it to register itself to
 * MessageSources and thus receive fluid messages.
 *
 * AccelVectorProcessor takes 3d -vectors (Fluid Vector3, to be exact) and
 * calculates acceleration of the values within vector. It functions as a Fluid
 * system message source and can thus invoke actions among its
 * listeners. Messages it casts are of type VectorMessage, the fourth classtype
 * implemented so far. It is child to Fluid::Message class.
 *
 * Rest of processors are of same breed. LocationVectorProcessor passes fluid
 * sensors' input forward with same mechanism as Accel-one. Rotation passes
 * matrices and GloveProcessor arrays of five elements.
 *
 * \par An example of usage of the plugin
 * Creating one fluid sensor controlled object in single threaded environment.
 *  - Include this file in your application.
 *  - Instantiate one FluidHost with constructor parameter being typical fluid
 *    configuration file.
 *  - Call hosts init method and set its default timedifference with setDefaultDt
 *    method.
 *  - Inherit your own controller from FluidController and instantiate it.
 *  - Create your fluid processor network and instantiate a new OwnController
 *    with its name and the network as ctor arguments.
 *  - Link your controller to the control data feeder (one or more) with
 *    linkActors method
 *  - Create lifespace actor, fill its keymap with your choice of controls and
 *    add it with fluidControllers addActor function.
 *  - Add the created controller to the fluidHost
 *  - Register the host as the visualization windows (Juggler or GLOW)
 *    evenlistener (this won't be necessary, if you run Plugin multithreaded)
 *  - When your program finishes, clear allocated recources by calling
 *    terminate() to the FluidHost.
 *
 * Next chapter will be in finnish for the TIE developers and users.

 \section Rakenne
 Fluid-plugin koostuu kahdesta toiminnallisuuden rungosta: FluidHost ja
 FluidController. Luokkien toiminta perustuu fluid:n viestinvälitykseen ja
 synkronointi lifespaceen tapahtuu kytkemällä FluidHost lifespacen
 grafiikanpäivityssykliin.
 
 \section fluidhost FluidHost
 FluidHost on tarkoitettu initialisoitavaksi ja käynnistettäväksi ainoastaan
 kerran koko sovelluksen aikana. Se suorittaa kaiken staattisen
 alustustoiminnallisuuden ja toimii varastona FluidController -luokan
 instansseille.
 
 FluidHost ei tällä hetkellä pyöri omassa threadissaan, vaan jakaa
 suoritussäikeen yhdessä pääsovelluksen kanssa. Tämän takia luokan suoritus
 täytyy synkronisoida. Synkronointi suoritetaan lisäämällä FluidHost luokan
 instanssi kuuntelijaksi lifespacen grafiikanpiirtoentiteettiin (tätä
 kirjoitettaessa GlowWindow- tai VRJWindow -luokan instanssi).
 
 \section fluidcontroller FluidController
 Tämä luokka on tarkoitettu fluid:n syötteen välittämiseen lifespace:n
 tietoon. Luokka on tarkoitettu pohjaksi, josta perimällä ja msgProcess
 ylikirjoittamalla voidaan toiminnallisuus yksilöidä tiettyä käyttötapausta
 varten. Cotnrollerit sisältävät tarvitsemansa prosessoriverkon, jotka
 lähettävät viestin tarvittaessa. FluidController havaitsee tämän ja käsittelee
 viestin haluamallaan tavalla. Lisätietoja lifespacen Controller
 toiminnallisuudesta löytyy lifespacen käyttäjän ohjeesta (erillinen
 dokumentti verkossa).
 
 \section esim Käytön esimerkki
 Seuraava koodipätkä on TIE:n toteutukseen käytetystä SensorController
 -luokasta. Luokka on peritty FluidControllerista ja se hoitaa sensorin paikka-
 ja orientaatioinformaation välityksen lifespace:n tietyille Actor -Luokan
 instansseille.
 
 \code

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
    
    SensorController( const std::string sensor );
    
    // sets location translation for sensor location in fluid coordinates
    void setLocTranslationFluid( lifespace::Vector trans );

    // sets location translation for sensor location relative to sensors
    // coordinates
    void setLocTranslationRel( lifespace::Vector trans );
    
    virtual bool msgProcess( const char * id, Fluid::Message * msg );
    
  };

} // namespace tie


#endif  // TIE_SENSORCONTROLLER



\section kaant Kääntyminen

\endcode

Metodin msgprocess ja konstruktorin toteutukset. Konstruktorissa käytetään
fluid-pluginin alta löytyviä yksinkertaisia prosessoreja.

Konstruktorin toteutus:

\code
SensorController::SensorController( const std::string sensor ) :
  FluidController( sensor, new Fluid::Processors() ),
  relTranslation( Vector3(0.0,0.0,0.0) ),
  isTranslated( false )
{
  // Create processors
  source =
    new TrackerSensorSelector( network,
                               sensor,
                               "tracker",
                               sensor );


  loc =
    new LocationVectorProcessor( network,
                                 sensor + "/loc",
                                 sensor + "/location" );
  
  rot =
    new RotationMatrixProcessor( network,
                                 sensor + "/rot",
                                 sensor + "/rotation" );
  
  
  
  // Connect processors
  linkActors( (sensor + "/loc").c_str(),
              (sensor + "/loc").c_str() );
  
  linkActors( (sensor + "/rot").c_str(),
              (sensor + "/rot").c_str() );
  
  start();
  
}
\endcode

Ja msgProcessin toteutus:

\code
bool
SensorController::msgProcess( const char *id, Message * msg )
{
  
  if( NULL != strstr(id, "/loc") ) {
    
    Vector3 * locVec = ((FluidMessage<Vector3> *)msg)->getContent();
    
    if( isTranslated && currRot != 0 ) {
      // calculate scaled base vectors
      Vector3 transAbs = *currRot * relTranslation;
      
      // transform and pass coordinates forward
      useControl( CTRL_SETLOC, 1.0 );
      useControl( FluidController::CTRL_VECTOR_DIM0,
                  locVec->x + transAbs[0] );
      useControl( FluidController::CTRL_VECTOR_DIM1,
                  locVec->y + transAbs[1] );
      useControl( FluidController::CTRL_VECTOR_DIM2,
                  locVec->z + transAbs[2] );
    } else {
      useControl( CTRL_SETLOC, 1.0 );
      useControl( FluidController::CTRL_VECTOR_DIM0, locVec->x );
      useControl( FluidController::CTRL_VECTOR_DIM1, locVec->y );
      useControl( FluidController::CTRL_VECTOR_DIM2, locVec->z );
    }    

    return true;
  } else if( NULL != strstr(id, "/rot") ) {
    Matrix3 * orient = ((FluidMessage<Matrix3> *)msg)->getContent();
    currRot = orient;
    
    useControl( CTRL_SETBASIS, 1.0 );
    // Pass the matrix (column-major) forward.
    for( int i=0; i<3; i++ )
      for( int j=0; j<3; j++ )
        useControl( CTRL_MATRIX_0_0+(i*3+j), orient->get( j, i ) );
    
    return true;
  } else {
    // add new controls here..
  }
  return false; // we should never get here..
}
\endcode
*/

#ifndef LS_P_FLUID_HPP
#define LS_P_FLUID_HPP


#include "fluid/FluidHost.hpp"
#include "fluid/FluidController.hpp"
#include "fluid/processors/GloveProcessor.hpp"
#include "fluid/processors/AccelVectorProcessor.hpp"
#include "fluid/processors/LocationVectorProcessor.hpp"
#include "fluid/processors/RotationMatrixProcessor.hpp"
#include "fluid/processors/FluidMessage.hpp"

namespace lifespace { namespace plugins { namespace pfluid {
  
  
  
  
  
  
  
  
 }}}   /* lifespace::plugins::pfluid */




#endif   /* LS_P_FLUID_HPP */
