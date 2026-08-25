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
#include <arch/generic/mips.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  // Check acyclic switch-case regions with structural CFG analysis.

  WIR_Function f( "foo" );
  auto &r1 = f.pushBackVirtualRegister( MIPS_RegV() );
  auto &r2 = f.pushBackVirtualRegister( MIPS_RegV() );

  // For this test CFG, we need 7 basic blocks.
  WIR_BasicBlock &b1 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b2 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b3 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b4 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b5 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b6 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b7 = f.pushBackBasicBlock( {} );

  b1.pushBackInstruction(
    { { MIPS::OpCode::LW, MIPS::OperationFormat::RIR_1,
        WIR_RegisterParameter( r1, WIR_Usage::def ),
        MIPS_Immediate16_Signed( -138 ),
        WIR_RegisterParameter( r2, WIR_Usage::use ) } } );
  b1.pushBackInstruction(
    { { MIPS::OpCode::JR, MIPS::OperationFormat::R_2,
        WIR_RegisterParameter( r1, WIR_Usage::use ) } } );
  b1.rbegin()->get().begin()->get().addJumpTarget( b2 );
  b1.rbegin()->get().begin()->get().addJumpTarget( b3 );
  b1.rbegin()->get().begin()->get().addJumpTarget( b4 );
  b1.rbegin()->get().begin()->get().addJumpTarget( b5 );
  b1.rbegin()->get().begin()->get().addJumpTarget( b6 );

  b2.pushBackInstruction(
    { { MIPS::OpCode::J, MIPS::OperationFormat::L,
        WIR_LabelParameter( b7 ) } } );

  b3.pushBackInstruction(
    { { MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI,
        WIR_RegisterParameter( r1, WIR_Usage::def ),
        WIR_RegisterParameter( r2, WIR_Usage::use ),
        MIPS_Immediate16_Signed( 0 ) } } );

  b4.pushBackInstruction(
    { { MIPS::OpCode::J, MIPS::OperationFormat::L,
        WIR_LabelParameter( b7 ) } } );

  b5.pushBackInstruction(
    { { MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI,
        WIR_RegisterParameter( r1, WIR_Usage::def ),
        WIR_RegisterParameter( r2, WIR_Usage::use ),
        MIPS_Immediate16_Signed( 0 ) } } );

  b6.pushBackInstruction(
    { { MIPS::OpCode::J, MIPS::OperationFormat::L,
        WIR_LabelParameter( b7 ) } } );

  b7.pushBackInstruction(
    { { MIPS::OpCode::JR, MIPS::OperationFormat::R_2,
        WIR_RegisterParameter( r1, WIR_Usage::use ) } } );

  // Do structural analysis of f.
  WIR_StructuralAnalysis a { f };
  a.analyze();

  // Check resulting control tree.
  auto &c = b1.getContainers<WIR_ControlTree>().begin()->get();
  auto &leaf = c.getBasicBlockTreeNode();
  auto &switchcase = leaf.getParent();

  ufAssert( switchcase.getType() == WIR_CTNodeType::switchcase );
  ufAssert( switchcase.isAcyclic() && !switchcase.isCyclic() );
  auto &sw = dynamic_cast<WIR_SwitchCaseTreeNode &>( switchcase );
  ufAssert( switchcase.getEntry() == sw.getCondition() );

  ufAssert(
    dynamic_cast<const WIR_BasicBlockTreeNode &>( sw.getCondition() ).getBasicBlock() == b1 );

  auto it = f.begin();
  ++it;
  for ( WIR_ControlTreeNode &c : sw.getCases() ) {
    ufAssert( c.getType() == WIR_CTNodeType::bb );
    auto &bb = dynamic_cast<WIR_BasicBlockTreeNode &>( c );
    ufAssert( bb.getBasicBlock() == it->get() );
    ++it;
  }
  ufAssert( sw.getFallthroughCases().size() == 2 );
  auto ftit = sw.getFallthroughCases().front();
  ufAssert(
    dynamic_cast<WIR_BasicBlockTreeNode &>( ftit->get() ).getBasicBlock() ==
      b3 );
  ftit = sw.getFallthroughCases().back();
  ufAssert(
    dynamic_cast<WIR_BasicBlockTreeNode &>( ftit->get() ).getBasicBlock() ==
      b5 );

  auto &root = leaf.getRoot();
  ufAssert( root.getType() == WIR_CTNodeType::block );
  auto &b = dynamic_cast<WIR_BlockTreeNode &>( root );

  ufAssert( b.getBlockList().front().get() == switchcase );
  ufAssert(
    dynamic_cast<WIR_BasicBlockTreeNode &>( b.getBlockList().back().get() ).getBasicBlock() == b7 );

  return( 0 );
}
