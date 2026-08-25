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
  auto &tr1 = f.pushBackVirtualRegister( MIPS_RegV() );
  auto &tr2 = f.pushBackVirtualRegister( MIPS_RegV() );

  MIPS_RegV &r1 = static_cast<MIPS_RegV &>( tr1 );
  MIPS_RegV &r2 = static_cast<MIPS_RegV &>( tr2 );
  MIPS_RegV r3;

  f.insertPrecolor( r2, m.r4() );

  ufAssert( !r1.isPrecolored() );
  ufAssert( r2.isPrecolored() );
  ufAssert( !r3.isPrecolored() );
  ufAssert( r2.getPrecolor() == m.r4() );

  f.insertPrecolor( r1, m.r4() );
  f.insertPrecolor( r2, m.r20() );

  ufAssert( r1.isPrecolored() );
  ufAssert( r2.isPrecolored() );
  ufAssert( !r3.isPrecolored() );
  ufAssert( r1.getPrecolor() == m.r4() );
  ufAssert( r2.getPrecolor() == m.r20() );

  f.erasePrecolor( r1 );

  ufAssert( !r1.isPrecolored() );
  ufAssert( r2.isPrecolored() );
  ufAssert( !r3.isPrecolored() );
  ufAssert( r2.getPrecolor() == m.r20() );

  f.erasePrecolor( r1 );
  f.erasePrecolor( r3 );

  ufAssert( !r1.isPrecolored() );
  ufAssert( r2.isPrecolored() );
  ufAssert( !r3.isPrecolored() );
  ufAssert( r2.getPrecolor() == m.r20() );

  f.insertPrecolor( r1, m.r29() );

  ufAssert( r1.isPrecolored() );
  ufAssert( r2.isPrecolored() );
  ufAssert( !r3.isPrecolored() );
  ufAssert( r1.getPrecolor() == m.r29() );
  ufAssert( r2.getPrecolor() == m.r20() );

  f.clearPrecolors();

  ufAssert( !r1.isPrecolored() );
  ufAssert( !r2.isPrecolored() );
  ufAssert( !r3.isPrecolored() );

  return( 0 );
}
