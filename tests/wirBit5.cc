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

  // Complete test of WIR_L4 operator ^.

  // 0 XOR b = b XOR 0 = b.
  for ( auto b : WIR_L4Set { WIR_L4::bU, WIR_L4::bL, WIR_L4::bN, WIR_L4::b0,
                             WIR_L4::b1, WIR_L4::bX } ) {
    ufAssert( ( WIR_L4::b0 ^ b ) == b );
    ufAssert( ( b ^ WIR_L4::b0 ) == b );
  }

  // 1 XOR b = b XOR 1 = ~b.
  for ( auto b : WIR_L4Set { WIR_L4::bU, WIR_L4::bL, WIR_L4::bN, WIR_L4::b0,
                             WIR_L4::b1, WIR_L4::bX } ) {
    ufAssert( ( WIR_L4::b1 ^ b ) == ~b );
    ufAssert( ( b ^ WIR_L4::b1 ) == ~b );
  }

  // b XOR b = 0 (almost always, except for b = U or X).
  for ( auto b : WIR_L4Set { WIR_L4::bL, WIR_L4::bN, WIR_L4::b0, WIR_L4::b1 } )
    ufAssert( ( b ^ b ) == WIR_L4::b0 );

  // cppcheck-suppress duplicateExpression
  ufAssert( ( WIR_L4::bU ^ WIR_L4::bU ) == WIR_L4::bU );
  // cppcheck-suppress duplicateExpression
  ufAssert( ( WIR_L4::bX ^ WIR_L4::bX ) == WIR_L4::bX );

  // L XOR N = N XOR L = 1.
  ufAssert( ( WIR_L4::bL ^ WIR_L4::bN ) == WIR_L4::b1 );
  ufAssert( ( WIR_L4::bN ^ WIR_L4::bL ) == WIR_L4::b1 );

  // U XOR b = b XOR U = U.
  for ( auto b : WIR_L4Set { WIR_L4::bU, WIR_L4::bL, WIR_L4::bN, WIR_L4::b0,
                             WIR_L4::b1, WIR_L4::bX } ) {
    ufAssert( ( WIR_L4::bU ^ b ) == WIR_L4::bU );
    ufAssert( ( b ^ WIR_L4::bU ) == WIR_L4::bU );
  }

  // Special case: X OR b = b OR X = b (for b = L, N or U).
  for ( auto b : WIR_L4Set { WIR_L4::bU, WIR_L4::bL, WIR_L4::bN } ) {
    ufAssert( ( WIR_L4::bX ^ b ) == b );
    ufAssert( ( b ^ WIR_L4::bX ) == b );
  }

  return( 0 );
}
