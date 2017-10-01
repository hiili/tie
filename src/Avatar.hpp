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
 * @file Avatar.hpp
 *
 * Avatar is the base class for user representation in TIE structure.
 */

/**
 * @class tie::Avatar
 *
 * @brief
 * Avatar is the base class for user representation in TIE structure.
 *
 * Avatar is the users representation in TIE virtual world. It's role is
 * twofolded: first of all it listens to fluid input and passes the needed info
 * to lifespace for interaction between user and modeled world. Second of all
 * it has physical ODE controlled skeleton for passing world's feedback to the
 * user visually.
 *
 * \par Base or "EVE-cube"
 * Avatars base (Subspace's object form) is used to store avatar's two
 * skeletons. This allows skeletons to be connected to lifespace's prepare-step
 * -cycle and thus be updated for each frame. Other purpose for base is to act
 * as physical modeled balancer in virtual world. This base keeps avatar
 * standing and acts as control object for other than fluid-based or physically
 * modeled world-based input (i.e. movement, turning). Also VRJuggler camera is
 * attached to this object so it represents the users coordinates in virtual
 * world. 
 *
 * \par Fluid skeleton
 * Fluid skeleton is used to read sensorinput from fluid system, and create
 * "static" skeleton for ODE modeled parts for each frame. Static skeleton is
 * achieved by moving static fluid-skeleton objects created in init() method
 * according to fluid input. This skeleton is then used in controlling the ODE
 * managed skeleton by transforming avatars connectors according to these
 * init() created objects.
 *
 * \par ODE skeleton
 * ODESkeleton handles all interaction between avatar and physically modeled
 * world. Currently it has three parts: head, lefthand and righthand. As
 * described in last paragraph, ODESkeleton is controlled by
 * FluidSkeleton. Through these FluidSkeleton connectors it is possible for the
 * pyshical world to affect avatars base node and thus VRJuggler camera. This
 * camera movement appears as visual feedback from world. For instance, when
 * picking up heavy objects, their weight pulls down avatars ODE controlled
 * hand(s), the force is transferred through FluidSkeleton's hand connectors to
 * the avatar base. Base, being physically modeled object itself, then is
 * pushed downward at the point of connector and, depending to the position of
 * ODE controlled hand object, either just gets more weight or falls
 * over. Hopefully not too complicated ;).
 *
 * \par Abstract model of avatar
 * This paragraph describes, how different parts of world see avatar and it's
 * parts.
 * - From ODE's point of view, avatar is a collection of four parts. ODE
 * sees the base object as individual object, in which three objects (hands and
 * the head) are connected with ODEFixedConnectors. During lifespace's
 * prepare-step -cycle these connectors suddenly warp to different positions
 * (as fluid updates them). ODE sees this as an error created by "something"
 * and reacts by correcting it by adding forces to the slave objects (visual
 * hands, for instance). These forces are calculated by connections' ERP and
 * CFM values, which are parameters introduced by ODE.
 * - Fluid sees avatar as three static objects (static in here means objects
 * with ODE control removed from them). For each round, fluid updates it's
 * sensor input and controls these static objects through lifespace's plugin,
 * FluidController.
 * - Virtual world sees avatar as a lifespace::Subspace in which objects
 * described above are added. If these objects have visual form, they are
 * rendered by current renderer. If they have lifespace::ODELocator, they take
 * part in ODE's timestep. Finally, if they have collision forms, they take
 * part in lifespace::CollisionRenderers render. Simple.
 * - The avatar sees itself as subspace, which contains two skeletons ODE, and
 * Fluid. It initializes them, adjusts their parameters and sees that the
 * transformation from the fluid controlled objects affect the ODESkeleton by
 * moving it's ODEFixedConnectors.
 *
 * \par Prepare-step -cycle, detailed
 * For each prepare, avatar does following:
 * - Call fluidSkeleton->prepare(). This updates fluid input from inputreaders
 * to connector, then attaches avatar's odeskeleton to it. In detail:
 *   - Checks whether "control object" exists, if not moves forward to the next
 *   <control object>
 *     - If "control object" exists checks, if it's already connected to
 *   something. If connected, disconnects it. This is done because lifespace
 *   doesn't currently allow connected connectors to be translated.
 *     - Moves the connector according to "control object"
 *     . 
 *   - This cycle is manually repeated for all of the "control object", which
 *   are currently head, lefthand and righthand.
 *   After all of the connectors are in right place, fluidskeleton calls
 *   avatars ODESkeleton to connect.
 *   - ODESkeleton then calls lifespace::Connector's connect -method for all
 *   of the connectors.
 *   .
 * - Call underlying subspace's and object's prepare method individually. This
 * is done because of lifespace's virtual inheritance of Subspace from Object.
 * .
 * Step cycle consist only of calling undelying lifespace::Subspace's and
 * lifespace::Object's step.
 *
 *
 * \par Code example: Connection of individual control object during prepare-step -pass. 
 * In Avatar:
 *
 * \code
 * void
 * Avatar::prepare( real dt )
 * {
 *   fluidSkeleton->prepare( dt );
 * 
 *   // call the both underlying bases' prepare (for subspace: local version)
 *   Subspace::localPrepare( dt );
 * 
 *   Object::prepare( dt );
 * }
 * \endcode
 * 
 * In FluidSkeleton:
 *
 * \code 
 * void
 * FluidSkeleton::prepare( real dt )
 * {
 *   // if target exists and is connected, disconnect connector before moving
 *  if( headControl != 0 ) {
 *     if( baseNode->getConnector( CONN_HEAD_BASE ).isConnected() )
 *       baseNode->getConnector( CONN_HEAD_BASE ).disconnect();
 *     
 *     //move and rotate connector according to fluidcontrolled skeletal object..
 *     baseNode->getConnector( CONN_HEAD_BASE ).setLoc
 *       ( headControl->getLocator()->getLoc() );
 *     baseNode->getConnector( CONN_HEAD_BASE ).setBasis
 *       ( headControl->getLocator()->getBasis() );
 *   }
 *   
 *   // reconnect all broken joints
 *   odeSkeleton->connect( this );
 * }
 * \endcode
 *
 * And in ODESkeleton:
 *
 * \code
 * void
 * ODESkeleton::connect( Skeleton * master )
 * {
 *   //connect head
 *   if( head ) {
 *     master->getBaseNode()->getConnector(FluidSkeleton::CONN_HEAD_BASE).connect
 *       ( head->getConnector( Head::CONN_BASE ), Connector::DontAlign );
 *     ((ODEFixedConnector &)master->getBaseNode()->getConnector
 *      (FluidSkeleton::CONN_HEAD_BASE)).setParam( dParamERP, 0.5 );
 *     ((ODEFixedConnector &)master->getBaseNode()->getConnector
 *      (FluidSkeleton::CONN_HEAD_BASE)).setParam( dParamCFM, 0.001 );
 *   }
 * }
 * \endcode
 *
 * @note Currently all things including connectors' connection and movement
 * _must_ be done within prepare -method because of lifespace's uncharted bug!
 *
 * @see TIE::ODESkeleton,
 *      TIE::FluidSkeleton,
 *      lifespace::ODEFixedConnector,
 *      lifespace::Subspace,
 *      lifespace::ODELocator,
 *      lifespace::plugins::pfluid::FluidController
 */

 
