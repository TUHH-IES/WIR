/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armv4t.cc
  @brief This file implements the specific interface of the ARMv4T instruction
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
#include <arch/arm/armv4t.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for ARMv4T-based processor architectures.
*/
ARMv4T::ARMv4T( void ) :
  ARMv4 {}
{
  DSTART( "ARMv4T::ARMv4T()" );

  // Specify the processor architecture modeled by this class.
  setISAName( "ARMv4T" );
};


/*
  Copy constructor.
*/
ARMv4T::ARMv4T( const ARMv4T &__o ) :
  ARMv4 { __o }
{
  DSTART( "ARMv4T::ARMv4T(const ARMv4T&)" );
};


/*
  Move constructor.
*/
ARMv4T::ARMv4T( ARMv4T &&__o ) :
  ARMv4 { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
ARMv4T::~ARMv4T( void )
{
  DSTART( "virtual ARMv4T::~ARMv4T()" );
};


/*
  Copy-assignment operator.
*/
ARMv4T & ARMv4T::operator = ( const ARMv4T &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  ARMv4::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
ARMv4T & ARMv4T::operator = ( ARMv4T &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  ARMv4::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for ARMv4T-based processor
  architectures.

  This includes setting up the ARMv4T machine operation formats and the
  assignment of valid operation formats to ARMv4T opcodes.

  init shall be called globally by WIR_Init(). It shall only perform tasks that
  cannot be expressed as initializations of static class members (since the
  order of static initialization is unspecified in C++) and that thus require
  execution by active code.
*/
// cppcheck-suppress duplInheritedMember
void ARMv4T::init( void )
{
  DSTART( "static void ARMv4T::init()" );


  //
  // Register ARMv4T operation formats.
  //

  WIR_BasicBlock b;
  const ARM_RegV *regV = new ARM_RegV;
  const ARM_LoRegV *loregV = new ARM_LoRegV;
  const ARM_HiRegP *sp = new ARM_HiRegP( "13", true );
  const ARM_HiRegP *lr = new ARM_HiRegP( "14" );
  const ARM_HiRegP *pc = new ARM_HiRegP( "15" );

  registerOperationFormat(
    OperationFormat::CR_3,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TC8_1,
    { new ARM_Const8_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::TC9_1,
    { new ARM_Const9_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::TCL,
    { new WIR_ConditionFieldParameter( ARMv4::Condition::al ),
      new WIR_LabelParameter( b ) } );

  registerOperationFormat(
    OperationFormat::TL_1,
    { new WIR_LabelParameter( b ) } );

  registerOperationFormat(
    OperationFormat::TL_2,
    { new WIR_LabelParameter( b ) } );

  registerOperationFormat(
    OperationFormat::TR_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR1_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR1PC,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *pc, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR1_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR1LR,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *lr, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR2_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR2PC,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *pc, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR2_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR2LR,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *lr, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR3_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR3PC,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *pc, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR3_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR3LR,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *lr, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR4_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR4PC,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *pc, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR4_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR4LR,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *lr, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR5_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR5PC,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *pc, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR5_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR5LR,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *lr, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR6_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR6PC,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *pc, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR6_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR6LR,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *lr, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR7_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR7PC,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *pc, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR7_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR7LR,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *lr, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR8_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR8PC,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *pc, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TR8_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TR8LR,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *lr, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRC8_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new ARM_Const8_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRC8_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new ARM_Const8_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRC8_3,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new ARM_Const8_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRPCC10_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *pc, WIR_Usage::use ),
      new ARM_Const10_Unsigned4( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRPCC10_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *pc, WIR_Usage::use ),
      new ARM_Const10_Unsigned4( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRR_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRR_2,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRR_3,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRR_4,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRR_5,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRR_6,
    { new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRR1_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TRR1_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRR2_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TRR2_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRR3_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TRR3_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRR4_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TRR4_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRR5_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TRR5_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRR6_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TRR6_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRR7_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TRR7_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRR8_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::TRR8_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRRC3_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new ARM_Const3_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRRC5_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRRC5_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRRC5_3,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new ARM_Const5_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRRC6_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new ARM_Const6_Unsigned0( 1 ) } );

  registerOperationFormat(
    OperationFormat::TRRC6_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new ARM_Const6_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRRC6_3,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new ARM_Const6_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRRC7_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new ARM_Const7_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRRC7_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new ARM_Const7_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRRR_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRRR_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRRR_3,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *loregV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::TRSPC10_1,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *sp, WIR_Usage::use ),
      new ARM_Const10_Unsigned4( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRSPC10_2,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::def ),
      new WIR_RegisterParameter( *sp, WIR_Usage::use ),
      new ARM_Const10_Unsigned4( 0 ) } );

  registerOperationFormat(
    OperationFormat::TRSPC10_3,
    { new WIR_RegisterParameter( *loregV, WIR_Usage::use ),
      new WIR_RegisterParameter( *sp, WIR_Usage::use ),
      new ARM_Const10_Unsigned4( 0 ) } );


  //
  // ARMv4T opcode to operation format mapping.
  //

  registerOpCode( OpCode::ADC, OperationFormat::TRR_5 );
  registerOpCode( OpCode::ADD, OperationFormat::TRRC3_1 );
  registerOpCode( OpCode::ADD, OperationFormat::TRC8_3 );
  registerOpCode( OpCode::ADD, OperationFormat::TRRR_1 );
  registerOpCode( OpCode::ADD, OperationFormat::TRR_6 );
  registerOpCode( OpCode::ADD, OperationFormat::TRPCC10_1 );
  registerOpCode( OpCode::ADD, OperationFormat::TRSPC10_1 );
  registerOpCode( OpCode::ADD, OperationFormat::TC9_1 );
  registerOpCode( OpCode::AND, OperationFormat::TRR_5 );
  registerOpCode( OpCode::ASR, OperationFormat::TRRC6_1 );
  registerOpCode( OpCode::ASR, OperationFormat::TRR_5 );
  registerOpCode( OpCode::B, OperationFormat::TCL );
  registerOpCode( OpCode::B, OperationFormat::TL_1 );
  registerOpCode( OpCode::BIC, OperationFormat::TRR_5 );
  registerOpCode( OpCode::BL, OperationFormat::TL_2 );
  registerOpCode( OpCode::BX, OperationFormat::CR_3 );
  registerOpCode( OpCode::BX, OperationFormat::TR_1 );
  registerOpCode( OpCode::CMN, OperationFormat::TRR_5 );
  registerOpCode( OpCode::CMP, OperationFormat::TRC8_2 );
  registerOpCode( OpCode::CMP, OperationFormat::TRR_3 );
  registerOpCode( OpCode::CMP, OperationFormat::TRR_4 );
  registerOpCode( OpCode::EOR, OperationFormat::TRR_5 );
  registerOpCode( OpCode::LDMIA, OperationFormat::TRR1_1 );
  registerOpCode( OpCode::LDMIA, OperationFormat::TRR2_1 );
  registerOpCode( OpCode::LDMIA, OperationFormat::TRR3_1 );
  registerOpCode( OpCode::LDMIA, OperationFormat::TRR4_1 );
  registerOpCode( OpCode::LDMIA, OperationFormat::TRR5_1 );
  registerOpCode( OpCode::LDMIA, OperationFormat::TRR6_1 );
  registerOpCode( OpCode::LDMIA, OperationFormat::TRR7_1 );
  registerOpCode( OpCode::LDMIA, OperationFormat::TRR8_1 );
  registerOpCode( OpCode::LDR, OperationFormat::TRRC7_1 );
  registerOpCode( OpCode::LDR, OperationFormat::TRRR_2 );
  registerOpCode( OpCode::LDR, OperationFormat::TRPCC10_2 );
  registerOpCode( OpCode::LDR, OperationFormat::TRSPC10_2 );
  registerOpCode( OpCode::LDRB, OperationFormat::TRRC5_2 );
  registerOpCode( OpCode::LDRB, OperationFormat::TRRR_2 );
  registerOpCode( OpCode::LDRH, OperationFormat::TRRC6_2 );
  registerOpCode( OpCode::LDRH, OperationFormat::TRRR_2 );
  registerOpCode( OpCode::LDRSB, OperationFormat::TRRR_2 );
  registerOpCode( OpCode::LDRSH, OperationFormat::TRRR_2 );
  registerOpCode( OpCode::LSL, OperationFormat::TRRC5_1 );
  registerOpCode( OpCode::LSL, OperationFormat::TRR_5 );
  registerOpCode( OpCode::LSR, OperationFormat::TRRC6_1 );
  registerOpCode( OpCode::LSR, OperationFormat::TRR_5 );
  registerOpCode( OpCode::MOV, OperationFormat::TRC8_1 );
  registerOpCode( OpCode::MOV, OperationFormat::TRR_1 );
  registerOpCode( OpCode::MOV, OperationFormat::TRR_2 );
  registerOpCode( OpCode::MUL, OperationFormat::TRR_5 );
  registerOpCode( OpCode::MVN, OperationFormat::TRR_1 );
  registerOpCode( OpCode::NEG, OperationFormat::TRR_1 );
  registerOpCode( OpCode::ORR, OperationFormat::TRR_5 );
  registerOpCode( OpCode::POP, OperationFormat::TR1_1 );
  registerOpCode( OpCode::POP, OperationFormat::TR1PC );
  registerOpCode( OpCode::POP, OperationFormat::TR2_1 );
  registerOpCode( OpCode::POP, OperationFormat::TR2PC );
  registerOpCode( OpCode::POP, OperationFormat::TR3_1 );
  registerOpCode( OpCode::POP, OperationFormat::TR3PC );
  registerOpCode( OpCode::POP, OperationFormat::TR4_1 );
  registerOpCode( OpCode::POP, OperationFormat::TR4PC );
  registerOpCode( OpCode::POP, OperationFormat::TR5_1 );
  registerOpCode( OpCode::POP, OperationFormat::TR5PC );
  registerOpCode( OpCode::POP, OperationFormat::TR6_1 );
  registerOpCode( OpCode::POP, OperationFormat::TR6PC );
  registerOpCode( OpCode::POP, OperationFormat::TR7_1 );
  registerOpCode( OpCode::POP, OperationFormat::TR7PC );
  registerOpCode( OpCode::POP, OperationFormat::TR8_1 );
  registerOpCode( OpCode::POP, OperationFormat::TR8PC );
  registerOpCode( OpCode::PUSH, OperationFormat::TR1_2 );
  registerOpCode( OpCode::PUSH, OperationFormat::TR1LR );
  registerOpCode( OpCode::PUSH, OperationFormat::TR2_2 );
  registerOpCode( OpCode::PUSH, OperationFormat::TR2LR );
  registerOpCode( OpCode::PUSH, OperationFormat::TR3_2 );
  registerOpCode( OpCode::PUSH, OperationFormat::TR3LR );
  registerOpCode( OpCode::PUSH, OperationFormat::TR4_2 );
  registerOpCode( OpCode::PUSH, OperationFormat::TR4LR );
  registerOpCode( OpCode::PUSH, OperationFormat::TR5_2 );
  registerOpCode( OpCode::PUSH, OperationFormat::TR5LR );
  registerOpCode( OpCode::PUSH, OperationFormat::TR6_2 );
  registerOpCode( OpCode::PUSH, OperationFormat::TR6LR );
  registerOpCode( OpCode::PUSH, OperationFormat::TR7_2 );
  registerOpCode( OpCode::PUSH, OperationFormat::TR7LR );
  registerOpCode( OpCode::PUSH, OperationFormat::TR8_2 );
  registerOpCode( OpCode::PUSH, OperationFormat::TR8LR );
  registerOpCode( OpCode::ROR, OperationFormat::TRR_5 );
  registerOpCode( OpCode::SBC, OperationFormat::TRR_5 );
  registerOpCode( OpCode::STMIA, OperationFormat::TRR1_2 );
  registerOpCode( OpCode::STMIA, OperationFormat::TRR2_2 );
  registerOpCode( OpCode::STMIA, OperationFormat::TRR3_2 );
  registerOpCode( OpCode::STMIA, OperationFormat::TRR4_2 );
  registerOpCode( OpCode::STMIA, OperationFormat::TRR5_2 );
  registerOpCode( OpCode::STMIA, OperationFormat::TRR6_2 );
  registerOpCode( OpCode::STMIA, OperationFormat::TRR7_2 );
  registerOpCode( OpCode::STMIA, OperationFormat::TRR8_2 );
  registerOpCode( OpCode::STR, OperationFormat::TRRC7_2 );
  registerOpCode( OpCode::STR, OperationFormat::TRRR_3 );
  registerOpCode( OpCode::STR, OperationFormat::TRSPC10_3 );
  registerOpCode( OpCode::STRB, OperationFormat::TRRC5_3 );
  registerOpCode( OpCode::STRB, OperationFormat::TRRR_3 );
  registerOpCode( OpCode::STRH, OperationFormat::TRRC6_3 );
  registerOpCode( OpCode::STRH, OperationFormat::TRRR_3 );
  registerOpCode( OpCode::SUB, OperationFormat::TRRC3_1 );
  registerOpCode( OpCode::SUB, OperationFormat::TRC8_3 );
  registerOpCode( OpCode::SUB, OperationFormat::TRRR_1 );
  registerOpCode( OpCode::SUB, OperationFormat::TC9_1 );
  registerOpCode( OpCode::SWI, OperationFormat::TC8_1 );
  registerOpCode( OpCode::TST, OperationFormat::TRR_3 );


  //
  // Register this current processor model.
  //

  registerProcessor( ARMv4T() );
};


//
// Private class methods
//

/*
  clone creates a copy of a ARMv4T processor.
*/
WIR_BaseProcessor *ARMv4T::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* ARMv4T::clone() const" );

  return( new ARMv4T( *this ) );
};

}       // namespace WIR
