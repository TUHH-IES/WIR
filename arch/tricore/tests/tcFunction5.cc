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
#include <arch/tricore/tc131.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  // Check precolors.

  TC131 t;
  WIR_Function f( "foobar" );
  TC_ERegV &r1 =
    static_cast<TC_ERegV &>( f.pushBackVirtualRegister( TC_ERegV() ) );

  // Precolor a complete register hierarchy from its root.
  f.insertPrecolor( r1, t.E6() );
  ufAssert( r1.isPrecolored() );
  ufAssert( r1.getChilds().front().get().isPrecolored() );
  ufAssert( r1.getChilds().back().get().isPrecolored() );
  ufAssert( r1.getPrecolor() == t.E6() );
  ufAssert( r1.getChilds().front().get().getPrecolor() == t.D6() );
  ufAssert( r1.getChilds().back().get().getPrecolor() == t.D7() );

  // Precolor the register hierarchy from a leaf register.
  f.insertPrecolor( r1.getChilds().back().get(), t.D13() );
  ufAssert( r1.isPrecolored() );
  ufAssert( r1.getChilds().front().get().isPrecolored() );
  ufAssert( r1.getChilds().back().get().isPrecolored() );
  ufAssert( r1.getPrecolor() == t.E12() );
  ufAssert( r1.getChilds().front().get().getPrecolor() == t.D12() );
  ufAssert( r1.getChilds().back().get().getPrecolor() == t.D13() );

  // Clear precolors of the entire register hierarchy.
  f.erasePrecolor( r1.getChilds().front().get() );
  ufAssert( !r1.isPrecolored() );
  ufAssert( !r1.getChilds().front().get().isPrecolored() );
  ufAssert( !r1.getChilds().back().get().isPrecolored() );

  return( 0 );
}
