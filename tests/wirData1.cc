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

  WIR_Data d( "d1" );
  d.setSize( 42 );
  d.pushBackInitData( WIR_DataInit( WIR_DataInitType::ibyte, "b1" ) );
  d.pushBackInitData( WIR_DataInit( WIR_DataInitType::ihword, "h1" ) );
  d.pushBackInitData( WIR_DataInit( 10 ) );
  d.pushBackInitData( WIR_DataInit( 100 ) );
  d.pushBackInitData( WIR_DataInit( WIR_DataInitType::iword, "w1" ) );

  auto &d1 = c.pushBackData( d );
  c.pushBackData( WIR_Data( "d2" ) );

  d1.setDontOptimize();
  d1.clearInitData();

  return( 0 );
}
