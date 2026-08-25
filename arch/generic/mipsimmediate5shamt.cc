/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file mipsimmediate5shamt.cc
  @brief This file implements 5-bit shift amount parameters.

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
  Default constructor for 5-bit shift amount parameters.

  The constructor ensures that __i lies in the range of values that can be
  represented with 5 bits (unsigned).
*/
MIPS_Immediate5_Shamt::MIPS_Immediate5_Shamt( unsigned long long __i ) :
  WIR_UnsignedImmediateParameter<MIPS_Immediate5_Shamt> { __i, 5 }
{
  DSTART(
    "MIPS_Immediate5_Shamt::MIPS_Immediate5_Shamt(long long unsigned int)" );
};


/*
  Copy constructor.
*/
MIPS_Immediate5_Shamt::MIPS_Immediate5_Shamt( const MIPS_Immediate5_Shamt &__o ) :
  WIR_UnsignedImmediateParameter<MIPS_Immediate5_Shamt> { __o }
{
  DSTART(
    "MIPS_Immediate5_Shamt::MIPS_Immediate5_Shamt(const MIPS_Immediate5_Shamt&)" );
};


/*
  Move constructor.
*/
MIPS_Immediate5_Shamt::MIPS_Immediate5_Shamt( MIPS_Immediate5_Shamt &&__o ) :
  WIR_UnsignedImmediateParameter<MIPS_Immediate5_Shamt> { std::move( __o ) }
{
  DSTART(
    "MIPS_Immediate5_Shamt::MIPS_Immediate5_Shamt(MIPS_Immediate5_Shamt&&)" );
};


/*
  Destructor.
*/
MIPS_Immediate5_Shamt::~MIPS_Immediate5_Shamt( void )
{
  DSTART( "virtual MIPS_Immediate5_Shamt::~MIPS_Immediate5_Shamt()" );
};


/*
  Copy-assignment operator.
*/
MIPS_Immediate5_Shamt & MIPS_Immediate5_Shamt::operator = ( const MIPS_Immediate5_Shamt &__o )
{
  DSTART(
    "MIPS_Immediate5_Shamt& MIPS_Immediate5_Shamt::operator=(const MIPS_Immediate5_Shamt&)" );

  WIR_UnsignedImmediateParameter<MIPS_Immediate5_Shamt>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
MIPS_Immediate5_Shamt & MIPS_Immediate5_Shamt::operator = ( MIPS_Immediate5_Shamt &&__o )
{
  DSTART(
    "MIPS_Immediate5_Shamt& MIPS_Immediate5_Shamt::operator=(MIPS_Immediate5_Shamt&&)" );

  WIR_UnsignedImmediateParameter<MIPS_Immediate5_Shamt>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
