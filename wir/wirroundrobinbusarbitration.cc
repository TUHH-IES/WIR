/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirroundrobinbusarbitration.cc
  @brief This file implements a round robin %WIR bus arbitration policy.

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
WIR_RoundRobinBusArbitration::~WIR_RoundRobinBusArbitration( void )
{
  DSTART(
    "virtual WIR_RoundRobinBusArbitration::~WIR_RoundRobinBusArbitration()" );
};


/*
  This operator compares two WIR_BusArbitrations for equality.
*/
bool WIR_RoundRobinBusArbitration::operator == ( const WIR_BusArbitration &__o ) const
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  auto *p = dynamic_cast<const WIR_RoundRobinBusArbitration *>( &__o );
  return( p != nullptr );
};


/*
  This operator compares two WIR_BusArbitrations for inequality.
*/
bool WIR_RoundRobinBusArbitration::operator != ( const WIR_BusArbitration &__o ) const
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  return( !( *this == __o ) );
};


//
// Protected class methods
//

/*
  clone creates a copy of a round robin bus arbiter.
*/
WIR_RoundRobinBusArbitration *WIR_RoundRobinBusArbitration::clone( void ) const
{
  DSTART(
    "virtual WIR_RoundRobinBusArbitration* WIR_RoundRobinBusArbitration::clone() const" );

  return( new WIR_RoundRobinBusArbitration( *this ) );
};

}       // namespace WIR
