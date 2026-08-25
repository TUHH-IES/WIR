/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32imfdc.cc
  @brief This file implements the specific interface of the RISC-V RV32IMFDC
         Base Integer instruction set plus the M Standard Extension for Integer
         Multiplication and Division plus the F and D Standard Extensions for
         single- and double-precision Floating-Point Instructions and the C
         Standard Extension for Compressed Extension for Compressed
         Instructions, version 2.0.

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
#include <arch/riscv/rv32imfdc.h>


//
// Code Section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for RV32IMFDC processor architectures.
*/
RV32IMFDC::RV32IMFDC( void ) :
  RV32IM {},
  RV32IFD {},
  RV32IC {}
{
  DSTART( "RV32IMFDC::RV32IMFDC()" );

  // Specify the processor architecture modeled by this class.
  setISAName( "RV32IMFDC" );
};


/*
  Copy constructor.
*/
RV32IMFDC::RV32IMFDC( const RV32IMFDC &__o ) :
  RV32I { __o },
  RV32IF { __o },
  RV32IM { __o },
  RV32IFD { __o },
  RV32IC { __o }
{
  DSTART( "RV32IMFDC::RV32IMFDC(const RV32IMFDC&)" );
};


/*
  Move constructor.
*/
RV32IMFDC::RV32IMFDC( RV32IMFDC &&__o ) :
  RV32IM { __o },
  RV32IFD { __o },
  RV32IC { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
RV32IMFDC::~RV32IMFDC( void )
{
  DSTART( "virtual RV32IMFDC::~RV32IMFDC()" );
};


/*
  Copy-assignment operator.
*/
RV32IMFDC & RV32IMFDC::operator = ( const RV32IMFDC &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IM::operator = ( __o );
  RV32IFD::operator = ( __o );
  RV32IC::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV32IMFDC & RV32IMFDC::operator = ( RV32IMFDC &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IM::operator = ( __o );
  RV32IFD::operator = ( __o );
  RV32IC::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for RV32IMFDC processor
  architectures.

  This includes setting up the assignment of valid operation formats to
  RV32IMFDC opcodes. init, however, only registers the RV32IMFDC processor, as
  all OpCodes and OperationFormats have already been registerd by its base
  classes.
*/
// cppcheck-suppress duplInheritedMember
void RV32IMFDC::init( void )
{
  DSTART( "static void RV32IMFDC::init()" );

  //
  // Register this current processor model.
  //

  registerProcessor( RV32IMFDC() );
};


//
// Private class methods
//

/*
  clone creates a copy of an RV32IMFDC processor.
*/
WIR_BaseProcessor *RV32IMFDC::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* RV32IMFDC::clone() const" );

  return ( new RV32IMFDC( *this ) );
};

}       // namespace WIR
