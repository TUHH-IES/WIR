/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armv4.cc
  @brief This file implements the interface that is common for all ARM-based
         architectures.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <set>
#include <vector>

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/arm/armv4.h>
#include <arch/arm/armv4t.h>
#include <arch/arm/armv5t.h>
#include <arch/arm/armv5te.h>
#include <arch/arm/armv5tej.h>
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
  Default constructor for ARM-based processor architectures.
*/
ARMv4::ARMv4( void ) :
  WIR_Processor<ARMv4> {}
{
  DSTART( "ARMv4::ARMv4()" );

  // Specify the processor architecture modeled by this class.
  setProcessorName( "ARM" );
  setISAName( "ARMv4" );

  // Create physical registers.
  addPhReg<ARM_LoRegP>( "0" );
  addPhReg<ARM_LoRegP>( "1" );
  addPhReg<ARM_LoRegP>( "2" );
  addPhReg<ARM_LoRegP>( "3" );
  addPhReg<ARM_LoRegP>( "4" );
  addPhReg<ARM_LoRegP>( "5" );
  addPhReg<ARM_LoRegP>( "6" );
  addPhReg<ARM_LoRegP>( "7" );
  addPhReg<ARM_HiRegP>( "8" );
  addPhReg<ARM_HiRegP>( "9" );
  addPhReg<ARM_HiRegP>( "10" );
  addPhReg<ARM_HiRegP>( "11" );
  addPhReg<ARM_HiRegP>( "12" );
  addPhReg<ARM_HiRegP>( "13", true );
  addPhReg<ARM_HiRegP>( "14" );
  addPhReg<ARM_HiRegP>( "15" );
};


/*
  Copy constructor.
*/
ARMv4::ARMv4( const ARMv4 &__o ) :
  WIR_Processor<ARMv4> { __o }
{
  DSTART( "ARMv4::ARMv4(const ARMv4&)" );
};


