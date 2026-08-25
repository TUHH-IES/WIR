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

// Include WIR headers
#include <wir/wir.h>
#include <arch/generic/mips.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  // Check precolors.

  MIPS m;
  WIR_Function f( "main" );
  f.pushBackVirtualRegister( MIPS_RegV() );
  f.pushBackVirtualRegister( MIPS_RegV() );

  MIPS_RegV r3;

  // This must assert since r3 is not inserted into f.
  f.findPrecolor( r3 );

  return( 0 );
}
