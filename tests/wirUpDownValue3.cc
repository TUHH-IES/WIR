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

  MIPS p;
  WIR_Function f( "foo" );
  MIPS_RegV &x =
    static_cast<MIPS_RegV &>( f.pushBackVirtualRegister( MIPS_RegV() ) );
  WIR_Operation o1
    { MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI,
      WIR_RegisterParameter( x, WIR_Usage::def ),
      WIR_RegisterParameter( p.r0(), WIR_Usage::use ),
      MIPS_Immediate16_Signed( 2 ) };
  auto &rp1 = dynamic_cast<WIR_RegisterParameter &>( o1.begin()->get() );
  WIR_Operation o2
    { MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI,
      WIR_RegisterParameter( x, WIR_Usage::def ),
      WIR_RegisterParameter( p.r0(), WIR_Usage::use ),
      MIPS_Immediate16_Signed( 2 ) };
  auto &rp2 = dynamic_cast<WIR_RegisterParameter &>( o2.begin()->get() );

  WIR_UpDownValue zero { MIPS_Immediate16_Signed( 0 ) };
  WIR_UpDownValue one { MIPS_Immediate16_Signed( 1 ) };
  WIR_UpDownValue minusOne { MIPS_Immediate16_Signed( -1 ) };

  WIR_UpDownValue v1 { 11 };
  v1.setBit( 0, WIR_L4::bL, { rp1, 9 } );
  v1.setBit( 1, WIR_L4::bL, { rp1, 9 } );
  v1.setBit( 2, WIR_L4::bL, { rp1, 9 } );
  v1.setBit( 3, WIR_L4::bL, { rp1, 9 } );
  v1.setBit( 4, WIR_L4::bN, { rp1, 9 } );
  v1.setBit( 5, WIR_L4::bL, { rp1, 9 } );
  v1.setBit( 6, WIR_L4::bL, { rp1, 9 } );
  v1.setBit( 7, WIR_L4::bN, { rp1, 9 } );
  v1.setBit( 8, WIR_L4::bL, { rp1, 9 } );
  v1.setBit( 9, WIR_L4::bL, { rp1, 9 } );
  v1.setBit( 10, WIR_L4::bN, { rp1, 9 } );

  WIR_UpDownValue v2 { 11 };
  v2.setBit( 0, WIR_L4::b1 );
  v2.setBit( 1, WIR_L4::b0 );
  v2.setBit( 2, WIR_L4::bN, { rp1, 9 } );
  v2.setBit( 3, WIR_L4::bL, { rp1, 9 } );
  v2.setBit( 4, WIR_L4::bN, { rp1, 9 } );
  v2.setBit( 5, WIR_L4::bN, { rp2, 9 } );
  v2.setBit( 6, WIR_L4::bL, { rp2, 9 } );
  v2.setBit( 7, WIR_L4::bN, { rp2, 9 } );
  v2.setBit( 8, WIR_L4::bN, { rp1, 13 } );
  v2.setBit( 9, WIR_L4::bL, { rp1, 13 } );
  v2.setBit( 10, WIR_L4::bN, { rp1, 13 } );

  // Test bitwise operators.
  auto r1 = v1 & v2;
  ufAssert(
    ( r1[ 0 ] == WIR_L4::bL ) &&
    ( r1.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( r1[ 1 ] == WIR_L4::b0 );
  ufAssert( r1[ 2 ] == WIR_L4::b0 );
  ufAssert(
    ( r1[ 3 ] == WIR_L4::bL ) &&
    ( r1.getLocation( 3 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( r1[ 4 ] == WIR_L4::bN ) &&
    ( r1.getLocation( 4 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( r1[ 5 ] == WIR_L4::bU );
  ufAssert( r1[ 6 ] == WIR_L4::bU );
  ufAssert( r1[ 7 ] == WIR_L4::bU );
  ufAssert( r1[ 8 ] == WIR_L4::bU );
  ufAssert( r1[ 9 ] == WIR_L4::bU );
  ufAssert( r1[ 10 ] == WIR_L4::bU );

  auto r2 = v1 | v2;
  ufAssert( r2[ 0 ] == WIR_L4::b1 );
  ufAssert(
    ( r2[ 1 ] == WIR_L4::bL ) &&
    ( r2.getLocation( 1 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( r2[ 2 ] == WIR_L4::b1 );
  ufAssert(
    ( r2[ 3 ] == WIR_L4::bL ) &&
    ( r2.getLocation( 3 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( r2[ 4 ] == WIR_L4::bN ) &&
    ( r2.getLocation( 4 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( r2[ 5 ] == WIR_L4::bU );
  ufAssert( r2[ 6 ] == WIR_L4::bU );
  ufAssert( r2[ 7 ] == WIR_L4::bU );
  ufAssert( r2[ 8 ] == WIR_L4::bU );
  ufAssert( r2[ 9 ] == WIR_L4::bU );
  ufAssert( r2[ 10 ] == WIR_L4::bU );

  auto r3 = v1 ^ v2;
  ufAssert(
    ( r3[ 0 ] == WIR_L4::bN ) &&
    ( r3.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( r3[ 1 ] == WIR_L4::bL ) &&
    ( r3.getLocation( 1 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( r3[ 2 ] == WIR_L4::b1 );
  ufAssert( r3[ 3 ] == WIR_L4::b0 );
  ufAssert( r3[ 4 ] == WIR_L4::b0 );
  ufAssert( r3[ 5 ] == WIR_L4::bU );
  ufAssert( r3[ 6 ] == WIR_L4::bU );
  ufAssert( r3[ 7 ] == WIR_L4::bU );
  ufAssert( r3[ 8 ] == WIR_L4::bU );
  ufAssert( r3[ 9 ] == WIR_L4::bU );
  ufAssert( r3[ 10 ] == WIR_L4::bU );

  auto r4 = ~v2;
  ufAssert( r4[ 0 ] == WIR_L4::b0 );
  ufAssert( r4[ 1 ] == WIR_L4::b1 );
  ufAssert(
    ( r4[ 2 ] == WIR_L4::bL ) &&
    ( r4.getLocation( 2 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( r4[ 3 ] == WIR_L4::bN ) &&
    ( r4.getLocation( 3 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( r4[ 4 ] == WIR_L4::bL ) &&
    ( r4.getLocation( 4 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( r4[ 5 ] == WIR_L4::bL ) &&
    ( r4.getLocation( 5 ) == WIR_Location( rp2, 9 ) ) );
  ufAssert(
    ( r4[ 6 ] == WIR_L4::bN ) &&
    ( r4.getLocation( 6 ) == WIR_Location( rp2, 9 ) ) );
  ufAssert(
    ( r4[ 7 ] == WIR_L4::bL ) &&
    ( r4.getLocation( 7 ) == WIR_Location( rp2, 9 ) ) );
  ufAssert(
    ( r4[ 8 ] == WIR_L4::bL ) &&
    ( r4.getLocation( 8 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert(
    ( r4[ 9 ] == WIR_L4::bN ) &&
    ( r4.getLocation( 9 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert(
    ( r4[ 10 ] == WIR_L4::bL ) &&
    ( r4.getLocation( 10 ) == WIR_Location( rp1, 13 ) ) );

  // Test << operator.
  // Shift amounts including U, L or N must result in U*.
  WIR_UpDownValue v3 { MIPS_Immediate16_Signed( 26361 ) };

  WIR_UpDownValue s1 { WIR_L4::bU, 1, true };
  WIR_UpDownValue s2 { s1 };
  s2.setBit(
    0, WIR_L4::bL,
    { dynamic_cast<WIR_RegisterParameter &>( o1.begin()->get() ), 9 } );
  WIR_UpDownValue s3 { 3 };
  s3 = s1;
  s3.setBit(
    0, WIR_L4::bN,
    { dynamic_cast<WIR_RegisterParameter &>( o2.begin()->get() ), 9 } );

  WIR_UpDownValue r5 { 5 };
  r5 = v3 << s1;
  ufAssert( r5.getBitWidth() == 16 );
  ufAssert( r5.containsOnlyBit( WIR_L4::bU ) );

  r5 = v3;
  ufAssert( r5.isBinaryInteger() );

  r5 = v3 << s2;
  ufAssert( r5.containsOnlyBit( WIR_L4::bU ) );

  r5 = v3;
  ufAssert( r5.isBinaryInteger() );

  r5 = v3 << s3;
  ufAssert( r5.containsOnlyBit( WIR_L4::bU ) );

  // Shifting by 0 changes nothing.
  WIR_UpDownValue s4 { WIR_L4::b0, 4, true };
  r5 = v3 << s4;
  ufAssert( ( r5 == v3 ) == WIR_L4::b1 );

  // Shifting by large values results in 0 (everything's shifted out).
  WIR_UpDownValue s5 { MIPS_Immediate16_Signed( 64 ) };
  r5 = v3 << s5;
  ufAssert( r5.getSignedValue() == 0 );

  r5 = v3;
  ufAssert( r5.getSignedValue() == 26361 );

  WIR_UpDownValue s6 { MIPS_Immediate16_Signed( -64 ) };
  r5 = v3 << s6;
  ufAssert( r5.getSignedValue() == 0 );

  // Left-shift by some binary integer value.
  WIR_UpDownValue s7 { MIPS_Immediate16_Signed( 5 ) };
  r5 = v3 << s7;
  ufAssert( r5.getSignedValue() == -8416 );

  // Right-shift by some binary integer value.
  WIR_UpDownValue s8 { MIPS_Immediate16_Signed( -5 ) };
  r5 = v3 << s8;
  ufAssert( r5.getSignedValue() == 823 );

  // Left-shift by some integer value containing X.
  WIR_UpDownValue s9 { MIPS_Immediate16_Signed( 5 ) };
  s9.setBit( 1, WIR_L4::bX );
  r5 = v3 << s9;
  ufAssert( r5.getSignedValue() == 31872 );

  // Right-shift by some integer value containing X.
  WIR_UpDownValue s10 { MIPS_Immediate16_Signed( -5 ) };
  s10.setBit( 3, WIR_L4::bX );
  r5 = v3 << s10;
  ufAssert( r5.getSignedValue() == 3 );

  // Right-shift a negative value to test proper sign extension.
  WIR_UpDownValue v4 { MIPS_Immediate16_Signed( -4711 ) };
  r5 = v4 << s10;
  ufAssert( r5.getSignedValue() == -1 );

  // Test >> operator.
  // Shift amounts including U, L or N must result in U*.
  r5 = v3 >> s1;
  ufAssert( r5.getBitWidth() == 16 );
  ufAssert( r5.containsOnlyBit( WIR_L4::bU ) );

  r5 = v3;
  ufAssert( r5.isBinaryInteger() );

  r5 = v3 >> s2;
  ufAssert( r5.containsOnlyBit( WIR_L4::bU ) );

  r5 = v3;
  ufAssert( r5.isBinaryInteger() );

  r5 = v3 >> s3;
  ufAssert( r5.containsOnlyBit( WIR_L4::bU ) );

  // Shifting by 0 changes nothing.
  r5 = v3 >> s4;
  ufAssert( ( r5 == v3 ) == WIR_L4::b1 );

  // Shifting by large values results in 0 (everything's shifted out).
  r5 = v3 >> s5;
  ufAssert( r5.getSignedValue() == 0 );

  r5 = v3;
  ufAssert( r5.getSignedValue() == 26361 );

  r5 = v3 >> s6;
  ufAssert( r5.getSignedValue() == 0 );

  // Right-shift by some binary integer value.
  r5 = v3 >> s7;
  ufAssert( r5.getSignedValue() == 823 );

  // Left-shift by some binary integer value.
  r5 = v3 >> s8;
  ufAssert( r5.getSignedValue() == -8416 );

  // Right-shift by some integer value containing X.
  r5 = v3 >> s9;
  ufAssert( r5.getSignedValue() == 205 );

  // Left-shift by some integer value containing X.
  r5 = v3 >> s10;
  ufAssert( r5.getSignedValue() == 8192 );

  // Right-shift a negative value to test proper sign extension.
  r5 = v4 >> s9;
  ufAssert( r5.getSignedValue() == -37 );

  // Test * operator.
  WIR_UpDownValue num1 { WIR_L4::b1, 16 };
  num1.setBit( 0, WIR_L4::bL, { rp1, 9 } );
  num1.setBit( 2, WIR_L4::bX );
  num1.setBit( 3, WIR_L4::bX );
  num1.setBit( 4, WIR_L4::b0 );
  num1.setBit( 5, WIR_L4::bX );
  num1.setBit( 8, WIR_L4::bN, { rp1, 13 } );
  num1.setBit( 9, WIR_L4::bL, { rp1, 13 } );
  num1.setBit( 11, WIR_L4::bX );
  num1.setBit( 13, WIR_L4::b0 );
  num1.setBit( 14, WIR_L4::b0 );
  num1.setBit( 15, WIR_L4::b0 );

  WIR_UpDownValue num2 { WIR_L4::b1, 16 };
  num2.setBit( 1, WIR_L4::bX );
  num2.setBit( 2, WIR_L4::b0 );
  num2.setBit( 5, WIR_L4::bX );
  num2.setBit( 6, WIR_L4::bN, { rp2, 9 } );
  num2.setBit( 7, WIR_L4::bL, { rp1, 13 } );
  num2.setBit( 8, WIR_L4::b0 );
  num2.setBit( 9, WIR_L4::bN, { rp1, 13 } );
  num2.setBit( 11, WIR_L4::bX );
  num2.setBit( 13, WIR_L4::bL, { rp1, 9 } );
  num2.setBit( 14, WIR_L4::bL, { rp1, 9 } );
  num2.setBit( 15, WIR_L4::b0 );

  WIR_UpDownValue res { 32 };

  res = num1 * num2;
  ufAssert(
    ( res[ 0 ] == WIR_L4::bL ) &&
    ( res.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( res[ 1 ] == WIR_L4::bN ) &&
    ( res.getLocation( 1 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( res[ 2 ] == WIR_L4::bL ) &&
    ( res.getLocation( 2 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( res[ 3 ] == WIR_L4::bL ) &&
    ( res.getLocation( 3 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( res[ 4 ] == WIR_L4::bN ) &&
    ( res.getLocation( 4 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( res[ 5 ] == WIR_L4::bX );
  ufAssert( res.extract( 6, 26 ).containsOnlyBit( WIR_L4::bU ) );
  ufAssert( res.isSigned() && ( res.getBitWidth() == 32 ) );

  res = num2 * num1;
  ufAssert( res.containsOnlyBit( WIR_L4::bU ) );

  res = num1 * zero;
  ufAssert( res.getSignedValue() == 0 );

  auto res1 = zero * num1;
  ufAssert( res1.containsOnlyBit( WIR_L4::b0 ) );
  ufAssert( res1.isSigned() && ( res1.getBitWidth() == 32 ) );

  res = num1 * one;
  for ( unsigned int i = 0; i < 16; ++i ) {
    ufAssert( res[ i ] == num1[ i ] );
    if ( getLevel( res[ i ] ) == 1 )
      ufAssert( res.getLocation( i ) == num1.getLocation( i ) );
  }
  ufAssert( res.extract( 16, 16 ).containsOnlyBit( WIR_L4::b0 ) );

  res = one * num1;
  for ( unsigned int i = 0; i < 16; ++i ) {
    ufAssert( res[ i ] == num1[ i ] );
    if ( getLevel( res[ i ] ) == 1 )
      ufAssert( res.getLocation( i ) == num1.getLocation( i ) );
  }
  ufAssert( res.extract( 16, 16 ).containsOnlyBit( WIR_L4::b0 ) );

  res = num1 * minusOne;
  auto tmp = -num1;
  for ( unsigned int i = 0; i < 16; ++i ) {
    ufAssert( res[ i ] == tmp[ i ] );
    if ( getLevel( res[ i ] ) == 1 )
      ufAssert( res.getLocation( i ) == tmp.getLocation( i ) );
  }
  ufAssert( res.extract( 16, 16 ).containsOnlyBit( WIR_L4::b1 ) );

  res = minusOne * num1;
  for ( unsigned int i = 0; i < 16; ++i ) {
    ufAssert( res[ i ] == tmp[ i ] );
    if ( getLevel( res[ i ] ) == 1 )
      ufAssert( res.getLocation( i ) == tmp.getLocation( i ) );
  }
  ufAssert( res.extract( 16, 16 ).containsOnlyBit( WIR_L4::b1 ) );

  // Test / and % operators.
  num1.setBit( 15, WIR_L4::b1 );
  num2.setBit( 15, WIR_L4::bU );
  auto result = num1 / num2;
  ufAssert( result.containsOnlyBit( WIR_L4::bU ) );
  ufAssert( result.isSigned() && ( result.getBitWidth() == 16 ) );

  auto rest = num1 % num2;
  ufAssert(
    ( rest[ 0 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( rest[ 1 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 1 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( rest.extract( 2, 2 ).containsOnlyBit( WIR_L4::bX ) );
  ufAssert( rest[ 4 ] == WIR_L4::b1 );
  ufAssert( rest[ 5 ] == WIR_L4::bX );
  ufAssert( rest.extract( 6, 2 ).containsOnlyBit( WIR_L4::b0 ) );
  ufAssert(
    ( rest[ 8 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 8 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert(
    ( rest[ 9 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 9 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert( rest[ 10 ] == WIR_L4::b0 );
  ufAssert( rest[ 11 ] == WIR_L4::bX );
  ufAssert( rest[ 12 ] == WIR_L4::b0 );
  ufAssert( rest.extract( 13, 2 ).containsOnlyBit( WIR_L4::b1 ) );
  ufAssert( rest[ 15 ] == WIR_L4::b0 );
  ufAssert( rest.isSigned() && ( rest.getBitWidth() == 16 ) );

  num1.setBit( 15, WIR_L4::bU );
  num2.setBit( 15, WIR_L4::b1 );
  result = num1 / num2;
  ufAssert( result.extract( 0, 5 ).containsOnlyBit( WIR_L4::bU ) );
  ufAssert( result.extract( 5, 11 ).containsOnlyBit( WIR_L4::b0 ) );

  rest = num1 % num2;
  ufAssert(
    ( rest[ 0 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( rest[ 1 ] == WIR_L4::b1 );
  ufAssert( rest.extract( 2, 2 ).containsOnlyBit( WIR_L4::bX ) );
  ufAssert( rest[ 4 ] == WIR_L4::b0 );
  ufAssert( rest[ 5 ] == WIR_L4::bX );
  ufAssert( rest.extract( 6, 2 ).containsOnlyBit( WIR_L4::b1 ) );
  ufAssert(
    ( rest[ 8 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 8 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert(
    ( rest[ 9 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 9 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert( rest[ 10 ] == WIR_L4::b1 );
  ufAssert( rest[ 11 ] == WIR_L4::bX );
  ufAssert( rest[ 12 ] == WIR_L4::b1 );
  ufAssert( rest.extract( 13, 2 ).containsOnlyBit( WIR_L4::b0 ) );
  ufAssert( rest[ 15 ] == WIR_L4::bU );

  num1.setBit( 15, WIR_L4::bX );
  num2.setBit( 15, WIR_L4::b1 );
  result = num1 / num2;
  ufAssert(
    ( result[ 0 ] == WIR_L4::bN ) &&
    ( result.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( result.extract( 1, 15 ).containsOnlyBit( WIR_L4::b0 ) );

  rest = num1 % num2;
  ufAssert(
    ( rest[ 0 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( rest[ 1 ] == WIR_L4::b1 );
  ufAssert( rest.extract( 2, 2 ).containsOnlyBit( WIR_L4::bX ) );
  ufAssert( rest[ 4 ] == WIR_L4::b0 );
  ufAssert( rest[ 5 ] == WIR_L4::bX );
  ufAssert( rest.extract( 6, 2 ).containsOnlyBit( WIR_L4::b1 ) );
  ufAssert(
    ( rest[ 8 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 8 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert(
    ( rest[ 9 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 9 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert( rest[ 10 ] == WIR_L4::b1 );
  ufAssert( rest[ 11 ] == WIR_L4::bX );
  ufAssert( rest[ 12 ] == WIR_L4::b1 );
  ufAssert( rest.extract( 13, 2 ).containsOnlyBit( WIR_L4::b0 ) );
  ufAssert( rest[ 15 ] == WIR_L4::bX );

  num1.setBit( 15, WIR_L4::b1 );
  num2.setBit( 15, WIR_L4::b1 );
  result = num1 / num2;
  ufAssert(
    ( result[ 0 ] == WIR_L4::bL ) &&
    ( result.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 1 ] == WIR_L4::bL ) &&
    ( result.getLocation( 1 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 2 ] == WIR_L4::bL ) &&
    ( result.getLocation( 2 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( result.extract( 3, 13 ).containsOnlyBit( WIR_L4::b0 ) );

  rest = num1 % num2;
  ufAssert(
    ( rest[ 0 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( rest[ 1 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 1 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( rest.extract( 2, 2 ).containsOnlyBit( WIR_L4::bX ) );
  ufAssert( rest[ 4 ] == WIR_L4::b1 );
  ufAssert( rest[ 5 ] == WIR_L4::bX );
  ufAssert( rest.extract( 6, 2 ).containsOnlyBit( WIR_L4::b0 ) );
  ufAssert(
    ( rest[ 8 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 8 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert(
    ( rest[ 9 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 9 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert( rest[ 10 ] == WIR_L4::b0 );
  ufAssert( rest[ 11 ] == WIR_L4::bX );
  ufAssert( rest[ 12 ] == WIR_L4::b0 );
  ufAssert( rest.extract( 13, 2 ).containsOnlyBit( WIR_L4::b1 ) );
  ufAssert( rest[ 15 ] == WIR_L4::b0 );

  num1.setBit( 15, WIR_L4::bL, { rp1, 9 } );
  num2.setBit( 15, WIR_L4::bL, { rp1, 9 } );
  result = num1 / num2;
  ufAssert( result[ 0 ] == WIR_L4::b0 );
  ufAssert(
    ( result[ 1 ] == WIR_L4::bL ) &&
    ( result.getLocation( 1 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 2 ] == WIR_L4::bL ) &&
    ( result.getLocation( 2 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 3 ] == WIR_L4::bL ) &&
    ( result.getLocation( 3 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 4 ] == WIR_L4::bL ) &&
    ( result.getLocation( 4 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 5 ] == WIR_L4::bL ) &&
    ( result.getLocation( 5 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 6 ] == WIR_L4::bL ) &&
    ( result.getLocation( 6 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 7 ] == WIR_L4::bL ) &&
    ( result.getLocation( 7 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 8 ] == WIR_L4::bL ) &&
    ( result.getLocation( 8 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 9 ] == WIR_L4::bL ) &&
    ( result.getLocation( 9 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 10 ] == WIR_L4::bL ) &&
    ( result.getLocation( 10 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 11 ] == WIR_L4::bL ) &&
    ( result.getLocation( 11 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 12 ] == WIR_L4::bL ) &&
    ( result.getLocation( 12 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 13 ] == WIR_L4::bL ) &&
    ( result.getLocation( 13 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 14 ] == WIR_L4::bL ) &&
    ( result.getLocation( 14 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 15 ] == WIR_L4::bL ) &&
    ( result.getLocation( 15 ) == WIR_Location( rp1, 9 ) ) );

  rest = num1 % num2;
  ufAssert(
    ( rest[ 0 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( rest[ 0 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( rest[ 1 ] == WIR_L4::b1 );
  ufAssert( rest.extract( 2, 2 ).containsOnlyBit( WIR_L4::bX ) );
  ufAssert( rest[ 4 ] == WIR_L4::b0 );
  ufAssert( rest[ 5 ] == WIR_L4::bX );
  ufAssert( rest.extract( 6, 2 ).containsOnlyBit( WIR_L4::b1 ) );
  ufAssert(
    ( rest[ 8 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 8 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert(
    ( rest[ 9 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 9 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert( rest[ 10 ] == WIR_L4::b1 );
  ufAssert( rest[ 11 ] == WIR_L4::bX );
  ufAssert( rest[ 12 ] == WIR_L4::b1 );
  ufAssert( rest.extract( 13, 2 ).containsOnlyBit( WIR_L4::b0 ) );
  ufAssert(
    ( rest[ 15 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 15 ) == WIR_Location( rp1, 9 ) ) );

  num1.setBit( 15, WIR_L4::b0 );
  num2.setBit( 15, WIR_L4::b1 );
  result = num1 / num2;
  ufAssert(
    ( result[ 0 ] == WIR_L4::bL ) &&
    ( result.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 1 ] == WIR_L4::bL ) &&
    ( result.getLocation( 1 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 2 ] == WIR_L4::bL ) &&
    ( result.getLocation( 2 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 3 ] == WIR_L4::bL ) &&
    ( result.getLocation( 3 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 4 ] == WIR_L4::bL ) &&
    ( result.getLocation( 4 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 5 ] == WIR_L4::bL ) &&
    ( result.getLocation( 5 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 6 ] == WIR_L4::bL ) &&
    ( result.getLocation( 6 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 7 ] == WIR_L4::bL ) &&
    ( result.getLocation( 7 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 8 ] == WIR_L4::bL ) &&
    ( result.getLocation( 8 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 9 ] == WIR_L4::bL ) &&
    ( result.getLocation( 9 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 10 ] == WIR_L4::bL ) &&
    ( result.getLocation( 10 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 11 ] == WIR_L4::bL ) &&
    ( result.getLocation( 11 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 12 ] == WIR_L4::bL ) &&
    ( result.getLocation( 12 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 13 ] == WIR_L4::bL ) &&
    ( result.getLocation( 13 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 14 ] == WIR_L4::bL ) &&
    ( result.getLocation( 14 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 15 ] == WIR_L4::bL ) &&
    ( result.getLocation( 15 ) == WIR_Location( rp1, 9 ) ) );

  rest = num1 % num2;
  ufAssert(
    ( rest[ 0 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( rest[ 1 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 1 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( rest.extract( 2, 2 ).containsOnlyBit( WIR_L4::bX ) );
  ufAssert( rest[ 4 ] == WIR_L4::b1 );
  ufAssert( rest[ 5 ] == WIR_L4::bX );
  ufAssert( rest.extract( 6, 2 ).containsOnlyBit( WIR_L4::b0 ) );
  ufAssert(
    ( rest[ 8 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 8 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert(
    ( rest[ 9 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 9 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert( rest[ 10 ] == WIR_L4::b0 );
  ufAssert( rest[ 11 ] == WIR_L4::bX );
  ufAssert( rest[ 12 ] == WIR_L4::b0 );
  ufAssert( rest.extract( 13, 3 ).containsOnlyBit( WIR_L4::b1 ) );

  num1.setBit( 15, WIR_L4::b1 );
  num2.setBit( 15, WIR_L4::b0 );
  result = num1 / num2;
  ufAssert(
    ( result[ 0 ] == WIR_L4::bN ) &&
    ( result.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( result[ 1 ] == WIR_L4::b0 );
  ufAssert(
    ( result[ 2 ] == WIR_L4::bN ) &&
    ( result.getLocation( 2 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 3 ] == WIR_L4::bN ) &&
    ( result.getLocation( 3 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 4 ] == WIR_L4::bN ) &&
    ( result.getLocation( 4 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 5 ] == WIR_L4::bN ) &&
    ( result.getLocation( 5 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 6 ] == WIR_L4::bN ) &&
    ( result.getLocation( 6 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 7 ] == WIR_L4::bN ) &&
    ( result.getLocation( 7 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 8 ] == WIR_L4::bN ) &&
    ( result.getLocation( 8 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 9 ] == WIR_L4::bN ) &&
    ( result.getLocation( 9 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 10 ] == WIR_L4::bN ) &&
    ( result.getLocation( 10 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 11 ] == WIR_L4::bN ) &&
    ( result.getLocation( 11 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 12 ] == WIR_L4::bN ) &&
    ( result.getLocation( 12 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 13 ] == WIR_L4::bN ) &&
    ( result.getLocation( 13 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 14 ] == WIR_L4::bN ) &&
    ( result.getLocation( 14 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( result[ 15 ] == WIR_L4::bN ) &&
    ( result.getLocation( 15 ) == WIR_Location( rp1, 9 ) ) );

  rest = num1 % num2;
  ufAssert(
    ( rest[ 0 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 0 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( rest[ 1 ] == WIR_L4::b1 );
  ufAssert( rest.extract( 2, 2 ).containsOnlyBit( WIR_L4::bX ) );
  ufAssert( rest[ 4 ] == WIR_L4::b0 );
  ufAssert( rest[ 5 ] == WIR_L4::bX );
  ufAssert( rest.extract( 6, 2 ).containsOnlyBit( WIR_L4::b1 ) );
  ufAssert(
    ( rest[ 8 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 8 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert(
    ( rest[ 9 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 9 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert( rest[ 10 ] == WIR_L4::b1 );
  ufAssert( rest[ 11 ] == WIR_L4::bX );
  ufAssert( rest[ 12 ] == WIR_L4::b1 );
  ufAssert( rest.extract( 13, 2 ).containsOnlyBit( WIR_L4::b0 ) );
  ufAssert( rest[ 15 ] == WIR_L4::b1 );

  result = num2 / num2;
  ufAssert( result[ 0 ] == WIR_L4::bX );
  ufAssert( result[ 1 ] == WIR_L4::b0 );
  ufAssert( result[ 2 ] == WIR_L4::bU );
  ufAssert( result.extract( 3, 13 ).containsOnlyBit( WIR_L4::b0 ) );

  rest = num2 % num2;
  ufAssert( rest[ 0 ] == WIR_L4::b1 );
  ufAssert( rest[ 1 ] == WIR_L4::bX );
  ufAssert( rest[ 2 ] == WIR_L4::b0 );
  ufAssert( rest.extract( 3, 2 ).containsOnlyBit( WIR_L4::b1 ) );
  ufAssert( rest[ 5 ] == WIR_L4::bX );
  ufAssert(
    ( rest[ 6 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 6 ) == WIR_Location( rp2, 9 ) ) );
  ufAssert(
    ( rest[ 7 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 7 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert( rest[ 8 ] == WIR_L4::b0 );
  ufAssert(
    ( rest[ 9 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 9 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert( rest[ 10 ] == WIR_L4::b1 );
  ufAssert( rest[ 11 ] == WIR_L4::bX );
  ufAssert( rest[ 12 ] == WIR_L4::b1 );
  ufAssert(
    ( rest[ 13 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 13 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( rest[ 14 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 14 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( rest[ 15 ] == WIR_L4::b0 );

  result = -num2 / num2;
  ufAssert( result.extract( 0, 2 ).containsOnlyBit( WIR_L4::bX ) );
  ufAssert( result[ 2 ] == WIR_L4::bU );
  ufAssert( result.extract( 3, 13 ).containsOnlyBit( WIR_L4::b1 ) );

  rest = -num2 % num2;
  ufAssert( rest[ 0 ] == WIR_L4::b1 );
  ufAssert( rest[ 1 ] == WIR_L4::bX );
  ufAssert( rest[ 2 ] == WIR_L4::b1 );
  ufAssert( rest.extract( 3, 2 ).containsOnlyBit( WIR_L4::b0 ) );
  ufAssert( rest[ 5 ] == WIR_L4::bX );
  ufAssert(
    ( rest[ 6 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 6 ) == WIR_Location( rp2, 9 ) ) );
  ufAssert(
    ( rest[ 7 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 7 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert( rest[ 8 ] == WIR_L4::b1 );
  ufAssert(
    ( rest[ 9 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 9 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert( rest[ 10 ] == WIR_L4::b0 );
  ufAssert( rest[ 11 ] == WIR_L4::bX );
  ufAssert( rest[ 12 ] == WIR_L4::b0 );
  ufAssert(
    ( rest[ 13 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 13 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( rest[ 14 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 14 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( rest[ 15 ] == WIR_L4::b1 );

  result = num2 / -num2;
  ufAssert( result.extract( 0, 2 ).containsOnlyBit( WIR_L4::bX ) );
  ufAssert( result[ 2 ] == WIR_L4::bU );
  ufAssert( result.extract( 3, 13 ).containsOnlyBit( WIR_L4::b1 ) );

  rest = num2 % -num2;
  ufAssert( rest[ 0 ] == WIR_L4::b1 );
  ufAssert( rest[ 1 ] == WIR_L4::bX );
  ufAssert( rest[ 2 ] == WIR_L4::b1 );
  ufAssert( rest.extract( 3, 2 ).containsOnlyBit( WIR_L4::b0 ) );
  ufAssert( rest[ 5 ] == WIR_L4::bX );
  ufAssert(
    ( rest[ 6 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 6 ) == WIR_Location( rp2, 9 ) ) );
  ufAssert(
    ( rest[ 7 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 7 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert( rest[ 8 ] == WIR_L4::b1 );
  ufAssert(
    ( rest[ 9 ] == WIR_L4::bL ) &&
    ( rest.getLocation( 9 ) == WIR_Location( rp1, 13 ) ) );
  ufAssert( rest[ 10 ] == WIR_L4::b0 );
  ufAssert( rest[ 11 ] == WIR_L4::bX );
  ufAssert( rest[ 12 ] == WIR_L4::b0 );
  ufAssert(
    ( rest[ 13 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 13 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert(
    ( rest[ 14 ] == WIR_L4::bN ) &&
    ( rest.getLocation( 14 ) == WIR_Location( rp1, 9 ) ) );
  ufAssert( rest[ 15 ] == WIR_L4::b1 );

  num2.setAllBits( WIR_L4::b0 );
  result = num2 / num1;
  rest = num2 % num1;
  ufAssert( result.getSignedValue() == 0 );
  ufAssert( rest.getSignedValue() == 0 );

  return( 0 );
}
