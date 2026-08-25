/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wircontroltree.cc
  @brief This file implements a %WIR container representing control trees.

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
  Default constructor for the normal use case where a container refers to a leaf
  node of a control tree, i.e., to a single basic block.
*/
WIR_ControlTree::WIR_ControlTree( WIR_BasicBlockTreeNode &b ) :
  mLeafNode { &b }
{
  DSTART( "WIR_ControlTree::WIR_ControlTree(WIR_BasicBlockTreeNode&)" );
};


/*
  Destructor.
*/
WIR_ControlTree::~WIR_ControlTree( void )
{
  DSTART( "virtual WIR_ControlTree::~WIR_ControlTree()" );

  // Free memory of all associated control tree roots.
  for ( const auto *p : mRootNodes )
    delete( p );
};


/*
  isUnique returns whether control tree containers are unique, i.e., whether at
  most one instance of this container type can be attached to a WIR class.
*/
bool WIR_ControlTree::isUnique( void ) const
{
  DSTART( "virtual bool WIR_ControlTree::isUnique() const" );

  return( true );
};


/*
  getBasicBlockTreeNode returns the basic block tree node associated with a
  control tree container.
*/
WIR_BasicBlockTreeNode &WIR_ControlTree::getBasicBlockTreeNode( void ) const
{
  DSTART(
    "WIR_BasicBlockTreeNode& WIR_ControlTree::getBasicBlockTreeNode() const" );

  return( *mLeafNode );
};


//
// Private class methods
//

/*
  Default constructor for the internal use case where a container refers to a
  root node of a control tree.
*/
WIR_ControlTree::WIR_ControlTree( void ) :
  mLeafNode { nullptr }
{
  DSTART( "WIR_ControlTree::WIR_ControlTree()" );
};


/*
  pushBackRootNode adds a new control tree root to list mRootNodes.
*/
void WIR_ControlTree::pushBackRootNode( WIR_ControlTreeNode &n )
{
  DSTART( "void WIR_ControlTree::pushBackRootNode(WIR_ControlTreeNode&)" );

  mLeafNode = nullptr;
  mRootNodes.push_back( &n );
};

}       // namespace WIR
