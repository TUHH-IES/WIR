/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirjumpcorrection.cc
  @brief This file implements a generic optimimzation detecting and correcting
         jump instructions with too large displacements.

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

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <optimizations/jumpcorrection/wirjumpcorrection.h>


//
// Preprocessor macros
//

// #define ENABLE_INVARIANTS

#ifdef ENABLE_INVARIANTS

// This invariant verifies that the memory addresses as determined by the full
// WIR memory layout and by the internal light-weight data structure mMemLayout
// are the same for a WIR basic block.
#define DEBUG_MEMORYLAYOUT_INVARIANT( __b )                                    \
  {                                                                            \
    DSTART( "WIR_JumpCorrection.invariants" );                                 \
    DACTION(                                                                   \
      DOUT(                                                                    \
        "Checking memory layout invariant for block '" << __b.getName() <<     \
        "' (0x" << hex << mBBPosition.at( __b.getID() )->address << ")." <<    \
        endl );                                                                \
      auto &__sym = mSystem.findSymbol( __b );                                 \
      bool __memLayoutInvariant =                                              \
        ( __sym.getBaseAddress().getContent() ==                               \
            mBBPosition.at( __b.getID() )->address );                          \
      if ( !__memLayoutInvariant )                                             \
        DOUT(                                                                  \
          "Memory Layout Invariant failed for basic block '" <<                \
          __b.getName() << "': memory layout = 0x" << hex <<                   \
          __sym.getBaseAddress().getContent() << ", mMemLayout = 0x" <<        \
          mBBPosition.at( __b.getID() )->address << endl );                    \
      ufAssert( __memLayoutInvariant );                                        \
    );                                                                         \
  }

#else

#define DEBUG_MEMORYLAYOUT_INVARIANT(...)

#endif


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
WIR_JumpCorrection::WIR_JumpCorrection( WIR_System &s ) :
  WIR_Optimization { s },
  mSystem { s },
  mPhysicalWIR { true },
  mSinglePass { false },
  mCorrectedJumps { 0 }
{
  DSTART( "WIR_JumpCorrection::WIR_JumpCorrection(WIR_System&)" );
};


/*
  Destructor.
*/
WIR_JumpCorrection::~WIR_JumpCorrection( void )
{
  DSTART( "virtual WIR_JumpCorrection::~WIR_JumpCorrection()" );
};


/*
  setSinglePass sets whether jump correction is applied once or if it executes
  until a fixed point is reached.
*/
void WIR_JumpCorrection::setSinglePass( bool b )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  mSinglePass = b;
};


/*
  getSinglePass returns whether jump correction is applied once or if it
  executes until a fixed point is reached.
*/
bool WIR_JumpCorrection::getSinglePass( void ) const
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  return( mSinglePass );
};


//
// Protected class methods
//

WIR_JumpCorrection::MemLayoutInfo::MemLayoutInfo( const WIR_BasicBlock &b,
                                                  unsigned long long adr ) :
  block { &b },
  address { adr },
  alignment { 0 },
  fixedAddress { false }
{
  DSTART(
    "WIR_JumpCorrection::MemLayoutInfo::MemLayoutInfo(const WIR_BasicBlock&, "
    "long long unsigned int)" );
};


/*
  Default constructor for invalid jump containers.
*/
WIR_JumpCorrection::InvalidJump::InvalidJump( const WIR_Operation &o,
                                              const WIR_Symbol &s,
                                              const WIR_Symbol &t,
                                              const WIR_disp_t d,
                                              const bool i ) :
  op { o },
  srcSym { s },
  tgtSym { t },
  disp { d },
  isImplicit { i }
{
  DSTART(
    "WIR_JumpCorrection::InvalidJump::InvalidJump(const WIR_Operation&, const "
    "WIR_Symbol&, const WIR_Symbol&, WIR_disp_t, bool)" );
};


