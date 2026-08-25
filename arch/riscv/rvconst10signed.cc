/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2025 - 2026, Heiko Falk.

*/

/*!
  @file rvconst10signed.cc
  @brief This file implements signed 10 bits-wide immediate parameters.

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
  Default constructor for signed const10 parameters.

  The constructor ensures that __i lies in the range of values that can be
  represented with 10 bits and is a non-zero multiple of 16, assuming
  two's-complement as underlying data format.
*/
RV_Const10_Signed::RV_Const10_Signed( signed long long __i ) :
  WIR_SignedImmediateParameter<RV_Const10_Signed> { __i, 10 }
{
  DSTART( "RV_Const10_Signed::RV_Const10_Signed(long long int)" );

  // cppcheck-suppress knownArgument
  ufAssert( __i % 16 == 0 );
  ufAssert( __i != 0 );
};


/*
  Copy constructor.
*/
RV_Const10_Signed::RV_Const10_Signed( const RV_Const10_Signed &__o ) :
  WIR_SignedImmediateParameter<RV_Const10_Signed> { __o }
{
  DSTART( "RV_Const10_Signed::RV_Const10_Signed(const RV_Const10_Signed&)" );
};


/*
  Move constructor.
*/
RV_Const10_Signed::RV_Const10_Signed( RV_Const10_Signed &&__o ) :
  WIR_SignedImmediateParameter<RV_Const10_Signed> { std::move( __o ) }
{
  DSTART( "RV_Const10_Signed::RV_Const10_Signed(RV_Const10_Signed&&)" );
};


/*
  Destructor.
*/
RV_Const10_Signed::~RV_Const10_Signed( void )
{
  DSTART( "virtual RV_Const10_Signed::~RV_Const10_Signed()" );
};


/*
  Copy-assignment operator.
*/
RV_Const10_Signed & RV_Const10_Signed::operator = ( const RV_Const10_Signed &__o )
{
  DSTART(
    "RV_Const10_Signed& RV_Const10_Signed::operator=(const"
    " RV_Const10_Signed&)" );

  WIR_SignedImmediateParameter<RV_Const10_Signed>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV_Const10_Signed & RV_Const10_Signed::operator = ( RV_Const10_Signed &&__o )
{
  DSTART(
    "RV_Const10_Signed& RV_Const10_Signed::operator=(RV_Const10_Signed&&)" );

  WIR_SignedImmediateParameter<RV_Const10_Signed>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
