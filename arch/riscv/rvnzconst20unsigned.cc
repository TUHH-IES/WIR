/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2026, Heiko Falk.

*/

/*!
  @file rvnzconst20unsigned.cc
  @brief This file implements unsigned, non-zero 20 bits-wide immediate
         parameters from the subintervals [0x1, 0x1f] and [0xfffe0, 0xfffff].

  @author Heiko Falk<Heiko.Falk.de>
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
  Default constructor for unsigned non-zero const20 parameters.

  The constructor ensures that __i lies in the range of values that can be
  represented with 20 bits (unsigned) from the subintervals [0x1, 0x1f] and
  [0xfffe0, 0xfffff].

  See also:
  https://github.com/riscv-non-isa/riscv-asm-manual/pull/66/commits/6196c03643af1ea2dff103b58565e38776b0c34d
*/
RV_NZConst20_Unsigned::RV_NZConst20_Unsigned( unsigned long long __i ) :
  WIR_UnsignedImmediateParameter<RV_NZConst20_Unsigned> { __i, 20 }
{
  DSTART(
    "RV_NZConst20_Unsigned::RV_NZConst20_Unsigned(long long unsigned int)" );

  ufAssert(
    ( ( __i >= 0x1 ) && ( __i <= 0x1F ) ) ||
    ( ( __i >= 0xFFFE0 ) && ( __i <= 0xFFFFF ) ) );
};


/*
  Copy constructor.
*/
RV_NZConst20_Unsigned::RV_NZConst20_Unsigned( const RV_NZConst20_Unsigned &__o ) :
  WIR_UnsignedImmediateParameter<RV_NZConst20_Unsigned> { __o }
{
  DSTART(
    "RV_NZConst20_Unsigned::RV_NZConst20_Unsigned(const "
    "RV_NZConst20_Unsigned&)" );
};


/*
  Move constructor.
*/
RV_NZConst20_Unsigned::RV_NZConst20_Unsigned( RV_NZConst20_Unsigned &&__o ) :
  WIR_UnsignedImmediateParameter<RV_NZConst20_Unsigned> { std::move( __o ) }
{
  DSTART(
    "RV_NZConst20_Unsigned::RV_NZConst20_Unsigned(RV_NZConst20_Unsigned&&)" );
};


/*
  Destructor.
*/
RV_NZConst20_Unsigned::~RV_NZConst20_Unsigned( void )
{
  DSTART( "virtual RV_NZConst20_Unsigned::~RV_NZConst20_Unsigned()" );
};


/*
  Copy-assignment operator.
*/
RV_NZConst20_Unsigned & RV_NZConst20_Unsigned::operator = ( const RV_NZConst20_Unsigned &__o )
{
  DSTART(
    "RV_NZConst20_Unsigned& RV_NZConst20_Unsigned::operator=(const "
    "RV_NZConst20_Unsigned&)" );

  WIR_UnsignedImmediateParameter<RV_NZConst20_Unsigned>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV_NZConst20_Unsigned & RV_NZConst20_Unsigned::operator = ( RV_NZConst20_Unsigned &&__o )
{
  DSTART(
    "RV_NZConst20_Unsigned& RV_NZConst20_Unsigned::operator=("
    "RV_NZConst20_Unsigned&&)" );

  WIR_UnsignedImmediateParameter<RV_NZConst20_Unsigned>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
