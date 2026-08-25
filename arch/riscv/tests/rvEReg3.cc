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
#include <arch/riscv/rv32i.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  RV_ERegV r1;
  RV_RegV &c1 =
    static_cast<RV_RegV &>( r1.getChilds().front().get() );

  // Moving a register being child of another register must assert.
  RV_RegV c2;
  c2 = std::move( c1 );

  return( 0 );
}
