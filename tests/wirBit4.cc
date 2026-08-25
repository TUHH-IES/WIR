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
#include <analyses/bit/wirupdownvalue.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  // Complete test of WIR_L4 operator |.

  // 0 OR b = b OR 0 = b.
  for ( auto b : WIR_L4Set { WIR_L4::bU, WIR_L4::bL, WIR_L4::bN, WIR_L4::b0,
                             WIR_L4::b1, WIR_L4::bX } ) {
    ufAssert( ( WIR_L4::b0 | b ) == b );
    ufAssert( ( b | WIR_L4::b0 ) == b );
  }

  // 1 OR b = b OR 1 = 1.
  for ( auto b : WIR_L4Set { WIR_L4::bU, WIR_L4::bL, WIR_L4::bN, WIR_L4::b0,
                             WIR_L4::b1, WIR_L4::bX } ) {
    ufAssert( ( WIR_L4::b1 | b ) == WIR_L4::b1 );
    ufAssert( ( b | WIR_L4::b1 ) == WIR_L4::b1 );
  }

  // b OR b = b.
  for ( auto b : WIR_L4Set { WIR_L4::bU, WIR_L4::bL, WIR_L4::bN, WIR_L4::b0,
                             WIR_L4::b1, WIR_L4::bX } )
    ufAssert( ( b | b ) == b );

  // L OR N = N OR L = 1.
  ufAssert( ( WIR_L4::bL | WIR_L4::bN ) == WIR_L4::b1 );
  ufAssert( ( WIR_L4::bN | WIR_L4::bL ) == WIR_L4::b1 );

  // U OR b = b OR U = U (almost always, except for b = X and b = 1).
  for ( auto b : WIR_L4Set { WIR_L4::bU, WIR_L4::bL, WIR_L4::bN,
                             WIR_L4::b0 } ) {
    ufAssert( ( WIR_L4::bU | b ) == WIR_L4::bU );
    ufAssert( ( b | WIR_L4::bU ) == WIR_L4::bU );
  }

  // Special case: X OR b = b OR X = 1 (for b = L, N or U).
  for ( auto b : WIR_L4Set { WIR_L4::bU, WIR_L4::bL, WIR_L4::bN } ) {
    ufAssert( ( WIR_L4::bX | b ) == WIR_L4::b1 );
    ufAssert( ( b | WIR_L4::bX ) == WIR_L4::b1 );
  }

  return( 0 );
}
