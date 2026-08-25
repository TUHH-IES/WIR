/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirunreachableblocks.cc
  @brief This file implements an optimization removing unreachable basic blocks.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <algorithm>
#include <set>

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
  Default constructor for system-level optimization.
*/
WIR_UnreachableBlocks::WIR_UnreachableBlocks( WIR_System &s ) :
  WIR_Optimization { s }
{
  DSTART( "WIR_UnreachableBlocks::WIR_UnreachableBlocks(WIR_System&)" );
};


/*
  Default constructor for compilation unit-level optimization.
*/
WIR_UnreachableBlocks::WIR_UnreachableBlocks( WIR_CompilationUnit &c ) :
  WIR_Optimization { c }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for function-level optimization.
*/
WIR_UnreachableBlocks::WIR_UnreachableBlocks( WIR_Function &f ) :
  WIR_Optimization { f }
{
  DSTART( "WIR_UnreachableBlocks::WIR_UnreachableBlocks(WIR_Function&)" );
};


/*
  Destructor.
*/
WIR_UnreachableBlocks::~WIR_UnreachableBlocks( void )
{
  DSTART( "virtual WIR_UnreachableBlocks::~WIR_UnreachableBlocks()" );
};


//
// Protected class methods
//

/*
  runOptimization removes unreachable basic blocks in the given system.
*/
void WIR_UnreachableBlocks::runOptimization( WIR_System &s )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  for ( WIR_CompilationUnit &c : s )
    runOptimization( c );
};


/*
  runOptimization removes unreachable basic blocks in the given compilation
  unit.
*/
void WIR_UnreachableBlocks::runOptimization( WIR_CompilationUnit &c )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  for ( WIR_Function &f : c )
    runOptimization( f );
};


/*
  runOptimization removes unreachable basic blocks in the given function.
*/
void WIR_UnreachableBlocks::runOptimization( WIR_Function &f )
{
  DSTART(
    "virtual void WIR_UnreachableBlocks::runOptimization(WIR_Function&)" );

  // Get list of visited basic blocks and collect their IDs.
  WIR_CFG cfg { f, true };
  set<WIR_id_t> visitedBBs;
  for ( const WIR_BasicBlock &b : cfg.getDFSOrder() )
    visitedBBs.insert( b.getID() );

  // Erase all basic blocks not occuring in visitedBBs.
  auto it = f.getBasicBlocks().begin();
  while ( it != f.getBasicBlocks().end() ) {
    // Check whether the current basic block is a literal pool that hence only
    // consists of ASM data directives.
    auto &b = it->get();
    bool isLiteralPool = b.getInstructions().empty() ? false : true;
    for ( const WIR_Instruction &i : b ) {
      if ( any_of(
             i.begin(), i.end(),
             [&]( const WIR_Operation &o ) {
               return( !o.isAsmDataDirective() ); } ) )
        isLiteralPool = false;

      if ( !isLiteralPool )
        break;
    }

    // Erase current basic block if it is unreachable AND not a literal pool.
    if ( !visitedBBs.count( b.getID() ) && !isLiteralPool ) {
      DOUT(
        "Removing unreachable basic block '" << it->get().getName() << "'." <<
        endl );

      // Update flow facts.
      WIR_FlowFactUpdater::eraseUnreachableBasicBlock( b );

      // Erase all bit-value containers associated with b's parameters.
      for ( WIR_Instruction &i : b )
        for ( WIR_Operation &o : i )
          for ( WIR_Parameter &p : o )
            if ( p.containsContainers( WIR_BitValues::getContainerTypeID() ) ) {
              // Get the current parameter's bitValue container.
              auto &cont = p.getContainers<WIR_BitValues>().begin()->get();

              // Iterate all incoming edges.
              for ( auto &inEdge : cont.getInValues() ) {
                auto &srcContainer =
                  inEdge.rp->getContainers<WIR_BitValues>().begin()->get();
                srcContainer.eraseOutValues( p );
              }
              cont.clearInValues();

              // Iterate all outgoing edges.
              for ( auto &outEdge : cont.getOutValues() ) {
                auto &tgtContainer =
                  outEdge.rp->getContainers<WIR_BitValues>().begin()->get();
                tgtContainer.eraseInValues( p );
              }
              cont.clearOutValues();
            }

      it = f.eraseBasicBlock( it );
    } else
      ++it;
  }
};

}       // namespace WIR
