/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirimmediatedomination.cc
  @brief This file implements a %WIR container representing immediate dominators
         of basic blocks.

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
  Default constructor.
*/
WIR_ImmediateDomination::WIR_ImmediateDomination( void ) :
  WIR_Container<WIR_ImmediateDomination> {},
  mImmediateDominator { nullptr }
{
  DSTART( "WIR_ImmediateDomination::WIR_ImmediateDomination()" );
};


/*
  Destructor.
*/
WIR_ImmediateDomination::~WIR_ImmediateDomination( void )
{
  DSTART( "virtual WIR_ImmediateDomination::~WIR_ImmediateDomination()" );
};


/*
  isUnique returns whether immediate dominators are unique, i.e., whether at
  most one instance of this container type can be attached to a WIR class.
*/
bool WIR_ImmediateDomination::isUnique( void ) const
{
  DSTART( "virtual bool WIR_ImmediateDomination::isUnique() const" );

  return( true );
};


/*
  setImmediateDominator sets a new immediate dominator.
*/
void WIR_ImmediateDomination::setImmediateDominator( WIR_BasicBlock &b )
{
  DSTART(
    "void WIR_ImmediateDomination::setImmediateDominator(WIR_BasicBlock&)" );

  mImmediateDominator = &b;
};


/*
  getImmediateDominator returns the immediate dominator block.
*/
WIR_BasicBlock &WIR_ImmediateDomination::getImmediateDominator( void ) const
{
  DSTART(
    "WIR_BasicBlock& WIR_ImmediateDomination::getImmediateDominator() const" );

  return( *mImmediateDominator );
};

}       // namespace WIR