#ifndef TIE_AVATAR_HPP
#define TIE_AVATAR_HPP


#include "SensorController.hpp"
#include "GloveController.hpp"

#include "ODESkeleton.hpp"
#include "FluidSkeleton.hpp"

#include <lifespace/lifespace.hpp>
#include <lifespace/plugins/fluid.hpp>

namespace tie {


  class Avatar :
    public lifespace::Subspace
  {
  private:
    // Fluid input readers
    SensorController * head;
    SensorController * leftHand;
    SensorController * rightHand;
    GloveController * leftGlove;
    GloveController * rightGlove;

    // Avatar's fluid controlled skeleton
    FluidSkeleton * fluidSkeleton;
    // Avatar's ODE controlled skeleton
    ODESkeleton * odeSkeleton;
    
    // fills fluid keymap for sensorcontrollers
    void fillFluidKeymap( lifespace::Controller::ControlMap & keymap );
    // fills flexion keymap for glovecontrollers
    void fillFlexionKeymap( lifespace::Controller::ControlMap & keymap );

    
  public:

    /**
     * Constructs a new avatar, with lifespace's object parameters of avatar's
     * base.
     * 
     * @note avatar should always have collision shape, otherwise it just falls
     * through the floor and hangs by its hands and head. Collision shape
     * should also be some sort of cube if no external balancing force is used.
     *
     * @note After creation, avatar should always be initialized with init().
     *
     * @see lifespace::Object
     * 
     * @param params Avatars base object params, defined in
     * lifespace::Object.hpp
     */
    Avatar( lifespace::Object::Params params ) :
      lifespace::Object( params )   // instantiate object for subspace
    {}
    
    /** dtor */
    ~Avatar() {}

    // getters and setters
    /**
     * Returns avatar's head sensor for later use in gesture recognition.
     *
     * @return pointer to avatar's head sensor controller
     * @see tie::SensorController.hpp
     */
    SensorController * getHeadSensor() { return head; }
    
    /**
     * Returns Avatar's ode controlled skeleton
     *
     * @return pointer to avatar's ode controlled skeleton.
     */
    ODESkeleton * getODESkeleton() { return odeSkeleton; }

    // initializes the avatar
    /**
     * Initializes the avatar. Does all the initialization needed for all
     * avatar's parts:
     * - Creates connectors, updated by fluid input system each frame. Adds
     * them to base object.
     * - Creates sensor and glove inputreaders for Avatar's FluidSkeleton.
     * - Creates Fluid- and ODESkeletons and adusts their parameters.
     *
     * @param inputHost pointer to program's single FluidHost for
     * sensorController creation.
     *
     * @param world pointer to created, ode controlled world object.
     */
    void init( lifespace::plugins::pfluid::FluidHost * inputHost,
               lifespace::ODEWorld * world );

    /**
     * Overridden method from lifespace::Subspace
     *
     * @see lifespace::Subspace
     */
    virtual void prepare( lifespace::real dt );

    /**
     * Overridden method from lifespace::Subspace
     *
     * @see lifespace::Subspace
     */
    virtual void step();

    /**
     * Connects the ode controlled skeleton nodes to given underlying skeleton.
     */
    void connect() { odeSkeleton->connect( fluidSkeleton ); }    
  };


}  /* namespace tie */


#endif /* TIE_AVATAR_HPP */
