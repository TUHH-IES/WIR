/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2025 - 2026, Heiko Falk.

*/

/*!
  @file rvconst10unsigned.cc
  @brief This file implements unsigned 10 bits-wide immediate parameters.

  @author Sasha Tosta <Sasha.Tosta@tuhh.de>
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
#include <arch/riscv/rv32ic.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for unsigned const10 parameters.

  The constructor ensures that __i lies in the range of values that can be
  represented with 10 bits (unsigned) and is a non-zero multiple of 4.
*/
RV_Const10_Unsigned::RV_Const10_Unsigned( unsigned long long __i ) :
  WIR_UnsignedImmediateParameter<RV_Const10_Unsigned> { __i, 10 }
{
  DSTART( "RV_Const10_Unsigned::RV_Const10_Unsigned(long long unsigned int)" );

  ufAssert( __i % 4 == 0 );
  ufAssert( __i != 0 );
};


/*
  Copy constructor.
*/
RV_Const10_Unsigned::RV_Const10_Unsigned( const RV_Const10_Unsigned &__o ) :
  WIR_UnsignedImmediateParameter<RV_Const10_Unsigned> { __o }
{
  DSTART(
    "RV_Const10_Unsigned::RV_Const10_Unsigned(const RV_Const10_Unsigned&)" );
};


/*
  Move constructor.
*/
RV_Const10_Unsigned::RV_Const10_Unsigned( RV_Const10_Unsigned &&__o ) :
  WIR_UnsignedImmediateParameter<RV_Const10_Unsigned> { std::move( __o ) }
{
  DSTART( "RV_Const10_Unsigned::RV_Const10_Unsigned(RV_Const10_Unsigned&&)" );
};


/*
  Destructor.
*/
RV_Const10_Unsigned::~RV_Const10_Unsigned( void )
{
  DSTART( "virtual RV_Const10_Unsigned::~RV_Const10_Unsigned()" );
};


/*
  Copy-assignment operator.
*/
RV_Const10_Unsigned & RV_Const10_Unsigned::operator = ( const RV_Const10_Unsigned &__o )
{
  DSTART(
    "RV_Const10_Unsigned& RV_Const10_Unsigned::operator=(const"
    " RV_Const10_Unsigned&)" );

  WIR_UnsignedImmediateParameter<RV_Const10_Unsigned>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV_Const10_Unsigned & RV_Const10_Unsigned::operator = ( RV_Const10_Unsigned &&__o )
{
  DSTART(
    "RV_Const10_Unsigned& RV_Const10_Unsigned::operator=("
    "RV_Const10_Unsigned&&)" );

  WIR_UnsignedImmediateParameter<RV_Const10_Unsigned>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