/*
  runOptimization performs jump correction in the given system.
*/
void WIR_JumpCorrection::runOptimization( WIR_System &s )
{
  DSTART( "virtual void WIR_JumpCorrection::runOptimization(WIR_System&)" );

  // Determine whether jump correction is applied to a physical or a virtual
  // WIR.
  mPhysicalWIR = true;
  for ( const WIR_CompilationUnit &c : s ) {
    if ( any_of(
           c.begin(), c.end(),
           [&]( const reference_wrapper<WIR_Function> ref ) {
             return( !ref.get().getVirtualRegisters().empty() ); } ) )
      mPhysicalWIR = false;

    if ( !mPhysicalWIR )
      break;
  }

  initializeMemoryLayout();

  do {
    mCorrectedJumps = 0;

    for ( WIR_CompilationUnit &c : s )
      runOptimization( c );
  } while ( ( mCorrectedJumps != 0 ) && !mSinglePass );
};


/*
  runOptimization performs jump correction in the given compilation unit.
*/
void WIR_JumpCorrection::runOptimization( WIR_CompilationUnit &c )
{
  DSTART(
    "virtual void WIR_JumpCorrection::runOptimization(WIR_CompilationUnit&)" );

  for ( WIR_Function &f : c ) {
    runOptimization( f );

    // Cleanup the current function.
    WIR_EmptyBlocks{ f }.optimize();
    WIR_UnreachableBlocks{ f }.optimize();
    WIR_RedundantBlocks{ f }.optimize();
  }
};


