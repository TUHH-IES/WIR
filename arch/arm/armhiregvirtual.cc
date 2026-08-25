/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armhiregvirtual.cc
  @brief This file implements high virtual ARM registers.

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
  Default constructor for high virtual registers.
*/
ARM_HiRegV::ARM_HiRegV( void ) :
  WIR_VirtualRegister { ARMv4::RegisterType::hi }
{
  DSTART( "ARM_HiRegV::ARM_HiRegV()" );
};


/*
  Copy constructor.
*/
ARM_HiRegV::ARM_HiRegV( const ARM_HiRegV &__o ) :
  WIR_VirtualRegister { __o }
{
  DSTART( "ARM_HiRegV::ARM_HiRegV(const ARM_HiRegV&)" );
};


/*
  Move constructor.
*/
ARM_HiRegV::ARM_HiRegV( ARM_HiRegV &&__o ) :
  WIR_VirtualRegister { std::move( __o ) }
{
  DSTART( "ARM_HiRegV::ARM_HiRegV(ARM_HiRegV&&)" );
};


/*
  Destructor.
*/
ARM_HiRegV::~ARM_HiRegV( void )
{
  DSTART( "virtual ARM_HiRegV::~ARM_HiRegV()" );
};


/*
  Copy-assignment operator.
*/
ARM_HiRegV & ARM_HiRegV::operator = ( const ARM_HiRegV &__o )
{
  DSTART( "ARM_HiRegV& ARM_HiRegV::operator=(const ARM_HiRegV&)" );

  WIR_VirtualRegister::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
ARM_HiRegV & ARM_HiRegV::operator = ( ARM_HiRegV &&__o )
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
  clone creates a copy of a high virtual ARM register.
*/
ARM_HiRegV *ARM_HiRegV::clone( void ) const
{
  DSTART( "virtual ARM_HiRegV* ARM_HiRegV::clone() const" );

  return( new ARM_HiRegV( *this ) );
};

}       // namespace WIR
