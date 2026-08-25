/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcmobilitypriority.cc
  @brief This file implements a base class computing the mobility scheduling
         priority for TriCore operations.

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

// Include local headers
#include "tcmobilitypriority.h"


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
TC_MobilityPriority::TC_MobilityPriority( void ) :
  WIR_MobilityPriority {},
  TC_SchedulingPriority {}
{
  DSTART( "TC_MobilityPriority::TC_MobilityPriority()" );
};


/*
  Copy constructor.
*/
TC_MobilityPriority::TC_MobilityPriority( const TC_MobilityPriority &__o ) :
  WIR_SchedulingPriority { __o },
  WIR_MobilityPriority { __o },
  TC_SchedulingPriority { __o }
{
  DSTART(
    "TC_MobilityPriority::TC_MobilityPriority(const TC_MobilityPriority&)" );
};


/*
  Destructor.
*/
TC_MobilityPriority::~TC_MobilityPriority( void )
{
  DSTART( "virtual TC_MobilityPriority::~TC_MobilityPriority()" );
};


/*
  Copy-assignment operator.
*/
TC_MobilityPriority & TC_MobilityPriority::operator = ( const TC_MobilityPriority &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_MobilityPriority::operator = ( __o );
  TC_SchedulingPriority::operator = ( __o );

  return( *this );
};


//
// Protected class methods
//

/*
  clone creates a copy of a TriCore mobility scheduling priority.
*/
WIR_SchedulingPriority *TC_MobilityPriority::clone( void ) const
{
  DSTART(
    "virtual WIR_SchedulingPriority* TC_MobilityPriority::clone() const" );

  return( new TC_MobilityPriority( *this ) );
};

}       // namespace WIR
