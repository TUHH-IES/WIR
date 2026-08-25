/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirlivenessanalysis.cc
  @brief This file implements the %WIR liveness analysis.

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
WIR_LivenessAnalysis::WIR_LivenessAnalysis( WIR_Function &f ) :
  WIR_Analysis { f },
  mProblemSize { 0 }
{
  DSTART( "WIR_LivenessAnalysis::WIR_LivenessAnalysis(WIR_Function&)" );
};


/*
  Destructor.
*/
WIR_LivenessAnalysis::~WIR_LivenessAnalysis( void )
{
  DSTART( "virtual WIR_LivenessAnalysis::~WIR_LivenessAnalysis()" );
};


//
// Protected class methods
//

/*
  runAnalysis performs liveness analysis by iteration of the given function.
*/
void WIR_LivenessAnalysis::runAnalysis( WIR_Function &f )
{
  DSTART( "virtual void WIR_LivenessAnalysis::runAnalysis(WIR_Function&)" );

  // Initialize instruction-level data structures.
  init( f );

  // Propagate instruction-level data to basic block-level.
  propagateIns2BB( f );

  // Get reverse-topologically sorted list of basic blocks.
  WIR_CFG cfg { f };
  auto &topologicBBs = cfg.getReverseTopologicalOrder();

  // Compute liveness by iteration (Appel, page 221, Algorithm 10.4) over basic
  // blocks. live-outs are computed before live-ins (Appel, page 222).
  bool modified = false;

  do {
    modified = false;

    for ( const WIR_BasicBlock &b : topologicBBs )
      if ( mUpdateBlock[ b.getID() ] ) {
        mUpdateBlock[ b.getID() ] = false;

        auto &blockLiveIn = mBlockLiveIn.at( b.getID() );
        auto &blockLiveOut = mBlockLiveOut.at( b.getID() );
        const auto &blockDefs = mBlockDefs.at( b.getID() );

        // Apply data flow equations.
        for ( const WIR_BasicBlock &succ : mSuccessors[ b.getID() ] ) {
          // mBlockLiveOut[ b ] := mBlockLiveOut[ b ] U mBlockLiveIn[ succ ]
          blockLiveOut.set_union( mBlockLiveIn.at( succ.getID() ) );

          // According to Appel and Muchnick:
          //   diff := mBlockLiveOut[ b ] - mBlockDefs[ b ]
          //   mBlockLiveIn[ b ] := mBlockUses[ b ] U diff
          //
          // However, the data flow equations are monotone from iteration to
          // iteration, i.e., the incoming/outgoing live sets can only increase.
          // Thus, mBlockUses[ b ] in the above equation must always be part of
          // mBlockLiveIn[ b ], it is independent of this current analysis
          // iteration. Hence, the consideration of mBlockUses is already done
          // in method propagateIns2BB below and not here in the innermost loop
          // of the analysis.
          // As a consequence, only those live ranges added to set diff above
          // must iteratively be added to mBlockLiveIn[ b ]. Exactly this is
          // done in the lines below.
          WIR_BitVector diff { blockLiveOut };
          diff.set_difference( blockDefs );
          blockLiveIn.set_union( diff );
        }

        // Check if live-ins were modified in this iteration.
        // Set update flag of predecessor blocks if necessary.
        if ( blockLiveIn.isModified() ) {
          modified = true;

          for ( const WIR_BasicBlock &pred : mPredecessors[ b.getID() ] )
            mUpdateBlock[ pred.getID() ] = true;
        }

        // Check if live-outs were modified in this iteration.
        modified |= blockLiveOut.isModified();
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
  register definitions/uses of WIR instructions and of predecessor/successor
  relations between basic blocks.
*/
void WIR_LivenessAnalysis::init( WIR_Function &f )
{
  DSTART( "void WIR_LivenessAnalysis::init(WIR_Function&)" );

  // Compute information about the data flow lattice, i.e., the number of
  // involved registers and the hash and reverse-hash functions for the used
  // bit vectors.
  mProblemSize = 0;
  mHash.clear();
  mReverseHash.clear();

  set<WIR_id_t> consideredRegs;

  // A small lambda to consider a register during analysis.
  auto considerReg = [&]( WIR_BaseRegister &r ) {
    if ( !consideredRegs.count( r.getID() ) ) {
      mHash[ r.getID() ] = mProblemSize;
      mReverseHash[ mProblemSize ] = &r;
      mProblemSize++;
      consideredRegs.insert( r.getID() );
    }
  };

  for ( const WIR_BasicBlock &b : f )
    for ( const WIR_Instruction &i : b )
      for ( const WIR_Operation &o : i )
        for ( const WIR_Parameter &p : o )
          if ( p.getType() == WIR_ParameterType::reg ) {
            const auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p );
            const auto &r = rp.getRegister();

            if ( r.isVirtual() )
              for ( WIR_VirtualRegister &leaf :
                      dynamic_cast<const WIR_VirtualRegister &>(
                        r ).getLeafs() ) {
                considerReg( leaf );

                if ( leaf.isPrecolored() )
                  considerReg( leaf.getPrecolor() );
              }
            else
              for ( WIR_PhysicalRegister &leaf :
                      dynamic_cast<const WIR_PhysicalRegister &>(
                        r ).getLeafs() )
                considerReg( leaf );
          }

  // Extract definitions/uses per WIR instruction.
  mInstrDefs.clear();
  mInstrUses.clear();

  for ( WIR_BasicBlock &b : f )
    for ( WIR_Instruction &i : b ) {
      auto &instrDef =
        mInstrDefs.insert(
          make_pair(
            i.getID(), WIR_BitVector { mProblemSize } ) ).first->second;
      auto &instrUse =
        mInstrUses.insert(
          make_pair(
            i.getID(), WIR_BitVector { mProblemSize } ) ).first->second;

      // Clear previous analysis results.
      i.eraseContainers( WIR_LiveOut::getContainerTypeID() );

      // Compute sets of registers defined and used by i.
      for ( WIR_Operation &o : i )
        for ( WIR_Parameter &p : o )
          if ( p.getType() == WIR_ParameterType::reg ) {
            WIR_RegisterParameter &rp =
              dynamic_cast<WIR_RegisterParameter &>( p );
            WIR_BaseRegister &r = rp.getRegister();

            if ( r.isVirtual() ) {
              for ( WIR_VirtualRegister &leaf :
                      dynamic_cast<WIR_VirtualRegister &>( r ).getLeafs() ) {
                if ( rp.isUsed() || rp.isDefUsed() ) {
                  instrUse.setBit( mHash[ leaf.getID() ] );
                  // Add precolored physical registers to mInstrUses.
                  if ( leaf.isPrecolored() )
                    instrUse.setBit( mHash[ leaf.getPrecolor().getID() ] );
                }

                if ( rp.isDefined() || rp.isDefUsed() ) {
                  instrDef.setBit( mHash[ leaf.getID() ] );
                  // Add precolored physical registers to mInstrDefs.
                  if ( leaf.isPrecolored() ) {
                    instrDef.setBit( mHash[ leaf.getPrecolor().getID() ] );

                    // i defines the virtual register leaf which is pre-colored
                    // with some physical register R. Thus, i implicitly also
                    // defines all other virtual registers that are also pre-
                    // colored with R.
                    for ( const WIR_VirtualRegister &r1 :
                            f.getVirtualRegisters() )
                      for ( const WIR_VirtualRegister &leaf1 : r1.getLeafs() )
                        if ( ( leaf1 != leaf ) && leaf1.isPrecolored() &&
                             ( leaf1.getPrecolor() == leaf.getPrecolor() ) )
                          instrDef.setBit( mHash[ leaf1.getID() ] );
                  }
                }
              }
            } else
              for ( WIR_PhysicalRegister &leaf :
                      dynamic_cast<WIR_PhysicalRegister &>( r ).getLeafs() ) {
                if ( rp.isUsed() || rp.isDefUsed() )
                  instrUse.setBit( mHash[ leaf.getID() ] );
                if ( rp.isDefined() || rp.isDefUsed() )
                  instrDef.setBit( mHash[ leaf.getID() ] );
              }
          }
    }

  // Determine non-empty predecessors/successors per WIR basic block.
  mPredecessors.clear();
  mSuccessors.clear();
  mUpdateBlock.clear();
  mBlockLiveIn.clear();
  mBlockLiveOut.clear();
  mBlockDefs.clear();

  for ( const WIR_BasicBlock &b : f ) {
    for ( WIR_BasicBlock &pred : b.getPredecessors() )
      mPredecessors[ b.getID() ].insert( pred );
    for ( WIR_BasicBlock &succ : b.getSuccessors() )
      mSuccessors[ b.getID() ].insert( succ );
    mUpdateBlock[ b.getID() ] = true;

    // Replace empty basic blocks by their non-empty predecessors/successors.
    WIR_BasicBlockSet nonEmpty;
    while ( !mPredecessors[ b.getID() ].empty() ) {
      WIR_BasicBlock &pred = mPredecessors[ b.getID() ].begin()->get();
      mPredecessors[ b.getID() ].erase( pred );

      if ( pred.getInstructions().empty() )
        for ( WIR_BasicBlock &ppred : pred.getPredecessors() )
          mPredecessors[ b.getID() ].insert( ppred );
      else
        nonEmpty.insert( pred );
    }
    mPredecessors[ b.getID() ] = std::move( nonEmpty );

    nonEmpty.clear();
    while ( !mSuccessors[ b.getID() ].empty() ) {
      WIR_BasicBlock &succ = mSuccessors[ b.getID() ].begin()->get();
      mSuccessors[ b.getID() ].erase( succ );

      if ( succ.getInstructions().empty() )
        for ( WIR_BasicBlock &ssucc : succ.getSuccessors() )
          mSuccessors[ b.getID() ].insert( ssucc );
      else
        nonEmpty.insert( succ );
    }
    mSuccessors[ b.getID() ] = std::move( nonEmpty );
  }
};


/*
  propagateIns2BB propagates instruction-level def/use information to basic
  block-level where liveness analysis is actually done (Appel, pages 394-395).
*/
void WIR_LivenessAnalysis::propagateIns2BB( const WIR_Function &f )
{
  DSTART( "void WIR_LivenessAnalysis::propagateIns2BB(const WIR_Function&)" );

  // Aggregate all instruction-level def/use values for basic blocks.
  for ( const WIR_BasicBlock &b : f ) {
    auto &blockDef =
      mBlockDefs.insert(
        make_pair(
          b.getID(), WIR_BitVector { mProblemSize } ) ).first->second;
    auto &blockLiveIn =
      mBlockLiveIn.insert(
        make_pair(
          b.getID(), WIR_BitVector { mProblemSize } ) ).first->second;
    mBlockLiveOut.insert(
      make_pair( b.getID(), WIR_BitVector { mProblemSize } ) );

    WIR_BitVector definedRegs { mProblemSize };

    for ( const WIR_Instruction &i : b ) {
      // mBlockUses[ b ] := mInstrUses[ i ] - definedRegs
      // Since mBlockUses only serves for the proper initialization of
      // mBlockLiveIn[ b ], we do not compute mBlockUses here explicitly but
      // instead simply initialize mBlockLiveIn accordingly.
      WIR_BitVector &instrDef = mInstrDefs.at( i.getID() );
      WIR_BitVector instrUse = mInstrUses.at( i.getID() );
      instrUse.set_difference( definedRegs );
      blockLiveIn.set_union( instrUse );

      // mBlockDefs[ b ] := mBlockDefs[ b ] U mInstrDefs[ i ]
      blockDef.set_union( instrDef );
      definedRegs.set_union( instrDef );
    }
  }
};


/*
  propagateBB2Ins propagates basic block-level analysis results to
  instruction-level.
*/
void WIR_LivenessAnalysis::propagateBB2Ins( WIR_Function &f )
{
  DSTART( "void WIR_LivenessAnalysis::propagateBB2Ins(WIR_Function&)" );

  // Free some no longer needed memory.
  mUpdateBlock.clear();
  mPredecessors.clear();
  mSuccessors.clear();
  mBlockDefs.clear();
  mBlockLiveIn.clear();

  // Propagate basic block-level analysis results down to the instruction level.
  for ( WIR_BasicBlock &b : f ) {
    // liveInSucc contains the set of live-in registers at the end of the
    // current instruction i.
    WIR_BitVector liveInSucc { mProblemSize };

    for ( auto it = b.getInstructions().rbegin();
          it != b.getInstructions().rend(); ++it ) {
      WIR_Instruction &i = (*it).get();

      // Attach a fresh container for live-out sets to the current instruction.
      i.insertContainer( new WIR_LiveOut() );
      WIR_LiveOut &res = i.getContainers<WIR_LiveOut>().begin()->get();

      // liveOut contains the set of live-out registers at the end of the
      // current instruction i.
      WIR_BitVector liveOut =
        ( i == *(b.getInstructions().rbegin()) ) ?
          // For the last instruction of a basic block, its live-out set is that
          // of the entire block.
          std::move( mBlockLiveOut.at( b.getID() ) ) :
          // liveOut[ i ] := liveIn[ succ ]
          std::move( liveInSucc );

      // Save live-out data in its associated container.
      for ( size_t j = 0; j < mProblemSize; ++j )
        if ( liveOut[ j ] )
          res.insertRegister( *(mReverseHash[ j ]) );

      // liveIn[ i ] := mInstrUses[ i ] U ( liveOut[ i ] - mInstrDefs[ i ] )
      // Since the current instruction i will be successor instruction in the
      // next iteration of this loop over i, we directly store the results in
      // liveInSucc.
      liveInSucc = std::move( mInstrUses.at( i.getID() ) );
      WIR_BitVector diff { liveOut };
      diff.set_difference( mInstrDefs.at( i.getID() ) );
      liveInSucc.set_union( diff );
    }
  }

  // Free some no longer needed memory.
  mBlockLiveOut.clear();
  mInstrDefs.clear();
  mInstrUses.clear();
  mReverseHash.clear();
  mHash.clear();
};

}       // namespace WIR