/*
  Move constructor.
*/
ARMv4::ARMv4( ARMv4 &&__o ) :
  WIR_Processor<ARMv4> { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
ARMv4::~ARMv4( void )
{
  DSTART( "virtual ARMv4::~ARMv4()" );
};


/*
  Copy-assignment operator.
*/
ARMv4 & ARMv4::operator = ( const ARMv4 &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_Processor<ARMv4>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
ARMv4 & ARMv4::operator = ( ARMv4 &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_Processor<ARMv4>::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for ARM-based processor
  architectures.

  This includes setting up the common ARM machine operation formats and the
  assignment of valid operation formats to ARM opcodes.

  init shall be called globally by WIR_Init(). It shall only perform tasks that
  cannot be expressed as initializations of static class members (since the
  order of static initialization is unspecified in C++) and that thus require
  execution by active code.
*/
void ARMv4::init( void )
{
  DSTART( "static void ARMv4::init()" );

  //
  // Register ARM I/O functions.
  //

  registerBasicBlockDumper( getProcessorTypeID(), dumpARMBasicBlock );
  registerCompilationUnitDumper( getProcessorTypeID(), dumpARMCompilationUnit );
  registerDataDumper( getProcessorTypeID(), dumpARMData );
  registerDataSectionDumper( getProcessorTypeID(), dumpARMDataSection );
  registerFunctionDumper( getProcessorTypeID(), dumpARMFunction );
  registerLdScriptDumper( getProcessorTypeID(), dumpARMLdScript );
  registerLdScriptSectionDumper( getProcessorTypeID(), dumpARMLdScriptSection );
  registerOperationDumper( getProcessorTypeID(), dumpARMOperation );
  registerRegisterParameterDumper(
    getProcessorTypeID(), dumpARMRegisterParameter );
  registerCommentDumper( getProcessorTypeID(), dumpARMComment );
  registerFileInfoDumper( getProcessorTypeID(), dumpARMFileInfo );


  //
  // Register generic ARM operation formats.
  //

  WIR_BasicBlock b;
  const ARM_RegV *regV = new ARM_RegV;

  registerOperationFormat(
    OperationFormat::CAAAAC8RA_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new ARM_Const8_Unsigned( 0 ),
      new ARM_Const5_RotateAmount( 0 ) } );

  registerOperationFormat(
    OperationFormat::CAAAAC8RA_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new ARM_Const8_Unsigned( 0 ),
      new ARM_Const5_RotateAmount( 0 ) } );

  registerOperationFormat(
    OperationFormat::CAAAAR_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CAAAAR_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::c ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CAORSSO_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new ARM_Const3_CoprocessorOpcode( 1 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_StringParameter( "CRn" ),
      new WIR_StringParameter( "CRm" ),
      new ARM_Const3_CoprocessorOpcode( 2 ) } );

  registerOperationFormat(
    OperationFormat::CAORSSO_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new ARM_Const3_CoprocessorOpcode( 1 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_StringParameter( "CRn" ),
      new WIR_StringParameter( "CRm" ),
      new ARM_Const3_CoprocessorOpcode( 2 ) } );

  registerOperationFormat(
    OperationFormat::CAOSSSO,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new ARM_Const4_CoprocessorOpcode( 1 ),
      new WIR_StringParameter( "CRn" ),
      new WIR_StringParameter( "CRd" ),
      new WIR_StringParameter( "CRm" ),
      new ARM_Const3_CoprocessorOpcode( 2 ) } );

  registerOperationFormat(
    OperationFormat::CARR1_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR1_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR1_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR1_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR1_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR1_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR1_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR1_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR2_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR2_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR2_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR2_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR2_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR2_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR2_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR2_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR3_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR3_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR3_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR3_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR3_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR3_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR3_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR3_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR4_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR4_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR4_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR4_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR4_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR4_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR4_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR4_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR5_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR5_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR5_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR5_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR5_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR5_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR5_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR5_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR6_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR6_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR6_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR6_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR6_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR6_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR6_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR6_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR7_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR7_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR7_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR7_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR7_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR7_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR7_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR7_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR8_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR8_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR8_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR8_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR8_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR8_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR8_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR8_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR9_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR9_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR9_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR9_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR9_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR9_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR9_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR9_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR10_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR10_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR10_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR10_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR10_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR10_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR10_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR10_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR11_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR11_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR11_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR11_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR11_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR11_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR11_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR11_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR12_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR12_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR12_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR12_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR12_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR12_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR12_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR12_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR13_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR13_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR13_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR13_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR13_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR13_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR13_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR13_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR14_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR14_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR14_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR14_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR14_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR14_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR14_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR14_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR15_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR15_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR15_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR15_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR15_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR15_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR15_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR15_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR16_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR16_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR16_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR16_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CARR16_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR16_8,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CARR16_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::ea ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CASARAC10_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new WIR_StringParameter( "CRd" ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const10_CoprocessorOffset( 0 ) } );

  registerOperationFormat(
    OperationFormat::CASARAC10_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new WIR_StringParameter( "CRd" ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const10_CoprocessorOffset( 0 ) } );

  registerOperationFormat(
    OperationFormat::CASRAC10_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new WIR_StringParameter( "CRd" ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const10_CoprocessorOffset( 0 ) } );

  registerOperationFormat(
    OperationFormat::CASRAC10_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new WIR_StringParameter( "CRd" ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const10_CoprocessorOffset( 0 ) } );

  registerOperationFormat(
    OperationFormat::CASRC8_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new WIR_StringParameter( "CRd" ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const8_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CASRC8_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::p0 ),
      new WIR_StringParameter( "CRd" ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const8_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CC24,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new ARM_Const24_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::CL,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_LabelParameter( b ) } );

  registerOperationFormat(
    OperationFormat::CR_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CR_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::CRARAC12_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const12_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRARAC12_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const12_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRARAC8_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const8_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRARAC8_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const8_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRARAR_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRARAR_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRARAR_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRARAR_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRARARAC60_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsr ),
      new ARM_Const6_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRARARAC60_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsr ),
      new ARM_Const6_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRARARC50_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRARARC50_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRARARC5_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRARARC5_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::pre ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRC8RA_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new ARM_Const8_Unsigned( 0 ),
      new ARM_Const5_RotateAmount( 0 ) } );

  registerOperationFormat(
    OperationFormat::CRC8RA_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new ARM_Const8_Unsigned( 0 ),
      new ARM_Const5_RotateAmount( 0 ) } );

  registerOperationFormat(
    OperationFormat::CRC8RA_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const8_Unsigned( 0 ),
      new ARM_Const5_RotateAmount( 0 ) } );

  registerOperationFormat(
    OperationFormat::CRR_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRR_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRR_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRR_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRR_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRR_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRAC12_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const12_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRAC12_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const12_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRAC60_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsr ),
      new ARM_Const6_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRAC60_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsr ),
      new ARM_Const6_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRAC60_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsr ),
      new ARM_Const6_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRAC8_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const8_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRAC8_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new ARM_Const8_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRAR_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsl ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRAR_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsl ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRAR_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRAR_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRAR_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsl ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRAR_6,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRAR_7,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRARAC60_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsr ),
      new ARM_Const6_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRARAC60_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsr ),
      new ARM_Const6_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRARC50_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRARC50_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRARC5_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRARC5_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::plus ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRC50_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRC50_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRC50_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRC5_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::CRRC5_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::CRRC5_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::CRRC8RA_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const8_Unsigned( 0 ),
      new ARM_Const5_RotateAmount( 0 ) } );

  registerOperationFormat(
    OperationFormat::CRRC8RA_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const8_Unsigned( 0 ),
      new ARM_Const5_RotateAmount( 0 ) } );

  registerOperationFormat(
    OperationFormat::CRRR_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRR_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRR_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRR_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRR_5,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRRAC60_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsr ),
      new ARM_Const6_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRRAC60_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsr ),
      new ARM_Const6_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRRAR_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsl ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRRAR_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_AddressingModeParameter( ARMv4::AddressingMode::lsl ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRRC50_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRRC50_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::CRRRC5_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::CRRRC5_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::CRRRR_1,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRRR_2,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRRR_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::CRRRR_4,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );


  //
  // ARM opcode to operation format mapping.
  //

  for ( const auto &opcode : vector<OpCode> { OpCode::ADC, OpCode::ADD,
                                              OpCode::AND, OpCode::BIC,
                                              OpCode::EOR, OpCode::ORR,
                                              OpCode::RSB, OpCode::RSC,
                                              OpCode::SBC, OpCode::SUB } )
    for ( const auto &format : vector<OperationFormat> { OperationFormat::CRRC8RA_1,
                                                         OperationFormat::CRRC8RA_2,
                                                         OperationFormat::CRRR_1,
                                                         OperationFormat::CRRR_2,
                                                         OperationFormat::CRRR_3,
                                                         OperationFormat::CRRR_4,
                                                         OperationFormat::CRRRAR_1,
                                                         OperationFormat::CRRRAR_2,
                                                         OperationFormat::CRRRC5_1,
                                                         OperationFormat::CRRRC5_2,
                                                         OperationFormat::CRRRAC60_1,
                                                         OperationFormat::CRRRAC60_2,
                                                         OperationFormat::CRRRC50_1,
                                                         OperationFormat::CRRRC50_2 } )
      registerOpCode( opcode, format );

  registerOpCode( OpCode::B, OperationFormat::CL );
  registerOpCode( OpCode::BL, OperationFormat::CL );
  registerOpCode( OpCode::CDP, OperationFormat::CAOSSSO );

  for ( const auto &opcode : vector<OpCode> { OpCode::CMN, OpCode::CMP,
                                              OpCode::TEQ, OpCode::TST } )
    for ( const auto &format : vector<OperationFormat> { OperationFormat::CRC8RA_3,
                                                         OperationFormat::CRR_5,
                                                         OperationFormat::CRR_6,
                                                         OperationFormat::CRRAR_5,
                                                         OperationFormat::CRRC5_3,
                                                         OperationFormat::CRRAC60_3,
                                                         OperationFormat::CRRC50_3 } )
      registerOpCode( opcode, format );

  for ( const auto &opcode : vector<OpCode> { OpCode::LDC, OpCode::STC } )
    for ( const auto &format : vector<OperationFormat> { OperationFormat::CASRAC10_1,
                                                         OperationFormat::CASRAC10_2,
                                                         OperationFormat::CASARAC10_1,
                                                         OperationFormat::CASARAC10_2,
                                                         OperationFormat::CASRC8_1,
                                                         OperationFormat::CASRC8_2 } )
      registerOpCode( opcode, format );

  for ( const auto &format : vector<OperationFormat> { OperationFormat::CARR1_1,
                                                       OperationFormat::CARR1_2,
                                                       OperationFormat::CARR1_3,
                                                       OperationFormat::CARR1_4,
                                                       OperationFormat::CARR1_5,
                                                       OperationFormat::CARR2_1,
                                                       OperationFormat::CARR2_2,
                                                       OperationFormat::CARR2_3,
                                                       OperationFormat::CARR2_4,
                                                       OperationFormat::CARR2_5,
                                                       OperationFormat::CARR3_1,
                                                       OperationFormat::CARR3_2,
                                                       OperationFormat::CARR3_3,
                                                       OperationFormat::CARR3_4,
                                                       OperationFormat::CARR3_5,
                                                       OperationFormat::CARR4_1,
                                                       OperationFormat::CARR4_2,
                                                       OperationFormat::CARR4_3,
                                                       OperationFormat::CARR4_4,
                                                       OperationFormat::CARR4_5,
                                                       OperationFormat::CARR5_1,
                                                       OperationFormat::CARR5_2,
                                                       OperationFormat::CARR5_3,
                                                       OperationFormat::CARR5_4,
                                                       OperationFormat::CARR5_5,
                                                       OperationFormat::CARR6_1,
                                                       OperationFormat::CARR6_2,
                                                       OperationFormat::CARR6_3,
                                                       OperationFormat::CARR6_4,
                                                       OperationFormat::CARR6_5,
                                                       OperationFormat::CARR7_1,
                                                       OperationFormat::CARR7_2,
                                                       OperationFormat::CARR7_3,
                                                       OperationFormat::CARR7_4,
                                                       OperationFormat::CARR7_5,
                                                       OperationFormat::CARR8_1,
                                                       OperationFormat::CARR8_2,
                                                       OperationFormat::CARR8_3,
                                                       OperationFormat::CARR8_4,
                                                       OperationFormat::CARR8_5,
                                                       OperationFormat::CARR9_1,
                                                       OperationFormat::CARR9_2,
                                                       OperationFormat::CARR9_3,
                                                       OperationFormat::CARR9_4,
                                                       OperationFormat::CARR9_5,
                                                       OperationFormat::CARR10_1,
                                                       OperationFormat::CARR10_2,
                                                       OperationFormat::CARR10_3,
                                                       OperationFormat::CARR10_4,
                                                       OperationFormat::CARR10_5,
                                                       OperationFormat::CARR11_1,
                                                       OperationFormat::CARR11_2,
                                                       OperationFormat::CARR11_3,
                                                       OperationFormat::CARR11_4,
                                                       OperationFormat::CARR11_5,
                                                       OperationFormat::CARR12_1,
                                                       OperationFormat::CARR12_2,
                                                       OperationFormat::CARR12_3,
                                                       OperationFormat::CARR12_4,
                                                       OperationFormat::CARR12_5,
                                                       OperationFormat::CARR13_1,
                                                       OperationFormat::CARR13_2,
                                                       OperationFormat::CARR13_3,
                                                       OperationFormat::CARR13_4,
                                                       OperationFormat::CARR13_5,
                                                       OperationFormat::CARR14_1,
                                                       OperationFormat::CARR14_2,
                                                       OperationFormat::CARR14_3,
                                                       OperationFormat::CARR14_4,
                                                       OperationFormat::CARR14_5,
                                                       OperationFormat::CARR15_1,
                                                       OperationFormat::CARR15_2,
                                                       OperationFormat::CARR15_3,
                                                       OperationFormat::CARR15_4,
                                                       OperationFormat::CARR15_5,
                                                       OperationFormat::CARR16_1,
                                                       OperationFormat::CARR16_2,
                                                       OperationFormat::CARR16_3,
                                                       OperationFormat::CARR16_4 } )
    registerOpCode( OpCode::LDM, format );

  for ( const auto &opcode : vector<OpCode> { OpCode::LDR, OpCode::LDRB } )
    for ( const auto &format : vector<OperationFormat> { OperationFormat::CRRAC12_1,
                                                         OperationFormat::CRARAC12_1,
                                                         OperationFormat::CRRAR_3,
                                                         OperationFormat::CRARAR_1,
                                                         OperationFormat::CRRARC5_1,
                                                         OperationFormat::CRRARAC60_1,
                                                         OperationFormat::CRRARC50_1,
                                                         OperationFormat::CRRAR_4,
                                                         OperationFormat::CRARARC5_1,
                                                         OperationFormat::CRARARAC60_1,
                                                         OperationFormat::CRARARC50_1,
                                                         OperationFormat::CRARAR_2 } )
      registerOpCode( opcode, format );

  for ( const auto &opcode : vector<OpCode> { OpCode::LDRBT, OpCode::LDRT } )
    for ( const auto &format : vector<OperationFormat> { OperationFormat::CRARAC12_1,
                                                         OperationFormat::CRARAR_1,
                                                         OperationFormat::CRARARC5_1,
                                                         OperationFormat::CRARARAC60_1,
                                                         OperationFormat::CRARARC50_1,
                                                         OperationFormat::CRARAR_2 } )
      registerOpCode( opcode, format );

  for ( const auto &opcode : vector<OpCode> { OpCode::LDRH, OpCode::LDRSB,
                                              OpCode::LDRSH } )
    for ( const auto &format : vector<OperationFormat> { OperationFormat::CRRAC8_1,
                                                         OperationFormat::CRARAC8_1,
                                                         OperationFormat::CRRAR_3,
                                                         OperationFormat::CRARAR_1 } )
      registerOpCode( opcode, format );

  registerOpCode( OpCode::MCR, OperationFormat::CAORSSO_2 );

  registerOpCode( OpCode::MLA, OperationFormat::CRRRR_1 );
  registerOpCode( OpCode::MLA, OperationFormat::CRRRR_2 );

  for ( const auto &opcode : vector<OpCode> { OpCode::MOV, OpCode::MVN } )
    for ( const auto &format : vector<OperationFormat> { OperationFormat::CRC8RA_1,
                                                         OperationFormat::CRC8RA_2,
                                                         OperationFormat::CRR_1,
                                                         OperationFormat::CRR_2,
                                                         OperationFormat::CRR_3,
                                                         OperationFormat::CRR_4,
                                                         OperationFormat::CRRAR_1,
                                                         OperationFormat::CRRAR_2,
                                                         OperationFormat::CRRC5_1,
                                                         OperationFormat::CRRC5_2,
                                                         OperationFormat::CRRAC60_1,
                                                         OperationFormat::CRRAC60_2,
                                                         OperationFormat::CRRC50_1,
                                                         OperationFormat::CRRC50_2 } )
      registerOpCode( opcode, format );

  registerOpCode( OpCode::MRC, OperationFormat::CAORSSO_1 );

  registerOpCode( OpCode::MRS, OperationFormat::CR_1 );
  registerOpCode( OpCode::MRS, OperationFormat::CR_2 );
  registerOpCode( OpCode::MSR, OperationFormat::CAAAAC8RA_1 );
  registerOpCode( OpCode::MSR, OperationFormat::CAAAAC8RA_2 );
  registerOpCode( OpCode::MSR, OperationFormat::CAAAAR_1 );
  registerOpCode( OpCode::MSR, OperationFormat::CAAAAR_2 );
  registerOpCode( OpCode::MUL, OperationFormat::CRRR_1 );
  registerOpCode( OpCode::MUL, OperationFormat::CRRR_2 );

  for ( const auto &opcode : vector<OpCode> { OpCode::SMLAL, OpCode::SMULL,
                                              OpCode::UMLAL, OpCode::UMULL } )
    for ( const auto &format : vector<OperationFormat> { OperationFormat::CRRRR_3,
                                                         OperationFormat::CRRRR_4 } )
      registerOpCode( opcode, format );

  for ( const auto &format : vector<OperationFormat> { OperationFormat::CARR1_6,
                                                       OperationFormat::CARR1_7,
                                                       OperationFormat::CARR1_8,
                                                       OperationFormat::CARR2_6,
                                                       OperationFormat::CARR2_7,
                                                       OperationFormat::CARR2_8,
                                                       OperationFormat::CARR3_6,
                                                       OperationFormat::CARR3_7,
                                                       OperationFormat::CARR3_8,
                                                       OperationFormat::CARR4_6,
                                                       OperationFormat::CARR4_7,
                                                       OperationFormat::CARR4_8,
                                                       OperationFormat::CARR5_6,
                                                       OperationFormat::CARR5_7,
                                                       OperationFormat::CARR5_8,
                                                       OperationFormat::CARR6_6,
                                                       OperationFormat::CARR6_7,
                                                       OperationFormat::CARR6_8,
                                                       OperationFormat::CARR7_6,
                                                       OperationFormat::CARR7_7,
                                                       OperationFormat::CARR7_8,
                                                       OperationFormat::CARR8_6,
                                                       OperationFormat::CARR8_7,
                                                       OperationFormat::CARR8_8,
                                                       OperationFormat::CARR9_6,
                                                       OperationFormat::CARR9_7,
                                                       OperationFormat::CARR9_8,
                                                       OperationFormat::CARR10_6,
                                                       OperationFormat::CARR10_7,
                                                       OperationFormat::CARR10_8,
                                                       OperationFormat::CARR11_6,
                                                       OperationFormat::CARR11_7,
                                                       OperationFormat::CARR11_8,
                                                       OperationFormat::CARR12_6,
                                                       OperationFormat::CARR12_7,
                                                       OperationFormat::CARR12_8,
                                                       OperationFormat::CARR13_6,
                                                       OperationFormat::CARR13_7,
                                                       OperationFormat::CARR13_8,
                                                       OperationFormat::CARR14_6,
                                                       OperationFormat::CARR14_7,
                                                       OperationFormat::CARR14_8,
                                                       OperationFormat::CARR15_6,
                                                       OperationFormat::CARR15_7,
                                                       OperationFormat::CARR15_8,
                                                       OperationFormat::CARR16_6,
                                                       OperationFormat::CARR16_7,
                                                       OperationFormat::CARR16_8 } )
    registerOpCode( OpCode::STM, format );

  for ( const auto &opcode : vector<OpCode> { OpCode::STR, OpCode::STRB } )
    for ( const auto &format : vector<OperationFormat> { OperationFormat::CRRAC12_2,
                                                         OperationFormat::CRARAC12_2,
                                                         OperationFormat::CRRAR_6,
                                                         OperationFormat::CRARAR_3,
                                                         OperationFormat::CRRARC5_2,
                                                         OperationFormat::CRRARAC60_2,
                                                         OperationFormat::CRRARC50_2,
                                                         OperationFormat::CRRAR_7,
                                                         OperationFormat::CRARARC5_2,
                                                         OperationFormat::CRARARAC60_2,
                                                         OperationFormat::CRARARC50_2,
                                                         OperationFormat::CRARAR_4 } )
      registerOpCode( opcode, format );

  for ( const auto &opcode : vector<OpCode> { OpCode::STRBT, OpCode::STRT } )
    for ( const auto &format : vector<OperationFormat> { OperationFormat::CRARAC12_2,
                                                         OperationFormat::CRARAR_3,
                                                         OperationFormat::CRARARC5_2,
                                                         OperationFormat::CRARARAC60_2,
                                                         OperationFormat::CRARARC50_2,
                                                         OperationFormat::CRARAR_4 } )
      registerOpCode( opcode, format );

  registerOpCode( OpCode::STRH, OperationFormat::CRRAC8_2 );
  registerOpCode( OpCode::STRH, OperationFormat::CRARAC8_2 );
  registerOpCode( OpCode::STRH, OperationFormat::CRRAR_6 );
  registerOpCode( OpCode::STRH, OperationFormat::CRARAR_3 );

  registerOpCode( OpCode::SWI, OperationFormat::CC24 );

  registerOpCode( OpCode::SWP, OperationFormat::CRRR_5 );
  registerOpCode( OpCode::SWPB, OperationFormat::CRRR_5 );


  //
  // Register this current processor model.
  //

  registerProcessor( ARMv4() );


  //
  // Finally, initialize derived processor models.
  //

  ARMv4T::init();
  ARMv5T::init();
  ARMv5TE::init();
  ARMv5TEJ::init();
  ARMv6::init();
};


