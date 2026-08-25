/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvgraphcoloring.cc
  @brief This file implements a RISC-V-specific graph-coloring based register
         allocator.

  @author Rasmus Mecklenburg <Rasmus.Mecklenburg@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <algorithm>
#include <functional>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <stack>

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>
#include <libuseful/exceptions.h>
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32ifc.h>

// Include local headers
#include "rvgraphcoloring.h"


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
RV_GraphColoring::RV_GraphColoring( WIR_System &s, bool verbosity ) :
  WIR_Optimization { s },
  WIR_GraphColoring { s, verbosity },
  mUncoloredSpill { nullptr },
  mHaveRV32IF { false },
  mHaveRV32ID { false },
  mStackAccessLatency { 0 }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for compilation unit-level optimization.
*/
RV_GraphColoring::RV_GraphColoring( WIR_CompilationUnit &c, bool verbosity ) :
  WIR_Optimization { c },
  WIR_GraphColoring { c, verbosity },
  mUncoloredSpill { nullptr },
  mHaveRV32IF { false },
  mHaveRV32ID { false },
  mStackAccessLatency { 0 }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for function-level optimization.
*/
RV_GraphColoring::RV_GraphColoring( WIR_Function &f, bool verbosity ) :
  WIR_Optimization { f },
  WIR_GraphColoring { f, verbosity },
  mUncoloredSpill { nullptr },
  mHaveRV32IF { false },
  mHaveRV32ID { false },
  mStackAccessLatency { 0 }
{
  DSTART( "RV_GraphColoring::RV_GraphColoring(WIR_Function&, bool)" );
};


/*
  Destructor.
*/
RV_GraphColoring::~RV_GraphColoring( void )
{
  DSTART( "virtual RV_GraphColoring::~RV_GraphColoring()" );
};


/*
  setRV32IF sets whether register allocation shall be performed for RV32IF
  compatible architectures featuring 32 bits wide floating-point registers.
*/
void RV_GraphColoring::setRV32IF( bool f )
{
  DSTART( "void RV_GraphColoring::setRV32IF(bool)" );

  mHaveRV32IF = f;
};


/*
  setRV32ID sets whether register allocation shall be performed for RV32ID
  compatible architectures featuring 64 bits wide floating-point registers.
*/
void RV_GraphColoring::setRV32ID( bool d )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  mHaveRV32ID = d;
};


//
// Protected class methods
//

/*
  runOptimization allocates registers in the given function.
*/
void RV_GraphColoring::runOptimization( WIR_Function &f )
{
  DSTART( "virtual void RV_GraphColoring::runOptimization(WIR_Function&)" );

  // TODO: Add bit-true data flow analysis for RISC-V.

  WIR_GraphColoring::runOptimization( f );
};


