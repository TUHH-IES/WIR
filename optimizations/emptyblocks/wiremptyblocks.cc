/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wiremptyblocks.cc
  @brief This file implements an optimization removing empty basic blocks.

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
  Default constructor for system-level optimization.
*/
WIR_EmptyBlocks::WIR_EmptyBlocks( WIR_System &s ) :
  WIR_Optimization { s }
{
  DSTART( "WIR_EmptyBlocks::WIR_EmptyBlocks(WIR_System&)" );
};


/*
  Default constructor for compilation unit-level optimization.
*/
WIR_EmptyBlocks::WIR_EmptyBlocks( WIR_CompilationUnit &c ) :
  WIR_Optimization { c }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for function-level optimization.
*/
WIR_EmptyBlocks::WIR_EmptyBlocks( WIR_Function &f ) :
  WIR_Optimization { f }
{
  DSTART( "WIR_EmptyBlocks::WIR_EmptyBlocks(WIR_Function&)" );
};


/*
  Destructor.
*/
WIR_EmptyBlocks::~WIR_EmptyBlocks( void )
{
  DSTART( "virtual WIR_EmptyBlocks::~WIR_EmptyBlocks()" );
};


//
// Protected class methods
//

/*
  runOptimization removes empty basic blocks in the given system.
*/
void WIR_EmptyBlocks::runOptimization( WIR_System &s )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  for ( WIR_CompilationUnit &c : s )
    runOptimization( c );
};


/*
  runOptimization removes empty basic blocks in the given compilation unit.
*/
void WIR_EmptyBlocks::runOptimization( WIR_CompilationUnit &c )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  for ( WIR_Function &f : c )
    runOptimization( f );
};


/*
  runOptimization removes empty basic blocks in the given function.
*/
void WIR_EmptyBlocks::runOptimization( WIR_Function &f )
{
  DSTART( "virtual void WIR_EmptyBlocks::runOptimization(WIR_Function&)" );

  // Traverse all basic blocks of f.
  auto it = f.begin();
  while ( it != f.end() ) {
    WIR_BasicBlock &b = it->get();

    // Remove any empty instruction from b, if any.
    auto iit = b.begin();
    while ( iit != b.end() ) {
      const WIR_Instruction &i = iit->get();

      if ( i.getOperations().empty() )
        iit = b.eraseInstruction( iit );
      else
        ++iit;
    }

    if ( b.getInstructions().size() == 0 ) {
      // Found an empty basic block to be removed. By definition, such a basic
      // block can have at most one successor.

      // Determine first non-empty successor basic block (transitive).
      auto succIt = std::next( it );
      while ( ( succIt != f.end() ) && succIt->get().getInstructions().empty() )
        ++succIt;

      if ( succIt != f.end() ) {
        // Found a non-empty successor block.

        // Replace all occurrences of b's label by this successor's label.
        for ( const WIR_BasicBlock &pred : b.getPredecessors() )
          for ( auto lastIns = pred.rbegin(); lastIns != pred.rend();
                ++lastIns ) {
            for ( WIR_Operation &o : lastIns->get() )
              for ( auto paramIt = o.begin(); paramIt != o.end(); ++paramIt )
                if ( paramIt->get().getType() == WIR_ParameterType::label ) {
                  const auto &lp =
                    dynamic_cast<const WIR_LabelParameter &>( paramIt->get() );

                  if ( ( lp.getLabelType() == WIR_SymbolType::block ) &&
                       ( lp.getBasicBlock() == b ) ) {
                    DOUT(
                      "Replacing label '" << lp.getBasicBlock().getName() <<
                      "' by '" << succIt->get().getName() << "'." << endl );
                    paramIt =
                      o.replaceParameter(
                        paramIt, WIR_LabelParameter( succIt->get() ) );
                  }
                }

            // Exit from the loop over all instructions, since only the very
            // last instruction of a basic block is allowed to be a jump with
            // labels.
            break;
          }

        // Move loop bounds from b to its non-empty successor.
        if ( b.containsContainers( WIR_FlowFactRef::getContainerTypeID() ) ) {
          auto &ffRef = WIR_FlowFactRef::get( b );
          auto lbs = ffRef.getFlowFacts<WIR_LoopBound>();

          for ( WIR_LoopBound &lb : lbs )
            lb.setLoop( succIt->get(), lb.getLoopControlType(), false );
        }

        // Move containers from b to its non-empty successor, except flow
        // fact-related containers, since flow facts are already handled above
        // and below.
        for ( WIR_BaseContainer &c : b.getContainers() )
          if ( c.getContainerType() != WIR_FlowFactRef::getContainerTypeID() )
            succIt->get().insertContainer( c );
        b.clearContainers();

        // Since the successor basic block is executed every time that the empty
        // basic block is excecuted, the former can be replaced by the latter in
        // flow restrictions.
        WIR_FlowFactUpdater::replaceBasicBlock( b, succIt->get() );
      }

      // Update flow facts.

      // There should not be any loop bounds attached to an empty basic block,
      // but just in case, we erase them now.
      WIR_FlowFactUpdater::eraseLoopBound( b );

      // Finally, remove current basic block.
      DOUT(
        "Removing empty basic block '" << it->get().getName() << "'." << endl );
      it = f.eraseBasicBlock( it );
    } else
      ++it;
  }
};

}       // namespace WIR
