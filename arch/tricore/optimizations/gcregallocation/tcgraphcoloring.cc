/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcgraphcoloring.cc
  @brief This file implements a TriCore-specific graph-coloring based register
         allocator.

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
#include <iterator>
#include <limits>
#include <map>
#include <sstream>
#include <stack>

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>
#include <libuseful/exceptions.h>
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/tricore/tc13.h>
#include <arch/tricore/analyses/bit/tcbitdfa.h>

// Include local headers
#include "tcgraphcoloring.h"


//
// Preprocessor macros
//

#define BBPOS( __i )                                                           \
  WIR_BasicBlock &__b = __i.getBasicBlock();                                   \
  unsigned int __bbPos = 1;                                                    \
  for ( auto it = __b.getInstructions().begin(); it->get() != __i;             \
        ++it, ++__bbPos ) ;

#define BBID                                                                   \
  "(" << __b.getName() << "/" << __bbPos << ")" << endl


//
// Code section
//

namespace WIR {


using namespace boost;
using namespace std;


//
// Public class methods
//

/*
  Default constructor for system-level optimization.
*/
TC_GraphColoring::TC_GraphColoring( WIR_System &s, bool verbosity, bool uc ) :
  WIR_Optimization { s },
  WIR_GraphColoring { s, verbosity },
  mStackAccessLatency { 0 },
  mUseOnlyUC { uc }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for compilation unit-level optimization.
*/
TC_GraphColoring::TC_GraphColoring( WIR_CompilationUnit &c, bool verbosity,
                                    bool uc ) :
  WIR_Optimization { c },
  WIR_GraphColoring { c, verbosity },
  mStackAccessLatency { 0 },
  mUseOnlyUC { uc }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for function-level optimization.
*/
TC_GraphColoring::TC_GraphColoring( WIR_Function &f, bool verbosity, bool uc ) :
  WIR_Optimization { f },
  WIR_GraphColoring { f, verbosity },
  mStackAccessLatency { 0 },
  mUseOnlyUC { uc }
{
  DSTART( "TC_GraphColoring::TC_GraphColoring(WIR_Function&, bool, bool)" );
};


/*
  Destructor.
*/
TC_GraphColoring::~TC_GraphColoring( void )
{
  DSTART( "virtual TC_GraphColoring::~TC_GraphColoring()" );
};


/*
  setUseOnlyUC (de-) activates whether the register allocator should only use
  physical registers from the TriCore's upper context, or not.
*/
void TC_GraphColoring::setUseOnlyUC( bool uc )
{
  DSTART( "void TC_GraphColoring::setUseOnlyUC(bool)" );

  mUseOnlyUC = uc;
};


/*
  getUseOnlyUC returns whether the register allocator should only use physical
  registers from the TriCore's upper context, or not.
*/
bool TC_GraphColoring::getUseOnlyUC( void ) const
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  return( mUseOnlyUC );
};


//
// Protected class methods
//

/*
  runOptimization allocates registers in the given function.
*/
void TC_GraphColoring::runOptimization( WIR_Function &f )
{
  DSTART( "virtual void TC_GraphColoring::runOptimization(WIR_Function&)" );

  TC_BitDFA analyzer { f };
  setDFA( analyzer );

  WIR_GraphColoring::runOptimization( f );
};


/*
  createPhregs sets up the lists mPhregs and mPhregsForPrecoloringOnly of all
  TriCore-specific physical registers contained in the specified WIR function.
*/
void TC_GraphColoring::createPhregs( WIR_Function &f )
{
  DSTART( "virtual void TC_GraphColoring::createPhregs(WIR_Function&)" );

  // Determine the involved processor core.
  const WIR_System &sys = f.getCompilationUnit().getSystem();
  const WIR_Section &sec = sys.findSymbol( f ).getSection();
  const WIR_BaseProcessor &p = sec.getProcessor();

  // It must be a TriCore, of course.
  ufAssert( p.getISAName().find( "TC1.3" ) == 0 );
  const TC13 &tc = dynamic_cast<const TC13 &>( p );

  // Determine memory access latency of the stack.
  ufAssert( tc.containsSection( ".stack" ) );
  const WIR_MemoryRegion &stackMem =
    tc.findSection( ".stack" )->get().getRegion();
  mStackAccessLatency = stackMem.getMaxDelay();

  // Determine the code access latencies of all basic blocks.
  mBBAccessLatency.clear();
  for ( const WIR_BasicBlock &b : f ) {
    const auto &region = sys.findSymbol( b ).getSection().getRegion();
    mBBAccessLatency[ b.getID() ] = region.getMaxDelay();
  }

  mPhregs = {
    // TriCore data registers subject to register allocation.
    const_cast<TC_DRegP &>( tc.D0() ), const_cast<TC_DRegP &>( tc.D1() ),
    const_cast<TC_DRegP &>( tc.D2() ), const_cast<TC_DRegP &>( tc.D3() ),
    const_cast<TC_DRegP &>( tc.D4() ), const_cast<TC_DRegP &>( tc.D5() ),
    const_cast<TC_DRegP &>( tc.D6() ), const_cast<TC_DRegP &>( tc.D7() ),
    const_cast<TC_DRegP &>( tc.D8() ), const_cast<TC_DRegP &>( tc.D9() ),
    const_cast<TC_DRegP &>( tc.D10() ), const_cast<TC_DRegP &>( tc.D11() ),
    const_cast<TC_DRegP &>( tc.D12() ), const_cast<TC_DRegP &>( tc.D13() ),
    const_cast<TC_DRegP &>( tc.D14() ), const_cast<TC_DRegP &>( tc.D15() ),

    // TriCore address registers subject to register allocation.
    // We do not include A10 and A11 here, since these are the stack and return
    // address pointers, resp. Furthermore, A0, A1, A8 and A9 are omitted, since
    // they are global registers that do neither belong to a function's upper
    // nor to its lower context (cf. TriCore EABI, section 2.2.1.4 "System
    // Global Registers": "Address registers A[0], A[1], A[8], and A[9] are
    // designated as system global registers. They are not part of either
    // context partition and are not saved/restored across calls. [...] By
    // convention, A[0] and A[1] are reserved for compiler use, while A[8] and
    // A[9] are reserved for OS or application use. A[0] is intended as a base
    // pointer to the "small" data section [...]. A[1] is intended as a base
    // pointer to the "literal data section"."
    const_cast<TC_ARegP &>( tc.A2() ), const_cast<TC_ARegP &>( tc.A3() ),
    const_cast<TC_ARegP &>( tc.A4() ), const_cast<TC_ARegP &>( tc.A5() ),
    const_cast<TC_ARegP &>( tc.A6() ), const_cast<TC_ARegP &>( tc.A7() ),
    const_cast<TC_ARegP &>( tc.A12() ), const_cast<TC_ARegP &>( tc.A13() ),
    const_cast<TC_ARegP &>( tc.A14() ), const_cast<TC_ARegP &>( tc.A15() ) };

  // TriCore registers subject to pre-coloring (e.g., if used in some snippet of
  // inline assembly) but NOT subject to register allocation. Here, we list all
  // address registers not included in mPhregs, see the comment on the TriCore
  // EABI above.
  mPhregsForPrecoloringOnly = {
    const_cast<TC_ARegP &>( tc.A0() ), const_cast<TC_ARegP &>( tc.A1() ),
    const_cast<TC_ARegP &>( tc.A8() ), const_cast<TC_ARegP &>( tc.A9() ),
    const_cast<TC_ARegP &>( tc.A10() ), const_cast<TC_ARegP &>( tc.A11() ) };

  // Initialize some TriCore-specific data structures.
  mOrderedAREGs = {
    const_cast<TC_ARegP &>( tc.A15() ), const_cast<TC_ARegP &>( tc.A14() ),
    const_cast<TC_ARegP &>( tc.A13() ), const_cast<TC_ARegP &>( tc.A12() ) };

  if ( !mUseOnlyUC ) {
    mOrderedAREGs.push_back( const_cast<TC_ARegP &>( tc.A3() ) );
    mOrderedAREGs.push_back( const_cast<TC_ARegP &>( tc.A7() ) );
    mOrderedAREGs.push_back( const_cast<TC_ARegP &>( tc.A6() ) );
    mOrderedAREGs.push_back( const_cast<TC_ARegP &>( tc.A5() ) );
    mOrderedAREGs.push_back( const_cast<TC_ARegP &>( tc.A2() ) );
    mOrderedAREGs.push_back( const_cast<TC_ARegP &>( tc.A4() ) );
  }

  mOrderedDREGs = {
    const_cast<TC_DRegP &>( tc.D15() ), const_cast<TC_DRegP &>( tc.D14() ),
    const_cast<TC_DRegP &>( tc.D13() ), const_cast<TC_DRegP &>( tc.D12() ),
    const_cast<TC_DRegP &>( tc.D11() ), const_cast<TC_DRegP &>( tc.D10() ),
    const_cast<TC_DRegP &>( tc.D9() ), const_cast<TC_DRegP &>( tc.D8() ) };

  if ( !mUseOnlyUC ) {
    mOrderedDREGs.push_back( const_cast<TC_DRegP &>( tc.D1() ) );
    mOrderedDREGs.push_back( const_cast<TC_DRegP &>( tc.D0() ) );
    mOrderedDREGs.push_back( const_cast<TC_DRegP &>( tc.D3() ) );
    mOrderedDREGs.push_back( const_cast<TC_DRegP &>( tc.D7() ) );
    mOrderedDREGs.push_back( const_cast<TC_DRegP &>( tc.D6() ) );
    mOrderedDREGs.push_back( const_cast<TC_DRegP &>( tc.D5() ) );
    mOrderedDREGs.push_back( const_cast<TC_DRegP &>( tc.D2() ) );
    mOrderedDREGs.push_back( const_cast<TC_DRegP &>( tc.D4() ) );
  }

  mOrderedEREGs = {
    const_cast<TC_ERegP &>( tc.E14() ), const_cast<TC_ERegP &>( tc.E12() ),
    const_cast<TC_ERegP &>( tc.E10() ), const_cast<TC_ERegP &>( tc.E8() ) };

  if ( !mUseOnlyUC ) {
    mOrderedEREGs.push_back( const_cast<TC_ERegP &>( tc.E6() ) );
    mOrderedEREGs.push_back( const_cast<TC_ERegP &>( tc.E4() ) );
    mOrderedEREGs.push_back( const_cast<TC_ERegP &>( tc.E2() ) );
    mOrderedEREGs.push_back( const_cast<TC_ERegP &>( tc.E0() ) );
  }

  mOrderedPREGs = {
    const_cast<TC_PRegP &>( tc.P14() ), const_cast<TC_PRegP &>( tc.P12() ) };

  if ( !mUseOnlyUC ) {
    mOrderedPREGs.push_back( const_cast<TC_PRegP &>( tc.P6() ) );
    mOrderedPREGs.push_back( const_cast<TC_PRegP &>( tc.P4() ) );
    mOrderedPREGs.push_back( const_cast<TC_PRegP &>( tc.P2() ) );
  }
};


/*
  initializationHook allows to perform processor-specific actions before doing
  some actual coloring or spilling.

  Here, initializationHook is used to perform the bit-true data flow analysis
  for TriCore before jumping into the actual graph coloring.
*/
void TC_GraphColoring::initializationHook( WIR_Function &f )
{
  DSTART( "virtual void TC_GraphColoring::initializationHook(WIR_Function&)" );

  (void) f;

  if ( getRematerialization() )
    mBitDFA->analyze();
};