/*
  createPhregs sets up the lists mPhregs and mPhregsForPrecoloringOnly of all
  RISC-V-specific physical registers contained in the specified WIR function.
*/
void RV_GraphColoring::createPhregs( WIR_Function &f )
{
  DSTART( "virtual void RV_GraphColoring::createPhregs(WIR_Function&)" );

  // Determine the involved processor core.
  const WIR_System &sys = f.getCompilationUnit().getSystem();
  const WIR_Section &sec = sys.findSymbol( f ).getSection();
  const WIR_BaseProcessor &p = sec.getProcessor();

  // It must be a RISC-V, of course.
  ufAssert( p.getISAName().find( "RV32I" ) == 0 );
  const RV32I &rv = dynamic_cast<const RV32I &>( p );

  // Determine memory access latency of the stack.
  ufAssert( rv.containsSection( ".stack" ) );
  const WIR_MemoryRegion &stackMem =
    rv.findSection( ".stack" )->get().getRegion();
  mStackAccessLatency = stackMem.getMaxDelay();

  // Determine the code access latencies of all basic blocks.
  mBBAccessLatency.clear();
  for ( const WIR_BasicBlock &b : f ) {
    const auto &region = sys.findSymbol( b ).getSection().getRegion();
    mBBAccessLatency[ b.getID() ] = region.getMaxDelay();
  }

  mPhregs = {
    // RISC-V integer registers subject to register allocation.
    const_cast<RV_RegP &>( rv.x5() ), const_cast<RV_RegP &>( rv.x6() ),
    const_cast<RV_RegP &>( rv.x7() ), const_cast<RV_RegP &>( rv.x9() ),
    const_cast<RV_RegP &>( rv.x10() ), const_cast<RV_RegP &>( rv.x11() ),
    const_cast<RV_RegP &>( rv.x12() ), const_cast<RV_RegP &>( rv.x13() ),
    const_cast<RV_RegP &>( rv.x14() ), const_cast<RV_RegP &>( rv.x15() ),
    const_cast<RV_RegP &>( rv.x16() ), const_cast<RV_RegP &>( rv.x17() ),
    const_cast<RV_RegP &>( rv.x18() ), const_cast<RV_RegP &>( rv.x19() ),
    const_cast<RV_RegP &>( rv.x20() ), const_cast<RV_RegP &>( rv.x21() ),
    const_cast<RV_RegP &>( rv.x22() ), const_cast<RV_RegP &>( rv.x23() ),
    const_cast<RV_RegP &>( rv.x24() ), const_cast<RV_RegP &>( rv.x25() ),
    const_cast<RV_RegP &>( rv.x26() ), const_cast<RV_RegP &>( rv.x27() ),
    const_cast<RV_RegP &>( rv.x28() ), const_cast<RV_RegP &>( rv.x29() ),
    const_cast<RV_RegP &>( rv.x30() ), const_cast<RV_RegP &>( rv.x31() ) };

  // RISC-V registers subject to pre-coloring (e.g., if used in some snippet of
  // inline assembly) but NOT subject to register allocation. Here, we list all
  // registers not included in mPhregs.
  mPhregsForPrecoloringOnly = {
    const_cast<RV_RegP &>( rv.x0() ), const_cast<RV_RegP &>( rv.x1() ),
    const_cast<RV_RegP &>( rv.x2() ), const_cast<RV_RegP &>( rv.x3() ),
    const_cast<RV_RegP &>( rv.x4() ), const_cast<RV_RegP &>( rv.x8() ) };

  // Initialize some RISC-V-specific data structures.

  // mOrderedREGsAliveAcrossCall contains all RISC-V registers which are
  // preserved across calls, i.e., which are callee-saved according to the
  // RISC-V ABI, section 1.1.
  mOrderedREGsAliveAcrossCall = {
    const_cast<RV_RegP &>( rv.x8() ), const_cast<RV_RegP &>( rv.x9() ),
    const_cast<RV_RegP &>( rv.x18() ), const_cast<RV_RegP &>( rv.x19() ),
    const_cast<RV_RegP &>( rv.x20() ), const_cast<RV_RegP &>( rv.x21() ),
    const_cast<RV_RegP &>( rv.x22() ), const_cast<RV_RegP &>( rv.x23() ),
    const_cast<RV_RegP &>( rv.x24() ), const_cast<RV_RegP &>( rv.x25() ),
    const_cast<RV_RegP &>( rv.x26() ), const_cast<RV_RegP &>( rv.x27() ) };

  // All physical registers to be used for coloring, in their precedence order.
  mOrderedPhregs = {
    // We first use the callee-saved registers x9 and x18 - x27.
    const_cast<RV_RegP &>( rv.x9() ), const_cast<RV_RegP &>( rv.x18() ),
    const_cast<RV_RegP &>( rv.x19() ), const_cast<RV_RegP &>( rv.x20() ),
    const_cast<RV_RegP &>( rv.x21() ), const_cast<RV_RegP &>( rv.x22() ),
    const_cast<RV_RegP &>( rv.x23() ), const_cast<RV_RegP &>( rv.x24() ),
    const_cast<RV_RegP &>( rv.x25() ), const_cast<RV_RegP &>( rv.x26() ),
    const_cast<RV_RegP &>( rv.x27() ),

    // Next, we prefer the temporary registers x5-x7 and x28 - x31, because they
    // are caller-saved.
    const_cast<RV_RegP &>( rv.x5() ), const_cast<RV_RegP &>( rv.x6() ),
    const_cast<RV_RegP &>( rv.x7() ), const_cast<RV_RegP &>( rv.x28() ),
    const_cast<RV_RegP &>( rv.x29() ), const_cast<RV_RegP &>( rv.x30() ),
    const_cast<RV_RegP &>( rv.x31() ),

    // Finally, we use the argument registers x10 - x17.
    const_cast<RV_RegP &>( rv.x10() ), const_cast<RV_RegP &>( rv.x11() ),
    const_cast<RV_RegP &>( rv.x12() ), const_cast<RV_RegP &>( rv.x13() ),
    const_cast<RV_RegP &>( rv.x14() ), const_cast<RV_RegP &>( rv.x15() ),
    const_cast<RV_RegP &>( rv.x16() ), const_cast<RV_RegP &>( rv.x17() ) };

  mOrderedPhregPairs = {
    // We first use the callee-saved registers x9 and x18 - x27.
    const_cast<RV_ERegP &>( rv.e18() ),
    const_cast<RV_ERegP &>( rv.e20() ),
    const_cast<RV_ERegP &>( rv.e22() ),
    const_cast<RV_ERegP &>( rv.e24() ),
    const_cast<RV_ERegP &>( rv.e26() ),

    // Next, we prefer the temporary registers x5 - x7 and x28 - 31, because
    // they are caller-saved.
    const_cast<RV_ERegP &>( rv.e6() ),
    const_cast<RV_ERegP &>( rv.e28() ),
    const_cast<RV_ERegP &>( rv.e30() ),

    // Finally, we use the argument registers x10 - x17.
    const_cast<RV_ERegP &>( rv.e10() ),
    const_cast<RV_ERegP &>( rv.e12() ),
    const_cast<RV_ERegP &>( rv.e14() ),
    const_cast<RV_ERegP &>( rv.e16() ) };

  // Add floating-point registers to the above sets, if RV32IF is available.
  if ( mHaveRV32IF ) {
    // Get the RV32IF processor core.
    const RV32IF &rvf = dynamic_cast<const RV32IF &>( p );

    // RISC-V floating-point registers subject to register allocation.
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f0() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f1() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f2() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f3() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f4() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f5() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f6() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f7() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f8() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f9() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f10() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f11() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f12() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f13() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f14() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f15() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f16() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f17() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f18() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f19() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f20() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f21() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f22() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f23() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f24() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f25() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f26() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f27() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f28() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f29() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f30() ) );
    mPhregs.push_back( const_cast<RV_FRegP &>( rvf.f31() ) );

    // Extend mOrderedREGsAliveAcrossCall by all RISC-V floating-point registers
    // which are preserved across calls, i.e., which are callee-saved according
    // to the RISC-V ABI, section 1.2.
    mOrderedREGsAliveAcrossCall.push_back( const_cast<RV_FRegP &>( rvf.f8() ) );
    mOrderedREGsAliveAcrossCall.push_back( const_cast<RV_FRegP &>( rvf.f9() ) );
    mOrderedREGsAliveAcrossCall.push_back(
      const_cast<RV_FRegP &>( rvf.f18() ) );
    mOrderedREGsAliveAcrossCall.push_back(
      const_cast<RV_FRegP &>( rvf.f19() ) );
    mOrderedREGsAliveAcrossCall.push_back(
      const_cast<RV_FRegP &>( rvf.f20() ) );
    mOrderedREGsAliveAcrossCall.push_back(
      const_cast<RV_FRegP &>( rvf.f21() ) );
    mOrderedREGsAliveAcrossCall.push_back(
      const_cast<RV_FRegP &>( rvf.f22() ) );
    mOrderedREGsAliveAcrossCall.push_back(
      const_cast<RV_FRegP &>( rvf.f23() ) );
    mOrderedREGsAliveAcrossCall.push_back(
      const_cast<RV_FRegP &>( rvf.f24() ) );
    mOrderedREGsAliveAcrossCall.push_back(
      const_cast<RV_FRegP &>( rvf.f25() ) );
    mOrderedREGsAliveAcrossCall.push_back(
      const_cast<RV_FRegP &>( rvf.f26() ) );
    mOrderedREGsAliveAcrossCall.push_back(
      const_cast<RV_FRegP &>( rvf.f27() ) );

    // All physical registers to be used for coloring, in their precedence
    // order. We first use the callee-saved registers f8, f9 and f18 - f27.
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f8() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f9() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f18() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f19() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f20() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f21() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f22() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f23() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f24() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f25() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f26() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f27() ) );

    // Next, we prefer the temporary registers f0 - f7 and f28 - f31, because
    // they are caller-saved.
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f0() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f1() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f2() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f3() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f4() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f5() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f6() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f7() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f28() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f29() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f30() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f31() ) );

    // Finally, we use the argument registers f10 - f17.
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f10() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f11() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f12() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f13() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f14() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f15() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f16() ) );
    mOrderedPhregs.push_back( const_cast<RV_FRegP &>( rvf.f17() ) );
  }

  // Eliminate the hierarchy of virtual quad-registers so that they become
  // independent integer registers instead.
  map<WIR_id_t, list<WIR_VirtualRegister *>> childsOfqReg;

  for ( WIR_VirtualRegister &vReg : f.getVirtualRegisters() )
    if ( vReg.getType() == RV32I::RegisterType::qReg )
      childsOfqReg[ vReg.getID() ] = vReg.releaseChilds();

  // Replace all occurrences of quad-registers by their childs.
  for ( WIR_BasicBlock &b : f )
    for ( WIR_Instruction &i : b )
      for ( WIR_Operation &o : i )
        for ( auto it = o.begin(); it != o.end(); ) {
          bool erased = false;

          if ( it->get().getType() == WIR_ParameterType::reg ) {
            const auto &rp = dynamic_cast<WIR_RegisterParameter &>( it->get() );

            if ( rp.getRegister().getType() == RV32I::RegisterType::qReg ) {
              ufAssert( rp.isImplicit() );

              const auto &childs = childsOfqReg.at( rp.getRegister().getID() );

              // Add the qReg's childs as implicit parameters.
              for ( const auto *child : childs )
                o.pushBackParameter(
                  WIR_RegisterParameter( *child, rp.getUsage(), true ) );

              // Erase the qReg's parameter.
              it = o.eraseParameter( it );
              erased = true;
            }
          }

          if ( !erased )
            ++it;
        }

  // Finally erase all quad-registers from the current function.
  for ( auto it = f.getVirtualRegisters().begin();
        it != f.getVirtualRegisters().end(); ) {
    if ( it->get().getType() == RV32I::RegisterType::qReg )
      it = f.eraseVirtualRegister( it );
    else
      ++it;
  }
};


