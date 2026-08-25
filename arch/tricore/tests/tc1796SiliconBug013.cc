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
#include <arch/tricore/tc131.h>
#include <arch/tricore/optimizations/siliconbugs/tc1796_cpu_tc_013.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();
  WIR_TaskManager t;
  WIR_System sys( "tc1796.sys", t );

  auto &p = sys.getComponents<TC13>().begin()->get();

  auto &c = sys.pushBackCompilationUnit( {} );
  auto &f = c.pushBackFunction( WIR_Function( "main" ) );
  auto &b1 = f.pushBackBasicBlock( {} );
  auto &b2 = f.pushBackBasicBlock( {} );

  b1.pushBackInstruction(
    { { TC13::OpCode::LD_A, TC13::OperationFormat::AC18ABSA,
        WIR_RegisterParameter( p.A7(), WIR_Usage::def ),
        TC_Const18_Unsigned( 42 ) } } );
  b1.pushBackInstruction(
    { { TC13::OpCode::STLCX, TC13::OperationFormat::AC10BOA,
        WIR_RegisterParameter( p.A7(), WIR_Usage::use ),
        TC_Const10_Signed( 444 ) } } );
  b1.pushBackInstruction(
    { { TC13::OpCode::LD_DA, TC13::OperationFormat::PC18ABSA,
        WIR_RegisterParameter( p.P2(), WIR_Usage::def ),
        TC_Const18_Unsigned( 23 ) } } );
  b2.pushBackInstruction(
    { { TC13::OpCode::LDUCX, TC13::OperationFormat::AC10BOA,
        WIR_RegisterParameter( p.A3(), WIR_Usage::use ),
        TC_Const10_Signed( 236 ) } } );

  TC1796_CPU_TC_013 tc013( f );
  tc013.optimize();

  ufAssert( b1.getInstructions().size() == 4 );
  auto it = b1.begin();
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::LD_A );
  ++it;
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::NOP );
  ++it;
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::STLCX );
  ++it;
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::LD_DA );

  ufAssert( b2.getInstructions().size() == 2 );
  it = b2.begin();
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::NOP );
  ++it;
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::LDUCX );

  for ( WIR_BasicBlock &b : f )
    for ( WIR_Instruction &i : b )
      i.setDontOptimize( false );

  return( 0 );
}