/*
  isCompatible returns whether one ARM addressing mode is compatible with
  another.

  This compatibility check is used by WIR_Operation::checkParameters in order to
  verify that addressing mode parameters inserted into some WIR operation
  actually match with the ARM operation formats.
*/
bool ARMv4::AddressingMode::isCompatible( const WIR_BaseProcessor::AddressingMode &t,
                                             const WIR_BaseProcessor::OperationFormat &f,
                                             const WIR_BaseProcessor::OpCode &o ) const
{
  DSTART(
    "virtual bool ARMv4::AddressingMode::isCompatible(const WIR_BaseProcessor::AddressingMode&, const WIR_BaseProcessor::OperationFormat&, const WIR_BaseProcessor::OpCode&) const" );

  // We first check that the provided addressing mode also belongs to the
  // correct processor architecture.
  if ( this->getProcessorTypeName() != t.getProcessorTypeName() )
    return( false );

  const set<AddressingMode> rightShiftModes {
    AddressingMode::lsr,
    AddressingMode::asr };
  const set<WIR_BaseProcessor::OpCode> onlyPostIndexedOps {
    ARMv4::OpCode::LDRBT,
    ARMv4::OpCode::LDRT,
    ARMv4::OpCode::STRBT,
    ARMv4::OpCode::STRT };

  // Addressing mode t stems from the operation format f.
  // Check for the very special uses of lsr and asr in some particular data-
  // processing and memory addressing modes.
  if ( ( ( f == ARMv4::OperationFormat::CRRRAC60_1 ) ||
         ( f == ARMv4::OperationFormat::CRRRAC60_2 ) ) &&
       !rightShiftModes.count( *this ) )
    return( false );

  if ( ( ( f == ARMv4::OperationFormat::CRRARAC60_1 ) ) &&
       rightShiftModes.count(
         dynamic_cast<const ARMv4::AddressingMode &>( t ) ) &&
       !rightShiftModes.count( *this ) )
    return( false );

  const set<AddressingMode> dataProcessingModes {
    AddressingMode::lsl,
    AddressingMode::lsr,
    AddressingMode::asr,
    AddressingMode::ror };
  const set<AddressingMode> memoryAddressingModes {
    AddressingMode::pre,
    AddressingMode::post };
  const set<AddressingMode> memoryAddSubModes {
    AddressingMode::plus,
    AddressingMode::minus };
  const set<AddressingMode> multipleModes1 {
    AddressingMode::ia,
    AddressingMode::ib,
    AddressingMode::da,
    AddressingMode::db,
    AddressingMode::fd,
    AddressingMode::ed,
    AddressingMode::fa,
    AddressingMode::ea };
  const set<AddressingMode> multipleModes2 {
    AddressingMode::ia,
    AddressingMode::ib,
    AddressingMode::da,
    AddressingMode::db };
  const set<AddressingMode> msrModes {
    AddressingMode::c,
    AddressingMode::x,
    AddressingMode::s,
    AddressingMode::f };
  const set<AddressingMode> coprocessors {
    AddressingMode::p0,
    AddressingMode::p1,
    AddressingMode::p2,
    AddressingMode::p3,
    AddressingMode::p4,
    AddressingMode::p5,
    AddressingMode::p6,
    AddressingMode::p7,
    AddressingMode::p8,
    AddressingMode::p9,
    AddressingMode::p10,
    AddressingMode::p11,
    AddressingMode::p12,
    AddressingMode::p13,
    AddressingMode::p14,
    AddressingMode::p15 };
  const set<AddressingMode> processorStateBits {
    AddressingMode::cpsra,
    AddressingMode::cpsrf,
    AddressingMode::cpsri,
    AddressingMode::cpsraf,
    AddressingMode::cpsrai,
    AddressingMode::cpsrfi,
    AddressingMode::cpsrafi };
  const set<AddressingMode> endianess {
    AddressingMode::be,
    AddressingMode::le };
  const set<AddressingMode> rotates {
    AddressingMode::ror0,
    AddressingMode::ror8,
    AddressingMode::ror16,
    AddressingMode::ror24 };

  // If a data-processing mode is required by the operation format but something
  // different is given (and analogously for memory addressing modes and
  // address addition/subtraction modes), return false.
  if ( ( dataProcessingModes.count(
           dynamic_cast<const ARMv4::AddressingMode &>( t ) ) &&
         !dataProcessingModes.count( *this ) ) ||
       ( memoryAddressingModes.count(
           dynamic_cast<const ARMv4::AddressingMode &>( t ) ) &&
         !memoryAddressingModes.count( *this ) ) ||
       ( memoryAddSubModes.count(
           dynamic_cast<const ARMv4::AddressingMode &>( t ) ) &&
         !memoryAddSubModes.count( *this ) ) ||
       ( msrModes.count(
           dynamic_cast<const ARMv4::AddressingMode &>( t ) ) &&
         !msrModes.count( *this ) ) ||
       ( multipleModes2.count(
           dynamic_cast<const ARMv4::AddressingMode &>( t ) ) &&
         !multipleModes2.count( *this ) ) ||
       ( multipleModes1.count(
           dynamic_cast<const ARMv4::AddressingMode &>( t ) ) &&
         !multipleModes1.count( *this ) ) ||
       ( coprocessors.count(
           dynamic_cast<const ARMv4::AddressingMode &>( t ) ) &&
         !coprocessors.count( *this ) ) ||
       ( processorStateBits.count(
           dynamic_cast<const ARMv4::AddressingMode &>( t ) ) &&
         !processorStateBits.count( *this ) ) ||
       ( endianess.count(
           dynamic_cast<const ARMv4::AddressingMode &>( t ) ) &&
         !endianess.count( *this ) ) ||
       ( rotates.count(
           dynamic_cast<const ARMv4::AddressingMode &>( t ) ) &&
         !rotates.count( *this ) ) )
    return( false );

  // If an operation with only post-indexed addressing is supplied with pre-
  // indexed addressing, return false.
  if ( onlyPostIndexedOps.count( o ) &&
       memoryAddressingModes.count(
         dynamic_cast<const ARMv4::AddressingMode &>( t ) ) &&
       ( *this != ARMv4::AddressingMode::post ) )
    return( false );

  return( true );
};


