/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirblocktreenode.cc
  @brief This file implements block tree nodes.

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
WIR_BlockTreeNode::WIR_BlockTreeNode( void ) :
  WIR_ControlTreeNode {}
{
  DSTART( "WIR_BlockTreeNode::WIR_BlockTreeNode()" );
};


/*
  Destructor.
*/
WIR_BlockTreeNode::~WIR_BlockTreeNode( void )
{
  DSTART( "virtual WIR_BlockTreeNode::~WIR_BlockTreeNode()" );
};


/*
  getType returns the type of a block region node.
*/
WIR_CTNodeType WIR_BlockTreeNode::getType( void ) const
{
  DSTART( "virtual WIR_CTNodeType WIR::WIR_BlockTreeNode::getType() const" );

  return( WIR_CTNodeType::block );
};


/*
  isCyclic returns whether a block tree node is cyclic or not.
*/
bool WIR_BlockTreeNode::isCyclic( void ) const
{
  DSTART( "virtual bool WIR_BlockTreeNode::isCyclic() const" );

  return( false );
};


/*
  getEntry returns a block region's unique entry child node.
*/
const WIR_ControlTreeNode &WIR_BlockTreeNode::getEntry( void ) const
{
  DSTART(
    "virtual const WIR_ControlTreeNode& WIR_BlockTreeNode::getEntry() const" );

  return( mBlockList.front().get() );
};


/*
  getBlockList returns the list of stored child nodes in their sequential order.
*/
const list<reference_wrapper<WIR_ControlTreeNode>> &WIR_BlockTreeNode::getBlockList( void ) const
{
  DSTART(
    "const list<reference_wrapper<WIR_ControlTreeNode> >& WIR_BlockTreeNode::getBlockList() const" );

  return( mBlockList );
};


//
// Protected class methods
//

/*
  visualize dumps a block region node into a given DOT file.
*/
void WIR_BlockTreeNode::visualize( std::fstream &dotFile ) const
{
  DSTART( "virtual void WIR_BlockTreeNode::visualize(fstream&) const" );

  dotFile << "subgraph cluster" << getID() << " {" << endl;

  dotFile << "  label=\"" << buildNodeName() << "\";" << endl;
  dotFile << "  color=blue;" << endl;

  // Recursively visualize all included hierarchical child nodes.
  for ( const WIR_ControlTreeNode &c : mBlockList )
    c.visualize( dotFile );

  dotFile << "}" << endl;
};


//
// Private class methods
//

/*
  pushBackBlockNode adds a new node to a block region at the end of list
  mBlockList.
*/
void WIR_BlockTreeNode::pushBackBlockNode( WIR_ControlTreeNode &n )
{
  DSTART( "void WIR_BlockTreeNode::pushBackBlockNode(WIR_ControlTreeNode&)" );

  mBlockList.push_back( ref( n ) );
};

}       // namespace WIR
