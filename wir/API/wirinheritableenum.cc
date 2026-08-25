/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirinheritableenum.cc
  @brief This file implements an API for inheritable enumeration classes.

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
#include "wirinheritableenum.h"


//
// Code section
//

namespace WIR {


using namespace std;


unsigned int WIR_InheritableEnum::mMaxValue = 0;


//
// Public class methods
//

/*
  The == operator checks for equality of enumerators.
*/
bool WIR_InheritableEnum::operator == ( const WIR_InheritableEnum & __o ) const
{
  DSTART(
    "bool WIR_InheritableEnum::operator==(const WIR_InheritableEnum&) const" );

  return( mID == __o.mID );
};


/*
  The != operator checks for inequality of enumerators.
*/
bool WIR_InheritableEnum::operator != ( const WIR_InheritableEnum & __o ) const
{
  DSTART(
    "bool WIR_InheritableEnum::operator!=(const WIR_InheritableEnum&) const" );

  return( mID != __o.mID );
};


/*
  The < operator checks for less-than of enumerators.
*/
bool WIR_InheritableEnum::operator < ( const WIR_InheritableEnum & __o ) const
{
  DSTART(
    "bool WIR_InheritableEnum::operator<(const WIR_InheritableEnum&) const" );

  return( mID < __o.mID );
};


//
// Protected class methods
//

/*
  Default constructor assigning a new unique ID to an enumerator.
*/
WIR_InheritableEnum::WIR_InheritableEnum() :
  mID { mMaxValue++ }
{
  DSTART( "WIR_InheritableEnum::WIR_InheritableEnum()" );
};

}       // namespace WIR
