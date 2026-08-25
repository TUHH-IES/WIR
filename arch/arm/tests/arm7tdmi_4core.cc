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
#include <sstream>

// Include libuseful headers
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/arm/armv4t.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();
  WIR_TaskManager t;
  WIR_System sys( "arm7tdmi_4core.sys", t );

  ufAssert( sys.getComponents<WIR_BaseProcessor>().size() == 4 );

  auto &p1 = sys.getComponents<ARMv4T>().begin()->get();
  ufAssert( p1.getName() == "CORE0" );
  ufAssert( p1.getISAName() == "ARMv4T" );
  ufAssert( p1.getClockFrequency() == 200000000 );
  ufAssert( p1.getVoltage() == 3.3f );
  ufAssert( p1.isInserted() );
  ufAssert( p1.getSystem() == sys );

  auto &p2 = sys.getComponents<ARMv4T>().rbegin()->get();
  ufAssert( p2.getName() == "CORE3" );
  ufAssert( p2.getISAName() == "ARMv4T" );
  ufAssert( p2.getClockFrequency() == 200000000 );
  ufAssert( p2.getVoltage() == 3.3f );
  ufAssert( p2.isInserted() );
  ufAssert( p2.getSystem() == sys );

  return( 0 );
}