/*
  isCallerSaved checks whether the specified physical register is caller-saved.

  For the TriCore architecture, the lower-context registers are caller-saved.
*/
bool TC_GraphColoring::isCallerSaved( const WIR_PhysicalRegister &r ) const
{
  DSTART(
    "virtual bool TC_GraphColoring::isCallerSaved(const WIR_PhysicalRegister&) "
    "const" );

  return( TC13::isLCReg( r ) );
};


/*
  checkCallerSavedRegsAliveAcrossCall checks whether the two specified virtual
  leaf registers are mapped to a caller-saved physical register and whether they
  are alive across a function call.

  If so, the respective function call and the physical register are stored in
  mLocalCallerSavedRegsAliveAcrossCall.
*/
void TC_GraphColoring::checkCallerSavedRegsAliveAcrossCall( const WIR_PhysicalRegister &phreg,
                                                            const WIR_RegisterSet &vregs_c1,
                                                            const WIR_RegisterSet &vregs_c2 )
{
  DSTART(
    "void TC_GraphColoring::checkCallerSavedRegsAliveAcrossCall(const "
    "WIR_PhysicalRegister&, const WIR_RegisterSet&, const WIR_RegisterSet&)" );

  if ( !isCallerSaved( phreg ) )
    return;

  // If a lower-context register was chosen, we have to check whether the
  // associated virtual register is live across some function call. If so, we
  // must make sure to save/restore the chosen lower-context register across
  // such a function call later.
  TC13 &tc = dynamic_cast<TC13 &>( phreg.getProcessor() );

  for ( auto &[ theCall, liveVregs ] : mVregsAliveAcrossCall ) {
    WIR_Operation &theCallOp = theCall.get().getOperations().front().get();

    // First, determine whether the current call instruction defines registers
    // D2, D3 or E2, i.e., whether the call returns some result via these
    // registers.
    bool callDefinesD2 = false;
    bool callDefinesD3 = false;
    bool callDefinesE2 = false;

    for ( const WIR_Parameter &p1 : theCallOp )
      if ( p1.getType() == WIR_ParameterType::reg ) {
        const auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p1 );
        const auto &r = rp.getRegister();

        if ( rp.isDefined() ) {
          if ( r == tc.D2() )
            callDefinesD2 = true;
          else

          if ( r == tc.D3() )
            callDefinesD3 = true;
          else

          if ( r == tc.E2() )
            callDefinesE2 = callDefinesD2 = callDefinesD3 = true;
        }
      }

    if ( callDefinesD2 && callDefinesD3 )
      callDefinesE2 = true;

    if ( vregs_c2.empty() ) {
      // We are dealing with a simple, non-hierarchical register or with a root
      // of a hierarchical register.
      for ( WIR_BaseRegister &r : vregs_c1 )
        if ( r.isVirtual() &&
             liveVregs.count( dynamic_cast<WIR_VirtualRegister &>( r ) ) ) {
          // The current virtual register is live across the call.
          mLocalCallerSavedRegsAliveAcrossCall[ theCall ].insert(
            const_cast<WIR_PhysicalRegister &>( phreg ) );

          DACTION(
            BBPOS( theCall.get() );
            DOUT(
              "Storing LC register " << r.getName() << "/" << phreg.getName() <<
              " to be caller-saved across call " << BBID << theCallOp <<
              endl ); );

          // If phreg is one of D2, D3 or E2, and if this phreg is also defined
          // by the current call, then we have to pay attention. In this
          // particular situation, D2/D3/E2 is alive across a function call, but
          // the call itself overwrites this phreg. Later on, in the post-
          // processing hook, dedicated store and load instructions are
          // generated before and after the call in order to save/restore this
          // phreg:
          //
          //   <some instruction defining, e.g., D2>
          //   ...
          //   st.w [SP] offset, D2;        # this store is missing here!
          //   call func;                   # function call overwrites D2!
          //
          // However, the save instruction storing D2/D3/E2 before the call is
          // not generated NOW. This means that the current call instruction
          // defines/overwrites D2/D3/E2, while a store instruction making use
          // of the original value of D2/D3/E2 and saving it on the stack is
          // still missing. This has the effect that liveness information
          // computed during subsequent iterations of the graph coloring
          // allocator is WRONG here. Liveness analysis only sees the DEF of
          // D2/D3/E2 by the call instruction, but a USE of these registers
          // immediately before the call is missing, which finally leads to
          // eventually missing edges in the interference graph.
          // In order to resolve this issue, we add a dummy implicit parameter
          // using D2/D3/E2 to the call instruction in order to model the
          // liveness of D2/D3/E2 correctly. Later on in the post-processing
          // hook, all these dummy USE parameters will be properly removed
          // again.
          if ( ( ( phreg == tc.D2() ) && callDefinesD2 ) ||
               ( ( phreg == tc.D3() ) && callDefinesD3 ) ||
               ( ( phreg == tc.E2() ) && callDefinesE2 ) )
            mLocalDummyParameters.push_back(
              { const_cast<WIR_PhysicalRegister &>( phreg ), theCall } );
        }
    } else {
      // We are dealing with a hierarchical register.
      for ( WIR_BaseRegister &r : vregs_c1 )
        if ( r.isVirtual() &&
             liveVregs.count( dynamic_cast<WIR_VirtualRegister &>( r ) ) ) {
          WIR_PhysicalRegister &child1 =
            const_cast<WIR_PhysicalRegister &>( phreg.begin()->get() );

          // The first child of the extended virtual register is live across the
          // call.
          mLocalCallerSavedRegsAliveAcrossCall[ theCall ].insert( child1 );

          DACTION(
            BBPOS( theCall.get() );
            DOUT(
              "Storing LC register " << r.getName() << "/" <<
              child1.getName() << " to be caller-saved across call " << BBID <<
              theCallOp << endl ); );

          // See lengthy comment above.
          if ( ( child1 == tc.D2() ) && callDefinesD2 )
            mLocalDummyParameters.push_back( { child1, theCall } );
        }

      for ( WIR_BaseRegister &r : vregs_c2 )
        if ( r.isVirtual() &&
             liveVregs.count( dynamic_cast<WIR_VirtualRegister &>( r ) ) ) {
          WIR_PhysicalRegister &child2 =
            const_cast<WIR_PhysicalRegister &>( phreg.rbegin()->get() );

          // The second child of the extended virtual register is live across
          // the call.
          mLocalCallerSavedRegsAliveAcrossCall[ theCall ].insert( child2 );

          DACTION(
            BBPOS( theCall.get() );
            DOUT(
              "Storing LC register " << r.getName() << "/" <<
              child2.getName() << " to be caller-saved across call " << BBID <<
              theCallOp << endl ); );

          // See lengthy comment above.
          if ( ( child2 == tc.D3() ) && callDefinesD3 )
            mLocalDummyParameters.push_back( { child2, theCall } );
        }
    }

    // Compact the register set stored in mLocalCallerSavedRegsAliveAcrossCall.
    WIR_RegisterSet &lcRegs = mLocalCallerSavedRegsAliveAcrossCall[ theCall ];

    if ( lcRegs.count( const_cast<TC_DRegP &>( tc.D0() ) ) &&
         lcRegs.count( const_cast<TC_DRegP &>( tc.D1() ) ) ) {
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D0() ) );
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D1() ) );
      lcRegs.insert( const_cast<TC_ERegP &>( tc.E0() ) );
    }

    if ( lcRegs.count( const_cast<TC_ERegP &>( tc.E0() ) ) &&
         lcRegs.count( const_cast<TC_DRegP &>( tc.D0() ) ) )
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D0() ) );
    if ( lcRegs.count( const_cast<TC_ERegP &>( tc.E0() ) ) &&
         lcRegs.count( const_cast<TC_DRegP &>( tc.D1() ) ) )
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D1() ) );

    if ( lcRegs.count( const_cast<TC_DRegP &>( tc.D2() ) ) &&
         lcRegs.count( const_cast<TC_DRegP &>( tc.D3() ) ) ) {
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D2() ) );
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D3() ) );
      lcRegs.insert( const_cast<TC_ERegP &>( tc.E2() ) );
    }

    if ( lcRegs.count( const_cast<TC_ERegP &>( tc.E2() ) ) &&
         lcRegs.count( const_cast<TC_DRegP &>( tc.D2() ) ) )
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D2() ) );
    if ( lcRegs.count( const_cast<TC_ERegP &>( tc.E2() ) ) &&
         lcRegs.count( const_cast<TC_DRegP &>( tc.D3() ) ) )
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D3() ) );

    if ( lcRegs.count( const_cast<TC_DRegP &>( tc.D4() ) ) &&
         lcRegs.count( const_cast<TC_DRegP &>( tc.D5() ) ) ) {
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D4() ) );
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D5() ) );
      lcRegs.insert( const_cast<TC_ERegP &>( tc.E4() ) );
    }

    if ( lcRegs.count( const_cast<TC_ERegP &>( tc.E4() ) ) &&
         lcRegs.count( const_cast<TC_DRegP &>( tc.D4() ) ) )
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D4() ) );
    if ( lcRegs.count( const_cast<TC_ERegP &>( tc.E4() ) ) &&
         lcRegs.count( const_cast<TC_DRegP &>( tc.D5() ) ) )
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D5() ) );

    if ( lcRegs.count( const_cast<TC_DRegP &>( tc.D6() ) ) &&
         lcRegs.count( const_cast<TC_DRegP &>( tc.D7() ) ) ) {
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D6() ) );
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D7() ) );
      lcRegs.insert( const_cast<TC_ERegP &>( tc.E6() ) );
    }

    if ( lcRegs.count( const_cast<TC_ERegP &>( tc.E6() ) ) &&
         lcRegs.count( const_cast<TC_DRegP &>( tc.D6() ) ) )
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D6() ) );
    if ( lcRegs.count( const_cast<TC_ERegP &>( tc.E6() ) ) &&
         lcRegs.count( const_cast<TC_DRegP &>( tc.D7() ) ) )
      lcRegs.erase( const_cast<TC_DRegP &>( tc.D7() ) );

    if ( lcRegs.count( const_cast<TC_ARegP &>( tc.A2() ) ) &&
         lcRegs.count( const_cast<TC_ARegP &>( tc.A3() ) ) ) {
      lcRegs.erase( const_cast<TC_ARegP &>( tc.A2() ) );
      lcRegs.erase( const_cast<TC_ARegP &>( tc.A3() ) );
      lcRegs.insert( const_cast<TC_PRegP &>( tc.P2() ) );
    }

    if ( lcRegs.count( const_cast<TC_PRegP &>( tc.P2() ) ) &&
         lcRegs.count( const_cast<TC_ARegP &>( tc.A2() ) ) )
      lcRegs.erase( const_cast<TC_ARegP &>( tc.A2() ) );
    if ( lcRegs.count( const_cast<TC_PRegP &>( tc.P2() ) ) &&
         lcRegs.count( const_cast<TC_ARegP &>( tc.A3() ) ) )
      lcRegs.erase( const_cast<TC_ARegP &>( tc.A3() ) );

    if ( lcRegs.count( const_cast<TC_ARegP &>( tc.A4() ) ) &&
         lcRegs.count( const_cast<TC_ARegP &>( tc.A5() ) ) ) {
      lcRegs.erase( const_cast<TC_ARegP &>( tc.A4() ) );
      lcRegs.erase( const_cast<TC_ARegP &>( tc.A5() ) );
      lcRegs.insert( const_cast<TC_PRegP &>( tc.P4() ) );
    }

    if ( lcRegs.count( const_cast<TC_PRegP &>( tc.P4() ) ) &&
         lcRegs.count( const_cast<TC_ARegP &>( tc.A4() ) ) )
      lcRegs.erase( const_cast<TC_ARegP &>( tc.A4() ) );
    if ( lcRegs.count( const_cast<TC_PRegP &>( tc.P4() ) ) &&
         lcRegs.count( const_cast<TC_ARegP &>( tc.A5() ) ) )
      lcRegs.erase( const_cast<TC_ARegP &>( tc.A5() ) );

    if ( lcRegs.count( const_cast<TC_ARegP &>( tc.A6() ) ) &&
         lcRegs.count( const_cast<TC_ARegP &>( tc.A7() ) ) ) {
      lcRegs.erase( const_cast<TC_ARegP &>( tc.A6() ) );
      lcRegs.erase( const_cast<TC_ARegP &>( tc.A7() ) );
      lcRegs.insert( const_cast<TC_PRegP &>( tc.P6() ) );
    }

    if ( lcRegs.count( const_cast<TC_PRegP &>( tc.P6() ) ) &&
         lcRegs.count( const_cast<TC_ARegP &>( tc.A6() ) ) )
      lcRegs.erase( const_cast<TC_ARegP &>( tc.A6() ) );
    if ( lcRegs.count( const_cast<TC_PRegP &>( tc.P6() ) ) &&
         lcRegs.count( const_cast<TC_ARegP &>( tc.A7() ) ) )
      lcRegs.erase( const_cast<TC_ARegP &>( tc.A7() ) );
  }
};


