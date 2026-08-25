/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirregisterparameter.cc
  @brief This file implements register parameters.

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
  Default constructor for register parameters.
*/
WIR_RegisterParameter::WIR_RegisterParameter( const WIR_BaseRegister &__r,
                                              WIR_Usage __u, bool __i ) :
  WIR_Parameter {},
  mRegister { const_cast<WIR_BaseRegister *>( &__r ) },
  mUsage { __u }
{
  DSTART(
    "WIR_RegisterParameter::WIR_RegisterParameter(const WIR_BaseRegister&, WIR_Usage, bool)" );

  setImplicit( __i );
};


/*
  Copy constructor.
*/
WIR_RegisterParameter::WIR_RegisterParameter( const WIR_RegisterParameter &__o ) :
  WIR_Parameter { __o },
  mRegister { __o.mRegister },
  mUsage { __o.mUsage }
{
  DSTART(
    "WIR_RegisterParameter::WIR_RegisterParameter(const WIR_RegisterParameter&)" );
};


/*
  Move constructor.
*/
WIR_RegisterParameter::WIR_RegisterParameter( WIR_RegisterParameter &&__o ) :
  WIR_Parameter { std::move( __o ) },
  mRegister { __o.mRegister },
  mUsage { __o.mUsage }
{
  DSTART(
    "WIR_RegisterParameter::WIR_RegisterParameter(WIR_RegisterParameter&&)" );

  __o.mRegister = nullptr;
};


/*
  Destructor.
*/
WIR_RegisterParameter::~WIR_RegisterParameter( void )
{
  DSTART( "virtual WIR_RegisterParameter::~WIR_RegisterParameter()" );
};


/*
  Copy-assignment operator.
*/
WIR_RegisterParameter & WIR_RegisterParameter::operator = ( const WIR_RegisterParameter &__o )
{
  DSTART(
    "WIR_RegisterParameter& WIR_RegisterParameter::operator=(const WIR_RegisterParameter&)" );

  WIR_Parameter::operator = ( __o );

  mRegister = __o.mRegister;
  mUsage = __o.mUsage;

  return( *this );
};


/*
  Move-assignment operator.
*/
WIR_RegisterParameter & WIR_RegisterParameter::operator = ( WIR_RegisterParameter &&__o )
{
  DSTART(
    "WIR_RegisterParameter& WIR_RegisterParameter::operator=(WIR_RegisterParameter&&)" );

  WIR_Parameter::operator = ( std::move( __o ) );

  mRegister = __o.mRegister;
  __o.mRegister = nullptr;
  mUsage = __o.mUsage;

  return( *this );
};


/*
  getType returns the type of a WIR parameter, i.e., that it is a register
  parameter.
*/
WIR_ParameterType WIR_RegisterParameter::getType( void ) const
{
  DSTART( "virtual WIR_ParameterType WIR_RegisterParameter::getType() const" );

  return( WIR_ParameterType::reg );
};


/*
  getRegister returns the register associated with a parameter.
*/
WIR_BaseRegister &WIR_RegisterParameter::getRegister( void ) const
{
  DSTART( "WIR_Register& WIR_RegisterParameter::getRegister() const" );

  return( *mRegister );
};


/*
  setUsage sets a register parameter's usage type to the specified one.
*/
void WIR_RegisterParameter::setUsage( WIR_Usage t )
{
  DSTART( "void WIR_RegisterParameter::setUsage(WIR_Usage)" );

  checkDontOptimize();

  mUsage = t;
};


/*
  getUsage gets a register parameter's usage type.
*/
WIR_Usage WIR_RegisterParameter::getUsage( void ) const
{
  DSTART( "WIR_Usage WIR_RegisterParameter::getUsage() const" );

  return( mUsage );
};


/*
  isDefined returns whether this register parameter is defined.
*/
bool WIR_RegisterParameter::isDefined( void ) const
{
  DSTART( "bool WIR_RegisterParameter::isDefined() const" );

  return( mUsage == WIR_Usage::def );
};


/*
  isUsed returns whether this register parameter is used.
*/
bool WIR_RegisterParameter::isUsed( void ) const
{
  DSTART( "bool WIR_RegisterParameter::isUsed() const" );

  return( mUsage == WIR_Usage::use );
};


/*
  isDefUsed returns whether this register parameter is defined and used.
*/
bool WIR_RegisterParameter::isDefUsed( void ) const
{
  DSTART( "bool WIR_RegisterParameter::isDefUsed() const" );

  return( mUsage == WIR_Usage::defuse );
};


/*
  The << operator dumps a WIR register parameter to an output stream.
*/
std::ostream & operator << ( std::ostream &os, const WIR_RegisterParameter &p )
{
  DSTART( "ostream& operator<<(ostream&, const WIR_RegisterParameter&)" );

  WIR_Registry::getRegisterParameterDumper(
    os.iword( WIR_ProcessorIO() ) )( os, p );

  return( os );
};


//
// Protected class methods
//

/*
  clone creates a copy of a register parameter.

  Clone just calls the corresponding copy constructor.
*/
WIR_Parameter *WIR_RegisterParameter::clone( void ) const
{
  DSTART( "virtual WIR_Parameter* WIR_RegisterParameter::clone() const" );

  return( new WIR_RegisterParameter( *this ) );
};

}       // namespace WIR
