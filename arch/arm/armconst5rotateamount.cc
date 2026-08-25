/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armconst5rotateamount.cc
  @brief This file implements unsigned 5 bits-wide immediate parameters for
         immediate data-processing operands.

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

  The constructor ensures that __i lies in the range of values that can be
  represented as rotate amounts.
*/
ARM_Const5_RotateAmount::ARM_Const5_RotateAmount( unsigned long long __i ) :
  WIR_UnsignedImmediateParameter<ARM_Const5_RotateAmount> { __i, 5 }
{
  DSTART(
    "ARM_Const5_RotateAmount::ARM_Const5_RotateAmount(long long unsigned int)" );

  ufAssertT(
    __i % 2 == 0,
    "Only even values between 0 and 30 are allowed as rotate amounts." );
};


/*
  Copy constructor.
*/
ARM_Const5_RotateAmount::ARM_Const5_RotateAmount( const ARM_Const5_RotateAmount &__o ) :
  WIR_UnsignedImmediateParameter<ARM_Const5_RotateAmount> { __o }
{
  DSTART(
    "ARM_Const5_RotateAmount::ARM_Const5_RotateAmount(const ARM_Const5_RotateAmount&)" );
};


/*
  Move constructor.
*/
ARM_Const5_RotateAmount::ARM_Const5_RotateAmount( ARM_Const5_RotateAmount &&__o ) :
  WIR_UnsignedImmediateParameter<ARM_Const5_RotateAmount> { std::move( __o ) }
{
  DSTART(
    "ARM_Const5_RotateAmount::ARM_Const5_RotateAmount(ARM_Const5_RotateAmount&&)" );
};


/*
  Destructor.
*/
ARM_Const5_RotateAmount::~ARM_Const5_RotateAmount( void )
{
  DSTART( "virtual ARM_Const5_RotateAmount::~ARM_Const5_RotateAmount()" );
};


/*
  Copy-assignment operator.
*/
ARM_Const5_RotateAmount & ARM_Const5_RotateAmount::operator = ( const ARM_Const5_RotateAmount &__o )
{
  DSTART(
    "ARM_Const5_RotateAmount& ARM_Const5_RotateAmount::operator=(const ARM_Const5_RotateAmount&)" );

  WIR_UnsignedImmediateParameter<ARM_Const5_RotateAmount>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
ARM_Const5_RotateAmount & ARM_Const5_RotateAmount::operator = ( ARM_Const5_RotateAmount &&__o )
{
  DSTART(
    "ARM_Const5_RotateAmount& ARM_Const5_RotateAmount::operator=(ARM_Const5_RotateAmount&&)" );

  WIR_UnsignedImmediateParameter<ARM_Const5_RotateAmount>::operator =
    ( std::move( __o ) );

  return( *this );
};


/*
  setValue sets an unsigned immediate parameter's actual value.

  setValue ensures that i lies in the range of values that can be represented as
  rotate amounts.
*/
void ARM_Const5_RotateAmount::setValue( unsigned long long i )
{
  DSTART(
    "virtual void ARM_Const5_RotateAmount::setValue(long long unsigned int)" );

  ufAssertT(
    i % 2 == 0,
    "Only even values between 0 and 30 are allowed as rotate amounts." );

  WIR_UnsignedImmediateParameter<ARM_Const5_RotateAmount>::setValue( i );
};

}       // namespace WIR