/*
  buildProcessorSpecificInterferences adds edges to the interference graph
  expressing TriCore-specific interferences.
*/
void TC_GraphColoring::buildProcessorSpecificInterferences( WIR_Function &f,
                                                            WIR_InterferenceGraph &igraph )
{
  DSTART(
    "virtual void TC_GraphColoring::buildProcessorSpecificInterferences("
    "WIR_Function&, WIR_InterferenceGraph&)" );

  // Update the interference graph first.
  for ( const WIR_VirtualRegister &r : f.getVirtualRegisters() )
    if ( !r.isChild() && igraph.containsNode( r ) && !isPrecolored( r ) )
      // We have found a virtual register which is either a simple,
      // non-hierarchical register or is the root of a complex register
      // hierarchy. Now, add interferences between a virtual DREG and all
      // physical AREGs, and vice versa.
      for ( const WIR_VirtualRegister &l : r.getLeafs() )
        for ( const WIR_PhysicalRegister &preg : mPhregs )
          if ( !igraph.interfere( l, preg ) &&
               !igraph.areSameNodes( l, preg ) &&
               ( ( ( l.getType() == TC13::RegisterType::dReg ) &&
                   ( preg.getType() == TC13::RegisterType::aReg ) ) ||
                 ( ( l.getType() == TC13::RegisterType::aReg ) &&
                   ( preg.getType() == TC13::RegisterType::dReg ) ) ) ) {
            DOUT(
              "Adding interference " << l.getName() << " <-> " <<
              preg.getName() << endl );
            igraph.addInterference( l, preg, 1 );
          }

  // Next, we check for LOOP instructions and mark all address registers used by
  // them as high-priority registers.
  for ( const WIR_BasicBlock &b : f )
    for ( const WIR_Instruction &i : b )
      if ( i.getOperations().front().get().getOpCode() == TC13::OpCode::LOOP ) {
        const WIR_Parameter &p =
          i.getOperations().front().get().getParameters().front().get();
        const auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p );
        mHighPriorityRegs.insert( rp.getRegister().getID() );
      }
};


/*
  isFunctionReturnMove checks whether a given move operation stores the result
  of a function call somewhere.
*/
bool TC_GraphColoring::isFunctionReturnMove( const WIR_Operation &o ) const
{
  DSTART(
    "virtual bool TC_GraphColoring::isFunctionReturnMove(const WIR_Operation&) "
    "const" );

  if ( !o.isMove() )
    return( false );

  const WIR_BaseRegister &useReg = getUseOfMove( o );

  // First, verify that the register used by the move is D2, D3 or E2.
  bool isD2D3E2 = false;

  if ( useReg.isPhysical() &&
       ( ( useReg.getName() == "D2" ) || ( useReg.getName() == "D3" ) ||
         ( useReg.getName() == "E2" ) ) )
    isD2D3E2 = true;

  if ( useReg.isVirtual() && isPrecolored( useReg ) ) {
    const WIR_PhysicalRegister &preg =
      mPrecolored.at( const_cast<WIR_BaseRegister &>( useReg ) ).get();

    if ( ( preg.getName() == "D2" ) || ( preg.getName() == "D3" ) ||
         ( preg.getName() == "E2" ) )
      isD2D3E2 = true;
  }

  if ( !isD2D3E2 )
    return( false );

  // Next, verify that the predecessor basic blocks all end with a call that
  // define useReg.
  WIR_Instruction &i = o.getInstruction();
  WIR_BasicBlock &b = i.getBasicBlock();

  for ( const WIR_BasicBlock &pred : b.getPredecessors() ) {
    const WIR_Instruction &lastIns = *(pred.getInstructions().rbegin());

    if ( lastIns.getOperations().front().get().isCall() ||
         lastIns.getOperations().front().get().isIndirectCall() ) {
      bool rIsDefinedByCall = false;

      for ( const WIR_Parameter &p :
              lastIns.getOperations().front().get().getParameters() )
        if ( p.getType() == WIR_ParameterType::reg ) {
          const auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p );

          if ( ( rp.isDefined() || rp.isDefUsed() ) &&
               ( ( rp.getRegister() == useReg ) ||
                 useReg.isChildOf( rp.getRegister() ) ||
                 rp.getRegister().isChildOf( useReg ) ) ) {
            rIsDefinedByCall = true;
            break;
          }
        }

      if ( !rIsDefinedByCall )
        return( false );
    } else
      return( false );
  }

  // Finally, verify that o is either the very first instruction within b, or
  // that o is the second instruction AND the first one is also a
  // function-return move.
  auto it = b.getInstructions().begin();
  if ( ( i == it->get() ) ||
       ( ( i == (++it)->get() ) &&
         ( isFunctionReturnMove(
             b.getInstructions().front().get().getOperations().front().get() ) ) ) )
    return( true );

  return( false );
};


/*
  isPriorityRegister returns whether a given register has high priority for
  color assignment.

  For the TriCore register allocator, high-priority registers are kept in set
  mHighPriorityRegs. Thus, this method only checks whether r is in
  mHighPriorityRegs or not.
*/
bool TC_GraphColoring::isPriorityRegister( const WIR_VirtualRegister &r ) const
{
  DSTART(
    "virtual bool TC_GraphColoring::isPriorityRegister(const WIR_VirtualRegister&) const" );

  return( mHighPriorityRegs.count( r.getID() ) );
};


/*
  getSpillLoadCosts returns the TriCore-specific costs of one single spill-load
  for the specified register parameter.
*/
unsigned int TC_GraphColoring::getSpillLoadCosts( const WIR_RegisterParameter &p ) const
{
  DSTART(
    "virtual unsigned int TC_GraphColoring::getSpillLoadCosts(const WIR_RegisterParameter&) const" );

  auto &b = p.getOperation().getInstruction().getBasicBlock();
  auto blockLatency = mBBAccessLatency.at( b.getID() );

  auto &r = dynamic_cast<WIR_VirtualRegister &>( p.getRegister() );

  if ( r.getType() == TC13::RegisterType::aReg )
    return( mStackAccessLatency + blockLatency );
  else

  if ( r.getType() == TC13::RegisterType::dReg )
    return( mStackAccessLatency + blockLatency );
  else

  if ( r.getType() == TC13::RegisterType::eReg )
    return( ( 2 * mStackAccessLatency ) + blockLatency );
  else

  if ( r.getType() == TC13::RegisterType::pReg )
    return( ( 2 * mStackAccessLatency ) + blockLatency );

  throw( ufFatalError( "This should never happen...", false ) );
};


/*
  getSpillStoreCosts returns the TriCore-specific costs of one single
  spill-store for the specified register parameter.
*/
unsigned int TC_GraphColoring::getSpillStoreCosts( const WIR_RegisterParameter &p ) const
{
  DSTART(
    "virtual unsigned int TC_GraphColoring::getSpillStoreCosts(const WIR_RegisterParameter&) const" );

  return( getSpillLoadCosts( p ) );
};


/*
  getMoveCosts returns the TriCore-specific costs of one single move operation
  that can be omitted due to spilling.
*/
unsigned int TC_GraphColoring::getMoveCosts( const WIR_Operation &o ) const
{
  DSTART(
    "virtual unsigned int TC_GraphColoring::getMoveCosts(const WIR_Operation&) const" );

  const auto &b = o.getInstruction().getBasicBlock();

  if ( ( o.getOpCode() == TC13::OpCode::MOV_RR ) ||
       ( o.getOpCode() == TC13::OpCode::MOV_AA ) )
    return( mBBAccessLatency.at( b.getID() ) );

  throw( ufFatalError( "This should never happen...", false ) );
};


/*
  getUseOfMove returns the used register of the specified TriCore WIR move
  operation.
*/
WIR_BaseRegister &TC_GraphColoring::getUseOfMove( const WIR_Operation &o ) const
{
  DSTART(
    "virtual WIR_BaseRegister& TC_GraphColoring::getUseOfMove(const WIR_Operation&) const" );

  return( dynamic_cast<WIR_RegisterParameter &>(
    o.getExplicitParameter( 2 ) ).getRegister() );
};


/*
  getDefOfMove returns the defined register of the specified TriCore WIR move
  operation.
*/
WIR_BaseRegister &TC_GraphColoring::getDefOfMove( const WIR_Operation &o ) const
{
  DSTART(
    "virtual WIR_BaseRegister& TC_GraphColoring::getDefOfMove(const WIR_Operation&) const" );

  return(
    dynamic_cast<WIR_RegisterParameter &>(
      o.getExplicitParameter( 1 ) ).getRegister() );
};


