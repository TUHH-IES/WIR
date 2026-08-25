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
#include <arch/tricore/optimizations/siliconbugs/tc1796_cpu_tc_071.h>


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
  auto &b3 = f.pushBackBasicBlock( {} );

  b1.pushBackInstruction(
    { { TC13::OpCode::ADD, TC13::OperationFormat::DDD_1,
        WIR_RegisterParameter( p.D7(), WIR_Usage::def ),
        WIR_RegisterParameter( p.D8(), WIR_Usage::use ),
        WIR_RegisterParameter( p.D9(), WIR_Usage::use ) } } );
  b2.pushBackInstruction(
    { { TC13::OpCode::LOOPU, TC13::OperationFormat::L,
        WIR_LabelParameter( b1 ) } } );
  b3.pushBackInstruction(
    { { TC13::OpCode::LOOP, TC13::OperationFormat::AL_3,
        WIR_RegisterParameter( p.A7(), WIR_Usage::defuse ),
        WIR_LabelParameter( b2 ) } } );

  TC1796_CPU_TC_071 tc071( f );
  tc071.optimize();

  ufAssert( b1.getInstructions().size() == 1 );
  auto it = b1.begin();
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::ADD );

  ufAssert( b2.getInstructions().size() == 2 );
  it = b2.begin();
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::NOP );
  ++it;
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::LOOPU );

  ufAssert( b3.getInstructions().size() == 1 );
  it = b3.begin();
  ufAssert( it->get().begin()->get().getOpCode() == TC13::OpCode::LOOP );

  for ( WIR_BasicBlock &b : f )
    for ( WIR_Instruction &i : b )
      i.setDontOptimize( false );

  return( 0 );
}
