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
#include <arch/arm/armv5t.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  ARMv5T p;
  const ARM_HiRegP &r8 = p.R8(), &r11 = p.R11();
  WIR_Function f( "main" );

  // The following operations must be accepted for the ARMv5T ISA.
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

  WIR_Operation o1(
    ARMv5T::OpCode::BKPT, ARMv5T::OperationFormat::C16_1,
    ARM_Const16_Unsigned( 65535 ) );

  ufAssert( o1.getSize() == 4 );
  ufAssert(
    !o1.isImplicitMemoryAccess() && !o1.isMemoryStore() && !o1.isMemoryLoad() &&
    !o1.isMove() && !o1.isCall() && !o1.isIndirectCall() && !o1.isReturn() &&
    o1.isJump() && !o1.isConditionalJump() && !o1.isUnconditionalJump() &&
    o1.isIndirectJump() );

  WIR_Operation o2(
    ARMv5T::OpCode::BLX, ARMv5T::OperationFormat::L,
    WIR_LabelParameter( f ) );

  ufAssert( o2.getSize() == 4 );
  ufAssert(
    !o2.isImplicitMemoryAccess() && !o2.isMemoryStore() && !o2.isMemoryLoad() &&
    !o2.isMove() && o2.isCall() && !o2.isIndirectCall() && !o2.isReturn() &&
    !o2.isJump() && !o2.isConditionalJump() && !o2.isUnconditionalJump() &&
    !o2.isIndirectJump() );

  for ( auto &cond : conditions ) {
    WIR_Operation o(
      ARMv5T::OpCode::BLX, ARMv5T::OperationFormat::CR_3,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::use ) );

    ufAssert( o.getSize() == 4 );
    ufAssert(
      !o.isImplicitMemoryAccess() && !o.isMemoryStore() && !o.isMemoryLoad() &&
      !o.isMove() && o.isCall() && o.isIndirectCall() && !o.isReturn() &&
      !o.isJump() && !o.isConditionalJump() && !o.isUnconditionalJump() &&
      !o.isIndirectJump() );
  }

  for ( auto &proc : coprocessors ) {
    WIR_Operation o(
      ARMv5T::OpCode::CDP2, ARMv5T::OperationFormat::AOSSSO,
      WIR_AddressingModeParameter( proc ),
      ARM_Const4_CoprocessorOpcode( 15 ),
      WIR_StringParameter( "c9" ),
      WIR_StringParameter( "c5" ),
      WIR_StringParameter( "c6" ),
      ARM_Const3_CoprocessorOpcode( 1 ) );

    ufAssert( o.getSize() == 4 );
    ufAssert(
      !o.isImplicitMemoryAccess() && !o.isMemoryStore() && !o.isMemoryLoad() &&
      !o.isMove() && !o.isCall() && !o.isIndirectCall() && !o.isReturn() &&
      !o.isJump() && !o.isConditionalJump() && !o.isUnconditionalJump() &&
      !o.isIndirectJump() );
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o(
      ARMv5T::OpCode::CLZ, ARMv5T::OperationFormat::CRR_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r11, WIR_Usage::use ) );

    ufAssert( o.getSize() == 4 );
    ufAssert(
      !o.isImplicitMemoryAccess() && !o.isMemoryStore() && !o.isMemoryLoad() &&
      !o.isMove() && !o.isCall() && !o.isIndirectCall() && !o.isReturn() &&
      !o.isJump() && !o.isConditionalJump() && !o.isUnconditionalJump() &&
      !o.isIndirectJump() );
  }

  for ( auto &proc : coprocessors ) {
    for ( auto &addsub : memoryAddSubModes ) {
      for ( auto &format : vector<ARMv4::OperationFormat> { ARMv5T::OperationFormat::ASRAC8_1,
                                                               ARMv5T::OperationFormat::ASRAC8_2 } ) {
        WIR_Operation o(
          ARMv5T::OpCode::LDC2, format,
          WIR_AddressingModeParameter( proc ),
          WIR_StringParameter( "c9" ),
          WIR_RegisterParameter( r8, WIR_Usage::use ),
          WIR_AddressingModeParameter( addsub ),
          ARM_Const10_CoprocessorOffset( 20 ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }

      for ( auto &prepost : memoryAddressingModes )
        for ( auto &format : vector<ARMv4::OperationFormat> { ARMv5T::OperationFormat::ASARAC8_1,
                                                                 ARMv5T::OperationFormat::ASARAC8_2 } ) {
          WIR_Operation o(
            ARMv5T::OpCode::LDC2, format,
            WIR_AddressingModeParameter( proc ),
            WIR_StringParameter( "c12" ),
            WIR_AddressingModeParameter( prepost ),
            WIR_RegisterParameter( p.R9(), WIR_Usage::defuse ),
            WIR_AddressingModeParameter( addsub ),
            ARM_Const10_CoprocessorOffset( 44 ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
        }

      for ( auto &format : vector<ARMv4::OperationFormat> { ARMv5T::OperationFormat::ASRC8_1,
                                                               ARMv5T::OperationFormat::ASRC8_2 } ) {
        WIR_Operation o(
          ARMv5T::OpCode::LDC2, format,
          WIR_AddressingModeParameter( proc ),
          WIR_StringParameter( "c4" ),
          WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
          ARM_Const8_Unsigned( 255 ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && !o.isMemoryStore() &&
          o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }
    }
  }

  for ( auto &proc : coprocessors ) {
    WIR_Operation o(
      ARMv5T::OpCode::MCR2, ARMv5T::OperationFormat::AORSSO_2,
      WIR_AddressingModeParameter( proc ),
      ARM_Const3_CoprocessorOpcode( 0 ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      WIR_StringParameter( "c9" ),
      WIR_StringParameter( "c5" ),
      ARM_Const3_CoprocessorOpcode( 1 ) );

    ufAssert( o.getSize() == 4 );
    ufAssert(
      !o.isImplicitMemoryAccess() && !o.isMemoryStore() && !o.isMemoryLoad() &&
      !o.isMove() && !o.isCall() && !o.isIndirectCall() && !o.isReturn() &&
      !o.isJump() && !o.isConditionalJump() && !o.isUnconditionalJump() &&
      !o.isIndirectJump() );
  }

  for ( auto &proc : coprocessors ) {
    for ( auto &addsub : memoryAddSubModes ) {
      for ( auto &format : vector<ARMv4::OperationFormat> { ARMv5T::OperationFormat::ASRAC8_1,
                                                               ARMv5T::OperationFormat::ASRAC8_2 } ) {
        WIR_Operation o(
          ARMv5T::OpCode::STC2, format,
          WIR_AddressingModeParameter( proc ),
          WIR_StringParameter( "c9" ),
          WIR_RegisterParameter( r8, WIR_Usage::use ),
          WIR_AddressingModeParameter( addsub ),
          ARM_Const10_CoprocessorOffset( 20 ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }

      for ( auto &prepost : memoryAddressingModes )
        for ( auto &format : vector<ARMv4::OperationFormat> { ARMv5T::OperationFormat::ASARAC8_1,
                                                                 ARMv5T::OperationFormat::ASARAC8_2 } ) {
          WIR_Operation o(
            ARMv5T::OpCode::STC2, format,
            WIR_AddressingModeParameter( proc ),
            WIR_StringParameter( "c12" ),
            WIR_AddressingModeParameter( prepost ),
            WIR_RegisterParameter( p.R9(), WIR_Usage::defuse ),
            WIR_AddressingModeParameter( addsub ),
            ARM_Const10_CoprocessorOffset( 44 ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
        }

      for ( auto &format : vector<ARMv4::OperationFormat> { ARMv5T::OperationFormat::ASRC8_1,
                                                               ARMv5T::OperationFormat::ASRC8_2 } ) {
        WIR_Operation o(
          ARMv5T::OpCode::STC2, format,
          WIR_AddressingModeParameter( proc ),
          WIR_StringParameter( "c4" ),
          WIR_RegisterParameter( p.R9(), WIR_Usage::use ),
          ARM_Const8_Unsigned( 255 ) );

        ufAssert( o.getSize() == 4 );
        ufAssert(
          !o.isImplicitMemoryAccess() && o.isMemoryStore() &&
          !o.isMemoryLoad() && !o.isMove() && !o.isCall() &&
          !o.isIndirectCall() && !o.isReturn() && !o.isJump() &&
          !o.isConditionalJump() && !o.isUnconditionalJump() &&
          !o.isIndirectJump() );
      }
    }
  }

  WIR_Operation o3(
    ARMv5T::OpCode::BLX, ARMv4T::OperationFormat::TL_2,
    WIR_LabelParameter( f ) );

  ufAssert( o3.getSize() == 4 );
  ufAssert(
    !o3.isImplicitMemoryAccess() && !o3.isMemoryStore() && !o3.isMemoryLoad() &&
    !o3.isMove() && o3.isCall() && !o3.isIndirectCall() && !o3.isReturn() &&
    !o3.isJump() && !o3.isConditionalJump() && !o3.isUnconditionalJump() &&
    !o3.isIndirectJump() );

  WIR_Operation o4(
    ARMv5T::OpCode::BLX, ARMv5T::OperationFormat::TR_1,
    WIR_RegisterParameter( r8, WIR_Usage::use ) );

  ufAssert( o4.getSize() == 2 );
  ufAssert(
    !o4.isImplicitMemoryAccess() && !o4.isMemoryStore() && !o4.isMemoryLoad() &&
    !o4.isMove() && o4.isCall() && o4.isIndirectCall() && !o4.isReturn() &&
    !o4.isJump() && !o4.isConditionalJump() && !o4.isUnconditionalJump() &&
    !o4.isIndirectJump() );

  return( 0 );
}
