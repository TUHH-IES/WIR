/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armhiregphysical.cc
  @brief This file implements high physical ARM registers.

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
  Destructor.
*/
ARM_HiRegP::~ARM_HiRegP( void )
{
  DSTART( "virtual ARM_HiRegP::~ARM_HiRegP()" );
};


//
// Private class methods
//

/*
  Default constructor for high physical registers.
*/
ARM_HiRegP::ARM_HiRegP( const std::string &__s, bool __sp ) :
  WIR_PhysicalRegister { ARMv4::RegisterType::hi, __s, __sp }
{
  DSTART( "ARM_HiRegP::ARM_HiRegP(const string&, bool)" );
};


/*
  Copy constructor.
*/
ARM_HiRegP::ARM_HiRegP( const ARM_HiRegP &__o ) :
  WIR_PhysicalRegister { __o }
{
  DSTART( "ARM_HiRegP::ARM_HiRegP(const ARM_HiRegP&)" );
};


/*
  Copy-assignment operator.
*/
ARM_HiRegP & ARM_HiRegP::operator = ( const ARM_HiRegP &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_PhysicalRegister::operator = ( __o );

  return( *this );
};


/*
  clone creates a copy of a high physical ARM register.
*/
ARM_HiRegP *ARM_HiRegP::clone( void ) const
{
  DSTART( "virtual ARM_HiRegP* ARM_HiRegP::clone() const" );

  return( new ARM_HiRegP( *this ) );
};

}       // namespace WIR
