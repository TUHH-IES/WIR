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

  // Check regular for/while-do loops with structural CFG analysis.
  // Cf. S. S. Muchnick, Fig 7.36, page 203.

  WIR_Function f( "foo" );
  auto &r1 = f.pushBackVirtualRegister( MIPS_RegV() );
  auto &r2 = f.pushBackVirtualRegister( MIPS_RegV() );

  // For this test CFG, we need 8 basic blocks.
  WIR_BasicBlock &b1 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b2 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b3 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b4 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b5 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b6 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b7 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b8 = f.pushBackBasicBlock( {} );

  b1.pushBackInstruction(
    { { MIPS::OpCode::LW, MIPS::OperationFormat::RIR_1,
        WIR_RegisterParameter( r1, WIR_Usage::def ),
        MIPS_Immediate16_Signed( -138 ),
        WIR_RegisterParameter( r2, WIR_Usage::use ) } } );

  b2.pushBackInstruction(
    { { MIPS::OpCode::JR, MIPS::OperationFormat::R_2,
        WIR_RegisterParameter( r1, WIR_Usage::use ) } } );
  b2.rbegin()->get().begin()->get().addJumpTarget( b3 );
  b2.rbegin()->get().begin()->get().addJumpTarget( b6 );

  b3.pushBackInstruction(
    { { MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI,
        WIR_RegisterParameter( r1, WIR_Usage::def ),
        WIR_RegisterParameter( r2, WIR_Usage::use ),
        MIPS_Immediate16_Signed( 0 ) } } );

  b4.pushBackInstruction(
    { { MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI,
        WIR_RegisterParameter( r1, WIR_Usage::def ),
        WIR_RegisterParameter( r2, WIR_Usage::use ),
        MIPS_Immediate16_Signed( 0 ) } } );

  b5.pushBackInstruction(
    { { MIPS::OpCode::JR, MIPS::OperationFormat::R_2,
        WIR_RegisterParameter( r1, WIR_Usage::use ) } } );
  b5.rbegin()->get().begin()->get().addJumpTarget( b2 );

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

  b8.pushBackInstruction(
    { { MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI,
        WIR_RegisterParameter( r1, WIR_Usage::def ),
        WIR_RegisterParameter( r2, WIR_Usage::use ),
        MIPS_Immediate16_Signed( 0 ) } } );

  // Do structural analysis of f.
  WIR_StructuralAnalysis a { f };
  a.analyze();

  // Check resulting control tree.
  auto &c = b2.getContainers<WIR_ControlTree>().begin()->get();
  auto &leaf = c.getBasicBlockTreeNode();
  auto &whileloop = leaf.getParent();

  ufAssert( whileloop.getType() == WIR_CTNodeType::whileloop );
  ufAssert( whileloop.isCyclic() && !whileloop.isAcyclic() );
  auto &wl = dynamic_cast<WIR_WhileLoopTreeNode &>( whileloop );
  ufAssert(
    dynamic_cast<const WIR_BasicBlockTreeNode &>(
      whileloop.getEntry() ).getBasicBlock() == b2 );

  set<WIR_id_t> bbIDs;
  bbIDs.insert( b1.getID() );
  bbIDs.insert( b2.getID() );
  bbIDs.insert( b3.getID() );
  bbIDs.insert( b4.getID() );
  bbIDs.insert( b5.getID() );
  bbIDs.insert( b6.getID() );
  bbIDs.insert( b7.getID() );
  bbIDs.insert( b8.getID() );

  ufAssert( wl.getNodes().size() == 2 );
  for ( WIR_BasicBlock &b : wl.getBasicBlocks() )
    bbIDs.erase( b.getID() );

  ufAssert( bbIDs.size() == 4 );
  ufAssert( bbIDs.count( b1.getID() ) );
  ufAssert( bbIDs.count( b6.getID() ) );
  ufAssert( bbIDs.count( b7.getID() ) );
  ufAssert( bbIDs.count( b8.getID() ) );

  return( 0 );
}