/*
  isCompatible returns whether one ARM register type is compatible with another.

  This compatibility check is used by WIR_Operation::checkParameters. It ensures
  that lo or hi registers can be used as register parameters whenever the ARM
  operation format requires general registers. However, it prohibits the use of
  hi registers whenever lo registers are required, i.e., in THUMB operations, or
  vice versa.
*/
bool ARMv4::RegisterType::isCompatible( const WIR_BaseProcessor::RegisterType &t,
                                           const WIR_BaseRegister &ra,
                                           const WIR_BaseRegister &rf ) const
{
  DSTART(
    "virtual bool ARMv4::RegisterType::isCompatible(const WIR_BaseProcessor::RegisterType&, const WIR_BaseRegister&, const WIR_BaseRegister&) const" );

  if ( rf.isPhysical() ) {
    // Special handling of physical registers required by an operation format.
    // We need to make sure that ra matches exactly the given physical register,
    // or that is precolored accordingly.
    if ( ra.isPhysical() ) {
      if ( ( ra.getName() == rf.getName() ) && ( *this == t ) )
        return( true );
      else
        return( false );
    }

    auto &vr = dynamic_cast<const WIR_VirtualRegister &>( ra );
    if ( vr.isPrecolored() && ( vr.getPrecolor().getName() == rf.getName() ) )
      return( true );
    else
      return( false );
  }

  // Register type t stems from some operation format.
  if ( t == ARMv4::RegisterType::reg )
    // Whenever the operation format requires a general-purpose register, any
    // ARM register type is legal.
    return( true );
  else
    // Otherwise, an exact match of the register types is necessary.
    return( *this == t );
};


