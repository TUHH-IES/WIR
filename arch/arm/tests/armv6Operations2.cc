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
#include <arch/arm/armv6.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  ARMv6 p;
  const ARM_LoRegP &r3 = p.R3(), &r6 = p.R6();
  const ARM_HiRegP &r8 = p.R8(), &r11 = p.R11();

  cout.iword( WIR_Indentation() ) = 8;
  cout << arm;

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
  const vector<ARMv4::AddressingMode> endianess {
    ARMv4::AddressingMode::be,
    ARMv4::AddressingMode::le };
  const vector<ARMv4::AddressingMode> multipleModes1 {
    ARMv4::AddressingMode::ia,
    ARMv4::AddressingMode::ib,
    ARMv4::AddressingMode::da,
    ARMv4::AddressingMode::db,
    ARMv4::AddressingMode::fd,
    ARMv4::AddressingMode::ed,
    ARMv4::AddressingMode::fa,
    ARMv4::AddressingMode::ea };
  const vector<ARMv4::AddressingMode> multipleModes2 {
    ARMv4::AddressingMode::ia,
    ARMv4::AddressingMode::ib,
    ARMv4::AddressingMode::da,
    ARMv4::AddressingMode::db };
  const vector<ARMv4::AddressingMode> processorStateBits {
    ARMv4::AddressingMode::cpsra,
    ARMv4::AddressingMode::cpsrf,
    ARMv4::AddressingMode::cpsri,
    ARMv4::AddressingMode::cpsraf,
    ARMv4::AddressingMode::cpsrai,
    ARMv4::AddressingMode::cpsrfi,
    ARMv4::AddressingMode::cpsrafi };

  // The following operations must be accepted for the ARMv6 ISA.
  WIR_Operation o1(
    ARMv6::OpCode::CPS, ARMv6::OperationFormat::C5_1,
    ARM_Const5_Unsigned( 31 ) );
  cout << o1 << endl;

  for ( auto &opcode : vector<ARMv6::OpCode> { ARMv6::OpCode::CPSID,
                                               ARMv6::OpCode::CPSIE } )
    for ( auto &state : processorStateBits ) {
      WIR_Operation o1(
        opcode, ARMv6::OperationFormat::A_1,
        WIR_AddressingModeParameter( state ) );
      cout << o1 << endl;

      WIR_Operation o2(
        opcode, ARMv6::OperationFormat::AC5_1,
        WIR_AddressingModeParameter( state ),
        ARM_Const5_Unsigned( 31 ) );
      cout << o2 << endl;

      WIR_Operation o3(
        opcode, ARMv6::OperationFormat::TA_1,
        WIR_AddressingModeParameter( state ) );
      cout << o3 << endl;
    }

  for ( auto &cond : conditions ) {
    WIR_Operation o(
      ARMv6::OpCode::CPY, ARMv4::OperationFormat::CRR_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::use ) );
    cout << o << endl;
  }

  WIR_Operation o2(
    ARMv6::OpCode::CPY, ARMv4T::OperationFormat::TRR_2,
    WIR_RegisterParameter( r8, WIR_Usage::def ),
    WIR_RegisterParameter( r3, WIR_Usage::use ) );
  cout << o2 << endl;

  for ( auto &cond : conditions ) {
    WIR_Operation o(
      ARMv6::OpCode::LDREX, ARMv6::OperationFormat::CRR_7,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::use ) );
    cout << o << endl;
  }

  for ( auto &proc : coprocessors ) {
    WIR_Operation o(
      ARMv6::OpCode::MCRR2, ARMv6::OperationFormat::AORRS_2,
      WIR_AddressingModeParameter( proc ),
      ARM_Const3_CoprocessorOpcode( 0 ),
      WIR_RegisterParameter( r8, WIR_Usage::use ),
      WIR_RegisterParameter( r3, WIR_Usage::use ),
      WIR_StringParameter( "c9" ) );
    cout << o << endl;
  }

  for ( auto &proc : coprocessors ) {
    WIR_Operation o(
      ARMv6::OpCode::MRRC2, ARMv6::OperationFormat::AORRS_1,
      WIR_AddressingModeParameter( proc ),
      ARM_Const3_CoprocessorOpcode( 0 ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::def ),
      WIR_StringParameter( "c9" ) );
    cout << o << endl;
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o(
      ARMv6::OpCode::PKHBT, ARMv6::OperationFormat::CRRRC5_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::use ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      ARM_Const5_Unsigned( 31 ) );
    cout << o << endl;
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o(
      ARMv6::OpCode::PKHTB, ARMv6::OperationFormat::CRRRC5_3,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::use ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      ARM_Const5_Unsigned( 31 ) );
    cout << o << endl;
  }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::QADD16,
                                                 ARMv6::OpCode::QADD8,
                                                 ARMv6::OpCode::QADDSUBX,
                                                 ARMv6::OpCode::QSUB16,
                                                 ARMv6::OpCode::QSUB8,
                                                 ARMv6::OpCode::QSUBADDX } )
    for ( auto &cond : conditions ) {
      WIR_Operation o(
        opcode, ARMv5TE::OperationFormat::CRRR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r11, WIR_Usage::use ),
        WIR_RegisterParameter( r3, WIR_Usage::use ) );
      cout << o << endl;
    }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::REV,
                                                 ARMv6::OpCode::REV16,
                                                 ARMv6::OpCode::REVSH } )
    for ( auto &cond : conditions ) {
      WIR_Operation o1(
        opcode, ARMv4::OperationFormat::CRR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r3, WIR_Usage::use ) );
      cout << o1 << endl;

      WIR_Operation o2(
        opcode, ARMv4T::OperationFormat::TRR_1,
        WIR_RegisterParameter( r6, WIR_Usage::def ),
        WIR_RegisterParameter( r3, WIR_Usage::use ) );
      cout << o2 << endl;
    }

  for ( auto &mode : multipleModes1 ) {
    WIR_Operation o1(
      ARMv6::OpCode::RFE, ARMv6::OperationFormat::AR_1,
      WIR_AddressingModeParameter( mode ),
      WIR_RegisterParameter( r3, WIR_Usage::use ) );
    cout << o1 << endl;

    WIR_Operation o2(
      ARMv6::OpCode::RFE, ARMv6::OperationFormat::AR_2,
      WIR_AddressingModeParameter( mode ),
      WIR_RegisterParameter( r3, WIR_Usage::defuse ) );
    cout << o2 << endl;
  }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::SADD16,
                                                 ARMv6::OpCode::SADD8,
                                                 ARMv6::OpCode::SADDSUBX,
                                                 ARMv6::OpCode::SEL } )
    for ( auto &cond : conditions ) {
      WIR_Operation o(
        opcode, ARMv5TE::OperationFormat::CRRR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r11, WIR_Usage::use ),
        WIR_RegisterParameter( r3, WIR_Usage::use ) );
      cout << o << endl;
    }

  for ( auto &mode : endianess ) {
    WIR_Operation o1(
      ARMv6::OpCode::SETEND, ARMv6::OperationFormat::A_2,
      WIR_AddressingModeParameter( mode ) );
    cout << o1 << endl;

    WIR_Operation o2(
      ARMv6::OpCode::SETEND, ARMv6::OperationFormat::TA_2,
      WIR_AddressingModeParameter( mode ) );
    cout << o2 << endl;
  }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::SHADD16,
                                                 ARMv6::OpCode::SHADD8,
                                                 ARMv6::OpCode::SHADDSUBX,
                                                 ARMv6::OpCode::SHSUB16,
                                                 ARMv6::OpCode::SHSUB8,
                                                 ARMv6::OpCode::SHSUBADDX } )
    for ( auto &cond : conditions ) {
      WIR_Operation o(
        opcode, ARMv5TE::OperationFormat::CRRR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r11, WIR_Usage::use ),
        WIR_RegisterParameter( r3, WIR_Usage::use ) );
      cout << o << endl;
    }

  for ( auto &cond : conditions ) {
    WIR_Operation o1(
      ARMv6::OpCode::SMLAD, ARMv6::OperationFormat::CRRRR_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::use ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o1 << endl;

    WIR_Operation o2(
      ARMv6::OpCode::SMLADX, ARMv6::OperationFormat::CRRRR_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::use ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o2 << endl;
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o1(
      ARMv6::OpCode::SMLALD, ARMv6::OperationFormat::CRRRR_5,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::def ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o1 << endl;

    WIR_Operation o2(
      ARMv6::OpCode::SMLALDX, ARMv6::OperationFormat::CRRRR_5,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::def ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o2 << endl;
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o1(
      ARMv6::OpCode::SMLSD, ARMv6::OperationFormat::CRRRR_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::use ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o1 << endl;

    WIR_Operation o2(
      ARMv6::OpCode::SMLSDX, ARMv6::OperationFormat::CRRRR_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::use ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o2 << endl;
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o1(
      ARMv6::OpCode::SMLSLD, ARMv6::OperationFormat::CRRRR_5,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::def ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o1 << endl;

    WIR_Operation o2(
      ARMv6::OpCode::SMLSLDX, ARMv6::OperationFormat::CRRRR_5,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::def ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o2 << endl;
  }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::SMMLA,
                                                 ARMv6::OpCode::SMMLAR,
                                                 ARMv6::OpCode::SMMLS,
                                                 ARMv6::OpCode::SMMLSR } )
    for ( auto &cond : conditions ) {
      WIR_Operation o(
        opcode, ARMv6::OperationFormat::CRRRR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r11, WIR_Usage::use ),
        WIR_RegisterParameter( r3, WIR_Usage::use ),
        WIR_RegisterParameter( r6, WIR_Usage::use ) );
      cout << o << endl;
    }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::SMMUL,
                                                 ARMv6::OpCode::SMMULR,
                                                 ARMv6::OpCode::SMUAD,
                                                 ARMv6::OpCode::SMUADX,
                                                 ARMv6::OpCode::SMUSD,
                                                 ARMv6::OpCode::SMUSDX } )
    for ( auto &cond : conditions ) {
      WIR_Operation o(
        opcode, ARMv6::OperationFormat::CRRR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r3, WIR_Usage::use ),
        WIR_RegisterParameter( r6, WIR_Usage::use ) );
      cout << o << endl;
    }

  for ( auto &mode : multipleModes2 ) {
    WIR_Operation o1(
      ARMv6::OpCode::SRS, ARMv6::OperationFormat::AC5_2,
      WIR_AddressingModeParameter( mode ),
      ARM_Const5_Unsigned( 31 ) );
    cout << o1 << endl;

    WIR_Operation o2(
      ARMv6::OpCode::SRS, ARMv6::OperationFormat::AC5_3,
      WIR_AddressingModeParameter( mode ),
      ARM_Const5_Unsigned( 31 ) );
    cout << o2 << endl;
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o1(
      ARMv6::OpCode::SSAT, ARMv6::OperationFormat::CRC60R_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      ARM_Const6_Unsigned0( 32 ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o1 << endl;

    WIR_Operation o2(
      ARMv6::OpCode::SSAT, ARMv6::OperationFormat::CRC60RC5_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      ARM_Const6_Unsigned0( 32 ),
      WIR_RegisterParameter( r6, WIR_Usage::use ),
      ARM_Const5_Unsigned( 14 ) );
    cout << o2 << endl;

    WIR_Operation o3(
      ARMv6::OpCode::SSAT, ARMv6::OperationFormat::CRC60RC60_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      ARM_Const6_Unsigned0( 32 ),
      WIR_RegisterParameter( r6, WIR_Usage::use ),
      ARM_Const6_Unsigned0( 14 ) );
    cout << o3 << endl;

    WIR_Operation o4(
      ARMv6::OpCode::SSAT16, ARMv6::OperationFormat::CRC5SPR_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      ARM_Const5_SatPos( 16 ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o4 << endl;
  }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::SSUB16,
                                                 ARMv6::OpCode::SSUB8,
                                                 ARMv6::OpCode::SSUBADDX } )
    for ( auto &cond : conditions ) {
      WIR_Operation o(
        opcode, ARMv5TE::OperationFormat::CRRR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r11, WIR_Usage::use ),
        WIR_RegisterParameter( r3, WIR_Usage::use ) );
      cout << o << endl;
    }

  for ( auto &cond : conditions ) {
    WIR_Operation o(
      ARMv6::OpCode::STREX, ARMv6::OperationFormat::CRRR_5,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o << endl;
  }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::SXTAB,
                                                 ARMv6::OpCode::SXTAB16,
                                                 ARMv6::OpCode::SXTAH } )
    for ( auto &mode : vector<ARMv4::AddressingMode> { ARMv4::AddressingMode::ror0,
                                                          ARMv4::AddressingMode::ror8,
                                                          ARMv4::AddressingMode::ror16,
                                                          ARMv4::AddressingMode::ror24 } )
      for ( auto &cond : conditions ) {
        WIR_Operation o(
          opcode, ARMv6::OperationFormat::CRRRA_1,
          WIR_ConditionFieldParameter( cond ),
          WIR_RegisterParameter( r8, WIR_Usage::def ),
          WIR_RegisterParameter( r11, WIR_Usage::use ),
          WIR_RegisterParameter( r3, WIR_Usage::use ),
          WIR_AddressingModeParameter( mode ) );
        cout << o << endl;
      }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::SXTB,
                                                 ARMv6::OpCode::SXTB16,
                                                 ARMv6::OpCode::SXTH } )
    for ( auto &mode : vector<ARMv4::AddressingMode> { ARMv4::AddressingMode::ror0,
                                                          ARMv4::AddressingMode::ror8,
                                                          ARMv4::AddressingMode::ror16,
                                                          ARMv4::AddressingMode::ror24 } )
      for ( auto &cond : conditions ) {
        WIR_Operation o(
          opcode, ARMv6::OperationFormat::CRRA_1,
          WIR_ConditionFieldParameter( cond ),
          WIR_RegisterParameter( r8, WIR_Usage::def ),
          WIR_RegisterParameter( r3, WIR_Usage::use ),
          WIR_AddressingModeParameter( mode ) );
        cout << o << endl;
      }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::SXTB,
                                                 ARMv6::OpCode::SXTH } ) {
    WIR_Operation o(
      opcode, ARMv4T::OperationFormat::TRR_1,
      WIR_RegisterParameter( r6, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::use ) );
    cout << o << endl;
  }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::UADD16,
                                                 ARMv6::OpCode::UADD8,
                                                 ARMv6::OpCode::UADDSUBX,
                                                 ARMv6::OpCode::UHADD16,
                                                 ARMv6::OpCode::UHADD8,
                                                 ARMv6::OpCode::UHADDSUBX,
                                                 ARMv6::OpCode::UHSUB16,
                                                 ARMv6::OpCode::UHSUB8,
                                                 ARMv6::OpCode::UHSUBADDX } )
    for ( auto &cond : conditions ) {
      WIR_Operation o(
        opcode, ARMv5TE::OperationFormat::CRRR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r11, WIR_Usage::use ),
        WIR_RegisterParameter( r3, WIR_Usage::use ) );
      cout << o << endl;
    }

  for ( auto &cond : conditions ) {
    WIR_Operation o(
      ARMv6::OpCode::UMAAL, ARMv6::OperationFormat::CRRRR_5,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::def ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o << endl;
  }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::UQADD16,
                                                 ARMv6::OpCode::UQADD8,
                                                 ARMv6::OpCode::UQADDSUBX,
                                                 ARMv6::OpCode::UQSUB16,
                                                 ARMv6::OpCode::UQSUB8,
                                                 ARMv6::OpCode::UQSUBADDX,
                                                 ARMv6::OpCode::USAD8 } )
    for ( auto &cond : conditions ) {
      WIR_Operation o(
        opcode, ARMv5TE::OperationFormat::CRRR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r11, WIR_Usage::use ),
        WIR_RegisterParameter( r3, WIR_Usage::use ) );
      cout << o << endl;
    }

  for ( auto &cond : conditions ) {
    WIR_Operation o(
      ARMv6::OpCode::USADA8, ARMv6::OperationFormat::CRRRR_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::use ),
      WIR_RegisterParameter( r11, WIR_Usage::use ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o << endl;
  }

  for ( auto &cond : conditions ) {
    WIR_Operation o1(
      ARMv6::OpCode::USAT, ARMv6::OperationFormat::CRC5RC5_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      ARM_Const5_Unsigned0( 31 ),
      WIR_RegisterParameter( r6, WIR_Usage::use ),
      ARM_Const5_Unsigned( 14 ) );
    cout << o1 << endl;

    WIR_Operation o2(
      ARMv6::OpCode::USAT, ARMv6::OperationFormat::CRC5RC60_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      ARM_Const5_Unsigned0( 27 ),
      WIR_RegisterParameter( r6, WIR_Usage::use ),
      ARM_Const6_Unsigned0( 14 ) );
    cout << o2 << endl;

    WIR_Operation o3(
      ARMv6::OpCode::USAT16, ARMv6::OperationFormat::CRC4R_1,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::def ),
      ARM_Const4_CoprocessorOpcode( 15 ),
      WIR_RegisterParameter( r6, WIR_Usage::use ) );
    cout << o3 << endl;
  }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::USUB16,
                                                 ARMv6::OpCode::USUB8,
                                                 ARMv6::OpCode::USUBADDX } )
    for ( auto &cond : conditions ) {
      WIR_Operation o(
        opcode, ARMv5TE::OperationFormat::CRRR_1,
        WIR_ConditionFieldParameter( cond ),
        WIR_RegisterParameter( r8, WIR_Usage::def ),
        WIR_RegisterParameter( r11, WIR_Usage::use ),
        WIR_RegisterParameter( r3, WIR_Usage::use ) );
      cout << o << endl;
    }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::UXTAB,
                                                 ARMv6::OpCode::UXTAB16,
                                                 ARMv6::OpCode::UXTAH } )
    for ( auto &mode : vector<ARMv4::AddressingMode> { ARMv4::AddressingMode::ror0,
                                                          ARMv4::AddressingMode::ror8,
                                                          ARMv4::AddressingMode::ror16,
                                                          ARMv4::AddressingMode::ror24 } )
      for ( auto &cond : conditions ) {
        WIR_Operation o(
          opcode, ARMv6::OperationFormat::CRRRA_1,
          WIR_ConditionFieldParameter( cond ),
          WIR_RegisterParameter( r8, WIR_Usage::def ),
          WIR_RegisterParameter( r11, WIR_Usage::use ),
          WIR_RegisterParameter( r3, WIR_Usage::use ),
          WIR_AddressingModeParameter( mode ) );
        cout << o << endl;
      }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::UXTB,
                                                 ARMv6::OpCode::UXTB16,
                                                 ARMv6::OpCode::UXTH } )
    for ( auto &mode : vector<ARMv4::AddressingMode> { ARMv4::AddressingMode::ror0,
                                                          ARMv4::AddressingMode::ror8,
                                                          ARMv4::AddressingMode::ror16,
                                                          ARMv4::AddressingMode::ror24 } )
      for ( auto &cond : conditions ) {
        WIR_Operation o(
          opcode, ARMv6::OperationFormat::CRRA_1,
          WIR_ConditionFieldParameter( cond ),
          WIR_RegisterParameter( r8, WIR_Usage::def ),
          WIR_RegisterParameter( r3, WIR_Usage::use ),
          WIR_AddressingModeParameter( mode ) );
        cout << o << endl;
      }

  for ( auto &opcode : vector<ARMv5TE::OpCode> { ARMv6::OpCode::UXTB,
                                                 ARMv6::OpCode::UXTH } ) {
    WIR_Operation o(
      opcode, ARMv4T::OperationFormat::TRR_1,
      WIR_RegisterParameter( r6, WIR_Usage::def ),
      WIR_RegisterParameter( r3, WIR_Usage::use ) );
    cout << o << endl;
  }

  return( 0 );
}
