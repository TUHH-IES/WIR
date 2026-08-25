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
#include <iterator>

// Include libuseful headers
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32imc.h>
#include <arch/riscv/analyses/bit/rv32bitdfa.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  // This test case tests the bit-true top-down data flow analysis for RISC-V
  // operations LUI followed by ADDI using labels.

  WIR_TaskManager t;
  WIR_System sys( "rudolv.sys", t );
  auto &p = sys.getComponents<RV32I>().begin()->get();

  WIR_CompilationUnit &c = sys.pushBackCompilationUnit( {} );
  c.setName( __FILE__ );
  WIR_Function &f = c.pushBackFunction( WIR_Function { "main" } );
  WIR_BasicBlock &b1 = f.pushBackBasicBlock( {} );
  auto &input1 = f.pushBackVirtualRegister( RV_RegV() );
  auto &input2 = f.pushBackVirtualRegister( RV_RegV() );

  // This lambda serves for generating and inserting a RISC-V operation.
  auto rv32op = [&]( WIR_Operation &&o ) -> WIR_Operation & {
    auto &i = b1.pushBackInstruction( WIR_Instruction { WIR_Operation { o } } );
    return( i.begin()->get() );
  };

  // This lambda serves to retrieve an operation's incoming up value.
  auto uval = []( const WIR_Operation &o,
                  unsigned int pos ) -> const WIR_UpDownValue & {
    auto it = o.begin();
    std::advance( it, pos );
    auto &c = it->get().getContainers<WIR_BitValues>().begin()->get();
    return( c.getInValues().begin()->upVal );
  };

  // Create WIR code.

  auto &d1 = c.pushFrontData( WIR_Data( "d1" ) );

  rv32op(
    { RV32I::OpCode::LUI, RV32I::OperationFormat::RL_2,
      new WIR_RegisterParameter( input1, WIR_Usage::def ),
      new WIR_LabelParameter( d1 ) } );

  auto &addi1 = rv32op(
    { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRL_2,
      new WIR_RegisterParameter( input1, WIR_Usage::def ),
      new WIR_RegisterParameter( input1, WIR_Usage::use ),
      new WIR_LabelParameter( d1 ) } );

  auto &or1 = rv32op(
    { RV32I::OpCode::OR, RV32I::OperationFormat::RRR_1,
      new WIR_RegisterParameter( input2, WIR_Usage::def ),
      new WIR_RegisterParameter( input1, WIR_Usage::use ),
      new WIR_RegisterParameter( p.x0(), WIR_Usage::use ) } );

  // Do bit-true data flow analysis.
  RV32_BitDFA a { f };
  a.analyze();

  // Evaluate analysis results.
  ufAssert( uval( or1, 1 ).containsOnlyBit( WIR_L4::bL ) );
  for ( unsigned int i = 0; i < 32; ++i ) {
    auto &loc = uval( or1, 1 ).getLocation( i );
    ufAssert( loc.isSymbol() );
    ufAssert( loc.getSymbol() == sys.findSymbol( d1 ) );
    ufAssert( loc.getBitPosition() == i );
  }

  ufAssert( uval( addi1, 1 ).extract( 0, 12 ).containsOnlyBit( WIR_L4::b0 ) );
  ufAssert( uval( addi1, 1 ).extract( 12, 20 ).containsOnlyBit( WIR_L4::bL ) );
  for ( unsigned int i = 12; i < 32; ++i ) {
    auto &loc = uval( addi1, 1 ).getLocation( i );
    ufAssert( loc.isSymbol() );
    ufAssert( loc.getSymbol() == sys.findSymbol( d1 ) );
    ufAssert( loc.getBitPosition() == i );
  }

  return( 0 );
}