/*
  getRematerializationCosts returns the TriCore-specific costs of one single
  recomputation of the specified used parameter.
*/
unsigned int TC_GraphColoring::getRematerializationCosts( const WIR_RegisterParameter &p ) const
{
  DSTART(
    "virtual unsigned int TC_GraphColoring::getRematerializationCosts(const WIR_RegisterParameter&) const" );

  const auto &b = p.getOperation().getInstruction().getBasicBlock();
  auto blockLatency = mBBAccessLatency.at( b.getID() );

  // Check whether the value carried along p's register is a constant.
  int constValue = getRematerializationConstant( p );

  if ( constValue != numeric_limits<int>::max() ) {
    // It now remains to determine how many TriCore machine instructions it
    // takes to recompute this constant.
    TC_ERegV e;
    bool eReg = p.getRegister().getType() == e.getType();

    DDECLARE( ostringstream __sstr );
    DACTION(
      __sstr << "Rematerialization cost of " << p.getRegister().getName()
             << " (const = " << constValue << ") in " << p.getOperation()
             << ": "; );

    if ( ( constValue >= TC_Const16_Signed::getMinValue( 16 ) ) &&
         ( constValue <= TC_Const16_Signed::getMaxValue( 16 ) ) ) {
      // In this case, only a simple MOV instruction is sufficient.
      DOUT(
        __sstr.str() << ( eReg ? 2 * blockLatency : blockLatency ) << endl );
      return( eReg ? 2 * blockLatency : blockLatency );
    }

    if ( ( constValue >= 0 ) &&
         ( (unsigned int) constValue <=
             TC_Const16_Unsigned::getMaxValue( 16 ) ) ) {
      // In this case, one simple MOV.U instruction is sufficient.
      DOUT(
        __sstr.str() << ( eReg ? 2 * blockLatency : blockLatency ) << endl );
      return( eReg ? 2 * blockLatency : blockLatency );
    }

    // Extract lowest 16 bits from constant, use 2's-complement representation.
    int low = constValue & 0x0000FFFF;

    if ( low > TC_Const16_Signed::getMaxValue( 16 ) )
      low =
        TC_Const16_Signed::getMinValue( 16 ) +
        ( low - TC_Const16_Signed::getMaxValue( 16 ) ) - 1;

    if ( low != 0 ) {
      // We need a MOVH and ADDI combination.
      DOUT(
        __sstr.str() << ( eReg ? 3 * blockLatency : 2 * blockLatency ) <<
        endl );
      return( eReg ? 3 * blockLatency : 2 * blockLatency );
    } else {
      // We only need a MOVH.
      DOUT(
        __sstr.str() << ( eReg ? 2 * blockLatency : blockLatency ) << endl );
      return( eReg ? 2 * blockLatency : blockLatency );
    }
  }

  return( numeric_limits<unsigned int>::max() );
};


/*
  getRematerializationConstant returns a constant integer value that is
  equivalent to the specified WIR parameter.
*/
int TC_GraphColoring::getRematerializationConstant( const WIR_RegisterParameter &p ) const
{
  DSTART(
    "int TC_GraphColoring::getRematerializationConstant(const WIR_RegisterParameter&) const" );

  int res = numeric_limits<int>::max();

  // If p refers to an AREG or an APREG, stop here. Rematerialization currently
  // only supports constants in DREGs or EREGs.
  if ( ( p.getRegister().getType() == TC13::RegisterType::aReg ) ||
       ( p.getRegister().getType() == TC13::RegisterType::pReg ) )
    return( res );

  // Combine incoming up values of p.
  map<WIR_id_t, WIR_UpDownValue> inValue;
  combineInValues( p, inValue );

  auto it = inValue.find( p.getID() );
  if ( it == inValue.end() )
    return( res );

  auto &combinedInValue = it->second;
  if ( !combinedInValue.isInteger() )
    return( res );

  // Replace all don't care bits of combinedInValue by 0.
  for ( unsigned int i = 0; i < combinedInValue.getBitWidth(); ++i )
    if ( combinedInValue.at( i ) == WIR_L4::bX )
      combinedInValue.setBit( i, WIR_L4::b0 );

  // At this point, we know that the value carried along p's register is a
  // constant.
  res = combinedInValue.getSignedValue();

  DACTION(
    stringstream sstr;
    sstr << wir << p.getOperation();

    DOUT(
      "Rematerialization constant of parameter '" << p << "' in operation '" <<
      sstr.str().substr( 8 ) << "' = " << res << endl ); );

  return( res );
};


/*
  getRematerializationInstructions returns a list of TriCore instructions for
  one single recomputation of the specified used parameter.
*/
std::list<WIR_Instruction *> TC_GraphColoring::getRematerializationInstructions( const WIR_RegisterParameter &p ) const
{
  DSTART(
    "virtual list<WIR_Instruction*> TC_GraphColoring::getRematerializationInstructions(const WIR_RegisterParameter&) const" );

  list<WIR_Instruction *> res;
  WIR_VirtualRegister &r =
    dynamic_cast<WIR_VirtualRegister &>( p.getRegister() );

  // Check whether the value carried along p's register is a constant.
  int constValue = getRematerializationConstant( p );

  if ( constValue != numeric_limits<int>::max() ) {
    // It now remains to list the TriCore machine instructions it takes to
    // recompute this constant.
    TC_ERegV e;
    bool eReg = p.getRegister().getType() == e.getType();
    TC_DRegV d;
    bool dReg = p.getRegister().getType() == d.getType();

    if ( ( constValue >= TC_Const16_Signed::getMinValue( 16 ) ) &&
         ( constValue <= TC_Const16_Signed::getMaxValue( 16 ) ) ) {
      // In this case, only a simple MOV instruction is sufficient.
      res.push_back( new WIR_Instruction(
        { TC13::OpCode::MOV, TC13::OperationFormat::DC16_1,
          WIR_RegisterParameter(
            ( dReg ? r : r.begin()->get() ), WIR_Usage::def ),
          TC_Const16_Signed( constValue ) } ) );

      DOUT(
        "For parameter " << p.getID() << "/" << r.getName() <<
        ", generated rematerialization instruction" << *(res.back()) << endl );

      if ( eReg ) {
        res.push_back( new WIR_Instruction(
          { TC13::OpCode::MOV, TC13::OperationFormat::DC16_1,
            WIR_RegisterParameter( r.rbegin()->get(), WIR_Usage::def ),
            TC_Const16_Signed( 0 ) } ) );

        DOUT(
          "For parameter " << p.getID() << "/" << r.getName() <<
          ", generated rematerialization instruction" << *(res.back()) <<
          endl );
      }
    } else

    if ( ( constValue >= 0 ) &&
         ( (unsigned int) constValue <=
             TC_Const16_Unsigned::getMaxValue( 16 ) ) ) {
      // In this case, one simple MOV.U instruction is sufficient.
      res.push_back( new WIR_Instruction(
        { TC13::OpCode::MOV_U, TC13::OperationFormat::DC16_2,
          WIR_RegisterParameter(
            ( dReg ? r : r.begin()->get() ), WIR_Usage::def ),
          TC_Const16_Unsigned( constValue ) } ) );

      DOUT(
        "For parameter " << p.getID() << "/" << r.getName() <<
        ", generated rematerialization instruction" << *(res.back()) << endl );

      if ( eReg ) {
        res.push_back( new WIR_Instruction(
          { TC13::OpCode::MOV, TC13::OperationFormat::DC16_1,
            WIR_RegisterParameter( r.rbegin()->get(), WIR_Usage::def ),
            TC_Const16_Signed( 0 ) } ) );

        DOUT(
          "For parameter " << p.getID() << "/" << r.getName() <<
          ", generated rematerialization instruction" << *(res.back()) <<
          endl );
      }
    } else {
      int low, high;

      // Extract lowest 16 bits from constant, use 2's-complement
      // representation.
      low = constValue & 0x0000FFFF;

      if ( low > TC_Const16_Signed::getMaxValue( 16 ) )
        low =
          TC_Const16_Signed::getMinValue( 16 ) +
          ( low - TC_Const16_Signed::getMaxValue( 16 ) ) - 1;

      // Extract upper part of constant.
      high = constValue - low;

      // We don't use '>> 16' here, since C/C++ does not specify whether >>
      // performs shifting with or without sign extension.
      for ( int i = 0; i < 16; ++i )
        high /= 2;

      if ( high < 0 )
        high += TC_Const16_Unsigned::getMaxValue( 16 ) + 1;

      // Generate MOVH instruction.
      res.push_back( new WIR_Instruction(
        { TC13::OpCode::MOVH, TC13::OperationFormat::DC16_2,
          WIR_RegisterParameter(
            ( dReg ? r : r.begin()->get() ), WIR_Usage::def ),
          TC_Const16_Unsigned( high ) } ) );

      DOUT(
        "For parameter " << p.getID() << "/" << r.getName() <<
        ", generated rematerialization instruction" << *(res.back()) << endl );

      // Generate ADDI instruction.
      if ( low != 0 ) {
        res.push_back( new WIR_Instruction(
          { TC13::OpCode::ADDI, TC13::OperationFormat::DDC16_1,
            WIR_RegisterParameter(
              ( dReg ? r : r.begin()->get() ), WIR_Usage::def ),
            WIR_RegisterParameter(
              ( dReg ? r : r.begin()->get() ), WIR_Usage::use ),
            TC_Const16_Unsigned( low ) } ) );

        DOUT(
          "For parameter " << p.getID() << "/" << r.getName() <<
          ", generated rematerialization instruction" << *(res.back()) <<
          endl );
      }

      if ( eReg ) {
        res.push_back( new WIR_Instruction(
          { TC13::OpCode::MOV, TC13::OperationFormat::DC16_1,
            WIR_RegisterParameter( r.rbegin()->get(), WIR_Usage::def ),
            TC_Const16_Unsigned( 0 ) } ) );

        DOUT(
          "For parameter " << p.getID() << "/" << r.getName() <<
          ", generated rematerialization instruction" << *(res.back()) <<
          endl );
      }
    }
  }

  return( res );
};


