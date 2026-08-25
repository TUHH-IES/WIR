/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armv6.cc
  @brief This file implements the specific interface of the ARMv6 instruction
         set architecture.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/arm/armv6.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for ARMv6-based processor architectures.
*/
ARMv6::ARMv6( void ) :
  ARMv5TEJ {}
{
  DSTART( "ARMv6::ARMv6()" );

  // Specify the processor architecture modeled by this class.
  setISAName( "ARMv6" );
};


/*
  Copy constructor.
*/
ARMv6::ARMv6( const ARMv6 &__o ) :
  ARMv5TEJ { __o }
{
  DSTART( "ARMv6::ARMv6(const ARMv6&)" );
};


/*
  Move constructor.
*/
ARMv6::ARMv6( ARMv6 &&__o ) :
  ARMv5TEJ { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
ARMv6::~ARMv6( void )
{
  DSTART( "virtual ARMv6::~ARMv6()" );
};


/*
  Copy-assignment operator.
*/
ARMv6 & ARMv6::operator = ( const ARMv6 &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  ARMv5TEJ::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
ARMv6 & ARMv6::operator = ( ARMv6 &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  ARMv5TEJ::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for ARMv6-based processor
  architectures.

  This includes setting up the ARMv6 machine operation formats and the
  assignment of valid operation formats to ARMv6 opcodes.

  init shall be called globally by WIR_Init(). It shall only perform tasks that
  cannot be expressed as initializations of static class members (since the
  order of static initialization is unspecified in C++) and that thus require
  execution by active code.
*/
// cppcheck-suppress duplInheritedMember
void ARMv6::init( void )
{
  DSTART( "static void ARMv6::init()" );


  //
  // Register ARMv6 operation formats.
  //

  const ARM_RegV *regV = new ARM_RegV;

  registerOperationFormat(
    OperationFormat::A_1,
    { new WIR_AddressingModeParameter( AddressingMode::cpsra ) } );

  registerOperationFormat(
    OperationFormat::A_2,
    WIR_OperationFormat {
      new WIR_AddressingModeParameter( AddressingMode::be ) } );

  registerOperationFormat(
    OperationFormat::AC5_1,
    { new WIR_AddressingModeParameter( AddressingMode::cpsra ),
      new ARM_Const5_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::AC5_2,
    { new WIR_AddressingModeParameter( AddressingMode::ia ),
      new ARM_Const5_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::AC5_3,
    { new WIR_AddressingModeParameter( AddressingMode::ia ),
      new ARM_Const5_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::AORRS_1,
    { new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new ARM_Const3_CoprocessorOpcode( 1 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_StringParameter( "CRm" ) } );

  registerOperationFormat(
    OperationFormat::AORRS_2,
    { new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new ARM_Const3_CoprocessorOpcode( 1 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_StringParameter( "CRm" ) } );

  registerOperationFormat(
    OperationFormat::AR_1,
    { new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::AR_2,
    { new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ) } );

  registerOperationFormat(
    OperationFormat::C5_1,
    { new ARM_Const5_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRC4R_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new ARM_Const4_CoprocessorOpcode( 1 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRC5SPR_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new ARM_Const5_SatPos( 1 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRC60R_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new ARM_Const6_Unsigned0( 32 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRC5RC5_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new ARM_Const5_Unsigned0( 31 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRC5RC60_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new ARM_Const5_Unsigned0( 31 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const6_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRC60RC5_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new ARM_Const6_Unsigned0( 32 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRC60RC60_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new ARM_Const6_Unsigned0( 32 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const6_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRR_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRA_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ror0 ) } );

  registerOperationFormat(
    OperationFormat::CRRRA_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ror0 ) } );

  registerOperationFormat(
    OperationFormat::CRRRC5_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::CRRRR_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TA_1,
    { new WIR_AddressingModeParameter( AddressingMode::cpsra ) } );

  registerOperationFormat(
    OperationFormat::TA_2,
    { new WIR_AddressingModeParameter( AddressingMode::be ) } );


  //
  // ARMv6 opcode to operation format mapping.
  //

  registerOpCode( OpCode::CPS, OperationFormat::C5_1 );
  registerOpCode( OpCode::CPSID, OperationFormat::A_1 );
  registerOpCode( OpCode::CPSID, OperationFormat::AC5_1 );
  registerOpCode( OpCode::CPSID, OperationFormat::TA_1 );
  registerOpCode( OpCode::CPSIE, OperationFormat::A_1 );
  registerOpCode( OpCode::CPSIE, OperationFormat::AC5_1 );
  registerOpCode( OpCode::CPSIE, OperationFormat::TA_1 );
  registerOpCode( OpCode::CPY, OperationFormat::CRR_1 );
  registerOpCode( OpCode::CPY, OperationFormat::TRR_2 );
  registerOpCode( OpCode::LDREX, OperationFormat::CRR_7 );
  registerOpCode( OpCode::MCRR2, OperationFormat::AORRS_2 );
  registerOpCode( OpCode::MRRC2, OperationFormat::AORRS_1 );
  registerOpCode( OpCode::PKHBT, OperationFormat::CRRRC5_1 );
  registerOpCode( OpCode::PKHTB, OperationFormat::CRRRC5_3 );
  registerOpCode( OpCode::QADD16, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::QADD8, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::QADDSUBX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::QSUB16, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::QSUB8, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::QSUBADDX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::REV, OperationFormat::CRR_1 );
  registerOpCode( OpCode::REV, OperationFormat::TRR_1 );
  registerOpCode( OpCode::REV16, OperationFormat::CRR_1 );
  registerOpCode( OpCode::REV16, OperationFormat::TRR_1 );
  registerOpCode( OpCode::REVSH, OperationFormat::CRR_1 );
  registerOpCode( OpCode::REVSH, OperationFormat::TRR_1 );
  registerOpCode( OpCode::RFE, OperationFormat::AR_1 );
  registerOpCode( OpCode::RFE, OperationFormat::AR_2 );
  registerOpCode( OpCode::SADD16, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SADD8, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SADDSUBX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SEL, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SETEND, OperationFormat::A_2 );
  registerOpCode( OpCode::SETEND, OperationFormat::TA_2 );
  registerOpCode( OpCode::SHADD16, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SHADD8, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SHADDSUBX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SHSUB16, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SHSUB8, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SHSUBADDX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SMLAD, OperationFormat::CRRRR_1 );
  registerOpCode( OpCode::SMLADX, OperationFormat::CRRRR_1 );
  registerOpCode( OpCode::SMLALD, OperationFormat::CRRRR_5 );
  registerOpCode( OpCode::SMLALDX, OperationFormat::CRRRR_5 );
  registerOpCode( OpCode::SMLSD, OperationFormat::CRRRR_1 );
  registerOpCode( OpCode::SMLSDX, OperationFormat::CRRRR_1 );
  registerOpCode( OpCode::SMLSLD, OperationFormat::CRRRR_5 );
  registerOpCode( OpCode::SMLSLDX, OperationFormat::CRRRR_5 );
  registerOpCode( OpCode::SMMLA, OperationFormat::CRRRR_1 );
  registerOpCode( OpCode::SMMLAR, OperationFormat::CRRRR_1 );
  registerOpCode( OpCode::SMMLS, OperationFormat::CRRRR_1 );
  registerOpCode( OpCode::SMMLSR, OperationFormat::CRRRR_1 );
  registerOpCode( OpCode::SMMUL, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SMMULR, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SMUAD, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SMUADX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SMUSD, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SMUSDX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SRS, OperationFormat::AC5_2 );
  registerOpCode( OpCode::SRS, OperationFormat::AC5_3 );
  registerOpCode( OpCode::SSAT, OperationFormat::CRC60R_1 );
  registerOpCode( OpCode::SSAT, OperationFormat::CRC60RC5_1 );
  registerOpCode( OpCode::SSAT, OperationFormat::CRC60RC60_1 );
  registerOpCode( OpCode::SSAT16, OperationFormat::CRC5SPR_1 );
  registerOpCode( OpCode::SSUB16, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SSUB8, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::SSUBADDX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::STREX, OperationFormat::CRRR_5 );
  registerOpCode( OpCode::SXTAB, OperationFormat::CRRRA_1 );
  registerOpCode( OpCode::SXTAB16, OperationFormat::CRRRA_1 );
  registerOpCode( OpCode::SXTAH, OperationFormat::CRRRA_1 );
  registerOpCode( OpCode::SXTB, OperationFormat::CRRA_1 );
  registerOpCode( OpCode::SXTB, OperationFormat::TRR_1 );
  registerOpCode( OpCode::SXTB16, OperationFormat::CRRA_1 );
  registerOpCode( OpCode::SXTH, OperationFormat::CRRA_1 );
  registerOpCode( OpCode::SXTH, OperationFormat::TRR_1 );
  registerOpCode( OpCode::UADD16, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UADD8, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UADDSUBX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UHADD16, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UHADD8, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UHADDSUBX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UHSUB16, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UHSUB8, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UHSUBADDX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UMAAL, OperationFormat::CRRRR_5 );
  registerOpCode( OpCode::UQADD16, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UQADD8, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UQADDSUBX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UQSUB16, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UQSUB8, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UQSUBADDX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::USAD8, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::USADA8, OperationFormat::CRRRR_1 );
  registerOpCode( OpCode::USAT, OperationFormat::CRC5RC5_1 );
  registerOpCode( OpCode::USAT, OperationFormat::CRC5RC60_1 );
  registerOpCode( OpCode::USAT16, OperationFormat::CRC4R_1 );
  registerOpCode( OpCode::USUB16, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::USUB8, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::USUBADDX, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::UXTAB, OperationFormat::CRRRA_1 );
  registerOpCode( OpCode::UXTAB16, OperationFormat::CRRRA_1 );
  registerOpCode( OpCode::UXTAH, OperationFormat::CRRRA_1 );
  registerOpCode( OpCode::UXTB, OperationFormat::CRRA_1 );
  registerOpCode( OpCode::UXTB, OperationFormat::TRR_1 );
  registerOpCode( OpCode::UXTB16, OperationFormat::CRRA_1 );
  registerOpCode( OpCode::UXTH, OperationFormat::CRRA_1 );
  registerOpCode( OpCode::UXTH, OperationFormat::TRR_1 );


  //
  // Register this current processor model.
  //

  registerProcessor( ARMv6() );
};


//
// Private class methods
//

/*
  clone creates a copy of a ARMv6 processor.
*/
WIR_BaseProcessor *ARMv6::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* ARMv6::clone() const" );

  return( new ARMv6( *this ) );
};

}       // namespace WIR
