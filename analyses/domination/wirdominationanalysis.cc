/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirdominationanalysis.cc
  @brief This file implements the %WIR domination analysis.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <set>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <analyses/generic/wirbitvector.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for function-level analysis.
*/
WIR_DominationAnalysis::WIR_DominationAnalysis( WIR_Function &f, bool i ) :
  WIR_Analysis { f },
  mProblemSize { 0 },
  mCFG { f, true },
  mAnalyzeInstructions { i }
{
  DSTART(
    "WIR_DominationAnalysis::WIR_DominationAnalysis(WIR_Function&, bool)" );
};


/*
  Destructor.
*/
WIR_DominationAnalysis::~WIR_DominationAnalysis( void )
{
  DSTART( "virtual WIR_DominationAnalysis::~WIR_DominationAnalysis()" );
};


//
// Protected class methods
//

/*
  runAnalysis performs domination analysis by iteration of the given function.
*/
void WIR_DominationAnalysis::runAnalysis( WIR_Function &f )
{
  DSTART( "virtual void WIR_DominationAnalysis::runAnalysis(WIR_Function&)" );

  DOUT( "Processing function '" << f.getName() << "'." << endl );

  // Initialize data structures.
  init( f );

  // Get depth-first order of all reachable basic blocks.
  auto &dfsBBs = mCFG.getDFSOrder();

  // Compute domination by iteration (Appel, page 413) over basic blocks.
  bool modified = false;

  do {
    modified = false;

    for ( const WIR_BasicBlock &b : dfsBBs )
      if ( mUpdateBlock[ b.getID() ] ) {
        mUpdateBlock[ b.getID() ] = false;

        auto &blockDominators = mBlockDominators.at( b.getID() );
        auto tmp = blockDominators;

        // Apply data flow equations.
        if ( !mCFG.getStartNodes().count(
               const_cast<WIR_BasicBlock &>( b ) ) ) {
          // mBlockDominators[ b ] := { b } U (∩ mBlockDominators[ pred ] )
          bool firstPredecessor = true;
          for ( const WIR_BasicBlock &pred : mPredecessors[ b.getID() ] ) {
            if ( firstPredecessor )
              blockDominators = mBlockDominators.at( pred.getID() );
            else
              blockDominators.set_intersection(
                mBlockDominators.at( pred.getID() ) );

            firstPredecessor = false;
          }

          blockDominators.setBit( mHash[ b.getID() ] );
        }

        // Check if dominators were modified in this iteration.
        // Set update flag of successor blocks if necessary.
        if ( tmp != blockDominators ) {
          modified = true;

          for ( const WIR_BasicBlock &succ : mSuccessors[ b.getID() ] )
            mUpdateBlock[ succ.getID() ] = true;
        }
      }
  } while ( modified );

  // Propagate basic block-level analysis data to instruction-level.
  propagateBB2Ins( f );
};


//
// Private class methods
//

