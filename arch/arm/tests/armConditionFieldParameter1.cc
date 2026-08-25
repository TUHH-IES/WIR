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
#include <arch/arm/armv4.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  WIR_ConditionFieldParameter p1( ARMv4::Condition::eq );
  WIR_ConditionFieldParameter p2( ARMv4::Condition::ne );
  WIR_ConditionFieldParameter p3( ARMv4::Condition::hs );
  WIR_ConditionFieldParameter p4( ARMv4::Condition::lo );
  WIR_ConditionFieldParameter p5( ARMv4::Condition::mi );
  WIR_ConditionFieldParameter p6( ARMv4::Condition::pl );
  WIR_ConditionFieldParameter p7( ARMv4::Condition::vs );
  WIR_ConditionFieldParameter p8( ARMv4::Condition::vc );
  WIR_ConditionFieldParameter p9( ARMv4::Condition::hi );
  WIR_ConditionFieldParameter p10( ARMv4::Condition::ls );
  WIR_ConditionFieldParameter p11( ARMv4::Condition::ge );
  WIR_ConditionFieldParameter p12( ARMv4::Condition::lt );
  WIR_ConditionFieldParameter p13( ARMv4::Condition::gt );
  WIR_ConditionFieldParameter p14( ARMv4::Condition::le );
  WIR_ConditionFieldParameter p15( ARMv4::Condition::al );

  // Check properties of the created parameters.
  ufAssert( p1.getType() == WIR_ParameterType::cond );
  ufAssert( p1.getType() == p2.getType() );

  ufAssert( p1.getCondition() == ARMv4::Condition::eq );
  ufAssert( p2.getCondition() == ARMv4::Condition::ne );
  ufAssert( p3.getCondition() == ARMv4::Condition::hs );
  ufAssert( p4.getCondition() == ARMv4::Condition::lo );
  ufAssert( p5.getCondition() == ARMv4::Condition::mi );
  ufAssert( p6.getCondition() == ARMv4::Condition::pl );
  ufAssert( p7.getCondition() == ARMv4::Condition::vs );
  ufAssert( p8.getCondition() == ARMv4::Condition::vc );
  ufAssert( p9.getCondition() == ARMv4::Condition::hi );
  ufAssert( p10.getCondition() == ARMv4::Condition::ls );
  ufAssert( p11.getCondition() == ARMv4::Condition::ge );
  ufAssert( p12.getCondition() == ARMv4::Condition::lt );
  ufAssert( p13.getCondition() == ARMv4::Condition::gt );
  ufAssert( p14.getCondition() == ARMv4::Condition::le );
  ufAssert( p15.getCondition() == ARMv4::Condition::al );

  p1.setCondition( ARMv4::Condition::al );
  p2.setCondition( ARMv4::Condition::eq );
  p3.setCondition( ARMv4::Condition::ne );
  p4.setCondition( ARMv4::Condition::hs );
  p5.setCondition( ARMv4::Condition::lo );
  p6.setCondition( ARMv4::Condition::mi );
  p7.setCondition( ARMv4::Condition::pl );
  p8.setCondition( ARMv4::Condition::vs );
  p9.setCondition( ARMv4::Condition::vc );
  p10.setCondition( ARMv4::Condition::hi );
  p11.setCondition( ARMv4::Condition::ls );
  p12.setCondition( ARMv4::Condition::ge );
  p13.setCondition( ARMv4::Condition::lt );
  p14.setCondition( ARMv4::Condition::gt );
  p15.setCondition( ARMv4::Condition::le );

  ufAssert( p1.getCondition() == ARMv4::Condition::al );
  ufAssert( p2.getCondition() == ARMv4::Condition::eq );
  ufAssert( p3.getCondition() == ARMv4::Condition::ne );
  ufAssert( p4.getCondition() == ARMv4::Condition::hs );
  ufAssert( p5.getCondition() == ARMv4::Condition::lo );
  ufAssert( p6.getCondition() == ARMv4::Condition::mi );
  ufAssert( p7.getCondition() == ARMv4::Condition::pl );
  ufAssert( p8.getCondition() == ARMv4::Condition::vs );
  ufAssert( p9.getCondition() == ARMv4::Condition::vc );
  ufAssert( p10.getCondition() == ARMv4::Condition::hi );
  ufAssert( p11.getCondition() == ARMv4::Condition::ls );
  ufAssert( p12.getCondition() == ARMv4::Condition::ge );
  ufAssert( p13.getCondition() == ARMv4::Condition::lt );
  ufAssert( p14.getCondition() == ARMv4::Condition::gt );
  ufAssert( p15.getCondition() == ARMv4::Condition::le );

  WIR_Parameter &ref1 = p1;
  auto &ref2 = dynamic_cast<WIR_ConditionFieldParameter &>( ref1 );
  ufAssert( ref2.getCondition() == p1.getCondition() );

  // Test the copy constructors.
  WIR_ConditionFieldParameter c1( p1 );
  WIR_ConditionFieldParameter c2( p2 );
  WIR_ConditionFieldParameter c3( p3 );
  WIR_ConditionFieldParameter c4( p4 );
  WIR_ConditionFieldParameter c5( p5 );
  WIR_ConditionFieldParameter c6( p6 );
  WIR_ConditionFieldParameter c7( p7 );
  WIR_ConditionFieldParameter c8( p8 );
  WIR_ConditionFieldParameter c9( p9 );
  WIR_ConditionFieldParameter c10( p10 );
  WIR_ConditionFieldParameter c11( p11 );
  WIR_ConditionFieldParameter c12( p12 );
  WIR_ConditionFieldParameter c13( p13 );
  WIR_ConditionFieldParameter c14( p14 );
  WIR_ConditionFieldParameter c15( p15 );

  ufAssert( c4.getType() == WIR_ParameterType::cond );
  ufAssert( c4.getType() == c5.getType() );

  ufAssert( c1.getCondition() == ARMv4::Condition::al );
  ufAssert( c2.getCondition() == ARMv4::Condition::eq );
  ufAssert( c3.getCondition() == ARMv4::Condition::ne );
  ufAssert( c4.getCondition() == ARMv4::Condition::hs );
  ufAssert( c5.getCondition() == ARMv4::Condition::lo );
  ufAssert( c6.getCondition() == ARMv4::Condition::mi );
  ufAssert( c7.getCondition() == ARMv4::Condition::pl );
  ufAssert( c8.getCondition() == ARMv4::Condition::vs );
  ufAssert( c9.getCondition() == ARMv4::Condition::vc );
  ufAssert( c10.getCondition() == ARMv4::Condition::hi );
  ufAssert( c11.getCondition() == ARMv4::Condition::ls );
  ufAssert( c12.getCondition() == ARMv4::Condition::ge );
  ufAssert( c13.getCondition() == ARMv4::Condition::lt );
  ufAssert( c14.getCondition() == ARMv4::Condition::gt );
  ufAssert( c15.getCondition() == ARMv4::Condition::le );

  // Test the move constructors.
  WIR_ConditionFieldParameter m1( std::move( c1 ) );
  WIR_ConditionFieldParameter m2( std::move( c2 ) );
  WIR_ConditionFieldParameter m3( std::move( c3 ) );
  WIR_ConditionFieldParameter m4( std::move( c4 ) );
  WIR_ConditionFieldParameter m5( std::move( c5 ) );
  WIR_ConditionFieldParameter m6( std::move( c6 ) );
  WIR_ConditionFieldParameter m7( std::move( c7 ) );
  WIR_ConditionFieldParameter m8( std::move( c8 ) );
  WIR_ConditionFieldParameter m9( std::move( c9 ) );
  WIR_ConditionFieldParameter m10( std::move( c10 ) );
  WIR_ConditionFieldParameter m11( std::move( c11 ) );
  WIR_ConditionFieldParameter m12( std::move( c12 ) );
  WIR_ConditionFieldParameter m13( std::move( c13 ) );
  WIR_ConditionFieldParameter m14( std::move( c14 ) );
  WIR_ConditionFieldParameter m15( std::move( c15 ) );

  ufAssert( m7.getType() == WIR_ParameterType::cond );
  ufAssert( m7.getType() == m8.getType() );

  ufAssert( m1.getCondition() == ARMv4::Condition::al );
  ufAssert( m2.getCondition() == ARMv4::Condition::eq );
  ufAssert( m3.getCondition() == ARMv4::Condition::ne );
  ufAssert( m4.getCondition() == ARMv4::Condition::hs );
  ufAssert( m5.getCondition() == ARMv4::Condition::lo );
  ufAssert( m6.getCondition() == ARMv4::Condition::mi );
  ufAssert( m7.getCondition() == ARMv4::Condition::pl );
  ufAssert( m8.getCondition() == ARMv4::Condition::vs );
  ufAssert( m9.getCondition() == ARMv4::Condition::vc );
  ufAssert( m10.getCondition() == ARMv4::Condition::hi );
  ufAssert( m11.getCondition() == ARMv4::Condition::ls );
  ufAssert( m12.getCondition() == ARMv4::Condition::ge );
  ufAssert( m13.getCondition() == ARMv4::Condition::lt );
  ufAssert( m14.getCondition() == ARMv4::Condition::gt );
  ufAssert( m15.getCondition() == ARMv4::Condition::le );

  // Test the copy assignment operator.
  WIR_ConditionFieldParameter ca1( ARMv4::Condition::le );
  ca1 = p1;
  WIR_ConditionFieldParameter ca2( ARMv4::Condition::al );
  ca2 = p2;
  WIR_ConditionFieldParameter ca3( ARMv4::Condition::al );
  ca3 = p3;
  WIR_ConditionFieldParameter ca4( ARMv4::Condition::al );
  ca4 = p4;
  WIR_ConditionFieldParameter ca5( ARMv4::Condition::al );
  ca5 = p5;
  WIR_ConditionFieldParameter ca6( ARMv4::Condition::al );
  ca6 = p6;
  WIR_ConditionFieldParameter ca7( ARMv4::Condition::al );
  ca7 = p7;
  WIR_ConditionFieldParameter ca8( ARMv4::Condition::al );
  ca8 = p8;
  WIR_ConditionFieldParameter ca9( ARMv4::Condition::al );
  ca9 = p9;
  WIR_ConditionFieldParameter ca10( ARMv4::Condition::al );
  ca10 = p10;
  WIR_ConditionFieldParameter ca11( ARMv4::Condition::al );
  ca11 = p11;
  WIR_ConditionFieldParameter ca12( ARMv4::Condition::al );
  ca12 = p12;
  WIR_ConditionFieldParameter ca13( ARMv4::Condition::al );
  ca13 = p13;
  WIR_ConditionFieldParameter ca14( ARMv4::Condition::al );
  ca14 = p14;
  WIR_ConditionFieldParameter ca15( ARMv4::Condition::al );
  ca15 = p15;

  ufAssert( ca7.getType() == WIR_ParameterType::cond );
  ufAssert( ca7.getType() == ca8.getType() );

  ufAssert( ca1.getCondition() == ARMv4::Condition::al );
  ufAssert( ca2.getCondition() == ARMv4::Condition::eq );
  ufAssert( ca3.getCondition() == ARMv4::Condition::ne );
  ufAssert( ca4.getCondition() == ARMv4::Condition::hs );
  ufAssert( ca5.getCondition() == ARMv4::Condition::lo );
  ufAssert( ca6.getCondition() == ARMv4::Condition::mi );
  ufAssert( ca7.getCondition() == ARMv4::Condition::pl );
  ufAssert( ca8.getCondition() == ARMv4::Condition::vs );
  ufAssert( ca9.getCondition() == ARMv4::Condition::vc );
  ufAssert( ca10.getCondition() == ARMv4::Condition::hi );
  ufAssert( ca11.getCondition() == ARMv4::Condition::ls );
  ufAssert( ca12.getCondition() == ARMv4::Condition::ge );
  ufAssert( ca13.getCondition() == ARMv4::Condition::lt );
  ufAssert( ca14.getCondition() == ARMv4::Condition::gt );
  ufAssert( ca15.getCondition() == ARMv4::Condition::le );

  // Test the move assignment operator.
  WIR_ConditionFieldParameter ma1( ARMv4::Condition::le );
  ma1 = std::move( ca1 );
  WIR_ConditionFieldParameter ma2( ARMv4::Condition::al );
  ma2 = std::move( ca2 );
  WIR_ConditionFieldParameter ma3( ARMv4::Condition::al );
  ma3 = std::move( ca3 );
  WIR_ConditionFieldParameter ma4( ARMv4::Condition::al );
  ma4 = std::move( ca4 );
  WIR_ConditionFieldParameter ma5( ARMv4::Condition::al );
  ma5 = std::move( ca5 );
  WIR_ConditionFieldParameter ma6( ARMv4::Condition::al );
  ma6 = std::move( ca6 );
  WIR_ConditionFieldParameter ma7( ARMv4::Condition::al );
  ma7 = std::move( ca7 );
  WIR_ConditionFieldParameter ma8( ARMv4::Condition::al );
  ma8 = std::move( ca8 );
  WIR_ConditionFieldParameter ma9( ARMv4::Condition::al );
  ma9 = std::move( ca9 );
  WIR_ConditionFieldParameter ma10( ARMv4::Condition::al );
  ma10 = std::move( ca10 );
  WIR_ConditionFieldParameter ma11( ARMv4::Condition::al );
  ma11 = std::move( ca11 );
  WIR_ConditionFieldParameter ma12( ARMv4::Condition::al );
  ma12 = std::move( ca12 );
  WIR_ConditionFieldParameter ma13( ARMv4::Condition::al );
  ma13 = std::move( ca13 );
  WIR_ConditionFieldParameter ma14( ARMv4::Condition::al );
  ma14 = std::move( ca14 );
  WIR_ConditionFieldParameter ma15( ARMv4::Condition::al );
  ma15 = std::move( ca15 );

  ufAssert( ma13.getType() == WIR_ParameterType::cond );
  ufAssert( ma13.getType() == ma14.getType() );

  ufAssert( ma1.getCondition() == ARMv4::Condition::al );
  ufAssert( ma2.getCondition() == ARMv4::Condition::eq );
  ufAssert( ma3.getCondition() == ARMv4::Condition::ne );
  ufAssert( ma4.getCondition() == ARMv4::Condition::hs );
  ufAssert( ma5.getCondition() == ARMv4::Condition::lo );
  ufAssert( ma6.getCondition() == ARMv4::Condition::mi );
  ufAssert( ma7.getCondition() == ARMv4::Condition::pl );
  ufAssert( ma8.getCondition() == ARMv4::Condition::vs );
  ufAssert( ma9.getCondition() == ARMv4::Condition::vc );
  ufAssert( ma10.getCondition() == ARMv4::Condition::hi );
  ufAssert( ma11.getCondition() == ARMv4::Condition::ls );
  ufAssert( ma12.getCondition() == ARMv4::Condition::ge );
  ufAssert( ma13.getCondition() == ARMv4::Condition::lt );
  ufAssert( ma14.getCondition() == ARMv4::Condition::gt );
  ufAssert( ma15.getCondition() == ARMv4::Condition::le );

  return( 0 );
}
