/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32imfd.cc
  @brief This file implements the specific interface of the RISC-V RV32IMFD Base
         Integer instruction set plus the M Standard Extension for Integer
         Multiplication and Division plus the F and D Standard Extensions for
         single- and double-precision Floating-Point Instructions, version 2.0.

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
#include <arch/riscv/rv32imfd.h>


//
// Code Section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for RV32IMFD processor architectures.
*/
RV32IMFD::RV32IMFD( void ) :
  RV32IM {},
  RV32IFD {}
{
  DSTART( "RV32IMFD::RV32IMFD()" );

  // Specify the processor architecture modeled by this class.
  setISAName( "RV32IMFD" );
};


/*
  Copy constructor.
*/
RV32IMFD::RV32IMFD( const RV32IMFD &__o ) :
  RV32I { __o },
  RV32IF { __o },
  RV32IM { __o },
  RV32IFD { __o }
{
  DSTART( "RV32IMFD::RV32IMFD(const RV32IMFD&)" );
};


/*
  Move constructor.
*/
RV32IMFD::RV32IMFD( RV32IMFD &&__o ) :
  RV32IM { __o },
  RV32IFD { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
RV32IMFD::~RV32IMFD( void )
{
  DSTART( "virtual RV32IMFD::~RV32IMFD()" );
};


/*
  Copy-assignment operator.
*/
RV32IMFD & RV32IMFD::operator = ( const RV32IMFD &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IM::operator = ( __o );
  RV32IFD::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV32IMFD & RV32IMFD::operator = ( RV32IMFD &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IM::operator = ( __o );
  RV32IFD::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for RV32IMFD processor
  architectures.

  This includes setting up the assignment of valid operation formats to RV32IMFD
  opcodes. init, however, only registers the RV32IMFD processor, as all OpCodes
  and OperationFormats have already been registerd by its base classes.
*/
// cppcheck-suppress duplInheritedMember
void RV32IMFD::init( void )
{
  DSTART( "static void RV32IMFD::init()" );

  //
  // Register this current processor model.
  //

  registerProcessor( RV32IMFD() );
};


//
// Private class methods
//

/*
  clone creates a copy of an RV32IMFD processor.
*/
WIR_BaseProcessor *RV32IMFD::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* RV32IMFD::clone() const" );

  return ( new RV32IMFD( *this ) );
};

}       // namespace WIR
