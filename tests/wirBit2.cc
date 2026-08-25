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

  // Test basic properties of WIR_L4 operator ~.
  ufAssert( ~WIR_L4::bU == WIR_L4::bU );
  ufAssert( ~WIR_L4::bL == WIR_L4::bN );
  ufAssert( ~WIR_L4::bN == WIR_L4::bL );
  ufAssert( ~WIR_L4::b0 == WIR_L4::b1 );
  ufAssert( ~WIR_L4::b1 == WIR_L4::b0 );
  ufAssert( ~WIR_L4::bX == WIR_L4::bX );

  return( 0 );
}
