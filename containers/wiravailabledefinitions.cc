/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wiravailabledefinitions.cc
  @brief This file implements WIR containers representing sets of register
         definitions that are available at instructions.

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
  Default constructor.
*/
WIR_AvailableDefinitions::WIR_AvailableDefinitions( void ) :
  WIR_Container<WIR_AvailableDefinitions> {}
{
  DSTART( "WIR_AvailableDefinitions::WIR_AvailableDefinitions()" );
};


/*
  Destructor.
*/
WIR_AvailableDefinitions::~WIR_AvailableDefinitions( void )
{
  DSTART( "virtual WIR_AvailableDefinitions::~WIR_AvailableDefinitions()" );
};


/*
  isUnique returns whether available-definition sets are unique, i.e., whether
  at most one instance of this container type can be attached to a WIR class.
*/
bool WIR_AvailableDefinitions::isUnique( void ) const
{
  DSTART( "virtual bool WIR_AvailableDefinitions::isUnique() const" );

  return( true );
};


/*
  insertAvailableDefinition adds a new available definition to set
  mAvailableDefinitions.
*/
void WIR_AvailableDefinitions::insertAvailableDefinition( const WIR_RegisterParameter &rp )
{
  DSTART(
    "void WIR_AvailableDefinitions::insertAvailableDefinition(const WIR_RegisterParameter&)" );

  mAvailableDefinitions.insert( const_cast<WIR_RegisterParameter &>( rp ) );
};


/*
  getAvailableDefinitions returns the set mAvailableDefinitions.
*/
const WIR_RegisterParameterSet &WIR_AvailableDefinitions::getAvailableDefinitions( void ) const
{
  DSTART(
    "const WIR_RegisterParameterSet& WIR_AvailableDefinitions::getAvailableDefinitions() const" );

  return( mAvailableDefinitions );
};


/*
  insertAvailableInput adds a new reaching function input to set
  mAvailableInputs.
*/
void WIR_AvailableDefinitions::insertAvailableInput( const WIR_PhysicalRegister &p )
{
  DSTART(
    "void WIR_AvailableDefinitions::insertAvailableInput(const WIR_PhysicalRegister&)" );

  mAvailableInputs.insert( const_cast<WIR_PhysicalRegister &>( p ) );
};


/*
  getAvailableInputs returns the set mAvailableInputs.
*/
const WIR_PhysicalRegisterSet &WIR_AvailableDefinitions::getAvailableInputs( void ) const
{
  DSTART(
    "const WIR_PhysicalRegisterSet& WIR_AvailableDefinitions::getAvailableInputs() const" );

  return( mAvailableInputs );
};

}       // namespace WIR