/*
  isCallerSaved checks wether the specified physical register is caller-saved.
*/
bool RV_GraphColoring::isCallerSaved( const WIR_PhysicalRegister &r ) const
{
  DSTART(
    "virtual bool RV_GraphColoring::isCallerSaved(const WIR_PhysicalRegister&) "
    "const" );

  // mOrderedREGsAliveAcrossCall contains all registers that are not
  // caller-saved and is defined in createPhregs.
  if ( any_of(
         mOrderedREGsAliveAcrossCall.begin(), mOrderedREGsAliveAcrossCall.end(),
         [&]( const std::reference_wrapper<WIR_PhysicalRegister> ref ) {
           return( r == ref.get() ); } ) )
    return( false );

  return ( true );
};


/*
  checkCallerSavedRegsAliveAcrossCall checks whether the two specified virtual
  leaf registers are mapped to a caller-saved physical register and whether they
  are alive across a function call.

  If so, the respective function call and the physical register are stored in
  mLocalCallerSavedRegsAliveAcrossCall.
*/
void RV_GraphColoring::checkCallerSavedRegsAliveAcrossCall( const WIR_PhysicalRegister &phreg,
                                                            const WIR_RegisterSet &vregs_c1,
                                                            const WIR_RegisterSet &vregs_c2 )
{
  DSTART(
    "void RV_GraphColoring::checkCallerSavedRegsAliveAcrossCall(const "
    "WIR_PhysicalRegister&, const WIR_RegisterSet&, const WIR_RegisterSet&)" );

  if ( !isCallerSaved( phreg ) )
    return;

  // If a caller-saved register was chosen, we have to check whether the
  // associated virtual register is live across some function call. If so, we
  // must make sure to save/restore the chosen caller-saved register across such
  // a function call later.
  RV32I &rv = dynamic_cast<RV32I &>( phreg.getProcessor() );
  auto *rvf =
    mHaveRV32IF ?
      dynamic_cast<RV32IF *>( &(phreg.getProcessor()) ) : nullptr;

  for ( auto &[ theCall, liveVregs ] : mVregsAliveAcrossCall ) {
    WIR_Operation &theCallOp = theCall.get().getOperations().front().get();

    // First, determine whether the current call instruction defines registers
    // x10, x11, e10, f10 or f11, i.e., whether the call returns some result via
    // these registers.
    bool callDefinesX10 = false;
    bool callDefinesX11 = false;
    bool callDefinesE10 = false;
    bool callDefinesF10 = false;
    bool callDefinesF11 = false;

    for ( const WIR_Parameter &p1 : theCallOp )
      if ( p1.getType() == WIR_ParameterType::reg ) {
        const auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p1 );
        const auto &r = rp.getRegister();

        if ( rp.isDefined() ) {
          if ( r == rv.x10() )
            callDefinesX10 = true;
          else

          if ( r == rv.x11() )
            callDefinesX11 = true;
          else

          if ( r == rv.e10() )
            callDefinesE10 = callDefinesX10 = callDefinesX11 = true;
          else

          if ( rvf ) {
            if ( r == rvf->f10() )
              callDefinesF10 = true;
            else

            if ( r == rvf->f11() )
              callDefinesF11 = true;
          }
        }
      }

    if ( callDefinesX10 && callDefinesX11 )
      callDefinesE10 = true;

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
              "Storing caller-saved register " << r.getName() << "/" <<
              phreg.getName() << " to be caller-saved across call " << BBID <<
              theCallOp << endl ); );

          // If phreg is one of x10, x11, e10, f10 or f11, and if this phreg is
          // also defined by the current call, then we have to pay attention. In
          // this particular situation, x10/x11/e10/f10/f11 is alive across a
          // function call, but the call itself overwrites this phreg. Later on,
          // in the post-processing hook, dedicated store and load instructions
          // are generated before and after the call in order to save/restore
          // this phreg:
          //
          //   <some instruction defining, e.g., x10>
          //   ...
          //   st.w x10, [SP] offset;       # this store is missing here!
          //   call func;                   # function call overwrites x10!
          //
          // However, the save instruction storing x10/x11/e10/f10/f11 before
          // the call is not generated NOW. This means that the current call
          // instruction defines/overwrites x10/x11/e10/f10/f11, while a store
          // instruction making use of the original value of x10/x11/e10/f10/f11
          // and saving it on the stack is still missing. This has the effect
          // that liveness information computed during subsequent iterations of
          // the graph coloring allocator is WRONG here. Liveness analysis only
          // sees the DEF of x10/x11/e10/f10/f11 by the call instruction, but a
          // USE of these registers immediately before the call is missing,
          // which finally leads to eventually missing edges in the interference
          // graph.
          // In order to resolve this issue, we add a dummy implicit parameter
          // using x10/x11/e10/f10/f11 to the call instruction in order to model
          // the liveness of x10/x11/e10/f10/f11 correctly. Later on in the
          // post-processing hook, all these dummy USE parameters will be
          // properly removed again.
          if ( ( ( phreg == rv.x10() ) && callDefinesX10 ) ||
               ( ( phreg == rv.x11() ) && callDefinesX11 ) ||
               ( ( phreg == rv.e10() ) && callDefinesE10 ) ||
               ( rvf && ( phreg == rvf->f10() ) && callDefinesF10 ) ||
               ( rvf && ( phreg == rvf->f11() ) && callDefinesF11 ) )
            mLocalDummyParameters.push_back(
              { const_cast<WIR_PhysicalRegister &>( phreg ), theCall } );
        }
    } else {
      // We are dealing with a hierarchical register.
      for ( WIR_BaseRegister &r : vregs_c1 )
        if ( r.isVirtual() &&
             liveVregs.count( dynamic_cast<WIR_VirtualRegister &>( r ) ) ) {
          auto &child1 =
            const_cast<WIR_PhysicalRegister &>( phreg.begin()->get() );

          // The first child of the extended virtual register is live across the
          // call.
          mLocalCallerSavedRegsAliveAcrossCall[ theCall ].insert( child1 );

          DACTION(
            BBPOS( theCall.get() );
            DOUT(
              "Storing caller-saved register " << r.getName() << "/" <<
              child1.getName() << " to be caller-saved across call " << BBID <<
              theCallOp << endl ); );

          // See lengthy comment above.
          if ( ( phreg == rv.x10() ) && callDefinesX10 )
            mLocalDummyParameters.push_back( { child1, theCall } );
        }

      for ( WIR_BaseRegister &r : vregs_c2 )
        if ( r.isVirtual() &&
             liveVregs.count( dynamic_cast<WIR_VirtualRegister &>( r ) ) ) {
          auto &child2 =
            const_cast<WIR_PhysicalRegister &>( phreg.rbegin()->get() );

          // The second child of the extended virtual register is live across
          // the call.
          mLocalCallerSavedRegsAliveAcrossCall[ theCall ].insert( child2 );

          DACTION(
            BBPOS( theCall.get() );
            DOUT(
              "Storing caller-saved register " << r.getName() << "/" <<
              child2.getName() << " to be caller-saved across call " << BBID <<
              theCallOp << endl ); );

          // See lengthy comment above.
          if ( ( phreg == rv.x11() ) && callDefinesX11 )
            mLocalDummyParameters.push_back( { child2, theCall } );
        }
    }
  }
};


