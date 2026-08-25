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
  // operation C.BEQZ.

  RV32I p;
  WIR_CompilationUnit c;
  c.setName( __FILE__ );
  WIR_Function &f = c.pushBackFunction( WIR_Function { "main" } );
  WIR_BasicBlock &b1 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b = f.pushBackBasicBlock( {} );
  auto &input1 = f.pushBackVirtualRegister( RV_RegV() );

  // This lambda serves for generating and inserting a RISC-V operation.
  auto rv32op = [&]( WIR_Operation &&o,
                     WIR_BasicBlock &bb ) -> WIR_Operation & {
    auto &i = bb.pushBackInstruction( WIR_Instruction { WIR_Operation { o } } );
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

  rv32op(
    { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1,
      new WIR_RegisterParameter( input1, WIR_Usage::def ),
      new WIR_RegisterParameter( p.x0(), WIR_Usage::use ),
      new RV_Const12_Signed( 40 ) }, b1 );

  // C.BEQZ
  auto &cbeqz1 = rv32op(
    { RV32IC::OpCode::CBEQZ, RV32IC::OperationFormat::SRL_1,
      new WIR_RegisterParameter( input1, WIR_Usage::use ),
      new WIR_LabelParameter( b ) }, b1 );

  auto &cbeqz2 = rv32op(
    { RV32IC::OpCode::CBEQZ, RV32IC::OperationFormat::SRL_1,
      new WIR_RegisterParameter( p.x0(), WIR_Usage::use ),
      new WIR_LabelParameter( b1 ) }, b );

  // Do bit-true data flow analysis.
  RV32_BitDFA a { f };
  a.analyze();

  // Evaluate analysis results.
  ufAssert( uval( cbeqz1, 0 ).extract( 0, 3 ).containsOnlyBit( WIR_L4::bX ) );
  ufAssert( uval( cbeqz1, 0 ).extract( 3, 1 ).containsOnlyBit( WIR_L4::b1 ) );
  ufAssert( uval( cbeqz1, 0 ).extract( 4, 28 ).containsOnlyBit( WIR_L4::bX ) );

  ufAssert( uval( cbeqz2, 0 ).isBinaryInteger() );
  ufAssert( uval( cbeqz2, 0 ).getSignedValue() == 0 );

  return( 0 );
}