/*
  isMove returns whether an opcode is a register-register move.
*/
bool ARMv4::OpCode::isMove( const WIR_Operation &o ) const
{
  DSTART( "virtual bool ARMv4::OpCode::isMove(const WIR_Operation&) const" );

  if ( ( ( *this == MOV ) &&
         ( ( ( ( o.getOperationFormat() == ARMv4::OperationFormat::CRR_1 ) ||
               ( o.getOperationFormat() == ARMv4::OperationFormat::CRR_2 ) ) &&
             ( dynamic_cast<const WIR_ConditionFieldParameter &>(
                 o.getParameters().front().get() ).getCondition() ==
                 ARMv4::Condition::al ) ) ||
           ( o.getOperationFormat() == ARMv4T::OperationFormat::TRR_1 ) ||
           ( o.getOperationFormat() == ARMv4T::OperationFormat::TRR_2 ) ) ) ||
       ( ( *this == ARMv6::OpCode::CPY ) &&
         ( ( o.getOperationFormat() == ARMv4T::OperationFormat::TRR_2 ) ||
           ( dynamic_cast<const WIR_ConditionFieldParameter &>(
               o.getParameters().front().get() ).getCondition() ==
               ARMv4::Condition::al ) ) ) )
    return( true );

  return( false );
};