/*
  buildProcessorSpecificInterferences adds edges to the interference graph
  expressing RISC-V-specific interferences.
*/
void RV_GraphColoring::buildProcessorSpecificInterferences( WIR_Function &f,
                                                            WIR_InterferenceGraph &igraph )
{
  DSTART(
    "virtual void RV_GraphColoring::buildProcessorSpecificInterferences("
    "WIR_Function&, WIR_InterferenceGraph&)" );

  if ( mHaveRV32IF ) {
    // Update the interference graph for RISC-V RV32IF.
    for ( const WIR_VirtualRegister &r : f.getVirtualRegisters() )
      if ( !r.isChild() && igraph.containsNode( r ) && !isPrecolored( r ) )
        // We have found a virtual register which is either a simple,
        // non-hierarchical register or is the root of a complex register
        // hierarchy. Now, add interferences between a virtual REG and all
        // physical FREGs, and vice versa.
        for ( const WIR_VirtualRegister &l : r.getLeafs() )
          for ( const WIR_PhysicalRegister &preg : mPhregs )
            if ( !igraph.interfere( l, preg ) &&
                !igraph.areSameNodes( l, preg ) &&
                ( ( ( l.getType() == RV32I::RegisterType::reg ) &&
                    ( preg.getType() == RV32IF::RegisterType::fReg ) ) ||
                  ( ( l.getType() == RV32IF::RegisterType::fReg ) &&
                    ( preg.getType() == RV32I::RegisterType::reg ) ) ) ) {
              DOUT(
                "Adding interference " << l.getName() << " <-> " <<
                preg.getName() << endl );
              igraph.addInterference( l, preg, 1 );
            }
  }
};


