/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcconst2unsigned.cc
  @brief This file implements unsigned 2 bits-wide immediate parameters.

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
  Default constructor for unsigned const2 parameters.

  The constructor ensures that i lies in the range of values that can be
  represented with 2 bits (unsigned).
*/
TC_Const2_Unsigned::TC_Const2_Unsigned( unsigned long long __i ) :
  WIR_UnsignedImmediateParameter<TC_Const2_Unsigned> { __i, 2 }
{
  DSTART( "TC_Const2_Unsigned::TC_Const2_Unsigned(long long unsigned int)" );
};


/*
  Copy constructor.
*/
TC_Const2_Unsigned::TC_Const2_Unsigned( const TC_Const2_Unsigned &__o ) :
  WIR_UnsignedImmediateParameter<TC_Const2_Unsigned> { __o }
{
  DSTART( "TC_Const2_Unsigned::TC_Const2_Unsigned(const TC_Const2_Unsigned&)" );
};


/*
  Move constructor.
*/
TC_Const2_Unsigned::TC_Const2_Unsigned( TC_Const2_Unsigned &&__o ) :
  WIR_UnsignedImmediateParameter<TC_Const2_Unsigned> { std::move( __o ) }
{
  DSTART( "TC_Const2_Unsigned::TC_Const2_Unsigned(TC_Const2_Unsigned&&)" );
};


/*
  Destructor.
*/
TC_Const2_Unsigned::~TC_Const2_Unsigned( void )
{
  DSTART( "virtual TC_Const2_Unsigned::~TC_Const2_Unsigned()" );
};


/*
  Copy-assignment operator.
*/
TC_Const2_Unsigned & TC_Const2_Unsigned::operator = ( const TC_Const2_Unsigned &__o )
{
  DSTART(
    "TC_Const2_Unsigned& TC_Const2_Unsigned::operator=(const TC_Const2_Unsigned&)" );

  WIR_UnsignedImmediateParameter<TC_Const2_Unsigned>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
TC_Const2_Unsigned & TC_Const2_Unsigned::operator = ( TC_Const2_Unsigned &&__o )
{
  DSTART(
    "TC_Const2_Unsigned& TC_Const2_Unsigned::operator=(TC_Const2_Unsigned&&)" );

  WIR_UnsignedImmediateParameter<TC_Const2_Unsigned>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
