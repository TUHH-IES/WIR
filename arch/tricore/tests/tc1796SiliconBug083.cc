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
#include <arch/tricore/optimizations/siliconbugs/tc1796_cpu_tc_083.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();
  WIR_TaskManager t;
  WIR_System sys( "tc1796.sys", t );

  auto &c = sys.pushBackCompilationUnit( {} );
  auto &f = c.pushBackFunction( WIR_Function( "main" ) );
  auto &b1 = f.pushBackBasicBlock( {} );

  b1.pushBackInstruction(
    { { TC13::OpCode::DISABLE, TC13::OperationFormat::SYS } } );
  b1.pushBackInstruction(
    { { TC13::OpCode::DISABLE, TC13::OperationFormat::SYS } } );
  b1.pushBackInstruction(
    { { TC13::OpCode::NOP, TC13::OperationFormat::SYS } } );

  TC1796_CPU_TC_083 tc083( f );
  tc083.optimize();

  ufAssert( b1.getInstructions().size() == 4 );
  auto it = b1.begin();
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::DISABLE );
  ++it;
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::NOP );
  ++it;
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::DISABLE );
  ++it;
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::NOP );

  for ( WIR_BasicBlock &b : f )
    for ( WIR_Instruction &i : b )
      i.setDontOptimize( false );

  return( 0 );
}
