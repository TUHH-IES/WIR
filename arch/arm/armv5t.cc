/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armv5t.cc
  @brief This file implements the specific interface of the ARMv5T instruction
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
#include <arch/arm/armv5t.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for ARMv5T-based processor architectures.
*/
ARMv5T::ARMv5T( void ) :
  ARMv4T {}
{
  DSTART( "ARMv5T::ARMv5T()" );

  // Specify the processor architecture modeled by this class.
  setISAName( "ARMv5T" );
};


/*
  Copy constructor.
*/
ARMv5T::ARMv5T( const ARMv5T &__o ) :
  ARMv4T { __o }
{
  DSTART( "ARMv5T::ARMv5T(const ARMv5T&)" );
};


/*
  Move constructor.
*/
ARMv5T::ARMv5T( ARMv5T &&__o ) :
  ARMv4T { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
ARMv5T::~ARMv5T( void )
{
  DSTART( "virtual ARMv5T::~ARMv5T()" );
};


/*
  Copy-assignment operator.
*/
ARMv5T & ARMv5T::operator = ( const ARMv5T &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  ARMv4T::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
ARMv5T & ARMv5T::operator = ( ARMv5T &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  ARMv4T::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for ARMv5T-based processor
  architectures.

  This includes setting up the ARMv5T machine operation formats and the
  assignment of valid operation formats to ARMv5T opcodes.

  init shall be called globally by WIR_Init(). It shall only perform tasks that
  cannot be expressed as initializations of static class members (since the
  order of static initialization is unspecified in C++) and that thus require
  execution by active code.
*/
// cppcheck-suppress duplInheritedMember
void ARMv5T::init( void )
{
  DSTART( "static void ARMv5T::init()" );


  //
  // Register ARMv5T operation formats.
  //

  WIR_BasicBlock b;
  const ARM_RegV *regV = new ARM_RegV;

  registerOperationFormat(
    OperationFormat::AORSSO_2,
    { new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new ARM_Const3_CoprocessorOpcode( 1 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_StringParameter( "CRn" ),
      new WIR_StringParameter( "CRm" ),
      new ARM_Const3_CoprocessorOpcode( 2 ) } );

  registerOperationFormat(
    OperationFormat::AOSSSO,
    { new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new ARM_Const4_CoprocessorOpcode( 1 ),
      new WIR_StringParameter( "CRn" ),
      new WIR_StringParameter( "CRd" ),
      new WIR_StringParameter( "CRm" ),
      new ARM_Const3_CoprocessorOpcode( 2 ) } );

  registerOperationFormat(
    OperationFormat::ASARAC8_1,
    { new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new WIR_StringParameter( "CRd" ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const10_CoprocessorOffset( 0 ) } );

  registerOperationFormat(
    OperationFormat::ASARAC8_2,
    { new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new WIR_StringParameter( "CRd" ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const10_CoprocessorOffset( 0 ) } );

  registerOperationFormat(
    OperationFormat::ASRAC8_1,
    { new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new WIR_StringParameter( "CRd" ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const10_CoprocessorOffset( 0 ) } );

  registerOperationFormat(
    OperationFormat::ASRAC8_2,
    { new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new WIR_StringParameter( "CRd" ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const10_CoprocessorOffset( 0 ) } );

  registerOperationFormat(
    OperationFormat::ASRC8_1,
    { new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new WIR_StringParameter( "CRd" ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const8_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::ASRC8_2,
    { new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new WIR_StringParameter( "CRd" ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const8_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::C16_1,
    { new ARM_Const16_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::L,
    { new WIR_LabelParameter( b ) } );


  //
  // ARMv5T opcode to operation format mapping.
  //

  registerOpCode( OpCode::BKPT, OperationFormat::C16_1 );
  registerOpCode( OpCode::BLX, OperationFormat::L );
  registerOpCode( OpCode::BLX, OperationFormat::CR_3 );
  registerOpCode( OpCode::CDP2, OperationFormat::AOSSSO );
  registerOpCode( OpCode::CLZ, OperationFormat::CRR_1 );
  registerOpCode( OpCode::LDC2, OperationFormat::ASRAC8_1 );
  registerOpCode( OpCode::LDC2, OperationFormat::ASRAC8_2 );
  registerOpCode( OpCode::LDC2, OperationFormat::ASARAC8_1 );
  registerOpCode( OpCode::LDC2, OperationFormat::ASARAC8_2 );
  registerOpCode( OpCode::LDC2, OperationFormat::ASRC8_1 );
  registerOpCode( OpCode::LDC2, OperationFormat::ASRC8_2 );
  registerOpCode( OpCode::MCR2, OperationFormat::AORSSO_2 );
  registerOpCode( OpCode::STC2, OperationFormat::ASRAC8_1 );
  registerOpCode( OpCode::STC2, OperationFormat::ASRAC8_2 );
  registerOpCode( OpCode::STC2, OperationFormat::ASARAC8_1 );
  registerOpCode( OpCode::STC2, OperationFormat::ASARAC8_2 );
  registerOpCode( OpCode::STC2, OperationFormat::ASRC8_1 );
  registerOpCode( OpCode::STC2, OperationFormat::ASRC8_2 );

  registerOpCode( OpCode::BLX, OperationFormat::TL_2 );
  registerOpCode( OpCode::BLX, OperationFormat::TR_1 );


  //
  // Register this current processor model.
  //

  registerProcessor( ARMv5T() );
};


//
// Private class methods
//

/*
  clone creates a copy of a ARMv5T processor.
*/
WIR_BaseProcessor *ARMv5T::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* ARMv5T::clone() const" );

  return( new ARMv5T( *this ) );
};

}       // namespace WIR
