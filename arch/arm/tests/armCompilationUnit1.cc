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
#include <vector>

// Include WIR headers
#include <wir/wir.h>
#include <arch/arm/armv5te.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  ARMv5TE p;
  const ARM_LoRegP &r4 = p.R4(), &r6 = p.R6();
  const ARM_HiRegP &r8 = p.R8(), &r11 = p.R11();
  const ARMv5TE_PRegP &p6 = p.P6();
  WIR_CompilationUnit c;
  WIR_Function f( "main" );
  WIR_Function f1( "foo" );

  // The following operations must be accepted for the ARMv5TE ISA.
  const vector<ARMv4::Condition> conditions {
    ARMv4::Condition::eq,
    ARMv4::Condition::ne,
    ARMv4::Condition::hs,
    ARMv4::Condition::lo,
    ARMv4::Condition::mi,
    ARMv4::Condition::pl,
    ARMv4::Condition::vs,
    ARMv4::Condition::vc,
    ARMv4::Condition::hi,
    ARMv4::Condition::ls,
    ARMv4::Condition::ge,
    ARMv4::Condition::lt,
    ARMv4::Condition::gt,
    ARMv4::Condition::le,
    ARMv4::Condition::al };
  const vector<ARMv4::AddressingMode> coprocessors {
    ARMv4::AddressingMode::p0,
    ARMv4::AddressingMode::p1,
    ARMv4::AddressingMode::p2,
    ARMv4::AddressingMode::p3,
    ARMv4::AddressingMode::p4,
    ARMv4::AddressingMode::p5,
    ARMv4::AddressingMode::p6,
    ARMv4::AddressingMode::p7,
    ARMv4::AddressingMode::p8,
    ARMv4::AddressingMode::p9,
    ARMv4::AddressingMode::p10,
    ARMv4::AddressingMode::p11,
    ARMv4::AddressingMode::p12,
    ARMv4::AddressingMode::p13,
    ARMv4::AddressingMode::p14,
    ARMv4::AddressingMode::p15 };
  const vector<ARMv4::AddressingMode> memoryAddSubModes {
    ARMv4::AddressingMode::plus,
    ARMv4::AddressingMode::minus };
  const vector<ARMv4::AddressingMode> rightShiftModes {
    ARMv4::AddressingMode::lsr,
    ARMv4::AddressingMode::asr };

  auto &b1 = f.pushBackBasicBlock( {} );
  for ( auto &cond : conditions )
    for ( auto &addsub : memoryAddSubModes )
      b1.pushBackInstruction(
        { { ARMv5TE::OpCode::LDRD, ARMv5TE::OperationFormat::CPRAC8_1,
            WIR_ConditionFieldParameter( cond ),
            WIR_RegisterParameter( p6, WIR_Usage::def ),
            WIR_RegisterParameter( r4, WIR_Usage::use ),
            WIR_AddressingModeParameter( addsub ),
            ARM_Const8_Unsigned( 255 ) } } );

  auto &b2 = f.pushBackBasicBlock( {} );
  for ( auto &cond : conditions )
    for ( auto &proc : coprocessors )
      b2.pushBackInstruction(
        { { ARMv5TE::OpCode::MCRR, ARMv5TE::OperationFormat::CAORRS_2,
            WIR_ConditionFieldParameter( cond ),
            WIR_AddressingModeParameter( proc ),
            ARM_Const3_CoprocessorOpcode( 0 ),
            WIR_RegisterParameter( r8, WIR_Usage::use ),
            WIR_RegisterParameter( r11, WIR_Usage::use ),
            WIR_StringParameter( "c9" ) } } );

  auto &b3 = f1.pushBackBasicBlock( {} );
  for ( auto &cond : conditions )
    for ( auto &proc : coprocessors )
      b3.pushBackInstruction(
        { { ARMv5TE::OpCode::MRRC, ARMv5TE::OperationFormat::CAORRS_1,
            WIR_ConditionFieldParameter( cond ),
            WIR_AddressingModeParameter( proc ),
            ARM_Const3_CoprocessorOpcode( 0 ),
            WIR_RegisterParameter( r8, WIR_Usage::def ),
            WIR_RegisterParameter( r11, WIR_Usage::def ),
            WIR_StringParameter( "c9" ) } } );

  auto &b4 = f1.pushBackBasicBlock( {} );
  for ( auto &addsub : memoryAddSubModes ) {
    b4.pushBackInstruction(
      { { ARMv5TE::OpCode::PLD, ARMv5TE::OperationFormat::RAC12_1,
          WIR_RegisterParameter( r6, WIR_Usage::use ),
          WIR_AddressingModeParameter( addsub ),
          ARM_Const12_Unsigned( 4095 ) } } );

    b4.pushBackInstruction(
      { { ARMv5TE::OpCode::PLD, ARMv5TE::OperationFormat::RAR_1,
          WIR_RegisterParameter( r6, WIR_Usage::use ),
          WIR_AddressingModeParameter( addsub ),
          WIR_RegisterParameter( r11, WIR_Usage::use ) } } );

    b4.pushBackInstruction(
      { { ARMv5TE::OpCode::PLD, ARMv5TE::OperationFormat::RARC5_1,
          WIR_RegisterParameter( r6, WIR_Usage::use ),
          WIR_AddressingModeParameter( addsub ),
          WIR_RegisterParameter( r11, WIR_Usage::use ),
          ARM_Const5_Unsigned( 31 ) } } );

    for ( auto &rshift : rightShiftModes )
      b4.pushBackInstruction(
        { { ARMv5TE::OpCode::PLD, ARMv5TE::OperationFormat::RARAC60_1,
            WIR_RegisterParameter( r6, WIR_Usage::use ),
            WIR_AddressingModeParameter( addsub ),
            WIR_RegisterParameter( r11, WIR_Usage::use ),
            WIR_AddressingModeParameter( rshift ),
            ARM_Const6_Unsigned0( 32 ) } } );

    b4.pushBackInstruction(
      { { ARMv5TE::OpCode::PLD, ARMv5TE::OperationFormat::RARC50_1,
          WIR_RegisterParameter( r6, WIR_Usage::use ),
          WIR_AddressingModeParameter( addsub ),
          WIR_RegisterParameter( r11, WIR_Usage::use ),
          ARM_Const5_Unsigned0( 31 ) } } );

    b4.pushBackInstruction(
      { { ARMv5TE::OpCode::PLD, ARMv5TE::OperationFormat::RAR_2,
          WIR_RegisterParameter( r6, WIR_Usage::use ),
          WIR_AddressingModeParameter( addsub ),
          WIR_RegisterParameter( r11, WIR_Usage::use ) } } );
  }

  f.insertContainer( WIR_Comment( "Comment 1" ) );
  f.insertContainer( WIR_FileInfo( "test.c", 42 ) );
  f1.insertContainer( WIR_Comment( "Comment 2" ) );
  f1.insertContainer( WIR_FileInfo( "test.c", 333 ) );

  c.pushBackFunction( f );
  c.pushBackFunction( f1 );
  c.setName( "test.c" );

  c.insertContainer( WIR_Comment( "Comment 3" ) );
  c.insertContainer( WIR_FileInfo( "test.c", 1 ) );

  cout << arm << comment << fileinfo << c;

  return( 0 );
}
