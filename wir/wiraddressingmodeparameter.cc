/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wiraddressingmodeparameter.cc
  @brief This file implements parameters representing addressing modes.

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
#include <libuseful/io.h>

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
  Default constructor for addressing mode parameters.
*/
WIR_AddressingModeParameter::WIR_AddressingModeParameter( const WIR_BaseProcessor::AddressingMode &__r ) :
  WIR_Parameter {},
  mMode { const_cast<WIR_BaseProcessor::AddressingMode *>( &__r ) }
{
  DSTART(
    "WIR_AddressingModeParameter::WIR_AddressingModeParameter(const WIR_BaseProcessor::AddressingMode&)" );
};


/*
  Copy constructor.
*/
WIR_AddressingModeParameter::WIR_AddressingModeParameter( const WIR_AddressingModeParameter &__o ) :
  WIR_Parameter { __o },
  mMode { __o.mMode }
{
  DSTART(
    "WIR_AddressingModeParameter::WIR_AddressingModeParameter(const WIR_AddressingModeParameter&)" );
};



/*
  Move constructor.
*/
WIR_AddressingModeParameter::WIR_AddressingModeParameter( WIR_AddressingModeParameter &&__o ) :
  WIR_Parameter { std::move( __o ) },
  mMode { std::move( __o.mMode ) }
{
  DSTART(
    "WIR_AddressingModeParameter::WIR_AddressingModeParameter(WIR_AddressingModeParameter&&)" );

  __o.mMode = nullptr;
};


/*
  Destructor.
*/
WIR_AddressingModeParameter::~WIR_AddressingModeParameter( void )
{
  DSTART(
    "virtual WIR_AddressingModeParameter::~WIR_AddressingModeParameter()" );
};


/*
  Copy-assignment operator.
*/
WIR_AddressingModeParameter & WIR_AddressingModeParameter::operator = ( const WIR_AddressingModeParameter &__o )
{
  DSTART(
    "WIR_AddressingModeParameter& WIR_AddressingModeParameter::operator=(const WIR_AddressingModeParameter&)" );

  WIR_Parameter::operator = ( __o );

  mMode = __o.mMode;

  return( *this );
};


/*
  Move-assignment operator.
*/
WIR_AddressingModeParameter & WIR_AddressingModeParameter::operator = ( WIR_AddressingModeParameter &&__o )
{
  DSTART(
    "WIR_AddressingModeParameter& WIR_AddressingModeParameter::operator=(WIR_AddressingModeParameter&&)" );

  WIR_Parameter::operator = ( std::move( __o ) );

  mMode = std::move( __o.mMode );
  __o.mMode = nullptr;

  return( *this );
};


/*
  getType returns the type of a WIR parameter, i.e., that it is an addressing
  mode parameter.
*/
WIR_ParameterType WIR_AddressingModeParameter::getType( void ) const
{
  DSTART(
    "virtual WIR_ParameterType WIR_AddressingModeParameter::getType() const" );

  return( WIR_ParameterType::addr );
};


/*
  setAddressingMode sets a parameter's actual addressing mode.
*/
void WIR_AddressingModeParameter::setAddressingMode( const WIR_BaseProcessor::AddressingMode &m )
{
  DSTART(
    "void WIR_AddressingModeParameter::setAddressingMode(const WIR_BaseProcessor::AddressingMode&)" );

  checkDontOptimize();

  mMode = const_cast<WIR_BaseProcessor::AddressingMode *>( &m );
};


/*
  getAddressingMode gets a parameter's addressing mode.
*/
WIR_BaseProcessor::AddressingMode &WIR_AddressingModeParameter::getAddressingMode( void ) const
{
  DSTART(
    "WIR_BaseProcessor::AddressingMode& WIR_AddressingModeParameter::getAddressingMode() const" );

  ufAssertT(
    mMode != nullptr,
    "Attempt to get addressing mode from a parameter that has previously " <<
    "been moved." );

  return( *mMode );
};


/*
  The << operator dumps a WIR addressing mode parameter to an output stream.
*/
std::ostream & operator << ( std::ostream &os,
                             const WIR_AddressingModeParameter &p )
{
  DSTART( "ostream& operator<<(ostream&, const WIR_AddressingModeParameter&)" );

  WIR_Registry::getAddressingModeParameterDumper(
    os.iword( WIR_ProcessorIO() ) )( os, p );

  return( os );
};


//
// Protected class methods
//

/*
  clone creates a copy of an addressing mode parameter.

  Clone just calls the corresponding copy constructor.
*/
WIR_Parameter *WIR_AddressingModeParameter::clone( void ) const
{
  DSTART( "virtual WIR_Parameter* WIR_AddressingModeParameter::clone() const" );

  return( new WIR_AddressingModeParameter( *this ) );
};

}       // namespace WIR
