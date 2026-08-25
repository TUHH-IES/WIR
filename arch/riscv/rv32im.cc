/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32im.cc
  @brief This file implements the specific interface of the RISC-V RV32IM Base
         Integer instruction set plus the M Standard Extension for Integer
         Multiplication and Division, version 2.0.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
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
#include <arch/riscv/rv32im.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for RV32IM processor architectures.
*/
RV32IM::RV32IM( void ) :
  RV32I {}
{
  DSTART( "RV32IM::RV32IM()" );

  // Specify the processor architecture modeled by this class.
  setISAName( "RV32IM" );
};


/*
  Copy constructor.
*/
RV32IM::RV32IM( const RV32IM &__o ) :
  RV32I { __o }
{
  DSTART( "RV32IM::RV32IM(const RV32IM&)" );
};


/*
  Move constructor.
*/
RV32IM::RV32IM( RV32IM &&__o ) :
  RV32I { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
RV32IM::~RV32IM( void )
{
  DSTART( "virtual RV32IM::~RV32IM()" );
};


/*
  Copy-assignment operator.
*/
RV32IM & RV32IM::operator = ( const RV32IM &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32I::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV32IM & RV32IM::operator = ( RV32IM &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32I::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for RV32IM processor
  architectures.

  This includes setting up the assignment of valid operation formats to RV32IM
  opcodes.

  init shall be called globally by WIR_Init(). It shall only perform tasks that
  cannot be expressed as initializations of static class members (since the
  order of static initialization is unspecified in C++) and that thus require
  execution by active code.
*/
// cppcheck-suppress duplInheritedMember
void RV32IM::init( void )
{
  DSTART( "static void RV32IM::init()" );

  //
  // RV32IM opcode to operation format mapping.
  //

  registerOpCode( OpCode::DIV, OperationFormat::RRR_1 );
  registerOpCode( OpCode::DIVU, OperationFormat::RRR_1 );
  registerOpCode( OpCode::MUL, OperationFormat::RRR_1 );
  registerOpCode( OpCode::MULH, OperationFormat::RRR_1 );
  registerOpCode( OpCode::MULHSU, OperationFormat::RRR_1 );
  registerOpCode( OpCode::MULHU, OperationFormat::RRR_1 );
  registerOpCode( OpCode::REM, OperationFormat::RRR_1 );
  registerOpCode( OpCode::REMU, OperationFormat::RRR_1 );

  //
  // Register this current processor model.
  //

  registerProcessor( RV32IM() );
};


//
// Private class methods
//

/*
  clone creates a copy of an RV32IM processor.
*/
WIR_BaseProcessor *RV32IM::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* RV32IM::clone() const" );

  return( new RV32IM( *this ) );
};

}       // namespace WIR
