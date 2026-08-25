/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirifthenelsetreenode.cc
  @brief This file implements if-then-else tree nodes.

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
WIR_IfThenElseTreeNode::WIR_IfThenElseTreeNode( const WIR_ControlTreeNode &c,
                                                const WIR_ControlTreeNode &b1,
                                                const WIR_ControlTreeNode &b2 ) :
  WIR_ControlTreeNode {},
  mCondition { c },
  mBranch1 { b1 },
  mBranch2 { b2 }
{
  DSTART(
    "WIR_IfThenElseTreeNode::WIR_IfThenElseTreeNode(const WIR_ControlTreeNode&, const WIR_ControlTreeNode&, const WIR_ControlTreeNode&)" );
};


/*
  Destructor.
*/
WIR_IfThenElseTreeNode::~WIR_IfThenElseTreeNode( void )
{
  DSTART( "virtual WIR_IfThenElseTreeNode::~WIR_IfThenElseTreeNode()" );
};


/*
  getType returns the type of an if-then-else region node.
*/
WIR_CTNodeType WIR_IfThenElseTreeNode::getType( void ) const
{
  DSTART( "virtual WIR_CTNodeType WIR_IfThenElseTreeNode::getType() const" );

  return( WIR_CTNodeType::ifthenelse );
};


/*
  isCyclic returns whether an if-then-else region is cyclic or not.
*/
bool WIR_IfThenElseTreeNode::isCyclic( void ) const
{
  DSTART( "virtual bool WIR_IfThenElseTreeNode::isCyclic() const" );

  return( false );
};


/*
  getEntry returns an if-then-else region's unique entry child node.
*/
const WIR_ControlTreeNode &WIR_IfThenElseTreeNode::getEntry( void ) const
{
  DSTART(
    "virtual const WIR_ControlTreeNode& WIR_IfThenElseTreeNode::getEntry() const" );

  return( mCondition );
};


/*
  getCondition returns an if-then-else's condition node.
*/
const WIR_ControlTreeNode& WIR_IfThenElseTreeNode::getCondition( void ) const
{
  DSTART(
    "const WIR_ControlTreeNode& WIR_IfThenElseTreeNode::getCondition() const" );

  return( mCondition );
};


/*
  getBranch1 returns the control tree node representing the first branch of an
  if-then-else.
*/
const WIR_ControlTreeNode& WIR_IfThenElseTreeNode::getBranch1( void ) const
{
  DSTART(
    "const WIR_ControlTreeNode& WIR_IfThenElseTreeNode::getBranch1() const" );

  return( mBranch1 );
};


/*
  getBranch2 returns the control tree node representing the second branch of an
  if-then-else.
*/
const WIR_ControlTreeNode& WIR_IfThenElseTreeNode::getBranch2( void ) const
{
  DSTART(
    "const WIR_ControlTreeNode& WIR_IfThenElseTreeNode::getBranch2() const" );

  return( mBranch2 );
};


//
// Protected class methods
//

/*
  visualize dumps an if-then-else region node into a given DOT file.
*/
void WIR_IfThenElseTreeNode::visualize( std::fstream &dotFile ) const
{
  DSTART( "virtual void WIR_IfThenElseTreeNode::visualize(fstream&) const" );

  dotFile << "subgraph cluster" << getID() << " {" << endl;

  dotFile << "  label=\"" << buildNodeName() << "\";" << endl;
  dotFile << "  color=blue;" << endl;

  // Recursively visualize all included hierarchical child nodes.
  mCondition.visualize( dotFile );
  mBranch1.visualize( dotFile );
  mBranch2.visualize( dotFile );

  dotFile << "}" << endl;
};

}       // namespace WIR