/*
  isFunctionReturnMove checks whether a given move operation stores the result
  of a function call somewhere.
*/
bool RV_GraphColoring::isFunctionReturnMove( const WIR_Operation &o ) const
{
  DSTART(
    "virtual bool RV_GraphColoring::isFunctionReturnMove(const WIR_Operation&) "
    "const" );

  if ( !o.isMove() )
    return( false );

  const WIR_BaseRegister &useReg = getUseOfMove( o );

  // First, verify that the register used by the move is x10, x11, f10 or f11.
  bool isX10X11F10F11 = false;

  if ( useReg.isPhysical() &&
       ( useReg.getName() == "x10" ) || ( useReg.getName() == "x11" ) ||
       ( useReg.getName() == "f10" ) || ( useReg.getName() == "f11" ) )
    isX10X11F10F11 = true;

  if ( useReg.isVirtual() && isPrecolored( useReg ) ) {
    const WIR_PhysicalRegister &preg =
      mPrecolored.at( const_cast<WIR_BaseRegister &>( useReg ) ).get();

    if ( ( preg.getName() == "x10" ) || ( preg.getName() == "x11" ) ||
         ( preg.getName() == "f10" ) || ( preg.getName() == "f11" ) )
      isX10X11F10F11 = true;
  }

  if ( !isX10X11F10F11 )
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

  For the RISC-V register allocator, high-priority registers are kept in set
  mHighPriorityRegs. Thus, this method only checks whether r is in
  mHighPriorityRegs or not.
*/
bool RV_GraphColoring::isPriorityRegister( const WIR_VirtualRegister &r ) const
{
  DSTART(
    "virtual bool RV_GraphColoring::isPriorityRegister(const "
    "WIR_VirtualRegister&) const" );

  return( mHighPriorityRegs.count( r.getID() ) );
};


/*
  getSpillLoadCosts returns the RISC-V-specific costs of one single spill-load
  for the specified register parameter.
*/
unsigned int RV_GraphColoring::getSpillLoadCosts( const WIR_RegisterParameter &p ) const
{
  DSTART(
    "virtual unsigned int RV_GraphColoring::getSpillLoadCosts(const "
    "WIR_RegisterParameter&) const" );

  const auto &b = p.getOperation().getInstruction().getBasicBlock();
  auto blockLatency = mBBAccessLatency.at( b.getID() );

  return( mStackAccessLatency + blockLatency );
};


/*
  getSpillStoreCosts returns the RISC-V-specific costs of one single spill-store
  for the specified register parameter.
*/
unsigned int RV_GraphColoring::getSpillStoreCosts( const WIR_RegisterParameter &p ) const
{
  DSTART(
    "virtual unsigned int RV_GraphColoring::getSpillStoreCosts(const "
    "WIR_RegisterParameter&) const" );

  return( getSpillLoadCosts( p ) );
};


/*
  getMoveCosts returns the RISC-V-specific costs of one single move operation
  that can be omitted due to spilling.
*/
unsigned int RV_GraphColoring::getMoveCosts( const WIR_Operation &o ) const
{
  DSTART(
    "virtual unsigned int RV_GraphColoring::getMoveCosts(const "
    "WIR_Operation&) const" );

  const auto &b = o.getInstruction().getBasicBlock();

  if ( ( o.getOpCode() == RV32I::OpCode::MOV ) ||
       ( o.getOpCode() == RV32IC::OpCode::CMV ) ||
       ( o.getOpCode() == RV32IF::OpCode::FMV_S ) )
    return( mBBAccessLatency.at( b.getID() ) );

  throw( ufFatalError( "This should never happen...", false ) );
};


/*
  getUseOfMove returns the used register of the specified RISC-V WIR move
  operation.
*/
WIR_BaseRegister &RV_GraphColoring::getUseOfMove( const WIR_Operation &o ) const
{
  DSTART(
    "virtual WIR_BaseRegister& RV_GraphColoring::getUseOfMove(const "
    "WIR_Operation&) const" );

  return(
    dynamic_cast<WIR_RegisterParameter &>(
      o.getExplicitParameter( 2 ) ).getRegister() );
};


/*
  getDefOfMove returns the defined register of the specified RISC-V WIR move
  operation.
*/
WIR_BaseRegister &RV_GraphColoring::getDefOfMove( const WIR_Operation &o ) const
{
  DSTART(
    "virtual WIR_BaseRegister& RV_GraphColoring::getDefOfMove(const "
    "WIR_Operation&) const" );

  return(
    dynamic_cast<WIR_RegisterParameter &>(
      o.getExplicitParameter( 1 ) ).getRegister() );
};


/*
  selectColors assigns actual colors to the RISC-V leaf registers in the
  specified vector.

  This method must not yet assign colors to the interference graph - this is
  done elsewhere. In order to determine feasible colors for the leaf registers,
  this method should make use of WIR_InterferenceGraph::getPossibleColors(). It
  must be ensured that the returned map is either empty or contains exactly one
  entry per leaf. It must hold that none of the colors used in this returned map
  is already used for adjacent interference graph nodes.

  The registers are prioritized according to mOrderedPhregs.
*/
WIR_GraphColoring::WIR_ColorMap RV_GraphColoring::selectColors( const std::vector<std::reference_wrapper<WIR_VirtualRegister>> &leafs,
                                                                const WIR_InterferenceGraph &igraph)
{
  DSTART(
    "virtual WIR_GraphColoring::WIR_ColorMap RV_GraphColoring::selectColors("
    "const vector<reference_wrapper<WIR_VirtualRegister>>&, const "
    "WIR_InterferenceGraph&)" );

  WIR_GraphColoring::WIR_ColorMap res;

  ufAssert( ( leafs.size() == 1 ) || ( leafs.size() == 2 ) );
  set<unsigned int> possibleColors = igraph.getPossibleColors( leafs.front() );

  WIR_VirtualRegister &c1 = leafs.front().get();
  WIR_VirtualRegister &c2 = leafs.back().get();

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
    // A simple non-hierarchical register has to be colored.
    // Now, check the phregs in the specified precedence order.
    for ( const WIR_PhysicalRegister &phreg : mOrderedPhregs ) {
      unsigned int color = igraph.getColorOfPhreg( phreg );

      if ( possibleColors.count( color ) ) {
        res[ c1 ] = color;

        WIR_RegisterSet aliases_c1 = igraph.getCoalescedAliases( c1 );
        aliases_c1.insert( c1 );
        WIR_RegisterSet aliases_c2;

        // Check if the selected color is a caller-saved register and alive
        // across calls. If this is the case, it is saved in
        // mCallerSavedRegsAliveAcrossCall.
        checkCallerSavedRegsAliveAcrossCall( phreg, aliases_c1, aliases_c2 );

        break;
      }
    }
  } else {
    // An extended hierarchical register has to be colored.
    // Now, check the phregs in the specified precedence order.
    for ( const WIR_PhysicalRegister &phreg : mOrderedPhregPairs ) {
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

        // Check if the selected color is a caller-saved register and alive
        // across calls. If this is the case, it is saved in
        // mCallerSavedRegsAliveAcrossCall.
        checkCallerSavedRegsAliveAcrossCall( phreg, aliases_c1, aliases_c2 );

        break;
      }
    }
  }

  return( res );
};


/*
  getStackPosOfSubReg returns the stack position of some child register, if the
  root of the entire register hierarchy is located in the specified stack
  position.

  Note: RISC-V registers are not really hierarchical, but the notion of
        hierarchical registers is used to conveniently implement pairs of
        registers for storing data types with double word width. Thus, this
        method returns the offset for the child register accordingly.
*/
unsigned int RV_GraphColoring::getStackPosOfSubReg( const WIR_VirtualRegister &r,
                                                    unsigned int rootPos ) const
{
  DSTART(
    "virtual unsigned int RV_GraphColoring::getStackPosOfSubReg(const "
    "WIR_VirtualRegister&, unsigned int) const" );

  if ( !r.isChild() || ( r == r.getRoot().begin()->get() ) )
    return( rootPos );
  else
    return( rootPos + 4 );
};


