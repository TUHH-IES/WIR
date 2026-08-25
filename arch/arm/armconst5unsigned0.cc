/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armconst5unsigned0.cc
  @brief This file implements unsigned 5 bits-wide immediate parameters,
         excluding the value '0'.

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
#include <libuseful/io.h>

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
  Default constructor for unsigned const5 parameters.

  The constructor ensures that i lies in the range of values that can be
  represented with 5 bits (unsigned) and that __i is greater than 0.
*/
ARM_Const5_Unsigned0::ARM_Const5_Unsigned0( unsigned long long __i ) :
  WIR_UnsignedImmediateParameter<ARM_Const5_Unsigned0> { __i, 5 }
{
  DSTART(
    "ARM_Const5_Unsigned0::ARM_Const5_Unsigned0(long long unsigned int)" );

  ufAssert( __i > 0 );
};


/*
  Copy constructor.
*/
ARM_Const5_Unsigned0::ARM_Const5_Unsigned0( const ARM_Const5_Unsigned0 &__o ) :
  WIR_UnsignedImmediateParameter<ARM_Const5_Unsigned0> { __o }
{
  DSTART(
    "ARM_Const5_Unsigned0::ARM_Const5_Unsigned0(const ARM_Const5_Unsigned0&)" );
};


/*
  Move constructor.
*/
ARM_Const5_Unsigned0::ARM_Const5_Unsigned0( ARM_Const5_Unsigned0 &&__o ) :
  WIR_UnsignedImmediateParameter<ARM_Const5_Unsigned0> { std::move( __o ) }
{
  DSTART(
    "ARM_Const5_Unsigned0::ARM_Const5_Unsigned0(ARM_Const5_Unsigned0&&)" );
};


/*
  Destructor.
*/
ARM_Const5_Unsigned0::~ARM_Const5_Unsigned0( void )
{
  DSTART( "virtual ARM_Const5_Unsigned0::~ARM_Const5_Unsigned0()" );
};


/*
  Copy-assignment operator.
*/
ARM_Const5_Unsigned0 & ARM_Const5_Unsigned0::operator = ( const ARM_Const5_Unsigned0 &__o )
{
  DSTART(
    "ARM_Const5_Unsigned0& ARM_Const5_Unsigned0::operator=(const ARM_Const5_Unsigned0&)" );

  WIR_UnsignedImmediateParameter<ARM_Const5_Unsigned0>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
ARM_Const5_Unsigned0 & ARM_Const5_Unsigned0::operator = ( ARM_Const5_Unsigned0 &&__o )
{
  DSTART(
    "ARM_Const5_Unsigned0& ARM_Const5_Unsigned0::operator=(ARM_Const5_Unsigned0&&)" );

  WIR_UnsignedImmediateParameter<ARM_Const5_Unsigned0>::operator =
    ( std::move( __o ) );

  return( *this );
};


/*
  setValue sets an unsigned immediate parameter's actual value.

  setValue ensures that __i lies in the range of values that can be represented
  with 5 bits (unsigned) and that __i is greater than 0.
*/
void ARM_Const5_Unsigned0::setValue( unsigned long long i )
{
  DSTART(
    "virtual void ARM_Const5_Unsigned0::setValue(long long unsigned int)" );

  ufAssert( i > 0 );

  WIR_UnsignedImmediateParameter<ARM_Const5_Unsigned0>::setValue( i );
};

}       // namespace WIR
