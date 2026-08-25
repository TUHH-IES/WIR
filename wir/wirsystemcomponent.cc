/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirsystemcomponent.cc
  @brief This file implements generic %WIR system components.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <algorithm>

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>
#include <libuseful/io.h>

// Include WIR headers
#include <wir/API/wirinsertionapi.h>
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
WIR_SystemComponent::~WIR_SystemComponent( void )
{
  DSTART( "virtual WIR_SystemComponent::~WIR_SystemComponent()" );
};


//
// API implementations.
//

WIR_INSERTION_IMPL( WIR_System, System, WIR_SystemComponent );


/*
  getName returns a system component's specific name.
*/
std::string WIR_SystemComponent::getName( void ) const
{
  DSTART( "string WIR_SystemComponent::getName() const" );

  return( mName );
};


/*
  getAddressRanges returns the set of address ranges for which this system
  component is active.
*/
const WIR_AddressRangeSet &WIR_SystemComponent::getAddressRanges( void ) const
{
  DSTART(
    "const WIR_AddressRangeSet& WIR_SystemComponent::getAddressRanges() const" );

  return( mAddressRanges );
};


/*
  getHullRange returns the convex hull over all address ranges for which this
  system component is active.

  Obviously, getHullRange produces an address range that may contain addresses
  for which this system component is not active, since these addresses lie
    between other, active ranges.
*/
const WIR_AddressRange WIR_SystemComponent::getHullRange( void ) const
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_AddressRange res;

  for ( const auto &ar : mAddressRanges )
    res.unite( ar );

  return( res );
};


/*
  isActiveInRange returns whether this system component is active for the
  specified address range.

  Derived classes may overwrite this method if they need to support different
  conditions for enabling system components other than the active address range
  assignment.
*/
bool WIR_SystemComponent::isActiveInRange( const WIR_AddressRange &r ) const
{
  DSTART(
    "virtual bool WIR_SystemComponent::isActiveInRange(const WIR_AddressRange&) const" );

  return(
    any_of(
      mAddressRanges.begin(), mAddressRanges.end(),
      [&]( const WIR_AddressRange &ar ) {
        return( !r.hasEmptyIntersection( ar ) ); } ) );
};


//
// Protected class methods
//

/*
  Default constructor creating a named system component.

  This constructor asserts if it is passed an empty string.
*/
WIR_SystemComponent::WIR_SystemComponent( const std::string &s ) :
  WIR_ID_API {}
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  ufAssert( !s.empty() );
  setName( s );
};


/*
  Default constructor creating a named system component.

  This constructor asserts if it is passed an empty string.
*/
WIR_SystemComponent::WIR_SystemComponent( std::string &&s ) :
  WIR_ID_API {}
{
  DSTART( "WIR_SystemComponent::WIR_SystemComponent(string&&)" );

  ufAssert( !s.empty() );
  setName( std::move( s ) );
};


/*
  Copy constructor.
*/
WIR_SystemComponent::WIR_SystemComponent( const WIR_SystemComponent &__o ) :
  WIR_ID_API { __o },
  mSystemPointer { nullptr },
  mName { __o.mName },
  mAddressRanges { __o.mAddressRanges }
{
  DSTART(
    "WIR_SystemComponent::WIR_SystemComponent(const WIR_SystemComponent&)" );
};


/*
  Move constructor.
*/
WIR_SystemComponent::WIR_SystemComponent( WIR_SystemComponent &&__o ) :
  WIR_ID_API { std::move( __o ) },
  mSystemPointer { nullptr },
  mName { std::move( __o.mName ) },
  mAddressRanges { std::move( __o.mAddressRanges ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  ufAssertT(
    __o.mSystemPointer == nullptr,
    "Invalid attempt to move a system component out of its owning system '" <<
    __o.getSystem().getName() << "'." );

  __o.mName.clear();
  __o.mAddressRanges.clear();
};


/*
  Copy-assignment operator.
*/
WIR_SystemComponent & WIR_SystemComponent::operator = ( const WIR_SystemComponent &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  mName = __o.mName;
  mAddressRanges = __o.mAddressRanges;

  mSystemPointer = nullptr;

  return( *this );
};


/*
  Move-assignment operator.
*/
WIR_SystemComponent & WIR_SystemComponent::operator = ( WIR_SystemComponent &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  ufAssertT(
    __o.mSystemPointer == nullptr,
    "Invalid attempt to move a system component out of its owning system '" <<
    __o.getSystem().getName() << "'." );

  mName = std::move( __o.mName );
  __o.mName.clear();

  mAddressRanges = std::move( __o.mAddressRanges );
  __o.mAddressRanges.clear();

  mSystemPointer = nullptr;

  return( *this );
};


/*
  addAddressRange adds a range to the set of address ranges for which a system
  component is active.
*/
void WIR_SystemComponent::addAddressRange( const WIR_AddressRange &r )
{
  DSTART(
    "void WIR_SystemComponent::addAddressRange(const WIR_AddressRange&)" );

  mAddressRanges.unite( r );
};


/*
  setName sets an system component's specific name.
*/
void WIR_SystemComponent::setName( const std::string &s )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  mName = s;
};


/*
  setName sets an system component's specific name.
*/
void WIR_SystemComponent::setName( std::string &&s )
{
  DSTART( "void WIR_SystemComponent::setName(string&&)" );

  mName = std::move( s );
  s.clear();
};

}       // namespace WIR