/*
  isIndirectCall returns whether an opcode indirectly calls a function.
*/
bool ARMv4::OpCode::isIndirectCall( const WIR_Operation &o ) const
{
  DSTART(
    "virtual bool ARMv4::OpCode::isIndirectCall(const WIR_Operation&) const" );

  if ( ( *this == ARMv5T::OpCode::BLX ) &&
       ( ( o.getOperationFormat() == ARMv5T::OperationFormat::CR_3 ) ||
         ( o.getOperationFormat() == ARMv4T::OperationFormat::TR_1 ) ) )
    return( true );

  return( mIsIndirectCall );
};


/*
  isReturn returns whether an opcode returns from a function call.
*/
bool ARMv4::OpCode::isReturn( const WIR_Operation &o ) const
{
  DSTART(
    "virtual bool ARMv4::OpCode::isReturn(const WIR_Operation&) const" );

  if ( ( *this == ARMv4T::OpCode::BX ) &&
       ( o.getOperationFormat() == ARMv4T::OperationFormat::TR_1 ) ) {
    const WIR_BaseRegister &r =
      dynamic_cast<const WIR_RegisterParameter &>(
        o.getParameters().front().get() ).getRegister();

    if ( r.isPhysical() ) {
      if ( r.getName() == "r14" )
        return( true );
      else
        return( false );
    }

    const WIR_VirtualRegister &vr =
      dynamic_cast<const WIR_VirtualRegister &>( r );
    if ( vr.isPrecolored() && ( vr.getPrecolor().getName() == "r14" ) )
      return( true );
  } else

  if ( *this == ARMv6::OpCode::RFE )
    return( true );

  return( false );
};


