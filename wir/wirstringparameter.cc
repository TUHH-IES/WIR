/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirstringparameter.cc
  @brief This file implements parameters that contain simple, unstructured
         strings.

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
  Default constructor for string parameters.
*/
WIR_StringParameter::WIR_StringParameter( const std::string &s ) :
  WIR_Parameter {},
  mString { s }
{
  DSTART( "WIR_StringParameter::WIR_StringParameter(const string&)" );
};


/*
  Default constructor for string parameters.
*/
WIR_StringParameter::WIR_StringParameter( std::string &&s ) :
  WIR_Parameter {},
  mString { std::move( s ) }
{
  DSTART( "WIR_StringParameter::WIR_StringParameter(string&&)" );

  s.clear();
};


/*
  Copy constructor.
*/
WIR_StringParameter::WIR_StringParameter( const WIR_StringParameter &__o ) :
  WIR_Parameter { __o },
  mString { __o.mString }
{
  DSTART(
    "WIR_StringParameter::WIR_StringParameter(const WIR_StringParameter&)" );
};


/*
  Move constructor.
*/
WIR_StringParameter::WIR_StringParameter( WIR_StringParameter &&__o ) :
  WIR_Parameter { std::move( __o ) },
  mString { std::move( __o.mString ) }
{
  DSTART(
    "WIR_StringParameter::WIR_StringParameter(WIR_StringParameter&&)" );

  __o.mString.clear();
};


/*
  Destructor.
*/
WIR_StringParameter::~WIR_StringParameter( void )
{
  DSTART( "virtual WIR_StringParameter::~WIR_StringParameter()" );
};


/*
  Copy-assignment operator.
*/
WIR_StringParameter & WIR_StringParameter::operator = ( const WIR_StringParameter &__o )
{
  DSTART(
    "WIR_StringParameter& WIR_StringParameter::operator=(const WIR_StringParameter&)" );

  WIR_Parameter::operator = ( __o );

  mString = __o.mString;

  return( *this );
};


/*
  Move-assignment operator.
*/
WIR_StringParameter & WIR_StringParameter::operator = ( WIR_StringParameter &&__o )
{
  DSTART(
    "WIR_StringParameter& WIR_StringParameter::operator=(WIR_StringParameter&&)" );

  WIR_Parameter::operator = ( std::move( __o ) );

  mString = std::move( __o.mString );
  __o.mString.clear();

  return( *this );
};


/*
  getType returns the type of a WIR parameter, i.e., that it is a string
  parameter.
*/
WIR_ParameterType WIR_StringParameter::getType( void ) const
{
  DSTART( "virtual WIR_ParameterType WIR_StringParameter::getType() const" );

  return( WIR_ParameterType::str );
};


/*
  setString sets a parameter's string.
*/
void WIR_StringParameter::setString( const std::string &s )
{
  DSTART( "void WIR_StringParameter::setString(const string&)" );

  mString = s;
};


/*
  setString sets a parameter's string.
*/
void WIR_StringParameter::setString( std::string &&s )
{
  DSTART( "void WIR_StringParameter::setString(string&&)" );

  mString = std::move( s );
  s.clear();
};


/*
  getString returns a parameter's string.
*/
string WIR_StringParameter::getString( void ) const
{
  DSTART( "string WIR_StringParameter::getString() const" );

  return( mString );
};


/*
  The << operator dumps a WIR string parameter to an output stream.
*/
std::ostream & operator << ( std::ostream &os, const WIR_StringParameter &p )
{
  DSTART( "ostream& operator<<(ostream&, const WIR_StringParameter&)" );

  os << p.getString();

  return( os );
};


//
// Protected class methods
//

/*
  clone creates a copy of a label parameter.

  Clone just calls the corresponding copy constructor.
*/
WIR_Parameter *WIR_StringParameter::clone( void ) const
{
  DSTART( "virtual WIR_Parameter* WIR_StringParameter::clone() const" );

  return( new WIR_StringParameter( *this ) );
};

}       // namespace WIR
