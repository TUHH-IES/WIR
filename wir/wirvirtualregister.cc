/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirvirtualregister.cc
  @brief This file implements virtual %WIR registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <iostream>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for virtual registers.
*/
WIR_VirtualRegister::WIR_VirtualRegister( const WIR_BaseProcessor::RegisterType &__r ) :
  WIR_Register<WIR_VirtualRegister, true> { __r }
{
  DSTART(
    "WIR_VirtualRegister::WIR_VirtualRegister(const WIR_BaseProcessor::RegisterType&)" );
};


/*
  Copy constructor.

  When copying a register that is inserted in some WIR function, the resulting
  copy will not be inserted in a function.
*/
WIR_VirtualRegister::WIR_VirtualRegister( const WIR_VirtualRegister &__o ) :
  WIR_Register<WIR_VirtualRegister, true> { __o }
{
  DSTART(
    "WIR_VirtualRegister::WIR_VirtualRegister(const WIR_VirtualRegister&)" );
};


/*
  Move constructor.

  Trying to move a register that is inserted in some WIR function results in an
  assertion, since you are not allowed to move a register whose ownership is
  managed by a function.
*/
WIR_VirtualRegister::WIR_VirtualRegister( WIR_VirtualRegister &&__o ) :
  WIR_Register<WIR_VirtualRegister, true> { std::move( __o ) }
{
  DSTART( "WIR_VirtualRegister::WIR_VirtualRegister(WIR_VirtualRegister&&)" );
};


/*
  Destructor.
*/
WIR_VirtualRegister::~WIR_VirtualRegister( void )
{
  DSTART( "virtual WIR_VirtualRegister::~WIR_VirtualRegister()" );
};


/*
  Copy-assignment operator.

  When copying a register that is inserted in some %WIR function, the resulting
  copy will not be inserted in a function.
*/
WIR_VirtualRegister & WIR_VirtualRegister::operator = ( const WIR_VirtualRegister &__o )
{
  DSTART(
    "WIR_VirtualRegister& WIR_VirtualRegister::operator=(const WIR_VirtualRegister&)" );

  WIR_Register<WIR_VirtualRegister, true>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.

  Trying to move a register that is inserted in some %WIR function results in an
  assertion, since you are not allowed to move a register whose ownership is
  managed by a function.
*/
WIR_VirtualRegister & WIR_VirtualRegister::operator = ( WIR_VirtualRegister &&__o )
{
  DSTART(
    "WIR_VirtualRegister& WIR_VirtualRegister::operator=(WIR_VirtualRegister&&)" );

  WIR_Register<WIR_VirtualRegister, true>::operator = ( std::move( __o ) );

  return( *this );
};

}       // namespace WIR
