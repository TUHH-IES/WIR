/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file mipsimmediate16signed.cc
  @brief This file implements signed 16-bit immediate parameters.

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
#include <arch/generic/mips.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for signed 16-bit immediate parameters.

  The constructor ensures that __i lies in the range of values that can be
  represented with 16 bits, assuming two's-complement as underlying data format.
*/
MIPS_Immediate16_Signed::MIPS_Immediate16_Signed( signed long long __i ) :
  WIR_SignedImmediateParameter<MIPS_Immediate16_Signed> { __i, 16 }
{
  DSTART( "MIPS_Immediate16_Signed::MIPS_Immediate16_Signed(long long int)" );
};


/*
  Copy constructor.
*/
MIPS_Immediate16_Signed::MIPS_Immediate16_Signed( const MIPS_Immediate16_Signed &__o ) :
  WIR_SignedImmediateParameter<MIPS_Immediate16_Signed> { __o }
{
  DSTART(
    "MIPS_Immediate16_Signed::MIPS_Immediate16_Signed(const MIPS_Immediate16_Signed&)" );
};


/*
  Move constructor.
*/
MIPS_Immediate16_Signed::MIPS_Immediate16_Signed( MIPS_Immediate16_Signed &&__o ) :
  WIR_SignedImmediateParameter<MIPS_Immediate16_Signed> { std::move( __o ) }
{
  DSTART(
    "MIPS_Immediate16_Signed::MIPS_Immediate16_Signed(MIPS_Immediate16_Signed&&)" );
};


/*
  Destructor.
*/
MIPS_Immediate16_Signed::~MIPS_Immediate16_Signed( void )
{
  DSTART( "virtual MIPS_Immediate16_Signed::~MIPS_Immediate16_Signed()" );
};


/*
  Copy-assignment operator.
*/
MIPS_Immediate16_Signed & MIPS_Immediate16_Signed::operator = ( const MIPS_Immediate16_Signed &__o )
{
  DSTART(
    "MIPS_Immediate16_Signed& MIPS_Immediate16_Signed::operator=(const MIPS_Immediate16_Signed&)" );

  WIR_SignedImmediateParameter<MIPS_Immediate16_Signed>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
MIPS_Immediate16_Signed & MIPS_Immediate16_Signed::operator = ( MIPS_Immediate16_Signed &&__o )
{
  DSTART(
    "MIPS_Immediate16_Signed& MIPS_Immediate16_Signed::operator=(MIPS_Immediate16_Signed&&)" );

  WIR_SignedImmediateParameter<MIPS_Immediate16_Signed>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