/*
  selectColors assigns actual colors to the TriCore leaf registers in the
  specified vector.

  This method must not yet assign colors to the interference graph - this is
  done elsewhere. In order to determine feasible colors for the leaf registers,
  this method should make use of WIR_InterferenceGraph::getPossibleColors(). It
  must be ensured that the returned map is either empty or contains exactly one
  entry per leaf. It must hold that none of the colors used in this returned map
  is already used for adjacent interference graph nodes.

  This method checks whether the leafs to be processed are TriCore data or
  address registers, and whether they are simple or extended registers. It first
  tries to select the implicit registers D15 and A15, since this is beneficial
  for the generation of 16-bit instructions. If D15/A15 are not available, other
  data/address registers from the TriCore's upper context are checked next. This
  is done, because upper context registers are automatically saved across
  function calls without any additional context saving code. If the entire upper
  context is not available, the registers of the lower context are finally
  checked.
*/
WIR_GraphColoring::WIR_ColorMap TC_GraphColoring::selectColors( const std::vector<std::reference_wrapper<WIR_VirtualRegister>> &leafs,
                                                                const WIR_InterferenceGraph &igraph )
{
  DSTART(
    "virtual WIR_GraphColoring::WIR_ColorMap TC_GraphColoring::selectColors("
    "const vector<reference_wrapper<WIR_VirtualRegister>>&, const "
    "WIR_InterferenceGraph&)" );

  WIR_GraphColoring::WIR_ColorMap res;

  ufAssert( ( leafs.size() == 1 ) || ( leafs.size() == 2 ) );
  set<unsigned int> possibleColors = igraph.getPossibleColors( leafs.front() );

  WIR_VirtualRegister &c1 = leafs.front().get();
  WIR_VirtualRegister &c2 = leafs.back().get();

  vector<std::reference_wrapper<WIR_PhysicalRegister>> *orderedPhregs = nullptr;

  // Determine whether the VREG to be colored is alive across function calls.
  DACTION(
    bool vregIsAliveAcrossCall = false;
    for ( auto &[ call, regs ] : mVregsAliveAcrossCall )
      if ( regs.count( c1 ) || ( ( c2 != c1 ) && regs.count( c2 ) ) ) {
        vregIsAliveAcrossCall = true;
        break;
      }

    DOUT(
      "c1 = " << c1.getName() << "\t\tc2 = " <<
      ( c2 != c1 ? c2.getName() : "NULL" ) << "\tis " <<
      ( vregIsAliveAcrossCall ? "" : "not " ) << "alive across call." <<
      endl ); );

  if ( leafs.size() == 1 ) {
    // A simple non-hierarchical register has to be colored. We distinguish
    // between DREGs and AREGs.
    orderedPhregs =
      ( c1.getType() == TC13::RegisterType::dReg ) ?
        &mOrderedDREGs : &mOrderedAREGs;

    // Now, check the phregs in the specified precedence order.
    for ( const WIR_PhysicalRegister &phreg : *orderedPhregs ) {
      unsigned int color = igraph.getColorOfPhreg( phreg );

      if ( possibleColors.count( color ) ) {
        res[ c1 ] = color;

        WIR_RegisterSet aliases_c1 = igraph.getCoalescedAliases( c1 );
        aliases_c1.insert( c1 );
        WIR_RegisterSet aliases_c2;

        checkCallerSavedRegsAliveAcrossCall( phreg, aliases_c1, aliases_c2 );

        break;
      }
    }
  } else {
    // An extended hierarchical register has to be colored. We again distinguish
    // between DREGs and AREGs.
    ufAssert(
      ( ( c1.getType() == TC13::RegisterType::dReg ) &&
        ( c2.getType() == TC13::RegisterType::dReg ) ) ||
      ( ( c1.getType() == TC13::RegisterType::aReg ) &&
        ( c2.getType() == TC13::RegisterType::aReg ) ) );

    orderedPhregs =
      ( c1.getType() == TC13::RegisterType::dReg ) ?
        &mOrderedEREGs : &mOrderedPREGs;

    // Now, check the phregs in the specified precedence order.
    for ( const WIR_PhysicalRegister &phreg : *orderedPhregs ) {
      unsigned int color1 = igraph.getColorOfPhreg( phreg.begin()->get() );
      unsigned int color2 = igraph.getColorOfPhreg( phreg.rbegin()->get() );

      if ( possibleColors.count( color1 ) && possibleColors.count( color2 ) ) {
        res[ c1 ] = color1;
        res[ c2 ] = color2;

        WIR_RegisterSet aliases_root =
          igraph.getCoalescedAliases( c1.getRoot() );
        aliases_root.insert( c1.getRoot() );
        WIR_RegisterSet emptySet;

        checkCallerSavedRegsAliveAcrossCall( phreg, aliases_root, emptySet );

        WIR_RegisterSet aliases_c1 = igraph.getCoalescedAliases( c1 );
        aliases_c1.insert( c1 );
        WIR_RegisterSet aliases_c2 = igraph.getCoalescedAliases( c2 );
        aliases_c2.insert( c2 );

        checkCallerSavedRegsAliveAcrossCall( phreg, aliases_c1, aliases_c2 );

        break;
      }
    }
  }

  return( res );
};


/*
  resolveSpillCoalescingConflict resolves a conflict when two registers with
  different positions in a register hierarchy shall be coalesced during
  computeStackLocations.
*/
std::pair<std::reference_wrapper<WIR_VirtualRegister>,
          std::reference_wrapper<WIR_VirtualRegister>> TC_GraphColoring::resolveSpillCoalescingConflict( const WIR_VirtualRegister &r1,
                                                                                                         const WIR_VirtualRegister &r2 ) const
{
  DSTART(
    "virtual pair<reference_wrapper<WIR_VirtualRegister>, reference_wrapper<WIR_VirtualRegister> > TC_GraphColoring::resolveSpillCoalescingConflict(const WIR_VirtualRegister&, const WIR_VirtualRegister&) const" );

  WIR_VirtualRegister &reg1 = const_cast<WIR_VirtualRegister &>( r1 );
  WIR_VirtualRegister &reg2 = const_cast<WIR_VirtualRegister &>( r2 );

  unsigned int leafs1 = r1.getLeafs().size();
  unsigned int leafs2 = r2.getLeafs().size();

  if ( leafs1 != leafs2 ) {
    WIR_VirtualRegister *large = ( leafs1 > leafs2 ? &reg1 : &reg2 );
    WIR_VirtualRegister *small = ( *large == reg1 ? &reg2 : &reg1 );

    return(
      pair<std::reference_wrapper<WIR_VirtualRegister>,
           std::reference_wrapper<WIR_VirtualRegister>>(
        large->begin()->get(), *small ) );
  } else
    return(
      pair<std::reference_wrapper<WIR_VirtualRegister>,
           std::reference_wrapper<WIR_VirtualRegister>>( reg1, reg2 ) );
};


/*
  getStackPosOfSubReg returns the stack position of some child register, if the
  root of the entire register hierarchy is located in the specified stack
  position.
*/
unsigned int TC_GraphColoring::getStackPosOfSubReg( const WIR_VirtualRegister &r,
                                                    unsigned int rootPos ) const
{
  DSTART(
    "virtual unsigned int TC_GraphColoring::getStackPosOfSubReg(const WIR_VirtualRegister&, unsigned int) const" );

  if ( !r.isChild() || ( r == r.getRoot().begin()->get() ) )
    return( rootPos );
  else
    return( rootPos + 4 );
};


/*
  insertSpillLoad inserts TriCore code for a spill-load of a register into the
  WIR.

  insertSpillLoad is responsible to add all generated spill-load instructions to
  map mSpillLoads.
*/
void TC_GraphColoring::insertSpillLoad( const WIR_BaseRegister &clone,
                                        const WIR_BaseRegister &r,
                                        int stackPos, WIR_BasicBlock &b,
                                        std::list<std::reference_wrapper<WIR_Instruction>>::const_iterator pos )
{
  DSTART(
    "virtual void TC_GraphColoring::insertSpillLoad(const WIR_BaseRegister&, const WIR_BaseRegister&, int, WIR_BasicBlock&, list<reference_wrapper<WIR_Instruction> >::const_iterator)"  );

  // Determine the size of the function's overflow area. According to the
  // TriCore EABI (section 2.2.2.1), the argument overflow area for outgoing
  // arguments must be located at the bottom (low address end) of the frame,
  // with the first overflow argument at zero offset from the stack pointer. We
  // must not overwrite the overflow area by spilled registers so that we
  // increase the stack positions of spills by the size of the overflow area.
  int overflowAreaSize = b.getFunction().getFrameSize();

  DACTION(
    DOUT( "Inserting spill-load of register '" << clone.getName() << "'" );
    if ( r != clone )
      DOUT(  "/'" << r.getName() << "'" );
    DOUT(
      " from stack offset " << stackPos + overflowAreaSize << " (stackPos = " <<
      stackPos << ", overflowAreaSize = " << overflowAreaSize << ") " );

    if ( pos != b.getInstructions().end() ) {
      BBPOS( pos->get() );
      DOUT( "before position " << tricore << BBID << pos->get() << endl );
    } else
      DOUT( "at end of block '" << b.getName() << "'." << endl ); );

  DDECLARE(
    // Save the current number of spill instructions in mInsertedSpillCode.
    const size_t oldPos = mInsertedSpillCode.size(); );

  const WIR_BasicBlock &newBB =
    insertSpillCode( clone, r, stackPos + overflowAreaSize, b, pos, false );

  DACTION(
    auto it = mInsertedSpillCode.begin();
    for ( std::advance( it, oldPos ); it != mInsertedSpillCode.end(); ++it )
      cout << tricore << it->get(); );

  if ( newBB != b ) {
    // TODO: Add update of back-annotation mapping!
  }
};


/*
  insertSpillStore inserts TriCore code for a spill-store of a register into the
  WIR.

  insertSpillStore is responsible to add all generated spill-store instructions
  to map mSpillStores.
*/
void TC_GraphColoring::insertSpillStore( const WIR_BaseRegister &clone,
                                         const WIR_BaseRegister &r,
                                         int stackPos, WIR_BasicBlock &b,
                                         std::list<std::reference_wrapper<WIR_Instruction>>::const_iterator pos )
{
  DSTART(
    "virtual void TC_GraphColoring::insertSpillStore(const WIR_BaseRegister&, const WIR_BaseRegister&, int, WIR_BasicBlock&, list<reference_wrapper<WIR_Instruction> >::const_iterator)" );

  // Determine the size of the function's overflow area. According to the
  // TriCore EABI (section 2.2.2.1), the argument overflow area for outgoing
  // arguments must be located at the bottom (low address end) of the frame,
  // with the first overflow argument at zero offset from the stack pointer. We
  // must not overwrite the overflow area by spilled registers so that we
  // increase the stack positions of spills by the size of the overflow area.
  int overflowAreaSize = b.getFunction().getFrameSize();

  DACTION(
    DOUT( "Inserting spill-store of register '" << clone.getName() << "'" );
    if ( r != clone )
      DOUT(  "/'" << r.getName() << "'" );
    DOUT(
      " from stack offset " << stackPos + overflowAreaSize << " (stackPos = " <<
      stackPos << ", overflowAreaSize = " << overflowAreaSize << ") " );

    if ( pos != b.getInstructions().end() ) {
      BBPOS( pos->get() );
      DOUT( "before position " << tricore << BBID << pos->get() << endl );
    } else
      DOUT( "at end of block '" << b.getName() << "'." << endl ); );

  DDECLARE(
    // Save the current number of spill instructions in mInsertedSpillCode.
    const size_t oldPos = mInsertedSpillCode.size(); );

  const WIR_BasicBlock &newBB =
    insertSpillCode( clone, r, stackPos + overflowAreaSize, b, pos, true );

  DACTION(
    auto it = mInsertedSpillCode.begin();
    for ( std::advance( it, oldPos ); it != mInsertedSpillCode.end(); ++it )
      cout << tricore << it->get(); );

  if ( newBB != b ) {
    // TODO: Add update of back-annotation mapping!
  }
};


