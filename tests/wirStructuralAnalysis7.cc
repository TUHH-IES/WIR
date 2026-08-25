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

  // Check cyclic self-loop regions with structural CFG analysis.

  WIR_Function f( "foo" );
  auto &r1 = f.pushBackVirtualRegister( MIPS_RegV() );
  auto &r2 = f.pushBackVirtualRegister( MIPS_RegV() );

  // For this test CFG, we need 3 basic blocks.
  WIR_BasicBlock &b1 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b2 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b3 = f.pushBackBasicBlock( {} );

  b1.pushBackInstruction(
    { { MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI,
        WIR_RegisterParameter( r1, WIR_Usage::def ),
        WIR_RegisterParameter( r2, WIR_Usage::use ),
        MIPS_Immediate16_Signed( 1 ) } } );

  b2.pushBackInstruction(
    { { MIPS::OpCode::LW, MIPS::OperationFormat::RIR_1,
        WIR_RegisterParameter( r1, WIR_Usage::def ),
        MIPS_Immediate16_Signed( -138 ),
        WIR_RegisterParameter( r2, WIR_Usage::use ) } } );
  b2.pushBackInstruction(
    { { MIPS::OpCode::BEQ, MIPS::OperationFormat::RRL,
        WIR_RegisterParameter( r1, WIR_Usage::use ),
        WIR_RegisterParameter( r2, WIR_Usage::use ),
        WIR_LabelParameter( b2 ) } } );

  b3.pushBackInstruction(
    { { MIPS::OpCode::JR, MIPS::OperationFormat::R_2,
        WIR_RegisterParameter( r1, WIR_Usage::use ) } } );

  // Do structural analysis of f.
  WIR_StructuralAnalysis a { f };
  a.analyze();

  // Check resulting control tree.
  auto &c = b2.getContainers<WIR_ControlTree>().begin()->get();
  auto &leaf = c.getBasicBlockTreeNode();
  auto &selfloop = leaf.getParent();

  ufAssert( selfloop.getType() == WIR_CTNodeType::selfloop );
  ufAssert( selfloop.isCyclic() && !selfloop.isAcyclic() );
  auto &sl = dynamic_cast<WIR_SelfLoopTreeNode &>( selfloop );

  ufAssert(
    dynamic_cast<const WIR_BasicBlockTreeNode &>( sl.getEntry() ).getBasicBlock() == b2 );

  auto &root = leaf.getRoot();
  ufAssert( root.getType() == WIR_CTNodeType::block );
  auto &b = dynamic_cast<WIR_BlockTreeNode &>( root );

  auto it = b.getBlockList().begin();
  ufAssert(
    dynamic_cast<WIR_BasicBlockTreeNode &>( it->get() ).getBasicBlock() == b1 );
  ++it;
  ufAssert( it->get() == sl );
  ++it;
  ufAssert(
    dynamic_cast<WIR_BasicBlockTreeNode &>( it->get() ).getBasicBlock() == b3 );

  return( 0 );
}
