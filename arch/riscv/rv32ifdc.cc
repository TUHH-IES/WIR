/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32ifdc.cc
  @brief This file implements the specific interface of the RISC-V RV32IFDC Base
         Integer instruction set plus the F and D Standard Extensions for
         single- and double-precision Floating-Point Instructions and the C
         Standard Extension for Compressed Instructions, version 2.0.

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
#include <arch/riscv/rv32ifdc.h>


//
// Code Section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for RV32IFDC processor architectures.
*/
RV32IFDC::RV32IFDC( void ) :
  RV32IFD {},
  RV32IC {}
{
  DSTART( "RV32IFDC::RV32IFDC()" );

  // Specify the processor architecture modeled by this class.
  setISAName( "RV32IFDC" );
};


/*
  Copy constructor.
*/
RV32IFDC::RV32IFDC( const RV32IFDC &__o ) :
  RV32I { __o },
  RV32IF { __o },
  RV32IFD { __o },
  RV32IC { __o }
{
  DSTART( "RV32IFDC::RV32IFDC(const RV32IFDC&)" );
};


/*
  Move constructor.
*/
RV32IFDC::RV32IFDC( RV32IFDC &&__o ) :
  RV32IFD { __o },
  RV32IC { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
RV32IFDC::~RV32IFDC( void )
{
  DSTART( "virtual RV32IFDC::~RV32IFDC()" );
};


/*
  Copy-assignment operator.
*/
RV32IFDC & RV32IFDC::operator = ( const RV32IFDC &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IFD::operator = ( __o );
  RV32IC::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV32IFDC & RV32IFDC::operator = ( RV32IFDC &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IFD::operator = ( __o );
  RV32IC::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for RV32IFDC processor
  architectures.

  This includes setting up the assignment of valid operation formats to RV32IFDC
  opcodes. init, however, only registers the RV32IFDC processor, as all OpCodes
  and OperationFormats have already been registerd by its base classes.
*/
// cppcheck-suppress duplInheritedMember
void RV32IFDC::init( void )
{
  DSTART( "static void RV32IFDC::init()" );

  //
  // Register this current processor model.
  //

  registerProcessor( RV32IFDC() );
};


//
// Private class methods
//

/*
  clone creates a copy of an RV32IFDC processor.
*/
WIR_BaseProcessor *RV32IFDC::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* RV32IFDC::clone() const" );

  return ( new RV32IFDC( *this ) );
};

}       // namespace WIR
