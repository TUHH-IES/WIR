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
#include <set>

// Include libuseful headers
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/arm/armv4t.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  WIR_TaskManager t;
  WIR_System sys( "lpc2880.sys", t );
  auto &p = sys.getComponents<ARMv4T>().begin()->get();

  WIR_CompilationUnit c1;
  WIR_Function f( "foo" );

  WIR_BasicBlock &bb1 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &bb3 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &bb2 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &bb5 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &bb4 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &bb6 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &bb7 = f.pushBackBasicBlock( {} );

  // Create a CFG.
  bb1.pushBackInstruction(
    { { ARMv4::OpCode::ADD, ARMv4T::OperationFormat::TRRR_1,
        WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
        WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        WIR_RegisterParameter( p.R3(), WIR_Usage::use ) } } );
  bb1.pushBackInstruction(
    { { ARMv4T::OpCode::BX, ARMv4T::OperationFormat::CR_3,
        WIR_ConditionFieldParameter( ARMv4::Condition::pl ),
        WIR_RegisterParameter( p.R4(), WIR_Usage::use ) } } );

  bb3.pushBackInstruction(
    { { ARMv4::OpCode::ADD, ARMv4T::OperationFormat::TRPCC10_1,
        WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
        WIR_RegisterParameter( p.PC(), WIR_Usage::use ),
        ARM_Const10_Unsigned4( 1020 ) } } );
  bb3.pushBackInstruction(
    { { ARMv4T::OpCode::POP, ARMv4T::OperationFormat::TR7PC,
        WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
        WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
        WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
        WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
        WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
        WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
        WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
        WIR_RegisterParameter( p.PC(), WIR_Usage::def ) } } );

  bb2.pushBackInstruction(
    { { ARMv4T::OpCode::PUSH, ARMv4T::OperationFormat::TR3LR,
        WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
        WIR_RegisterParameter( p.LR(), WIR_Usage::use ) } });
  bb2.pushBackInstruction(
    { { ARMv4::OpCode::MUL, ARMv4::OperationFormat::CRRR_1,
        WIR_ConditionFieldParameter( ARMv4::Condition::gt ),
        WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
        WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        WIR_RegisterParameter( p.R4(), WIR_Usage::use ) } } );
  bb2.pushBackInstruction(
    { { ARMv4::OpCode::B, ARMv4::OperationFormat::CL,
        WIR_ConditionFieldParameter( ARMv4::Condition::vs ),
        WIR_LabelParameter( bb6 ) } } );

  bb5.pushBackInstruction(
    { { ARMv4::OpCode::MRS, ARMv4::OperationFormat::CR_2,
        WIR_ConditionFieldParameter( ARMv4::Condition::eq ),
        WIR_RegisterParameter( p.R1(), WIR_Usage::def ) } } );
  bb5.pushBackInstruction(
    { { ARMv4::OpCode::MSR, ARMv4::OperationFormat::CAAAAC8RA_1,
        WIR_ConditionFieldParameter( ARMv4::Condition::al ),
        WIR_AddressingModeParameter( ARMv4::AddressingMode::f ),
        WIR_AddressingModeParameter( ARMv4::AddressingMode::x ),
        WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
        WIR_AddressingModeParameter( ARMv4::AddressingMode::s ),
        ARM_Const8_Unsigned( 42 ),
        ARM_Const5_RotateAmount( 6 ) } } );
  bb5.pushBackInstruction(
    { { ARMv4::OpCode::BL, ARMv4::OperationFormat::CL,
        WIR_ConditionFieldParameter( ARMv4::Condition::hi ),
        WIR_LabelParameter( f ) } } );

  bb4.pushBackInstruction(
    { { ARMv4::OpCode::SWI, ARMv4::OperationFormat::CC24,
        WIR_ConditionFieldParameter( ARMv4::Condition::ls ),
        ARM_Const24_Unsigned( 42 ) } } );
  bb4.pushBackInstruction(
    { { ARMv4::OpCode::SWP, ARMv4::OperationFormat::CRRR_5,
        WIR_ConditionFieldParameter( ARMv4::Condition::le ),
        WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
        WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        WIR_RegisterParameter( p.R4(), WIR_Usage::use ) } } );

  bb6.pushBackInstruction(
    { { ARMv4::OpCode::SUB, ARMv4T::OperationFormat::TRRR_1,
        WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
        WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        WIR_RegisterParameter( p.R3(), WIR_Usage::use ) } } );

  bb7.pushBackInstruction(
    { { ARMv4T::OpCode::STR, ARMv4T::OperationFormat::TRRC7_2,
        WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        ARM_Const7_Unsigned( 16 ) } } );

  c1.pushBackFunction( std::move( f ) );
  c1.pushBackFunction( WIR_Function( "bar" ) );

  WIR_CompilationUnit &c = sys.pushBackCompilationUnit( c1 );
  WIR_Function &f1 = c.getFunctions().front().get();
  auto it = f1.getBasicBlocks().begin();
  ++it;
  ++it;
  WIR_BasicBlock &b3 = it->get();

  auto &d3 = c.pushBackData( WIR_Data( "d3" ) );
  auto &d2 = c.pushFrontData( WIR_Data( "d2" ) );
  auto &d1 = c.pushFrontData( WIR_Data( "d1" ) );
  auto &d4 = c.pushBackData( WIR_Data( "d4" ) );

  d1.setSize( 5 );
  d2.setSize( 13 );
  d3.setSize( 42 );
  d4.setSize( 1000 );

  // Move some items to different sections.
  sys.findSymbol( d2 ).setConst();                  // -> .rodata
  d4.pushBackInitData( WIR_DataInit( 42 ) );        // -> .data
  sys.findSymbol( b3 ).setSection( *( p.findSection( ".text_spm" ) ) );
  sys.findSymbol( f1 ).setGlobal();

  // Dump the entire code for arm-elf-as.
  cout << arm << sys;

  return( 0 );
}
