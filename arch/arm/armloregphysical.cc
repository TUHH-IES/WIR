/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armloregphysical.cc
  @brief This file implements low physical ARM registers.

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
ARM_LoRegP::~ARM_LoRegP( void )
{
  DSTART( "virtual ARM_LoRegP::~ARM_LoRegP()" );
};


//
// Private class methods
//

/*
  Default constructor for low physical registers.
*/
ARM_LoRegP::ARM_LoRegP( const std::string &__s, bool __sp ) :
  WIR_PhysicalRegister { ARMv4::RegisterType::lo, __s, __sp }
{
  DSTART( "ARM_LoRegP::ARM_LoRegP(const string&, bool)" );
};


/*
  Copy constructor.
*/
ARM_LoRegP::ARM_LoRegP( const ARM_LoRegP &__o ) :
  WIR_PhysicalRegister { __o }
{
  DSTART( "ARM_LoRegP::ARM_LoRegP(const ARM_LoRegP&)" );
};


/*
  Copy-assignment operator.
*/
ARM_LoRegP & ARM_LoRegP::operator = ( const ARM_LoRegP &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_PhysicalRegister::operator = ( __o );

  return( *this );
};


/*
  clone creates a copy of a low physical ARM register.
*/
ARM_LoRegP *ARM_LoRegP::clone( void ) const
{
  DSTART( "virtual ARM_LoRegP* ARM_LoRegP::clone() const" );

  return( new ARM_LoRegP( *this ) );
};

}       // namespace WIR
