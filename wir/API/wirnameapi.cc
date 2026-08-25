/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirnameapi.cc
  @brief This file implements a base class for managing names of named derived
         classes.

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

// Include private headers
#include "wirnameapi.h"


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  setName sets an object's specific name.
*/
void WIR_Name_API::setName( const std::string &s )
{
  DSTART( "void WIR_Name_API::setName(const string&)" );

  mName = s;
};


/*
  setName sets an object's specific name.
*/
void WIR_Name_API::setName( std::string &&s )
{
  DSTART( "void WIR_Name_API::setName(string&&)" );

  mName = std::move( s );
  s.clear();
};


/*
  getName returns an object's specific name.
*/
std::string WIR_Name_API::getName( void ) const
{
  DSTART( "string WIR_Name_API::getName() const" );

  return( mName );
};


//
// Protected class methods
//

/*
  Default constructor assigning an empty name.
*/
WIR_Name_API::WIR_Name_API( void ) :
  mName { "" }
{
  DSTART( "WIR_Name_API::WIR_Name_API()" );
};


/*
  Copy constructor.
*/
WIR_Name_API::WIR_Name_API( const WIR_Name_API &__o ) :
  mName { __o.mName }
{
  DSTART( "WIR_Name_API::WIR_Name_API(const WIR_Name_API&)" );
};


/*
  Move constructor.
*/
WIR_Name_API::WIR_Name_API( WIR_Name_API &&__o ) :
  mName { std::move( __o.mName ) }
{
  DSTART( "WIR_Name_API::WIR_Name_API(WIR_Name_API&&)" );

  __o.mName.clear();
};


/*
  Destructor.
*/
WIR_Name_API::~WIR_Name_API( void )
{
  DSTART( "virtual WIR_Name_API::~WIR_Name_API()" );
};


/*
  Copy-assignment operator.
*/
WIR_Name_API & WIR_Name_API::operator = ( const WIR_Name_API &__o )
{
  DSTART( "WIR_Name_API& WIR_Name_API::operator=(const WIR_Name_API&)" );

  mName = __o.mName;

  return( *this );
};


/*
  Move-assignment operator.
*/
WIR_Name_API & WIR_Name_API::operator = ( WIR_Name_API &&__o )
{
  DSTART( "WIR_Name_API& WIR_Name_API::operator=(WIR_Name_API&&)" );

  mName = std::move( __o.mName );
  __o.mName.clear();

  return( *this );
};

}       // namespace WIR
