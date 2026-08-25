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
#include <analyses/bit/wirupdownvalue.h>
#include <arch/generic/mips.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  WIR_UpDownValue e1 { MIPS_Immediate16_Signed( -32654 ) };
  WIR_UpDownValue zero { MIPS_Immediate16_Signed( 0 ) };

  // Test division by zero.
  auto e2 = e1 % zero;

  return( 0 );
}
