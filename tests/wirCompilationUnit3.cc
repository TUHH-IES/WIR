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

  WIR_CompilationUnit c;

  // Insert functions in DESCENDING order of IDs.
  c.pushBackFunction( WIR_Function( "f1" ) );
  c.pushFrontFunction( WIR_Function( "f2" ) );
  c.pushFrontFunction( WIR_Function( "f3" ) );

  // Copy compilation unit. The functions in c1 should also appear in DESCENDING
  // ID order.
  WIR_CompilationUnit c1( c );

  ufAssert( c.getFunctions().size() == c1.getFunctions().size() );

  // Check descending order of function IDs.
  WIR_id_t prev = nullid;
  for ( WIR_Function &f : c1 ) {
    if ( prev == nullid )
      ufAssert( f.getID() > prev );
    else
      ufAssert( f.getID() < prev );
    prev = f.getID();
  }

  return( 0 );
}
