/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvconst6unsigned.cc
  @brief This file implements unsigned 6 bits-wide immediate parameters.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
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
  Default constructor for unsigned const6 parameters.

  The constructor ensures that i lies in the range of values that can be
  represented with 6 bits (unsigned).
*/
RV_Const6_Unsigned::RV_Const6_Unsigned( unsigned long long __i ) :
  WIR_UnsignedImmediateParameter<RV_Const6_Unsigned> { __i, 6 }
{
  DSTART( "RV_Const6_Unsigned::RV_Const6_Unsigned(long long unsigned int)" );
};


/*
  Copy constructor.
*/
RV_Const6_Unsigned::RV_Const6_Unsigned( const RV_Const6_Unsigned &__o ) :
  WIR_UnsignedImmediateParameter<RV_Const6_Unsigned> { __o }
{
  DSTART( "RV_Const6_Unsigned::RV_Const6_Unsigned(const RV_Const6_Unsigned&)" );
};


/*
  Move constructor.
*/
RV_Const6_Unsigned::RV_Const6_Unsigned( RV_Const6_Unsigned &&__o ) :
  WIR_UnsignedImmediateParameter<RV_Const6_Unsigned> { std::move( __o ) }
{
  DSTART( "RV_Const6_Unsigned::RV_Const6_Unsigned(RV_Const6_Unsigned&&)" );
};


/*
  Destructor.
*/
RV_Const6_Unsigned::~RV_Const6_Unsigned( void )
{
  DSTART( "virtual RV_Const6_Unsigned::~RV_Const6_Unsigned()" );
};


/*
  Copy-assignment operator.
*/
RV_Const6_Unsigned & RV_Const6_Unsigned::operator = ( const RV_Const6_Unsigned &__o )
{
  DSTART(
    "RV_Const6_Unsigned& RV_Const6_Unsigned::operator=(const"
    " RV_Const6_Unsigned&)" );

  WIR_UnsignedImmediateParameter<RV_Const6_Unsigned>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV_Const6_Unsigned & RV_Const6_Unsigned::operator = ( RV_Const6_Unsigned &&__o )
{
  DSTART(
    "RV_Const6_Unsigned& RV_Const6_Unsigned::operator=(RV_Const6_Unsigned&&)" );

  WIR_UnsignedImmediateParameter<RV_Const6_Unsigned>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