/*
  insertSpillLoad inserts RISC-V code for a spill-load of a register into the
  WIR.

  insertSpillLoad is responsible to add all generated spill-load instructions to
  map mSpillLoads.
*/
void RV_GraphColoring::insertSpillLoad( const WIR_BaseRegister &clone,
                                        const WIR_BaseRegister &r,
                                        int stackPos, WIR_BasicBlock &b,
                                        std::list<std::reference_wrapper<WIR_Instruction>>::const_iterator pos )
{
  DSTART(
    "virtual void RV_GraphColoring::insertSpillLoad(const WIR_BaseRegister&, "
    "const WIR_BaseRegister&, int, WIR_BasicBlock&, "
    "list<reference_wrapper<WIR_Instruction> >::const_iterator)" );

  // Determine the size of the function's overflow area. We must not overwrite
  // the overflow area by spilled registers so that we increase the stack
  // positions of spills by the size of the overflow area.
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
      DOUT( "before position " << riscv << BBID << pos->get() << endl );
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
      cout << riscv << it->get(); );

  if ( newBB != b ) {
    // TODO: Add update of back-annotation mapping!
  }
};


/*
  insertSpillStore inserts RISC-V code for a spill-store of a register into the
  WIR.

  insertSpillStore is responsible to add all generated spill-store instructions
  to map mSpillStores.
*/
void RV_GraphColoring::insertSpillStore( const WIR_BaseRegister &clone,
                                         const WIR_BaseRegister &r,
                                         int stackPos, WIR_BasicBlock &b,
                                         std::list<std::reference_wrapper<WIR_Instruction>>::const_iterator pos )
{
  DSTART(
    "virtual void RV_GraphColoring::insertSpillStore(const WIR_BaseRegister&, "
    "const WIR_BaseRegister&, int, WIR_BasicBlock&, "
    "list<reference_wrapper<WIR_Instruction> >::const_iterator)" );

  // Determine the size of the function's overflow area. We must not overwrite
  // the overflow area by spilled registers so that we increase the stack
  // positions of spills by the size of the overflow area.
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
      DOUT( "before position " << riscv << BBID << pos->get() << endl );
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
      cout << riscv << it->get(); );

  if ( newBB != b ) {
    // TODO: Add update of back-annotation mapping!
  }
};


