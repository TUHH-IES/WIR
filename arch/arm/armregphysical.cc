/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armregphysical.cc
  @brief This file implements physical ARM general-purpose registers.

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
ARM_RegP::~ARM_RegP( void )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


//
// Private class methods
//

/*
  Default constructor for physical general-purpose registers.
*/
ARM_RegP::ARM_RegP( const std::string &__s, bool __sp ) :
  WIR_PhysicalRegister { ARMv4::RegisterType::reg, __s, __sp }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Copy constructor.
*/
ARM_RegP::ARM_RegP( const ARM_RegP &__o ) :
  WIR_PhysicalRegister { __o }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Copy-assignment operator.
*/
ARM_RegP & ARM_RegP::operator = ( const ARM_RegP &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_PhysicalRegister::operator = ( __o );

  return( *this );
};


/*
  clone creates a copy of a physical ARM general-purpose register.
*/
ARM_RegP *ARM_RegP::clone( void ) const
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  return( new ARM_RegP( *this ) );
};

}       // namespace WIR
