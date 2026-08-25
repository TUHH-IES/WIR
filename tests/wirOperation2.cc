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
#include <arch/generic/mips.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  // Check invalid operation format/opcode constellations.

  WIR_Function f( "main" );
  WIR_VirtualRegister &r1 =
    f.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );
  WIR_VirtualRegister &r2 =
    f.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );

  // Assertion (failsafe mode only): RIR_1 is not associated with SW.
  WIR_Operation o1(
    MIPS::OpCode::SW, MIPS::OperationFormat::RIR_1,
    WIR_RegisterParameter( r1, WIR_Usage::use ),
    MIPS_Immediate16_Signed( 112 ),
    WIR_RegisterParameter( r2, WIR_Usage::use ) );

  // Fallback for disabled failsafe mode.
  ufAssertT(
    WIR_Registry::isLegalOperationFormat(
      MIPS::OpCode::SW, MIPS::OperationFormat::RIR_1 ),
    "Instruction format '" << MIPS::OperationFormat::RIR_1.getName() <<
    "' incompatible with opcode '" << MIPS::OpCode::SW.getName() << "'." );

  return( 0 );
}
