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

  // Check name handling of WIR systems.
  WIR_TaskManager t;
  WIR_System sys1( "genericmips.sys", t );
  WIR_System sys2( "genericmips.sys", t );

  string name = "Dummy WIR system";
  sys1.setName( name );
  sys2.setName( "Test WIR system" );

  ufAssert( sys1.getName() == "Dummy WIR system" );
  ufAssert( sys2.getName() == "Test WIR system" );

  // Check copy constructors of WIR systems.
  WIR_System sys3( sys1 );
  ufAssert( sys3.getName() == "Dummy WIR system" );
  WIR_System sys4( std::move( sys2 ) );
  ufAssert( sys2.getName() == "" );
  ufAssert( sys4.getName() == "Test WIR system" );

  // Check assignment operators of WIR systems.
  WIR_System sys5( "genericmips.sys", t );
  sys5 = sys3;
  ufAssert( sys5.getName() == "Dummy WIR system" );
  WIR_System sys6( "genericmips.sys", t );
  sys6 = std::move( sys4 );
  ufAssert( sys4.getName() == "" );
  ufAssert( sys6.getName() == "Test WIR system" );

  return( 0 );
}
