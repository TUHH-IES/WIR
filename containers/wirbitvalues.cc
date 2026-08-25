/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirbitvalues.cc
  @brief This file implements WIR containers representing bit-true up/down
         values of register parameters.

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
  Default constructor.
*/
WIR_BitValues:: WIR_BitValues( void ) :
  WIR_Container<WIR_BitValues> {}
{
  DSTART( "WIR_BitValues::WIR_BitValues()" );
};


/*
  Copy constructor.
*/
WIR_BitValues::WIR_BitValues( const WIR_BitValues &__o ) :
  WIR_Container<WIR_BitValues> { __o },
  mInValues { __o.mInValues },
  mOutValues { __o.mOutValues }
{
  DSTART( "WIR_BitValues::WIR_BitValues(const WIR_BitValues&)" );
};


/*
  Move constructor.
*/
WIR_BitValues::WIR_BitValues( WIR_BitValues &&__o ) :
  WIR_Container<WIR_BitValues> { std::move( __o ) },
  mInValues { std::move( __o.mInValues ) },
  mOutValues { std::move( __o.mOutValues ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  __o.mInValues.clear();
  __o.mOutValues.clear();
};


/*
  Destructor.
*/
WIR_BitValues::~WIR_BitValues( void )
{
  DSTART( "virtual WIR_BitValues::~WIR_BitValues()" );
};


/*
  Copy-assignment operator.
*/
WIR_BitValues & WIR_BitValues::operator = ( const WIR_BitValues &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_Container<WIR_BitValues>::operator = ( __o );

  mInValues = __o.mInValues;
  mOutValues = __o.mOutValues;

  return( *this );
};


/*
  Move-assignment operator.
*/
WIR_BitValues & WIR_BitValues::operator = ( WIR_BitValues &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_Container<WIR_BitValues>::operator = ( std::move( __o ) );

  mInValues = std::move( __o.mInValues );
  __o.mInValues.clear();
  mOutValues = std::move( __o.mOutValues );
  __o.mOutValues.clear();

  return( *this );
};


/*
  isUnique returns whether bit-true up/down values are unique, i.e., whether at
  most one instance of this container type can be attached to a WIR class.
*/
bool WIR_BitValues::isUnique( void ) const
{
  DSTART( "virtual bool WIR_BitValues::isUnique() const" );

  return( true );
};


/*
  insertInValues adds new incoming edge values to set mInValues.
*/
void WIR_BitValues::insertInValues( WIR_Parameter &p, WIR_UpDownValue &&d,
                                    WIR_UpDownValue &&u )
{
  DSTART(
    "void WIR_BitValues::insertInValues(WIR_Parameter&, WIR_UpDownValue&&, WIR_UpDownValue&&)" );

  mInValues.push_back( edge( p, std::move( d ), std::move( u ) ) );
};


/*
  eraseInValues removes the incoming edge starting at the specified parameter
  from set mInValues.
*/
void WIR_BitValues::eraseInValues( const WIR_Parameter &p )
{
  DSTART( "void WIR_BitValues::eraseInValues(const WIR_Parameter&)" );

  auto it = findInValues( p );

  if ( it != mInValues.end() )
    mInValues.erase( it );
};


/*
  eraseInValues removes the specified incoming edge from set mInValues.
*/
void WIR_BitValues::eraseInValues( std::list<edge>::iterator pos )
{
  DSTART(
    "void WIR_BitValues::eraseInValues(list<WIR_BitValues::edge>::iterator)" );

  mInValues.erase( pos );
};


/*
  clearInValues removes all elements from set mInValues.
*/
void WIR_BitValues::clearInValues( void )
{
  DSTART( "void WIR_BitValues::clearInValues()" );

  mInValues.clear();
};


/*
  getInValues returns the set mInValues.
*/
list<WIR_BitValues::edge> &WIR_BitValues::getInValues( void )
{
  DSTART( "list<WIR_BitValues::edge>& WIR_BitValues::getInValues()" );

  return( mInValues );
};


/*
  findInValues finds the incoming up/down values of the edge starting at the
  specified register parameter.
*/
list<WIR_BitValues::edge>::iterator WIR_BitValues::findInValues( const WIR_Parameter &p )
{
  DSTART(
    "list<WIR_BitValues::edge>::iterator WIR_BitValues::findInValues(const WIR_Parameter&)" );

  return(
    find_if(
      mInValues.begin(), mInValues.end(),
      [&]( const edge &e ) { return( *(e.rp) == p ); } ) );
};


/*
  insertOutValues adds new outgoing edge values to set mOutValues.
*/
void WIR_BitValues::insertOutValues( WIR_Parameter &p, WIR_UpDownValue &&d,
                                     WIR_UpDownValue &&u )
{
  DSTART(
    "void WIR_BitValues::insertOutValues(WIR_Parameter&, WIR_UpDownValue&&, WIR_UpDownValue&&)" );

  mOutValues.push_back( edge( p, std::move( d ), std::move( u ) ) );
};


/*
  eraseOutValues removes the outgoing edge ending at the specified parameter
  from set mOutValues.
*/
void WIR_BitValues::eraseOutValues( const WIR_Parameter &p )
{
  DSTART( "void WIR_BitValues::eraseOutValues(const WIR_Parameter&)" );

  auto it = findOutValues( p );

  if ( it != mOutValues.end() )
    mOutValues.erase( it );
};


/*
  clearOutValues removes all elements from set mOutValues.
*/
void WIR_BitValues::clearOutValues( void )
{
  DSTART( "void WIR_BitValues::clearOutValues()" );

  mOutValues.clear();
};


/*
  getOutValues returns the set mOutValues.
*/
list<WIR_BitValues::edge> &WIR_BitValues::getOutValues( void )
{
  DSTART( "list<WIR_BitValues::edge>& WIR_BitValues::getOutValues()" );

  return( mOutValues );
};


/*
  findOutValues finds the outgoing up/down values of the edge ending at the
  specified parameter.
*/
list<WIR_BitValues::edge>::iterator WIR_BitValues::findOutValues( const WIR_Parameter &p )
{
  DSTART(
    "list<WIR_BitValues::edge>::iterator WIR_BitValues::findOutValues(const WIR_Parameter&)" );

  return(
    find_if(
      mOutValues.begin(), mOutValues.end(),
      [&]( const edge &e ) { return( *(e.rp) == p ); } ) );

  return( mOutValues.end() );
};

}       // namespace WIR
