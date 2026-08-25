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
#include <arch/arm/armv4.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  ARMv4 p;
  const ARM_LoRegP &r1 = p.R2(), &r2 = p.R6();
  const ARM_HiRegP &r3 = p.R11(), &r4 = p.R9();
  WIR_BasicBlock b;
  WIR_Function f( "main" );

  cout.iword( WIR_Indentation() ) = 8;
  cout << arm;

  // The following operations must be accepted for any ARM-based architecture.
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

  const vector<ARMv4::OperationFormat> f11 {
    ARMv4::OperationFormat::CRRC8RA_1,
    ARMv4::OperationFormat::CRRC8RA_2 };
  const vector<ARMv4::OperationFormat> f12 {
    ARMv4::OperationFormat::CRRR_1, ARMv4::OperationFormat::CRRR_2,
    ARMv4::OperationFormat::CRRR_3, ARMv4::OperationFormat::CRRR_4 };
  const vector<ARMv4::OperationFormat> f13 {
    ARMv4::OperationFormat::CRRRAR_1, ARMv4::OperationFormat::CRRRAR_2 };
  const vector<ARMv4::OperationFormat> f14 {
    ARMv4::OperationFormat::CRRRC5_1, ARMv4::OperationFormat::CRRRC5_2 };
  const vector<ARMv4::OperationFormat> f15 {
    ARMv4::OperationFormat::CRRRAC60_1,
    ARMv4::OperationFormat::CRRRAC60_2 };
  const vector<ARMv4::OperationFormat> f16 {
    ARMv4::OperationFormat::CRRRC50_1,
    ARMv4::OperationFormat::CRRRC50_2 };

  const vector<ARMv4::AddressingMode> amode1 {
    ARMv4::AddressingMode::lsl,
    ARMv4::AddressingMode::lsr,
    ARMv4::AddressingMode::asr,
    ARMv4::AddressingMode::ror };
  const vector<ARMv4::AddressingMode> amode2 {
    ARMv4::AddressingMode::lsr,
    ARMv4::AddressingMode::asr };
  const vector<ARMv4::AddressingMode> memoryAddSubModes {
    ARMv4::AddressingMode::plus,
    ARMv4::AddressingMode::minus };
  const vector<ARMv4::AddressingMode> memoryAddressingModes {
    ARMv4::AddressingMode::pre,
    ARMv4::AddressingMode::post };
  const vector<ARMv4::AddressingMode> rightShiftModes {
    ARMv4::AddressingMode::lsr,
    ARMv4::AddressingMode::asr };
  const vector<ARMv4::AddressingMode> multipleModes {
    ARMv4::AddressingMode::ia,
    ARMv4::AddressingMode::ib,
    ARMv4::AddressingMode::da,
    ARMv4::AddressingMode::db,
    ARMv4::AddressingMode::fd,
    ARMv4::AddressingMode::ed,
    ARMv4::AddressingMode::fa,
    ARMv4::AddressingMode::ea };
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

  // Standard data-processing operations.
  const vector<ARMv4::OpCode> dataProcessingOps1 {
    ARMv4::OpCode::ADC,
    ARMv4::OpCode::ADD,
    ARMv4::OpCode::AND,
    ARMv4::OpCode::BIC,
    ARMv4::OpCode::EOR,
    ARMv4::OpCode::ORR,
    ARMv4::OpCode::RSB,
    ARMv4::OpCode::RSC,
    ARMv4::OpCode::SBC,
    ARMv4::OpCode::SUB };

  for ( auto &opcode : dataProcessingOps1 )
    for ( auto &cond : conditions ) {
      for ( auto &format : f11 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new ARM_Const8_Unsigned( 42 ),
          new ARM_Const5_RotateAmount( 6 ) );
        cout << o << endl;
      }

      for ( auto &format : f12 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );
        cout << o << endl;
      }

      for ( auto &format : f13 )
        for ( auto &amode : amode1 ) {
          WIR_Operation o(
            opcode, format, WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_RegisterParameter( r2, WIR_Usage::use ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new WIR_AddressingModeParameter( amode ),
            new WIR_RegisterParameter( r4, WIR_Usage::use ) );
          cout << o << endl;
        }

      for ( auto &format : f14 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned( 0 ) );
        cout << o << endl;
      }

      for ( auto &format : f15 )
        for ( auto &amode : amode2 ) {
          WIR_Operation o(
            opcode, format, WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_RegisterParameter( r2, WIR_Usage::use ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new WIR_AddressingModeParameter( amode ),
            new ARM_Const6_Unsigned0( 1 ) );
          cout << o << endl;
        }

      for ( auto &format : f16 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned0( 1 ) );
        cout << o << endl;
      }
    }

  const vector<ARMv4::OperationFormat> f21 {
    ARMv4::OperationFormat::CRC8RA_1, ARMv4::OperationFormat::CRC8RA_2 };
  const vector<ARMv4::OperationFormat> f22 {
    ARMv4::OperationFormat::CRR_1, ARMv4::OperationFormat::CRR_2,
    ARMv4::OperationFormat::CRR_3, ARMv4::OperationFormat::CRR_4 };
  const vector<ARMv4::OperationFormat> f23 {
    ARMv4::OperationFormat::CRRAR_1, ARMv4::OperationFormat::CRRAR_2 };
  const vector<ARMv4::OperationFormat> f24 {
    ARMv4::OperationFormat::CRRC5_1, ARMv4::OperationFormat::CRRC5_2 };
  const vector<ARMv4::OperationFormat> f25 {
    ARMv4::OperationFormat::CRRAC60_1,
    ARMv4::OperationFormat::CRRAC60_2 };
  const vector<ARMv4::OperationFormat> f26 {
    ARMv4::OperationFormat::CRRC50_1, ARMv4::OperationFormat::CRRC50_2 };

  const vector<ARMv4::OpCode> dataProcessingOps2 {
    ARMv4::OpCode::MOV,
    ARMv4::OpCode::MVN };

  for ( auto &opcode : dataProcessingOps2 )
    for ( auto &cond : conditions ) {
      for ( auto &format : f21 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new ARM_Const8_Unsigned( 42 ),
          new ARM_Const5_RotateAmount( 6 ) );
        cout << o << endl;
      }

      for ( auto &format : f22 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );
        cout << o << endl;
      }

      for ( auto &format : f23 )
        for ( auto &amode : amode1 ) {
          WIR_Operation o(
            opcode, format, WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new WIR_AddressingModeParameter( amode ),
            new WIR_RegisterParameter( r4, WIR_Usage::use ) );
          cout << o << endl;
        }

      for ( auto &format : f24 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned( 0 ) );
        cout << o << endl;
      }

      for ( auto &format : f25 )
        for ( auto &amode : amode2 ) {
          WIR_Operation o(
            opcode, format, WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new WIR_AddressingModeParameter( amode ),
            ARM_Const6_Unsigned0( 1 ) );
          cout << o << endl;
        }

      for ( auto &format : f26 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned0( 1 ) );
        cout << o << endl;
      }
    }

  const vector<ARMv4::OperationFormat> f32 {
    ARMv4::OperationFormat::CRR_5, ARMv4::OperationFormat::CRR_6 };

  const vector<ARMv4::OpCode> dataProcessingOps3 {
    ARMv4::OpCode::CMN,
    ARMv4::OpCode::CMP,
    ARMv4::OpCode::TEQ,
    ARMv4::OpCode::TST };

  for ( auto &opcode : dataProcessingOps3 )
    for ( auto &cond : conditions ) {
      WIR_Operation o1(
        opcode, ARMv4::OperationFormat::CRC8RA_3,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_RegisterParameter( r2, WIR_Usage::use ),
        new ARM_Const8_Unsigned( 42 ),
        new ARM_Const5_RotateAmount( 6 ) );
      cout << o1 << endl;

      for ( auto &format : f32 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );
        cout << o << endl;
      }

      for ( auto amode : amode1 ) {
        WIR_Operation o(
          opcode, ARMv4::OperationFormat::CRRAR_5,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new WIR_AddressingModeParameter( amode ),
          new WIR_RegisterParameter( r4, WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o2(
        opcode, ARMv4::OperationFormat::CRRC5_3,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_RegisterParameter( r2, WIR_Usage::use ),
        new WIR_RegisterParameter( r3, WIR_Usage::use ),
        new ARM_Const5_Unsigned( 0 ) );
      cout << o2 << endl;

      for ( auto amode : amode2 ) {
        WIR_Operation o(
          opcode, ARMv4::OperationFormat::CRRAC60_3,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new WIR_AddressingModeParameter( amode ),
          new ARM_Const6_Unsigned0( 1 ) );
        cout << o << endl;
      }

      WIR_Operation o3(
        opcode, ARMv4::OperationFormat::CRRC50_3,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_RegisterParameter( r2, WIR_Usage::use ),
        new WIR_RegisterParameter( r3, WIR_Usage::use ),
        new ARM_Const5_Unsigned0( 1 ) );
      cout << o3 << endl;
    }

  const vector<ARMv4::OperationFormat> f41 {
    ARMv4::OperationFormat::CRRRR_1, ARMv4::OperationFormat::CRRRR_2 };

  for ( auto &cond : conditions ) {
    for ( auto &format : f41 ) {
      WIR_Operation o(
        ARMv4::OpCode::MLA, format, WIR_ConditionFieldParameter( cond ),
        new WIR_RegisterParameter( r1, WIR_Usage::def ),
        new WIR_RegisterParameter( r2, WIR_Usage::use ),
        new WIR_RegisterParameter( r3, WIR_Usage::use ),
        new WIR_RegisterParameter( r4, WIR_Usage::use ) );
      cout << o << endl;
    }
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o1(
      ARMv4::OpCode::MUL, ARMv4::OperationFormat::CRRR_1,
      new WIR_ConditionFieldParameter( cond ),
      new WIR_RegisterParameter( r1, WIR_Usage::def ),
      new WIR_RegisterParameter( r2, WIR_Usage::use ),
      new WIR_RegisterParameter( r4, WIR_Usage::use ) );
    cout << o1 << endl;

    WIR_Operation o2(
      ARMv4::OpCode::MUL, ARMv4::OperationFormat::CRRR_2,
      new WIR_ConditionFieldParameter( cond ),
      new WIR_RegisterParameter( r1, WIR_Usage::def ),
      new WIR_RegisterParameter( r2, WIR_Usage::use ),
      new WIR_RegisterParameter( r4, WIR_Usage::use ) );
    cout << o2 << endl;
  }

  const vector<ARMv4::OperationFormat> f51 {
    ARMv4::OperationFormat::CRRRR_3, ARMv4::OperationFormat::CRRRR_4 };

  const vector<ARMv4::OpCode> dataProcessingOps5 {
    ARMv4::OpCode::SMLAL,
    ARMv4::OpCode::SMULL,
    ARMv4::OpCode::UMLAL,
    ARMv4::OpCode::UMULL };

  for ( auto &opcode : dataProcessingOps5 )
    for ( auto &cond : conditions ) {
      for ( auto &format : f51 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new WIR_RegisterParameter( r4, WIR_Usage::use ) );
        cout << o << endl;
      }
    }

  // Load operations
  const vector<ARMv4::OpCode> ldOps1 {
    ARMv4::OpCode::LDR,
    ARMv4::OpCode::LDRB };

  for ( auto &opcode : ldOps1 )
    for ( auto &cond : conditions )
      for ( auto &addsub : memoryAddSubModes ) {
        for ( auto &prepost : memoryAddressingModes ) {
          WIR_Operation o1(
            opcode, ARMv4::OperationFormat::CRARAC12_1,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new ARM_Const12_Unsigned( 4095 ) );
          cout << o1 << endl;

          WIR_Operation o2(
            opcode, ARMv4::OperationFormat::CRARAR_1,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ) );
          cout << o2 << endl;
        }

        WIR_Operation o1(
          opcode, ARMv4::OperationFormat::CRRAC12_1,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new ARM_Const12_Unsigned( 4095 ) );
        cout << o1 << endl;

        WIR_Operation o2(
          opcode, ARMv4::OperationFormat::CRRAR_3,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );
        cout << o2 << endl;

        WIR_Operation o3(
          opcode, ARMv4::OperationFormat::CRRARC5_1,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned( 31 ) );
        cout << o3 << endl;

        for ( auto &rshift : rightShiftModes ) {
          WIR_Operation o4(
            opcode, ARMv4::OperationFormat::CRRARAC60_1,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_RegisterParameter( r2, WIR_Usage::use ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new WIR_AddressingModeParameter( rshift ),
            new ARM_Const6_Unsigned0( 32 ) );
          cout << o4 << endl;
        }

        WIR_Operation o4(
          opcode, ARMv4::OperationFormat::CRRARC50_1,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned0( 31 ) );
        cout << o4 << endl;

        WIR_Operation o5(
          opcode, ARMv4::OperationFormat::CRRAR_4,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );
        cout << o5 << endl;

        for ( auto &prepost : memoryAddressingModes ) {
          WIR_Operation o6(
            opcode, ARMv4::OperationFormat::CRARARC5_1,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new ARM_Const5_Unsigned( 31 ) );
          cout << o6 << endl;

          for ( auto &rshift : rightShiftModes ) {
            WIR_Operation o7(
              opcode, ARMv4::OperationFormat::CRARARAC60_1,
              new WIR_ConditionFieldParameter( cond ),
              new WIR_RegisterParameter( r1, WIR_Usage::def ),
              new WIR_AddressingModeParameter( prepost ),
              new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
              new WIR_AddressingModeParameter( addsub ),
              new WIR_RegisterParameter( r3, WIR_Usage::use ),
              new WIR_AddressingModeParameter( rshift ),
              new ARM_Const6_Unsigned0( 32 ) );
            cout << o7 << endl;
          }

          WIR_Operation o8(
            opcode, ARMv4::OperationFormat::CRARARC50_1,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new ARM_Const5_Unsigned0( 31 ) );
          cout << o8 << endl;

          WIR_Operation o9(
            opcode, ARMv4::OperationFormat::CRARAR_2,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ) );
          cout << o9 << endl;
        }
      }

  const vector<ARMv4::OpCode> ldOps2 {
    ARMv4::OpCode::LDRBT,
    ARMv4::OpCode::LDRT };

  for ( auto &opcode : ldOps2 )
    for ( auto &cond : conditions )
      for ( auto &addsub : memoryAddSubModes ) {
        WIR_Operation o1(
          opcode, ARMv4::OperationFormat::CRARAC12_1,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new ARM_Const12_Unsigned( 4095 ) );
        cout << o1 << endl;

        WIR_Operation o2(
          opcode, ARMv4::OperationFormat::CRARAR_1,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );
        cout << o2 << endl;

        WIR_Operation o6(
          opcode, ARMv4::OperationFormat::CRARARC5_1,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned( 31 ) );
        cout << o6 << endl;

        for ( auto rshift : rightShiftModes ) {
          WIR_Operation o7(
            opcode, ARMv4::OperationFormat::CRARARAC60_1,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new WIR_AddressingModeParameter( rshift ),
            new ARM_Const6_Unsigned0( 32 ) );
          cout << o7 << endl;
        }

        WIR_Operation o8(
          opcode, ARMv4::OperationFormat::CRARARC50_1,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned0( 31 ) );
        cout << o8 << endl;

        WIR_Operation o9(
          opcode, ARMv4::OperationFormat::CRARAR_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );
        cout << o9 << endl;
      }

  for ( auto &opcode : vector<ARMv4::OpCode> { ARMv4::OpCode::LDRH,
                                                  ARMv4::OpCode::LDRSB,
                                                  ARMv4::OpCode::LDRSH } )
    for ( auto &cond : conditions )
      for ( auto &addsub : memoryAddSubModes ) {
        WIR_Operation o1(
          opcode, ARMv4::OperationFormat::CRRAC8_1,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new ARM_Const8_Unsigned( 255 ) );
        cout << o1 << endl;

        WIR_Operation o2(
          opcode, ARMv4::OperationFormat::CRRAR_3,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );
        cout << o2 << endl;

        for ( auto &prepost : memoryAddressingModes ) {
          WIR_Operation o3(
            opcode, ARMv4::OperationFormat::CRARAC8_1,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new ARM_Const8_Unsigned( 255 ) );
          cout << o3 << endl;

          WIR_Operation o4(
            opcode, ARMv4::OperationFormat::CRARAR_1,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ) );
          cout << o4 << endl;
        }
      }

  // Store operations
  const vector<ARMv4::OpCode> stOps1 {
    ARMv4::OpCode::STR,
    ARMv4::OpCode::STRB };

  for ( auto &opcode : stOps1 )
    for ( auto &cond : conditions )
      for ( auto &addsub : memoryAddSubModes ) {
        for ( auto &prepost : memoryAddressingModes ) {
          WIR_Operation o1(
            opcode, ARMv4::OperationFormat::CRARAC12_2,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::use ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new ARM_Const12_Unsigned( 4095 ) );
          cout << o1 << endl;

          WIR_Operation o2(
            opcode, ARMv4::OperationFormat::CRARAR_3,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::use ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ) );
          cout << o2 << endl;
        }

        WIR_Operation o1(
          opcode, ARMv4::OperationFormat::CRRAC12_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new ARM_Const12_Unsigned( 4095 ) );
        cout << o1 << endl;

        WIR_Operation o2(
          opcode, ARMv4::OperationFormat::CRRAR_6,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );
        cout << o2 << endl;

        WIR_Operation o3(
          opcode, ARMv4::OperationFormat::CRRARC5_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned( 31 ) );
        cout << o3 << endl;

        for ( auto &rshift : rightShiftModes ) {
          WIR_Operation o4(
            opcode, ARMv4::OperationFormat::CRRARAC60_2,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::use ),
            new WIR_RegisterParameter( r2, WIR_Usage::use ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new WIR_AddressingModeParameter( rshift ),
            new ARM_Const6_Unsigned0( 32 ) );
          cout << o4 << endl;
        }

        WIR_Operation o4(
          opcode, ARMv4::OperationFormat::CRRARC50_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned0( 31 ) );
        cout << o4 << endl;

        WIR_Operation o5(
          opcode, ARMv4::OperationFormat::CRRAR_7,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );
        cout << o5 << endl;

        for ( auto &prepost : memoryAddressingModes ) {
          WIR_Operation o6(
            opcode, ARMv4::OperationFormat::CRARARC5_2,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::use ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new ARM_Const5_Unsigned( 31 ) );
          cout << o6 << endl;

          for ( auto &rshift : rightShiftModes ) {
            WIR_Operation o7(
              opcode, ARMv4::OperationFormat::CRARARAC60_2,
              new WIR_ConditionFieldParameter( cond ),
              new WIR_RegisterParameter( r1, WIR_Usage::use ),
              new WIR_AddressingModeParameter( prepost ),
              new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
              new WIR_AddressingModeParameter( addsub ),
              new WIR_RegisterParameter( r3, WIR_Usage::use ),
              new WIR_AddressingModeParameter( rshift ),
              new ARM_Const6_Unsigned0( 32 ) );
            cout << o7 << endl;
          }

          WIR_Operation o8(
            opcode, ARMv4::OperationFormat::CRARARC50_2,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::use ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new ARM_Const5_Unsigned0( 31 ) );
          cout << o8 << endl;

          WIR_Operation o9(
            opcode, ARMv4::OperationFormat::CRARAR_4,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::use ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ) );
          cout << o9 << endl;
        }
      }

  const vector<ARMv4::OpCode> stOps2 {
    ARMv4::OpCode::STRBT,
    ARMv4::OpCode::STRT };

  for ( auto &opcode : stOps2 )
    for ( auto &cond : conditions )
      for ( auto &addsub : memoryAddSubModes ) {
        WIR_Operation o1(
          opcode, ARMv4::OperationFormat::CRARAC12_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new ARM_Const12_Unsigned( 4095 ) );
        cout << o1 << endl;

        WIR_Operation o2(
          opcode, ARMv4::OperationFormat::CRARAR_3,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );
        cout << o2 << endl;

        WIR_Operation o6(
          opcode, ARMv4::OperationFormat::CRARARC5_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned( 31 ) );
        cout << o6 << endl;

        for ( auto &rshift : rightShiftModes ) {
          WIR_Operation o7(
            opcode, ARMv4::OperationFormat::CRARARAC60_2,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::use ),
            new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new WIR_AddressingModeParameter( rshift ),
            new ARM_Const6_Unsigned0( 32 ) );
          cout << o7 << endl;
        }

        WIR_Operation o8(
          opcode, ARMv4::OperationFormat::CRARARC50_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned0( 31 ) );
        cout << o8 << endl;

        WIR_Operation o9(
          opcode, ARMv4::OperationFormat::CRARAR_4,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );
        cout << o9 << endl;
      }

  for ( auto &cond : conditions )
    for ( auto &addsub : memoryAddSubModes ) {
      WIR_Operation o1(
        ARMv4::OpCode::STRH, ARMv4::OperationFormat::CRRAC8_2,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_RegisterParameter( r1, WIR_Usage::use ),
        new WIR_RegisterParameter( r2, WIR_Usage::use ),
        new WIR_AddressingModeParameter( addsub ),
        new ARM_Const8_Unsigned( 255 ) );
      cout << o1 << endl;

      WIR_Operation o2(
        ARMv4::OpCode::STRH, ARMv4::OperationFormat::CRRAR_6,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_RegisterParameter( r1, WIR_Usage::use ),
        new WIR_RegisterParameter( r2, WIR_Usage::use ),
        new WIR_AddressingModeParameter( addsub ),
        new WIR_RegisterParameter( r3, WIR_Usage::use ) );
      cout << o2 << endl;

      for ( auto &prepost : memoryAddressingModes ) {
        WIR_Operation o3(
          ARMv4::OpCode::STRH, ARMv4::OperationFormat::CRARAC8_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_AddressingModeParameter( prepost ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new ARM_Const8_Unsigned( 255 ) );
        cout << o3 << endl;

        WIR_Operation o4(
          ARMv4::OpCode::STRH, ARMv4::OperationFormat::CRARAR_3,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_AddressingModeParameter( prepost ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );
        cout << o4 << endl;
      }
    }

  // Branch operations.
  for ( auto &cond : conditions ) {
    WIR_Operation o1(
      ARMv4::OpCode::B, ARMv4::OperationFormat::CL,
      new WIR_ConditionFieldParameter( cond ),
      new WIR_LabelParameter( b ) );
    cout << o1 << endl;

    WIR_Operation o2(
      ARMv4::OpCode::BL, ARMv4::OperationFormat::CL,
      new WIR_ConditionFieldParameter( cond ),
      new WIR_LabelParameter( f ) );
    cout << o2 << endl;
  }

  // Load multiple operations.
  for ( auto &cond : conditions )
    for ( auto &mode : multipleModes ) {

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR1_1,
              ARMv4::OperationFormat::CARR1_3,
              ARMv4::OperationFormat::CARR1_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR1_2,
              ARMv4::OperationFormat::CARR1_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR2_1,
              ARMv4::OperationFormat::CARR2_3,
              ARMv4::OperationFormat::CARR2_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR2_2,
              ARMv4::OperationFormat::CARR2_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR3_1,
              ARMv4::OperationFormat::CARR3_3,
              ARMv4::OperationFormat::CARR3_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR3_2,
              ARMv4::OperationFormat::CARR3_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR4_1,
              ARMv4::OperationFormat::CARR4_3,
              ARMv4::OperationFormat::CARR4_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR4_2,
              ARMv4::OperationFormat::CARR4_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR5_1,
              ARMv4::OperationFormat::CARR5_3,
              ARMv4::OperationFormat::CARR5_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR5_2,
              ARMv4::OperationFormat::CARR5_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR6_1,
              ARMv4::OperationFormat::CARR6_3,
              ARMv4::OperationFormat::CARR6_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR6_2,
              ARMv4::OperationFormat::CARR6_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR7_1,
              ARMv4::OperationFormat::CARR7_3,
              ARMv4::OperationFormat::CARR7_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR7_2,
              ARMv4::OperationFormat::CARR7_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR8_1,
              ARMv4::OperationFormat::CARR8_3,
              ARMv4::OperationFormat::CARR8_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR8_2,
              ARMv4::OperationFormat::CARR8_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR9_1,
              ARMv4::OperationFormat::CARR9_3,
              ARMv4::OperationFormat::CARR9_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR9_2,
              ARMv4::OperationFormat::CARR9_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR10_1,
              ARMv4::OperationFormat::CARR10_3,
              ARMv4::OperationFormat::CARR10_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR10_2,
              ARMv4::OperationFormat::CARR10_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR11_1,
              ARMv4::OperationFormat::CARR11_3,
              ARMv4::OperationFormat::CARR11_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR11_2,
              ARMv4::OperationFormat::CARR11_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR12_1,
              ARMv4::OperationFormat::CARR12_3,
              ARMv4::OperationFormat::CARR12_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR12_2,
              ARMv4::OperationFormat::CARR12_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR13_1,
              ARMv4::OperationFormat::CARR13_3,
              ARMv4::OperationFormat::CARR13_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R12(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR13_2,
              ARMv4::OperationFormat::CARR13_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R12(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR14_1,
              ARMv4::OperationFormat::CARR14_3,
              ARMv4::OperationFormat::CARR14_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R12(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R13(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR14_2,
              ARMv4::OperationFormat::CARR14_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R12(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R13(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR15_1,
              ARMv4::OperationFormat::CARR15_3,
              ARMv4::OperationFormat::CARR15_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R12(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R13(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R14(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR15_2,
              ARMv4::OperationFormat::CARR15_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R12(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R13(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R14(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR16_1,
              ARMv4::OperationFormat::CARR16_3 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R12(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R13(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R14(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R15(), WIR_Usage::def ) );
        cout << o << endl;
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR16_2,
              ARMv4::OperationFormat::CARR16_4 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R12(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R13(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R14(), WIR_Usage::def ),
          new WIR_RegisterParameter( p.R15(), WIR_Usage::def ) );
        cout << o << endl;
      }
    }

  // Store multiple operations.
  for ( auto &cond : conditions )
    for ( auto &mode : multipleModes ) {

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR1_6,
              ARMv4::OperationFormat::CARR1_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o12(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR1_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ) );
      cout << o12 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR2_6,
              ARMv4::OperationFormat::CARR2_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o22(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR2_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ) );
      cout << o22 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR3_6,
              ARMv4::OperationFormat::CARR3_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o32(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR3_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ) );
      cout << o32 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR4_6,
              ARMv4::OperationFormat::CARR4_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o42(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR4_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R3(), WIR_Usage::use ) );
      cout << o42 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR5_6,
              ARMv4::OperationFormat::CARR5_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o52(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR5_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R4(), WIR_Usage::use ) );
      cout << o52 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR6_6,
              ARMv4::OperationFormat::CARR6_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o62(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR6_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R5(), WIR_Usage::use ) );
      cout << o62 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR7_6,
              ARMv4::OperationFormat::CARR7_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o72(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR7_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R6(), WIR_Usage::use ) );
      cout << o72 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR8_6,
              ARMv4::OperationFormat::CARR8_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o82(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR8_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R7(), WIR_Usage::use ) );
      cout << o82 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR9_6,
              ARMv4::OperationFormat::CARR9_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o92(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR9_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R8(), WIR_Usage::use ) );
      cout << o92 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR10_6,
              ARMv4::OperationFormat::CARR10_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o102(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR10_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R9(), WIR_Usage::use ) );
      cout << o102 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR11_6,
              ARMv4::OperationFormat::CARR11_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o112(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR11_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R10(), WIR_Usage::use ) );
      cout << o112 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR12_6,
              ARMv4::OperationFormat::CARR12_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o122(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR12_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R10(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R11(), WIR_Usage::use ) );
      cout << o122 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR13_6,
              ARMv4::OperationFormat::CARR13_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R12(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o132(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR13_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R10(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R11(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R12(), WIR_Usage::use ) );
      cout << o132 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR14_6,
              ARMv4::OperationFormat::CARR14_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R12(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R13(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o142(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR14_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R10(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R11(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R12(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R13(), WIR_Usage::use ) );
      cout << o142 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR15_6,
              ARMv4::OperationFormat::CARR15_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R12(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R13(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R14(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o152(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR15_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R10(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R11(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R12(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R13(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R14(), WIR_Usage::use ) );
      cout << o152 << endl;

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR16_6,
              ARMv4::OperationFormat::CARR16_8 } ) {
        WIR_Operation o(
          ARMv4::OpCode::STM, format, WIR_ConditionFieldParameter( cond ),
          new WIR_AddressingModeParameter( mode ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R10(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R11(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R12(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R13(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R14(), WIR_Usage::use ),
          new WIR_RegisterParameter( p.R15(), WIR_Usage::use ) );
        cout << o << endl;
      }

      WIR_Operation o162(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR16_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R3(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R4(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R5(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R6(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R7(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R8(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R10(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R11(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R12(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R13(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R14(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R15(), WIR_Usage::use ) );
      cout << o162 << endl;
    }

  for ( auto &cond : conditions )
    for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CR_1,
              ARMv4::OperationFormat::CR_2 } ) {
      WIR_Operation o(
        ARMv4::OpCode::MRS, format, WIR_ConditionFieldParameter( cond ),
        new WIR_RegisterParameter( r1, WIR_Usage::def ) );
      cout << o << endl;
    }

  for ( auto &cond : conditions ) {
    for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CAAAAC8RA_1,
              ARMv4::OperationFormat::CAAAAC8RA_2 } ) {
      WIR_Operation o(
        ARMv4::OpCode::MSR, format, WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( ARMv4::AddressingMode::f ),
        new WIR_AddressingModeParameter( ARMv4::AddressingMode::x ),
        new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
        new WIR_AddressingModeParameter( ARMv4::AddressingMode::s ),
        new ARM_Const8_Unsigned( 42 ),
        new ARM_Const5_RotateAmount( 6 ) );
      cout << o << endl;
    }

    for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CAAAAR_1,
              ARMv4::OperationFormat::CAAAAR_2 } ) {
      WIR_Operation o(
        ARMv4::OpCode::MSR, format, WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( ARMv4::AddressingMode::s ),
        new WIR_AddressingModeParameter( ARMv4::AddressingMode::x ),
        new WIR_AddressingModeParameter( ARMv4::AddressingMode::s ),
        new WIR_AddressingModeParameter( ARMv4::AddressingMode::s ),
        new WIR_RegisterParameter( r1, WIR_Usage::use ) );
      cout << o << endl;
    }
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o(
      ARMv4::OpCode::SWI, ARMv4::OperationFormat::CC24,
      new WIR_ConditionFieldParameter( cond ),
      new ARM_Const24_Unsigned( 42 ) );
    cout << o << endl;
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o1(
      ARMv4::OpCode::SWP, ARMv4::OperationFormat::CRRR_5,
      new WIR_ConditionFieldParameter( cond ),
      new WIR_RegisterParameter( r1, WIR_Usage::def ),
      new WIR_RegisterParameter( r2, WIR_Usage::use ),
      new WIR_RegisterParameter( r4, WIR_Usage::use ) );
    cout << o1 << endl;

    WIR_Operation o2(
      ARMv4::OpCode::SWPB, ARMv4::OperationFormat::CRRR_5,
      new WIR_ConditionFieldParameter( cond ),
      new WIR_RegisterParameter( r1, WIR_Usage::def ),
      new WIR_RegisterParameter( r2, WIR_Usage::use ),
      new WIR_RegisterParameter( r4, WIR_Usage::use ) );
    cout << o2 << endl;
  }

  // Coprocessor operations.
  for ( auto &cond : conditions )
    for ( auto &proc : coprocessors ) {
      WIR_Operation o1(
        ARMv4::OpCode::MCR, ARMv4::OperationFormat::CAORSSO_2,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( proc ),
        new ARM_Const3_CoprocessorOpcode( 0 ),
        new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
        new WIR_StringParameter( "c9" ),
        new WIR_StringParameter( "c5" ),
        new ARM_Const3_CoprocessorOpcode( 1 ) );
      cout << o1 << endl;

      WIR_Operation o2(
        ARMv4::OpCode::MRC, ARMv4::OperationFormat::CAORSSO_1,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( proc ),
        new ARM_Const3_CoprocessorOpcode( 0 ),
        new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
        new WIR_StringParameter( "c9" ),
        new WIR_StringParameter( "c5" ),
        new ARM_Const3_CoprocessorOpcode( 1 ) );
      cout << o2 << endl;

      WIR_Operation o3(
        ARMv4::OpCode::CDP, ARMv4::OperationFormat::CAOSSSO,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( proc ),
        new ARM_Const4_CoprocessorOpcode( 15 ),
        new WIR_StringParameter( "c9" ),
        new WIR_StringParameter( "c5" ),
        new WIR_StringParameter( "c6" ),
        new ARM_Const3_CoprocessorOpcode( 1 ) );
      cout << o3 << endl;

      for ( auto &opcode : vector<ARMv4::OpCode> { ARMv4::OpCode::LDC,
                                                      ARMv4::OpCode::STC } ) {
        for ( auto &addsub : memoryAddSubModes ) {
          for ( auto &format : vector<ARMv4::OperationFormat> { ARMv4::OperationFormat::CASRAC10_1,
                                                                   ARMv4::OperationFormat::CASRAC10_2 } ) {
            WIR_Operation o(
              opcode, format,
              new WIR_ConditionFieldParameter( cond ),
              new WIR_AddressingModeParameter( proc ),
              new WIR_StringParameter( "c9" ),
              new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
              new WIR_AddressingModeParameter( addsub ),
              new ARM_Const10_CoprocessorOffset( 20 ) );
            cout << o << endl;
          }

          for ( auto &prepost : memoryAddressingModes )
            for ( auto &format : vector<ARMv4::OperationFormat> { ARMv4::OperationFormat::CASARAC10_1,
                                                                     ARMv4::OperationFormat::CASARAC10_2 } ) {
              WIR_Operation o(
                opcode, format,
                new WIR_ConditionFieldParameter( cond ),
                new WIR_AddressingModeParameter( proc ),
                new WIR_StringParameter( "c12" ),
                new WIR_AddressingModeParameter( prepost ),
                new WIR_RegisterParameter( p.R9(), WIR_Usage::defuse ),
                new WIR_AddressingModeParameter( addsub ),
                new ARM_Const10_CoprocessorOffset( 44 ) );
              cout << o << endl;
            }

          for ( auto &format : vector<ARMv4::OperationFormat> { ARMv4::OperationFormat::CASRC8_1,
                                                                   ARMv4::OperationFormat::CASRC8_2 } ) {
            WIR_Operation o(
              opcode, format,
              new WIR_ConditionFieldParameter( cond ),
              new WIR_AddressingModeParameter( proc ),
              new WIR_StringParameter( "c4" ),
              new WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
              new ARM_Const8_Unsigned( 255 ) );
            cout << o << endl;
          }
        }
      }
    }

  return( 0 );
}
