/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirselflooptreenode.cc
  @brief This file implements self-loop tree nodes.

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
WIR_SelfLoopTreeNode::WIR_SelfLoopTreeNode( const WIR_ControlTreeNode &n ) :
  WIR_ControlTreeNode {},
  mEntry { n }
{
  DSTART(
    "WIR_SelfLoopTreeNode::WIR_SelfLoopTreeNode(const WIR_ControlTreeNode&)" );

  mBackEdges.insert(
    make_pair(
      ref( const_cast<WIR_ControlTreeNode &>( n ) ),
      ref( const_cast<WIR_ControlTreeNode &>( n ) ) ) );
};


/*
  Destructor.
*/
WIR_SelfLoopTreeNode::~WIR_SelfLoopTreeNode( void )
{
  DSTART( "virtual WIR_SelfLoopTreeNode::~WIR_SelfLoopTreeNode()" );
};


/*
  getType returns the type of a self-loop region node.
*/
WIR_CTNodeType WIR_SelfLoopTreeNode::getType( void ) const
{
  DSTART( "virtual WIR_CTNodeType WIR_SelfLoopTreeNode::getType() const" );

  return( WIR_CTNodeType::selfloop );
};


/*
  isCyclic returns whether a self-loop region is cyclic or not.
*/
bool WIR_SelfLoopTreeNode::isCyclic( void ) const
{
  DSTART( "virtual bool WIR_SelfLoopTreeNode::isCyclic() const" );

  return( true );
};


/*
  getEntry returns a self-loop's unique entry child node.
*/
const WIR_ControlTreeNode& WIR_SelfLoopTreeNode::getEntry( void ) const
{
  DSTART( "const WIR_ControlTreeNode& WIR_SelfLoopTreeNode::getEntry() const" );

  return( mEntry );
};


//
// Protected class methods
//

/*
  visualize dumps a self-loop region node into a given DOT file.
*/
void WIR_SelfLoopTreeNode::visualize( std::fstream &dotFile ) const
{
  DSTART( "virtual void WIR_SelfLoopTreeNode::visualize(fstream&) const" );

  dotFile << "subgraph cluster" << getID() << " {" << endl;

  dotFile << "  label=\"" << buildNodeName() << "\";" << endl;
  dotFile << "  color=blue;" << endl;

  // Recursively visualize all included hierarchical child nodes.
  mEntry.visualize( dotFile );

  dotFile << "}" << endl;
};

}       // namespace WIR
