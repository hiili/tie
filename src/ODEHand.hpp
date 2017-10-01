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
 * @file ODEHand.hpp
 *
 * Contains data structures and methods implementing TIEs avatar hand
 * functionality. Also contains explicitly controlled graphical fingers.
 */

/**
 * @class tie::ODEHand
 *
 * @brief
 * ODEHand subspace part of Avatar's ODE controlled form
 *
 * Contains data structures and methods implementing TIEs avatar hand
 * functionality (picking up objects). Also contains explicitly controlled graphical fingers.
 *
 * @note Thumb is currently not used or visualized.
 */
 
#ifndef TIE_ODEHAND_HPP
#define TIE_ODEHAND_HPP
#include <lifespace/lifespace.hpp>
#include "TIEMaterials.hpp"
#include "ODEFinger.hpp"

namespace tie {
  
  class ODEHand:
    public lifespace::Subspace,
    public lifespace::Actor,
    public lifespace::Geometry::AddContactEventListener
  {
  public:
    /** constants */
    static const float FIST_GRIP_PRETHRESHOLD;
    static const float FIST_GRIP_POSTHRESHOLD;

  private:
    /* type definitions */
    struct PickUpTriggerEvent {
      bool isContact;
      float timestamp; 
      
      PickUpTriggerEvent( bool isContact_, float timestamp_ ):
        isContact( isContact_ ),
        timestamp( timestamp_ )
      {}
    };
      
    enum Caller {
      FEEDBACK,
      FIST
    };


    /* Hand ODE specific data */
    lifespace::ODEBallConnector * base;
    lifespace::ODEBallConnector * wrist; 
    lifespace::shapes::Cube * geom;
    
    /* finger specifc data (for graphical representation) */
    int side;
    tie::ODEFinger * thumb,* index,* middle, * ring, * pinky;
    
    /* datafields used, when object pick (diff. from OGL) is decided. */
    lifespace::ODEFixedConnector * gripConn; // connector for grip (slave)
    float timer; // timer for pickup thresholds (abstract)
    lifespace::Object * objectPicked;
    lifespace::Object * pickCandidate;
    bool isPicked;
    unsigned int objectPickedConnId;
    
    /* methods */
    void initControls(); // method for initializing controls
    void updateFingers(); // method for updating fingers according to flexion
                          // values 
    void release(lifespace::Object * obj); // disconnects picked object 
    void pickUpObject( Object * objToPick ); // connects object to user hand

  public:
    
    typedef enum Sides {
      LEFT = -1,
      RIGHT = 1
    } HandSides;
    
    enum Connectors {
      CONN_BASE,
      CONN_WRIST,
      CONN_GRIP
    };
    
    enum Controls {
      CTRL_FLEXION_THUMB  = 0,
      CTRL_FLEXION_INDEX  = 1,
      CTRL_FLEXION_MIDDLE = 2,
      CTRL_FLEXION_RING   = 3,
      CTRL_FLEXION_PINKY  = 4,
      CTRL_IS_FISTED      = 5, // acts as an boolean value false < 0.5 ..
      ControlCount
    };
    
    /**
     * Default constructor, calls superclass constructor
     * and local init for controls.
     */
    ODEHand();
    
    /**
     * Default destructor.
     */
    virtual ~ODEHand();
    
    /* operations */
    
    /**
     * Creates physical appearance.
     *
     * @param width The width of hand
     * @param thickness The thickness of hand
     * @param side The side of hand, enum Sides LEFT or RIGHT
     */

    void create(float width, float thickness, enum Sides side_ );
    
    /**
     * overridden method from subspace
     * 
     * @param dt time difference of the timestep prepared
     *
     * @see lifespace::Subspace
     */
    void prepare( lifespace::real dt );

    /**
     * overridden method from subspace
     * 
     * @see lifespace::Subspace
     */
    void step();
    
    /**
     * overridden method from lifespace::Geometry::AddContactEventListener.
     * Method is used to pick objects ie. attach picked object to ODEHand with
     * lifespaces ODEFixedJoint, if all prequisitions are met.
     * 
     * @param event Contact event got.
     * @param source lifespaces geometry from which event was sent.
     */
    virtual void
    processEvent( const lifespace::Geometry::AddContactEvent & event,
                  lifespace::Geometry & source );
    
  };
  
} /* namespace tie */

#endif /* TIE_ODEHAND_HPP */
