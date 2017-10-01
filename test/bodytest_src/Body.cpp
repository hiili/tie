#include "Body.hpp"

#include <tie/tie.hpp>
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;


Body::Body(Subspace * superspace):Object(Object::Params( 
  new ODELocator( makeVector3d( 0.0, 0.0, 0.0 ))))
{
  leftHand = new Hand();
  
  /* Switching from this to superspace will fix finger tip problem */  
  leftHand->create(1.0, 0.2, Hand::LEFT, this);
  addObject(leftHand);
}

Body::~Body()
{
}

void Body::assemble()
{
  leftHand->assemble();
}

void Body::kickHand()
{
  leftHand->getLocator()->addForceRel(makeVector3d(0.0, 0.0, -200.0));
}

void Body::prepare(float dt)
{
  leftHand->getLocator()->addForceRel(makeVector3d(0.0, 0.0, -0.1));
  Subspace::prepare(dt);
}
