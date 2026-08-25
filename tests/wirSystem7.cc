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


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  WIR_TaskManager t;
  WIR_System s( "genericmips.sys", t );

  // Insert compilation units in DESCENDING order of IDs.
  s.pushBackCompilationUnit( WIR_CompilationUnit() );
  s.pushFrontCompilationUnit( WIR_CompilationUnit() );
  s.pushFrontCompilationUnit( WIR_CompilationUnit() );

  // Copy system.
  WIR_System s1( s );

  ufAssert(
    s.getCompilationUnits().size() == s1.getCompilationUnits().size() );

  // Check descending order of compilation unit IDs.
  WIR_id_t prev = nullid;
  for ( WIR_CompilationUnit &c : s1 ) {
    if ( prev == nullid )
      ufAssert( c.getID() > prev );
    else
      ufAssert( c.getID() < prev );
    prev = c.getID();
  }

  return( 0 );
}
