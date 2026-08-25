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
  @brief This file implements the specific interface of the RISC-V RV32IMFC Base
         Integer instruction set plus the M Standard Extension for Integer
         Multiplication and Division plus the F Standard Extension for single-
         precision Floating-Point Instructions and the C Standard Extension for
         Compressed Instructions, version 2.0.

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
#include <arch/riscv/rv32imfc.h>


//
// Code Section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for RV32IMFC processor architectures.
*/
RV32IMFC::RV32IMFC( void ) :
  RV32IMF {},
  RV32IC {}
{
  DSTART( "RV32IMFC::RV32IMFC()" );

  // Specify the processor architecture modeled by this class.
  setISAName( "RV32IMFC" );
};


/*
  Copy constructor.
*/
RV32IMFC::RV32IMFC( const RV32IMFC &__o ) :
  RV32I { __o },
  RV32IMF { __o },
  RV32IC { __o }
{
  DSTART( "RV32IMFC::RV32IMFC(const RV32IMFC&)" );
};


/*
  Move constructor.
*/
RV32IMFC::RV32IMFC( RV32IMFC &&__o ) :
  RV32IMF { __o },
  RV32IC { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
RV32IMFC::~RV32IMFC( void )
{
  DSTART( "virtual RV32IMFC::~RV32IMFC()" );
};


/*
  Copy-assignment operator.
*/
RV32IMFC & RV32IMFC::operator = ( const RV32IMFC &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IMF::operator = ( __o );
  RV32IC::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV32IMFC & RV32IMFC::operator = ( RV32IMFC &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IMF::operator = ( __o );
  RV32IC::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for RV32IMFC processor
  architectures.

  This includes setting up the assignment of valid operation formats to RV32IMFC
  opcodes. init, however, only registers the RV32IMFC processor, as all OpCodes
  and OperationFormats have already been registerd by its base classes.
*/
// cppcheck-suppress duplInheritedMember
void RV32IMFC::init( void )
{
  DSTART( "static void RV32IMFC::init()" );

  //
  // Register this current processor model.
  //

  registerProcessor( RV32IMFC() );
};


//
// Private class methods
//

/*
  clone creates a copy of an RV32IMFC processor.
*/
WIR_BaseProcessor *RV32IMFC::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* RV32IMFC::clone() const" );

  return ( new RV32IMFC( *this ) );
};

}       // namespace WIR
