/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armconst12unsigned.cc
  @brief This file implements unsigned 12 bits-wide immediate parameters.

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
#include <arch/arm/armv4.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for unsigned const12 parameters.

  The constructor ensures that i lies in the range of values that can be
  represented with 12 bits (unsigned).
*/
ARM_Const12_Unsigned::ARM_Const12_Unsigned( unsigned long long __i ) :
  WIR_UnsignedImmediateParameter<ARM_Const12_Unsigned> { __i, 12 }
{
  DSTART(
    "ARM_Const12_Unsigned::ARM_Const12_Unsigned(long long unsigned int)" );
};


/*
  Copy constructor.
*/
ARM_Const12_Unsigned::ARM_Const12_Unsigned( const ARM_Const12_Unsigned &__o ) :
  WIR_UnsignedImmediateParameter<ARM_Const12_Unsigned> { __o }
{
  DSTART(
    "ARM_Const12_Unsigned::ARM_Const12_Unsigned(const ARM_Const12_Unsigned&)" );
};


/*
  Move constructor.
*/
ARM_Const12_Unsigned::ARM_Const12_Unsigned( ARM_Const12_Unsigned &&__o ) :
  WIR_UnsignedImmediateParameter<ARM_Const12_Unsigned> { std::move( __o ) }
{
  DSTART(
    "ARM_Const12_Unsigned::ARM_Const12_Unsigned(ARM_Const12_Unsigned&&)" );
};


/*
  Destructor.
*/
ARM_Const12_Unsigned::~ARM_Const12_Unsigned( void )
{
  DSTART( "virtual ARM_Const12_Unsigned::~ARM_Const12_Unsigned()" );
};


/*
  Copy-assignment operator.
*/
ARM_Const12_Unsigned & ARM_Const12_Unsigned::operator = ( const ARM_Const12_Unsigned &__o )
{
  DSTART(
    "ARM_Const12_Unsigned& ARM_Const12_Unsigned::operator=(const ARM_Const12_Unsigned&)" );

  WIR_UnsignedImmediateParameter<ARM_Const12_Unsigned>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
ARM_Const12_Unsigned & ARM_Const12_Unsigned::operator = ( ARM_Const12_Unsigned &&__o )
{
  DSTART(
    "ARM_Const12_Unsigned& ARM_Const12_Unsigned::operator=(ARM_Const12_Unsigned&&)" );

  WIR_UnsignedImmediateParameter<ARM_Const12_Unsigned>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
