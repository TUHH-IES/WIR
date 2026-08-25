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

  WIR_TaskManager t;
  WIR_System s1( "genericmips.sys", t );
  WIR_CompilationUnit &c1 = s1.pushBackCompilationUnit( {} );
  WIR_CompilationUnit &c2 = s1.pushBackCompilationUnit( {} );

  c1.setName( "c1" );
  WIR_Function &f1 = c1.pushBackFunction( WIR_Function( "foo1" ) );
  WIR_Function &f2 = c1.pushBackFunction( WIR_Function( "bar1" ) );
  c2.setName( "c2" );
  WIR_Function &f3 = c2.pushBackFunction( WIR_Function( "foo2" ) );
  WIR_Function &f4 = c2.pushBackFunction( WIR_Function( "bar2" ) );

  WIR_BasicBlock &b1 = f1.pushBackBasicBlock( {} );
  b1.pushBackInstruction(
    { { MIPS::OpCode::JAL, MIPS::OperationFormat::L,
        WIR_LabelParameter( f4 ) } } );

  WIR_BasicBlock &b2 = f2.pushBackBasicBlock( {} );
  b2.pushBackInstruction(
    { { MIPS::OpCode::JAL, MIPS::OperationFormat::L,
        WIR_LabelParameter( f3 ) } } );

  WIR_BasicBlock &b3 = f3.pushBackBasicBlock( {} );
  b3.pushBackInstruction(
    { { MIPS::OpCode::JAL, MIPS::OperationFormat::L,
        WIR_LabelParameter( f1 ) } } );

  WIR_BasicBlock &b4 = f4.pushBackBasicBlock( {} );
  b4.pushBackInstruction(
    { { MIPS::OpCode::JAL, MIPS::OperationFormat::L,
        WIR_LabelParameter( f2 ) } } );

  // Copy system.
  WIR_System s2( s1 );

  WIR_CompilationUnit &c3 = s2.getCompilationUnits().front();
  WIR_CompilationUnit &c4 = s2.getCompilationUnits().back();
  c3.setName( "c3" );
  c4.setName( "c4" );

  WIR_Function &f5 = c3.getFunctions().front();
  WIR_Function &f6 = c3.getFunctions().back();
  WIR_Function &f7 = c4.getFunctions().front();
  WIR_Function &f8 = c4.getFunctions().back();

  f5.setName( "foo3" );
  f6.setName( "bar3" );
  f7.setName( "foo4" );
  f8.setName( "bar4" );

  // Make sure that the JALs in c3 refer to c4 and vice versa.
  auto &i5 = f5.getBasicBlocks().front().get().getInstructions().front().get();
  auto &i6 = f6.getBasicBlocks().front().get().getInstructions().front().get();
  auto &i7 = f7.getBasicBlocks().front().get().getInstructions().front().get();
  auto &i8 = f8.getBasicBlocks().front().get().getInstructions().front().get();
  auto &o5 = i5.getOperations().front().get();
  auto &o6 = i6.getOperations().front().get();
  auto &o7 = i7.getOperations().front().get();
  auto &o8 = i8.getOperations().front().get();
  auto &p5 =
    dynamic_cast<WIR_LabelParameter &>( o5.getParameters().front().get() );
  auto &p6 =
    dynamic_cast<WIR_LabelParameter &>( o6.getParameters().front().get() );
  auto &p7 =
    dynamic_cast<WIR_LabelParameter &>( o7.getParameters().front().get() );
  auto &p8 =
    dynamic_cast<WIR_LabelParameter &>( o8.getParameters().front().get() );

  ufAssert( p5.getFunction() == f8 );
  ufAssert( p6.getFunction() == f7 );
  ufAssert( p7.getFunction() == f5 );
  ufAssert( p8.getFunction() == f6 );

  return( 0 );
}
