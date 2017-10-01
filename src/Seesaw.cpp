#include "Seesaw.hpp"
#include "TIEMaterials.hpp"
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

namespace tie
{

Seesaw::Seesaw( lifespace::Vector a_location, float a_length,
  lifespace::Vector a_rotation ): 
  Object(Object::Params
    ( new ODELocator( a_location, BasisMatrix
      ( a_rotation, makeVector3d( 0.0, 1.0, 0.0 ) ))))
{
	/* Create planc part of the seesaw */
  float thickness = a_length / 30.0;
  float width = a_length / 5.0;
  
  Object * planc = new Object( Object::Params 
    ( new ODELocator
      ( makeVector3d( 0.0, a_length / 10.0, 0.0 ),
        BasisMatrix(),
        a_length / 2.0, 1.0)));
  
  planc->setVisual( sptr( new BasicVisual
    ( sptr( new shapes::Cube
      ( makeVector3d( a_length, thickness, width))), 
    &TIEMat::redMat )));
  
  planc->setGeometry( sptr( new BasicGeometry
    ( sptr( new shapes::Cube
      ( makeVector3d( a_length, thickness, width))), 
      sptr( new CollisionMaterial( 10.0, 0.0, 1.0 ) ))));
    
  addObject(planc);
  
  /* Create cylinder part of the seesaw */
  
  Object * cylinder = new Object( Object::Params
    ( new ODELocator( makeVector3d( 0.0, 0.0, 0.0 ),
      BasisMatrix(),
      a_length, 1.0 )));
  
  cylinder->setVisual( sptr( new BasicVisual
    ( shapes::CappedCylinder::create( width * 1.5, a_length / 13.0),
    &TIEMat::redMat )));
  
  cylinder->setGeometry( sptr( new BasicGeometry
    ( shapes::CappedCylinder::create( width * 1.5, a_length / 13.0),
      sptr( new CollisionMaterial( 10.0, 0.0, 1.0 ) ))));
  
  addObject(cylinder);
}

Seesaw::~Seesaw()
{
}

};
