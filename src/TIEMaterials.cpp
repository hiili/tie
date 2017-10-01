/* TIE - TIEMaterials.hpp */


#include "TIEMaterials.hpp"
using namespace tie;

#include <lifespace/lifespace.hpp>
using namespace lifespace;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <GL/gl.h>




const GLfloat TIEMat::none[4]        = { 0.0, 0.0, 0.0, 0.0 };
const GLfloat TIEMat::white[4]       = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat TIEMat::gray6[4]       = { 0.6, 0.6, 0.6, 1.0 };
const GLfloat TIEMat::gray3[4]       = { 0.3, 0.3, 0.3, 1.0 };
const GLfloat TIEMat::gray1[4]       = { 0.1, 0.1, 0.1, 1.0 };
const GLfloat TIEMat::red[4]         = { 1.0, 0.0, 0.0, 1.0 };
const GLfloat TIEMat::blue[4]	     = { 0.0, 0.0, 1.0, 1.0 };
const GLfloat TIEMat::red05[4]	     = { 0.5, 0.0, 0.0, 1.0 };
const GLfloat TIEMat::skyBlue[4]     = { 0.5, 0.5, 1.0, 1.0 };
const GLfloat TIEMat::skyBlue3[4]    = { 0.1, 0.1, 0.3, 1.0 };
const GLfloat TIEMat::green2[4]      = { 0.0, 0.2, 0.0, 1.0 };
const GLfloat TIEMat::green4[4]      = { 0.0, 0.4, 0.0, 1.0 };
const GLfloat TIEMat::green6[4]      = { 0.0, 0.6, 0.0, 1.0 };
const GLfloat TIEMat::green8[4]      = { 0.0, 0.8, 0.0, 1.0 };
const GLfloat TIEMat::green[4]       = { 0.0, 1.0, 0.0, 1.0 };
const GLfloat TIEMat::orange1[4]     = { 0.1, 0.1, 0.0, 1.0 };
const GLfloat TIEMat::orange6[4]     = { 0.6, 0.6, 0.0, 1.0 };
const GLfloat TIEMat::cyan1[4]       = { 0.0, 0.1, 0.1, 1.0 };
const GLfloat TIEMat::cyan6[4]       = { 0.0, 0.6, 0.6, 1.0 };
const GLfloat TIEMat::smooth[1]      = { 1.0 };
const GLfloat TIEMat::polished[1]    = { 40.0 };
const Material TIEMat::blackMat( none, gray6, white,
                                none, polished, GL_FRONT );
const Material TIEMat::redMat( none, red, white,
                               gray3, polished, GL_FRONT );
const Material TIEMat::whiteMat( none, gray6, white,
                                 gray3, polished, GL_FRONT );
const Material TIEMat::redEmissionMat( none, red, white,
			              red, polished, GL_FRONT );
const Material TIEMat::whiteEmissionMat( none, white, white,
                                         white, polished, GL_FRONT );
const Material TIEMat::emissionMat( gray6, gray6, white,
                                    gray6, polished, GL_FRONT );
const Material TIEMat::groundMat( green2, green8, none,
                                  none, smooth, GL_FRONT );
const Material TIEMat::wall1Mat( green4, green8, none,
                                  none, smooth, GL_FRONT );
const Material TIEMat::wall2Mat( orange1, orange6, none,
                                  none, smooth, GL_FRONT );
const Material TIEMat::wall3Mat( cyan1, cyan6, none,
                                  none, smooth, GL_FRONT );
const Material TIEMat::skyBlueMat(skyBlue3, skyBlue, white, 
                                  none, smooth, GL_FRONT );
const Material TIEMat::brightWhiteMat(none, white, white, 
                                      gray3, polished, GL_FRONT);
const Material TIEMat::purpleMat( red05, blue, blue,
                                 red05, smooth, GL_FRONT);
const Material TIEMat::polishedMetalMat( gray1, gray3, white,
                                         none, polished, GL_FRONT );

const shared_ptr<const CollisionMaterial> TIEMat::testCollMat
( new CollisionMaterial( 10.0, 0.1, 0.2 ) );
const shared_ptr<const CollisionMaterial> TIEMat::testCollMat1
( new CollisionMaterial( 1.0, 0.5, 0.2 ) );
const shared_ptr<const CollisionMaterial> TIEMat::ballCollMat
( new CollisionMaterial( 5.0, 0.5, 0.001 ) );
