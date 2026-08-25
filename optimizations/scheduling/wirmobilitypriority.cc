/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirmobilitypriority.cc
  @brief This file implements an abstract base class computing the mobility
         scheduling priority for %WIR operations.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <limits>

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>

// Include local headers
#include "wirmobilitypriority.h"


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor.
*/
WIR_MobilityPriority::WIR_MobilityPriority( void ) :
  WIR_SchedulingPriority {}
{
  DSTART( "WIR_MobilityPriority::WIR_MobilityPriority()" );
};


/*
  Copy constructor.
*/
WIR_MobilityPriority::WIR_MobilityPriority( const WIR_MobilityPriority &__o ) :
  WIR_SchedulingPriority { __o }
{
  DSTART(
    "WIR_MobilityPriority::WIR_MobilityPriority(const WIR_MobilityPriority&)" );
};


/*
  Destructor.
*/
WIR_MobilityPriority::~WIR_MobilityPriority( void )
{
  DSTART( "virtual WIR_MobilityPriority::~WIR_MobilityPriority()" );
};


/*
  Copy-assignment operator.
*/
WIR_MobilityPriority & WIR_MobilityPriority::operator = ( const WIR_MobilityPriority &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_SchedulingPriority::operator = ( __o );

  return( *this );
};


/*
  getPriority determines an operation's scheduling priority using the
  operation's mobility.

  Operations with only limited mobility receive a high priority.
*/
long long WIR_MobilityPriority::getPriority( const WIR_Operation &o ) const
{
  DSTART(
    "virtual long long int WIR_MobilityPriority::getPriority(const WIR_Operation&) const" );

  auto it = mMobilityMap->find( o.getID() );

  #ifdef FAILSAFEMOE
  ufAssert( it != mMobilityMap->end() );
  #endif

  return( numeric_limits<long long>::max() - it->second );
};

}       // namespace WIR
