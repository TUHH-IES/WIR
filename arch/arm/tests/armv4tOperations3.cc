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
#include <arch/arm/armv4t.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  ARM_RegV r1;
  ARM_HiRegV r2;

  // The following must assert since THUMB operations only accept low registers.
  WIR_Operation o1(
    ARMv4T::OpCode::MOV, ARMv4T::OperationFormat::TRR_1,
    WIR_RegisterParameter( r1, WIR_Usage::def ),
    WIR_RegisterParameter( r2, WIR_Usage::use ) );

  // Fallback for disabled failsafe mode.
  auto it = o1.end();
  --it;
  auto &regP = dynamic_cast<WIR_RegisterParameter &>( it->get() );
  ufAssertT(
    regP.getRegister().getType().isCompatible(
      WIR_RegisterParameter(
        ARM_LoRegV(), WIR_Usage::use ).getRegister().getType(),
      regP.getRegister(), ARM_LoRegV() ) ||
      ( regP.getUsage() != WIR_Usage::use ),
    "Parameters incompatible with operation format '" <<
    o1.getOperationFormat().getName() << "'." );

  return( 0 );
}
