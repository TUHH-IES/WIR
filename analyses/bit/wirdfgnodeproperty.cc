/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirdfgnodeproperty.cc
  @brief This file implements the basic properties of data flow graph nodes.

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

// Include local headers
#include "wirdfgnodeproperty.h"


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
WIR_DFGNodeProperty::WIR_DFGNodeProperty( void ) :
  mType { WIR_DFGNodeType::op },
  mIsSource { false },
  mIsSink { false }
{
};


/*
  Default constructur for data flow graph node information representing a WIR
  operation.
*/
WIR_DFGNodeProperty::WIR_DFGNodeProperty( WIR_Operation &o ) :
  mOp { o },
  mType { WIR_DFGNodeType::op },
  mIsSource { false },
  mIsSink { false }
{
  DSTART( "WIR_DFGNodeProperty::WIR_DFGNodeProperty(WIR_Operation&)" );
};


/*
  Default constructur for data flow graph node information representing an
  immediate parameter as graph input.
*/
WIR_DFGNodeProperty::WIR_DFGNodeProperty( WIR_BaseImmediateParameter &p ) :
  mImm { p },
  mType { WIR_DFGNodeType::imm },
  mIsSource { false },
  mIsSink { false }
{
  DSTART(
    "WIR_DFGNodeProperty::WIR_DFGNodeProperty(WIR_BaseImmediateParameter&)" );
};


/*
  Default constructur for data flow graph node information representing a
  register as graph input.
*/
WIR_DFGNodeProperty::WIR_DFGNodeProperty( WIR_RegisterParameter &p ) :
  mReg { p },
  mType { WIR_DFGNodeType::reg },
  mIsSource { false },
  mIsSink { false }
{
  DSTART( "WIR_DFGNodeProperty::WIR_DFGNodeProperty(WIR_RegisterParameter&)" );
};


/*
  Destructor.
*/
WIR_DFGNodeProperty::~WIR_DFGNodeProperty( void )
{
  DSTART( "WIR_DFGNodeProperty::~WIR_DFGNodeProperty()" );
};


/*
  getType returns the type of a WIR data flow graph node, i.e., whether it is an
  operation or an immediate or register parameter.
*/
WIR_DFGNodeType WIR_DFGNodeProperty::getType( void ) const
{
  DSTART( "WIR_DFGNodeType WIR_DFGNodeProperty::getType() const" );

  return( mType );
};


/*
  isOperation returns whether a DFG node represents a WIR operation.
*/
bool WIR_DFGNodeProperty::isOperation( void ) const
{
  DSTART( "bool WIR_DFGNodeProperty::isOperation() const" );

  return( mType == WIR_DFGNodeType::op );
};


/*
  getOperation returns the WIR operation associated with a DFG node.

  getOperation asserts if the DFG node does not represent an operation.
*/
WIR_Operation &WIR_DFGNodeProperty::getOperation( void ) const
{
  DSTART( "WIR_Operation& WIR_DFGNodeProperty::getOperation() const" );

  #ifdef FAILSAFEMODE
  ufAssert( mType == WIR_DFGNodeType::op );
  #endif

  return( mOp.get().get() );
};


/*
  getImmediateParameter returns the WIR immediate parameter associated with a
  DFG node.

  getImmediateParameter asserts if the DFG node does not represent an immediate
  parameter.
*/
WIR_BaseImmediateParameter &WIR_DFGNodeProperty::getImmediateParameter( void ) const
{
  DSTART(
    "WIR_BaseImmediateParameter& WIR_DFGNodeProperty::getImmediateParameter() const" );

  #ifdef FAILSAFEMODE
  ufAssert( mType == WIR_DFGNodeType::imm );
  #endif

  return( mImm.get().get() );
};


/*
  getRegisterParameter returns the WIR register parameter associated with a DFG
  node.

  getRegisterParameter asserts if the DFG node does not represent a register
  parameter.
*/
WIR_RegisterParameter &WIR_DFGNodeProperty::getRegisterParameter( void ) const
{
  DSTART(
    "WIR_RegisterParameter& WIR_DFGNodeProperty::getRegisterParameter() const" );

  #ifdef FAILSAFEMODE
  ufAssert( mType == WIR_DFGNodeType::reg );
  #endif

  return( mReg.get().get() );
};


/*
  setSource marks a DFG node as source node.

  According to Jens Wagner, Retargierbare Ausnutzung von Spezialoperationen für
  Eingebettete Systeme mit Hilfe bitgenauer Wertflussanalyse, page 155,
  definition 4.5, a node is a source node if it has no incoming edges except
  back-edges.
*/
void WIR_DFGNodeProperty::setSource( bool s )
{
  DSTART( "void WIR_DFGNodeProperty::setSource(bool)" );

  mIsSource = s;
};


/*
  isSource returns whether a DFG node is a source node.

  According to Jens Wagner, Retargierbare Ausnutzung von Spezialoperationen für
  Eingebettete Systeme mit Hilfe bitgenauer Wertflussanalyse, page 155,
  definition 4.5, a node is a source node if it has no incoming edges except
  back-edges.
*/
bool WIR_DFGNodeProperty::isSource( void ) const
{
  DSTART( "bool WIR_DFGNodeProperty::isSource() const" );

  return( mIsSource );
};


/*
  setSink marks a DFG node as sink node.

  According to Jens Wagner, Retargierbare Ausnutzung von Spezialoperationen für
  Eingebettete Systeme mit Hilfe bitgenauer Wertflussanalyse, page 155,
  definition 4.6, a node is a sink node if it has no outgoing edges except
  back-edges.
*/
void WIR_DFGNodeProperty::setSink( bool s )
{
  DSTART( "void WIR_DFGNodeProperty::setSink(bool)" );

  mIsSink = s;
};


/*
  isSink returns whether a DFG node is a sink node.

  According to Jens Wagner, Retargierbare Ausnutzung von Spezialoperationen für
  Eingebettete Systeme mit Hilfe bitgenauer Wertflussanalyse, page 155,
  definition 4.6, a node is a sink node if it has no outgoing edges except
  back-edges.
*/
bool WIR_DFGNodeProperty::isSink( void ) const
{
  DSTART( "bool WIR_DFGNodeProperty::isSink() const" );

  return( mIsSink );
};

}       // namespace WIR
