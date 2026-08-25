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
#include <string>

// Include WIR headers
#include <wir/wir.h>
#include <arch/generic/mips.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  // Check the entire class hierarchy from WIR_Function down to WIR_Parameter.

  WIR_Function f( "f42" );
  WIR_VirtualRegister &r1 =
    f.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );
  WIR_VirtualRegister &r2 =
    f.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );
  WIR_VirtualRegister r( MIPS::RegisterType::reg );
  WIR_RegisterParameter p1( r1, WIR_Usage::def );
  WIR_RegisterParameter p2( r2, WIR_Usage::use );
  MIPS_Immediate16_Signed p3( 42 );

  WIR_Operation o(
    MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI, p1, p2, p3 );
  WIR_Instruction i;
  WIR_BasicBlock b;

  i.pushBackOperation( o );
  i.pushBackOperation(
    WIR_Operation(
      MIPS::OpCode::SW, MIPS::OperationFormat::RIR_2,
      WIR_RegisterParameter( r, WIR_Usage::use ),
      MIPS_Immediate16_Signed( 112 ),
      WIR_RegisterParameter( r2, WIR_Usage::use ) ) );
  b.pushBackInstruction( i );
  b.pushBackInstruction( WIR_Instruction() );
  f.pushBackBasicBlock( b );
  f.pushBackBasicBlock( WIR_BasicBlock() );


  // The copy constructor must assert, since f's code uses a register r that is
  // not inserted into function f.
  WIR_Function f1( f );

  return( 0 );
}
