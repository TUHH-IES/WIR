/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armconst16unsigned.cc
  @brief This file implements unsigned 16 bits-wide immediate parameters.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/arm/armv5t.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for unsigned const16 parameters.

  The constructor ensures that i lies in the range of values that can be
  represented with 16 bits (unsigned).
*/
ARM_Const16_Unsigned::ARM_Const16_Unsigned( unsigned long long __i ) :
  WIR_UnsignedImmediateParameter<ARM_Const16_Unsigned> { __i, 16 }
{
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Copy constructor.
*/
ARM_Const16_Unsigned::ARM_Const16_Unsigned( const ARM_Const16_Unsigned &__o ) :
  WIR_UnsignedImmediateParameter<ARM_Const16_Unsigned> { __o }
{
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Move constructor.
*/
ARM_Const16_Unsigned::ARM_Const16_Unsigned( ARM_Const16_Unsigned &&__o ) :
  WIR_UnsignedImmediateParameter<ARM_Const16_Unsigned> { std::move( __o ) }
{
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
ARM_Const16_Unsigned::~ARM_Const16_Unsigned( void )
{
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Copy-assignment operator.
*/
ARM_Const16_Unsigned & ARM_Const16_Unsigned::operator = ( const ARM_Const16_Unsigned &__o )
{
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_UnsignedImmediateParameter<ARM_Const16_Unsigned>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
ARM_Const16_Unsigned & ARM_Const16_Unsigned::operator = ( ARM_Const16_Unsigned &&__o )
{
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_UnsignedImmediateParameter<ARM_Const16_Unsigned>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
