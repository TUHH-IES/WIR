/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armregvirtual.cc
  @brief This file implements virtual ARM general-purpose registers.

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
  Default constructor for virtual general-purpose registers.
*/
ARM_RegV::ARM_RegV( void ) :
  WIR_VirtualRegister { ARMv4::RegisterType::reg }
{
  DSTART( "ARM_RegV::ARM_RegV()" );
};


/*
  Copy constructor.
*/
ARM_RegV::ARM_RegV( const ARM_RegV &__o ) :
  WIR_VirtualRegister { __o }
{
  DSTART( "ARM_RegV::ARM_RegV(const ARM_RegV&)" );
};


/*
  Move constructor.
*/
ARM_RegV::ARM_RegV( ARM_RegV &&__o ) :
  WIR_VirtualRegister { std::move( __o ) }
{
  DSTART( "ARM_RegV::ARM_RegV(ARM_RegV&&)" );
};


/*
  Destructor.
*/
ARM_RegV::~ARM_RegV( void )
{
  DSTART( "virtual ARM_RegV::~ARM_RegV()" );
};


/*
  Copy-assignment operator.
*/
ARM_RegV & ARM_RegV::operator = ( const ARM_RegV &__o )
{
  DSTART( "ARM_RegV& ARM_RegV::operator=(const ARM_RegV&)" );

  WIR_VirtualRegister::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
ARM_RegV & ARM_RegV::operator = ( ARM_RegV &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_VirtualRegister::operator = ( std::move( __o ) );

  return( *this );
};


//
// Protected class methods
//

/*
  clone creates a copy of a virtual ARM general-purpose register.
*/
ARM_RegV *ARM_RegV::clone( void ) const
{
  DSTART( "virtual ARM_RegV* ARM_RegV::clone() const" );

  return( new ARM_RegV( *this ) );
};

}       // namespace WIR
