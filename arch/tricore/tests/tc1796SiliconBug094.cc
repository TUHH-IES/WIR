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
#include <arch/tricore/optimizations/siliconbugs/tc1796_cpu_tc_094.h>


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
    { { TC13::OpCode::JNEI, TC13::OperationFormat::DC4L_3,
        WIR_RegisterParameter( p.D13(), WIR_Usage::defuse ),
        TC_Const4_Signed( -8 ), WIR_LabelParameter( b2 ) } } );
  b1.pushBackInstruction(
    { { TC13::OpCode::SYSCALL, TC13::OperationFormat::C9PSW,
        TC_Const9_Unsigned( 42 ),
        WIR_RegisterParameter( p.PSW_C(), WIR_Usage::use ) } } );

  TC1796_CPU_TC_094 tc094( f );
  tc094.optimize();

  ufAssert( b1.getInstructions().size() == 3 );
  auto it = b1.begin();
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::JNEI );
  ++it;
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::NOP );
  ++it;
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::SYSCALL );

  for ( WIR_BasicBlock &b : f )
    for ( WIR_Instruction &i : b )
      i.setDontOptimize( false );

  return( 0 );
}