/*
  init initializes internal data structures by collecting information about
  predecessor/successor relations between basic blocks.
*/
void WIR_DominationAnalysis::init( WIR_Function &f )
{
  DSTART( "void WIR_DominationAnalysis::init(WIR_Function&)" );

  (void) f;

  // Compute information about the data flow lattice, i.e., the number of
  // involved basic blocks and the hash and reverse-hash functions for the used
  // bit vectors.
  mProblemSize = 0;
  mHash.clear();
  mReverseHash.clear();

  // Get depth-first order of all reachable basic blocks.
  auto &dfsBBs = mCFG.getDFSOrder();

  // Set of IDs of all basic blocks in dfsBBs.
  set<WIR_id_t> blockIDs;

  for ( WIR_BasicBlock &b : dfsBBs ) {
    mHash[ b.getID() ] = mProblemSize;
    mReverseHash[ mProblemSize ] = &b;
    ++mProblemSize;

    blockIDs.insert( b.getID() );
  }

  mBlockDominators.clear();
  mPredecessors.clear();
  mSuccessors.clear();
  mUpdateBlock.clear();

  if ( mProblemSize == 0 )
    return;

  WIR_BitVector allOne { mProblemSize };
  for ( size_t i = 0; i < mProblemSize; ++i )
    allOne.setBit( i );

  // Determine non-empty predecessors/successors per WIR basic block.
  for ( WIR_BasicBlock &b : dfsBBs ) {
    // Initialize all dominators to the universe, except for a CFG's start node.
    if ( mCFG.getStartNodes().count( b ) ) {
      auto &dom =
        mBlockDominators.insert(
          make_pair(
            b.getID(), WIR_BitVector { mProblemSize } ) ).first->second;
      dom.setBit( mHash[ b.getID() ] );
    } else
      mBlockDominators.insert(
        make_pair( b.getID(), WIR_BitVector { allOne } ) );

    for ( WIR_BasicBlock &pred : b.getPredecessors() )
      // Skip predecessors that are not reached by a DFS traversal of the CFG.
      if ( ( pred != b ) && blockIDs.count( pred.getID() ) )
        mPredecessors[ b.getID() ].insert( pred );
    for ( WIR_BasicBlock &succ : b.getSuccessors() )
      if ( succ != b )
        mSuccessors[ b.getID() ].insert( succ );
    mUpdateBlock[ b.getID() ] = true;

    // Clear previous analysis results.
    b.eraseContainers( WIR_Domination::getContainerTypeID() );

    for ( WIR_Instruction &i : b )
      i.eraseContainers( WIR_Domination::getContainerTypeID() );
  }
};


/*
  propagateBB2Ins propagates basic block-level analysis results to
  instruction-level.
*/
void WIR_DominationAnalysis::propagateBB2Ins( WIR_Function &f )
{
  DSTART( "void WIR_DominationAnalysis::propagateBB2Ins(WIR_Function&)" );

  // Free some no longer needed memory.
  mUpdateBlock.clear();
  mPredecessors.clear();
  mSuccessors.clear();

  // Propagate basic block-level analysis results down to the instruction level.
  for ( WIR_BasicBlock &b : f ) {
    // Attach a fresh container for dominator sets to the current basic block.
    b.insertContainer( new WIR_Domination() );
    WIR_Domination &bRes = b.getContainers<WIR_Domination>().begin()->get();

    // Save block-level domination data in its associated container.
    auto it = mBlockDominators.find( b.getID() );
    if ( it != mBlockDominators.end() ) {
      // b is a reachable block considered during analysis.
      const auto &dom = it->second;

      for ( size_t i = 0; i < mProblemSize; ++i )
        if ( dom[ i ] )
          bRes.insertDominator( *(mReverseHash[ i ]) );

      // Propagate dominators down to the instruction-level if desired.
      if ( mAnalyzeInstructions )
        for ( WIR_Instruction &i : b ) {
          // Attach a fresh container for dominator sets to the current
          // instruction.
          i.insertContainer( new WIR_Domination() );
          WIR_Domination &iRes =
            i.getContainers<WIR_Domination>().begin()->get();

          // All instructions j of all dominating basic blocks db != b also
          // dominate i.
          for ( const WIR_BasicBlock &db : bRes.getDominatorBlocks() )
            if ( db != b )
              for ( const WIR_Instruction &j : db )
                iRes.insertDominator( j );

          // Instructions j of basic block b in front of i also dominate i.
          for ( const WIR_Instruction &j : b ) {
            iRes.insertDominator( j );

            if ( j == i )
              break;
          }
        }
    } else {
      // b is unreachable, this is a pathological case where a block just
      // dominates itself.
      bRes.insertDominator( b );

      // Propagate dominators down to the instruction-level if desired.
      if ( mAnalyzeInstructions )
        for ( WIR_Instruction &i : b ) {
          // Attach a fresh container for dominator sets to the current
          // instruction.
          i.insertContainer( new WIR_Domination() );
          WIR_Domination &iRes =
            i.getContainers<WIR_Domination>().begin()->get();

          // Instruction i simply dominates itself in this pathology.
          iRes.insertDominator( i );
        }
    }
  }

  // Free some no longer needed memory.
  mBlockDominators.clear();
  mReverseHash.clear();
  mHash.clear();
};

}       // namespace WIR
