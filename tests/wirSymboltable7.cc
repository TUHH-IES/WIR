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
#include <set>

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

  WIR_TaskManager t;
  WIR_System sys( "genericmips.sys", t );
  auto &r = sys.getComponents<WIR_MemoryRegion>().begin()->get();

  WIR_CompilationUnit c1;
  WIR_CompilationUnit &c = sys.pushBackCompilationUnit( c1 );

  auto &d3 = c.pushBackData( WIR_Data( "d3" ) );
  auto &d2 = c.pushFrontData( WIR_Data( "d2" ) );
  auto &d1 = c.pushFrontData( WIR_Data( "d1" ) );
  auto &d4 = c.pushBackData( WIR_Data( "d4" ) );

  d1.setSize( 5 );
  d2.setSize( 13 );
  d3.setSize( 42 );
  d4.setSize( 1000 );

  // The data objects are assembled into section .bss at the very beginning of
  // region 'RAM'. Start addresses of the data objects are aligned by 3 bits.
  ufAssert( sys.findSymbol( r.getBaseAddress() + 21 ) == sys.findSymbol( d2 ) );

  return( 0 );
}
