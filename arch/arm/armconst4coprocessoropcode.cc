/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armconst4coprocessoropcode.cc
  @brief This file implements unsigned 4 bits-wide immediate parameters for
         coprocessor opcodes.

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
  Default constructor for unsigned const4 parameters.

  The constructor ensures that __i lies in the range of values that can be
  represented with 4 bits (unsigned).
*/
ARM_Const4_CoprocessorOpcode::ARM_Const4_CoprocessorOpcode( unsigned long long __i ) :
  WIR_UnsignedImmediateParameter<ARM_Const4_CoprocessorOpcode> { __i, 4 }
{
  DSTART(
    "ARM_Const4_CoprocessorOpcode::ARM_Const4_CoprocessorOpcode(long long unsigned int)" );
};


/*
  Copy constructor.
*/
ARM_Const4_CoprocessorOpcode::ARM_Const4_CoprocessorOpcode( const ARM_Const4_CoprocessorOpcode &__o ) :
  WIR_UnsignedImmediateParameter<ARM_Const4_CoprocessorOpcode> { __o }
{
  DSTART(
    "ARM_Const4_CoprocessorOpcode::ARM_Const4_CoprocessorOpcode(const ARM_Const4_CoprocessorOpcode&)" );
};


/*
  Move constructor.
*/
ARM_Const4_CoprocessorOpcode::ARM_Const4_CoprocessorOpcode( ARM_Const4_CoprocessorOpcode &&__o ) :
  WIR_UnsignedImmediateParameter<ARM_Const4_CoprocessorOpcode> {
    std::move( __o ) }
{
  DSTART(
    "ARM_Const4_CoprocessorOpcode::ARM_Const4_CoprocessorOpcode(ARM_Const4_CoprocessorOpcode&&)" );
};


/*
  Destructor.
*/
ARM_Const4_CoprocessorOpcode::~ARM_Const4_CoprocessorOpcode( void )
{
  DSTART(
    "virtual ARM_Const4_CoprocessorOpcode::~ARM_Const4_CoprocessorOpcode()" );
};


/*
  Copy-assignment operator.
*/
ARM_Const4_CoprocessorOpcode & ARM_Const4_CoprocessorOpcode::operator = ( const ARM_Const4_CoprocessorOpcode &__o )
{
  DSTART(
    "ARM_Const4_CoprocessorOpcode& ARM_Const4_CoprocessorOpcode::operator=(const ARM_Const4_CoprocessorOpcode&)" );

  WIR_UnsignedImmediateParameter<ARM_Const4_CoprocessorOpcode>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
ARM_Const4_CoprocessorOpcode & ARM_Const4_CoprocessorOpcode::operator = ( ARM_Const4_CoprocessorOpcode &&__o )
{
  DSTART(
    "ARM_Const4_CoprocessorOpcode& ARM_Const4_CoprocessorOpcode::operator=(ARM_Const4_CoprocessorOpcode&&)" );

  WIR_UnsignedImmediateParameter<ARM_Const4_CoprocessorOpcode>::operator =
    ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
