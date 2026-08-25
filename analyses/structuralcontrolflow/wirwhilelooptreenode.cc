/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirwhilelooptreenode.cc
  @brief This file implements regular for- or while-do loop tree nodes.

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
WIR_WhileLoopTreeNode::WIR_WhileLoopTreeNode( const WIR_ControlTreeNode &n ) :
  WIR_ControlTreeNode {},
  mEntry { n }
{
  DSTART(
    "WIR_WhileLoopTreeNode::WIR_WhileLoopTreeNode(const WIR_ControlTreeNode&)" );
};


/*
  Destructor.
*/
WIR_WhileLoopTreeNode::~WIR_WhileLoopTreeNode( void )
{
  DSTART( "virtual WIR_WhileLoopTreeNode::~WIR_WhileLoopTreeNode()" );
};


/*
  getType returns the type of a for/while loop region node.
*/
WIR_CTNodeType WIR_WhileLoopTreeNode::getType( void ) const
{
  DSTART( "virtual WIR_CTNodeType WIR_WhileLoopTreeNode::getType() const" );

  return( WIR_CTNodeType::whileloop );
};


/*
  isCyclic returns whether a for/while loop region is cyclic or not.
*/
bool WIR_WhileLoopTreeNode::isCyclic( void ) const
{
  DSTART( "virtual bool WIR_WhileLoopTreeNode::isCyclic() const" );

  return( true );
};


/*
  getEntry returns a for/while loop's unique entry child node.
*/
const WIR_ControlTreeNode &WIR_WhileLoopTreeNode::getEntry( void ) const
{
  DSTART(
    "virtual const WIR_ControlTreeNode& WIR_WhileLoopTreeNode::getEntry() const" );

  return( mEntry );
};


/*
  getNodes returns the set of all stored nodes.
*/
const WIR_ControlTreeNodeSet &WIR_WhileLoopTreeNode::getNodes( void ) const
{
  DSTART(
    "const WIR_ControlTreeNodeSet& WIR_WhileLoopTreeNode::getNodes() const" );

  return( mNodes );
};


//
// Protected class methods
//

/*
  visualize dumps a for/while loop region node into a given DOT file.
*/
void WIR_WhileLoopTreeNode::visualize( std::fstream &dotFile ) const
{
  DSTART( "virtual void WIR_WhileLoopTreeNode::visualize(fstream&) const" );

  dotFile << "subgraph cluster" << getID() << " {" << endl;

  dotFile << "  label=\"" << buildNodeName() << "\";" << endl;
  dotFile << "  color=blue;" << endl;

  // Recursively visualize all included hierarchical child nodes.
  for ( const WIR_ControlTreeNode &c : mNodes )
    c.visualize( dotFile );

  dotFile << "}" << endl;
};


//
// Private class methods
//

/*
  insertNode adds a new node to a for/while loop region.
*/
void WIR_WhileLoopTreeNode::insertNode( WIR_ControlTreeNode &n )
{
  DSTART( "void WIR_WhileLoopTreeNode::insertNode(WIR_ControlTreeNode&)" );

  mNodes.insert( ref( n ) );
};


/*
  insertEdge adds a new edge between two nodes to a for/while loop region.
*/
void WIR_WhileLoopTreeNode::insertEdge( const WIR_ControlTreeNode &s,
                                        const WIR_ControlTreeNode &t )
{
  DSTART(
    "void WIR_WhileLoopTreeNode::insertEdge(const WIR_ControlTreeNode&, const WIR_ControlTreeNode&)" );

  mEdges.insert(
    make_pair(
      ref( const_cast<WIR_ControlTreeNode &>( s ) ),
      ref( const_cast<WIR_ControlTreeNode &>( t ) ) ) );
};


/*
  insertBackEdge adds a new back-edge between two nodes to a for/while loop
  region.
*/
void WIR_WhileLoopTreeNode::insertBackEdge( const WIR_ControlTreeNode &s,
                                            const WIR_ControlTreeNode &t )
{
  DSTART(
    "void WIR_WhileLoopTreeNode::insertBackEdge(const WIR_ControlTreeNode&, const WIR_ControlTreeNode&)" );

  mBackEdges.insert(
    make_pair(
      ref( const_cast<WIR_ControlTreeNode &>( s ) ),
      ref( const_cast<WIR_ControlTreeNode &>( t ) ) ) );
};

}       // namespace WIR
