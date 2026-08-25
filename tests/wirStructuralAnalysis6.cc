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

  // Check acyclic proper regions with structural CFG analysis.

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

  b2.pushBackInstruction(
    { { MIPS::OpCode::J, MIPS::OperationFormat::L,
        WIR_LabelParameter( b4 ) } } );

  b3.pushBackInstruction(
    { { MIPS::OpCode::JR, MIPS::OperationFormat::R_2,
        WIR_RegisterParameter( r1, WIR_Usage::use ) } } );
  b3.rbegin()->get().begin()->get().addJumpTarget( b4 );
  b3.rbegin()->get().begin()->get().addJumpTarget( b5 );

  b4.pushBackInstruction(
    { { MIPS::OpCode::J, MIPS::OperationFormat::L,
        WIR_LabelParameter( b6 ) } } );

  b5.pushBackInstruction(
    { { MIPS::OpCode::J, MIPS::OperationFormat::L,
        WIR_LabelParameter( b6 ) } } );

  b6.pushBackInstruction(
    { { MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI,
        WIR_RegisterParameter( r1, WIR_Usage::def ),
        WIR_RegisterParameter( r2, WIR_Usage::use ),
        MIPS_Immediate16_Signed( 0 ) } } );

  b7.pushBackInstruction(
    { { MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI,
        WIR_RegisterParameter( r1, WIR_Usage::def ),
        WIR_RegisterParameter( r2, WIR_Usage::use ),
        MIPS_Immediate16_Signed( 0 ) } } );

  // Do structural analysis of f.
  WIR_StructuralAnalysis a { f };
  a.analyze();

  // Check resulting control tree.
  auto &c = b1.getContainers<WIR_ControlTree>().begin()->get();
  auto &leaf = c.getBasicBlockTreeNode();
  auto &proper = leaf.getParent();

  ufAssert( proper.getType() == WIR_CTNodeType::proper );
  ufAssert( proper.isAcyclic() && !proper.isCyclic() );
  auto &pr = dynamic_cast<WIR_ProperTreeNode &>( proper );
  ufAssert(
    dynamic_cast<const WIR_BasicBlockTreeNode &>(
      proper.getEntry() ).getBasicBlock() == b1 );

  set<WIR_id_t> bbIDs;
  bbIDs.insert( b1.getID() );
  bbIDs.insert( b2.getID() );
  bbIDs.insert( b3.getID() );
  bbIDs.insert( b4.getID() );
  bbIDs.insert( b5.getID() );
  bbIDs.insert( b6.getID() );
  bbIDs.insert( b7.getID() );

  for ( WIR_ControlTreeNode &c : pr.getNodes() )
    if ( c.getType() == WIR_CTNodeType::bb )
      bbIDs.erase(
        dynamic_cast<WIR_BasicBlockTreeNode &>( c ).getBasicBlock().getID() );

  ufAssert( bbIDs.size() == 5 );
  ufAssert( bbIDs.count( b2.getID() ) );
  ufAssert( bbIDs.count( b4.getID() ) );
  ufAssert( bbIDs.count( b5.getID() ) );
  ufAssert( bbIDs.count( b6.getID() ) );
  ufAssert( bbIDs.count( b7.getID() ) );

  auto &swtch = proper.getParent();

  ufAssert( swtch.getType() == WIR_CTNodeType::switchcase );
  ufAssert( swtch.isAcyclic() && !swtch.isCyclic() );
  auto &sw = dynamic_cast<WIR_SwitchCaseTreeNode &>( swtch );
  ufAssert( sw.getEntry() == proper );
  ufAssert( sw.getCondition() == proper );

  auto &cases = sw.getCases();
  ufAssert( cases.size() == 3 );
  ufAssert(
    dynamic_cast<const WIR_BasicBlockTreeNode &>(
      cases.front().get() ).getBasicBlock() == b2 );
  ufAssert(
    dynamic_cast<const WIR_BasicBlockTreeNode &>(
      next( cases.begin() )->get() ).getBasicBlock() == b4 );
  ufAssert(
    dynamic_cast<const WIR_BasicBlockTreeNode &>(
      cases.back().get() ).getBasicBlock() == b5 );

  auto &ftCases = sw.getFallthroughCases();
  ufAssert( ftCases.size() == 1 );
  ufAssert(
    dynamic_cast<const WIR_BasicBlockTreeNode &>(
      ftCases.front()->get() ).getBasicBlock() == b2 );

  auto &reg0 = sw.getParent();
  ufAssert( reg0.getType() == WIR_CTNodeType::block );

  auto &c1 = b7.getContainers<WIR_ControlTree>().begin()->get();
  auto &reg1 = c1.getBasicBlockTreeNode().getParent();
  ufAssert( reg1.getType() == WIR_CTNodeType::block );

  auto &c2 = b6.getContainers<WIR_ControlTree>().begin()->get();
  auto &reg2 = c2.getBasicBlockTreeNode().getParent();
  ufAssert( reg2.getType() == WIR_CTNodeType::block );

  ufAssert( reg0 == reg1 );
  ufAssert( reg1 == reg2 );

  return( 0 );
}
