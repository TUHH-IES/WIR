/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirreachingdefinitions.cc
  @brief This file implements WIR containers representing sets of register
         definitions that reach instructions.

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
WIR_ReachingDefinitions::WIR_ReachingDefinitions( void ) :
  WIR_Container<WIR_ReachingDefinitions> {}
{
  DSTART( "WIR_ReachingDefinitions::WIR_ReachingDefinitions()" );
};


/*
  Destructor.
*/
WIR_ReachingDefinitions::~WIR_ReachingDefinitions( void )
{
  DSTART( "virtual WIR_ReachingDefinitions::~WIR_ReachingDefinitions()" );
};


/*
  isUnique returns whether reaching-definition sets are unique, i.e., whether at
  most one instance of this container type can be attached to a WIR class.
*/
bool WIR_ReachingDefinitions::isUnique( void ) const
{
  DSTART( "virtual bool WIR_ReachingDefinitions::isUnique() const" );

  return( true );
};


/*
  insertReachingDefinition adds a new reaching definition to set
  mReachingDefinitions.
*/
void WIR_ReachingDefinitions::insertReachingDefinition( const WIR_RegisterParameter &rp )
{
  DSTART(
    "void WIR_ReachingDefinitions::insertReachingDefinition(const WIR_RegisterParameter&)" );

  mReachingDefinitions.insert( const_cast<WIR_RegisterParameter &>( rp ) );
};


/*
  getReachingDefinitions returns the set mReachingDefinitions.
*/
const WIR_RegisterParameterSet &WIR_ReachingDefinitions::getReachingDefinitions( void ) const
{
  DSTART(
    "const WIR_RegisterParameterSet& WIR_ReachingDefinitions::getReachingDefinitions() const" );

  return( mReachingDefinitions );
};


/*
  insertReachingInput adds a new reaching function input to set mReachingInputs.
*/
void WIR_ReachingDefinitions::insertReachingInput( const WIR_PhysicalRegister &p )
{
  DSTART(
    "void WIR_ReachingDefinitions::insertReachingInput(const WIR_PhysicalRegister&)" );

  mReachingInputs.insert( const_cast<WIR_PhysicalRegister &>( p ) );
};


/*
  getReachingInputs returns the set mReachingInputs.
*/
const WIR_PhysicalRegisterSet &WIR_ReachingDefinitions::getReachingInputs( void ) const
{
  DSTART(
    "const WIR_PhysicalRegisterSet& WIR_ReachingDefinitions::getReachingInputs() const" );

  return( mReachingInputs );
};

}       // namespace WIR
