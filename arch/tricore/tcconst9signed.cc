/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcconst9signed.cc
  @brief This file implements signed 9 bits-wide immediate parameters.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
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
#include <arch/tricore/tc13.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for signed const9 parameters.

  The constructor ensures that i lies in the range of values that can be
  represented with 9 bits, assuming two's-complement as underlying data format.
*/
TC_Const9_Signed::TC_Const9_Signed( signed long long __i ) :
  WIR_SignedImmediateParameter<TC_Const9_Signed> { __i, 9 }
{
  DSTART( "TC_Const9_Signed::TC_Const9_Signed(long long int)" );
};


/*
  Copy constructor.
*/
TC_Const9_Signed::TC_Const9_Signed( const TC_Const9_Signed &__o ) :
  WIR_SignedImmediateParameter<TC_Const9_Signed> { __o }
{
  DSTART( "TC_Const9_Signed::TC_Const9_Signed(const TC_Const9_Signed&)" );
};


/*
  Move constructor.
*/
TC_Const9_Signed::TC_Const9_Signed( TC_Const9_Signed &&__o ) :
  WIR_SignedImmediateParameter<TC_Const9_Signed> { std::move( __o ) }
{
  DSTART( "TC_Const9_Signed::TC_Const9_Signed(TC_Const9_Signed&&)" );
};


/*
  Destructor.
*/
TC_Const9_Signed::~TC_Const9_Signed( void )
{
  DSTART( "virtual TC_Const9_Signed::~TC_Const9_Signed()" );
};


/*
  Copy-assignment operator.
*/
TC_Const9_Signed & TC_Const9_Signed::operator = ( const TC_Const9_Signed &__o )
{
  DSTART(
    "TC_Const9_Signed& TC_Const9_Signed::operator=(const TC_Const9_Signed&)" );

  WIR_SignedImmediateParameter<TC_Const9_Signed>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
TC_Const9_Signed & TC_Const9_Signed::operator = ( TC_Const9_Signed &&__o )
{
  DSTART(
    "TC_Const9_Signed& TC_Const9_Signed::operator=(TC_Const9_Signed&&)" );

  WIR_SignedImmediateParameter<TC_Const9_Signed>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
