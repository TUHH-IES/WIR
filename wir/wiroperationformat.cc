/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wiroperationformat.cc
  @brief This file implements %WIR operation formats.

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
WIR_OperationFormat::~WIR_OperationFormat( void )
{
  DSTART( "WIR_OperationFormat::~WIR_OperationFormat()" );
};


/*
  getParameters returns the list mParameterReferences.
*/
const std::list<std::reference_wrapper<WIR_Parameter>> &WIR_OperationFormat::getParameters( void ) const
{
  DSTART(
    "const list<reference_wrapper<WIR_Parameter> >& WIR_OperationFormat::getParameters() const" );

  return( mParameterReferences );
};


/*
  begin returns an iterator to the first parameter of an operation format.
*/
std::list<std::reference_wrapper<WIR_Parameter>>::const_iterator WIR_OperationFormat::begin( void ) const
{
  DSTART(
    "list<reference_wrapper<WIR_Parameter> >::const_iterator WIR_OperationFormat::begin() const" );

  return( mParameterReferences.begin() );
};


/*
  end returns an iterator to the end of an operation format's parameter list.
*/
std::list<std::reference_wrapper<WIR_Parameter>>::const_iterator WIR_OperationFormat::end( void ) const
{
  DSTART(
    "list<reference_wrapper<WIR_Parameter> >::const_iterator WIR_OperationFormat::end() const" );

  return( mParameterReferences.end() );
};


//
// Private class methods
//

/*
  Move constructor.
*/
WIR_OperationFormat::WIR_OperationFormat( WIR_OperationFormat &&__o ) :
  mParameterPointers { std::move( __o.mParameterPointers ) },
  mParameterReferences { std::move( __o.mParameterReferences) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  __o.mParameterPointers.clear();
  __o.mParameterReferences.clear();
};


/*
  Move-assignment operator.
*/
WIR_OperationFormat & WIR_OperationFormat::operator = ( WIR_OperationFormat &&__o )
{
  DSTART(
    "WIR_OperationFormat& WIR_OperationFormat::operator=(WIR_OperationFormat&&)" );

  mParameterPointers = std::move( __o.mParameterPointers );
  __o.mParameterPointers.clear();

  mParameterReferences = std::move( __o.mParameterReferences );
  __o.mParameterReferences.clear();

  return( *this );
};


/*
  Dummy function for adding parameters which does nothing.

  It only serves to terminate the recursion of the variadic method
  addParameters.
*/
void WIR_OperationFormat::addParameters( void ) const
{
  DSTART( "void WIR_OperationFormat::addParameters() const" );
};

}       // namespace WIR
