/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvconst20unsigned.cc
  @brief This file implements unsigned 20 bits-wide immediate parameters.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32i.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for unsigned const20 parameters.

  The constructor ensures that i lies in the range of values that can be
  represented with 20 bits (unsigned).
*/
RV_Const20_Unsigned::RV_Const20_Unsigned( unsigned long long __i ) :
  WIR_UnsignedImmediateParameter<RV_Const20_Unsigned> { __i, 20 }
{
  DSTART( "RV_Const20_Unsigned::RV_Const20_Unsigned(long long unsigned int)" );
};


/*
  Copy constructor.
*/
RV_Const20_Unsigned::RV_Const20_Unsigned( const RV_Const20_Unsigned &__o ) :
  WIR_UnsignedImmediateParameter<RV_Const20_Unsigned> { __o }
{
  DSTART(
    "RV_Const20_Unsigned::RV_Const20_Unsigned(const RV_Const20_Unsigned&)" );
};


/*
  Move constructor.
*/
RV_Const20_Unsigned::RV_Const20_Unsigned( RV_Const20_Unsigned &&__o ) :
  WIR_UnsignedImmediateParameter<RV_Const20_Unsigned> { std::move( __o ) }
{
  DSTART( "RV_Const20_Unsigned::RV_Const20_Unsigned(RV_Const20_Unsigned&&)" );
};


/*
  Destructor.
*/
RV_Const20_Unsigned::~RV_Const20_Unsigned( void )
{
  DSTART( "virtual RV_Const20_Unsigned::~RV_Const20_Unsigned()" );
};


/*
  Copy-assignment operator.
*/
RV_Const20_Unsigned & RV_Const20_Unsigned::operator = ( const RV_Const20_Unsigned &__o )
{
  DSTART(
    "RV_Const20_Unsigned& RV_Const20_Unsigned::operator=(const"
    " RV_Const20_Unsigned&)" );

  WIR_UnsignedImmediateParameter<RV_Const20_Unsigned>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV_Const20_Unsigned & RV_Const20_Unsigned::operator = ( RV_Const20_Unsigned &&__o )
{
  DSTART(
    "RV_Const20_Unsigned& RV_Const20_Unsigned::operator=("
    "RV_Const20_Unsigned&&)" );

  WIR_UnsignedImmediateParameter<RV_Const20_Unsigned>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
