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

  cout.iword( WIR_Indentation() ) = 8;
  cout << arm;

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
  const vector<ARMv4::AddressingMode> memoryAddressingModes {
    ARMv4::AddressingMode::pre,
    ARMv4::AddressingMode::post };
  const vector<ARMv4::AddressingMode> memoryAddSubModes {
    ARMv4::AddressingMode::plus,
    ARMv4::AddressingMode::minus };
  const vector<ARMv4::AddressingMode> rightShiftModes {
    ARMv4::AddressingMode::lsr,
    ARMv4::AddressingMode::asr };

  for ( auto &cond : conditions )
    for ( auto &addsub : memoryAddSubModes ) {
      WIR_Operation o1(
        ARMv5TE::OpCode::LDRD, ARMv5TE::OperationFormat::CPRAC8_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( p6, WIR_Usage::def ),
        WIR_RegisterParameter( r4, WIR_Usage::use ),
        WIR_AddressingModeParameter( addsub ),
        ARM_Const8_Unsigned( 255 ) );
      cout << o1 << endl;

      WIR_Operation o2(
        ARMv5TE::OpCode::LDRD, ARMv5TE::OperationFormat::CPRAR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( p6, WIR_Usage::def ),
        WIR_RegisterParameter( r4, WIR_Usage::use ),
        WIR_AddressingModeParameter( addsub ),
        WIR_RegisterParameter( r11, WIR_Usage::use ) );
      cout << o2 << endl;

      for ( auto &prepost : memoryAddressingModes ) {
        WIR_Operation o3(
          ARMv5TE::OpCode::LDRD, ARMv5TE::OperationFormat::CPARAC8_1,
          WIR_ConditionFieldParameter( cond ),
          WIR_RegisterParameter( p6, WIR_Usage::def ),
          WIR_AddressingModeParameter( prepost ),
          WIR_RegisterParameter( r4, WIR_Usage::defuse ),
          WIR_AddressingModeParameter( addsub ),
          ARM_Const8_Unsigned( 255 ) );
        cout << o3 << endl;

        WIR_Operation o4(
          ARMv5TE::OpCode::LDRD, ARMv5TE::OperationFormat::CPARAR_1,
          WIR_ConditionFieldParameter( cond ),
          WIR_RegisterParameter( p6, WIR_Usage::def ),
          WIR_AddressingModeParameter( prepost ),
          WIR_RegisterParameter( r4, WIR_Usage::defuse ),
          WIR_AddressingModeParameter( addsub ),
          WIR_RegisterParameter( r11, WIR_Usage::use ) );
        cout << o4 << endl;
      }
    }

  for ( auto &cond : conditions )
    for ( auto &proc : coprocessors ) {
      WIR_Operation o(
        ARMv5TE::OpCode::MCRR, ARMv5TE::OperationFormat::CAORRS_2,
        WIR_ConditionFieldParameter( cond ),
        WIR_AddressingModeParameter( proc ),
        ARM_Const3_CoprocessorOpcode( 0 ),
        WIR_RegisterParameter( r8, WIR_Usage::use ),
        WIR_RegisterParameter( r11, WIR_Usage::use ),
        WIR_StringParameter( "c9" ) );
      cout << o << endl;
    }

  for ( auto &cond : conditions )
    for ( auto &proc : coprocessors ) {
      WIR_Operation o(
        ARMv5TE::OpCode::MRRC, ARMv5TE::OperationFormat::CAORRS_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_AddressingModeParameter( proc ),
        ARM_Const3_CoprocessorOpcode( 0 ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r11, WIR_Usage::def ),
        WIR_StringParameter( "c9" ) );
      cout << o << endl;
    }

  for ( auto &addsub : memoryAddSubModes ) {
    WIR_Operation o1(
      ARMv5TE::OpCode::PLD, ARMv5TE::OperationFormat::RAC12_1,
      WIR_RegisterParameter( r6, WIR_Usage::use ),
      WIR_AddressingModeParameter( addsub ),
      ARM_Const12_Unsigned( 4095 ) );
    cout << o1 << endl;

    WIR_Operation o2(
      ARMv5TE::OpCode::PLD, ARMv5TE::OperationFormat::RAR_1,
      WIR_RegisterParameter( r6, WIR_Usage::use ),
      WIR_AddressingModeParameter( addsub ),
      WIR_RegisterParameter( r11, WIR_Usage::use ) );
    cout << o2 << endl;

    WIR_Operation o3(
      ARMv5TE::OpCode::PLD, ARMv5TE::OperationFormat::RARC5_1,
      WIR_RegisterParameter( r6, WIR_Usage::use ),
      WIR_AddressingModeParameter( addsub ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      ARM_Const5_Unsigned( 31 ) );
    cout << o3 << endl;

    for ( auto &rshift : rightShiftModes ) {
      WIR_Operation o(
        ARMv5TE::OpCode::PLD, ARMv5TE::OperationFormat::RARAC60_1,
        WIR_RegisterParameter( r6, WIR_Usage::use ),
        WIR_AddressingModeParameter( addsub ),
        WIR_RegisterParameter( r11, WIR_Usage::use ),
        WIR_AddressingModeParameter( rshift ),
        ARM_Const6_Unsigned0( 32 ) );
      cout << o << endl;
    }

    WIR_Operation o4(
      ARMv5TE::OpCode::PLD, ARMv5TE::OperationFormat::RARC50_1,
      WIR_RegisterParameter( r6, WIR_Usage::use ),
      WIR_AddressingModeParameter( addsub ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      ARM_Const5_Unsigned0( 31 ) );
    cout << o4 << endl;

    WIR_Operation o5(
      ARMv5TE::OpCode::PLD, ARMv5TE::OperationFormat::RAR_2,
      WIR_RegisterParameter( r6, WIR_Usage::use ),
      WIR_AddressingModeParameter( addsub ),
      WIR_RegisterParameter( r11, WIR_Usage::use ) );
    cout << o5 << endl;
  }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv5TE::OpCode::QADD,
                                                 ARMv5TE::OpCode::QDADD,
                                                 ARMv5TE::OpCode::QDSUB,
                                                 ARMv5TE::OpCode::QSUB } )
    for ( auto &cond : conditions ) {
      WIR_Operation o(
        opcode, ARMv5TE::OperationFormat::CRRR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r11, WIR_Usage::use ),
        WIR_RegisterParameter( r4, WIR_Usage::use ) );
      cout << o << endl;
    }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv5TE::OpCode::SMLABB,
                                                 ARMv5TE::OpCode::SMLABT,
                                                 ARMv5TE::OpCode::SMLATB,
                                                 ARMv5TE::OpCode::SMLATT,
                                                 ARMv5TE::OpCode::SMLALBB,
                                                 ARMv5TE::OpCode::SMLALBT,
                                                 ARMv5TE::OpCode::SMLALTB,
                                                 ARMv5TE::OpCode::SMLALTT,
                                                 ARMv5TE::OpCode::SMLAWB,
                                                 ARMv5TE::OpCode::SMLAWT } )
    for ( auto &cond : conditions ) {
      WIR_Operation o(
        opcode, ARMv5TE::OperationFormat::CRRRR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r11, WIR_Usage::use ),
        WIR_RegisterParameter( r6, WIR_Usage::use ),
        WIR_RegisterParameter( r4, WIR_Usage::use ) );
      cout << o << endl;
    }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv5TE::OpCode::SMULBB,
                                                 ARMv5TE::OpCode::SMULBT,
                                                 ARMv5TE::OpCode::SMULTB,
                                                 ARMv5TE::OpCode::SMULTT,
                                                 ARMv5TE::OpCode::SMULWB,
                                                 ARMv5TE::OpCode::SMULWT } )
    for ( auto &cond : conditions ) {
      WIR_Operation o(
        opcode, ARMv5TE::OperationFormat::CRRR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r11, WIR_Usage::use ),
        WIR_RegisterParameter( r4, WIR_Usage::use ) );
      cout << o << endl;
    }

  for ( auto &cond : conditions )
    for ( auto &addsub : memoryAddSubModes ) {
      WIR_Operation o1(
        ARMv5TE::OpCode::STRD, ARMv5TE::OperationFormat::CPRAC8_2,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( p6, WIR_Usage::use ),
        WIR_RegisterParameter( r4, WIR_Usage::use ),
        WIR_AddressingModeParameter( addsub ),
        ARM_Const8_Unsigned( 255 ) );
      cout << o1 << endl;

      WIR_Operation o2(
        ARMv5TE::OpCode::STRD, ARMv5TE::OperationFormat::CPRAR_2,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( p6, WIR_Usage::use ),
        WIR_RegisterParameter( r4, WIR_Usage::use ),
        WIR_AddressingModeParameter( addsub ),
        WIR_RegisterParameter( r11, WIR_Usage::use ) );
      cout << o2 << endl;

      for ( auto &prepost : memoryAddressingModes ) {
        WIR_Operation o3(
          ARMv5TE::OpCode::STRD, ARMv5TE::OperationFormat::CPARAC8_2,
          WIR_ConditionFieldParameter( cond ),
          WIR_RegisterParameter( p6, WIR_Usage::use ),
          WIR_AddressingModeParameter( prepost ),
          WIR_RegisterParameter( r4, WIR_Usage::defuse ),
          WIR_AddressingModeParameter( addsub ),
          ARM_Const8_Unsigned( 255 ) );
        cout << o3 << endl;

        WIR_Operation o4(
          ARMv5TE::OpCode::STRD, ARMv5TE::OperationFormat::CPARAR_2,
          WIR_ConditionFieldParameter( cond ),
          WIR_RegisterParameter( p6, WIR_Usage::use ),
          WIR_AddressingModeParameter( prepost ),
          WIR_RegisterParameter( r4, WIR_Usage::defuse ),
          WIR_AddressingModeParameter( addsub ),
          WIR_RegisterParameter( r11, WIR_Usage::use ) );
        cout << o4 << endl;
      }
    }

  return( 0 );
}