/*
  insertSpillCode inserts RISC-V code for a spill-load or spill-store of a
  register into the WIR.
*/
WIR_BasicBlock &RV_GraphColoring::insertSpillCode( const WIR_BaseRegister &clone,
                                                   const WIR_BaseRegister &r,
                                                   int stackPos,
                                                   WIR_BasicBlock &b,
                                                   std::list<std::reference_wrapper<WIR_Instruction>>::const_iterator pos,
                                                   bool spillStore )
{
  DSTART(
    "virtual WIR_BasicBlock& RV_GraphColoring::insertSpillCode(const "
    "WIR_BaseRegister&, const WIR_BaseRegister&, int, WIR_BasicBlock&, "
    "list<reference_wrapper<WIR_Instruction> >::const_iterator, bool)" );

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
      // OK, we shall insert spill code after a jump. But this spill code cannot
      // be inserted in b itself. Instead, it has to be placed in all successors
      // of b. However, this is only valid if all these successors have only one
      // predecessor, namely b.
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
            { { { RV32I::OpCode::J, RV32I::OperationFormat::L_1,
                  WIR_LabelParameter( explicitSucc ) } } } )->get();

        // Create a second new basic block after the current block b.
        posAfterB = f.findBasicBlock( b1 );
        auto &b2 =
          f.insertBasicBlock(
            posAfterB,
            { { { RV32I::OpCode::J, RV32I::OperationFormat::L_1,
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
    // first ADDI instruction.
    --pos;
  else

  if ( ( pos != b.getInstructions().begin() ) &&
       isAdjustedLoadOrStoreInstruction( b, std::prev( pos ) ) )
    // New spill code shall be inserted after the already existing surrounded
    // load or store instruction. Thus, insert the new spill code after the
    // second ADDI instruction.
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
  RV32I &rv = dynamic_cast<RV32I &>( sec.getProcessor() );
  auto &stackPointer = rv.SP();

  short offset12 = stackPos & 0xFFF;

  auto insertedInsns = mInsertedSpillCode.end();

  if ( stackPos > RV_Const12_Signed::getMaxValue( 12 ) ) {
    // The stack offset is too large to fit into a const12. As a remedy, we need
    // to move the stack pointer closer to the destination.

    // stackPos, with the last 12 bits set to zero.
    // The stack pointer will have to be moved by this amount.
    int stackPointerMoveAmount =
      ( stackPos / (unsigned int) 0x1000 ) * (unsigned int) 0x1000;

    // TODO: The following code must be broken. If stackPos does not fit into
    //       12 bits, then simply clearing the least-significant 12 bits above
    //       in stackPointerMoveAmount still yields a value > getMaxValue( 12 )
    //       which does not fit into the two ADDI instructions below.
    auto newInstr =
      mInsertedSpillCode.insert(
        insertedInsns,
        markSpillInstruction(
          b.insertInstruction( pos,
            { { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1,
                WIR_RegisterParameter( stackPointer, WIR_Usage::def ),
                WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                RV_Const12_Signed( stackPointerMoveAmount ) } } )->get(),
          clone ) );
    newSpill.push_back( *newInstr );
    DOUT( *newInstr );

    insertedInsns =
      mInsertedSpillCode.insert(
        insertedInsns,
        markSpillInstruction(
          b.insertInstruction( pos,
            { { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1,
                WIR_RegisterParameter( stackPointer, WIR_Usage::def ),
                WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                RV_Const12_Signed( -stackPointerMoveAmount ) } } )->get(),
          clone ) );
    newSpill.push_back( *insertedInsns );
    DOUT( *insertedInsns );

    --pos;
  }

  // Insert the spill instruction itself.
  std::list<std::reference_wrapper<WIR_Instruction>>::iterator newInstr;

  if ( spillStore ) {
    if ( clone.getType() == RV32I::RegisterType::reg )
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { RV32I::OpCode::SW, RV32I::OperationFormat::RC12R_2,
                  WIR_RegisterParameter( clone, WIR_Usage::use ),
                  RV_Const12_Signed( offset12 ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ) } } )->get(),
            clone ) );
    else

    if ( clone.getType() == RV32I::RegisterType::eReg ) {
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { RV32I::OpCode::SW, RV32I::OperationFormat::RC12R_2,
                  WIR_RegisterParameter(
                    clone.getLeafs().begin()->get(), WIR_Usage::use ),
                  RV_Const12_Signed( offset12 ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ) } } )->get(),
            clone ) );
      newSpill.push_back( *newInstr );

      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { RV32I::OpCode::SW, RV32I::OperationFormat::RC12R_2,
                  WIR_RegisterParameter(
                    clone.getLeafs().rbegin()->get(), WIR_Usage::use ),
                  RV_Const12_Signed( offset12 + 4 ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ) } } )->get(),
            clone ) );
    } else

    if ( clone.getType() == RV32IF::RegisterType::fReg )
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { RV32IF::OpCode::FSW, RV32IF::OperationFormat::FC12R_2,
                  WIR_RegisterParameter( clone, WIR_Usage::use ),
                  RV_Const12_Signed( offset12 ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ) } } )->get(),
            clone ) );
  } else {
    if ( clone.getType() == RV32I::RegisterType::reg )
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { RV32I::OpCode::LW, RV32I::OperationFormat::RC12R_1,
                  WIR_RegisterParameter( clone, WIR_Usage::def ),
                  RV_Const12_Signed( offset12 ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ) } } )->get(),
            clone ) );
    else

    if ( clone.getType() == RV32I::RegisterType::eReg ) {
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { RV32I::OpCode::LW, RV32I::OperationFormat::RC12R_1,
                  WIR_RegisterParameter(
                    clone.getLeafs().begin()->get(), WIR_Usage::def ),
                  RV_Const12_Signed( offset12 ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ) } } )->get(),
            clone ) );
      newSpill.push_back( *newInstr );

      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { RV32I::OpCode::LW, RV32I::OperationFormat::RC12R_1,
                  WIR_RegisterParameter(
                    clone.getLeafs().rbegin()->get(), WIR_Usage::def ),
                  RV_Const12_Signed( offset12 + 4 ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ) } } )->get(),
            clone ) );
    } else

    if ( clone.getType() == RV32IF::RegisterType::fReg )
      newInstr =
        mInsertedSpillCode.insert(
          insertedInsns,
          markSpillInstruction(
            b.insertInstruction( pos,
              { { RV32IF::OpCode::FLW, RV32IF::OperationFormat::FC12R_1,
                  WIR_RegisterParameter( clone, WIR_Usage::def ),
                  RV_Const12_Signed( offset12 ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ) } } )->get(),
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
  which is surrounded by ADDI instructions that adjust the stack pointer.

  isAdjustedLoadOrStoreInstruction checks if the code looks like

    addi   x2, x2, const1               # with const1 > 0
    l__    __, __(x2)                   # pos
    addi   x2, x2, -const2              # with const2 > 0

  or

    addi   x2, x2, const1               # with const1 > 0
    s__    __, __(x2)                   # pos
    addi   x2, x2, -const2              # with const2 > 0

  TODO: Since the current implementation of handling offsets larger than 12 bits
        violates the RISC-V ABI, this needs to be rewritten once that the
        implementation is changed to be ABI-compatible.
*/
bool RV_GraphColoring::isAdjustedLoadOrStoreInstruction( const WIR_BasicBlock &b,
                                                         std::list<std::reference_wrapper<WIR_Instruction>>::const_iterator pos ) const
{
  DSTART(
    "bool RV_GraphColoring::isAdjustedLoadOrStoreInstruction(const "
    "WIR_BasicBlock&, "
    "list<reference_wrapper<WIR_Instruction> >::const_iterator) const" );

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
      if ( rp.isUsed() || rp.isDefUsed() &&
           isStackPointer( rp.getRegister() ) ) {
        usesSP = true;
        break;
      }
    }

  if ( !usesSP )
    return ( false );

  // Check whether predecessor and successor are stack-adjusting ADDIs.
  const WIR_Instruction &predI = std::prev( pos )->get();
  const WIR_Instruction &succI = std::next( pos )->get();

  if ( predI.getOperations().empty() || succI.getOperations().empty() )
    return( false );
  auto &predO = predI.begin()->get();
  auto &succO = succI.begin()->get();

  if ( !RV32IC::isStackPointerADDI( predO ) ||
       !RV32IC::isStackPointerADDI( succO ) )
    return ( false );

  signed long long predVal =
    dynamic_cast<const RV_Const12_Signed &>(
      predO.getExplicitParameter( 3 ) ).getSignedValue();
  signed long long succVal =
    dynamic_cast<const RV_Const12_Signed &>(
      succO.getExplicitParameter( 3 ) ).getSignedValue();

  return ( ( predVal > 0 ) && ( succVal < 0 ) );
};


/*
  getCandidatePhregs returns a set of physical registers that could be used for
  the specified virtual register according to the RISC-V's ISA.
*/
WIR_PhysicalRegisterSet RV_GraphColoring::getCandidatePhregs( const WIR_VirtualRegister &r )
{
  DSTART(
    "virtual WIR_PhysicalRegisterSet RV_GraphColoring::getCandidatePhregs("
    "const WIR::WIR_VirtualRegister&)" );

  WIR_PhysicalRegisterSet res;
  const WIR_Function &f = r.getFunction();
  const WIR_System &sys = f.getCompilationUnit().getSystem();
  const WIR_Section &sec = sys.findSymbol( f ).getSection();
  const RV32I &rv = dynamic_cast<RV32I &>( sec.getProcessor() );

  if ( r.getType() == RV32I::RegisterType::reg ) {
    res.insert( const_cast<RV_RegP &>( rv.x5() ) );
    res.insert( const_cast<RV_RegP &>( rv.x6() ) );
    res.insert( const_cast<RV_RegP &>( rv.x7() ) );
    res.insert( const_cast<RV_RegP &>( rv.x9() ) );
    res.insert( const_cast<RV_RegP &>( rv.x10() ) );
    res.insert( const_cast<RV_RegP &>( rv.x11() ) );
    res.insert( const_cast<RV_RegP &>( rv.x12() ) );
    res.insert( const_cast<RV_RegP &>( rv.x13() ) );
    res.insert( const_cast<RV_RegP &>( rv.x14() ) );
    res.insert( const_cast<RV_RegP &>( rv.x15() ) );
    res.insert( const_cast<RV_RegP &>( rv.x16() ) );
    res.insert( const_cast<RV_RegP &>( rv.x17() ) );
    res.insert( const_cast<RV_RegP &>( rv.x18() ) );
    res.insert( const_cast<RV_RegP &>( rv.x19() ) );
    res.insert( const_cast<RV_RegP &>( rv.x20() ) );
    res.insert( const_cast<RV_RegP &>( rv.x21() ) );
    res.insert( const_cast<RV_RegP &>( rv.x22() ) );
    res.insert( const_cast<RV_RegP &>( rv.x23() ) );
    res.insert( const_cast<RV_RegP &>( rv.x24() ) );
    res.insert( const_cast<RV_RegP &>( rv.x25() ) );
    res.insert( const_cast<RV_RegP &>( rv.x26() ) );
    res.insert( const_cast<RV_RegP &>( rv.x27() ) );
    res.insert( const_cast<RV_RegP &>( rv.x28() ) );
    res.insert( const_cast<RV_RegP &>( rv.x29() ) );
    res.insert( const_cast<RV_RegP &>( rv.x30() ) );
    res.insert( const_cast<RV_RegP &>( rv.x31() ) );
  } else

  if ( r.getType() == RV32IF::RegisterType::fReg ) {
    const RV32IF &rvf = dynamic_cast<RV32IF &>( sec.getProcessor() );

    res.insert( const_cast<RV_FRegP &>( rvf.f0() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f1() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f2() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f3() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f4() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f5() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f6() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f7() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f8() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f9() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f10() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f11() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f12() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f13() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f14() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f15() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f16() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f17() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f18() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f19() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f20() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f21() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f22() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f23() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f24() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f25() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f26() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f27() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f28() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f29() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f30() ) );
    res.insert( const_cast<RV_FRegP &>( rvf.f31() ) );
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
const WIR_PhysicalRegister &RV_GraphColoring::getCandidatePhreg( const WIR_PhysicalRegisterSet &candidates )
{
  DSTART(
    "virtual const WIR_PhysicalRegister& "
    "RV_GpaphColoring::getCandidatePhreg(const WIR_PhysicalRegisterSet&" );

  std::reference_wrapper<const WIR_PhysicalRegister> res(
    candidates.begin()->get() );

  // Now check the phregs in the specified precedence order.
  auto it =
    find_if(
      mOrderedPhregs.begin(), mOrderedPhregs.end(),
      [&]( const WIR_PhysicalRegister &phreg ) {
        return(
          candidates.count( const_cast<WIR_PhysicalRegister &>( phreg ) ) );
      } );
  if ( it != mOrderedPhregs.end() )
    res = it->get();

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
  postProcessingHook allows to perform RISC-V-specific actions after having
  done register allocation for a function, using e.g., the set of inserted spill
  operations mInsertedSpillCode.

  Here, postProcessingHook is used to realize the RISC-V-specific calling
  conventions afterwards.
*/
void RV_GraphColoring::postProcessingHook( WIR_Function &f )
{
  DSTART( "virtual void RV_GraphColoring::postProcessingHook(WIR_Function&)" );

  // First of all, we free some memory that is no longer used.
  f.eraseContainers( WIR_BitValues::getContainerTypeID(), true );
};


/*
  adjustStack allocates additional space in the specified function's stack frame
  and adjusts all stack-related memory accesses accordingly.

  According to the RISC-V ABI (section 2.1, Integer Calling Convention), the
  stack grows downwards (towards lower addresses) and the stack pointer shall be
  aligned to a 128-bit boundary upon procedure entry. The first argument passed
  on the stack is located at offset zero of the stack pointer on function entry;
  following arguments are stored at correspondingly higher addresses.

  In the standard ABI, the stack pointer must remain aligned throughout
  procedure execution. [...]

  Procedures must not rely upon the persistence of stack-allocated data whose
  addresses lie below the stack pointer.

  (Stack
   growing
   direction)
       |
       |   +-------------------------+      (high address)
       |   | Local Variables Func 1  |
       |   +-------------------------+
       |   | Argument Area for func- |
       |   | tions called by Func 1  |      (first argument passed on stack)
       |   +-------------------------+
       |   | Local Variables Func 2  |
       |   +-------------------------+
       |   | Argument Area for func- |
       |   | tions called by Func 2  |
       |   +-------------------------+ <--- Stack Pointer (SP) at entry
       V   | Local Variables Func 3  |      (CALL) to Function 3
           +-------------------------+
           | Argument Area for func- |
           | tions called by Func 3  |
           +-------------------------+ <--- Stack Pointer (SP) after stack
           |                         |      allocation of Function 3
           |           ...           |
           +-------------------------+      (low address)
*/
void RV_GraphColoring::adjustStack( WIR_Function &f )
{
  DSTART( "virtual void RV_GraphColoring::adjustStack(WIR_Function&)" );

  if ( ( mAdditionalStackSpace > 0 ) && getVerbosity() )
    ufProgrMsg << ufFile() << "Adjusting stack by " << mAdditionalStackSpace
               << " bytes." << endl;

  RV32IFC::adjustStack( f, mAdditionalStackSpace, mInsertedSpillCode );

  mInsertedSpillCode.clear();
};


/*
  postRACleanup allows to perform very final RISC-V-specific cleanup actions,
  particularly after stack frame reorganization.

  Here, postRACleanup is used to remove redundant MOV instructions. Furthermore,
  don't optimize flags of parameters (indicating accesses to the RISC-V's
  argument overflow stack region) are reset.
*/
void RV_GraphColoring::postRACleanup( WIR_Function &f )
{
  DSTART( "virtual void RV_GraphColoring::postRACleanup(WIR_Function&)" );

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
      //   fmv.s        <reg>, <reg>
      if ( o1.isMove() && ( getUseOfMove( o1 ) == getDefOfMove( o1 ) ) ) {
        DACTION(
          BBPOS( i1 );
          DOUT( "Removing redundant move " << BBID << riscv << o1 << endl ); );
        it = b.eraseInstruction( it );
        continue;
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
WIR_PhysicalRegister &RV_GraphColoring::getPhregOfSpill( const std::list<std::reference_wrapper<WIR_Instruction>> &spill )
{
  DSTART(
    "virtual WIR_PhysicalRegister& RV_GraphColoring::getPhregOfSpill("
    "const list<reference_wrapper<WIR_Instruction> >&)" );

  for ( const WIR_Instruction &i : spill )
    for ( const WIR_Operation &o : i )
      if ( o.isMemoryLoad() || o.isMemoryStore() ) {
        for ( const WIR_Parameter &p : o )
          if ( p.getType() == WIR_ParameterType::reg ) {
            const auto &regP = dynamic_cast<const WIR_RegisterParameter &>( p );

            if ( regP.getRegister().isPhysical() &&
                 !RV32I::isSP( regP.getRegister() ) )
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
      const auto &regP =
        dynamic_cast<const WIR_RegisterParameter &>( pIt->get() );

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
        const auto &regP = dynamic_cast<const WIR_RegisterParameter &>( p );

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