/*
  insertSpillCode inserts TriCore code for a spill-load or spill-store of a
  register into the WIR.
*/
WIR_BasicBlock &TC_GraphColoring::insertSpillCode( const WIR_BaseRegister &clone,
                                                   const WIR_BaseRegister &r,
                                                   int stackPos,
                                                   WIR_BasicBlock &b,
                                                   std::list<std::reference_wrapper<WIR_Instruction>>::const_iterator pos,
                                                   bool spillStore )
{
  DSTART(
    "virtual WIR_BasicBlock& TC_GraphColoring::insertSpillCode(const WIR_BaseRegister&, const WIR_BaseRegister&, int, WIR_BasicBlock&, list<reference_wrapper<WIR_Instruction> >::const_iterator, bool)" );

  std::reference_wrapper<WIR_BasicBlock> res = b;

  if ( pos == b.getInstructions().end() ) {
    // Someone wants to insert spill code at the end of basic block b. So, we
    // have to check whether b's very last instruction is a jump or a call.
    bool isJump = false;
    bool isCall = false;

    if ( !b.getInstructions().empty() )
      for ( const WIR_Operation &o : b.getInstructions().back().get() ) {
        if ( o.isJump() )
          isJump = true;
        if ( o.isCall() )
          isCall = true;
      }

    if ( isJump ) {
      // OK, we shall insert spill code after a jump. However, this spill code
      // cannot be inserted in b itself. Instead, it has to be placed in all
      // successors of b. However, this is only valid if all these successors
      // have only one predecessor, namely b.
      auto succs = b.getSuccessors();
      bool successorBBsAreValid = true;
      if ( any_of(
             succs.begin(), succs.end(),
             [&]( const std::reference_wrapper<WIR_BasicBlock> ref ) {
               return( ref.get().getPredecessors().size() > 1 ); } ) )
        successorBBsAreValid = false;

      if ( !successorBBsAreValid ) {
        // If the above criterion is not satisfied, we have to insert two new
        // basic blocks as successors of b into the CFG.
        WIR_Function &f = b.getFunction();
        WIR_Operation *jmpOp = nullptr;
        WIR_LabelParameter *jmpLabel = nullptr;

        // Determine the jump operation terminating b, and its jump label.
        for ( WIR_Operation &o : b.getInstructions().back().get() )
          if ( o.isJump() ) {
            jmpOp = &o;

            jmpLabel =
              &(const_cast<WIR_LabelParameter &>(
                dynamic_cast<const WIR_LabelParameter &>(
                  find_if(
                    o.begin(), o.end(),
                    [&]( const WIR_Parameter &p ) {
                      return( p.getType() == WIR_ParameterType::label );
                    } )->get() ) ));
          }

        ufAssert( jmpOp != nullptr );
        ufAssert( jmpLabel != nullptr );

        // Determine explicit and implicit successor blocks of b.
        auto &explicitSucc = jmpLabel->getBasicBlock();
        auto posAfterB = std::next( f.findBasicBlock( b ) );
        ufAssert( posAfterB != f.getBasicBlocks().end() );
        auto &implicitSucc = posAfterB->get();

        // Create a first new basic block after the current block b.
        auto &b1 =
          f.insertBasicBlock(
            posAfterB,
            { { { TC13::OpCode::J, TC13::OperationFormat::SL,
                  WIR_LabelParameter( explicitSucc ) } } } )->get();

        // Create a second new basic block after the current block b.
        posAfterB = f.findBasicBlock( b1 );
        auto &b2 =
          f.insertBasicBlock(
            posAfterB,
            { { { TC13::OpCode::J, TC13::OperationFormat::SL,
                  WIR_LabelParameter( implicitSucc ) } } } )->get();

        // Patch the original branch so that it conditionally jumps to b1.
        jmpOp->replaceParameter(
          jmpOp->findParameter( *jmpLabel ), WIR_LabelParameter( b1 ) );

        // Insert spill code at the beginning of each new basic block.
        insertSpillCode( clone, r, stackPos, b2, b2.begin(), spillStore );
        insertSpillCode( clone, r, stackPos, b1, b1.begin(), spillStore );

        return( b1 );
      } else
        // Insert spill code at the beginning of each successor block.
        for ( WIR_BasicBlock &succ : b.getSuccessors() )
          insertSpillCode( clone, r, stackPos, succ, succ.begin(), spillStore );

      return( b );
    } else

    if ( isCall ) {
      // If the preceding instruction is a function call, insert the new spill
      // code at the beginning of the sole succeeding basic block. If this
      // succeeding basic block has, however, more than one predecessor, it may
      // be entered from somewhere else, too. But the spill code to be generated
      // must not be executed if the successor block is not reached from the
      // current basic block b. In that case, insert a new basic block so that
      // the new spill code is inserted immediately after the function call and
      // this new successor basic block has exactly one predecessor which is b.
      const WIR_BasicBlock &succ = b.getSuccessors().begin()->get();

      if ( succ.getPredecessors().size() > 1 ) {
        WIR_Function &f = b.getFunction();

        // Create a new basic block between the current block b and its succ.
        res = f.insertBasicBlock( f.findBasicBlock( succ ), {} )->get();

        b = res;
        pos = b.getInstructions().end();
      }
    }
  }

  // Due to potential adjustments of the stack pointer, spill-load and -store
  // instructions can be surrounded by address calculation instructions that
  // set and reset the stack pointer. If pos refers to such an already existing
  // load or store instruction with a stack pointer-relative addressing mode, or
  // to one of these address calculation instructions, the new spill code must
  // not be placed between the old existing address calculation instructions.
  if ( isAdjustedLoadOrStoreInstruction( b, pos ) )
    // New spill code shall be inserted before the already existing surrounded
    // load or store instruction. Thus, insert the new spill code before the
    // first LEA instruction.
    --pos;
  else

  if ( ( pos != b.getInstructions().begin() ) &&
       isAdjustedLoadOrStoreInstruction( b, std::prev( pos ) ) )
    // New spill code shall be inserted after the already existing surrounded
    // load or store instruction. Thus, insert the new spill code after the
    // second LEA instruction.
    ++pos;

  // Now that we have correctly adjusted the position before which to insert a
  // new spill instruction, let's generate the spill code.

  list<std::reference_wrapper<WIR_Instruction>> newSpill;

  DOUT(
    "Extending " << string( spillStore ? "mSpillStores" : "mSpillLoads" ) <<
    "[ " << r.getRoot().getID() << " ] by" << endl );

  // Determine the involved processor core.
  WIR_Function &f = b.getFunction();
  WIR_System &sys = f.getCompilationUnit().getSystem();
  const WIR_Section &sec = sys.findSymbol( f ).getSection();
  TC13 &tc = dynamic_cast<TC13 &>( sec.getProcessor() );
  auto &stackPointer = tc.SP();

  // If the stack offset is too large to be encoded as immediate argument in the
  // spill instruction, insert additional address arithmetic instructions before
  // and after the spill code to change the stack pointer temporarily. Pay
  // attention that every offset is sign extended.
  short addressUpperHalf =
    ( ( stackPos + 0x8000 ) / (unsigned int) 0x10000 ) & 0xFFFF;
  short addressLowerHalf = stackPos & 0xFFFF;
  short offset10 = stackPos & 0x3FF;
  const short &offset16 = addressLowerHalf;

  auto insertedInsns = mInsertedSpillCode.end();

  if ( ( stackPos < TC_Const16_Signed::getMinValue( 16 ) )  ||
       ( stackPos > TC_Const16_Signed::getMaxValue( 16 ) ) ) {
    // Insert ADDIH.A instructions.
    auto newInstr =
      mInsertedSpillCode.insert(
        insertedInsns,
        markSpillInstruction(
          b.insertInstruction( pos,
            { { TC13::OpCode::ADDIH_A, TC13::OperationFormat::AAC16,
                WIR_RegisterParameter( stackPointer, WIR_Usage::def ),
                WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                TC_Const16_Signed( addressUpperHalf ) } } )->get(),
          clone ) );
    newSpill.push_back( *newInstr );
    DOUT( *newInstr );

    insertedInsns =
      mInsertedSpillCode.insert(
        insertedInsns,
        markSpillInstruction(
          b.insertInstruction( pos,
            { { TC13::OpCode::ADDIH_A, TC13::OperationFormat::AAC16,
                WIR_RegisterParameter( stackPointer, WIR_Usage::def ),
                WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                TC_Const16_Signed( -addressUpperHalf ) } } )->get(),
          clone ) );
    newSpill.push_back( *insertedInsns );
    DOUT( *insertedInsns );

    --pos;
  }

  if ( ( ( stackPos < TC_Const10_Signed::getMinValue( 10 ) )  ||
         ( stackPos > TC_Const10_Signed::getMaxValue( 10 ) ) ) &&
       ( ( ( clone.getType() == TC13::RegisterType::aReg ) && spillStore ) ||
         ( clone.getType() == TC13::RegisterType::eReg ) ||
         ( clone.getType() == TC13::RegisterType::pReg ) ) &&
       // Test if sign_ext( addressLowerHalf ) != sign_ext( offset10 ).
       // If so, the bits addressLowerHalf[16:10] will be either all zero or all
       // one.
       ( ( addressLowerHalf & 0xFE00 ) != 0 ) &&
       ( ( ( addressLowerHalf / 0x200 ) & 0x7F ) != 0x7F ) ) {
    // The current stack position does not fit into the 10 bits available in the
    // foreseen spill instruction. Thus, insert stack pointer-adjusting LEA
    // instructions.
    auto newInstr =
      mInsertedSpillCode.insert(
        insertedInsns,
        markSpillInstruction(
          b.insertInstruction( pos,
            { { TC13::OpCode::LEA, TC13::OperationFormat::AAC16BOA,
                WIR_RegisterParameter( stackPointer, WIR_Usage::def ),
                WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                TC_Const16_Signed( addressLowerHalf ) } } )->get(),
          clone ) );
    newSpill.push_back( *newInstr );
    DOUT( *newInstr );

    insertedInsns =
      mInsertedSpillCode.insert(
        insertedInsns,
        markSpillInstruction(
          b.insertInstruction( pos,
            { { TC13::OpCode::LEA, TC13::OperationFormat::AAC16BOA,
                WIR_RegisterParameter( stackPointer, WIR_Usage::def ),
                WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                TC_Const16_Signed( -addressLowerHalf ) } } )->get(),
          clone ) );
    newSpill.push_back( *insertedInsns );
    DOUT( *insertedInsns );

    --pos;

    offset10 = 0;
  }

  // Insert the spill instruction itself.
  auto newInstr = mInsertedSpillCode.end();

  if ( spillStore ) {

    if ( clone.getType() == TC13::RegisterType::aReg )
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { TC13::OpCode::ST_A, TC13::OperationFormat::AC10ABOA,
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                  TC_Const10_Signed( offset10 ),
                  WIR_RegisterParameter( clone, WIR_Usage::use ) } } )->get(),
            clone ) );
    else

    if ( clone.getType() == TC13::RegisterType::dReg )
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { TC13::OpCode::ST_W, TC13::OperationFormat::AC16DBOA,
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                  TC_Const16_Signed( offset16 ),
                  WIR_RegisterParameter( clone, WIR_Usage::use ) } } )->get(),
            clone ) );
    else

    if ( clone.getType() == TC13::RegisterType::eReg )
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { TC13::OpCode::ST_D, TC13::OperationFormat::AC10EBOA,
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                  TC_Const10_Signed( offset10 ),
                  WIR_RegisterParameter( clone, WIR_Usage::use ) } } )->get(),
            clone ) );
    else

    if ( clone.getType() == TC13::RegisterType::pReg )
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { TC13::OpCode::ST_DA, TC13::OperationFormat::AC10PBOA,
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                  TC_Const10_Signed( offset10 ),
                  WIR_RegisterParameter( clone, WIR_Usage::use ) } } )->get(),
            clone ) );
  } else {
    if ( clone.getType() == TC13::RegisterType::aReg )
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { TC13::OpCode::LD_A, TC13::OperationFormat::AAC16BOA,
                  WIR_RegisterParameter( clone, WIR_Usage::def ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                  TC_Const16_Signed( offset16 ) } } )->get(),
            clone ) );
    else

    if ( clone.getType() == TC13::RegisterType::dReg )
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { TC13::OpCode::LD_W, TC13::OperationFormat::DAC16BOA,
                  WIR_RegisterParameter( clone, WIR_Usage::def ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                  TC_Const16_Signed( offset16 ) } } )->get(),
            clone ) );
    else

    if ( clone.getType() == TC13::RegisterType::eReg )
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { TC13::OpCode::LD_D, TC13::OperationFormat::EAC10BOA,
                  WIR_RegisterParameter( clone, WIR_Usage::def ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                  TC_Const10_Signed( offset10 ) } } )->get(),
            clone ) );
    else

    if ( clone.getType() == TC13::RegisterType::pReg )
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { TC13::OpCode::LD_DA, TC13::OperationFormat::PAC10BOA,
                  WIR_RegisterParameter( clone, WIR_Usage::def ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                  TC_Const10_Signed( offset10 ) } } )->get(),
            clone ) );
  }

  newSpill.push_back( *newInstr );

  for ( const WIR_Instruction &i : newSpill )
    mStackOffsetOfSpillInstruction[ i.getID() ] = stackPos;

  auto &spillCode =
    spillStore ?
      mSpillStores[ r.getRoot().getID() ] : mSpillLoads[ r.getRoot().getID() ];
  spillCode.push_back( std::move( newSpill ) );

  DOUT( *newInstr );

  return( res.get() );
};


