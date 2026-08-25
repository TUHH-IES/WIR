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

  ARM_RegV r1;
  ARM_LoRegV r2;
  ARM_HiRegV r3;

  // The following must assert since LDRT and friends only accept post-indexed
  // addressing modes.
  WIR_Operation o(
    ARMv4::OpCode::LDRT, ARMv4::OperationFormat::CRARARAC60_1,
    WIR_ConditionFieldParameter( ARMv4::Condition::le ),
    WIR_RegisterParameter( r1, WIR_Usage::def ),
    WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
    WIR_RegisterParameter( r2, WIR_Usage::defuse ),
    WIR_AddressingModeParameter( ARMv4::AddressingMode::minus ),
    WIR_RegisterParameter( r3, WIR_Usage::use ),
    WIR_AddressingModeParameter( ARMv4::AddressingMode::asr ),
    ARM_Const6_Unsigned0( 32 ) );

  // Fallback for disabled failsafe mode.
  auto it = o.begin();
  ++it;
  ++it;
  auto &regP = dynamic_cast<WIR_AddressingModeParameter &>( it->get() );
  ufAssertT(
    regP.getAddressingMode().isCompatible(
      ARMv4::AddressingMode::pre, ARMv4::OperationFormat::CRARARAC60_1,
      ARMv4::OpCode::LDRT ),
    "Parameters incompatible with operation format '" <<
    o.getOperationFormat().getName() << "'." );

  return( 0 );
}
