/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcconst10signed.cc
  @brief This file implements signed 10 bits-wide immediate parameters.

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
  Default constructor for signed const10 parameters.

  The constructor ensures that i lies in the range of values that can be
  represented with 10 bits, assuming two's-complement as underlying data format.
*/
TC_Const10_Signed::TC_Const10_Signed( signed long long __i ) :
  WIR_SignedImmediateParameter<TC_Const10_Signed> { __i, 10 }
{
  DSTART( "TC_Const10_Signed::TC_Const10_Signed(long long int)" );
};


/*
  Copy constructor.
*/
TC_Const10_Signed::TC_Const10_Signed( const TC_Const10_Signed &__o ) :
  WIR_SignedImmediateParameter<TC_Const10_Signed> { __o }
{
  DSTART( "TC_Const10_Signed::TC_Const10_Signed(const TC_Const10_Signed&)" );
};


/*
  Move constructor.
*/
TC_Const10_Signed::TC_Const10_Signed( TC_Const10_Signed &&__o ) :
  WIR_SignedImmediateParameter<TC_Const10_Signed> { std::move( __o ) }
{
  DSTART( "TC_Const10_Signed::TC_Const10_Signed(TC_Const10_Signed&&)" );
};


/*
  Destructor.
*/
TC_Const10_Signed::~TC_Const10_Signed( void )
{
  DSTART( "virtual TC_Const10_Signed::~TC_Const10_Signed()" );
};


/*
  Copy-assignment operator.
*/
TC_Const10_Signed & TC_Const10_Signed::operator = ( const TC_Const10_Signed &__o )
{
  DSTART(
    "TC_Const10_Signed& TC_Const10_Signed::operator=(const TC_Const10_Signed&)" );

  WIR_SignedImmediateParameter<TC_Const10_Signed>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
TC_Const10_Signed & TC_Const10_Signed::operator = ( TC_Const10_Signed &&__o )
{
  DSTART(
    "TC_Const10_Signed& TC_Const10_Signed::operator=(TC_Const10_Signed&&)" );

  WIR_SignedImmediateParameter<TC_Const10_Signed>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
