/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armconst8unsigned.cc
  @brief This file implements unsigned 8 bits-wide immediate parameters.

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
  Default constructor for unsigned const8 parameters.

  The constructor ensures that i lies in the range of values that can be
  represented with 8 bits (unsigned).
*/
ARM_Const8_Unsigned::ARM_Const8_Unsigned( unsigned long long __i ) :
  WIR_UnsignedImmediateParameter<ARM_Const8_Unsigned> { __i, 8 }
{
  DSTART( "ARM_Const8_Unsigned::ARM_Const8_Unsigned(long long unsigned int)" );
};


/*
  Copy constructor.
*/
ARM_Const8_Unsigned::ARM_Const8_Unsigned( const ARM_Const8_Unsigned &__o ) :
  WIR_UnsignedImmediateParameter<ARM_Const8_Unsigned> { __o }
{
  DSTART(
    "ARM_Const8_Unsigned::ARM_Const8_Unsigned(const ARM_Const8_Unsigned&)" );
};


/*
  Move constructor.
*/
ARM_Const8_Unsigned::ARM_Const8_Unsigned( ARM_Const8_Unsigned &&__o ) :
  WIR_UnsignedImmediateParameter<ARM_Const8_Unsigned> { std::move( __o ) }
{
  DSTART( "ARM_Const8_Unsigned::ARM_Const8_Unsigned(ARM_Const8_Unsigned&&)" );
};


/*
  Destructor.
*/
ARM_Const8_Unsigned::~ARM_Const8_Unsigned( void )
{
  DSTART( "virtual ARM_Const8_Unsigned::~ARM_Const8_Unsigned()" );
};


/*
  Copy-assignment operator.
*/
ARM_Const8_Unsigned & ARM_Const8_Unsigned::operator = ( const ARM_Const8_Unsigned &__o )
{
  DSTART(
    "ARM_Const8_Unsigned& ARM_Const8_Unsigned::operator=(const ARM_Const8_Unsigned&)" );

  WIR_UnsignedImmediateParameter<ARM_Const8_Unsigned>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
ARM_Const8_Unsigned & ARM_Const8_Unsigned::operator = ( ARM_Const8_Unsigned &&__o )
{
  DSTART(
    "ARM_Const8_Unsigned& ARM_Const8_Unsigned::operator=(ARM_Const8_Unsigned&&)" );

  WIR_UnsignedImmediateParameter<ARM_Const8_Unsigned>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
