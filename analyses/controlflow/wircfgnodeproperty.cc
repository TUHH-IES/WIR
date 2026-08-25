/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wircfgnodeproperty.cc
  @brief This file implements the basic properties of control flow graph nodes.

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
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>

// Include private headers
#include "wircfgnodeproperty.h"


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

  Do not use this constructor! It exists only in order to make the Boost Graph
  Library happy. Instead, use one of the constructors below.
*/
WIR_CFGNodeProperty::WIR_CFGNodeProperty( void ) :
  mType { WIR_CFGNodeType::bb },
  mIsStart { false }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructur for control flow graph node information representing a WIR
  function.
*/
WIR_CFGNodeProperty::WIR_CFGNodeProperty( WIR_Function &f ) :
  mFct { f },
  mType { WIR_CFGNodeType::fct },
  mIsStart { false }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructur for control flow graph node information representing a WIR
  basic block.
*/
WIR_CFGNodeProperty::WIR_CFGNodeProperty( WIR_BasicBlock &b ) :
  mBB { b },
  mType { WIR_CFGNodeType::bb },
  mIsStart { false }
{
  DSTART( "WIR_CFGNodeProperty::WIR_CFGNodeProperty(WIR_BasicBlock&)" );
};


/*
  Destructor.
*/
WIR_CFGNodeProperty::~WIR_CFGNodeProperty( void )
{
  DSTART( "WIR_CFGNodeProperty::~WIR_CFGNodeProperty()" );
};


/*
  getType returns the type of a WIR control flow graph node, i.e., whether it is
  a function, basic block or instruction.
*/
WIR_CFGNodeType WIR_CFGNodeProperty::getType( void ) const
{
  DSTART( "WIR_CFGNodeType WIR_CFGNodeProperty::getType() const" );

  return( mType );
};


/*
  isFunction returns whether a CFG node represents a WIR function.
*/
bool WIR_CFGNodeProperty::isFunction( void ) const
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  return( mType == WIR_CFGNodeType::fct );
};


/*
  isBasicBlock returns whether a CFG node represents a WIR basic block.
*/
bool WIR_CFGNodeProperty::isBasicBlock( void ) const
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  return( mType == WIR_CFGNodeType::bb );
};


/*
  getFunction returns the WIR function associated with a CFG node.

  getFunction asserts if the CFG node does not represent a function.
*/
WIR_Function &WIR_CFGNodeProperty::getFunction( void ) const
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  ufAssert( mType == WIR_CFGNodeType::fct );

  return( mFct.get().get() );
};


/*
  getBasicBlock returns the WIR basic block associated with a CFG node.

  getBasicBlock asserts if the CFG node does not represent a basic block.
*/
WIR_BasicBlock &WIR_CFGNodeProperty::getBasicBlock( void ) const
{
  DSTART( "WIR_BasicBlock& WIR_CFGNodeProperty::getBasicBlock() const" );

  ufAssert( mType == WIR_CFGNodeType::bb );

  return( mBB.get().get() );
};


/*
  isStart returns whether a CFG node is a start node.

  A node is a start node if it has no incoming edges except back-edges.
*/
bool WIR_CFGNodeProperty::isStart( void ) const
{
  DSTART( "bool WIR_CFGNodeProperty::isStart() const" );

  return( mIsStart );
};


/*
  setStart marks a CFG node as start node.

  A node is a start node if it has no incoming edges except back-edges.
*/
void WIR_CFGNodeProperty::setStart( bool s )
{
  DSTART( "void WIR_CFGNodeProperty::setStart(bool)" );

  mIsStart = s;
};

}       // namespace WIR
