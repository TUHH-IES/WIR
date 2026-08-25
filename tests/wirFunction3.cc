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

// Include libuseful headers
#include <libuseful/io.h>

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

  MIPS_RegV r3;

  // This must assert since r3 is not inserted into f.
  f.insertPrecolor( r3, m.r4() );

  // Fallback for disabled failsafe mode.
  ufAssert( f.containsVirtualRegister( r3.getRoot() ) );
  ufAssert( r3.getType() == m.r4().getType() );

  return( 0 );
}