/*
  isAdjustedLoadOrStoreInstruction returns whether the given iterator refers to
  a load or a store instruction with a stack pointer relative addressing mode
  which is surrounded by LEA instructions that adjust the stack pointer.

  isAdjustedLoadOrStoreInstruction checks if the code looks like

    lea    A10, A10, const1             # with const1 > 0
    ld__   __, [A10] __                 # pos
    lea    A10, A10, -const2            # with const2 > 0

  or

    lea    A10, A10, const1             # with const1 > 0
    st__   [A10] __, __                 # pos
    lea    A10, A10, -const2            # with const2 > 0
*/
bool TC_GraphColoring::isAdjustedLoadOrStoreInstruction( const WIR_BasicBlock &b,
                                                         std::list<std::reference_wrapper<WIR_Instruction>>::const_iterator pos ) const
{
  DSTART(
    "bool TC_GraphColoring::isAdjustedLoadOrStoreInstruction(const WIR_BasicBlock&, list<reference_wrapper<WIR_Instruction> >::const_iterator) const" );

  auto &iList = b.getInstructions();

  // If pos refers to the very first instruction within b or to b's very last
  // instruction or if pos is the end() iterator or if pos contains no
  // operation, the above code structure is impossible.
  if ( ( pos == iList.begin() ) || ( pos == iList.end() ) ||
       ( pos->get() == iList.back().get() ) ||
       pos->get().getOperations().empty() )
    return( false );

  const WIR_Instruction &i = pos->get();

  // Check whether pos refers to a load or store instruction.
  if ( !i.getOperations().front().get().isMemoryLoad() &&
       !i.getOperations().front().get().isMemoryStore() )
    return( false );

  const WIR_Operation &o = i.getOperations().front().get();

  // Check whether the load/store uses the stack pointer.
  bool usesSP = false;
  for ( const WIR_Parameter &p : o )
    if ( p.getType() == WIR_ParameterType::reg ) {
      const auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p );
      if ( ( rp.isUsed() || rp.isDefUsed() ) &&
           isStackPointer( rp.getRegister() ) ) {
        usesSP = true;
        break;
      }
    }

  if ( !usesSP )
    return( false );

  // Check whether predecessor and successor are stack-adjusting LEAs.
  const WIR_Instruction &predI = std::prev( pos )->get();
  const WIR_Instruction &succI = std::next( pos )->get();

  if ( predI.getOperations().empty() || succI.getOperations().empty() )
    return( false );
  auto &predO = predI.begin()->get();
  auto &succO = succI.begin()->get();

  if ( !TC13::isStackPointerLEA( predO ) || !TC13::isStackPointerLEA( succO ) )
    return( false );

  signed long long predVal =
    ( predO.getOperationFormat() == TC13::OperationFormat::AAC10BOA ) ?
      dynamic_cast<const TC_Const10_Signed &>(
        predO.getExplicitParameters().back().get() ).getSignedValue() :
      dynamic_cast<const TC_Const16_Signed &>(
        predO.getExplicitParameters().back().get() ).getSignedValue();
  signed long long succVal =
    ( succO.getOperationFormat() == TC13::OperationFormat::AAC10BOA ) ?
      dynamic_cast<const TC_Const10_Signed &>(
        succO.getExplicitParameters().back().get() ).getSignedValue() :
      dynamic_cast<const TC_Const16_Signed &>(
        succO.getExplicitParameters().back().get() ).getSignedValue();

  return( ( predVal > 0 ) && ( succVal < 0 ) );
};


/*
  getCandidatePhregs returns a set of physical registers that could be used for
  the specified virtual register according to the TriCore's ISA.
*/
WIR_PhysicalRegisterSet TC_GraphColoring::getCandidatePhregs( const WIR_VirtualRegister &r )
{
  DSTART(
    "virtual WIR_PhysicalRegisterSet TC_GraphColoring::getCandidatePhregs(const WIR::WIR_VirtualRegister&)" );

  WIR_PhysicalRegisterSet res;

  // Determine the involved processor core.
  const WIR_Function &f = r.getFunction();
  const WIR_System &sys = f.getCompilationUnit().getSystem();
  const WIR_Section &sec = sys.findSymbol( f ).getSection();
  const TC13 &tc = dynamic_cast<TC13 &>( sec.getProcessor() );

  if ( r.getType() == TC13::RegisterType::aReg ) {
    if ( !mUseOnlyUC ) {
      res.insert( const_cast<TC_ARegP &>( tc.A2() ) );
      res.insert( const_cast<TC_ARegP &>( tc.A3() ) );
      res.insert( const_cast<TC_ARegP &>( tc.A4() ) );
      res.insert( const_cast<TC_ARegP &>( tc.A5() ) );
      res.insert( const_cast<TC_ARegP &>( tc.A6() ) );
      res.insert( const_cast<TC_ARegP &>( tc.A7() ) );
    }
    res.insert( const_cast<TC_ARegP &>( tc.A12() ) );
    res.insert( const_cast<TC_ARegP &>( tc.A13() ) );
    res.insert( const_cast<TC_ARegP &>( tc.A14() ) );
    res.insert( const_cast<TC_ARegP &>( tc.A15() ) );
  } else

  if ( r.getType() == TC13::RegisterType::dReg ) {
    if ( !mUseOnlyUC ) {
      res.insert( const_cast<TC_DRegP &>( tc.D0() ) );
      res.insert( const_cast<TC_DRegP &>( tc.D1() ) );
      res.insert( const_cast<TC_DRegP &>( tc.D2() ) );
      res.insert( const_cast<TC_DRegP &>( tc.D3() ) );
      res.insert( const_cast<TC_DRegP &>( tc.D4() ) );
      res.insert( const_cast<TC_DRegP &>( tc.D5() ) );
      res.insert( const_cast<TC_DRegP &>( tc.D6() ) );
      res.insert( const_cast<TC_DRegP &>( tc.D7() ) );
    }
    res.insert( const_cast<TC_DRegP &>( tc.D8() ) );
    res.insert( const_cast<TC_DRegP &>( tc.D9() ) );
    res.insert( const_cast<TC_DRegP &>( tc.D10() ) );
    res.insert( const_cast<TC_DRegP &>( tc.D11() ) );
    res.insert( const_cast<TC_DRegP &>( tc.D12() ) );
    res.insert( const_cast<TC_DRegP &>( tc.D13() ) );
    res.insert( const_cast<TC_DRegP &>( tc.D14() ) );
    res.insert( const_cast<TC_DRegP &>( tc.D15() ) );
  } else

  if ( r.getType() == TC13::RegisterType::eReg ) {
    if ( !mUseOnlyUC ) {
      res.insert( const_cast<TC_ERegP &>( tc.E0() ) );
      res.insert( const_cast<TC_ERegP &>( tc.E2() ) );
      res.insert( const_cast<TC_ERegP &>( tc.E4() ) );
      res.insert( const_cast<TC_ERegP &>( tc.E6() ) );
    }
    res.insert( const_cast<TC_ERegP &>( tc.E8() ) );
    res.insert( const_cast<TC_ERegP &>( tc.E10() ) );
    res.insert( const_cast<TC_ERegP &>( tc.E12() ) );
    res.insert( const_cast<TC_ERegP &>( tc.E14() ) );
  } else {
    if ( !mUseOnlyUC ) {
      res.insert( const_cast<TC_PRegP &>( tc.P2() ) );
      res.insert( const_cast<TC_PRegP &>( tc.P4() ) );
      res.insert( const_cast<TC_PRegP &>( tc.P6() ) );
    }
    res.insert( const_cast<TC_PRegP &>( tc.P12() ) );
    res.insert( const_cast<TC_PRegP &>( tc.P14() ) );
  }

  // Let's store the current uncolored spill for later use (see method
  // getCandidatePhreg below).
  mUncoloredSpill = const_cast<WIR_VirtualRegister *>( &r );

  return( res );
};


/*
  getCandidatePhreg returns one element from the specified set of registers that
  will finally be used within allocateUncoloredActualSpills for spilling.
*/
const WIR_PhysicalRegister &TC_GraphColoring::getCandidatePhreg( const WIR_PhysicalRegisterSet &candidates )
{
  DSTART(
    "virtual const WIR_PhysicalRegister& "
    "TC_GraphColoring::getCandidatePhreg(const WIR_PhysicalRegisterSet&)" );

  TC13 &tc = dynamic_cast<TC13 &>( candidates.begin()->get().getProcessor() );
  std::reference_wrapper<const WIR_PhysicalRegister> res(
    candidates.begin()->get() );

  list<std::reference_wrapper<const WIR_PhysicalRegister>> orderedPhregs = {
    // If possible, we prefer the implicit data/address registers D15 or A15.
    tc.D15(), tc.A15(),
    // Next, we prefer the upper-context registers.
    tc.D14(), tc.D13(), tc.D12(), tc.D11(), tc.D10(), tc.D9(), tc.D8(),
    tc.A14(), tc.A13(), tc.A12(),
    tc.E14(), tc.E12(), tc.E10(), tc.E8(),
    tc.P14(), tc.P12()
  };

  // Finally, we use the lower-context registers.
  if ( !mUseOnlyUC ) {
    orderedPhregs.push_back( tc.D1() );
    orderedPhregs.push_back( tc.D0() );
    orderedPhregs.push_back( tc.D3() );
    orderedPhregs.push_back( tc.D7() );
    orderedPhregs.push_back( tc.D6() );
    orderedPhregs.push_back( tc.D5() );
    orderedPhregs.push_back( tc.D2() );
    orderedPhregs.push_back( tc.D4() );
    orderedPhregs.push_back( tc.A3() );
    orderedPhregs.push_back( tc.A7() );
    orderedPhregs.push_back( tc.A6() );
    orderedPhregs.push_back( tc.A5() );
    orderedPhregs.push_back( tc.A2() );
    orderedPhregs.push_back( tc.A4() );
    orderedPhregs.push_back( tc.E0() );
    orderedPhregs.push_back( tc.E6() );
    orderedPhregs.push_back( tc.E2() );
    orderedPhregs.push_back( tc.E4() );
    orderedPhregs.push_back( tc.P6() );
    orderedPhregs.push_back( tc.P2() );
    orderedPhregs.push_back( tc.P4() );
  }

  // Treat P6, P4 and P2 specially.
  if ( !mUseOnlyUC && candidates.count( const_cast<TC_PRegP &>( tc.P6() ) ) )
    res = tc.P6();
  else

  if ( !mUseOnlyUC && candidates.count( const_cast<TC_PRegP &>( tc.P4() ) ) )
    res = tc.P4();
  else

  if ( !mUseOnlyUC && candidates.count( const_cast<TC_PRegP &>( tc.P2() ) ) )
    res = tc.P2();
  else {
    // Now check the phregs in the specified precedence order.
    auto it =
      find_if(
        orderedPhregs.begin(), orderedPhregs.end(),
        [&]( const WIR_PhysicalRegister &phreg ) {
          return(
            candidates.count( const_cast<WIR_PhysicalRegister &>( phreg ) ) );
        } );
    if ( it != orderedPhregs.end() )
      res = it->get();
  }

  WIR_VirtualRegister &vregRoot = mUncoloredSpill->getRoot();
  if ( !vregRoot.isHierarchical() ) {
    WIR_RegisterSet root;
    root.insert( vregRoot );
    WIR_RegisterSet emptySet;
    checkCallerSavedRegsAliveAcrossCall( res, root, emptySet );
  } else {
    WIR_RegisterSet c1;
    c1.insert( vregRoot.begin()->get() );
    WIR_RegisterSet c2;
    c2.insert( vregRoot.rbegin()->get() );
    checkCallerSavedRegsAliveAcrossCall( res, c1, c2 );
  }

  return( res.get() );
};