/*
  isConditionalJump returns whether an opcode performs a conditional jump.
*/
bool ARMv4::OpCode::isConditionalJump( const WIR_Operation &o ) const
{
  DSTART(
    "virtual bool ARMv4::OpCode::isConditionalJump(const WIR_Operation&) const" );

  if ( *this == B ) {
    if ( o.getOperationFormat() == ARMv4T::OperationFormat::TL_1 )
      return( false );
    else

    if ( dynamic_cast<const WIR_ConditionFieldParameter &>(
           o.getParameters().front().get() ).getCondition() !=
           ARMv4::Condition::al )
    return( true );
  }

  return( false );
};


/*
  isUnconditionalJump returns whether an opcode performs an unconditional jump.
*/
bool ARMv4::OpCode::isUnconditionalJump( const WIR_Operation &o ) const
{
  DSTART(
    "virtual bool ARMv4::OpCode::isUnconditionalJump(const WIR_Operation&) const" );

  if ( *this == B ) {
    if ( o.getOperationFormat() == ARMv4T::OperationFormat::TL_1 )
      return( true );
    else

    if ( dynamic_cast<const WIR_ConditionFieldParameter &>(
           o.getParameters().front().get() ).getCondition() ==
           ARMv4::Condition::al )
    return( true );
  }

  return( false );
};


/*
  isIndirectJump returns whether an opcode indirectly performs a jump.
*/
bool ARMv4::OpCode::isIndirectJump( const WIR_Operation &o ) const
{
  DSTART(
    "virtual bool ARMv4::OpCode::isIndirectJump(const WIR_Operation&) const" );

  if ( ( *this == ARMv4T::OpCode::BX ) &&
       ( o.getOperationFormat() == ARMv4T::OperationFormat::TR_1 ) ) {
    const WIR_BaseRegister &r =
      dynamic_cast<const WIR_RegisterParameter &>(
        o.getParameters().front().get() ).getRegister();

    if ( r.isPhysical() ) {
      if ( r.getName() == "r14" )
        return( false );
      else
        return( mIsIndirectJump );
    }

    const WIR_VirtualRegister &vr =
      dynamic_cast<const WIR_VirtualRegister &>( r );
    if ( vr.isPrecolored() && ( vr.getPrecolor().getName() == "r14" ) )
      return( false );
  }

  return( mIsIndirectJump );
};


