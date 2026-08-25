/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvconst6signed.cc
  @brief This file implements signed 6 bits-wide immediate parameters.

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
  Default constructor for signed const6 parameters.

  The constructor ensures that i lies in the range of values that can be
  represented with 6 bits, assuming two's-complement as underlying data format.
*/
RV_Const6_Signed::RV_Const6_Signed( signed long long __i ) :
  WIR_SignedImmediateParameter<RV_Const6_Signed> { __i, 6 }
{
  DSTART( "RV_Const6_Signed::RV_Const6_Signed(long long int)" );
};


/*
  Copy constructor.
*/
RV_Const6_Signed::RV_Const6_Signed( const RV_Const6_Signed &__o ) :
  WIR_SignedImmediateParameter<RV_Const6_Signed> { __o }
{
  DSTART( "RV_Const6_Signed::RV_Const6_Signed(const RV_Const6_Signed&)" );
};


/*
  Move constructor.
*/
RV_Const6_Signed::RV_Const6_Signed( RV_Const6_Signed &&__o ) :
  WIR_SignedImmediateParameter<RV_Const6_Signed> { std::move( __o ) }
{
  DSTART( "RV_Const6_Signed::RV_Const6_Signed(RV_Const6_Signed&&)" );
};


/*
  Destructor.
*/
RV_Const6_Signed::~RV_Const6_Signed( void )
{
  DSTART( "virtual RV_Const6_Signed::~RV_Const6_Signed()" );
};


/*
  Copy-assignment operator.
*/
RV_Const6_Signed & RV_Const6_Signed::operator = ( const RV_Const6_Signed &__o )
{
  DSTART(
    "RV_Const6_Signed& RV_Const6_Signed::operator=(const RV_Const6_Signed&)" );

  WIR_SignedImmediateParameter<RV_Const6_Signed>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV_Const6_Signed & RV_Const6_Signed::operator = ( RV_Const6_Signed &&__o )
{
  DSTART(
    "RV_Const6_Signed& RV_Const6_Signed::operator=(RV_Const6_Signed&&)" );

  WIR_SignedImmediateParameter<RV_Const6_Signed>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
