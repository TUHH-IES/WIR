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
  // operation CLUI.

  RV32I p;
  WIR_CompilationUnit c;
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

  // This lambda serves to retrieve an operation's outgoing down value.
  auto dval = []( const WIR_Operation &o,
                  unsigned int pos ) -> const WIR_UpDownValue & {
    auto it = o.begin();
    std::advance( it, pos );
    auto &c = it->get().getContainers<WIR_BitValues>().begin()->get();
    return( c.getOutValues().begin()->downVal );
  };

  // Create WIR code.

  auto &clui1 = rv32op(
    { RV32IC::OpCode::CLUI, RV32IC::OperationFormat::SRC20_1,
      new WIR_RegisterParameter( input1, WIR_Usage::def ),
      new RV_NZConst20_Unsigned( 0xFFFE0 ) } );

  rv32op(
    { RV32I::OpCode::SLLI, RV32I::OperationFormat::RRC5_1,
      new WIR_RegisterParameter( input1, WIR_Usage::def ),
      new WIR_RegisterParameter( input1, WIR_Usage::use ),
      new RV_Const5_Unsigned( 3 ) } );

  auto &clui2 = rv32op(
    { RV32IC::OpCode::CLUI, RV32IC::OperationFormat::SRC20_1,
      new WIR_RegisterParameter( input2, WIR_Usage::def ),
      new RV_NZConst20_Unsigned( 0xFFFFF ) } );

  rv32op(
    { RV32I::OpCode::OR, RV32I::OperationFormat::RRR_1,
      new WIR_RegisterParameter( input1, WIR_Usage::def ),
      new WIR_RegisterParameter( input1, WIR_Usage::use ),
      new WIR_RegisterParameter( input2, WIR_Usage::use ) } );

  rv32op(
    { RV32I::OpCode::SLLI, RV32I::OperationFormat::RRC5_1,
      new WIR_RegisterParameter( input1, WIR_Usage::def ),
      new WIR_RegisterParameter( input1, WIR_Usage::use ),
      new RV_Const5_Unsigned( 3 ) } );

  // Do bit-true data flow analysis.
  RV32_BitDFA a { f };
  a.analyze();

  // Evaluate analysis results.
  ufAssert( dval( clui1, 0 ).isBinaryInteger() );
  ufAssert( dval( clui1, 0 ).extract( 0, 32 ).getSignedValue() == -131072 );

  ufAssert( dval( clui2, 0 ).isBinaryInteger() );
  ufAssert( dval( clui2, 0 ).extract( 0, 32 ).getSignedValue() == -4096 );

  return( 0 );
}