/*
  postProcessingHook allows to perform TriCore-specific actions after having
  done register allocation for a function, using e.g., the set of inserted spill
  operations mInsertedSpillCode.

  Here, postProcessingHook is used to realize the TriCore-specific calling
  conventions afterwards.
*/
void TC_GraphColoring::postProcessingHook( WIR_Function &f )
{
  DSTART( "virtual void TC_GraphColoring::postProcessingHook(WIR_Function&)" );

  // First of all, we free some memory that is no longer used.
  f.eraseContainers( WIR_BitValues::getContainerTypeID(), true );
};


/*
  adjustStack allocates additional space in the specified function's stack frame
  and adjusts all stack-related memory accesses accordingly.

  According to the TriCore EABI (section 2.2.2.1), the stack pointer points to
  the bottom (low address) of the stack frame. The stack pointer alignment is 8
  bytes. The argument overflow area for outgoing arguments must be located at
  the bottom (low address end) of the frame, with the first overflow argument at
  zero offset from the stack pointer:

  (Stack
   growing
   direction)
       |
       |   +-------------------------+        (high address)
       |   | Local Variables Frame 1 |
       |   +-------------------------+
       |   | Argument Overflow Area, |
       |   | Function 2 Arguments    |        (first argument passed on stack)
       |   +-------------------------+
       |   | Local Variables Frame 2 |
       |   +-------------------------+
       |   | Argument Overflow Area, |
       |   | Function 3 Arguments    |
       |   +-------------------------+   <--- Stack Pointer (SP) at entry
       V   | Local Variables Frame 3 |        (CALL) to Function 3
           +-------------------------+
           | Argument Overflow Area  |
           +-------------------------+        (low address)
*/
void TC_GraphColoring::adjustStack( WIR_Function &f )
{
  DSTART( "virtual void TC_GraphColoring::adjustStack(WIR_Function&)" );

  if ( ( mAdditionalStackSpace > 0 ) && getVerbosity() )
    ufProgrMsg << ufFile() << "Adjusting stack by " << mAdditionalStackSpace
               << " bytes." << endl;

  TC13::adjustStack( f, mAdditionalStackSpace, mInsertedSpillCode );

  mInsertedSpillCode.clear();
};


/*
  postRACleanup allows to perform very final TriCore-specific cleanup actions,
  particularly after stack frame reorganization.

  Here, postRACleanup is used to remove redundant MOV and SWAP.W instructions.
  Furthermore, don't optimize flags of parameters (indicating accesses to the
  TriCore's argument overflow stack region) are reset.
*/
void TC_GraphColoring::postRACleanup( WIR_Function &f )
{
  DSTART( "virtual void TC_GraphColoring::postRACleanup(WIR_Function&)" );

  if ( getVerbosity() )
    ufProgrMsg << ufFile() << "Simplifying code." << endl;

  for ( WIR_BasicBlock &b : f ) {
    auto it = b.begin();

    while ( it != b.end() ) {
      WIR_Instruction &i1 = it->get();

      if ( i1.getOperations().empty() ) {
        ++it;
        continue;
      }

      WIR_Operation &o1 = i1.getOperations().front().get();

      for ( WIR_Parameter &p : o1 )
        p.setDontOptimize( false );

      // Redundant moves:
      //   mov          <reg>, <reg>                            or
      //   mov.aa       <reg>, <reg>
      if ( o1.isMove() && ( getUseOfMove( o1 ) == getDefOfMove( o1 ) ) ) {
        DACTION(
          BBPOS( i1 );
          DOUT(
            "Removing redundant move " << BBID << tricore << o1 << endl ); );
        it = b.eraseInstruction( it );
        continue;
      }

      if ( i1 == b.getInstructions().back().get() ) {
        ++it;
        continue;
      }

      auto pos = std::next( it );
      const WIR_Instruction &i2 = pos->get();
      if ( i2.getOperations().empty() ) {
        ++it;
        continue;
      }
      const WIR_Operation &o2 = i2.getOperations().front().get();

      // Redundant MOV.A/MOV.D:
      //   mov.a        <reg1>, <reg2>
      //   mov.d        <reg2>, <reg1>                          or
      //
      //   mov.d        <reg1>, <reg2>
      //   mov.a        <reg2>, <reg1>
      if ( ( ( o1.getOpCode() == TC13::OpCode::MOV_A ) &&
             ( o1.getOperationFormat() != TC13::OperationFormat::SAC4_1 ) &&
             ( o2.getOpCode() == TC13::OpCode::MOV_D ) ) ||
           ( ( o1.getOpCode() == TC13::OpCode::MOV_D ) &&
             ( o2.getOpCode() == TC13::OpCode::MOV_A ) &&
             ( o2.getOperationFormat() != TC13::OperationFormat::SAC4_1 ) ) ) {
        const WIR_BaseRegister &r11 =
          dynamic_cast<WIR_RegisterParameter &>(
            o1.getExplicitParameter( 1 ) ).getRegister();
        const WIR_BaseRegister &r12 =
          dynamic_cast<WIR_RegisterParameter &>(
            o1.getExplicitParameter( 2 ) ).getRegister();

        const WIR_BaseRegister &r21 =
          dynamic_cast<WIR_RegisterParameter &>(
            o2.getExplicitParameter( 1 ) ).getRegister();
        const WIR_BaseRegister &r22 =
          dynamic_cast<WIR_RegisterParameter &>(
            o2.getExplicitParameter( 2 ) ).getRegister();

        if ( ( r11 == r22 ) && ( r12 == r21 ) ) {
          DACTION(
            BBPOS( i2 );
            DOUT(
              "Removing redundant mov.a/mov.d " << BBID << tricore << o2 <<
              endl ); );
          b.eraseInstruction( pos );
          continue;
        }
      }

      // Redundant SWAP.W/SWAP.W:
      //   swap.w       [<areg>]c, <reg>
      //   swap.w       [<areg>]c, <reg>
      if ( ( o1.getOpCode() == TC13::OpCode::SWAP_W ) &&
           ( o1.getOperationFormat() == TC13::OperationFormat::AC10DBOA_2 ) &&
           ( o2.getOpCode() == TC13::OpCode::SWAP_W ) &&
           ( o2.getOperationFormat() == TC13::OperationFormat::AC10DBOA_2 ) ) {
        const WIR_BaseRegister &r11 =
          dynamic_cast<WIR_RegisterParameter &>(
            o1.getExplicitParameter( 1 ) ).getRegister();
        long long offset1 =
          dynamic_cast<WIR_BaseImmediateParameter &>(
            o1.getExplicitParameter( 2 ) ).getSignedValue();
        const WIR_BaseRegister &r12 =
          dynamic_cast<WIR_RegisterParameter &>(
            o1.getExplicitParameter( 3 ) ).getRegister();

        const WIR_BaseRegister &r21 =
          dynamic_cast<WIR_RegisterParameter &>(
            o2.getExplicitParameter( 1 ) ).getRegister();
        long long offset2 =
          dynamic_cast<WIR_BaseImmediateParameter &>(
            o2.getExplicitParameter( 2 ) ).getSignedValue();
        const WIR_BaseRegister &r22 =
          dynamic_cast<WIR_RegisterParameter &>(
            o2.getExplicitParameter( 3 ) ).getRegister();

        if ( ( r11 == r21 ) && ( offset1 == offset2 ) && ( r12 == r22 ) ) {
          DACTION(
            BBPOS( i2 );
            DOUT(
              "Removing redundant swap.w " << BBID << tricore << o2 <<
              endl ); );
          b.eraseInstruction( pos );
          DACTION(
            BBPOS( i1 );
            DOUT(
              "Removing redundant swap.w " << BBID << tricore << o1 <<
              endl ); );
          it = b.eraseInstruction( it );
          continue;
        }
      }

      ++it;
    }
  }
};


/*
  For a list of instructions implementing one spill-load or -store,
  getPhregOfSpill determines that physical register that is actually
  spill-loaded or -stored.
*/
WIR_PhysicalRegister &TC_GraphColoring::getPhregOfSpill( const std::list<std::reference_wrapper<WIR_Instruction>> &spill )
{
  DSTART(
    "virtual WIR_PhysicalRegister& TC_GraphColoring::getPhregOfSpill(const list<reference_wrapper<WIR_Instruction> >&)" );

  for ( const WIR_Instruction &i : spill )
    for ( const WIR_Operation &o : i )
      if ( o.isMemoryLoad() || o.isMemoryStore() ) {
        for ( const WIR_Parameter &p : o )
          if ( p.getType() == WIR_ParameterType::reg ) {
            const auto &regP = dynamic_cast<const WIR_RegisterParameter &>( p );

            if ( regP.getRegister().isPhysical() &&
                 !TC13::isSP( regP.getRegister() ) )
              return(
                dynamic_cast<WIR_PhysicalRegister &>( regP.getRegister() ) );
          }
      }

  // If we reach this point, no spill-load/-store was observed up 'till now so
  // that the current spill must be a rematerialization. In this case, we
  // determine the physical register that is defined by the rematerialization
  // instructions.

  // Check the very last rematerialization instruction.
  auto it = spill.rbegin();
  WIR_Operation &o = it->get().begin()->get();
  auto pIt = o.begin();
  for ( ; pIt != o.end(); ++pIt )
    if ( pIt->get().getType() == WIR_ParameterType::reg ) {
      const auto &regP = dynamic_cast<WIR_RegisterParameter &>( pIt->get() );

      if ( regP.getRegister().isPhysical() && regP.isDefined() )
        break;
    }

  auto &lastPhreg =
    dynamic_cast<WIR_PhysicalRegister &>(
      dynamic_cast<WIR_RegisterParameter &>( pIt->get() ).getRegister() );

  // Check the second last rematerialization instruction, if any.
  ++it;
  WIR_PhysicalRegister *prevPhreg = nullptr;
  if ( it != spill.rend() ) {
    o = it->get().begin()->get();

    for ( const WIR_Parameter &p : o )
      if ( p.getType() == WIR_ParameterType::reg ) {
        auto &regP = dynamic_cast<const WIR_RegisterParameter &>( p );

        if ( regP.getRegister().isPhysical() && regP.isDefined() ) {
          prevPhreg =
            dynamic_cast<WIR_PhysicalRegister *>( &regP.getRegister() );
          break;
        }
      }
  }

  if ( ( prevPhreg == nullptr ) ||
       ( prevPhreg->getRoot() != lastPhreg.getRoot() ) )
    return( lastPhreg );
  else
    return( lastPhreg.getRoot() );
};

}       // namespace WIR
