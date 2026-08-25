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

  // Assertion: Too many operands for RIR_2.
  WIR_Operation o1(
    MIPS::OpCode::SW, MIPS::OperationFormat::RIR_2,
    WIR_RegisterParameter( r1, WIR_Usage::use ),
    MIPS_Immediate16_Unsigned( 112 ),
    WIR_RegisterParameter( r2, WIR_Usage::use ),
    MIPS_Immediate16_Unsigned( 112 ) );

  // Fallback for disabled failsafe mode.
  ufAssertT(
    o1.getNumberOfExplicitParameters() == 3,
    "Number of parameters incompatible with operation format '" <<
    o1.getOperationFormat().getName() << "'." );

  return( 0 );
}
