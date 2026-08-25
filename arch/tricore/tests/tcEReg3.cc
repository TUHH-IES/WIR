/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/


//
// Include section
//

// Include standard headers
#include <sstream>

// Include WIR headers
#include <wir/wir.h>
#include <arch/tricore/tc131.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  TC_ERegV r1;
  TC_DRegV &c1 =
    static_cast<TC_DRegV &>( r1.getChilds().front().get() );

  // Moving a register being child of another register must assert.
  TC_DRegV c2;
  c2 = std::move( c1 );

  return( 0 );
}
