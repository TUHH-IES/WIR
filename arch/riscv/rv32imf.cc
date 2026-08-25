/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32imf.cc
  @brief This file implements the specific interface of the RISC-V RV32IMF Base
         Integer instruction set plus the M Standard Extension for Integer
         Multiplication and Division and the F Standard Extension for
         single-precision Floating-Point Instructions, version 2.0.

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
#include <arch/riscv/rv32imf.h>


//
// Code Section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for RV32IMF processor architectures.
*/
RV32IMF::RV32IMF( void ) :
  RV32IM {},
  RV32IF {}
{
  DSTART( "RV32IMF::RV32IMF()" );

  // Specify the processor architecture modeled by this class.
  setISAName( "RV32IMF" );
};


/*
  Copy constructor.
*/
RV32IMF::RV32IMF( const RV32IMF &__o ) :
  RV32I { __o },
  RV32IM { __o },
  RV32IF { __o }
{
  DSTART( "RV32IMF::RV32IMF(const RV32IMF&)" );
};


/*
  Move constructor.
*/
RV32IMF::RV32IMF( RV32IMF &&__o ) :
  RV32IM { __o },
  RV32IF { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
RV32IMF::~RV32IMF( void )
{
  DSTART( "virtual RV32IMF::~RV32IMF()" );
};


/*
  Copy-assignment operator.
*/
RV32IMF & RV32IMF::operator = ( const RV32IMF &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IM::operator = ( __o );
  RV32IF::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV32IMF & RV32IMF::operator = ( RV32IMF &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IM::operator = ( __o );
  RV32IF::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for RV32IMF processor
  architectures.

  This includes setting up the assignment of valid operation formats to RV32IMF
  opcodes. init, however, only registers the RV32IMF processor, as all OpCodes
  and OperationFormats have already been registerd by its base classes.
*/
// cppcheck-suppress duplInheritedMember
void RV32IMF::init( void )
{
  DSTART( "static void RV32IMF::init()" );

  //
  // Register this current processor model.
  //

  registerProcessor( RV32IMF() );
};


//
// Private class methods
//

/*
  clone creates a copy of an RV32IMF processor.
*/
WIR_BaseProcessor *RV32IMF::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* RV32IMF::clone() const" );

  return ( new RV32IMF( *this ) );
};

}       // namespace WIR
