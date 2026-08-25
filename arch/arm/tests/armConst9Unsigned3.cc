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

// Include WIR headers
#include <wir/wir.h>
#include <arch/arm/armv4t.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  // The following value must not be accepted for unsigned const9.
  ARM_Const9_Unsigned i( 7 );

  return( i.getValue() );
}
