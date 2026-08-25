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


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  WIR_StringParameter p1( "foo" );
  string s { "bar" };
  WIR_StringParameter p6( s );

  // Check properties of the created parameters.
  ufAssert( p1.getType() == WIR_ParameterType::str );
  ufAssert( p6.getType() == WIR_ParameterType::str );

  ufAssert( p1.getString() == "foo" );
  ufAssert( p6.getString() == "bar" );

  p1.setString( "bar" );
  string name = p1.getString();
  ufAssert( p1.getString() == "bar" );

  s = "foo";
  p6.setString( s );
  ufAssert( p6.getString() == "foo" );

  // Test the copy constructors.
  WIR_StringParameter p2( p1 );

  ufAssert( p2.getType() == WIR_ParameterType::str );

  ufAssert( p2.getString() == name );

  // Test the move constructors.
  WIR_StringParameter p3( std::move( p2 ) );

  ufAssert( p3.getType() == WIR_ParameterType::str );

  ufAssert( p3.getString() == name );
  ufAssert( p2.getString() == "" );

  // Test the copy assignment operator.
  WIR_StringParameter p4( "bla" );
  p4 = p1;

  ufAssert( p4.getType() == WIR_ParameterType::str );

  ufAssert( p4.getString() == name );

  // Test the move assignment operator.
  WIR_StringParameter p5( "xyz" );
  p5 = std::move( p3 );

  ufAssert( p5.getType() == WIR_ParameterType::str );

  ufAssert( p5.getString() == name );
  ufAssert( p3.getString() == "" );

  return( 0 );
}
