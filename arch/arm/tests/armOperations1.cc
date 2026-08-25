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

// Include libuseful headers
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/arm/armv4.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  ARMv4 p;
  ARM_RegV r1, r4;
  ARM_LoRegV r2;
  ARM_HiRegV r3;
  WIR_BasicBlock b;
  WIR_Function f( "main" );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }

      for ( auto &format : f12 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

          ufAssert( o.getSize() == 4 );
          ufAssert(
            !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
            !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
            !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
            !o.isConditionalJump() && !o.isUnconditionalJump() &&
            !o.isIndirectJump() );
        }

      for ( auto &format : f14 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned( 0 ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

          ufAssert( o.getSize() == 4 );
          ufAssert(
            !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
            !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
            !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
            !o.isConditionalJump() && !o.isUnconditionalJump() &&
            !o.isIndirectJump() );
        }

      for ( auto &format : f16 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned0( 1 ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }

      for ( auto &format : f22 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isCall() && !o.isIndirectCall() &&
          !o.isReturn() && !o.isJump() && !o.isConditionalJump() &&
          !o.isUnconditionalJump() && !o.isIndirectJump() );

        if ( ( opcode == ARMv4::OpCode::MOV ) &&
             ( cond == ARMv4::Condition::al ) &&
             ( ( format == ARMv4::OperationFormat::CRR_1 ) ||
               ( format == ARMv4::OperationFormat::CRR_2 ) ) )
          ufAssert( o.isMove() );
        else
          ufAssert( !o.isMove() );
      }

      for ( auto &format : f23 )
        for ( auto &amode : amode1 ) {
          WIR_Operation o(
            opcode, format, WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new WIR_AddressingModeParameter( amode ),
            new WIR_RegisterParameter( r4, WIR_Usage::use ) );

          ufAssert( o.getSize() == 4 );
          ufAssert(
            !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
            !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
            !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
            !o.isConditionalJump() && !o.isUnconditionalJump() &&
            !o.isIndirectJump() );
        }

      for ( auto &format : f24 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned( 0 ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }

      for ( auto &format : f25 )
        for ( auto &amode : amode2 ) {
          WIR_Operation o(
            opcode, format, WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ),
            new WIR_AddressingModeParameter( amode ),
            new ARM_Const6_Unsigned0( 1 ) );

          ufAssert( o.getSize() == 4 );
          ufAssert(
            !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
            !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
            !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
            !o.isConditionalJump() && !o.isUnconditionalJump() &&
            !o.isIndirectJump() );
        }

      for ( auto &format : f26 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned0( 1 ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

      ufAssert( o1.getSize() == 4 );
      ufAssert(
        !o1.isImplicitMemoryAccess() && !o1.isMemoryStore() &&
        !o1.isMemoryLoad() && !o1.isMove() && !o1.isCall() &&
        !o1.isIndirectCall() && !o1.isReturn() && !o1.isJump() &&
        !o1.isConditionalJump() && !o1.isUnconditionalJump() &&
        !o1.isIndirectJump() );

      for ( auto &format : f32 ) {
        WIR_Operation o(
          opcode, format, WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }

      for ( auto &amode : amode1 ) {
        WIR_Operation o(
          opcode, ARMv4::OperationFormat::CRRAR_5,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new WIR_AddressingModeParameter( amode ),
          new WIR_RegisterParameter( r4, WIR_Usage::use ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }

      WIR_Operation o2(
        opcode, ARMv4::OperationFormat::CRRC5_3,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_RegisterParameter( r2, WIR_Usage::use ),
        new WIR_RegisterParameter( r3, WIR_Usage::use ),
        new ARM_Const5_Unsigned( 0 ) );

      ufAssert( o2.getSize() == 4 );
      ufAssert(
        !o2.isImplicitMemoryAccess() && !o2.isMemoryStore() &&
        !o2.isMemoryLoad() && !o2.isMove() && !o2.isCall() &&
        !o2.isIndirectCall() && !o2.isReturn() && !o2.isJump() &&
        !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
        !o2.isIndirectJump() );

      for ( auto &amode : amode2 ) {
        WIR_Operation o(
          opcode, ARMv4::OperationFormat::CRRAC60_3,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new WIR_AddressingModeParameter( amode ),
          new ARM_Const6_Unsigned0( 1 ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }

      WIR_Operation o3(
        opcode, ARMv4::OperationFormat::CRRC50_3,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_RegisterParameter( r2, WIR_Usage::use ),
        new WIR_RegisterParameter( r3, WIR_Usage::use ),
        new ARM_Const5_Unsigned0( 1 ) );

      ufAssert( o3.getSize() == 4 );
      ufAssert(
        !o3.isImplicitMemoryAccess() && !o3.isMemoryStore() &&
        !o3.isMemoryLoad() && !o3.isMove() && !o3.isCall() &&
        !o3.isIndirectCall() && !o3.isReturn() && !o3.isJump() &&
        !o3.isConditionalJump() && !o3.isUnconditionalJump() &&
        !o3.isIndirectJump() );
    }

  const vector<ARMv4::OperationFormat> f41 {
    ARMv4::OperationFormat::CRRRR_1, ARMv4::OperationFormat::CRRRR_2 };

  for ( auto cond : conditions ) {
    for ( auto format : f41 ) {
      WIR_Operation o(
        ARMv4::OpCode::MLA, format, WIR_ConditionFieldParameter( cond ),
        new WIR_RegisterParameter( r1, WIR_Usage::def ),
        new WIR_RegisterParameter( r2, WIR_Usage::use ),
        new WIR_RegisterParameter( r3, WIR_Usage::use ),
        new WIR_RegisterParameter( r4, WIR_Usage::use ) );

      ufAssert( o.getSize() == 4 );
      ufAssert(
        !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
        !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
        !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
        !o.isConditionalJump() && !o.isUnconditionalJump() &&
        !o.isIndirectJump() );
    }
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o1(
      ARMv4::OpCode::MUL, ARMv4::OperationFormat::CRRR_1,
      new WIR_ConditionFieldParameter( cond ),
      new WIR_RegisterParameter( r1, WIR_Usage::def ),
      new WIR_RegisterParameter( r2, WIR_Usage::use ),
      new WIR_RegisterParameter( r4, WIR_Usage::use ) );

    ufAssert( o1.getSize() == 4 );
    ufAssert(
      !o1.isImplicitMemoryAccess() && !o1.isMemoryStore() &&
      !o1.isMemoryLoad() && !o1.isMove() && !o1.isCall() &&
      !o1.isIndirectCall() && !o1.isReturn() && !o1.isJump() &&
      !o1.isConditionalJump() && !o1.isUnconditionalJump() &&
      !o1.isIndirectJump() );

    WIR_Operation o2(
      ARMv4::OpCode::MUL, ARMv4::OperationFormat::CRRR_2,
      new WIR_ConditionFieldParameter( cond ),
      new WIR_RegisterParameter( r1, WIR_Usage::def ),
      new WIR_RegisterParameter( r2, WIR_Usage::use ),
      new WIR_RegisterParameter( r4, WIR_Usage::use ) );

    ufAssert( o2.getSize() == 4 );
    ufAssert(
      !o2.isImplicitMemoryAccess() && !o2.isMemoryStore() &&
      !o2.isMemoryLoad() && !o2.isMove() && !o2.isCall() &&
      !o2.isIndirectCall() && !o2.isReturn() && !o2.isJump() &&
      !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
      !o2.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }
    }

  // Load operations
  const vector<ARMv4::OpCode> ldOps {
    ARMv4::OpCode::LDR,
    ARMv4::OpCode::LDRB };

  for ( auto &opcode : ldOps )
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

          ufAssert( o1.getSize() == 4 );
          ufAssert(
            !o1.isImplicitMemoryAccess() && !o1.isMemoryStore() &&
            o1.isMemoryLoad() && !o1.isMove() && !o1.isCall() &&
            !o1.isIndirectCall() && !o1.isReturn() && !o1.isJump() &&
            !o1.isConditionalJump() && !o1.isUnconditionalJump() &&
            !o1.isIndirectJump() );

          WIR_Operation o2(
            opcode, ARMv4::OperationFormat::CRARAR_1,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ) );

          ufAssert( o2.getSize() == 4 );
          ufAssert(
            !o2.isImplicitMemoryAccess() && !o2.isMemoryStore() &&
            o2.isMemoryLoad() && !o2.isMove() && !o2.isCall() &&
            !o2.isIndirectCall() && !o2.isReturn() && !o2.isJump() &&
            !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
            !o2.isIndirectJump() );
        }

        WIR_Operation o1(
          opcode, ARMv4::OperationFormat::CRRAC12_1,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new ARM_Const12_Unsigned( 4095 ) );

        ufAssert( o1.getSize() == 4 );
        ufAssert(
          !o1.isImplicitMemoryAccess() && !o1.isMemoryStore() &&
          o1.isMemoryLoad() && !o1.isMove() && !o1.isCall() &&
          !o1.isIndirectCall() && !o1.isReturn() && !o1.isJump() &&
          !o1.isConditionalJump() && !o1.isUnconditionalJump() &&
          !o1.isIndirectJump() );

        WIR_Operation o2(
          opcode, ARMv4::OperationFormat::CRRAR_3,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );

        ufAssert( o2.getSize() == 4 );
        ufAssert(
          !o2.isImplicitMemoryAccess() && !o2.isMemoryStore() &&
          o2.isMemoryLoad() && !o2.isMove() && !o2.isCall() &&
          !o2.isIndirectCall() && !o2.isReturn() && !o2.isJump() &&
          !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
          !o2.isIndirectJump() );

        WIR_Operation o3(
          opcode, ARMv4::OperationFormat::CRRARC5_1,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned( 0 ) );

        ufAssert( o3.getSize() == 4 );
        ufAssert(
          !o3.isImplicitMemoryAccess() && !o3.isMemoryStore() &&
          o3.isMemoryLoad() && !o3.isMove() && !o3.isCall() &&
          !o3.isIndirectCall() && !o3.isReturn() && !o3.isJump() &&
          !o3.isConditionalJump() && !o3.isUnconditionalJump() &&
          !o3.isIndirectJump() );

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

          ufAssert( o4.getSize() == 4 );
          ufAssert(
            !o4.isImplicitMemoryAccess() && !o4.isMemoryStore() &&
            o4.isMemoryLoad() && !o4.isMove() && !o4.isCall() &&
            !o4.isIndirectCall() && !o4.isReturn() && !o4.isJump() &&
            !o4.isConditionalJump() && !o4.isUnconditionalJump() &&
            !o4.isIndirectJump() );
        }

        WIR_Operation o4(
          opcode, ARMv4::OperationFormat::CRRARC50_1,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned0( 31 ) );

        ufAssert( o4.getSize() == 4 );
        ufAssert(
          !o4.isImplicitMemoryAccess() && !o4.isMemoryStore() &&
          o4.isMemoryLoad() && !o4.isMove() && !o4.isCall() &&
          !o4.isIndirectCall() && !o4.isReturn() && !o4.isJump() &&
          !o4.isConditionalJump() && !o4.isUnconditionalJump() &&
          !o4.isIndirectJump() );

        WIR_Operation o5(
          opcode, ARMv4::OperationFormat::CRRAR_4,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );

        ufAssert( o5.getSize() == 4 );
        ufAssert(
          !o5.isImplicitMemoryAccess() && !o5.isMemoryStore() &&
          o5.isMemoryLoad() && !o5.isMove() && !o5.isCall() &&
          !o5.isIndirectCall() && !o5.isReturn() && !o5.isJump() &&
          !o5.isConditionalJump() && !o5.isUnconditionalJump() &&
          !o5.isIndirectJump() );

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

          ufAssert( o6.getSize() == 4 );
          ufAssert(
            !o6.isImplicitMemoryAccess() && !o6.isMemoryStore() &&
            o6.isMemoryLoad() && !o6.isMove() && !o6.isCall() &&
            !o6.isIndirectCall() && !o6.isReturn() && !o6.isJump() &&
            !o6.isConditionalJump() && !o6.isUnconditionalJump() &&
            !o6.isIndirectJump() );

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

            ufAssert( o7.getSize() == 4 );
            ufAssert(
              !o7.isImplicitMemoryAccess() && !o7.isMemoryStore() &&
              o7.isMemoryLoad() && !o7.isMove() && !o7.isCall() &&
              !o7.isIndirectCall() && !o7.isReturn() && !o7.isJump() &&
              !o7.isConditionalJump() && !o7.isUnconditionalJump() &&
              !o7.isIndirectJump() );
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

          ufAssert( o8.getSize() == 4 );
          ufAssert(
            !o8.isImplicitMemoryAccess() && !o8.isMemoryStore() &&
            o8.isMemoryLoad() && !o8.isMove() && !o8.isCall() &&
            !o8.isIndirectCall() && !o8.isReturn() && !o8.isJump() &&
            !o8.isConditionalJump() && !o8.isUnconditionalJump() &&
            !o8.isIndirectJump() );

          WIR_Operation o9(
            opcode, ARMv4::OperationFormat::CRARAR_2,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ) );

          ufAssert( o9.getSize() == 4 );
          ufAssert(
            !o9.isImplicitMemoryAccess() && !o9.isMemoryStore() &&
            o9.isMemoryLoad() && !o9.isMove() && !o9.isCall() &&
            !o9.isIndirectCall() && !o9.isReturn() && !o9.isJump() &&
            !o9.isConditionalJump() && !o9.isUnconditionalJump() &&
            !o9.isIndirectJump() );
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

        ufAssert( o1.getSize() == 4 );
        ufAssert(
          !o1.isImplicitMemoryAccess() && !o1.isMemoryStore() &&
          o1.isMemoryLoad() && !o1.isMove() && !o1.isCall() &&
          !o1.isIndirectCall() && !o1.isReturn() && !o1.isJump() &&
          !o1.isConditionalJump() && !o1.isUnconditionalJump() &&
          !o1.isIndirectJump() );

        WIR_Operation o2(
          opcode, ARMv4::OperationFormat::CRARAR_1,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );

        ufAssert( o2.getSize() == 4 );
        ufAssert(
          !o2.isImplicitMemoryAccess() && !o2.isMemoryStore() &&
          o2.isMemoryLoad() && !o2.isMove() && !o2.isCall() &&
          !o2.isIndirectCall() && !o2.isReturn() && !o2.isJump() &&
          !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
          !o2.isIndirectJump() );

        WIR_Operation o6(
          opcode, ARMv4::OperationFormat::CRARARC5_1,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned( 31 ) );

        ufAssert( o6.getSize() == 4 );
        ufAssert(
          !o6.isImplicitMemoryAccess() && !o6.isMemoryStore() &&
          o6.isMemoryLoad() && !o6.isMove() && !o6.isCall() &&
          !o6.isIndirectCall() && !o6.isReturn() && !o6.isJump() &&
          !o6.isConditionalJump() && !o6.isUnconditionalJump() &&
          !o6.isIndirectJump() );

        for ( auto &rshift : rightShiftModes ) {
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

          ufAssert( o7.getSize() == 4 );
          ufAssert(
            !o7.isImplicitMemoryAccess() && !o7.isMemoryStore() &&
            o7.isMemoryLoad() && !o7.isMove() && !o7.isCall() &&
            !o7.isIndirectCall() && !o7.isReturn() && !o7.isJump() &&
            !o7.isConditionalJump() && !o7.isUnconditionalJump() &&
            !o7.isIndirectJump() );
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

        ufAssert( o8.getSize() == 4 );
        ufAssert(
          !o8.isImplicitMemoryAccess() && !o8.isMemoryStore() &&
          o8.isMemoryLoad() && !o8.isMove() && !o8.isCall() &&
          !o8.isIndirectCall() && !o8.isReturn() && !o8.isJump() &&
          !o8.isConditionalJump() && !o8.isUnconditionalJump() &&
          !o8.isIndirectJump() );

        WIR_Operation o9(
          opcode, ARMv4::OperationFormat::CRARAR_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );

        ufAssert( o9.getSize() == 4 );
        ufAssert(
          !o9.isImplicitMemoryAccess() && !o9.isMemoryStore() &&
          o9.isMemoryLoad() && !o9.isMove() && !o9.isCall() &&
          !o9.isIndirectCall() && !o9.isReturn() && !o9.isJump() &&
          !o9.isConditionalJump() && !o9.isUnconditionalJump() &&
          !o9.isIndirectJump() );
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

        ufAssert( o1.getSize() == 4 );
        ufAssert(
          !o1.isImplicitMemoryAccess() && !o1.isMemoryStore() &&
          o1.isMemoryLoad() && !o1.isMove() && !o1.isCall() &&
          !o1.isIndirectCall() && !o1.isReturn() && !o1.isJump() &&
          !o1.isConditionalJump() && !o1.isUnconditionalJump() &&
          !o1.isIndirectJump() );

        WIR_Operation o2(
          opcode, ARMv4::OperationFormat::CRRAR_3,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::def ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );

        ufAssert( o2.getSize() == 4 );
        ufAssert(
          !o2.isImplicitMemoryAccess() && !o2.isMemoryStore() &&
          o2.isMemoryLoad() && !o2.isMove() && !o2.isCall() &&
          !o2.isIndirectCall() && !o2.isReturn() && !o2.isJump() &&
          !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
          !o2.isIndirectJump() );

        for ( auto &prepost : memoryAddressingModes ) {
          WIR_Operation o3(
            opcode, ARMv4::OperationFormat::CRARAC8_1,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new ARM_Const8_Unsigned( 255 ) );

          ufAssert( o3.getSize() == 4 );
          ufAssert(
            !o3.isImplicitMemoryAccess() && !o3.isMemoryStore() &&
            o3.isMemoryLoad() && !o3.isMove() && !o3.isCall() &&
            !o3.isIndirectCall() && !o3.isReturn() && !o3.isJump() &&
            !o3.isConditionalJump() && !o3.isUnconditionalJump() &&
            !o3.isIndirectJump() );

          WIR_Operation o4(
            opcode, ARMv4::OperationFormat::CRARAR_1,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::def ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ) );

          ufAssert( o4.getSize() == 4 );
          ufAssert(
            !o4.isImplicitMemoryAccess() && !o4.isMemoryStore() &&
            o4.isMemoryLoad() && !o4.isMove() && !o4.isCall() &&
            !o4.isIndirectCall() && !o4.isReturn() && !o4.isJump() &&
            !o4.isConditionalJump() && !o4.isUnconditionalJump() &&
            !o4.isIndirectJump() );
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

          ufAssert( o1.getSize() == 4 );
          ufAssert(
            !o1.isImplicitMemoryAccess() && o1.isMemoryStore() &&
            !o1.isMemoryLoad() && !o1.isMove() && !o1.isCall() &&
            !o1.isIndirectCall() && !o1.isReturn() && !o1.isJump() &&
            !o1.isConditionalJump() && !o1.isUnconditionalJump() &&
            !o1.isIndirectJump() );

          WIR_Operation o2(
            opcode, ARMv4::OperationFormat::CRARAR_3,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::use ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ) );

          ufAssert( o2.getSize() == 4 );
          ufAssert(
            !o2.isImplicitMemoryAccess() && o2.isMemoryStore() &&
            !o2.isMemoryLoad() && !o2.isMove() && !o2.isCall() &&
            !o2.isIndirectCall() && !o2.isReturn() && !o2.isJump() &&
            !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
            !o2.isIndirectJump() );
        }

        WIR_Operation o1(
          opcode, ARMv4::OperationFormat::CRRAC12_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new ARM_Const12_Unsigned( 4095 ) );

        ufAssert( o1.getSize() == 4 );
        ufAssert(
          !o1.isImplicitMemoryAccess() && o1.isMemoryStore() &&
          !o1.isMemoryLoad() && !o1.isMove() && !o1.isCall() &&
          !o1.isIndirectCall() && !o1.isReturn() && !o1.isJump() &&
          !o1.isConditionalJump() && !o1.isUnconditionalJump() &&
          !o1.isIndirectJump() );

        WIR_Operation o2(
          opcode, ARMv4::OperationFormat::CRRAR_6,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );

        ufAssert( o2.getSize() == 4 );
        ufAssert(
          !o2.isImplicitMemoryAccess() && o2.isMemoryStore() &&
          !o2.isMemoryLoad() && !o2.isMove() && !o2.isCall() &&
          !o2.isIndirectCall() && !o2.isReturn() && !o2.isJump() &&
          !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
          !o2.isIndirectJump() );

        WIR_Operation o3(
          opcode, ARMv4::OperationFormat::CRRARC5_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned( 31 ) );

        ufAssert( o3.getSize() == 4 );
        ufAssert(
          !o3.isImplicitMemoryAccess() && o3.isMemoryStore() &&
          !o3.isMemoryLoad() && !o3.isMove() && !o3.isCall() &&
          !o3.isIndirectCall() && !o3.isReturn() && !o3.isJump() &&
          !o3.isConditionalJump() && !o3.isUnconditionalJump() &&
          !o3.isIndirectJump() );

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

          ufAssert( o4.getSize() == 4 );
          ufAssert(
            !o4.isImplicitMemoryAccess() && o4.isMemoryStore() &&
            !o4.isMemoryLoad() && !o4.isMove() && !o4.isCall() &&
            !o4.isIndirectCall() && !o4.isReturn() && !o4.isJump() &&
            !o4.isConditionalJump() && !o4.isUnconditionalJump() &&
            !o4.isIndirectJump() );
        }

        WIR_Operation o4(
          opcode, ARMv4::OperationFormat::CRRARC50_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned0( 31 ) );

        ufAssert( o4.getSize() == 4 );
        ufAssert(
          !o4.isImplicitMemoryAccess() && o4.isMemoryStore() &&
          !o4.isMemoryLoad() && !o4.isMove() && !o4.isCall() &&
          !o4.isIndirectCall() && !o4.isReturn() && !o4.isJump() &&
          !o4.isConditionalJump() && !o4.isUnconditionalJump() &&
          !o4.isIndirectJump() );

        WIR_Operation o5(
          opcode, ARMv4::OperationFormat::CRRAR_7,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_RegisterParameter( r2, WIR_Usage::use ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );

        ufAssert( o5.getSize() == 4 );
        ufAssert(
          !o5.isImplicitMemoryAccess() && o5.isMemoryStore() &&
          !o5.isMemoryLoad() && !o5.isMove() && !o5.isCall() &&
          !o5.isIndirectCall() && !o5.isReturn() && !o5.isJump() &&
          !o5.isConditionalJump() && !o5.isUnconditionalJump() &&
          !o5.isIndirectJump() );

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

          ufAssert( o6.getSize() == 4 );
          ufAssert(
            !o6.isImplicitMemoryAccess() && o6.isMemoryStore() &&
            !o6.isMemoryLoad() && !o6.isMove() && !o6.isCall() &&
            !o6.isIndirectCall() && !o6.isReturn() && !o6.isJump() &&
            !o6.isConditionalJump() && !o6.isUnconditionalJump() &&
            !o6.isIndirectJump() );

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

            ufAssert( o7.getSize() == 4 );
            ufAssert(
              !o7.isImplicitMemoryAccess() && o7.isMemoryStore() &&
              !o7.isMemoryLoad() && !o7.isMove() && !o7.isCall() &&
              !o7.isIndirectCall() && !o7.isReturn() && !o7.isJump() &&
              !o7.isConditionalJump() && !o7.isUnconditionalJump() &&
              !o7.isIndirectJump() );
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

          ufAssert( o8.getSize() == 4 );
          ufAssert(
            !o8.isImplicitMemoryAccess() && o8.isMemoryStore() &&
            !o8.isMemoryLoad() && !o8.isMove() && !o8.isCall() &&
            !o8.isIndirectCall() && !o8.isReturn() && !o8.isJump() &&
            !o8.isConditionalJump() && !o8.isUnconditionalJump() &&
            !o8.isIndirectJump() );

          WIR_Operation o9(
            opcode, ARMv4::OperationFormat::CRARAR_4,
            new WIR_ConditionFieldParameter( cond ),
            new WIR_RegisterParameter( r1, WIR_Usage::use ),
            new WIR_AddressingModeParameter( prepost ),
            new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
            new WIR_AddressingModeParameter( addsub ),
            new WIR_RegisterParameter( r3, WIR_Usage::use ) );

          ufAssert( o9.getSize() == 4 );
          ufAssert(
            !o9.isImplicitMemoryAccess() && o9.isMemoryStore() &&
            !o9.isMemoryLoad() && !o9.isMove() && !o9.isCall() &&
            !o9.isIndirectCall() && !o9.isReturn() && !o9.isJump() &&
            !o9.isConditionalJump() && !o9.isUnconditionalJump() &&
            !o9.isIndirectJump() );
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

        ufAssert( o1.getSize() == 4 );
        ufAssert(
          !o1.isImplicitMemoryAccess() && o1.isMemoryStore() &&
          !o1.isMemoryLoad() && !o1.isMove() && !o1.isCall() &&
          !o1.isIndirectCall() && !o1.isReturn() && !o1.isJump() &&
          !o1.isConditionalJump() && !o1.isUnconditionalJump() &&
          !o1.isIndirectJump() );

        WIR_Operation o2(
          opcode, ARMv4::OperationFormat::CRARAR_3,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );

        ufAssert( o2.getSize() == 4 );
        ufAssert(
          !o2.isImplicitMemoryAccess() && o2.isMemoryStore() &&
          !o2.isMemoryLoad() && !o2.isMove() && !o2.isCall() &&
          !o2.isIndirectCall() && !o2.isReturn() && !o2.isJump() &&
          !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
          !o2.isIndirectJump() );

        WIR_Operation o6(
          opcode, ARMv4::OperationFormat::CRARARC5_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ),
          new ARM_Const5_Unsigned( 31 ) );

        ufAssert( o6.getSize() == 4 );
        ufAssert(
          !o6.isImplicitMemoryAccess() && o6.isMemoryStore() &&
          !o6.isMemoryLoad() && !o6.isMove() && !o6.isCall() &&
          !o6.isIndirectCall() && !o6.isReturn() && !o6.isJump() &&
          !o6.isConditionalJump() && !o6.isUnconditionalJump() &&
          !o6.isIndirectJump() );

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

          ufAssert( o7.getSize() == 4 );
          ufAssert(
            !o7.isImplicitMemoryAccess() && o7.isMemoryStore() &&
            !o7.isMemoryLoad() && !o7.isMove() && !o7.isCall() &&
            !o7.isIndirectCall() && !o7.isReturn() && !o7.isJump() &&
            !o7.isConditionalJump() && !o7.isUnconditionalJump() &&
            !o7.isIndirectJump() );
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

        ufAssert( o8.getSize() == 4 );
        ufAssert(
          !o8.isImplicitMemoryAccess() && o8.isMemoryStore() &&
          !o8.isMemoryLoad() && !o8.isMove() && !o8.isCall() &&
          !o8.isIndirectCall() && !o8.isReturn() && !o8.isJump() &&
          !o8.isConditionalJump() && !o8.isUnconditionalJump() &&
          !o8.isIndirectJump() );

        WIR_Operation o9(
          opcode, ARMv4::OperationFormat::CRARAR_4,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_AddressingModeParameter( ARMv4::AddressingMode::post ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );

        ufAssert( o9.getSize() == 4 );
        ufAssert(
          !o9.isImplicitMemoryAccess() && o9.isMemoryStore() &&
          !o9.isMemoryLoad() && !o9.isMove() && !o9.isCall() &&
          !o9.isIndirectCall() && !o9.isReturn() && !o9.isJump() &&
          !o9.isConditionalJump() && !o9.isUnconditionalJump() &&
          !o9.isIndirectJump() );
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

      ufAssert( o1.getSize() == 4 );
      ufAssert(
        !o1.isImplicitMemoryAccess() && o1.isMemoryStore() &&
        !o1.isMemoryLoad() && !o1.isMove() && !o1.isCall() &&
        !o1.isIndirectCall() && !o1.isReturn() && !o1.isJump() &&
        !o1.isConditionalJump() && !o1.isUnconditionalJump() &&
        !o1.isIndirectJump() );

      WIR_Operation o2(
        ARMv4::OpCode::STRH, ARMv4::OperationFormat::CRRAR_6,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_RegisterParameter( r1, WIR_Usage::use ),
        new WIR_RegisterParameter( r2, WIR_Usage::use ),
        new WIR_AddressingModeParameter( addsub ),
        new WIR_RegisterParameter( r3, WIR_Usage::use ) );

      ufAssert( o2.getSize() == 4 );
      ufAssert(
        !o2.isImplicitMemoryAccess() && o2.isMemoryStore() &&
        !o2.isMemoryLoad() && !o2.isMove() && !o2.isCall() &&
        !o2.isIndirectCall() && !o2.isReturn() && !o2.isJump() &&
        !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
        !o2.isIndirectJump() );

      for ( auto &prepost : memoryAddressingModes ) {
        WIR_Operation o3(
          ARMv4::OpCode::STRH, ARMv4::OperationFormat::CRARAC8_2,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_AddressingModeParameter( prepost ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new ARM_Const8_Unsigned( 255 ) );

        ufAssert( o3.getSize() == 4 );
        ufAssert(
          !o3.isImplicitMemoryAccess() && o3.isMemoryStore() &&
          !o3.isMemoryLoad() && !o3.isMove() && !o3.isCall() &&
          !o3.isIndirectCall() && !o3.isReturn() && !o3.isJump() &&
          !o3.isConditionalJump() && !o3.isUnconditionalJump() &&
          !o3.isIndirectJump() );

        WIR_Operation o4(
          ARMv4::OpCode::STRH, ARMv4::OperationFormat::CRARAR_3,
          new WIR_ConditionFieldParameter( cond ),
          new WIR_RegisterParameter( r1, WIR_Usage::use ),
          new WIR_AddressingModeParameter( prepost ),
          new WIR_RegisterParameter( r2, WIR_Usage::defuse ),
          new WIR_AddressingModeParameter( addsub ),
          new WIR_RegisterParameter( r3, WIR_Usage::use ) );

        ufAssert( o4.getSize() == 4 );
        ufAssert(
          !o4.isImplicitMemoryAccess() && o4.isMemoryStore() && !o4.isMemoryLoad() &&
          !o4.isMove() && !o4.isCall() && !o4.isIndirectCall() &&
          !o4.isReturn() && !o4.isJump() && !o4.isConditionalJump() &&
          !o4.isUnconditionalJump() && !o4.isIndirectJump() );
      }
    }

  // Branch operations.
  for ( auto &cond : conditions ) {
    WIR_Operation o1(
      ARMv4::OpCode::B, ARMv4::OperationFormat::CL,
      new WIR_ConditionFieldParameter( cond ),
      new WIR_LabelParameter( b ) );

    ufAssert( o1.getSize() == 4 );
    if ( cond == ARMv4::Condition::al )
      ufAssert(
        !o1.isImplicitMemoryAccess() && !o1.isMemoryStore() &&
        !o1.isMemoryLoad() && !o1.isMove() && !o1.isCall() &&
        !o1.isIndirectCall() && !o1.isReturn() && o1.isJump() &&
        !o1.isConditionalJump() && o1.isUnconditionalJump() &&
        !o1.isIndirectJump() );
    else
      ufAssert(
        !o1.isImplicitMemoryAccess() && !o1.isMemoryStore() &&
        !o1.isMemoryLoad() && !o1.isMove() && !o1.isCall() &&
        !o1.isIndirectCall() && !o1.isReturn() && o1.isJump() &&
        o1.isConditionalJump() && !o1.isUnconditionalJump() &&
        !o1.isIndirectJump() );

    WIR_Operation o2(
      ARMv4::OpCode::BL, ARMv4::OperationFormat::CL,
      new WIR_ConditionFieldParameter( cond ),
      new WIR_LabelParameter( f ) );

    ufAssert( o2.getSize() == 4 );
    ufAssert(
      !o2.isImplicitMemoryAccess() && !o2.isMemoryStore() &&
      !o2.isMemoryLoad() && !o2.isMove() && o2.isCall() &&
      !o2.isIndirectCall() && !o2.isReturn() && !o2.isJump() &&
      !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
      !o2.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }

      for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CARR9_1,
              ARMv4::OperationFormat::CARR9_3,
              ARMv4::OperationFormat::CARR9_5 } ) {
        WIR_Operation o(
          ARMv4::OpCode::LDM, format, WIR_ConditionFieldParameter( cond ),
          WIR_AddressingModeParameter( mode ),
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() &&  !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }

      WIR_Operation o12(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR1_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ) );

      ufAssert( o12.getSize() == 4 );
      ufAssert(
        !o12.isImplicitMemoryAccess() && o12.isMemoryStore() &&
        !o12.isMemoryLoad() && !o12.isMove() && !o12.isCall() &&
        !o12.isIndirectCall() && !o12.isReturn() && !o12.isJump() &&
        !o12.isConditionalJump() && !o12.isUnconditionalJump() &&
        !o12.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }

      WIR_Operation o22(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR2_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ) );

      ufAssert( o22.getSize() == 4 );
      ufAssert(
        !o22.isImplicitMemoryAccess() && o22.isMemoryStore() &&
        !o22.isMemoryLoad() && !o22.isMove() && !o22.isCall() &&
        !o22.isIndirectCall() && !o22.isReturn() && !o22.isJump() &&
        !o22.isConditionalJump() && !o22.isUnconditionalJump() &&
        !o22.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }

      WIR_Operation o32(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR3_7,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( mode ),
        new WIR_RegisterParameter( r1, WIR_Usage::defuse ),
        new WIR_RegisterParameter( p.R0(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
        new WIR_RegisterParameter( p.R2(), WIR_Usage::use ) );

      ufAssert( o32.getSize() == 4 );
      ufAssert(
        !o32.isImplicitMemoryAccess() && o32.isMemoryStore() &&
        !o32.isMemoryLoad() && !o32.isMove() && !o32.isCall() &&
        !o32.isIndirectCall() && !o32.isReturn() && !o32.isJump() &&
        !o32.isConditionalJump() && !o32.isUnconditionalJump() &&
        !o32.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

      ufAssert( o42.getSize() == 4 );
      ufAssert(
        !o42.isImplicitMemoryAccess() && o42.isMemoryStore() &&
        !o42.isMemoryLoad() && !o42.isMove() && !o42.isCall() &&
        !o42.isIndirectCall() && !o42.isReturn() && !o42.isJump() &&
        !o42.isConditionalJump() && !o42.isUnconditionalJump() &&
        !o42.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

      ufAssert( o52.getSize() == 4 );
      ufAssert(
        !o52.isImplicitMemoryAccess() && o52.isMemoryStore() &&
        !o52.isMemoryLoad() && !o52.isMove() && !o52.isCall() &&
        !o52.isIndirectCall() && !o52.isReturn() && !o52.isJump() &&
        !o52.isConditionalJump() && !o52.isUnconditionalJump() &&
        !o52.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

      ufAssert( o62.getSize() == 4 );
      ufAssert(
        !o62.isImplicitMemoryAccess() && o62.isMemoryStore() &&
        !o62.isMemoryLoad() && !o62.isMove() && !o62.isCall() &&
        !o62.isIndirectCall() && !o62.isReturn() && !o62.isJump() &&
        !o62.isConditionalJump() && !o62.isUnconditionalJump() &&
        !o62.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

      ufAssert( o72.getSize() == 4 );
      ufAssert(
        !o72.isImplicitMemoryAccess() && o72.isMemoryStore() &&
        !o72.isMemoryLoad() && !o72.isMove() && !o72.isCall() &&
        !o72.isIndirectCall() && !o72.isReturn() && !o72.isJump() &&
        !o72.isConditionalJump() && !o72.isUnconditionalJump() &&
        !o72.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }

      WIR_Operation o82(
        ARMv4::OpCode::STM, ARMv4::OperationFormat::CARR8_7,
        WIR_ConditionFieldParameter( cond ),
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

      ufAssert( o82.getSize() == 4 );
      ufAssert(
        !o82.isImplicitMemoryAccess() && o82.isMemoryStore() &&
        !o82.isMemoryLoad() && !o82.isMove() && !o82.isCall() &&
        !o82.isIndirectCall() && !o82.isReturn() && !o82.isJump() &&
        !o82.isConditionalJump() && !o82.isUnconditionalJump() &&
        !o82.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

      ufAssert( o92.getSize() == 4 );
      ufAssert(
        !o92.isImplicitMemoryAccess() && o92.isMemoryStore() &&
        !o92.isMemoryLoad() && !o92.isMove() && !o92.isCall() &&
        !o92.isIndirectCall() && !o92.isReturn() && !o92.isJump() &&
        !o92.isConditionalJump() && !o92.isUnconditionalJump() &&
        !o92.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

      ufAssert( o102.getSize() == 4 );
      ufAssert(
        !o102.isImplicitMemoryAccess() && o102.isMemoryStore() &&
        !o102.isMemoryLoad() && !o102.isMove() && !o102.isCall() &&
        !o102.isIndirectCall() && !o102.isReturn() && !o102.isJump() &&
        !o102.isConditionalJump() && !o102.isUnconditionalJump() &&
        !o102.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

      ufAssert( o112.getSize() == 4 );
      ufAssert(
        !o112.isImplicitMemoryAccess() && o112.isMemoryStore() &&
        !o112.isMemoryLoad() && !o112.isMove() && !o112.isCall() &&
        !o112.isIndirectCall() && !o112.isReturn() && !o112.isJump() &&
        !o112.isConditionalJump() && !o112.isUnconditionalJump() &&
        !o112.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

      ufAssert( o122.getSize() == 4 );
      ufAssert(
        !o122.isImplicitMemoryAccess() && o122.isMemoryStore() &&
        !o122.isMemoryLoad() && !o122.isMove() && !o122.isCall() &&
        !o122.isIndirectCall() && !o122.isReturn() && !o122.isJump() &&
        !o122.isConditionalJump() && !o122.isUnconditionalJump() &&
        !o122.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

      ufAssert( o132.getSize() == 4 );
      ufAssert(
        !o132.isImplicitMemoryAccess() && o132.isMemoryStore() &&
        !o132.isMemoryLoad() && !o132.isMove() && !o132.isCall() &&
        !o132.isIndirectCall() && !o132.isReturn() && !o132.isJump() &&
        !o132.isConditionalJump() && !o132.isUnconditionalJump() &&
        !o132.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

      ufAssert( o142.getSize() == 4 );
      ufAssert(
        !o142.isImplicitMemoryAccess() && o142.isMemoryStore() &&
        !o142.isMemoryLoad() && !o142.isMove() && !o142.isCall() &&
        !o142.isIndirectCall() && !o142.isReturn() && !o142.isJump() &&
        !o142.isConditionalJump() && !o142.isUnconditionalJump() &&
        !o142.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

      ufAssert( o152.getSize() == 4 );
      ufAssert(
        !o152.isImplicitMemoryAccess() && o152.isMemoryStore() &&
        !o152.isMemoryLoad() && !o152.isMove() && !o152.isCall() &&
        !o152.isIndirectCall() && !o152.isReturn() && !o152.isJump() &&
        !o152.isConditionalJump() && !o152.isUnconditionalJump() &&
        !o152.isIndirectJump() );

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

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
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

      ufAssert( o162.getSize() == 4 );
      ufAssert(
        !o162.isImplicitMemoryAccess() && o162.isMemoryStore() &&
        !o162.isMemoryLoad() && !o162.isMove() && !o162.isCall() &&
        !o162.isIndirectCall() && !o162.isReturn() && !o162.isJump() &&
        !o162.isConditionalJump() && !o162.isUnconditionalJump() &&
        !o162.isIndirectJump() );
    }

  for ( auto &cond : conditions )
    for ( auto &format :
            vector<ARMv4::OperationFormat> {
              ARMv4::OperationFormat::CR_1,
              ARMv4::OperationFormat::CR_2 } ) {
      WIR_Operation o(
        ARMv4::OpCode::MRS, format, WIR_ConditionFieldParameter( cond ),
        new WIR_RegisterParameter( r1, WIR_Usage::def ) );

      ufAssert( o.getSize() == 4 );
      ufAssert(
        !o.isImplicitMemoryAccess() && !o.isMemoryStore() && !o.isMemoryLoad() &&
        !o.isMove() && !o.isCall() && !o.isIndirectCall() && !o.isReturn() &&
        !o.isJump() && !o.isConditionalJump() && !o.isUnconditionalJump() &&
        !o.isIndirectJump() );
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

      ufAssert( o.getSize() == 4 );
      ufAssert(
        !o.isImplicitMemoryAccess() && !o.isMemoryStore() && !o.isMemoryLoad() &&
        !o.isMove() && !o.isCall() && !o.isIndirectCall() && !o.isReturn() &&
        !o.isJump() && !o.isConditionalJump() && !o.isUnconditionalJump() &&
        !o.isIndirectJump() );
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

      ufAssert( o.getSize() == 4 );
      ufAssert(
        !o.isImplicitMemoryAccess() && !o.isMemoryStore() && !o.isMemoryLoad() &&
        !o.isMove() && !o.isCall() && !o.isIndirectCall() && !o.isReturn() &&
        !o.isJump() && !o.isConditionalJump() && !o.isUnconditionalJump() &&
        !o.isIndirectJump() );
    }
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o(
      ARMv4::OpCode::SWI, ARMv4::OperationFormat::CC24,
      new WIR_ConditionFieldParameter( cond ),
      new ARM_Const24_Unsigned( 42 ) );

    ufAssert( o.getSize() == 4 );
    ufAssert(
      !o.isImplicitMemoryAccess() && !o.isMemoryStore() && !o.isMemoryLoad() &&
      !o.isMove() && !o.isCall() && o.isIndirectCall() && !o.isReturn() &&
      !o.isJump() && !o.isConditionalJump() && !o.isUnconditionalJump() &&
      !o.isIndirectJump() );
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o1(
      ARMv4::OpCode::SWP, ARMv4::OperationFormat::CRRR_5,
      new WIR_ConditionFieldParameter( cond ),
      new WIR_RegisterParameter( r1, WIR_Usage::def ),
      new WIR_RegisterParameter( r2, WIR_Usage::use ),
      new WIR_RegisterParameter( r4, WIR_Usage::use ) );

    ufAssert( o1.getSize() == 4 );
    ufAssert(
      !o1.isImplicitMemoryAccess() && o1.isMemoryStore() && o1.isMemoryLoad() &&
      !o1.isMove() && !o1.isCall() && !o1.isIndirectCall() && !o1.isReturn() &&
      !o1.isJump() && !o1.isConditionalJump() && !o1.isUnconditionalJump() &&
      !o1.isIndirectJump() );

    WIR_Operation o2(
      ARMv4::OpCode::SWPB, ARMv4::OperationFormat::CRRR_5,
      new WIR_ConditionFieldParameter( cond ),
      new WIR_RegisterParameter( r1, WIR_Usage::def ),
      new WIR_RegisterParameter( r2, WIR_Usage::use ),
      new WIR_RegisterParameter( r4, WIR_Usage::use ) );

    ufAssert( o2.getSize() == 4 );
    ufAssert(
      !o2.isImplicitMemoryAccess() && o2.isMemoryStore() && o2.isMemoryLoad() &&
      !o2.isMove() && !o2.isCall() && !o2.isIndirectCall() && !o2.isReturn() &&
      !o2.isJump() && !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
      !o2.isIndirectJump() );
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

      ufAssert( o1.getSize() == 4 );
      ufAssert(
        !o1.isImplicitMemoryAccess() && !o1.isMemoryStore() &&
        !o1.isMemoryLoad() && !o1.isMove() && !o1.isCall() &&
        !o1.isIndirectCall() && !o1.isReturn() && !o1.isJump() &&
        !o1.isConditionalJump() && !o1.isUnconditionalJump() &&
        !o1.isIndirectJump() );

      WIR_Operation o2(
        ARMv4::OpCode::MRC, ARMv4::OperationFormat::CAORSSO_1,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( proc ),
        new ARM_Const3_CoprocessorOpcode( 0 ),
        new WIR_RegisterParameter( p.R9(), WIR_Usage::def ),
        new WIR_StringParameter( "c9" ),
        new WIR_StringParameter( "c5" ),
        new ARM_Const3_CoprocessorOpcode( 1 ) );

      ufAssert( o2.getSize() == 4 );
      ufAssert(
        !o2.isImplicitMemoryAccess() && !o2.isMemoryStore() &&
        !o2.isMemoryLoad() && !o2.isMove() && !o2.isCall() &&
        !o2.isIndirectCall() && !o2.isReturn() && !o2.isJump() &&
        !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
        !o2.isIndirectJump() );

      WIR_Operation o3(
        ARMv4::OpCode::CDP, ARMv4::OperationFormat::CAOSSSO,
        new WIR_ConditionFieldParameter( cond ),
        new WIR_AddressingModeParameter( proc ),
        new ARM_Const4_CoprocessorOpcode( 15 ),
        new WIR_StringParameter( "c9" ),
        new WIR_StringParameter( "c5" ),
        new WIR_StringParameter( "c6" ),
        new ARM_Const3_CoprocessorOpcode( 1 ) );

      ufAssert( o3.getSize() == 4 );
      ufAssert(
        !o3.isImplicitMemoryAccess() && !o3.isMemoryStore() &&
        !o3.isMemoryLoad() && !o3.isMove() && !o3.isCall() &&
        !o3.isIndirectCall() && !o3.isReturn() && !o3.isJump() &&
        !o3.isConditionalJump() && !o3.isUnconditionalJump() &&
        !o3.isIndirectJump() );

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

            ufAssert( o.getSize() == 4 );
            if ( opcode == ARMv4::OpCode::LDC )
              ufAssert(
                !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
                o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
                !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
                !o.isConditionalJump() && !o.isUnconditionalJump() &&
                !o.isIndirectJump() );
            else
              ufAssert(
                !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
                !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
                !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
                !o.isConditionalJump() && !o.isUnconditionalJump() &&
                !o.isIndirectJump() );
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

            ufAssert( o.getSize() == 4 );
            if ( opcode == ARMv4::OpCode::LDC )
              ufAssert(
                !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
                o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
                !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
                !o.isConditionalJump() && !o.isUnconditionalJump() &&
                !o.isIndirectJump() );
            else
              ufAssert(
                !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
                !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
                !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
                !o.isConditionalJump() && !o.isUnconditionalJump() &&
                !o.isIndirectJump() );
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

            ufAssert( o.getSize() == 4 );
            if ( opcode == ARMv4::OpCode::LDC )
              ufAssert(
                !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
                o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
                !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
                !o.isConditionalJump() && !o.isUnconditionalJump() &&
                !o.isIndirectJump() );
            else
              ufAssert(
                !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
                !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
                !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
                !o.isConditionalJump() && !o.isUnconditionalJump() &&
                !o.isIndirectJump() );
          }
        }
      }
    }

  return( 0 );
}