/*
  Access to low physical register R0.
*/
const ARM_LoRegP &ARMv4::R0( void ) const
{
  DSTART( "const ARM_LoRegP& ARMv4::R0() const" );

  return( dynamic_cast<ARM_LoRegP &>( mPhRegReferences.at( 0 ).get() ) );
};


/*
  Access to low physical register R1.
*/
const ARM_LoRegP &ARMv4::R1( void ) const
{
  DSTART( "const ARM_LoRegP& ARMv4::R1() const" );

  return( dynamic_cast<ARM_LoRegP &>( mPhRegReferences.at( 1 ).get() ) );
};


/*
  Access to low physical register R2.
*/
const ARM_LoRegP &ARMv4::R2( void ) const
{
  DSTART( "const ARM_LoRegP& ARMv4::R2() const" );

  return( dynamic_cast<ARM_LoRegP &>( mPhRegReferences.at( 2 ).get() ) );
};


/*
  Access to low physical register R3.
*/
const ARM_LoRegP &ARMv4::R3( void ) const
{
  DSTART( "const ARM_LoRegP& ARMv4::R3() const" );

  return( dynamic_cast<ARM_LoRegP &>( mPhRegReferences.at( 3 ).get() ) );
};


/*
  Access to low physical register R4.
*/
const ARM_LoRegP &ARMv4::R4( void ) const
{
  DSTART( "const ARM_LoRegP& ARMv4::R4() const" );

  return( dynamic_cast<ARM_LoRegP &>( mPhRegReferences.at( 4 ).get() ) );
};


/*
  Access to low physical register R5.
*/
const ARM_LoRegP &ARMv4::R5( void ) const
{
  DSTART( "const ARM_LoRegP& ARMv4::R5() const" );

  return( dynamic_cast<ARM_LoRegP &>( mPhRegReferences.at( 5 ).get() ) );
};


/*
  Access to low physical register R6.
*/
const ARM_LoRegP &ARMv4::R6( void ) const
{
  DSTART( "const ARM_LoRegP& ARMv4::R6() const" );

  return( dynamic_cast<ARM_LoRegP &>( mPhRegReferences.at( 6 ).get() ) );
};


/*
  Access to low physical register R7.
*/
const ARM_LoRegP &ARMv4::R7( void ) const
{
  DSTART( "const ARM_LoRegP& ARMv4::R7() const" );

  return( dynamic_cast<ARM_LoRegP &>( mPhRegReferences.at( 7 ).get() ) );
};


/*
  Access to high physical register R8.
*/
const ARM_HiRegP &ARMv4::R8( void ) const
{
  DSTART( "const ARM_HiRegP& ARMv4::R8() const" );

  return( dynamic_cast<ARM_HiRegP &>( mPhRegReferences.at( 8 ).get() ) );
};


/*
  Access to high physical register R9.
*/
const ARM_HiRegP &ARMv4::R9( void ) const
{
  DSTART( "const ARM_HiRegP& ARMv4::R9() const" );

  return( dynamic_cast<ARM_HiRegP &>( mPhRegReferences.at( 9 ).get() ) );
};


/*
  Access to high physical register R10.
*/
const ARM_HiRegP &ARMv4::R10( void ) const
{
  DSTART( "const ARM_HiRegP& ARMv4::R10() const" );

  return( dynamic_cast<ARM_HiRegP &>( mPhRegReferences.at( 10 ).get() ) );
};


/*
  Access to high physical register R11.
*/
const ARM_HiRegP &ARMv4::R11( void ) const
{
  DSTART( "const ARM_HiRegP& ARMv4::R11() const" );

  return( dynamic_cast<ARM_HiRegP &>( mPhRegReferences.at( 11 ).get() ) );
};


/*
  Access to high physical register R12.
*/
const ARM_HiRegP &ARMv4::R12( void ) const
{
  DSTART( "const ARM_HiRegP& ARMv4::R12() const" );

  return( dynamic_cast<ARM_HiRegP &>( mPhRegReferences.at( 12 ).get() ) );
};


/*
  Access to high physical register R13.
*/
const ARM_HiRegP &ARMv4::R13( void ) const
{
  DSTART( "const ARM_HiRegP& ARMv4::R13() const" );

  return( dynamic_cast<ARM_HiRegP &>( mPhRegReferences.at( 13 ).get() ) );
};


/*
  Access to high physical register R14.
*/
const ARM_HiRegP &ARMv4::R14( void ) const
{
  DSTART( "const ARM_HiRegP& ARMv4::R14() const" );

  return( dynamic_cast<ARM_HiRegP &>( mPhRegReferences.at( 14 ).get() ) );
};


/*
  Access to high physical register R15.
*/
const ARM_HiRegP &ARMv4::R15( void ) const
{
  DSTART( "const ARM_HiRegP& ARMv4::R15() const" );

  return( dynamic_cast<ARM_HiRegP &>( mPhRegReferences.at( 15 ).get() ) );
};


/*
  Access to stack pointer (alias R13).
*/
const ARM_HiRegP &ARMv4::SP( void ) const
{
  DSTART( "const ARM_HiRegP& ARMv4::SP() const" );

  return( R13() );
};


/*
  Access to link register (alias R14).
*/
const ARM_HiRegP &ARMv4::LR( void ) const
{
  DSTART( "const ARM_HiRegP& ARMv4::LR() const" );

  return( R14() );
};


/*
  Access to program counter (alias R15).
*/
const ARM_HiRegP &ARMv4::PC( void ) const
{
  DSTART( "const ARM_HiRegP& ARMv4::PC() const" );

  return( R15() );
};

}       // namespace WIR
