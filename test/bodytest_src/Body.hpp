#ifndef TIE_BODY_HPP
#define TIE_BODY_HPP

#include <tie/tie.hpp>
#include <lifespace/lifespace.hpp>

class Body:public lifespace::Subspace
{
private:
  tie::Hand * leftHand;
  tie::Hand * rightHand;
public:
	Body(lifespace::Subspace * superspace);
	virtual ~Body();

  void assemble();
  void kickHand();
  
  virtual void prepare(float dt);
};

#endif //TIE_BODY_HPP
