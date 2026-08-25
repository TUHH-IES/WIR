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

  // Test arithmetic operators.

  // Test 0-extension for unsigned values.
  WIR_UpDownValue e1 { MIPS_Immediate5_Shamt( 21 ) };
  auto e2 = e1.extend( 13 );
  ufAssert( e2.getBitWidth() == 13 );
  ufAssert( e2.getSignedValue() == 21 );

  // Test 0-extension for signed value.
  WIR_UpDownValue e3 { MIPS_Immediate16_Signed( 26361 ) };
  e2 = e3.extend( 23 );
  ufAssert( e2.getBitWidth() == 23 );
  ufAssert( e2.getSignedValue() == 26361 );

  // Test 1-extension for signed value.
  WIR_UpDownValue e4 { MIPS_Immediate16_Signed( -26361 ) };
  e2 = e4.extend( 27 );
  ufAssert( e2.getBitWidth() == 27 );
  ufAssert( e2.getSignedValue() == -26361 );

  // Test binary + operator.
  e2 = e1 + e1;
  ufAssert( e2.isUnsigned() && ( e2.getBitWidth() == 5 ) );
  ufAssert( e2.getSignedValue() == 10 );                    // Overflow!

  e2 = e1.extend( 16 ) + e3;
  ufAssert( e2.isSigned() && ( e2.getBitWidth() == 16 ) );
  ufAssert( e2.getSignedValue() == 26382 );

  e2 = e4 + e1.extend( 16 );
  ufAssert( e2.isSigned() && ( e2.getBitWidth() == 16 ) );
  ufAssert( e2.getSignedValue() == -26340 );

  e2 = e3 + e3;
  ufAssert( e2.isSigned() && ( e2.getBitWidth() == 16 ) );
  ufAssert( e2.getSignedValue() == -12814 );              // Overflow!

  e2 = e4 + e4;
  ufAssert( e2.isSigned() && ( e2.getBitWidth() == 16 ) );
  ufAssert( e2.getSignedValue() == 12814 );               // Underflow!

  // Test unary - operator.
  e2 = -e4;
  ufAssert( e2.getSignedValue() == 26361 );
  e2 = -e3;
  ufAssert( e2.getSignedValue() == -26361 );

  // Test binary - operator.
  e2 = e1 - e1;
  ufAssert( e2.isSigned() && ( e2.getBitWidth() == 5 ) );
  ufAssert( e2.getSignedValue() == 0 );

  e2 = e1.extend( 16 ) - e3;
  ufAssert( e2.isSigned() && ( e2.getBitWidth() == 16 ) );
  ufAssert( e2.getSignedValue() == -26340 );

  e2 = e4 - e1.extend( 16 );
  ufAssert( e2.isSigned() && ( e2.getBitWidth() == 16 ) );
  ufAssert( e2.getSignedValue() == -26382 );

  e2 = e3 - e4;
  ufAssert( e2.isSigned() && ( e2.getBitWidth() == 16 ) );
  ufAssert( e2.getSignedValue() == -12814 );              // Overflow!

  e2 = e4 - e3;
  ufAssert( e2.isSigned() && ( e2.getBitWidth() == 16 ) );
  ufAssert( e2.getSignedValue() == 12814 );               // Underflow!

  e2 = e4 - -e3;
  ufAssert( e2.isSigned() && ( e2.getBitWidth() == 16 ) );
  ufAssert( e2.getSignedValue() == 0 );

  e2 = e1;
  ufAssert( e2.isUnsigned() && ( e2.getBitWidth() == 5 ) );

  // Test * operator.
  WIR_UpDownValue e5 { MIPS_Immediate16_Signed( -4711 ) };
  WIR_UpDownValue e6 { MIPS_Immediate16_Signed( 17 ) };
  auto e7 = e5 * e6;
  ufAssert( e7.isSigned() && ( e7.getBitWidth() == 32 ) );
  ufAssert( e7.getSignedValue() == -80087 );

  WIR_UpDownValue e8 { MIPS_Immediate16_Signed( 2 ) };
  WIR_UpDownValue e9 { MIPS_Immediate16_Signed( 32767 ) };
  e7 = e8 * e9;
  ufAssert( e7.getSignedValue() == 65534 );

  WIR_UpDownValue one { MIPS_Immediate16_Signed( 1 ) };
  e7 = one * e5;
  ufAssert( e7.extract( 0, 16 ).isEqual( e5 ) );
  e7 = e5 * one;
  ufAssert( e7.extract( 0, 16 ).isEqual( e5 ) );

  WIR_UpDownValue zero { MIPS_Immediate16_Signed( 0 ) };
  e7 = zero * e9;
  ufAssert( e7.extract( 0, 16 ).isEqual( zero ) );
  e7 = e9 * zero;
  ufAssert( e7.extract( 0, 16 ).isEqual( zero ) );

  // Test / and % operators.
  WIR_UpDownValue e10 { MIPS_Immediate16_Signed( -32654 ) };
  WIR_UpDownValue e11 { MIPS_Immediate16_Signed( 9867 ) };

  auto e12 = e10 / e11;
  ufAssert( e12.isSigned() && ( e12.getBitWidth() == 16 ) );
  ufAssert( e12.getSignedValue() == -3 );

  auto m12 = e10 % e11;
  ufAssert( m12.isSigned() && ( m12.getBitWidth() == 16 ) );
  ufAssert( m12.getSignedValue() == -3053 );

  e12 = e10 / -e11;
  ufAssert( e12.isSigned() && ( e12.getBitWidth() == 16 ) );
  ufAssert( e12.getSignedValue() == 3 );

  m12 = e10 % -e11;
  ufAssert( m12.isSigned() && ( m12.getBitWidth() == 16 ) );
  ufAssert( m12.getSignedValue() == 3053 );

  e12 = e10 / one;
  ufAssert( e12.isEqual( e10 ) );
  e12 = e11 / one;
  ufAssert( e12.isEqual( e11 ) );

  e12 = one / e10;
  ufAssert( e12.isEqual( zero ) );
  e12 = one / e11;
  ufAssert( e12.isEqual( zero ) );

  WIR_UpDownValue e13 { MIPS_Immediate16_Signed( -5 ) };
  WIR_UpDownValue e14 { MIPS_Immediate16_Signed( 25 ) };

  e12 = e13 / e14;
  ufAssert( e12.getSignedValue() == 0 );

  m12 = e13 % e14;
  ufAssert( m12.getSignedValue() == -5 );

  WIR_UpDownValue e15 { MIPS_Immediate16_Signed( 543 ) };
  WIR_UpDownValue e16 { MIPS_Immediate16_Signed( -45 ) };

  e12 = e15 / e16;
  ufAssert( e12.getSignedValue() == -12 );

  m12 = e15 % e16;
  ufAssert( m12.getSignedValue() == -3 );

  WIR_UpDownValue e17 { MIPS_Immediate16_Signed( 15 ) };
  WIR_UpDownValue e18 { MIPS_Immediate16_Signed( -9 ) };

  e12 = e17 / e18;
  ufAssert( e12.getSignedValue() == -1 );
  m12 = e17 % e18;
  ufAssert( m12.getSignedValue() == -6 );

  e12 = e17 / e17;
  ufAssert( e12.getSignedValue() == 1 );
  m12 = e17 % e17;
  ufAssert( m12.getSignedValue() == 0 );

  e12 = e17 / -e17;
  ufAssert( e12.getSignedValue() == -1 );
  m12 = e17 % -e17;
  ufAssert( m12.getSignedValue() == 0 );

  e12 = -e17 / e17;
  ufAssert( e12.getSignedValue() == -1 );
  m12 = -e17 % e17;
  ufAssert( m12.getSignedValue() == 0 );

  e12 = zero / e17;
  ufAssert( e12.getSignedValue() == 0 );
  m12 = zero % e17;
  ufAssert( m12.getSignedValue() == 0 );

  return( 0 );
}