/*
  runOptimization performs jump correction in the given function.
*/
void WIR_JumpCorrection::runOptimization( WIR_Function &f )
{
  DSTART( "virtual void WIR_JumpCorrection::runOptimization(WIR_Function&)" );

  {
    DSTART(
      "virtual void WIR_JumpCorrection::runOptimization("
      "WIR_Function&).visualize" );
    DACTION( WIR_CFG{ f }.visualize( true ); );
  }

  auto it = f.begin();
  while ( it != f.end() ) {
    WIR_BasicBlock &b = it->get();

    DDECLARE(
      const unsigned int prevCorrections = mCorrectedJumps;
      const unsigned int prevInstructions = b.getInstructions().size();
      bool blockModified = false; );

    // Skip empty basic blocks.
    if ( !b.getInstructions().empty() )
      runOpt( b );

    DACTION(
      if ( ( mCorrectedJumps != prevCorrections ) ||
           ( b.getInstructions().size() != prevInstructions ) )
        blockModified = true; );

    if ( b.getInstructions().empty() ) {
      // If the current basic block is empty, e.g., due to removal of a
      // redundant jump or of a trampoline, let's remove it.

      // Determine first non-empty successor basic block (transitive).
      auto succIt = std::next( it );
      while ( ( succIt != f.end() ) &&
              succIt->get().getInstructions().empty() )
        ++succIt;

      if ( succIt != f.end() ) {
        // Found a non-empty successor block.

        // Replace all occurrences of b's label by this successor's label.
        for ( const WIR_BasicBlock &pred : b.getPredecessors() ) {
          DOUT(
            "Checking predecessor '" << pred.getName() << "' of '" <<
            b.getName() << "'." << endl );

          for ( auto lastIns = pred.rbegin(); lastIns != pred.rend();
                ++lastIns ) {
            for ( WIR_Operation &o : lastIns->get() )
              for ( auto paramIt = o.begin(); paramIt != o.end(); ++paramIt )
                if ( paramIt->get().getType() == WIR_ParameterType::label ) {
                  auto &lp =
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
        }

        // Move containers from b to its non-empty successor, except flow
        // fact-related containers, since flow facts are already handled
        // below.
        for ( WIR_BaseContainer &c : b.getContainers() )
          if ( c.getContainerType() != WIR_FlowFactRef::getContainerTypeID() )
            succIt->get().insertContainer( c );
      }

      // Update flow facts.

      // Since the successor basic block is executed every time that the empty
      // basic block is excecuted, the former can be replaced by the latter in
      // flow restrictions.
      WIR_FlowFactUpdater::replaceBasicBlock( b, succIt->get() );

      // There should not be any loop bounds attached to an empty basic block,
      // but just in case, we erase them now.
      WIR_FlowFactUpdater::eraseLoopBound( b );

      DOUT( "Removing empty basic block '" << b.getName() << "'." << endl );

      // Update memory layout.
      auto bbPos = mBBPosition[ b.getID() ];
      const auto &r = mSystem.findSymbol( b ).getSection().getRegion();
      mMemLayout[ r.getID() ].erase( bbPos );
      mBBPosition.erase( b.getID() );

      it = f.eraseBasicBlock( it );
      DACTION( blockModified = true; );

      verifyMemoryLayout();
    } else
      ++it;

    {
      DSTART(
        "virtual void WIR_JumpCorrection::runOptimization("
        "WIR_Function&).visualize" );
      DACTION(
        if ( blockModified )
          WIR_CFG{ f }.visualize( true ); );
    }
  }

  // Cleanup the current function, i.e., remove redundant basic blocks.

  // Traverse all basic blocks of f.
  for ( auto it1 = f.begin(); it1 != f.end(); ++it1 ) {
    WIR_BasicBlock &b1 = it1->get();

    // Check if b1 ends with a call. If so, we skip b1.
    bool b1HasCall = false;
    if ( !b1.getInstructions().empty() ) {
      const WIR_Instruction &i = b1.rbegin()->get();
      b1HasCall =
        any_of(
          i.begin(), i.end(),
          [&]( const reference_wrapper<WIR_Operation> ref ) {
            return( ref.get().isCall() || ref.get().isIndirectCall() ); } );
    }
    if ( b1HasCall )
      continue;

    bool redundantSuccessorRemoved = false;

    do {
      // Determine successor block, if any.
      redundantSuccessorRemoved = false;
      auto it2 = std::next( it1 );

      if ( it2 != f.getBasicBlocks().end() ) {
        WIR_BasicBlock &b2 = it2->get();

        auto b1Succs = b1.getSuccessors();
        auto b2Preds = b2.getPredecessors();

        // b1 and b2 are redundant if
        // - b1 has exactly one successor, namely b2, AND
        // - b2 has exactly one predecessor, namely b1, AND
        // - b1 and b2 are allocated to the same memory region, AND
        // - b2 is the immediate successor of b1 within the current WIR
        //   function.
        if ( ( b1Succs.size() == 1 ) && ( b1Succs.begin()->get() == b2 ) &&
             ( b2Preds.size() == 1 ) && ( b2Preds.begin()->get() == b1 ) ) {
          // Found two redundant basic blocks b1 and b2 to be merged.
          DOUT(
            "Found redundant basic blocks '" + b1.getName() + "' and '" +
            b2.getName() + "'." << endl );

          // Remove potential jump operations from b1's tail.
          if ( !b1.getInstructions().empty() ) {
            WIR_Instruction &i = b1.rbegin()->get();
            for ( auto oit = i.begin(); oit != i.end(); ) {
              WIR_Operation &o = oit->get();
              if ( o.isJump() ) {
                DOUT( "Removing jump operation " << o << endl );
                o.setDontOptimize( false );
                i.setDontOptimize( false );
                oit = i.eraseOperation( oit );
              } else
                ++oit;
            }

            if ( i.getOperations().empty() ) {
              DOUT(
                "Removing empty last instruction of basic block '" +
                b1.getName() + "'." << endl );
              i.setDontOptimize( false );
              b1.eraseInstruction( --(b1.end()) );
            }
          }

          // Move instructions from b2 to b1.
          bool b2HasCall = false;
          while ( !b2.getInstructions().empty() ) {
            b2HasCall =
              any_of(
                b2.getInstructions().front().get().begin(),
                b2.getInstructions().front().get().end(),
                [&]( const reference_wrapper<WIR_Operation> ref ) {
                  return(
                    ref.get().isCall() || ref.get().isIndirectCall() );} );

            b2.getInstructions().front().get().setDontOptimize( false );
            b1.moveInstruction( b2.getInstructions().front() );
          }

          // Because of their redundancy, both basic blocks are executed with
          // the same frequency. All occurences of b2 in Flow Restrictions can
          // thus be replaced by b1.
          WIR_FlowFactUpdater::replaceBasicBlock( b2, b1 );

          // There should not be any LoopBounds attached to an b2, (it cannot be
          // an entry to a loop) but just in case, they are deleted.
          WIR_FlowFactUpdater::eraseLoopBound( b2 );

          // Move containers from b2 to b1, except flow fact-related containers,
          // since flow facts are already handled above.
          for ( WIR_BaseContainer &c : b2.getContainers() )
            if ( c.getContainerType() != WIR_FlowFactRef::getContainerTypeID() )
              b1.insertContainer( c );
          b2.clearContainers();

          // Finally, remove now redundant basic block b2.
          DOUT(
            "Removing redundant basic block '" + b2.getName() + "'." << endl );

          // Update memory layout.
          auto bbPos = mBBPosition[ b2.getID() ];
          const auto &r = mSystem.findSymbol( b2 ).getSection().getRegion();
          mMemLayout[ r.getID() ].erase( bbPos );
          mBBPosition.erase( b2.getID() );

          f.eraseBasicBlock( it2 );
          redundantSuccessorRemoved = !b2HasCall;
          updateMemoryLayout( b1 );

          {
            DSTART(
              "virtual void WIR_JumpCorrection::runOptimization("
              "WIR_Function&).visualize" );
            DACTION( WIR_CFG{ f }.visualize( true ); );
          }
        }
      }
    } while ( redundantSuccessorRemoved );
  }
};


/*
  initializeMemoryLayout sets up the jump correction's internal data structures
  representing the system's memory layout.

  This method is virtual and thus can be overloaded if required for
  processor-specific initializations.
*/
void WIR_JumpCorrection::initializeMemoryLayout( void )
{
  DSTART( "void WIR_JumpCorrection::initializeMemoryLayout()" );

  // Step 1: Build a sequence of basic blocks ordered by the start addresses and
  //         collect the block's memory regions.

  // regionOfBB maps the ID of a basic block to its physical memory region.
  map<WIR_id_t, WIR_MemoryRegion *> regionOfBB;

  // sortedBBs stores all basic blocks of the WIR system, sorted ascending by
  // their start addresses in memory.
  multimap<pair<unsigned long long, unsigned long long>,
           WIR_BasicBlock *> sortedBBs;
  unsigned long long bbCnt = 0;

  for ( WIR_CompilationUnit &c : mSystem )
    for ( WIR_Function &f : c )
      for ( WIR_BasicBlock &b : f ) {
        auto &sym = mSystem.findSymbol( b );
        sortedBBs.insert(
          { { sym.getBaseAddress().getContent(), bbCnt++ }, &b } );

        auto &r = sym.getSection().getRegion();
        regionOfBB.insert( { b.getID(), &r } );
      }

  // Step 2: Put all basic blocks in their region's lists in mMemLayout in
  //         sorted order.

  mMemLayout.clear();
  mBBPosition.clear();

  for ( auto &[p, b] : sortedBBs ) {
    const WIR_MemoryRegion &r = *(regionOfBB.at( b->getID() ));

    mMemLayout[ r.getID() ].push_back( { *b, p.first } );
    DOUT(
      "Pushing back '" << b->getName() << "' with start address 0x" << hex <<
      p.first << dec << " in region '" << r.getName() << "'." << endl );

    mBBPosition[ b->getID() ] = std::prev( mMemLayout[ r.getID() ].end() );
  }

  // Step 3: Adjust the blocks' alignments according to the following rules:
  // A) Within each section:
  //    1) The very first basic block inside a section will not be aligned here,
  //       this case is handeled by rule B.2) below.
  //    2) Otherwise: The very first block of a function is aligned using
  //       getBlock().
  // B) Within each region:
  //    1) For a section with fixed start address, the start address of this
  //       section's very first basic block is fixed.
  //    2) When starting a completely fresh section inside a region, align the
  //       section's very first basic block using getAlignment().

  // nonEmptySections contains the IDs of all sections into which some basic
  // blocks have already been assembled. This is to support rule A.1) above.
  set<WIR_id_t> nonEmptySections;

  // firstBBOfSection contains a pointer to a section's very first basic block.
  // This is to support rules A.1) and B.1) above.
  map<WIR_id_t, WIR_BasicBlock *> firstBBOfSection;

  for ( WIR_CompilationUnit &c : mSystem )
    for ( WIR_Function &f : c )
      for ( WIR_BasicBlock &b : f ) {
        const auto &sym = mSystem.findSymbol( b );
        auto &sec = sym.getSection();

        if ( !firstBBOfSection.count( sec.getID() ) ) {
          // Rule A.1): b is the first block of its respective section.
          firstBBOfSection.insert( { sec.getID(), &b } );

          DOUT(
            "Rule A.1) applies for block '" << b.getName() <<
            "' and section '" << sec.getName() << "'." << endl );
        } else

        if ( b == f.begin()->get() ) {
          // Rule A.2): b is the first block of its respective function.
          MemLayoutInfo &memInfo = *(mBBPosition[ b.getID() ]);
          memInfo.alignment = sec.getBlock();

          DOUT(
            "Rule A.2) applies for block '" << b.getName() <<
            "' and section '" << sec.getName() << "': Setting alignment to " <<
            mBBPosition[ b.getID() ]->alignment << " bits (getBlock())." <<
            endl );
        }
      }

  // Process sections with fixed offsets first, i.e., sections with VMA != LMA.
  for ( const WIR_BaseProcessor &p :
          mSystem.getComponents<WIR_BaseProcessor>() )
    for ( const WIR_Section &sec : p )
      if ( firstBBOfSection.count( sec.getID() ) && sec.isStartSet() ) {
        // Rule B.1): The address of the very first basic block of a section
        //            with fixed start address is also fixed.
        const WIR_BasicBlock &b = *(firstBBOfSection.at( sec.getID() ));
        MemLayoutInfo &memInfo = *(mBBPosition[ b.getID() ]);
        memInfo.fixedAddress = true;

        DOUT(
          "Rule B.1) applies for block '" << b.getName() <<
          "' and section '" << sec.getName() <<
          "': Setting fixed address to " <<
          string(
            mBBPosition[ b.getID() ]->fixedAddress ? "true." : "false." ) <<
          endl );
      }

  for ( const WIR_MemoryRegion &r :
          mSystem.getComponents<WIR_MemoryRegion>() ) {
    for ( const WIR_Section &sec : r.getSections() )
      if ( firstBBOfSection.count( sec.getID() ) && !sec.isStartSet() ) {
        // Rule B.2): A completely fresh section is started inside a region.
        const WIR_BasicBlock &b = *(firstBBOfSection.at( sec.getID() ));
        MemLayoutInfo &memInfo = *(mBBPosition[ b.getID() ]);
        memInfo.alignment = sec.getAlignment();

        DOUT(
          "Rule B.2) applies for block '" << b.getName() <<
          "' and section '" << sec.getName() << "': Setting alignment to " <<
          mBBPosition[ b.getID() ]->alignment << " bits (getAlignment())." <<
          endl );
      }
  }

  {
    DSTART( "WIR_JumpCorrection.invariants" );
    DACTION(
      unsigned int layoutCounter = 0;
      for ( const auto &[id, l] : mMemLayout )
        for ( const auto &i : l ) {
          (void) i;
          ++layoutCounter;
          DEBUG_MEMORYLAYOUT_INVARIANT( (*(i.block)) );
        }

      unsigned int posCounter = 0;
      for ( const auto &[id, it] : mBBPosition ) {
        ++posCounter;
        ufAssert( id == it->block->getID() );
      }

      ufAssert( layoutCounter == posCounter );
    );
  }

  verifyMemoryLayout();
};


/*
  updateMemoryLayout incrementally updates the jump correction's internal memory
  layout data structures from the position of the specified basic block on in
  memory.

  This method is virtual and thus can be overloaded if required for
  processor-specific updates.
*/
void WIR_JumpCorrection::updateMemoryLayout( const WIR_BasicBlock &b )
{
  DSTART(
    "void WIR_JumpCorrection::updateMemoryLayout(const WIR_BasicBlock&)" );

  // Collect region and end address of the basic block where the updates shall
  // begin.
  auto memoryPos = mBBPosition.at( b.getID() );
  const WIR_MemoryRegion &r = mSystem.findSymbol( b ).getSection().getRegion();
  unsigned long long prevBBEnd = memoryPos->address + b.getSize();

  DEBUG_MEMORYLAYOUT_INVARIANT( b );

  // Propagate updates to all basic blocks allocated after b in memory.
  ++memoryPos;
  while ( memoryPos != mMemLayout[ r.getID() ].end() ) {
    MemLayoutInfo &memInfo = *memoryPos;

    DOUT(
      "Updating basic block '" << memInfo.block->getName() <<
      "' at original address 0x" << hex << memInfo.address << "." << endl );

    // If the current basic block is nailed to a fixed memory address, we can
    // stop here since no further changes need to be propagated.
    if ( memInfo.fixedAddress ) {
      DOUT(
        "Terminating update since block has a fixed memory address." << endl );
      DEBUG_MEMORYLAYOUT_INVARIANT( (*(memInfo.block)) );
      break;
    }

    // Otherwise, the new start address of the current basic block is the end
    // address of the previous one, under consideration of potential alignments.
    unsigned long long currentBBStart = prevBBEnd;
    DOUT( "End address of previous block: 0x" << hex << prevBBEnd << endl );

    // Check and correct alignment.
    if ( memInfo.alignment != 0 ) {
      unsigned long long alignment =
        (unsigned long long) 1 << memInfo.alignment;

      if ( currentBBStart & ( alignment - 1 ) ) {
        currentBBStart =
          ( ( currentBBStart + alignment - 1 ) & ~( alignment - 1 ) );
        DOUT(
          "Applying alignment by " << memInfo.alignment <<
          " bits, resulting start address is 0x" << hex << currentBBStart <<
          "." << endl );
      }
    }

    // Update memory layout if updates apply. Otherwise, simply stop if there
    // are no changes.
    if ( currentBBStart != memInfo.address ) {
      memInfo.address = currentBBStart;
      prevBBEnd = currentBBStart + memInfo.block->getSize();
      DOUT(
        "Setting new start address of block '" << memInfo.block->getName() <<
        "' to 0x" << hex << memInfo.address << "." << endl );
      DEBUG_MEMORYLAYOUT_INVARIANT( (*(memInfo.block)) );
      ++memoryPos;
    } else {
      DOUT(
        "Terminating update since block address does not change." << endl );
      DEBUG_MEMORYLAYOUT_INVARIANT( (*(memInfo.block)) );
      break;
    }
  }

  verifyMemoryLayout();
};


/*
  updateMemoryLayout incrementally updates the jump correction's internal memory
  layout data structures for the complete specified function.
*/
void WIR_JumpCorrection::updateMemoryLayout( const WIR_Function &f )
{
  DSTART( "void WIR_JumpCorrection::updateMemoryLayout(const WIR_Function&)" );

  for ( const WIR_BasicBlock &b : f )
    updateMemoryLayout( b );
};


/*
  verifyMemoryLayout verifies that the jump correction's internal memory layout
  is fully coherent with the overall system's memory layout.
*/
void WIR_JumpCorrection::verifyMemoryLayout( void ) const
{
  DSTART( "void WIR_JumpCorrection::verifyMemoryLayout() const" );

  DACTION(
    doMemoryLayoutVerification() );
};


/*
  computeDisplacement determines the displacement between the source basic block
  and the given symbol.
*/
WIR_disp_t WIR_JumpCorrection::computeDisplacement( const WIR_BasicBlock &src,
                                                    const WIR_Symbol &tgtSym,
                                                    const WIR_Operation &o ) const
{
  DSTART(
    "WIR_disp_t WIR_JumpCorrection::computeDisplacement(const WIR_BasicBlock&, "
    "const WIR_Symbol&, const WIR_Operation&) const" );

  const auto &tgt =
    tgtSym.getType() == WIR_SymbolType::block ?
      tgtSym.getBasicBlock() : tgtSym.getFunction().begin()->get();

  DOUT(
    "src basic block = " << src.getName() << endl <<
    "tgt = " << tgt.getName() << endl <<
    "getAddress( tgt ) = " << getAddress( tgt ) << endl <<
    "getAddress( src ) = " << getAddress( src ) << endl <<
    "src.getSize() = " << src.getSize() << endl <<
    "o.getSize() = " << o.getSize() << endl );

  // The displacement is the distance between jump target and the current
  // program counter, i.e., the address of jump operation o.
  // o's address is defined by the source basic block's base address plus the
  // block's byte size minus o's byte size itself.
  WIR_disp_t res =
    getAddress( tgt ) - ( getAddress( src ) + src.getSize() - o.getSize() );

  DOUT( "Displacement = " << dec << res << "." << endl );

  return( res );
};


/*
  isPhysicalSuccessor determines whether one basic blocks is a direct successor
  of another basic block in the address space.
*/
bool WIR_JumpCorrection::isPhysicalSuccessor( const WIR_BasicBlock &b1,
                                              const WIR_BasicBlock &b2 ) const
{
  DSTART(
    "bool WIR_JumpCorrection::isPhysicalSuccessor(const WIR_BasicBlock&, const "
    "WIR_BasicBlock&) const" );

  // Collect positions of both basic blocks in the memory layout.
  auto memoryPos1 = mBBPosition.at( b1.getID() );
  auto memoryPos2 = mBBPosition.at( b2.getID() );

  // Determine start addresses of both blocks.
  auto start1 = memoryPos1->address;
  auto start2 = memoryPos2->address;

  DOUT(
    "b1 = '" << b1.getName() << "' (start: 0x" << hex << start1 <<
    ", size: 0x" << b1.getSize() << ")" << endl <<
    "b2 = '" << b2.getName() << "' (start: 0x" << start2 << ")" << endl <<
    "  0x" << start2 << " == 0x" << start1 << " + 0x" << b1.getSize() << dec <<
    "? " << string( start2 == start1 + b1.getSize() ? "true" : "false" ) <<
    endl );

  return( start2 == start1 + b1.getSize() );
};


/*
  getAddress determines the start address of a basic block.
*/
unsigned long long WIR_JumpCorrection::getAddress( const WIR_BasicBlock &b ) const
{
  DSTART(
    "long long unsigned int WIR_JumpCorrection::getAddress(const "
    "WIR_BasicBlock&) const" );

  DEBUG_MEMORYLAYOUT_INVARIANT( b );

  return( mBBPosition.at( b.getID() )->address );
};


//
// Private class methods
//

/*
  doMemoryLayoutVerification performs the actual verification of the jump
  correction's internal memory layout.
*/
void WIR_JumpCorrection::doMemoryLayoutVerification( void ) const
{
  DSTART( "void WIR_JumpCorrection::doMemoryLayoutVerification() const" );

  // Step 1: Build a sequence of basic blocks ordered by the start addresses and
  //         collect the block's memory regions.
  map<WIR_id_t, WIR_MemoryRegion *> regionOfBB;

  // sortedBBs stores all basic blocks of the WIR system, sorted ascending by
  // their start addresses in memory.
  multimap<pair<unsigned long long, unsigned long long>,
           WIR_BasicBlock *> sortedBBs;

  map<WIR_id_t, std::list<MemLayoutInfo>::const_iterator> regionIterator;
  unsigned long long bbCnt = 0;

  for ( WIR_CompilationUnit &c : mSystem )
    for ( WIR_Function &f : c )
      for ( WIR_BasicBlock &b : f ) {
        auto &sym = mSystem.findSymbol( b );
        sortedBBs.insert(
          { { sym.getBaseAddress().getContent(), bbCnt++ }, &b } );

        auto &r = sym.getSection().getRegion();
        regionOfBB.insert( { b.getID(), &r } );
        regionIterator[ r.getID() ] = mMemLayout.at( r.getID() ).begin();
      }

  // Step 2: Verify all basic blocks in their region's lists in mMemLayout in
  //         sorted order.
  ufAssert( regionIterator.size() == mMemLayout.size() );

  for ( const auto &[p, b] : sortedBBs ) {
    const WIR_MemoryRegion &r = *(regionOfBB.at( b->getID() ));

    ufAssert( regionIterator[ r.getID() ] != mMemLayout.at( r.getID() ).end() );

    const auto &memInfo = *(regionIterator[ r.getID() ]);
    ufAssertT(
      memInfo.block == b,
      "Expected pointer to basic block '" << b->getName() << "' (ID " <<
      b->getID() << ", addr " << hex << b << "), found pointer to addr " <<
      memInfo.block << dec << "." );
    ufAssert( memInfo.address == p.first );

    ufAssert( mBBPosition.count( b->getID() ) );
    ufAssert( &memInfo == &(*(mBBPosition.at( b->getID() ))) );

    ++( regionIterator[ r.getID() ] );
  }

  for ( const auto &[id, it] : regionIterator )
    ufAssert( it == mMemLayout.at( id ).end() );

  ufAssert( mBBPosition.size() == sortedBBs.size() );
};

}       // namespace WIR
