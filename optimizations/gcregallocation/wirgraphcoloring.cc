/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirgraphcoloring.cc
  @brief This file implements a graph-coloring based register allocator.

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
#include <cmath>
#include <functional>
#include <limits>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

// Include libuseful headers
#include <libuseful/debugmacros.h>
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <analyses/liveness/wirlivenessanalysis.h>
#include <optimizations/webs/wirwebs.h>

// Include local headers
#include "wirgraphcoloring.h"
#include "wirinterferencegraph.h"


//
// Preprocessor macros
//

// #define ENABLE_INVARIANTS

#define BBPOS( __op )                                                          \
  WIR_Instruction &__i = __op.getInstruction();                                \
  WIR_BasicBlock &__b = __i.getBasicBlock();                                   \
  unsigned int __bbPos = 1;                                                    \
  for ( auto it = __b.getInstructions().begin(); it->get() != __i;             \
        ++it, ++__bbPos ) ;                                                    \
  unsigned int __nestingDepth = getLoopNestingDepth( __b );

#define BBID                                                                   \
  "(" << __b.getName() << "/" << __bbPos << "/" << __nestingDepth << ")" << endl

#ifdef ENABLE_INVARIANTS

#define DEBUG_OP_INS( __op, __reg )                                            \
  {                                                                            \
    DACTION(                                                                   \
      BBPOS( __op )                                                            \
      DOUT(                                                                    \
        "Inserting MOVE operation " << BBID << __op << endl <<                 \
        "into mMoveList[ " << __reg.getName() << " ]." << endl ); );           \
  }

#define DEBUG_OP_ADD( __op, __moveSet )                                        \
  {                                                                            \
    DSTART( "WIR_GraphColoring.worklists" );                                   \
    DACTION(                                                                   \
      BBPOS( __op )                                                            \
      DOUT(                                                                    \
        "Adding MOVE operation " << BBID << __op << endl << "to " <<           \
        __moveSet << "." << endl ); );                                         \
  }

#define DEBUG_OP_MOV( __op, __from_wl, __to_wl )                               \
  {                                                                            \
    DSTART( "WIR_GraphColoring.worklists" );                                   \
    DACTION(                                                                   \
      BBPOS( __op )                                                            \
      DOUT(                                                                    \
        "Moving MOVE operation " << BBID << __op << endl << "from " <<         \
        __from_wl << " to " << __to_wl << "." << endl ); );                    \
  }

#define DEBUG_OP_RM( __op, __moveSet )                                         \
  {                                                                            \
    DSTART( "WIR_GraphColoring.worklists" );                                   \
    DACTION(                                                                   \
      BBPOS( __op )                                                            \
      DOUT(                                                                    \
        "Removing MOVE operation " << BBID << __op << endl << "from " <<       \
        __moveSet << "." << endl ); );                                         \
  }

#define DEBUG_REG_ADD( __reg, __to_wl )                                        \
  {                                                                            \
    DSTART( "WIR_GraphColoring.worklists" );                                   \
    DOUT( "Adding '" << __reg.getName() << "' to " << __to_wl << "." << endl );\
  }

#define DEBUG_REG_MOV( __reg, __from_wl, __to_wl )                             \
  {                                                                            \
    DSTART( "WIR_GraphColoring.worklists" );                                   \
    DOUT(                                                                      \
      "Moving '" << __reg.getName() << "' from " << __from_wl << " to " <<     \
      __to_wl << "." << endl );                                                \
  }

#define DEBUG_REG_RM( __reg, __from_wl )                                       \
  {                                                                            \
    DSTART( "WIR_GraphColoring.worklists" );                                   \
    DOUT(                                                                      \
      "Removing '" << __reg.getName() << "' from " << __from_wl << "." <<      \
      endl );\
  }

#define DEBUG_REG_PUSH( __reg )                                                \
  {                                                                            \
    DSTART( "WIR_GraphColoring.worklists" );                                   \
    DOUT( "Pushing '" << __reg.getName() << "' onto stack." << endl );         \
  }

#define DEBUG_REG_POP( __reg )                                                 \
  {                                                                            \
    DSTART( "WIR_GraphColoring.worklists" );                                   \
    DOUT( "Popping '" << __reg.getName() << "' from stack." << endl );         \
  }

// The following macro tests the move sets invariant from A. W. Appel, page 250
// for the given move operation.
#define DEBUG_MOVE_SETS_INVARIANT( __op )                                      \
  {                                                                            \
    DSTART( "WIR_GraphColoring.invariants" );                                  \
    DACTION(                                                                   \
      bool __moveSetsInvariant =                                               \
        ( mActiveMoves.count( __op ) + mCoalescedMoves.count( __op ) +         \
          mConstrainedMoves.count( __op ) + mFrozenMoves.count( __op ) +       \
          mWorkListMoves.count( __op ) ) == 1;                                 \
      if ( !__moveSetsInvariant ) {                                            \
        BBPOS( __op )                                                          \
        DOUT(                                                                  \
          "Move Sets Invariant failed for MOVE operation " << BBID <<          \
          __op << ":" << endl );                                               \
        if ( mActiveMoves.count( __op ) )                                      \
          DOUT(                                                                \
            "mActiveMoves.count( " << __b.getName() << "/" << __bbPos <<       \
            " ) == " << mActiveMoves.count( __op ) << endl );                  \
        if ( mCoalescedMoves.count( __op ) )                                   \
          DOUT(                                                                \
            "mCoalescedMoves.count( " << __b.getName() << "/" << __bbPos <<    \
            " ) == " << mCoalescedMoves.count( __op ) << endl );               \
        if ( mConstrainedMoves.count( __op ) )                                 \
          DOUT(                                                                \
            "mConstrainedMoves.count( " << __b.getName() << "/" <<             \
            __bbPos << " ) == " << mConstrainedMoves.count( __op ) << endl );  \
        if ( mFrozenMoves.count( __op ) )                                      \
          DOUT(                                                                \
            "mFrozenMoves.count( " << __b.getName() << "/" << __bbPos <<       \
            " ) == " << mFrozenMoves.count( __op ) << endl );                  \
        if ( mWorkListMoves.count( __op ) )                                    \
          DOUT(                                                                \
            "mWorkListMoves.count( " << __b.getName() << "/" << __bbPos <<     \
            " ) == " << mWorkListMoves.count( __op ) << endl );                \
      }                                                                        \
      ufAssert( __moveSetsInvariant ); );                                      \
  }

// The following macro basically tests the spill worklist invariant from A. W.
// Appel, page 251 for the given register and the interference graph. In
// addition, we assert that pre-colored registers are not spilled, c.f. A. W.
// Appel, page 243.
#define DEBUG_SPILL_WL_INVARIANT( __r, __igraph )                              \
  {                                                                            \
    DSTART( "WIR_GraphColoring.invariants" );                                  \
    DACTION(                                                                   \
      bool __spillWorkListInvariant =                                          \
        __r.isVirtual() && !__r.isChild() && __igraph.containsNode( __r ) &&   \
        ( __r == __igraph.getUnaliasedReg( __r ) ) &&                          \
        !__igraph.isColored( __r ) &&                                          \
        !isPrecolored( __r ) &&                                                \
        ( mSpillAll ||                                                         \
          ( __igraph.getDegree( __r ) >= __igraph.getAvailableColors() ) );    \
      if ( !__spillWorkListInvariant )                                         \
        DOUT(                                                                  \
          "Spill Worklist Invariant failed for register '" << __r.getName() << \
          "':" << endl );                                                      \
      if ( !__r.isVirtual() )                                                  \
        DOUT( "  " << __r.getName() << ".isVirtual() == false" << endl );      \
      if ( __r.isChild() )                                                     \
        DOUT(                                                                  \
          "  " << __r.getName() << ".isChild() == true" << endl );             \
      if ( !__igraph.containsNode( __r ) )                                     \
        DOUT(                                                                  \
          "  igraph.containsNode( " << __r.getName() << " ) == false" <<       \
          endl );                                                              \
      if ( __r != __igraph.getUnaliasedReg( __r ) )                            \
        DOUT(                                                                  \
          "  " << __r.getName() << " != unaliasedReg == " <<                   \
          __igraph.getUnaliasedReg( __r ).getName() << endl );                 \
      if ( isPrecolored( __r ) )                                               \
        DOUT( "  isPrecolored( " << __r.getName() << " ) == true" << endl );   \
      if ( !mSpillAll &&                                                       \
           ( __igraph.getDegree( __r ) < __igraph.getAvailableColors() ) )     \
        DOUT(                                                                  \
          "  igraph.getDegree( " << __r.getName() << " ) == " <<               \
          __igraph.getDegree( __r ) << " < " <<                                \
          __igraph.getAvailableColors() << endl );                             \
      ufAssert( __spillWorkListInvariant ); );                                 \
  }

// The following macro basically tests the freeze worklist invariant from A. W.
// Appel, page 251 for the given register and the interference graph.
#define DEBUG_FREEZE_WL_INVARIANT( __r, __igraph )                             \
  {                                                                            \
    DSTART( "WIR_GraphColoring.invariants" );                                  \
    DACTION(                                                                   \
      bool __freezeWorkListInvariant =                                         \
        __r.isVirtual() && !__r.isChild() && __igraph.containsNode( __r ) &&   \
        ( __r == __igraph.getUnaliasedReg( __r ) ) &&                          \
        isMoveRelated( __r ) &&                                                \
        ( __igraph.getDegree( __r ) < __igraph.getAvailableColors() );         \
      if ( !__freezeWorkListInvariant )                                        \
        DOUT(                                                                  \
          "Freeze Worklist Invariant failed for register '" <<                 \
          __r.getName() << "':" << endl );                                     \
      if ( !__r.isVirtual() )                                                  \
        DOUT( "  " << __r.getName() << ".isVirtual() == false" << endl );      \
      if ( __r.isChild() )                                                     \
        DOUT(                                                                  \
          "  " << __r.getName() << ".isChild() == true" << endl );             \
      if ( !__igraph.containsNode( __r ) )                                     \
        DOUT(                                                                  \
          "  igraph.containsNode( " << __r.getName() << " ) == false" <<       \
          endl );                                                              \
      if ( __r != __igraph.getUnaliasedReg( __r ) )                            \
        DOUT(                                                                  \
          "  " << __r.getName() << " != unaliasedReg == " <<                   \
          __igraph.getUnaliasedReg( __r ).getName() << endl );                 \
      if ( !isMoveRelated( __r ) )                                             \
        DOUT( "  isMoveRelated( " << __r.getName() << " ) == false" << endl ); \
      if ( __igraph.getDegree( __r ) >= __igraph.getAvailableColors() )        \
        DOUT(                                                                  \
          "  igraph.getDegree( " << __r.getName() << " ) == " <<               \
          __igraph.getDegree( __r ) << " >= " <<                               \
          __igraph.getAvailableColors() << endl );                             \
      ufAssert( __freezeWorkListInvariant ); );                                \
  }

// The following macro weakly tests the freeze worklist invariant from A. W.
// Appel, page 251 for the given register and the interference graph. Weakly
// means here that, compared to the true worklist invariant, we do not check
// whether the given register is move-related. This weakened invariant should
// be used when removing a register from mWorkListFreeze that has been coalesced
// before and thus is no longer move-related.
#define DEBUG_FREEZE_WL_WEAK_INVARIANT( __r, __igraph )                        \
  {                                                                            \
    DSTART( "WIR_GraphColoring.invariants" );                                  \
    DACTION(                                                                   \
      bool __freezeWorkListWeakInvariant =                                     \
        __r.isVirtual() && !__r.isChild() && __igraph.containsNode( __r ) &&   \
        ( __r == __igraph.getUnaliasedReg( __r ) ) &&                          \
        ( __igraph.getDegree( __r ) < __igraph.getAvailableColors() );         \
      if ( !__freezeWorkListWeakInvariant )                                    \
        DOUT(                                                                  \
          "Freeze Worklist Weak Invariant failed for register '" <<            \
          __r.getName() << "':" << endl );                                     \
      if ( !__r.isVirtual() )                                                  \
        DOUT( "  " << __r.getName() << ".isVirtual() == false" << endl );      \
      if ( __r.isChild() )                                                     \
        DOUT(                                                                  \
          "  " << __r.getName() << ".isChild() == true" << endl );             \
      if ( !__igraph.containsNode( __r ) )                                     \
        DOUT(                                                                  \
          "  igraph.containsNode( " << __r.getName() << " ) == false" <<       \
          endl );                                                              \
      if ( __r != __igraph.getUnaliasedReg( __r ) )                            \
        DOUT(                                                                  \
          "  " << __r.getName() << " != unaliasedReg == " <<                   \
          __igraph.getUnaliasedReg( __r ).getName() << endl );                 \
      if ( __igraph.getDegree( __r ) >= __igraph.getAvailableColors() )        \
        DOUT(                                                                  \
          "  igraph.getDegree( " << __r.getName() << " ) == " <<               \
          __igraph.getDegree( __r ) << " >= " <<                               \
          __igraph.getAvailableColors() << endl );                             \
      ufAssert( __freezeWorkListWeakInvariant ); );                            \
  }

// The following macro basically tests the simplify worklist invariant from A.
// W. Appel, page 251 for the given register and the interference graph. In
// addition, we assert that pre-colored registers are not simplified, c.f. A. W.
// Appel, page 243.
#define DEBUG_SIMPLIFY_WL_INVARIANT( __r, __igraph )                           \
  {                                                                            \
    DSTART( "WIR_GraphColoring.invariants" );                                  \
    DACTION(                                                                   \
      bool __simplifyWorkListInvariant =                                       \
        __r.isVirtual() && !__r.isChild() && __igraph.containsNode( __r ) &&   \
        ( __r == __igraph.getUnaliasedReg( __r ) ) &&                          \
        !isPrecolored( __r ) && !isMoveRelated( __r ) &&                       \
        ( __igraph.getDegree( __r ) < __igraph.getAvailableColors() );         \
      if ( !__simplifyWorkListInvariant )                                      \
        DOUT(                                                                  \
          "Simplify Worklist Invariant failed for register '" << r.getName() <<\
          "':" << endl );                                                      \
      if ( !__r.isVirtual() )                                                  \
        DOUT( "  " << __r.getName() << ".isVirtual() == false" << endl );      \
      if ( __r.isChild() )                                                     \
        DOUT(                                                                  \
          "  " << __r.getName() << ".isChild() == true" << endl );             \
      if ( !__igraph.containsNode( __r ) )                                     \
        DOUT(                                                                  \
          "  igraph.containsNode( " << __r.getName() << " ) == false" <<       \
          endl );                                                              \
      if ( __r != __igraph.getUnaliasedReg( __r ) )                            \
        DOUT(                                                                  \
          "  " << __r.getName() << " != unaliasedReg == " <<                   \
          __igraph.getUnaliasedReg( __r ).getName() << endl );                 \
      if ( isPrecolored( __r ) )                                               \
        DOUT( "  isPrecolored( " << __r.getName() << " ) == true" << endl );   \
      if ( isMoveRelated( __r ) )                                              \
        DOUT( "  isMoveRelated( " << __r.getName() << " ) == true" << endl );  \
      if ( __igraph.getDegree( __r ) >= __igraph.getAvailableColors() )        \
        DOUT(                                                                  \
          "  igraph.getDegree( " << __r.getName() << " ) == " <<               \
          __igraph.getDegree( __r ) << " >= " <<                               \
          __igraph.getAvailableColors() << endl );                             \
      ufAssert( __simplifyWorkListInvariant ); );                              \
  }

#define DEBUG_SPILL_COSTS_INVARIANT( __r, __workListSpill )                    \
  {                                                                            \
    DSTART( "WIR_GraphColoring.invariants" );                                  \
    DACTION(                                                                   \
      bool __spillCostsInvariant = ( workListSpill.count( __r ) == 1 );        \
      if ( !__spillCostsInvariant )                                            \
      DOUT(                                                                    \
        "Spill Costs Invariant failed for register '" << __r.getName() <<      \
        "'." << endl );                                                        \
      ufAssert( __spillCostsInvariant ); );                                    \
  }

#define DEBUG_COLORED_GRAPH_INVARIANT1( __igraph )                             \
  {                                                                            \
    DSTART( "WIR_GraphColoring.invariants" );                                  \
    DACTION( ufAssert( !__igraph.containsUncoloredNodes() ); );                \
  }

// The following macro tests the graph coloring invariant, i.e., asserts that
// any two neighboring nodes have different colors.
#define DEBUG_COLORED_GRAPH_INVARIANT2( __f, __igraph )                        \
  {                                                                            \
    DSTART( "WIR_GraphColoring.invariants" );                                  \
    DACTION(                                                                   \
      WIR_RegisterSet __checkedRegs;                                           \
                                                                               \
      for ( WIR_VirtualRegister &__r : __f.getVirtualRegisters() )             \
        if ( __igraph.containsNode( __r.getRoot() ) ) {                        \
          WIR_BaseRegister &__key = __igraph.getUnaliasedReg( __r.getRoot() ); \
                                                                               \
          if ( !__checkedRegs.count( __key ) ) {                               \
            set<unsigned int> __colors = __igraph.getColors( __key );          \
                                                                               \
            for ( WIR_BaseRegister &__n : __igraph.getNeighbors( __key ) )     \
              for ( unsigned int __c : __igraph.getColors( __n ) ) {           \
                if ( __colors.count( __c ) )                                   \
                  DOUT(                                                        \
                    "Colored Graph Invariant failed for nodes '" <<            \
                    __key.getName() << "' and '" << __n.getName() << "':" <<   \
                    endl << "  color " << __c <<                               \
                    " is used for both adjacent nodes." << endl );             \
                ufAssert( !__colors.count( __c ) );                            \
              }                                                                \
            __checkedRegs.insert( __key );                                     \
          }                                                                    \
        } );                                                                   \
  }

#define DEBUG_COLOR_MAP_INVARIANTS1( __r, __colorMap, __leafs, __igraph )      \
  {                                                                            \
    DSTART( "WIR_GraphColoring.invariants" );                                  \
    DACTION(                                                                   \
      bool __colorMapInvariant1 =                                              \
        ( __colorMap.size() == 0 ) || ( __colorMap.size() == __leafs.size() ); \
      if ( !__colorMapInvariant1 )                                             \
        DOUT(                                                                  \
          "Color Map Invariant 1 failed for register '" << __r.getName() <<    \
          "':" << endl << "  colorMap.size() == " << __colorMap.size() <<      \
          " != 0 and != " << __leafs.size() << endl );                         \
      ufAssert( __colorMapInvariant1 );                                        \
                                                                               \
      if ( !__igraph.isColored( __r ) ) {                                      \
        bool __colorMapInvariant2 = true;                                      \
        set<unsigned int> __possibleColors =                                   \
          __igraph.getPossibleColors( __r );                                   \
        for ( auto &[ __reg, __col ] : __colorMap ) {                          \
          if ( ( __col == 0 ) ||                                               \
               ( !__possibleColors.count( __col ) ) ) {                        \
            if ( __colorMapInvariant2 ) {                                      \
              DOUT(                                                            \
                "Color Map Invariant 2 failed for register '" <<               \
                __r.getName() << "':" << endl );                               \
              __colorMapInvariant2 = false;                                    \
            }                                                                  \
            if ( __col == 0 )                                                  \
              DOUT(                                                            \
                "  colorMap[ " << __reg.get().getName() << " ] = 0" << endl ); \
            if ( !__possibleColors.count( __col ) ) {                          \
              DOUT(                                                            \
                "  possibleColors.count( " << __col << " ) == 0" <<            \
                endl << "  possibleColors = {" );                              \
              for ( unsigned int __c : __possibleColors )                      \
                DOUT( " " << __c );                                            \
              DOUT( " }" << endl );                                            \
            }                                                                  \
          }                                                                    \
        }                                                                      \
        ufAssert( __colorMapInvariant2 );                                      \
      } );                                                                     \
  }

#define DEBUG_COLOR_MAP_INVARIANTS2( __r, __p, __colorMap, __igraph )          \
  {                                                                            \
    DSTART( "WIR_GraphColoring.invariants" );                                  \
    DACTION(                                                                   \
      auto &__vReg =                                                           \
        const_cast<WIR_VirtualRegister &>(                                     \
          dynamic_cast<const WIR_VirtualRegister &>( __r ) );                  \
      if ( __colorMap.count( __vReg ) ) {                                      \
        bool __colorMapInvariant3 =                                            \
          ( __colorMap[ __vReg ] == __igraph.getColorOfPhreg( __p ) );         \
        if ( !__colorMapInvariant3 )                                           \
          DOUT(                                                                \
            "Color Map Invariant 3 failed for reg '" << __r.getName() <<       \
            "' and phreg '" << __p.getName() << "':" << endl <<                \
            "  colorMap[ " << __r.getName() << " ] == " <<                     \
            __colorMap[ __vReg ] << " != igraph.getColorOfPhreg( " <<          \
            __p.getName() << " ) == " << __igraph.getColorOfPhreg( __p ) <<    \
            endl );                                                            \
        ufAssert( __colorMapInvariant3 );                                      \
      } );                                                                     \
  }

// This invariant verifies that the register mapping produced by this register
// allocator does not violate any pre-coloring constraints originally specified
// for a WIR function.
#define DEBUG_PRECOLOR_INVARIANT( __f, __registerMapping )                     \
  {                                                                            \
    DSTART( "WIR_GraphColoring.invariants" );                                  \
    DACTION(                                                                   \
      for ( WIR_VirtualRegister &__r : __f.getVirtualRegisters() )             \
        if ( __f.containsPrecolor( __r ) &&                                    \
             ( __registerMapping.find( __r.getID() ) !=                        \
               __registerMapping.end() ) ) {                                   \
          bool __precolorInvariant =                                           \
            ( __registerMapping.at( __r.getID() ).get() ==                     \
              __f.findPrecolor( __r ) );                                       \
          if ( !__precolorInvariant )                                          \
            DOUT(                                                              \
              "Precolor Invariant failed for reg '" << __r.getName() <<        \
              "': f.findPrecolor = " << __f.findPrecolor( __r ).getName() <<   \
              ", registerMapping = " <<                                        \
              __registerMapping.at( __r.getID() ).get().getName() << endl );   \
          ufAssert( __precolorInvariant );                                     \
        } );                                                                   \
  }

// This invariant verifies that all registers of a WIR function are actually
// physical registers. Virtual registers must no longer occur.
#define DEBUG_ALLOCATION_INVARIANT( __f )                                      \
  {                                                                            \
    DSTART( "WIR_GraphColoring.invariants" );                                  \
    DACTION(                                                                   \
      bool __allocationInvariant = __f.getVirtualRegisters().empty();          \
      if ( !__allocationInvariant ) {                                          \
        DOUT(                                                                  \
          "Found " << __f.getVirtualRegisters().size() <<                      \
          " remaining virtual registers" << endl );                            \
        assert( __allocationInvariant );                                       \
      } );                                                                     \
  }

#else

#define DEBUG_OP_INS(...)
#define DEBUG_OP_ADD(...)
#define DEBUG_OP_MOV(...)
#define DEBUG_OP_RM(...)

#define DEBUG_REG_ADD(...)
#define DEBUG_REG_MOV(...)
#define DEBUG_REG_RM(...)
#define DEBUG_REG_PUSH(...)
#define DEBUG_REG_POP(...)

#define DEBUG_MOVE_SETS_INVARIANT(...)
#define DEBUG_SPILL_WL_INVARIANT(...)
#define DEBUG_FREEZE_WL_INVARIANT(...)
#define DEBUG_FREEZE_WL_WEAK_INVARIANT(...)
#define DEBUG_SIMPLIFY_WL_INVARIANT(...)

#define DEBUG_SPILL_COSTS_INVARIANT(...)

#define DEBUG_COLORED_GRAPH_INVARIANT1(...)
#define DEBUG_COLORED_GRAPH_INVARIANT2(...)
#define DEBUG_COLOR_MAP_INVARIANTS1(...)
#define DEBUG_COLOR_MAP_INVARIANTS2(...)
#define DEBUG_PRECOLOR_INVARIANT(...)
#define DEBUG_ALLOCATION_INVARIANT(...)

#endif


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
WIR_GraphColoring::WIR_GraphColoring( WIR_System &s, bool verbosity ) :
  WIR_BitOpt { s },
  mCallerSavingStackSpace { 0 },
  mAdditionalStackSpace { 0 },
  mBBToBeAllocated { nullptr },
  mVerbosity { verbosity },
  mCoalescingBriggs { true },
  mCoalescingGeorge { true },
  mGenerateWebs { true },
  mBestOfThreeSpilling { false },
  mRematerialization { false },
  mCoalesceStackLocations { true },
  mSpillAll { false },
  mMarkSpillCode { false },
  mSpillHeuristicToApply { 0 },
  mMinimalTotalSpillCosts { 0 },
  mBestSpillHeuristic { 0 },
  mTotalSpillCosts { 0 }
{
  DSTART( "WIR_GraphColoring::WIR_GraphColoring(WIR_System&, bool)" );
};


/*
  Default constructor for compilation unit-level optimization.
*/
WIR_GraphColoring::WIR_GraphColoring( WIR_CompilationUnit &c, bool verbosity ) :
  WIR_BitOpt { c },
  mCallerSavingStackSpace { 0 },
  mAdditionalStackSpace { 0 },
  mBBToBeAllocated { nullptr },
  mVerbosity { verbosity },
  mCoalescingBriggs { true },
  mCoalescingGeorge { true },
  mGenerateWebs { true },
  mBestOfThreeSpilling { false },
  mRematerialization { false },
  mCoalesceStackLocations { true },
  mSpillAll { false },
  mMarkSpillCode { false },
  mSpillHeuristicToApply { 0 },
  mMinimalTotalSpillCosts { 0 },
  mBestSpillHeuristic { 0 },
  mTotalSpillCosts { 0 }
{
  DSTART( "WIR_GraphColoring::WIR_GraphColoring(WIR_CompilationUnit&, bool)" );
};


/*
  Default constructor for function-level optimization.
*/
WIR_GraphColoring::WIR_GraphColoring( WIR_Function &f, bool verbosity ) :
  WIR_BitOpt { f },
  mCallerSavingStackSpace { 0 },
  mAdditionalStackSpace { 0 },
  mBBToBeAllocated { nullptr },
  mVerbosity { verbosity },
  mCoalescingBriggs { true },
  mCoalescingGeorge { true },
  mGenerateWebs { true },
  mBestOfThreeSpilling { false },
  mRematerialization { false },
  mCoalesceStackLocations { true },
  mSpillAll { false },
  mMarkSpillCode { false },
  mSpillHeuristicToApply { 0 },
  mMinimalTotalSpillCosts { 0 },
  mBestSpillHeuristic { 0 },
  mTotalSpillCosts { 0 }
{
  DSTART( "WIR_GraphColoring::WIR_GraphColoring(WIR_Function&, bool)" );
};


/*
  Destructor.
*/
WIR_GraphColoring::~WIR_GraphColoring( void )
{
  DSTART( "virtual WIR_GraphColoring::~WIR_GraphColoring()" );
};


/*
  setCoalescingBriggs (de-) activates coalescing according to Briggs.
*/
void WIR_GraphColoring::setCoalescingBriggs( bool coalescingBriggs )
{
  DSTART( "void WIR_GraphColoring::setCoalescingBriggs(bool)" );

  mCoalescingBriggs = coalescingBriggs;
};


/*
  getCoalescingBriggs returns whether coalescing according to Briggs is
  activated.
*/
bool WIR_GraphColoring::getCoalescingBriggs( void ) const
{
  DSTART( "bool WIR_GraphColoring::getCoalescingBriggs() const" );

  return( mCoalescingBriggs );
};


/*
  setCoalescingGeorge (de-) activates coalescing according to George.
*/
void WIR_GraphColoring::setCoalescingGeorge( bool coalescingGeorge )
{
  DSTART( "void WIR_GraphColoring::setCoalescingGeorge(bool)" );

  mCoalescingGeorge = coalescingGeorge;
};


/*
  getCoalescingGeorge returns whether coalescing according to George is
  activated.
*/
bool WIR_GraphColoring::getCoalescingGeorge( void ) const
{
  DSTART( "bool WIR_GraphColoring::getCoalescingGeorge() const" );

  return( mCoalescingGeorge );
};


/*
  getCoalescing returns whether any kind of coalescing is activated.
*/
bool WIR_GraphColoring::getCoalescing( void ) const
{
  DSTART( "bool WIR_GraphColoring::getCoalescing() const" );

  return( mCoalescingBriggs || mCoalescingGeorge );
};


/*
  setGenerateWebs (de-) activates the splitting of virtual registers into webs.
*/
void WIR_GraphColoring::setGenerateWebs( bool makeWebs )
{
  DSTART( "void WIR_GraphColoring::setGenerateWebs(bool)" );

  mGenerateWebs = makeWebs;
};


/*
  getGenerateWebs returns whether splitting of virtual registers into webs is
  activated.
*/
bool WIR_GraphColoring::getGenerateWebs( void ) const
{
  DSTART( "bool WIR_GraphColoring::getGenerateWebs() const" );

  return( mGenerateWebs );
};


/*
  setBestOfThreeSpilling (de-) activates the best-of-three spilling heuristic
  proposed by Bernstein.
*/
void WIR_GraphColoring::setBestOfThreeSpilling( bool bestOf3 )
{
  DSTART( "void WIR_GraphColoring::setBestOfThreeSpilling(bool)" );

  mBestOfThreeSpilling = bestOf3;
};


/*
  getBestOfThreeSpilling returns whether the best-of-three spilling heuristic
  proposed by Bernstein is activated.
*/
bool WIR_GraphColoring::getBestOfThreeSpilling( void ) const
{
  DSTART( "bool WIR_GraphColoring::getBestOfThreeSpilling() const" );

  return( mBestOfThreeSpilling );
};


/*
  setRematerialization (de-) activates rematerialization.
*/
void WIR_GraphColoring::setRematerialization( bool rematerialize )
{
  DSTART( "void WIR_GraphColoring::setRematerialization(bool)" );

  mRematerialization = rematerialize;
};


/*
  getRematerialization returns whether rematerialization is activated.
*/
bool WIR_GraphColoring::getRematerialization( void ) const
{
  DSTART( "bool WIR_GraphColoring::getRematerialization() const" );

  return( mRematerialization );
};


/*
  setCoalesceStackLocations (de-) activates coalescing of stack locations for
  spills.
*/
void WIR_GraphColoring::setCoalesceStackLocations( bool coalesceStackLocations )
{
  DSTART( "void WIR_GraphColoring::setCoalesceStackLocations(bool)" );

  mCoalesceStackLocations = coalesceStackLocations;
};


/*
  getCoalesceStackLocations returns whether coalescing of stack locations for
  spills is activated.
*/
bool WIR_GraphColoring::getCoalesceStackLocations( void ) const
{
  DSTART( "bool WIR_GraphColoring::getCoalesceStackLocations() const" );

  return( mCoalesceStackLocations );
};


/*
  setSpillAll (de-) activates full spilling of all virtual registers.
*/
void WIR_GraphColoring::setSpillAll( bool spillAll )
{
  DSTART( "void WIR_GraphColoring::setSpillAll(bool)" );

  mSpillAll = spillAll;
};


/*
  getSpillAll returns whether full spilling of all virtual registers is
  activated.
*/
bool WIR_GraphColoring::getSpillAll( void ) const
{
  DSTART( "bool WIR_GraphColoring::getSpillAll() const" );

  return( mSpillAll );
};


/*
  setMarkSpillCode (de-) activates marking of all generated spill and
  rematerialization instructions with a dedicated comment.
*/
void WIR_GraphColoring::setMarkSpillCode( bool markSpill )
{
  DSTART( "void WIR_GraphColoring::setMarkSpillCode(bool)" );

  mMarkSpillCode = markSpill;
};


/*
  getMarkSpillCode returns whether all generated spill and rematerialization
  instructions shall be marked with a dedicated comment.
*/
bool WIR_GraphColoring::getMarkSpillCode( void ) const
{
  DSTART( "bool WIR_GraphColoring::getMarkSpillCode() const" );

  return( mMarkSpillCode );
};


/*
  getVerbosity returns whether verbose messages should be dumped.
*/
bool WIR_GraphColoring::getVerbosity( void ) const
{
  DSTART( "bool WIR_GraphColoring::getVerbosity() const" );

  return( mVerbosity );
};


//
// Protected class methods
//

/*
  runOptimization allocates registers in the given system.
*/
void WIR_GraphColoring::runOptimization( WIR_System &s )
{
  DSTART( "virtual void WIR_GraphColoring::runOptimization(WIR_System&)" );

  for ( WIR_CompilationUnit &c : s )
    runOptimization( c );
};


/*
  runOptimization allocates registers in the given compilation unit.
*/
void WIR_GraphColoring::runOptimization( WIR_CompilationUnit &c )
{
  DSTART(
    "virtual void WIR_GraphColoring::runOptimization(WIR_CompilationUnit&)" );

  for ( WIR_Function &f : c )
    runOptimization( f );
};


/*
  runOptimization allocates registers in the given function.

  This method implements procedure Main() from A. W. Appel, page 251:

  procedure Main()
    LivenessAnalysis()
    Build()
    MakeWorklist()
    repeat
      if simplifyWorklist != {} then Simplify()
      else if worklistMoves != {} then Coalesce()
      else if freezeWorklist != {} then Freeze()
      else if spillWorklist != {} then SelectSpill()
    until simplifyWorklist = {} and worklistMoves = {}
          and freezeWorklist = {} and spillWorklist = {}
    AssignColors()
    if spilledNodes != {} then
      RewriteProgram(spilledNodes)
      Main()
*/
void WIR_GraphColoring::runOptimization( WIR_Function &f )
{
  DSTART( "virtual void WIR_GraphColoring::runOptimization(WIR_Function&)" );

  DOUT( "Processing function '" << f.getName() << "'." << endl );

  mUncoloredActualSpills.clear();
  mAdditionalStackSpace = 0;
  mSpillLoads.clear();
  mSpillStores.clear();
  mRematerializedSpills.clear();
  mStackOffsetOfSpillInstruction.clear();
  mSpillClones.clear();
  mFunctionReturnMoves.clear();

  bool onceAgain = true;
  bool firstIteration = true;
  bool originalSpillAllSwitch = mSpillAll;

  // Make sure that all available phregs exist in the current function.
  mPhregs.clear();
  mPhregsForPrecoloringOnly.clear();
  mCallerSavedRegsAliveAcrossCall.clear();
  mDummyParameters.clear();
  createPhregs( f );

  precolorSpecialRegs( f );

  // Generate webs.
  if ( mGenerateWebs ) {
    WIR_Webs w( f );
    w.optimize();
  }

  // Calculate loop depths of basic blocks.
  computeLoopNestingDepths( f );

  // Variable pass is used in case that only partial allocation of those VREGs
  // given in mAllocationSequence shall be done. In the conventional case, the
  // main allocation loop below iterates until all VREGs are colored. This is
  // certainly not what we want when using mAllocationSequence. In this
  // particular situation, we only want to do two passes of the loop below: The
  // allocation of VREGs given in mAllocationSequence during the first pass, and
  // the allocation of spill VREGs that have been generated during this first
  // pass in a second pass.
  unsigned int pass = 0;
  if ( !mAllocationSequence.empty() )
    pass = 1;

  // Outermost optimization loop.
  do {

    // Do liveness analysis.
    WIR_LivenessAnalysis lta( f );
    lta.analyze();

    // Do some processor-specific initializations.
    initializationHook( f );

    // Set initial spill heuristic to be applied now to 0 (i.e., classical
    // Chaitin-style spilling) or to 1 (i.e., Bernstein's best-of-three
    // spilling).
    if ( !mBestOfThreeSpilling )
      mSpillHeuristicToApply = 0;
    else
      mSpillHeuristicToApply = 1;

    // Specify whether to do full spilling in this iteration of the optimization
    // loop or not.
    if ( !firstIteration )
      mSpillAll = false;

    // Before iterating over all existing spill heuristics, set the best
    // allocation found so far to the empty set.
    mMinimalTotalSpillCosts = numeric_limits<unsigned long long>::max();
    mBestSpillHeuristic = 0;
    bool firstRound = true;

    // Reset best information found for the various spill heuristics.
    mBestSpilledNodes.clear();
    mBestCoalescedMoves.clear();
    mBestRegisterMapping.clear();
    mBestSpillAliases.clear();
    mBestRematerializationInstructions.clear();

    // Optimization loop over all existing spill heuristics, i.e., classical
    // Chaitin-style spilling or Bernstein's best-of-three spilling.
    do {

      DACTION(
        if ( mBestOfThreeSpilling )
          DOUT(
            "Simplifying interference graph using Bernstein " <<
            mSpillHeuristicToApply << " spill heuristic." << endl ); );

      // Reset accumulated spill costs for this current spill heuristic.
      mTotalSpillCosts = 0;

      // Initialize map for precolored nodes.
      initPrecolors( f );

      // Translate list mAllocationSequence to a register set, if any.
      WIR_VirtualRegisterSet vregsToAllocate;

      if ( pass != 0 )
        for ( const WIR_VirtualRegister &r : mAllocationSequence )
          vregsToAllocate.insert( r.getRoot() );

      // Build the interference graph.
      WIR_InterferenceGraph igraph( mPhregs, f, vregsToAllocate, mVerbosity );
      build( f, igraph );

      // Set up work lists.
      makeWorkList( f, igraph );

      // Simplify interference graph.
      do {
        if ( !mWorkListPriority.empty() )
          simplifyPriority( igraph );
        else

        if ( !mWorkListSimplify.empty() )
          simplify( igraph );
        else

        if ( !mWorkListMoves.empty() )
          coalesce( igraph );
        else

        if ( !mWorkListFreeze.empty() )
          freeze( igraph );
        else

        if ( !mWorkListSpill.empty() )
          selectSpill( f, igraph );
      } while ( !mWorkListPriority.empty() || !mWorkListSimplify.empty() ||
                !mWorkListMoves.empty() || !mWorkListFreeze.empty() ||
                !mWorkListSpill.empty() );

      {
        DSTART(
          "virtual void WIR_GraphColoring::runOptimization(WIR_Function&).simplified.visualize" );
        DACTION( igraph.visualize(); );
      }
      {
        DSTART(
          "virtual void WIR_GraphColoring::runOptimization(WIR_Function&).simplified.visualizeFull" );
        DACTION( igraph.visualize( true ); );
      }

      DEBUG_COLORED_GRAPH_INVARIANT1( igraph );

      DOUT(
        "Achieved total potential spill costs: " << mTotalSpillCosts << endl );

      // Color the interference graph.
      assignColors( igraph );

      DOUT( "Achieved total actual spill costs: " << mTotalSpillCosts << endl );

      // Check whether we achieved less spill costs with the current spill
      // heuristic.
      if ( mBestOfThreeSpilling &&
           ( firstRound || ( mTotalSpillCosts < mMinimalTotalSpillCosts ) ) )
        saveBestSolution();
      if ( mCoalesceStackLocations &&
           ( firstRound || ( mTotalSpillCosts < mMinimalTotalSpillCosts ) ) )
        mBestIgraph = igraph;
      firstRound = false;

      {
        DSTART(
          "virtual void WIR_GraphColoring::runOptimization(WIR_Function&).colored.visualizeFull" );
        DACTION( igraph.visualize(); );
      }

      // Update spill heuristic for the next round, if any.
      switch ( mSpillHeuristicToApply ) {

        case 0: {
          // In case of Chaitin's classical spilling, we can exit this loop.
          break;
        }

        case 1:
        case 2:
        case 3: {
          // In case of Bernstein's best-of-three spilling, we now switch from
          // the current to the next spill heuristic.
          ++mSpillHeuristicToApply;

          if ( mSpillHeuristicToApply == 4 )
            // In case of Bernstein's best-of-three spilling, now restore the
            // best spills/coalescences/register mappings in order to rewrite
            // the WIR code afterwards.
            restoreBestSolution();

          break;
        }
      }

      DEBUG_COLORED_GRAPH_INVARIANT2( f, igraph );

    } while ( ( mSpillHeuristicToApply > 0 ) &&
              ( mSpillHeuristicToApply < 4 ) );

    if ( mSpilledNodes.empty() )
      onceAgain = false;

    DEBUG_PRECOLOR_INVARIANT( f, mRegisterMapping );

    // Free some no longer needed memory.
    f.eraseContainers( WIR_LiveOut::getContainerTypeID(), true );

    // Finally, transform the WIR code.
    rewriteProgram( f );

    // Do some processor-specific things after WIR code transformation.
    rewriteProgramHook( f );

    firstIteration = false;

    if ( pass == 1 ) {
      // We have allocated all VREGs in mAllocationSequence during this
      // iteration of the main allocation loop. So, let's clear
      // mAllocationSequence and allocate all potentially inserted spill-clone
      // registers in the next iteration.
      pass = 2;
      mAllocationSequence.clear();

      // Collect all VREGs remaining in the currently considered basic blocks
      // (i.e., all spill-clones) and allocate them in the second round.
      auto vRegs = mBBToBeAllocated->getVREGs();
      copy( vRegs.begin(), vRegs.end(), back_inserter( mAllocationSequence ) );

      onceAgain = !mAllocationSequence.empty();
    } else

    if ( pass == 2 ) {
      mUncoloredActualSpills.clear();
      onceAgain = false;
    }

    // Remove unused virtual registers.
    WIR_UnusedVRegs unusedVRegs( f );
    unusedVRegs.optimize();

  } while ( onceAgain );

  // Restore original spill-all configuration switch.
  mSpillAll = originalSpillAllSwitch;

  // After the above allocation loop, some registers might not be allocated.
  // This is done right now, finally.
  mSpillClones.clear();
  allocateUncoloredActualSpills( f );

  // Apply caller-saving of registers alive across function calls.
  applyCallerSavingPolicy( f );

  // Do some final processor-specific post-processing.
  postProcessingHook( f );

  // Remove all artificially inserted dummy parameters of call instructions.
  while ( !mDummyParameters.empty() ) {
    WIR_Parameter &p = mDummyParameters.front();
    mDummyParameters.pop_front();

    WIR_Operation &o = p.getOperation();
    o.eraseParameter( o.findParameter( p ) );
  }

  mCallerSavedRegsAliveAcrossCall.clear();

  // After all allocation and spilling is done, finally adjust the stack frame.
  adjustStack( f );

  // Remove unused virtual registers.
  WIR_UnusedVRegs unusedVRegs( f );
  unusedVRegs.optimize();

  // Remove unnecessary spill code.
  removeRedundantSpills();

  // Clean up after register allocation, if necessary.
  postRACleanup( f );
  mFunctionReturnMoves.clear();

  DEBUG_ALLOCATION_INVARIANT( f );
};


/*
  saveBestSolutionHook allows to save processor-specific allocation data in the
  course of Bernstein's best-of-three spilling heuristic.

  Since this task is processor-specific and might or might not be necessary for
  some actual processor's EABI, this method is virtual and can be overloaded if
  required.
*/
void WIR_GraphColoring::saveBestSolutionHook( void )
{
  DSTART( "virtual void WIR_GraphColoring::saveBestSolutionHook()" );
};


/*
  restoreBestSolutionHook allows to restore processor-specific allocation data
  in the course of Bernstein's best-of-three spilling heuristic.

  Since this task is processor-specific and might or might not be necessary for
  some actual processor's EABI, this method is virtual and can be overloaded if
  required.
*/
void WIR_GraphColoring::restoreBestSolutionHook( void )
{
  DSTART( "virtual void WIR_GraphColoring::restoreBestSolutionHook()" );
};


/*
  initializationHook allows to perform processor-specific actions before doing
  some actual coloring or spilling.

  Since this task is processor-specific and might or might not be necessary for
  some actual processor's EABI, this method is virtual and can be overloaded if
  required.
*/
void WIR_GraphColoring::initializationHook( WIR_Function &f )
{
  DSTART( "virtual void WIR_GraphColoring::initializationHook(WIR_Function&)" );

  (void) f;
};


/*
  isStackPointer returns whether the specified %WIR register is a system's stack
  pointer.
*/
bool WIR_GraphColoring::isStackPointer( const WIR_BaseRegister &r ) const
{
  DSTART(
    "bool WIR_GraphColoring::isStackPointer(const WIR_BaseRegister&) const" );

  if ( r.isVirtual() ) {
    auto &vr = dynamic_cast<const WIR_VirtualRegister &>( r );

    if ( vr.isPrecolored() )
      return( vr.getPrecolor().isStackPointer() );
  } else {
    auto &pr = dynamic_cast<const WIR_PhysicalRegister &>( r );

    return( pr.isStackPointer() );
  }

  return( false );
};


/*
  isPrecolored checks map mPrecolored whether the specified WIR register is
  pre-colored.
*/
bool WIR_GraphColoring::isPrecolored( const WIR_BaseRegister &r ) const
{
  DSTART(
    "bool WIR_GraphColoring::isPrecolored(const WIR_BaseRegister&) const" );

  return(
    mPrecolored.find(
      const_cast<WIR_BaseRegister &>( r ) ) != mPrecolored.end() );
};


/*
  isPriorityRegister returns whether a given register has high priority for
  color assignment.

  Since the notion of high-priority registers is processor-specific, this method
  is virtual so that it can be overloaded.
*/
bool WIR_GraphColoring::isPriorityRegister( const WIR_VirtualRegister &r ) const
{
  DSTART(
    "virtual bool WIR_GraphColoring::isPriorityRegister(const WIR_VirtualRegister&) const" );

  (void) r;

  return( false );
};


/*
  computeSpillCosts computes the spill costs of all registers in the given set
  and stores them in map mSpillCosts.

  The cost of spilling a register r is taken to be

    defwt * \\sum_{def \\in r} 10^depth(def) +
    usewt * \\sum_{use \\in r} 10^depth(use) -
    copywt * \\sum{copy \\in r} 10^depth(copy)

  where def, use, and copy are individual definitions, uses, and register copies
  of the register r; defwt, usewt, and copywt are relative weights assigned to
  the instruction types.

  Computing spill costs should take the following into account:

  1. If a register's value can be more efficiently recomputed than reloaded, the
     cost of recomputing it is used instead.
  2. If the source or target of a copy instruction is spilled, the instruction
     is no longer needed.
  3. If a spilled value is used several times in the same basic block and the
     restored value remains live until the last use of the spilled value in that
     block, then only a single load of the value is needed in the block.

  This method implements procedure Compute_Spill_Costs() from S. S. Muchnick,
  page 502. The structure of computeSpillCosts corresponds to that of
  insertSpillCode for obvious reasons. Since for some processors, some other
  methods for spill cost computation might be desired, this method is virtual so
  that it can be overloaded.
*/
void WIR_GraphColoring::computeSpillCosts( const WIR_Function &f,
                                           const WIR_InterferenceGraph &igraph,
                                           const WIR_VirtualRegisterSet &workListSpill,
                                           WIR_RematerializationMap &rematInsns )
{
  DSTART(
    "virtual void WIR_GraphColoring::computeSpillCosts(const WIR_Function&, const WIR_InterferenceGraph&, const WIR_VirtualRegisterSet&, WIR_GraphColoring::WIR_RematerializationMap&)" );

  mSpillCosts.clear();

  map<WIR_id_t, WIR_BasicBlockSet> mapSpillCandidate2BBs;
  map<WIR_id_t, list<std::reference_wrapper<WIR_RegisterParameter>>> parameterOrderOfSpillCandidates;
  WIR_OperationSet movesToBeDeleted;

  for ( WIR_BasicBlock &b : f ) {
    map<WIR_id_t, bool> stillLiveInSameBB;
    unsigned int nestingDepth = getLoopNestingDepth( b );

    for ( WIR_Instruction &i : b ) {
      WIR_VirtualRegisterSet trueLivenessInSameBB;
      WIR_VirtualRegisterSet falseLivenessInSameBB;

      for ( WIR_Operation &o : i ) {
        for ( WIR_Parameter &p : o ) {
          if ( ( p.getType() == WIR_ParameterType::reg ) &&
               dynamic_cast<WIR_RegisterParameter &>( p ).getRegister().isVirtual() ) {
            auto &rp = dynamic_cast<WIR_RegisterParameter &>( p );
            auto &r = dynamic_cast<WIR_VirtualRegister &>( rp.getRegister() );

            // Check whether r or its unaliased register partner appears in
            // workListSpill;
            if ( igraph.containsNode( r.getRoot() ) ) {
              auto &key = igraph.getUnaliasedReg( r.getRoot() );

              if ( key.isVirtual() &&
                   workListSpill.count(
                     dynamic_cast<WIR_VirtualRegister &>( key ) ) &&
                   !mCoalescedMoves.count( o ) ) {
                // The current parameter p refers to a spilled register.
                mapSpillCandidate2BBs[ key.getID() ].insert( b );
                parameterOrderOfSpillCandidates[ key.getID() ].push_back( rp );

                if ( o.isMove() && !movesToBeDeleted.count( o ) &&
                     !isStackPointer( getUseOfMove( o ) ) &&
                     !isStackPointer( getDefOfMove( o ) ) ) {

                  // The current operation is a move where some parameter is
                  // spilled.

                  auto &use = getUseOfMove( o );
                  auto &def = getDefOfMove( o );

                  if ( use.isVirtual() && def.isVirtual() && ( def != use ) &&
                       workListSpill.count(
                         dynamic_cast<WIR_VirtualRegister &>(
                           use ).getRoot() ) &&
                       workListSpill.count(
                         dynamic_cast<WIR_VirtualRegister &>(
                           def ).getRoot() ) ) {
                    // This is a move where both operand registers are spilled.
                    // Such a move
                    //
                    //   MOV def, use;
                    //
                    // thus becomes:
                    //
                    //   LD clone, <mem[ use ]>;
                    //   ST <mem[ def ]>, clone;

                    // cppcheck-suppress variableScope
                    DACTION(
                      BBPOS( o );
                      DOUT(
                        endl <<
                        "Spill-move scenario (1), both operands of move " <<
                        "spilled. " << BBID << o << endl ); );

                    if ( !stillLiveInSameBB.count( use.getID() ) ||
                         ( stillLiveInSameBB[ use.getID() ] == false ) ) {
                      DOUT(
                        "Counting spill-load for used register '" <<
                        use.getName() << "' before the move." << endl <<
                        "Marking move for deletion." << endl );

                      computeSpillLoadCosts(
                        rp, key, nestingDepth, rematInsns, movesToBeDeleted,
                        true );
                    }

                    DOUT(
                      "Counting spill-store for defined register '" <<
                      def.getName() << "' after the move." << endl );

                    computeSpillStoreCosts(
                      rp, key, nestingDepth, movesToBeDeleted );

                    // After this current move, previous values of def, its
                    // childs and its parents are no more alive.
                    propagateFalseLiveness(
                      dynamic_cast<WIR_VirtualRegister &>( def ), b,
                      falseLivenessInSameBB );
                  } else

                  if ( r == use ) {
                    // This is a move where the used operand is spilled.
                    // cppcheck-suppress variableScope
                    DACTION(
                      BBPOS( o );
                      DOUT(
                        endl <<
                        "Spill-move scenario (2), used operand '" <<
                        r.getName() << "' of move spilled. " << BBID << o <<
                        endl ); );

                    if ( !stillLiveInSameBB.count( r.getID() ) ||
                         ( stillLiveInSameBB[ r.getID() ] == false ) ) {
                      // The current parameter is used in a move operation. We
                      // generate a spill-load right before the move which loads
                      // the value from r's position on the stack right into the
                      // def-register of the move.
                      //
                      //   MOV def, r;
                      //
                      // would naively become:
                      //
                      //   LD r, mem<mStackByteOffset[ r ]>;
                      //   MOV def, r;
                      //
                      // which can be optimized to:
                      //
                      //   LD def, mem<mStackByteOffset[ r ]>;
                      DOUT(
                        "Counting spill-load for used register '" <<
                        r.getName() << "' before the move." << endl <<
                        "Marking move for deletion." << endl );

                      computeSpillLoadCosts(
                        rp, key, nestingDepth, rematInsns, movesToBeDeleted,
                        true );
                    }
                  } else {
                    // This is a move where the defined operand is spilled.
                    // cppcheck-suppress variableScope
                    DACTION(
                      BBPOS( o );
                      DOUT(
                        endl <<
                        "Spill-move scenario (3), defined operand '" <<
                        r.getName() << "' of move spilled. " << BBID << o <<
                        endl ); );

                    // The current parameter is defined by a move operation. We
                    // generate a spill-store right after the move which stores
                    // the value of the use-register of the move to r's position
                    // on the stack.
                    //
                    //   MOV r, use;
                    //
                    // would naively become:
                    //
                    //   MOV r, use;
                    //   ST r, mem<mStackByteOffset[ r ]>;
                    //
                    // which can be optimized to:
                    //
                    //   ST use, mem<mStackByteOffset[ r ]>;

                    DOUT(
                      "Counting spill-store for defined register '" <<
                      r.getName() << "' after the move." << endl <<
                      "Marking move for deletion." << endl );

                    computeSpillStoreCosts(
                      rp, key, nestingDepth, movesToBeDeleted, true );

                    // After this current definition, previous values of r, its
                    // childs and its parents are no more alive.
                    propagateFalseLiveness( r, b, falseLivenessInSameBB );
                  }

                } else

                if ( !movesToBeDeleted.count( o ) ) {

                  // The current operation is a non-move where some parameter is
                  // spilled.

                  if ( rp.isUsed() ) {
                    // This is a non-move where a used operand is spilled.

                    // cppcheck-suppress variableScope
                    DACTION(
                      BBPOS( o );
                      DOUT(
                        endl << "Spill scenario (1), used operand '" <<
                        r.getName() << "' spilled. " << BBID << o << endl ) );

                    // The current parameter is used. We count a spill-load
                    // right before the current operation. However, if a spilled
                    // value is used several times in the very same basic block
                    // and the restored value remains live until the last use of
                    // the spill in that block, then only a single load of the
                    // value is needed in the block (cf. S. S. Muchnik, page
                    // 501). So, if r is still alive in the same basic block, we
                    // do not need to count another spill-load.
                    if ( !stillLiveInSameBB.count( r.getID() ) ||
                         ( stillLiveInSameBB[ r.getID() ] == false ) ) {
                      // If the register is not alive in the current basic block
                      // b, we count a spill-load.
                      DOUT(
                        "Counting spill-load for used register '" <<
                        r.getName() << "' before the operation." << endl );

                      computeSpillLoadCosts(
                        rp, key, nestingDepth, rematInsns, movesToBeDeleted );

                      // The register and all its child stay alive after the
                      // current use.
                      propagateTrueLiveness( r, b, trueLivenessInSameBB );
                    }
                  } else

                  if ( rp.isDefined() ) {
                    // This is a non-move where a defined operand is spilled.

                    // cppcheck-suppress variableScope
                    DACTION(
                      BBPOS( o );
                      DOUT(
                        endl << "Spill scenario (2), defined operand '" <<
                        r.getName() << "' spilled. " << BBID << o << endl ) );

                    // The current parameter is defined. We count a spill-store
                    // right after the current operation.

                    DOUT(
                      "Counting spill-store for defined register '" <<
                      r.getName() << "' after the operation." << endl );

                    computeSpillStoreCosts(
                      rp, key, nestingDepth, movesToBeDeleted );

                    // After this current definition, previous values of r, its
                    // childs and its parents are no more alive.
                    propagateFalseLiveness( r, b, falseLivenessInSameBB );
                  } else {
                    // This is a non-move where a def-used operand is spilled.

                    // cppcheck-suppress variableScope
                    DACTION(
                      BBPOS( o );
                      DOUT(
                        endl << "Spill scenario (3), def-used operand '" <<
                        r.getName() << "' spilled. " << BBID << o << endl ) );

                    // The current parameter is def-used. In principle, we have
                    // to count a spill-load before and a spill-load after the
                    // current operation.
                    // However, if r is still alive in the same basic block, we
                    // can omit counting the spill-load.

                    if ( !stillLiveInSameBB.count( r.getID() ) ||
                         ( stillLiveInSameBB[ r.getID() ] == false ) ) {
                      // If the register is not alive in the current basic block
                      // b, we count a spill-load.

                      DOUT(
                        "Counting spill-load for def-used register '" <<
                        r.getName() << "' before the operation." << endl );

                      computeSpillLoadCosts(
                        rp, key, nestingDepth, rematInsns, movesToBeDeleted );
                    }

                    DOUT(
                      "Counting spill-store for def-used register '" <<
                      r.getName() << "' after the operation." << endl );

                    computeSpillStoreCosts(
                      rp, key, nestingDepth, movesToBeDeleted );

                    // After this current def-use, previous values of r, its
                    // childs and its parents are no more alive.
                    propagateFalseLiveness( r, b, falseLivenessInSameBB );
                  }
                }
              }
            }
          }
        }
      }

      // Update whether register are still alive or not in this same basic block
      // after this current instruction i.
      for ( const auto &r : trueLivenessInSameBB )
        updateLivenessInSameBB( r, true, stillLiveInSameBB, igraph );
      for ( const auto &r : falseLivenessInSameBB )
        updateLivenessInSameBB( r, false, stillLiveInSameBB, igraph );
    }
  }

  // After spill costs are determined, we finally check whether a register in
  // workListSpill is defined and used in only one single basic block, and
  // whether no computation goes dead between its definition and its last use.
  // In this case, spilling this register cannot help to make the program
  // colorable. We therefore set the costs of spilling this register to
  // infinity. This keeps our algorithm from spilling computations that have
  // already been spilled (cf. G. J. Chaitin, page 70).
  for ( const WIR_VirtualRegister &spillCandidate : workListSpill )
    if ( mapSpillCandidate2BBs[ spillCandidate.getID() ].size() == 1 ) {
      bool firstParamIsDef = false;
      bool followingParamsAreUses = true;
      bool firstParam = true;

      for ( const WIR_RegisterParameter &rp :
              parameterOrderOfSpillCandidates[ spillCandidate.getID() ] ) {
        if ( firstParam ) {
          firstParam = false;

          if ( rp.isDefined() )
            firstParamIsDef = true;
        } else {
          if ( rp.isDefined() )
            followingParamsAreUses = false;
        }
      }

      if ( firstParamIsDef && followingParamsAreUses )
        // We found a register which occurs in exactly one basic block and which
        // is defined first and then only used.
        mSpillCosts[ spillCandidate.getID() ] =
          numeric_limits<unsigned int>::max();
    }
};


/*
  selectSpillCandidate selects one register from workListSpill as a candidate
  for spilling, using either Chaitin's or Bernstein's spilling heuristics.

  Since for some processors, some other spilling heuristics might be desired,
  this method is virtual so that it can be overloaded.
*/
WIR_VirtualRegister &WIR_GraphColoring::selectSpillCandidate( const WIR_InterferenceGraph &igraph,
                                                              const WIR_VirtualRegisterSet &workListSpill ) const
{
  DSTART(
    "virtual WIR_VirtualRegister& WIR_GraphColoring::selectSpillCandidate(const WIR_InterferenceGraph&, const WIR_VirtualRegisterSet&) const" );

  WIR_VirtualRegister *spillCandidate = nullptr;
  long double minSpillCosts = 0.0L;

  if ( mAllocationSequence.empty() )
    // If no precedence of VREGs is given in list mAllocationSequence, scan
    // workListSpill and search for the register with least spill costs.
    for ( WIR_VirtualRegister &r : workListSpill ) {
      DEBUG_SPILL_COSTS_INVARIANT( r, workListSpill );
      DEBUG_SPILL_WL_INVARIANT( r, igraph );
      ufAssert( mSpillCosts.find( r.getID() ) != mSpillCosts.end() );

      long double spillCosts = 0.0L;

      switch ( mSpillHeuristicToApply ) {
        case 0: {
          spillCosts = spillCostsChaitin( r, igraph );
          break;
        }

        case 1: {
          spillCosts = spillCostsBernstein1( r, igraph );
          break;
        }

        case 2: {
          spillCosts = spillCostsBernstein2( r, igraph );
          break;
        }

        case 3: {
          spillCosts = spillCostsBernstein3( r, igraph );
          break;
        }
      }

      if ( !spillCandidate || ( spillCosts < minSpillCosts ) ) {
        spillCandidate = &r;
        minSpillCosts = spillCosts;
      }
    }
  else
    // Some precedence of VREGs is given in list mAllocationSequence. Thus, we
    // traverse mAllocationSequence backwards and determine that member of set
    // mWorkListSpill that is closest to the tail of mAllocationSequence.
    for ( auto it = mAllocationSequence.rbegin();
          it != mAllocationSequence.rend(); ++it )
      if ( workListSpill.count( it->get().getRoot() ) ) {
        spillCandidate = &( it->get().getRoot() );
        break;
      }

  DACTION(
    ostringstream sstr;
    if ( mAllocationSequence.empty() )
      sstr << "' with spill costs " << minSpillCosts;
    else
      sstr << "' from list mAllocationSequence";

    DOUT(
      "Selecting register '" << spillCandidate->getName() << sstr.str() <<
      " as potential spill." << endl ); );

  // The following assertion is required for the clang static analyzer.
  ufAssert( spillCandidate != nullptr );

  return( *spillCandidate );
};


/*
  spillCostsChaitin returns the spill costs for the specified register according
  to Chaitin's rule.
*/
long double WIR_GraphColoring::spillCostsChaitin( const WIR_VirtualRegister &r,
                                                  const WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "long double WIR_GraphColoring::spillCostsChaitin(const WIR_VirtualRegister&, const WIR_InterferenceGraph&) const" );

  long double spillCosts;
  unsigned int costs = mSpillCosts.at( r.getID() );

  // Chaitin suggests choosing that node with the smallest ratio of spill costs
  // divided by current degree (cf. P. Briggs, page 19, equation (2.1)):
  if ( costs == numeric_limits<unsigned int>::max() )
    spillCosts = numeric_limits<long double>::infinity();
  else
    spillCosts =
      ( (long double) costs ) / ( (long double) igraph.getDegree( r ) );

  DOUT(
    "Chaitin spillCosts( " << r.getName() << " ) = " << costs << " / " <<
    igraph.getDegree( r ) << " = " << spillCosts << endl );

  return( spillCosts );
};


/*
  spillCostsBernstein1 returns the spill costs for the specified register
  according to Bernstein's first rule.
*/
long double WIR_GraphColoring::spillCostsBernstein1( const WIR_VirtualRegister &r,
                                                     const WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "long double WIR_GraphColoring::spillCostsBernstein1(const WIR_VirtualRegister&, const WIR_InterferenceGraph&) const" );

  long double spillCosts;
  unsigned int costs = mSpillCosts.at( r.getID() );

  // Bernstein et al. suggest to choose that node with the smallest ratio of
  // spill costs divided by the current degree raised to the power of 2, cf. P.
  // Briggs, page 19, equation (2.2), or S. S. Muchnick, page 521.
  if ( costs == numeric_limits<unsigned int>::max() )
    spillCosts = numeric_limits<long double>::infinity();
  else
    spillCosts =
      ( (long double) costs ) /
      ( (long double) pow( igraph.getDegree( r ), 2 ) );

  DOUT(
    "Bernstein (1) spillCosts( " << r.getName() << " ) = " << costs << " / " <<
    igraph.getDegree( r ) << "² = " << spillCosts << endl );

  return( spillCosts );
};


/*
  spillCostsBernstein1 returns the spill costs for the specified register
  according to Bernstein's second rule.
*/
long double WIR_GraphColoring::spillCostsBernstein2( const WIR_VirtualRegister &r,
                                                     const WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "long double WIR_GraphColoring::spillCostsBernstein2(const WIR_VirtualRegister&, const WIR_InterferenceGraph&) const" );

  long double spillCosts;
  unsigned int costs = mSpillCosts.at( r.getID() );

  // Bernstein et al. suggest to choose that node with the smallest ratio of
  // spill costs divided by area times current degree, cf. P. Briggs, page 19,
  // equation (2.3), or S. S. Muchnick, page 523.
  if ( costs == numeric_limits<unsigned int>::max() )
    spillCosts = numeric_limits<long double>::infinity();
  else
    spillCosts =
      ( (long double) costs ) /
      ( (long double) ( area( r, igraph ) * igraph.getDegree( r ) ) );

  DOUT(
    "Bernstein (2) spillCosts( " << r.getName() << " ) = " << costs <<
    " / ( " << area( r, igraph ) << " * " << igraph.getDegree( r ) << " ) = " <<
    spillCosts << endl );

  return( spillCosts );
};


/*
  spillCostsBernstein3 returns the spill costs for the specified register
  according to Bernstein's third rule.
*/
long double WIR_GraphColoring::spillCostsBernstein3( const WIR_VirtualRegister &r,
                                                     const WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "long double WIR_GraphColoring::spillCostsBernstein3(const WIR_VirtualRegister&, const WIR_InterferenceGraph&) const" );

  long double spillCosts;
  unsigned int costs = mSpillCosts.at( r.getID() );

  // Bernstein et al. suggest to choose that node with the smallest ratio of
  // spill costs divided by area times current degree raised to the power of 2,
  // cf. P. Briggs, page 19, equation (2.4), or S. S. Muchnick, page 523.
  if ( costs == numeric_limits<unsigned int>::max() )
    spillCosts = numeric_limits<long double>::infinity();
  else
    spillCosts =
      ( (long double) costs ) /
      ( (long double) ( area( r, igraph ) * pow( igraph.getDegree( r ), 2 ) ) );

  DOUT(
    "Bernstein (3) spillCosts( " << r.getName() << " ) = " << costs <<
    " / ( " << area( r, igraph ) << " * " << igraph.getDegree( r ) <<
    "² ) = " << spillCosts << endl );

  return( spillCosts );
};


/*
  area represents a spill cost measure attempting to quantify the impact a
  register has on live ranges throughout a WIR function.
*/
unsigned int WIR_GraphColoring::area( const WIR_VirtualRegister &r,
                                      const WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "unsigned int WIR_GraphColoring::area(const WIR_VirtualRegister&, const WIR_InterferenceGraph&) const" );

  unsigned int res = 0;
  WIR_OperationSet operationsToConsider;

  // In order to compute the area measure, we also have to consider coalesced
  // alias registers.
  WIR_RegisterSet aliases = igraph.getCoalescedAliases( r );
  aliases.insert( const_cast<WIR_VirtualRegister &>( r ) );

  for ( WIR_BasicBlock &b : r.getFunction() )
    for ( WIR_Instruction &i : b )
      for ( WIR_Operation &o : i )
        for ( const WIR_Parameter &p : o )
          if ( ( p.getType() == WIR_ParameterType::reg ) &&
               aliases.count(
                 dynamic_cast<const WIR_RegisterParameter &>(
                   p ).getRegister() ) )
            operationsToConsider.insert( o );

  // Now do the actual area computation using all collected operations.
  for ( WIR_Operation &o : operationsToConsider ) {
    WIR_Instruction &i = o.getInstruction();

    // Get live-out set.
    auto &ltaContainer = i.getContainers<WIR_LiveOut>().begin()->get();
    auto &live = ltaContainer.getRegisters();

    res += live.size() * pow( 5.0, getLoopNestingDepth( i.getBasicBlock() ) );
  }

  return( res );
};


/*
  selectCoalescingCandidate selects one register from workListMoves as a
  candidate for coalescing, using some favorite coalescing heuristic.

  This method implements the heuristic to select a move operation located in
  that basic block with the maximal loop nesting depth (cf. P. Briggs, page
  104). Since for some processor architectures, other heuristics might be
  desired, this method is virtual so that it can be overloaded.
*/
WIR_Operation &WIR_GraphColoring::selectCoalescingCandidate( const WIR_InterferenceGraph &igraph,
                                                             const WIR_OperationSet &workListMoves ) const
{
  DSTART(
    "virtual WIR_Operation& WIR_GraphColoring::selectCoalescingCandidate(const WIR_InterferenceGraph&, const WIR_OperationSet&) const" );

  (void) igraph;

  WIR_Operation *coalescingCandidate = nullptr;
  unsigned int loopNestingDepth = 0;

  // Scan workListMoves and search for the move with highest loop nesting depth.
  for ( WIR_Operation &o : workListMoves ) {
    unsigned int d = getLoopNestingDepth( o.getInstruction().getBasicBlock() );

    if ( ( coalescingCandidate == nullptr ) || ( d > loopNestingDepth ) ) {
      coalescingCandidate = &o;
      loopNestingDepth = d;
    }
  }

  // The following assertion is required for the clang static analyzer.
  ufAssert( coalescingCandidate != nullptr );

  return( *coalescingCandidate );
};


/*
  avoidCoalescing checks whether the two given registers which are both move-
  related must not be coalesced due to processor-specific reasons.

  In the present default implementation, coalescing will be avoided if
  - the move operation stores the result of a function call somewhere, and
  - the move immediately follows a call instruction,

  or if
  - one of the two registers is pre-colored and the other not, and
  - the pre-colored register is a caller-saved register, and
  - the non pre-colored register is alive across a function call.

  In the first case, the move is used to store away a result of a function call.
  We keep these explicit moves since otherwise, it would cause us trouble when
  implementing the saving/restoring polity of caller-saved registers later.

  If we would allow coalescing in the second case, the non pre-colored register
  will be allocated to the caller-saved register specified in the pre-color.
  However, since it is alive across a function call, caller-saving stack loads
  and stores must be added which is bad. Instead of inserting these costly
  memory loads and stores, we simply keep the move instruction and do not
  coalesce.

  Since there might be additional, processor-specific scenarios where coalescing
  should be avoided, this method is virtual so that it can be overloaded.
*/
bool WIR_GraphColoring::avoidCoalescing( const WIR_Operation &o,
                                         const WIR_BaseRegister &r1,
                                         const WIR_BaseRegister &r2,
                                         const WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "virtual bool WIR_GraphColoring::avoidCoalescing(const WIR_Operation&, "
    "const WIR_BaseRegister&, const WIR_BaseRegister&, const "
    "WIR_InterferenceGraph&) const" );

  // Check first case.
  if ( isFunctionReturnMove( o ) )
    return( true );

  // Check second case.
  if ( ( isPrecolored( r1 ) && isPrecolored( r2 ) ) ||
       ( !isPrecolored( r1 ) && !isPrecolored( r2 ) ) )
    return( false );

  // From here on, one register is precolored, the other not. Determine the
  // precolor and the uncolored register next.
  const WIR_PhysicalRegister *phreg = nullptr;
  WIR_BaseRegister *uncoloredVreg = nullptr;

  if ( isPrecolored( r1 ) ) {
    auto it = mPrecolored.find( const_cast<WIR_BaseRegister &>( r1 ) );
    phreg = &(it->second.get());
    uncoloredVreg = const_cast<WIR_BaseRegister *>( &r2 );
  } else {
    auto it = mPrecolored.find( const_cast<WIR_BaseRegister &>( r2 ) );
    phreg = &(it->second.get());
    uncoloredVreg = const_cast<WIR_BaseRegister *>( &r1 );
  }

  if ( isCallerSaved( *phreg ) ) {
    // Retrieve all coalescing aliases of the uncolored virtual register and its
    // root.
    WIR_RegisterSet aliases = igraph.getCoalescedAliases( *uncoloredVreg );
    aliases.insert( *uncoloredVreg );
    WIR_RegisterSet rootAliases =
      igraph.getCoalescedAliases( uncoloredVreg->getRoot() );
    aliases.insert( rootAliases.begin(), rootAliases.end() );
    // for ( WIR_BaseRegister &r : rootAliases )
    //   aliases.insert( r );
    aliases.insert( uncoloredVreg->getRoot() );

    // If a caller-saved register is used for pre-coloring, we have to check
    // whether the uncolored virtual register is live across some function call.
    for ( auto &[ call, liveVregs ] : mVregsAliveAcrossCall )
      for ( WIR_BaseRegister &r : aliases )
        if ( r.isVirtual() &&
             liveVregs.count( dynamic_cast<WIR_VirtualRegister &>( r ) ) )
          // The current virtual register is live across the call.
          return( true );
  }

  return( false );
};


/*
  getRematerializationCosts returns the costs of one single recomputation of the
  specified used parameter.

  Since the determination of rematerialization costs is processor-specific, this
  method is virtual so that it can be overloaded. The method of this class is
  just a dummy always returning maxInt.
*/
unsigned int WIR_GraphColoring::getRematerializationCosts( const WIR_RegisterParameter &p ) const
{
  DSTART(
    "virtual unsigned int WIR_GraphColoring::getRematerializationCosts(const WIR_RegisterParameter&) const" );

  (void) p;

  return( numeric_limits<unsigned int>::max() );
};


/*
  getRematerializationInstructions returns a list of machine instructions for
  one single recomputation of the specified used parameter.

  Since the determination of rematerialization code is processor-specific, this
  method is virtual so that it can be overloaded. The method of this class is
  just a dummy always returning an empty list of rematerialization instructions.
*/
std::list<WIR_Instruction *> WIR_GraphColoring::getRematerializationInstructions( const WIR_RegisterParameter &p ) const
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  (void) p;

  return( list<WIR_Instruction *> {} );
};



/*
  resolveSpillCoalescingConflict resolves a conflict when two registers with
  different positions in a register hierarchy shall be coalesced during
  computeStackLocations.

  Since the resolution of such a coalescing conflict is processor-specific, this
  method is virtual and can be overloaded if required.
*/
std::pair<std::reference_wrapper<WIR_VirtualRegister>,
          std::reference_wrapper<WIR_VirtualRegister>> WIR_GraphColoring::resolveSpillCoalescingConflict( const WIR_VirtualRegister &r1,
                                                                                                          const WIR_VirtualRegister &r2 ) const
{
  DSTART(
    "virtual pair<reference_wrapper<WIR_VirtualRegister>, reference_wrapper<WIR_VirtualRegister> > WIR_GraphColoring::resolveSpillCoalescingConflict(const WIR_VirtualRegister&, const WIR_VirtualRegister&) const" );

  return(
    pair<std::reference_wrapper<WIR_VirtualRegister>,
         std::reference_wrapper<WIR_VirtualRegister>>(
      const_cast<WIR_VirtualRegister &>( r1 ),
      const_cast<WIR_VirtualRegister &>( r2 ) ) );
};


/*
  getStackSize returns the byte size of the specified register on the stack.

  Since the computation of stack sizes is processor-specific, this method is
  virtual and can be overloaded if required.
*/
unsigned int WIR_GraphColoring::getStackSize( const WIR_BaseRegister &r ) const
{
  DSTART(
    "virtual unsigned int WIR_GraphColoring::getStackSize(const WIR_BaseRegister&) const" );

  return( r.getBitWidth() / 8 );
};


/*
  markSpillInstruction marks the given WIR instruction as spill code using a
  dedicated WIR comment.

  markSpillInstruction shall be invoked by implementations of the methods
  insertSpillLoad and insertSpillStore above.
*/
WIR_Instruction &WIR_GraphColoring::markSpillInstruction( WIR_Instruction &i,
                                                          const WIR_BaseRegister &r )
{
  DSTART(
    "WIR_Instruction& WIR_GraphColoring::markSpillInstruction(WIR_Instruction&, const WIR_BaseRegister&)" );

  if ( mMarkSpillCode )
    i.insertContainer( WIR_Comment( "RA SPILL " + r.getName() ) );

  return( i );
};


/*
  rewriteProgramHook allows to perform processor-specific actions after having
  transformed the WIR code.

  Since these actions are processor-specific and might or might not be necessary
  for some actual processor's EABI, this method is virtual and can be overloaded
  if required.
*/
void WIR_GraphColoring::rewriteProgramHook( WIR_Function &f )
{
  DSTART( "virtual void WIR_GraphColoring::rewriteProgramHook(WIR_Function&)" );

  (void) f;
};


/*
  applyCallerSavingPolicy inserts dedicated spill-stores and -loads of caller-
  saved registers that are alive across some function call.

  Since this task might be processor-specific for some actual processor's ABI, this method is virtual and can be overloaded if required.
*/
void WIR_GraphColoring::applyCallerSavingPolicy( WIR_Function &f )
{
  DSTART(
    "virtual void WIR_GraphColoring::applyCallerSavingPolicy(WIR_Function&)" );

  mVregsAliveAcrossCall.clear();

  // We now finally have to ensure that f as a calling function is responsible
  // for preserving any values residing in caller-saved registers that are live
  // across a function call. Determine the additional stack space required due
  // to caller-saved registers alive across a call.
  computeCallerSavingStackSpace( f );

  if ( mCallerSavingStackSpace == 0 )
    return;

  // Now, we finally generate the instructions in order to caller-save registers
  // in f.
  for ( auto &[ call, callerSavedRegs ] : mCallerSavedRegsAliveAcrossCall ) {
    unsigned int stackPos = mAdditionalStackSpace - mCallerSavingStackSpace;

    // Determine the position before the call where stores should go.
    WIR_BasicBlock &b = call.get().getBasicBlock();
    auto posSt = b.findInstruction( call );

    // Determine the call's successor basic block.
    std::reference_wrapper<WIR_BasicBlock> succBB =
      b.getSuccessors().begin()->get();
    if  ( ( mFunctionReturnMoves[ call.get().getID() ] == 0 ) &&
          ( succBB.get().getPredecessors().size() > 1 ) )
      // Create a new basic block between the current block b and its succ.
      succBB =
        f.insertBasicBlock(
          f.findBasicBlock( succBB.get() ), WIR_BasicBlock {} )->get();

    unsigned int earlyInsertedLoads = 0;
    for ( const WIR_BaseRegister &r : callerSavedRegs ) {
      // Determine the position after the call where loads should go.
      auto posLd = succBB.get().begin();

      // Since a call might define some register, there might be some move
      // instructions immediately after the call that move the function call's
      // result away. If the operations immediately after a call are moves, then
      // the caller-restore instruction to be generated here must be placed
      // after such a move.
      // However, spilling and coalescing can have the effect that such a move
      // no longer is present, but becomes a spill-store that depends on the
      // register to be restored here. In this case, the caller-restore
      // instruction to be generated here must be placed before such a spill-
      // load.
      // So, let's first determine which of the two cases above applies.
      bool returnWithMoves = true;

      for ( unsigned int i = 0; i < mFunctionReturnMoves[ call.get().getID() ];
            ++i ) {
        auto moveIt = std::next( posLd, earlyInsertedLoads + i );

        if ( moveIt == succBB.get().end() ) {
          returnWithMoves = false;
          break;
        }

        const auto &moveInstr = moveIt->get();

        for ( const WIR_Operation &moveOp : moveInstr ) {
          if ( !moveOp.isMove() && moveOp.isMemoryStore() )
            for ( const WIR_Parameter &p : moveOp )
              if ( p.getType() == WIR_ParameterType::reg ) {
                auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p );
                if ( rp.isUsed() && ( rp.getRegister() == r ) )
                  returnWithMoves = false;
              }
        }
      }

      if ( returnWithMoves )
        std::advance(
          posLd,
          earlyInsertedLoads + mFunctionReturnMoves[ call.get().getID() ] );
      else
        ++earlyInsertedLoads;

      // For each caller-saved register alive across a call, insert a store
      // immediately before the call, and a load immediately after.
      insertSpillStore( r, r, stackPos, b, posSt );
      insertSpillLoad( r, r, stackPos, succBB.get(), posLd );

      // cppcheck-suppress variableScope
      DACTION(
        BBPOS( call.get().begin()->get() );
        DOUT(
          "Inserting caller-saving instructions for function call " << BBID <<
          call << endl << std::prev( posSt )->get() <<
          std::prev( posLd )->get() ); );

      stackPos += getStackSize( r );
    }
  }
};


/*
  postProcessingHook allows to perform processor-specific actions after having
  done register allocation for a function, using e.g., the set of inserted spill
  operations mInsertedSpillCode.

  For example, postProcessingHook can be used to realize processor-specific
  calling conventions afterwards, if necessary.

  Since these actions are processor-specific and might or might not be necessary
  for some actual processor's EABI, this method is virtual and can be overloaded
  if required.
*/
void WIR_GraphColoring::postProcessingHook( WIR_Function &f )
{
  DSTART( "virtual void WIR_GraphColoring::postProcessingHook(WIR_Function&)" );

  (void) f;
};


/*
  postRACleanup allows to perform very final processor-specific cleanup actions,
  particularly after stack frame reorganization.

  Since these actions are processor-specific and might or might not be necessary
  for some actual processor's EABI, this method is virtual and can be overloaded
  if required.
*/
void WIR_GraphColoring::postRACleanup( WIR_Function &f )
{
  DSTART( "virtual void WIR_GraphColoring::postRACleanup(WIR_Function&)" );

  (void) f;
};


//
// Private class methods
//

/*
  saveBestSolution saves the best allocation determined so far using Bernstein's
  best-of-three spilling heuristic.
*/
void WIR_GraphColoring::saveBestSolution( void )
{
  DSTART( "void WIR_GraphColoring::saveBestSolution()" );

  DOUT(
    "Storing Bernstein " << mSpillHeuristicToApply <<
    " as best spill heuristic." << endl );

  mMinimalTotalSpillCosts = mTotalSpillCosts;
  mBestSpillHeuristic = mSpillHeuristicToApply;

  mBestSpilledNodes = std::move( mSpilledNodes );
  mBestCoalescedMoves = std::move( mCoalescedMoves );
  mBestRegisterMapping = std::move( mRegisterMapping );
  mBestSpillAliases = std::move( mSpillAliases );

  mBestLocalCallerSavedRegsAliveAcrossCall =
    std::move( mLocalCallerSavedRegsAliveAcrossCall );
  mBestLocalDummyParameters = std::move( mLocalDummyParameters );

  // Delete rematerialization code of a previous Bernstein round.
  for ( auto mapEntry : mBestRematerializationInstructions )
    for ( auto i : mapEntry.second )
      delete( i );

  mBestRematerializationInstructions =
    std::move( mRematerializationInstructions );

  mSpilledNodes.clear();
  mCoalescedMoves.clear();
  mRegisterMapping.clear();
  mSpillAliases.clear();
  mRematerializationInstructions.clear();

  mLocalCallerSavedRegsAliveAcrossCall.clear();
  mLocalDummyParameters.clear();

  saveBestSolutionHook();
};


/*
  restoreBestSolution restores the previously saved best allocation from
  Bernstein's best-of-three spilling heuristic.
*/
void WIR_GraphColoring::restoreBestSolution( void )
{
  DSTART( "void WIR_GraphColoring::restoreBestSolution()" );

  DOUT(
    "Reverting to results of best spill heuristic Bernstein " <<
    mBestSpillHeuristic << "." << endl );

  mSpilledNodes = std::move( mBestSpilledNodes );
  mCoalescedMoves = std::move( mBestCoalescedMoves );
  mRegisterMapping = std::move( mBestRegisterMapping );
  mSpillAliases = std::move( mBestSpillAliases );

  mLocalCallerSavedRegsAliveAcrossCall =
    std::move( mBestLocalCallerSavedRegsAliveAcrossCall );
  mLocalDummyParameters = std::move( mBestLocalDummyParameters );

  // Delete rematerialization code of the current Bernstein round.
  for ( auto mapEntry : mRematerializationInstructions )
    for ( auto i : mapEntry.second )
      delete( i );

  mRematerializationInstructions =
    std::move( mBestRematerializationInstructions );

  mBestSpilledNodes.clear();
  mBestCoalescedMoves.clear();
  mBestRegisterMapping.clear();
  mBestSpillAliases.clear();
  mBestRematerializationInstructions.clear();

  mBestLocalCallerSavedRegsAliveAcrossCall.clear();
  mBestLocalDummyParameters.clear();

  restoreBestSolutionHook();
};


/*
  precolorSpecialRegs scans the specified WIR function and replaces all
  occurrences of virtual registers pre-colored with a phreg from
  mPhregsForPrecoloringOnly by the associated physical register.
*/
void WIR_GraphColoring::precolorSpecialRegs( WIR_Function &f ) const
{
  DSTART( "void WIR_GraphColoring::precolorSpecialRegs(WIR_Function&) const" );

  map<WIR_id_t, std::reference_wrapper<WIR_PhysicalRegister>> regMap;
  WIR_VirtualRegisterSet regSet;

  for ( WIR_VirtualRegister &r : f.getVirtualRegisters() ) {
    if ( f.containsPrecolor( r ) ) {
      WIR_PhysicalRegister &preg = f.findPrecolor( r );
      if ( mPhregsForPrecoloringOnly.count( preg ) ) {
        // We have found a virtual register r that is pre-colored with a
        // physical register from mPhregsForPrecoloringOnly. In this case, we
        // replace all occurrences of r by the pre-colored phreg.
        regMap.insert( { r.getID(), preg } );
        regSet.insert( r );
      }
    }
  }

  // Replace all occurrences of vregs in regMap by the identified phreg.
  for ( WIR_BasicBlock &b : f )
    for ( WIR_Instruction &i : b )
      for ( WIR_Operation &o : i ) {
        map<WIR_id_t, set<WIR_Usage>> implicitRegParams;

        for ( auto it = o.getParameters().begin();
              it != o.getParameters().end(); ++it )
          if ( it->get().getType() == WIR_ParameterType::reg ) {
            auto &p = dynamic_cast<WIR_RegisterParameter &>( it->get() );
            auto &r = p.getRegister();
            auto regMapIt = regMap.find( r.getID() );

            // Check whether the current register parameter occurs in regMap,
            // i.e., whether it principally has to be replaced by its precolored
            // phreg.
            if ( regMapIt != regMap.end() ) {
              // If the current parameter is an explicit parameter, or if it is
              // an implicit parameter that we have not yet seen before for the
              // current operation o, replace the virtual register of the
              // parameter by its identified phreg.
              if ( p.isExplicit() ||
                   ( p.isImplicit() &&
                     ( implicitRegParams[ r.getID() ].count( p.getUsage() ) ==
                         0 ) ) ) {
                DOUT(
                  "Replacing pre-colored register '" << r.getName() <<
                  "' by '" << regMapIt->second.get().getName() << "'." <<
                  endl );

                bool isDontOptimize = p.getDontOptimize();
                p.setDontOptimize( false );

                it =
                  o.replaceParameter(
                    it,
                    WIR_RegisterParameter(
                      regMapIt->second.get(), p.getUsage(), p.isImplicit() ) );

                auto &newP = dynamic_cast<WIR_RegisterParameter &>( it->get() );
                newP.setDontOptimize( isDontOptimize );

                // Update implicit register information if necessary.
                if ( newP.isImplicit() )
                  implicitRegParams[ r.getID() ].insert( newP.getUsage() );
              }
            }
          }
      }

  // Delete unnecessary pre-color information from f.
  for ( const WIR_VirtualRegister &r : regSet )
    f.erasePrecolor( r );
};


/*
  initPrecolors initializes the map mPrecolored according to the pre-color
  information attached to the specified WIR function.
*/
void WIR_GraphColoring::initPrecolors( const WIR_Function &f )
{
  DSTART( "void WIR_GraphColoring::initPrecolors(const WIR_Function&)" );

  mPrecolored.clear();

  // Pre-color physical registers with themselves for the sake of completeness.
  for ( const WIR_PhysicalRegister &p : mPhregs )
    for ( WIR_PhysicalRegister &l : p.getLeafs() )
      mPrecolored.insert( { l, l } );
  for ( const WIR_PhysicalRegister &preg : mPhregsForPrecoloringOnly )
    for ( WIR_PhysicalRegister &l : preg.getLeafs() )
      mPrecolored.insert( { l, l } );

  // Pre-color virtual registers and their complete hierarchies.
  for ( const WIR_VirtualRegister &r : f.getVirtualRegisters() )
    if ( f.containsPrecolor( r ) ) {
      list<std::reference_wrapper<WIR_VirtualRegister>> worklist;
      worklist.push_back( r.getRoot() );

      do {
        WIR_VirtualRegister &vReg = worklist.front().get();
        worklist.pop_front();

        mPrecolored.insert( { vReg, f.findPrecolor( vReg ) } );
        DOUT(
          "mPrecolored[ " << vReg.getName() << " ] = " <<
          mPrecolored.at( vReg ).get().getName() << endl );

        copy( vReg.begin(), vReg.end(), back_inserter( worklist ) );
      } while ( !worklist.empty() );
    }
};


/*
  build builds the interference graph for the specified WIR function.

  Construct the interference graph, and categorize each node as either
  move-related or non-move-related. A move-related node is one that is either
  the source or destination of a move instruction.

  This function implements procedure Build() from A. W. Appel, page 252:

  procedure Build()
    forall b in blocks in program
      let live = liveOut(b)
      forall I in instructions(b) in reverse order
        if isMoveInstruction(I) then
          live <- live \ use(I)
          forall n in def(I) U use(I)
            moveList[n] <- moveList[n] U {I}
          worklistMoves <- worklistMoves U {I}
        live <- live U def(I)
        forall d in def(I)
          forall l in live
            AddEdge(l,d)
        live <- use(I) U (live \ def(I))
*/
void WIR_GraphColoring::build( WIR_Function &f, WIR_InterferenceGraph &igraph )
{
  DSTART(
    "void WIR_GraphColoring::build(WIR_Function&, WIR_InterferenceGraph&)" );

  if ( mVerbosity )
    ufProgrMsg << ufFile() << "Building interference graph." << endl;

  mActiveMoves.clear();
  mCoalescedMoves.clear();
  mConstrainedMoves.clear();
  mFrozenMoves.clear();
  mWorkListMoves.clear();
  mMoveList.clear();

  for ( auto mapEntry : mRematerializationInstructions )
    for ( auto i : mapEntry.second )
      delete( i );
  mRematerializationInstructions.clear();
  mVregsAliveAcrossCall.clear();

  for ( WIR_BasicBlock &b : f )
    for ( WIR_Instruction &i : b ) {
      // Get live-out set of instruction i.
      auto &ltaContainer = i.getContainers<WIR_LiveOut>().begin()->get();
      WIR_RegisterSet live = ltaContainer.getRegisters();

      for ( WIR_Operation &o : i ) {

        // Check which virtual registers are alive across function calls. This
        // information is required later in order to correctly handle caller-
        // saved registers during function calls.
        if ( o.isCall() || o.isIndirectCall() ) {
          for ( WIR_BaseRegister &r : ltaContainer.getRegisters() )
            if ( r.isVirtual() )
              mVregsAliveAcrossCall[ i ].insert(
                dynamic_cast<WIR_VirtualRegister &>( r ) );

          DACTION(
            if ( mVregsAliveAcrossCall[ i ].size() != 0 ) {
              BBPOS( o );
              DOUT(
                endl << "Virtual registers live across function call " <<
                BBID << i << "{" );
              for ( const WIR_VirtualRegister &r : mVregsAliveAcrossCall[ i ] )
                DOUT( " " << r.getName() );
              DOUT( " }" << endl );
            } );
        }

        if ( getCoalescing() && o.isMove() && isCoalescableMove( o, igraph ) ) {
          for ( WIR_Parameter &p : o )
            if ( p.getType() == WIR_ParameterType::reg ) {
              auto &rp = dynamic_cast<WIR_RegisterParameter &>( p );
              WIR_BaseRegister &r = rp.getRegister();

              if ( r.isVirtual() ) {
                auto &vreg = dynamic_cast<WIR_VirtualRegister &>( r );

                if ( rp.isUsed() || rp.isDefUsed() )
                  // Remove current USE parameter from set live so that no edge
                  // is generated in the interference graph for p here - it's a
                  // MOVE!
                  live.erase( r );

                // Identify the current operation o as a move that is related to
                // its current register parameter r AND the entire register
                // hierarchy coming along with r!
                list<std::reference_wrapper<WIR_VirtualRegister>> workList;
                workList.push_back( vreg.getRoot() );

                do {
                  const WIR_VirtualRegister &currentReg =
                    workList.front().get();
                  workList.pop_front();

                  mMoveList[ currentReg.getID() ].insert( o );
                  DEBUG_OP_INS( o, currentReg );

                  copy(
                    currentReg.begin(), currentReg.end(),
                    back_inserter( workList ) );
                } while ( !workList.empty() );
              }
            }

          mWorkListMoves.insert( o );
          DEBUG_OP_ADD( o, "mWorkListMoves" );
          DEBUG_MOVE_SETS_INVARIANT( o );
        }

        for ( const WIR_Parameter &p : o )
          if ( p.getType() == WIR_ParameterType::reg ) {
            const auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p );
            if ( rp.isDefined() || rp.isDefUsed() )
              live.insert( rp.getRegister() );
          }

        for ( WIR_Parameter &p : o )
          if ( p.getType() == WIR_ParameterType::reg ) {
            auto &rp = dynamic_cast<WIR_RegisterParameter &>( p );
            if ( rp.isDefined() || rp.isDefUsed() ) {
              WIR_BaseRegister &r = rp.getRegister();

              // If r is a physical and hierarchical register, r might not be
              // represented by an interference graph node. Instead, only the
              // leafs of a hierarchical physical register are graph nodes. To
              // properly deal with this, we put all these leafs in the
              // following list and iterate this list in order to insert
              // interference graph edges.
              // For virtual registers r, only r itself is put in the list.
              vector<std::reference_wrapper<WIR_BaseRegister>> definedRegs;

              if ( r.isPhysical() )
                definedRegs = r.getLeafs();
              else
                definedRegs.push_back( r );

              for ( const WIR_BaseRegister &definedReg : definedRegs )
                for ( const WIR_BaseRegister &l : live ) {
                  bool omitEdge = false;

                  if ( ( definedReg.isPhysical() && l.isPhysical() ) ||
                       ( isPrecolored( definedReg.getRoot() ) &&
                         ( mPrecolored.at( definedReg.getRoot() ).get() ==
                             l.getRoot() ) ) ||
                       ( isPrecolored( l.getRoot() ) &&
                         ( mPrecolored.at( l.getRoot() ).get() ==
                             definedReg.getRoot() ) ) ||
                       ( isPrecolored( definedReg.getRoot() ) &&
                         isPrecolored( l.getRoot() ) &&
                         definedReg.isHierarchical() &&
                         ( mPrecolored.at( definedReg.getRoot() ).get() ==
                             mPrecolored.at( l.getRoot() ).get() ) ) ||
                       ( isPrecolored( l.getRoot() ) && l.isHierarchical() &&
                         isPrecolored( definedReg.getRoot() ) &&
                         ( mPrecolored.at( l.getRoot() ).get() ==
                             mPrecolored.at( definedReg.getRoot() ).get() ) ) ||
                       ( isPrecolored( definedReg ) && isPrecolored( l ) &&
                         !definedReg.isHierarchical() &&
                         ( mPrecolored.at(
                             const_cast<WIR_BaseRegister &>(
                               definedReg ) ).get() ==
                             mPrecolored.at(
                               const_cast<WIR_BaseRegister &>( l ) ).get() ) ) ||
                       ( isPrecolored( l ) && !l.isHierarchical() &&
                         isPrecolored( definedReg ) &&
                         ( mPrecolored.at(
                             const_cast<WIR_BaseRegister &>( l ) ).get() ==
                             mPrecolored.at(
                               const_cast<WIR_BaseRegister &>(
                                 definedReg ) ).get() ) ) )
                    // We explicitly leave out edges between two physical
                    // registers and between a virtual register and its
                    // pre-colored physical counterpart.
                    omitEdge = true;

                  if ( !omitEdge &&
                       igraph.containsNode( definedReg ) &&
                       igraph.containsNode( l ) &&
                       !igraph.areSameNodes( definedReg, l ) &&
                       !igraph.interfere( definedReg, l ) ) {
                    // We add an edge if, for any two different registers
                    // definedReg and l, there exist nodes in the interference
                    // graph, and no edge has already been inserted previously.
                    DOUT(
                      "Adding interference " << definedReg.getName() <<
                      " <-> " << l.getName() << endl );
                    igraph.addInterference( definedReg, l );
                  }
                }
            }
          }
      }
    }

  // Add explicit interferences to the interference graph.
  for ( const WIR_VirtualRegister &vreg : f.getVirtualRegisters() )
    for ( const WIR_PhysicalRegister &phreg : f.findInterferences( vreg ) )
      if ( igraph.containsNode( vreg ) && igraph.containsNode( phreg ) &&
           !igraph.interfere( vreg, phreg ) ) {
        // We add an edge if, for any two different registers vreg and phreg,
        // there exist nodes in the interference graph, and no edge has already
        // been inserted previously.
        DOUT(
          "Adding interference " << vreg.getName() << " <-> " <<
          phreg.getName() << endl );
        igraph.addInterference( vreg, phreg );
      }

  // Pre-color the interference graph.
  buildPrecolors( igraph );

  // Assign loop nesting depths to the graph nodes.
  buildLoopNestingDepth( igraph );

  // Count how many moves storing a function call's result immediately follow a
  // call.
  buildReturnMoves( f );

  // Add processor-specific interference edges.
  buildProcessorSpecificInterferences( f, igraph );

  // Clear some data structures at the beginning of each and every allocation
  // round.
  mLocalCallerSavedRegsAliveAcrossCall.clear();
  mLocalDummyParameters.clear();

  {
    DSTART(
      "void WIR_GraphColoring::build(WIR_Function&, WIR_InterferenceGraph&).visualize" );
    DACTION( igraph.visualize(); );
  }
};


/*
  buildPrecolors realizes the pre-coloring of the interference graph.
*/
void WIR_GraphColoring::buildPrecolors( WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "void WIR_GraphColoring::buildPrecolors(WIR_InterferenceGraph&) const" );

  for ( auto &[ r, precol ] : mPrecolored )
    if ( r.get().isVirtual() && igraph.containsNode( r.get() ) &&
         !r.get().hasChilds() ) {
      // We have found a pre-colored virtual register that is leaf of a
      // potential register hierarchy.
      DOUT(
        "Pre-coloring " << r.get().getName() << " with " <<
        precol.get().getName() << endl );

      // Assign a color to the interference graph node.
      unsigned int preColor = *(igraph.getColors( precol.get() ).begin());
      igraph.setColor( r.get(), preColor );
    }
};


/*
  buildLoopNestingDepth assigns each interference graph node its loop nesting
  depth.
*/
void WIR_GraphColoring::buildLoopNestingDepth( WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "void WIR_GraphColoring::buildLoopNestingDepth(WIR_InterferenceGraph&) "
    "const" );

  // Iterate over all entries of mBBLoopDepth.
  for ( auto &[ b, nestingDepth ] : mBBLoopDepth )
    for ( const WIR_Instruction &i : b.get() )
      for ( const WIR_Operation &o : i )
        for ( const WIR_Parameter &p : o )
          if ( p.getType() == WIR_ParameterType::reg ) {
            const WIR_BaseRegister &r =
              dynamic_cast<const WIR_RegisterParameter &>( p ).getRegister();

            if ( igraph.containsNode( r ) &&
                 ( nestingDepth > igraph.getLoopNestingDepth( r ) ) )
              igraph.setLoopNestingDepth( r, nestingDepth );
          }
};


/*
  buildReturnMoves counts how many move operations storing a function call's
  result immediately follow a call.

  For a WIR instruction i being a function call, buildReturnMoves stores the
  number of return-related moves in map mFunctionReturnMoves using the unique ID
  of i.
*/
void WIR_GraphColoring::buildReturnMoves( const WIR_Function &f )
{
  DSTART( "void WIR_GraphColoring::buildReturnMoves(const WIR_Function&)" );

  // Count how many move operations storing a funcion call's result occur
  // immediately after a call.
  for ( const WIR_BasicBlock &b : f )
    for ( const WIR_Instruction &i : b )
      if ( i.getOperations().front().get().isCall() ||
           i.getOperations().front().get().isIndirectCall() ) {
        // Count the number of call-related move operations.
        unsigned int cnt = 0;

        for ( const WIR_Instruction &succIn :
                b.getSuccessors().begin()->get() ) {
          bool done = false;

          for ( const WIR_Operation &succOp : succIn ) {
            if ( isFunctionReturnMove( succOp ) )
              ++cnt;
            else {
              done = true;
              break;
            }
          }

          if ( done )
            break;
        }

        mFunctionReturnMoves[ i.getID() ] = cnt;
      }
};


/*
  makeWorkList sets up the various data structures to keep track of graph nodes
  and their states, i.e., mWorkListSpill, mWorkListFreeze and mWorkListSimplify.

  This function implements procedure MakeWorklist() from A. W. Appel, page 252:

  procedure MakeWorklist()
    forall n in initial
      initial <- initial \ {n}
      if degree[n] >= K then
        spillWorklist <- spillWorklist U {n}
      else if moveRelated(n) then
        freezeWorklist <- freezeWorklist U {n}
      else
        simplifyWorklist <- simplifyWorklist U {n}
*/
void WIR_GraphColoring::makeWorkList( const WIR_Function &f,
                                      const WIR_InterferenceGraph &igraph )
{
  DSTART(
    "void WIR_GraphColoring::makeWorkList(const WIR_Function&, const "
    "WIR_InterferenceGraph&)" );

  // Clear data structures initially.
  mActiveMoves.clear();
  mFrozenMoves.clear();
  mWorkListFreeze.clear();
  mWorkListPriority.clear();
  mWorkListSimplify.clear();
  mWorkListSpill.clear();

  for ( WIR_VirtualRegister &r : f.getVirtualRegisters() )
    if ( !r.isChild() && igraph.containsNode( r ) && !isPrecolored( r ) ) {
      if ( isPriorityRegister( r ) ) {
        mWorkListPriority.insert( r );
        DEBUG_REG_ADD( r, "mWorkListPriority" );
      } else

      if ( mSpillAll ||
           ( igraph.getDegree( r ) >= igraph.getAvailableColors() ) ) {
        mWorkListSpill.insert( r );
        DEBUG_REG_ADD( r, "mWorkListSpill" );
        DEBUG_SPILL_WL_INVARIANT( r, igraph );
      } else

      if ( isMoveRelated( r ) ) {
        mWorkListFreeze.insert( r );
        DEBUG_REG_ADD( r, "mWorkListFreeze" );
        DEBUG_FREEZE_WL_INVARIANT( r, igraph );
      } else {
        mWorkListSimplify.insert( r );
        DEBUG_REG_ADD( r, "mWorkListSimplify" );
        DEBUG_SIMPLIFY_WL_INVARIANT( r, igraph );
      }
    }
};


/*
  decrementDegree inspects an interference graph node and moves it into its
  appropriate work lists if necessary.

  This function implements procedure DecrementDegree(m) from A. W. Appel,
  page 253:

  procedure DegrementDegree(m)
    let d = degree[m]
    degree[m] <- d-1
    if d = K then
      EnableMoves({m} U Adjacent(m))
      spillWorklist <- spillWorklist \ {m}
      if MoveRelated(m) then
        freezeWorklist <- freezeWorklist U {m}
      else
        simplifyWorklist <- simplifyWorklist U {m}
*/
void WIR_GraphColoring::decrementDegree( const WIR_VirtualRegister &r,
                                         const WIR_InterferenceGraph &igraph )
{
  DSTART(
    "void WIR_GraphColoring::decrementDegree(const WIR_VirtualRegister&, const WIR_InterferenceGraph&)" );

  if ( ( igraph.getDegree( r ) < igraph.getAvailableColors() ) &&
       mWorkListSpill.count( r.getRoot() ) && !mSpillAll ) {
    // The current node r has transitioned from high to low degree!
    enableMoves( r, igraph );

    mWorkListSpill.erase( r.getRoot() );

    if ( isMoveRelated( r ) ) {
      mWorkListFreeze.insert( r.getRoot() );
      DEBUG_REG_MOV( r.getRoot(), "mWorkListSpill", "mWorkListFreeze" );
      DEBUG_FREEZE_WL_INVARIANT( r.getRoot(), igraph );
    } else {
      mWorkListSimplify.insert( r.getRoot() );
      DEBUG_REG_MOV( r.getRoot(), "mWorkListSpill", "mWorkListSimplify" );
      DEBUG_SIMPLIFY_WL_INVARIANT( r.getRoot(), igraph );
    }

    auto regSet = igraph.getCoalescedAliases( r );
    for ( const WIR_BaseRegister &reg : regSet )
      if ( reg.isVirtual() ) {
        const auto vreg = dynamic_cast<const WIR_VirtualRegister &>( reg );
        if ( mWorkListSpill.count( vreg.getRoot() ) ) {
          mWorkListSpill.erase( vreg.getRoot() );
          DEBUG_REG_RM( vreg.getRoot(), "mWorkListSpill" );
        }
      }
  }
};


/*
  addWorkList adds an interference graph node to the simplify work list if
  possible.

  This function implements procedure AddWorkList(u) from A. W. Appel, page 254:

  procedure AddWorkList(u)
    if (u not in precolored and not(MoveRelated(u)) and degree[u] < K) then
      freezeWorklist <- freezeWorklist \ {u}
      simplifyWorklist <- simplifyWorklist U {u}
*/
void WIR_GraphColoring::addWorkList( const WIR_BaseRegister &r,
                                     const WIR_InterferenceGraph &igraph )
{
  DSTART(
    "void WIR_GraphColoring::addWorkList(const WIR_BaseRegister&, const WIR_InterferenceGraph&)" );

  WIR_BaseRegister &key = igraph.getUnaliasedReg( r );

  if ( key.isVirtual() && !isPrecolored( key ) && !isMoveRelated( key ) &&
       !mSpillAll &&
       ( igraph.getDegree( key ) < igraph.getAvailableColors() ) ) {
    WIR_VirtualRegister &vkey =
      dynamic_cast<WIR_VirtualRegister &>( key ).getRoot();

    if ( mWorkListFreeze.count( vkey ) ) {
      DEBUG_FREEZE_WL_WEAK_INVARIANT( vkey, igraph );
      mWorkListFreeze.erase( vkey);
      mWorkListSimplify.insert( vkey );

      DEBUG_REG_MOV( vkey, "mWorkListFreeze", "mWorkListSimplify" );
      DEBUG_SIMPLIFY_WL_INVARIANT( vkey, igraph );

      auto regSet = igraph.getCoalescedAliases( vkey );
      for ( WIR_BaseRegister &reg : regSet )
        if ( reg.isVirtual() ) {
          auto &vreg = dynamic_cast<WIR_VirtualRegister &>( reg ).getRoot();

          if ( mWorkListFreeze.count( vreg ) ) {
            DEBUG_FREEZE_WL_WEAK_INVARIANT( vreg, igraph );
            mWorkListFreeze.erase( vreg );
            DEBUG_REG_RM( vreg, "mWorkListFreeze" );
          }
        }
    } else {
      mWorkListSimplify.insert( vkey );
      DEBUG_REG_ADD( vkey, "mWorkListSimplify" );
      DEBUG_SIMPLIFY_WL_INVARIANT( vkey, igraph );
    }
  }
};


/*
  simplifyPriority removes a high-priority node in mWorkListPriority from the
  interference graph and pushes it onto the priority stack.

  This function implements procedure Simplify() from A. W. Appel, page 253.
*/
void WIR_GraphColoring::simplifyPriority( WIR_InterferenceGraph &igraph )
{
  DSTART( "void WIR_GraphColoring::simplifyPriority(WIR_InterferenceGraph&)" );

  const WIR_VirtualRegister &r = mWorkListPriority.begin()->get();
  mWorkListPriority.erase( mWorkListPriority.begin() );
  DEBUG_REG_RM( r, "mWorkListPriority" );

  auto neighbors = igraph.getNeighborVREGs( r );

  freezeMoves( r, igraph );

  // Push current node r onto the stack.
  DEBUG_REG_PUSH( r );
  igraph.pushPriorityNode( r );

  // Update all neighbors of r and have them sorted into the correct work lists.
  for ( const WIR_VirtualRegister &neighbor : neighbors )
    if ( !neighbor.isChild() )
      decrementDegree( neighbor, igraph );
};


/*
  simplify removes a non-move-related node in mWorkListSimplify from the
  interference graph and pushes it onto the stack.

  One at a time, remove non-move-related nodes of low degree from the graph.

  This function implements procedure Simplify() from A. W. Appel, page 253:

  procedure Simplify()
    let n in simplifyWorklist
    simplifyWorklist <- simplifyWorklist \ {n}
    push(n, selectStack)
    forall m in Adjacent(n)
      DecrementDegree(m)
*/
void WIR_GraphColoring::simplify( WIR_InterferenceGraph &igraph )
{
  DSTART( "void WIR_GraphColoring::simplify(WIR_InterferenceGraph&)" );

  const WIR_VirtualRegister &r = mWorkListSimplify.begin()->get();
  DEBUG_SIMPLIFY_WL_INVARIANT( r, igraph );
  mWorkListSimplify.erase( mWorkListSimplify.begin() );
  DEBUG_REG_RM( r, "mWorkListSimplify" );

  auto neighbors = igraph.getNeighborVREGs( r );

  // Push current node r onto the stack.
  DEBUG_REG_PUSH( r );
  igraph.pushNode( r );

  // Update all neighbors of r and have them sorted into the correct work lists.
  for ( const WIR_VirtualRegister &neighbor : neighbors )
    if ( !neighbor.isChild() )
      decrementDegree( neighbor, igraph );
};


/*
  computeLoopNestingDepths computes the loop nesting level of all basic blocks
  of the specified function and stores the information in map mBBLoopDepth.
*/
void WIR_GraphColoring::computeLoopNestingDepths( WIR_Function &f )
{
  DSTART( "void WIR_GraphColoring::computeLoopNestingDepths(WIR_Function&)" );

  mBBLoopDepth.clear();

  // Compute control tree for function f.
  WIR_StructuralAnalysis scfa( f );
  scfa.analyze();

  // Iterate over all basic blocks in f.
  for ( WIR_BasicBlock &b : f ) {
    mBBLoopDepth[ b ] = 0;

    // Get the block's control tree node container.
    auto &c = b.getContainers<WIR_ControlTree>().begin()->get();
    WIR_ControlTreeNode *n = &(c.getBasicBlockTreeNode().getParent());

    // Traverse the control tree to its root and count how many loop structures
    // we encounter.
    do {
      if ( n->isCyclic() )
        mBBLoopDepth[ b ] = mBBLoopDepth[ b ] + 1;

      n = &(n->getParent());
    } while ( *n != n->getParent() );

    DOUT(
      "mBBLoopDepth[ '" << b.getName() << "' ] = " << mBBLoopDepth[ b ] <<
      endl );

    // Clear block-level control tree containers.
    b.eraseContainers( WIR_ControlTree::getContainerTypeID() );
  }

  // Clear function-level control tree container.
  f.eraseContainers( WIR_ControlTree::getContainerTypeID() );
};


/*
  getLoopNestingDepth returns the loop nesting depth of the specified basic
  block.
*/
unsigned int WIR_GraphColoring::getLoopNestingDepth( const WIR_BasicBlock &b ) const
{
  DSTART(
    "unsigned int WIR_GraphColoring::getLoopNestingDepth(const WIR_BasicBlock&) const" );

  auto it = mBBLoopDepth.find( const_cast<WIR_BasicBlock &>( b ) );
  if ( it != mBBLoopDepth.end() )
    return( it->second );

  return( 0 );
};


/*
  selectSpill selects a significant-degree node for potential spilling and
  pushes it on the stack.

  This function implements procedure SelectSpill() from A. W. Appel, page 255:

  procedure SelectSpill()
    let m in spillWorklist selected using favorite heuristic
      Note: avoid choosing nodes that are the tiny live ranges resulting from
      the fetches of previously spilled registers
    spillWorklist <- spillWorklist \ {m}
    simplifyWorklist <- simplifyWorklist U {m}
    FreezeMoves(m)
*/
void WIR_GraphColoring::selectSpill( const WIR_Function &f,
                                     WIR_InterferenceGraph &igraph )
{
  DSTART(
    "void WIR_GraphColoring::selectSpill(const WIR_Function&, WIR_InterferenceGraph&)" );

  WIR_RematerializationMap rematInsnsOfSpillCandidates;

  computeSpillCosts( f, igraph, mWorkListSpill, rematInsnsOfSpillCandidates );
  WIR_VirtualRegister &r =
    selectSpillCandidate( igraph, mWorkListSpill ).getRoot();

  WIR_RegisterSet regSet = igraph.getCoalescedAliases( r );
  regSet.insert( r );

  for ( WIR_BaseRegister &r1 : regSet )
    if ( r1.isVirtual() ) {
      auto &vr = dynamic_cast<WIR_VirtualRegister &>( r1 ).getRoot();

      if ( mWorkListSpill.count( vr ) ) {
        DEBUG_SPILL_WL_INVARIANT( vr, igraph );
        mWorkListSpill.erase( vr );
        DEBUG_REG_RM( vr, "mWorkListSpill" );
      }
    }

  freezeMoves( r, igraph );

  WIR_VirtualRegisterSet neighbors = igraph.getNeighborVREGs( r );

  // Push current node r onto the stack.
  igraph.setPotentialSpill( r );
  igraph.setSpillCosts( r, mSpillCosts[ r.getID() ] );
  DEBUG_REG_PUSH( r );
  igraph.pushNode( r );
  mTotalSpillCosts += mSpillCosts[ r.getID() ];

  // Update all neighbors of r and have them sorted into the correct work lists.
  for ( const WIR_VirtualRegister &neighbor : neighbors )
    if ( !neighbor.isChild() )
      decrementDegree( neighbor, igraph );

  // Store potential rematerialization code for the determined spill candidate
  // permanently now.
  for ( auto &[ rp, instrs ] : rematInsnsOfSpillCandidates )
    if ( !instrs.empty() &&
         ( igraph.getUnaliasedReg( rp.get().getRegister().getRoot() ) == r ) ) {
      // cppcheck-suppress variableScope
      DACTION(
        BBPOS( rp.get().getOperation() );
        DOUT(
          "Permanently storing rematerialization instructions for parameter " <<
          rp.get().getID() << "/" << rp.get().getRegister().getName() <<
          " of operation " << BBID << rp.get().getOperation() << ":" << endl );
        for ( auto i : instrs )
          cout << i->getID() << endl << wir << *i;
        cout << endl; );

      mRematerializationInstructions[ rp ] = instrs;
    } else {
      DOUT( "Deleting rematerialization instructions" );
      for ( auto i : instrs ) {
        DOUT( " " << i->getID() );
        delete( i );
      }
      DOUT( endl );
    }
};


/*
  computeSpillLoadCosts computes the costs for spill-loading the specified
  register at parameter p.
*/
void WIR_GraphColoring::computeSpillLoadCosts( const WIR_RegisterParameter &p,
                                               const WIR_BaseRegister &key,
                                               unsigned int nestingDepth,
                                               WIR_RematerializationMap &rematInsns,
                                               WIR_OperationSet &movesToBeDeleted,
                                               bool deleteMove )
{
  DSTART(
    "void WIR_GraphColoring::computeSpillLoadCosts(const WIR_RegisterParameter&, const WIR_BaseRegister&, unsigned int, WIR_GraphColoring::WIR_RematerializationMap&, WIR_OperationSet&, bool)" );

  unsigned int spillLoadCosts = getSpillLoadCosts( p );

  if ( mRematerialization ) {
    unsigned int rematerializationCosts = getRematerializationCosts( p );

    if ( rematerializationCosts <= spillLoadCosts ) {
      spillLoadCosts = rematerializationCosts;

      // Add required machine instructions to rematerialize parameter p to map
      // rematInsns.
      rematInsns[ const_cast<WIR_RegisterParameter &>( p ) ] =
        getRematerializationInstructions( p );
    }
  }

  mSpillCosts[ key.getID() ] += spillLoadCosts * pow( 10.0, nestingDepth );

  if ( deleteMove && !movesToBeDeleted.count( p.getOperation() ) ) {
    // The move is no longer needed.
    mSpillCosts[ key.getID() ] -=
      getMoveCosts( p.getOperation() ) * pow( 10.0, nestingDepth );
    movesToBeDeleted.insert( p.getOperation() );
  }
};


/*
  computeSpillStoreCosts computes the costs for spill-storing the specified
  register at parameter p.
*/
void WIR_GraphColoring::computeSpillStoreCosts( const WIR_RegisterParameter &p,
                                                const WIR_BaseRegister &key,
                                                unsigned int nestingDepth,
                                                WIR_OperationSet &movesToBeDeleted,
                                                bool deleteMove )
{
  DSTART(
    "void WIR_GraphColoring::computeSpillStoreCosts(const WIR_RegisterParameter&, const WIR_BaseRegister&, unsigned int, WIR_OperationSet&, bool)" );

  mSpillCosts[ key.getID() ] +=
    getSpillStoreCosts( p ) * pow( 10.0, nestingDepth );

  if ( deleteMove && !movesToBeDeleted.count( p.getOperation() ) ) {
    // The move is no longer needed.
    mSpillCosts[ key.getID() ] -=
      getMoveCosts( p.getOperation() ) * pow( 10.0, nestingDepth );
    movesToBeDeleted.insert( p.getOperation() );
  }
};


/*
  propagateFalseLiveness marks that the liveness of the given register including
  all its childs and parents stops within the considered basic block.
*/
void WIR_GraphColoring::propagateFalseLiveness( const WIR_VirtualRegister &r,
                                                const WIR_BasicBlock &b,
                                                WIR_VirtualRegisterSet &falseLivenessInSameBB ) const
{
  DSTART(
    "void WIR_GraphColoring::propagateFalseLiveness(const WIR_VirtualRegister&, const WIR_BasicBlock&, WIR_VirtualRegisterSet&) const" );

  (void) b;
  list<std::reference_wrapper<WIR_VirtualRegister>> worklist;
  worklist.push_back( const_cast<WIR_VirtualRegister &>( r ) );

  do {
    auto &reg = worklist.front().get();
    worklist.pop_front();

    falseLivenessInSameBB.insert( reg );
    DOUT(
      "Marking '" << reg.getName() << "' as dead within '" << b.getName() <<
      "'." << endl );

    copy( reg.begin(), reg.end(), back_inserter( worklist ) );
  } while ( !worklist.empty() );

  auto *reg = &r;
  while ( reg->isChild() ) {
    reg = &(reg->getParent());
    falseLivenessInSameBB.insert( const_cast<WIR_VirtualRegister &>( *reg ) );
    DOUT(
      "Marking '" << reg->getName() << "' as dead within '" << b.getName() <<
      "'." << endl );
  }
};


/*
  propagateTrueLiveness marks that the liveness of the given register including
  all its childs starts within the considered basic block.
*/
void WIR_GraphColoring::propagateTrueLiveness( const WIR_VirtualRegister &r,
                                               const WIR_BasicBlock &b,
                                               WIR_VirtualRegisterSet &trueLivenessInSameBB ) const
{
  DSTART(
    "void WIR_GraphColoring::propagateTrueLiveness(const WIR_VirtualRegister&, const WIR_BasicBlock&, WIR_VirtualRegisterSet&) const" );

  (void) b;
  list<std::reference_wrapper<WIR_VirtualRegister>> worklist;
  worklist.push_back( const_cast<WIR_VirtualRegister &>( r ) );

  do {
    auto &reg = worklist.front().get();
    worklist.pop_front();

    trueLivenessInSameBB.insert( reg );
    DOUT(
      "Marking '" << reg.getName() << "' as live within '" << b.getName() <<
      "'." << endl );

    copy( reg.begin(), reg.end(), back_inserter( worklist ) );
  } while ( !worklist.empty() );
};


/*
  updateLivenessInSameBB updates partial basic block-level liveness information
  in the course of spill cost computation.
*/
void WIR_GraphColoring::updateLivenessInSameBB( const WIR_VirtualRegister &r,
                                                bool live,
                                                std::map<WIR_id_t, bool> &stillLiveInSameBB,
                                                const WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "void WIR_GraphColoring::updateLivenessInSameBB(const WIR_VirtualRegister&, bool, map<WIR_id_t, bool>&, const WIR_InterferenceGraph&) const" );

  list<std::reference_wrapper<WIR_VirtualRegister>> worklist;
  worklist.push_back( const_cast<WIR_VirtualRegister &>( r ) );

  // Also add the coalescing partners of r to the worklist as they also have to
  // be updated.
  auto *leftmostChild = &r;
  unsigned int depth = 0;

  while ( leftmostChild->hasChilds() ) {
    leftmostChild = &(leftmostChild->begin()->get());
    ++depth;
  }

  WIR_RegisterSet aliases = igraph.getCoalescedAliases( *leftmostChild );
  for ( WIR_BaseRegister &a : aliases )
    if ( a.isVirtual() ) {
      WIR_VirtualRegister *alias = dynamic_cast<WIR_VirtualRegister *>( &a );
      unsigned int i = 0;

      while ( alias->isChild() && ( i < depth ) ) {
        alias = &(alias->getParent());
        ++i;
      }

      worklist.push_back( *alias );
    }

  // Finally, do the worklist algorithm.
  do {
    auto &reg = worklist.front().get();
    worklist.pop_front();

    stillLiveInSameBB[ reg.getID() ] = live;

    copy( reg.begin(), reg.end(), back_inserter( worklist ) );
  } while ( !worklist.empty() );
};


/*
  isCoalescableMove checks whether a given move operation could potentially be
  coalesced, i. e., it checks whether all register parameters of the operation
  are represented by interference graph nodes and whether the involved
  registers are different.

  This check is required while initializing the register allocator's data
  structures, since the user of this class can externally specify, which
  physical registers to consider during register allocation (see method
  createPhregs). If, for some reasons, the user wishes not to use a certain
  physical register during allocation, but a WIR function contains a move
  operation where this physical register is involved, we must make sure that
  this move operation is not considered during coalescing, since not all
  involved operands of the move operation are represented by interference graph
  nodes.
*/
bool WIR_GraphColoring::isCoalescableMove( const WIR_Operation &o,
                                           const WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "bool WIR_GraphColoring::isCoalescableMove(const WIR_Operation&, const WIR_InterferenceGraph&) const" );

  const WIR_BaseRegister &r1 = getUseOfMove( o );
  const WIR_BaseRegister &r2 = getDefOfMove( o );

  // If both registers involved in a move are already the same ones, then this
  // move is no candidate for coalescing, since it is already coalesced.
  if ( !igraph.containsNode( r1 ) || !igraph.containsNode( r2 ) ||
       igraph.areSameNodes( r1, r2 ) )
    return( false );

  // Furthermore, if one of the involved registers is listed in
  // mPhregsForPrecoloringOnly, we also do not coalesce.
  if ( ( r1.isPhysical() &&
         mPhregsForPrecoloringOnly.count(
           const_cast<WIR_PhysicalRegister &>(
             dynamic_cast<const WIR_PhysicalRegister &>( r1 ) ) ) ) ||
       ( r2.isPhysical() &&
         mPhregsForPrecoloringOnly.count(
           const_cast<WIR_PhysicalRegister &>(
             dynamic_cast<const WIR_PhysicalRegister &>( r2 ) ) ) ) )
    return( false );

  return( true );
};


/*
  isMoveRelated returns whether a given register is move-related.

  This function implements function MoveRelated(n) from A. W. Appel, page
  253:

  function MoveRelated(n)
    NodeMoves(n) != {}
*/
bool WIR_GraphColoring::isMoveRelated( const WIR_BaseRegister &r ) const
{
  DSTART(
    "bool WIR_GraphColoring::isMoveRelated(const WIR_BaseRegister&) const" );

  DOUT(
    "Node '" + r.getName() + "' is " <<
    string( nodeMoves( r ).empty() ? "not " : "" ) << "move-related." << endl );

  return( !nodeMoves( r ).empty() );
};


/*
  nodeMoves returns the set of move operations associated with the given
  register, that are either enabled or not yet ready for coalescing.

  This function implements function NodeMoves(n) from A. W. Appel, page 253:

  function NodeMoves(n)
    moveList[n] set_intersection (activeMoves U worklistMoves)
*/
WIR_OperationSet WIR_GraphColoring::nodeMoves( const WIR_BaseRegister &r ) const
{
  DSTART(
    "WIR_OperationSet WIR_GraphColoring::nodeMoves(const WIR_BaseRegister&) const" );

  WIR_OperationSet result;
  auto it = mMoveList.find( r.getID() );

  if ( it != mMoveList.end() ) {
    set<WIR_id_t> moveIDs;
    for ( const WIR_Operation &mov : mActiveMoves )
      moveIDs.insert( mov.getID() );
    for ( const WIR_Operation &mov : mWorkListMoves )
      moveIDs.insert( mov.getID() );

    for ( WIR_Operation &o : it->second )
      if ( moveIDs.count( o.getID() ) )
        result.insert( o );
  }

  return( result );
};


/*
  For a given interference graph node, freezeMoves freezes all moves in which
  this node is involved.

  That is, we give up hope of coalescing these moves. This causes the node (and
  perhaps other nodes related to the frozen moves) to be considered
  non-move-related, which should enable more simplification.

  This function implements procedure FreezeMoves(u) from A. W. Appel, page 255:

  procedure FreezeMoves(u)
    forall m (=copy(x,y)) in NodeMoves(u)
      if GetAlias(y) = GetAlias(u) then
        v <- GetAlias(x)
      else
        v <- GetAlias(y)
      activeMoves <- activeMoves \ {m}
      frozenMoves <- frozenMoves U {m}
      if NodeMoves(v) = {} and degree[v] < K then
        freezeWorklist <- freezeWorklist \ {v}
        simplifyWorklist <- simplifyWorklist U {v}
*/
void WIR_GraphColoring::freezeMoves( const WIR_VirtualRegister &r,
                                     WIR_InterferenceGraph &igraph )
{
  DSTART(
    "void WIR_GraphColoring::freezeMoves(const WIR_VirtualRegister&, WIR_InterferenceGraph&)" );

  for ( WIR_Operation &mov : nodeMoves( r ) ) {

    // Determine the registers involved in the current move operation.
    WIR_BaseRegister &x = getUseOfMove( mov );
    WIR_BaseRegister &y = getDefOfMove( mov );

    WIR_BaseRegister &v =
      ( igraph.areSameNodes( y, r ) ? x.getRoot() : y.getRoot() );

    if ( mActiveMoves.count( mov ) ) {
      mActiveMoves.erase( mov );
      DEBUG_OP_RM( mov, "mActiveMoves" );
    }
    if ( mWorkListMoves.count( mov ) ) {
      mWorkListMoves.erase( mov );
      DEBUG_OP_RM( mov, "mWorkListMoves" );
    }
    mFrozenMoves.insert( mov );
    DEBUG_OP_ADD( mov, "mFrozenMoves" );

    DEBUG_MOVE_SETS_INVARIANT( mov );

    if ( v.isVirtual() && !isMoveRelated( v ) &&
         ( igraph.getDegree( v ) < igraph.getAvailableColors() ) ) {
      auto regSet = igraph.getCoalescedAliases( v );
      regSet.insert( v );

      for ( WIR_BaseRegister &reg : regSet )
        if ( reg.isVirtual() ) {
          auto &vreg = dynamic_cast<WIR_VirtualRegister &>( reg );
          if ( mWorkListFreeze.count( vreg ) ) {
            DEBUG_FREEZE_WL_WEAK_INVARIANT( vreg.getRoot(), igraph );
            mWorkListFreeze.erase( vreg.getRoot() );
            DEBUG_REG_RM( vreg.getRoot(), "mWorkListFreeze" );
          }
        }

      if ( !isPrecolored( v ) ) {
        // We never push a colored node onto the stack, cf. A. W. Appel, page
        // 243: "We cannot simplify a precolored node - this would mean pulling
        // it from the graph in the hope that we can assign it a color later,
        // but in fact we have no freedom about what color to assign it."
        auto neighbors = igraph.getNeighborVREGs( v );

        // Push current node v onto the stack.
        DEBUG_REG_PUSH( v );
        igraph.pushNode( dynamic_cast<WIR_VirtualRegister &>( v ) );

        // Update all neighbors of v and have them sorted into the correct work
        // lists.
        for ( const WIR_VirtualRegister &neighbor : neighbors )
          if ( !neighbor.isChild() )
            decrementDegree( neighbor, igraph );
      }
    }
  }
};


/*
  enableMoves inspects an interference graph node and its neighbors, checks
  whether they are move-related and enables them for coalescing.

  This function implements procedure EnableMoves(nodes) from A. W. Appel,
  page 253:

  procedure EnableMoves(nodes)
    forall n in nodes
      forall m in NodeMoves(n)
        if m in activeMoves then
          activeMoves <- activeMoves \ {m}
          worklistMoves <- worklistMoves U {m}
*/
void WIR_GraphColoring::enableMoves( const WIR_BaseRegister &r,
                                     const WIR_InterferenceGraph &igraph )
{
  DSTART(
    "void WIR_GraphColoring::enableMoves(const WIR_BaseRegister&, const WIR_InterferenceGraph&)" );

  WIR_RegisterSet nodesToConsider;

  nodesToConsider.insert( const_cast<WIR_BaseRegister &>( r ) );
  for ( WIR_VirtualRegister &reg : igraph.getNeighborVREGs( r ) )
    nodesToConsider.insert( reg );

  for ( const WIR_BaseRegister &n : nodesToConsider )
    if ( n.isVirtual() && !n.isChild() ) {
      auto moves = nodeMoves( n );

      for ( WIR_Operation &m : moves )
        if ( mActiveMoves.count( m ) ) {
          DEBUG_OP_MOV( m, "mActiveMoves", "mWorkListMoves" );

          mActiveMoves.erase( m );
          mWorkListMoves.insert( m );
          DEBUG_MOVE_SETS_INVARIANT( m );
        }
    }
};


/*
  coalesce performs conservative coalescing on the reduced graph obtained from
  simplify().

  Perform conservative coalescing on the reduced graph obtained in the
  simplification phase. Since the degrees of many nodes have been reduced by
  simplify, the conservative strategy is likely to find many more moves to
  coalesce than it would have in the initial interference graph. After two
  nodes have been coalesced, if the resulting node is no longer move-related
  it will be available for the next round of simplification. simplify and
  coalesce are repeated until only significant-degree or move-related nodes
  remain.

  This function implements procedure Coalesce() from A. W. Appel, page 254:

  procedure Coalesce()
    let m (=copy(x,y)) in worklistMoves
    x <- GetAlias(x)
    y <- GetAlias(y)
    if y in precolored then
      let (u,v) = (y,x)
    else
      let (u,v) = (x,y)
    worklistMoves <- worklistMoves \ {m}
    if (u = v) then
      coalescedMoves <- coalescedMoves U {m}
      AddWorkList(u)
    else if v in precolored or (u,v) in adjSet then
      constrainedMoves <- constrainedMoves U {m}
      AddWorkList(u)
      AddWorkList(v)
    else if u in precolored and (forall t in Adjacent(v): OK(t,u))
            or u not in precolored and
            Conservative(Adjacent(u) U Adjacend(v)) then
      coalescedMoves <- coalescedMoves U {m}
      Combine(u,v)
      AddWorkList(u)
    else
      activeMoves <- activeMoves U {m}
*/
void WIR_GraphColoring::coalesce( WIR_InterferenceGraph &igraph )
{
  DSTART( "void WIR_GraphColoring::coalesce(WIR_InterferenceGraph&)" );

  WIR_Operation &o = selectCoalescingCandidate( igraph, mWorkListMoves );
  mWorkListMoves.erase( o );

  // Determine the registers involved in the current move operation.
  const WIR_BaseRegister &x = getUseOfMove( o );
  const WIR_BaseRegister &y = getDefOfMove( o );

  // Check whether some of these registers are pre-colored.
  const WIR_BaseRegister &u = ( isPrecolored( y ) ? y : x );
  const WIR_BaseRegister &v = ( isPrecolored( y ) ? x : y );

  if ( igraph.areSameNodes( u, v ) ) {
    // If the two registers of a move operation are already represented by the
    // same interference graph node, just add the move to mCoalescedMoves.
    mCoalescedMoves.insert( o );
    DEBUG_OP_MOV( o, "mWorkListMoves", "mCoalescedMoves" );
    DEBUG_MOVE_SETS_INVARIANT( o );

    addWorkList( u, igraph );
  } else

  // cppcheck-suppress knownConditionTrueFalse
  if ( ( isPrecolored( u ) && isPrecolored( v ) &&
       ( mPrecolored.at( const_cast<WIR_BaseRegister &>( u ) ).get() !=
           mPrecolored.at( const_cast<WIR_BaseRegister &>( v ) ).get() ) ) ||
       igraph.interfere( u, v ) ||
       areConstrainedHierarchicalRegs( u, v, igraph ) ||
       areIncompatiblyColoredNodes( u, v, igraph ) ||
       avoidCoalescing( o, u, v, igraph ) ) {
    // If both registers of a move operation are pre-colored incompatibly, or if
    // they interfere, or if they are in incompatible positions in complex
    // register hierarchies, or if coalescing yields an edge whose two nodes
    // share at least one common color, or if coalescing should be avoided in
    // very particular processor-specific situations, add the move to
    // mConstrainedMoves.
    mConstrainedMoves.insert( o );
    DEBUG_OP_MOV( o, "mWorkListMoves", "mConstrainedMoves" );
    DEBUG_MOVE_SETS_INVARIANT( o );

    addWorkList( u, igraph );
    addWorkList( v, igraph );
  } else

  if ( ( !isPrecolored( u ) && coalescingTestBriggs( u, v, igraph ) ) ||
       ( isPrecolored( u ) && coalescingTestGeorge( u, v, igraph ) ) ) {
    // Coalesce both registers of a move operation if the Briggs or the George
    // tests are successful, i.e., if coalescing is guaranteed to be safe.
    mCoalescedMoves.insert( o );
    DEBUG_OP_MOV( o, "mWorkListMoves", "mCoalescedMoves" );
    DEBUG_MOVE_SETS_INVARIANT( o );

    const WIR_BaseRegister &remainingReg = combine( u, v, igraph );
    addWorkList( igraph.getUnaliasedReg( remainingReg ), igraph );
  } else {
    mActiveMoves.insert( o );
    DEBUG_OP_MOV( o, "mWorkListMoves", "mActiveMoves" );
    DEBUG_MOVE_SETS_INVARIANT( o );
  }
};


/*
  areConstrainedHierarchicalRegs checks whether the two given registers which
  are both move-related must not be coalesced due to incompatible positions in
  complex register hierarchies.
*/
bool WIR_GraphColoring::areConstrainedHierarchicalRegs( const WIR_BaseRegister &r1,
                                                        const WIR_BaseRegister &r2,
                                                        const WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "bool WIR_GraphColoring::areConstrainedHierarchicalRegs(const WIR_BaseRegister&, const WIR_BaseRegister&, const WIR_InterferenceGraph&) const" );

  // The following comparison is only meaningful for hierarchical virtual
  // registers r1 and r2.
  if ( !r1.isVirtual() || !r2.isVirtual() ||
       ( !igraph.getUnaliasedReg( r1 ).isHierarchical() &&
         !igraph.getUnaliasedReg( r2 ).isHierarchical() ) )
    return( false );

  // Pick some arbitrary leafs of r1 and r2 which are potentially coalesced.
  WIR_BaseRegister *l1 = &(r1.getLeafs().front().get());
  WIR_BaseRegister *l2 = &(r2.getLeafs().front().get());

  // Determine the interference graph nodes' leafs representing leaf1 and leaf2.
  WIR_BaseRegister *igraphL1 = &(igraph.getUnaliasedReg( *l1 ));
  WIR_BaseRegister *igraphL2 = &(igraph.getUnaliasedReg( *l2 ));

  if ( isPrecolored( *igraphL1 ) )
    igraphL1 = &(mPrecolored.at( *igraphL1 ).get());
  if ( isPrecolored( *igraphL2 ) )
    igraphL2 = &(mPrecolored.at( *igraphL2 ).get());

  // If we coalesce a "smaller" graph node into a "larger" one (in terms of
  // numbers of leafs of hierarchical registers), we assume here that this is
  // OK since the move operation triggering this coalescing action is assumed to
  // be correct.
  if ( igraphL1->getRoot().getLeafs().size() !=
         igraphL2->getRoot().getLeafs().size() )
    return( false );

  // Starting from the graph nodes' leafs, go upwards in the register hierarchy
  // until we meet the graph's analogons of r1 and r2.
  while ( *l1 != r1 ) {
    l1 = &(l1->getParent());
    igraphL1 = &(igraphL1->getParent());
  }
  while ( *l2 != r2 ) {
    l2 = &(l2->getParent());
    igraphL2 = &(igraphL2->getParent());
  }

  // At this point, igraphL1 and igraphL2 refer to positions in the register
  // hierarchies that actually represent 'true' interference graph nodes and
  // that correspond to/alias with r1 and r2. Furthermore, both interference
  // graph nodes have the same size, i.e., the same number of leaf nodes.
  // Now, we can finally compare the positions of r1 and r2 by comparing the
  // positions of igraphL1 and igraphL2 within their respective register
  // hierarchies. A position of a sub-register within its register hierarchy
  // is unambiguously defined by its vertical distance to the hierarchy's root
  // and its horizontal distance among its siblings.
  unsigned int vertL1 = 0, vertL2 = 0;

  for ( const WIR_BaseRegister *r = igraphL1; r->isChild();
        r = &(r->getParent()), ++vertL1 ) ;
  for ( const WIR_BaseRegister *r = igraphL2; r->isChild();
        r = &(r->getParent()), ++vertL2 ) ;

  if ( vertL1 != vertL2 )
    return( true );

  unsigned int horizL1 = 0, horizL2 = 0;

  if ( igraphL1->isChild() ) {
    if ( igraphL1->isVirtual() ) {
      WIR_VirtualRegister &reg =
        dynamic_cast<WIR_VirtualRegister &>( *igraphL1 );
      auto childs = reg.getParent().getChilds();
      for ( auto it = childs.begin(); it->get() != reg; ++it, ++horizL1 ) ;
    } else {
      WIR_PhysicalRegister &reg =
        dynamic_cast<WIR_PhysicalRegister &>( *igraphL1 );
      auto childs = reg.getParent().getChilds();
      for ( auto it = childs.begin(); it->get() != reg; ++it, ++horizL1 ) ;
    }
  }

  if ( igraphL2->isChild() ) {
    if ( igraphL2->isVirtual() ) {
      WIR_VirtualRegister &reg =
        dynamic_cast<WIR_VirtualRegister &>( *igraphL2 );
      auto childs = reg.getParent().getChilds();
      for ( auto it = childs.begin(); it->get() != reg; ++it, ++horizL2 ) ;
    } else {
      WIR_PhysicalRegister &reg =
        dynamic_cast<WIR_PhysicalRegister &>( *igraphL2 );
      auto childs = reg.getParent().getChilds();
      for ( auto it = childs.begin(); it->get() != reg; ++it, ++horizL2 ) ;
    }
  }

  if ( horizL1 != horizL2 )
    return( true );

  return( false );
};


/*
  areIncompatiblyColoredNodes checks whether the two given registers which are
  both move-related must not be coalesced due to an incompatible coloring
  resulting from this coalescence.

  Suppose, a node u is to be coalesced into some other node v. During this
  coalescing, all edges between u and its neighbors n are re-directed from u to
  v. I.e., an edge (u, n) becomes (v, n) afterwards. In the case that v and n
  use some common color due to pre-coloring, then this coalescence violates the
  graph coloring property that no two adjacent nodes may have the same color.

  Another situation for an incompatible pre-coloring is: The vanishing node
  propagates its colors to the remaining node during coalescing. However, the
  remaining node is already adjacent to another neighbor having the same colors
  as the vanishing node would induce. In order to detect this scenario, we first
  add all colors induced somehow by the vanishing node to set
  colorsOfRemainingNode. After that, we check all neighbors of the remaining
  node and verify that the intersection of the neighbor's color set with
  colorsOfRemainingNode is empty.
*/
bool WIR_GraphColoring::areIncompatiblyColoredNodes( const WIR_BaseRegister &r1,
                                                     const WIR_BaseRegister &r2,
                                                     const WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "bool WIR_GraphColoring::areIncompatiblyColoredNodes(const WIR_BaseRegister&, const WIR_BaseRegister&, const WIR_InterferenceGraph&) const" );

  // Determine the remaining and vanishing graph nodes.
  WIR_BaseRegister &remainingReg = igraph.getRemainingReg( r1, r2 );
  WIR_BaseRegister &vanishingReg = igraph.getVanishingReg( r1, r2 );

  set<unsigned int> colorsOfRemainingNode =
    igraph.getColors( igraph.getUnaliasedReg( remainingReg.getRoot() ) );

  DACTION(
    DOUT(
      "Colors of remaining node " <<
      igraph.getUnaliasedReg( remainingReg.getRoot() ).getName() << ":" );
    for ( auto c : colorsOfRemainingNode )
      DOUT( " " << c );
    DOUT( endl ); );

  // Check the first situation decribed in this method's comment.
  if ( isPrecolored( remainingReg ) ) {
    auto neighbors = igraph.getNeighbors( vanishingReg );

    // Check all neighbors n of the vanishing node.
    if ( any_of(
           neighbors.begin(), neighbors.end(),
           [&]( const WIR_BaseRegister &n ) {
             set<unsigned int> colorsOfNeighborNode = igraph.getColors( n );
             set<unsigned int> intersection;

             DACTION(
               DOUT(
                 "Colors of neighbor " << n.getName() <<
                 " of vanishing node " << vanishingReg.getName() << ":" );
               for ( auto c : colorsOfNeighborNode )
                 DOUT( " " << c );
               DOUT( endl ) );

             // intersection :=
             //   colorsOfRemainingNode intersect colorsOfNeighborNode
             for ( auto c : colorsOfRemainingNode )
               if ( colorsOfNeighborNode.count( c ) )
                 intersection.insert( c );

             // If n and the remaining node share at least one common color,
             // exit.
             return( !intersection.empty() ); } ) )
      return( true );
  }

  // Check the second situation decribed in this method's comment.
  if ( isPrecolored( vanishingReg ) ) {
    WIR_BaseRegister *remainingRoot = &(igraph.getUnaliasedReg( remainingReg ));
    WIR_PhysicalRegister *precolRoot = &(mPrecolored.at( vanishingReg ).get());

    while ( remainingRoot->isChild() ) {
      remainingRoot = &(remainingRoot->getParent());
      precolRoot = &(precolRoot->getParent());
    }

    list<std::reference_wrapper<WIR_BaseRegister>> worklist;
    worklist.push_back( *remainingRoot );
    list<std::reference_wrapper<WIR_PhysicalRegister>> precolWorklist;
    precolWorklist.push_back( *precolRoot );

    do {
      WIR_BaseRegister &reg = worklist.front().get();
      worklist.pop_front();
      WIR_PhysicalRegister &precol = precolWorklist.front().get();
      precolWorklist.pop_front();

      if ( !reg.hasChilds() )
        colorsOfRemainingNode.insert( igraph.getColorOfPhreg( precol ) );

      if ( reg.isVirtual() ) {
        WIR_VirtualRegister &vreg = dynamic_cast<WIR_VirtualRegister &>( reg );
        copy( vreg.begin(), vreg.end(), back_inserter( worklist ) );
      } else {
        WIR_PhysicalRegister &vreg =
          dynamic_cast<WIR_PhysicalRegister &>( reg );
        copy( vreg.begin(), vreg.end(), back_inserter( worklist ) );
      }
      copy( precol.begin(), precol.end(), back_inserter( precolWorklist ) );
    } while ( !worklist.empty() );

    auto neighbors = igraph.getNeighbors( remainingReg );

    if ( any_of(
           neighbors.begin(), neighbors.end(),
           [&]( const WIR_BaseRegister &n ) {
             set<unsigned int> colorsOfNeighborNode = igraph.getColors( n );
             set<unsigned int> intersection;

             // intersection :=
             //   colorsOfRemainingNode intersect colorsOfNeighborNode
             for ( auto c : colorsOfRemainingNode )
               if ( colorsOfNeighborNode.count( c ) )
                 intersection.insert( c );

             // If n and the remaining node share at least one common color,
             // exit.
             return( !intersection.empty() );
           } ) )
      return( true );
  }

  return( false );
};


/*
  coalescingTestBriggs tests whether conservative coalescing of the two given
  registers according to Briggs is applicable.

  This function implements function Conservative(nodes) from A. W. Appel, page
  254:

  function Conservative(nodes)
    let k=0
    forall n in nodes
      if degree[n] >= K then k <- k + 1
    return (k < K)
*/
bool WIR_GraphColoring::coalescingTestBriggs( const WIR_BaseRegister &r1,
                                              const WIR_BaseRegister &r2,
                                              const WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "bool WIR_GraphColoring::coalescingTestBriggs(const WIR_BaseRegister&, const WIR_BaseRegister&, const WIR_InterferenceGraph&) const" );

  if ( !mCoalescingBriggs )
    return( false );

  unsigned int k = 0;
  WIR_VirtualRegisterSet neighbors1 = igraph.getNeighborVREGs( r1 );
  WIR_VirtualRegisterSet neighbors2 = igraph.getNeighborVREGs( r2 );
  neighbors1.insert( neighbors2.begin(), neighbors2.end() );

  for ( const WIR_VirtualRegister &n : neighbors1 )
    if ( !n.isChild() )
      if ( igraph.getDegree( n ) >= igraph.getAvailableColors() )
        ++k;

  return( k < igraph.getAvailableColors() );
};


/*
  coalescingTestGeorge tests whether conservative coalescing of the two given
  registers according to George is applicable.

  This function implements function OK(t,r) from A. W. Appel, page 254:

  function OK(t,r)
    degree[t] < K or t in precolored or (t,r) in adjSet
*/
bool WIR_GraphColoring::coalescingTestGeorge( const WIR_BaseRegister &r1,
                                              const WIR_BaseRegister &r2,
                                              const WIR_InterferenceGraph &igraph ) const
{
  DSTART(
    "bool WIR_GraphColoring::coalescingTestGeorge(const WIR_BaseRegister&, const WIR_BaseRegister&, const WIR_InterferenceGraph&) const" );

  if ( !mCoalescingGeorge )
    return( false );

  auto neighbors = igraph.getNeighborVREGs( r2 );

  if ( any_of(
         neighbors.begin(), neighbors.end(),
         [&]( const WIR_VirtualRegister &n ) {
           if ( !n.isChild() )
             if ( !( ( igraph.getDegree( n ) < igraph.getAvailableColors() ) ||
                     isPrecolored( n ) || igraph.interfere( n, r1 ) ) )
               return( true );
           return( false );
        } ) )
    return( false );

  return( true );
};


/*
  combine coalesces the two given interference graph nodes and updates the work
  lists.

  This function implements procedure Combine(u,v) from A. W. Appel, page 255:

  procedure Combine(u,v)
    if v in freezeWorklist then
      freezeWorklist <- freezeWorklist \ {v}
    else
      spillWorklist <- spillWorklist \ {v}
    coalescedNodes <- coalescedNodes U {v}
    alias[v] <- u
    moveList[u] <- moveList[u] U moveList[v]
    EnableMoves(v)
    forall t in Adjacent(v)
      AddEdge(t,u)
      DecrementDegree(t)
    if degree[u] >= K and u in freezeWorkList
      freezeWorkList <- freezeWorkList \ {u}
      spillWorkList <- spillWorkList U {u}
*/
WIR_BaseRegister &WIR_GraphColoring::combine( const WIR_BaseRegister &r1,
                                              const WIR_BaseRegister &r2,
                                              WIR_InterferenceGraph &igraph )
{
  DSTART(
    "WIR_BaseRegister& WIR_GraphColoring::combine(const WIR_BaseRegister&, const WIR_BaseRegister&, WIR_InterferenceGraph&)" );

  // Registers r1 and r2 do not necessarily denote roots of register
  // hierarchies. Instead, these parameters r1 and r2 are simply the registers
  // used and defined in the move operation to be coalesced.

  WIR_BaseRegister &remainingReg = igraph.getRemainingReg( r1, r2 );
  WIR_BaseRegister &vanishingReg = igraph.getVanishingReg( r1, r2 );

  WIR_VirtualRegisterSet neighbors = igraph.getNeighborVREGs( vanishingReg );

  // The vanishing graph node is always added to the set of registers to be
  // removed from mWorkListFreeze. However, if vanishingReg is pre-colored
  // before coalescing, vanishingReg will propagate its color information to
  // remainingReg. In this case, remainingReg also has to be removed from
  // mWorkListFreeze, since colored nodes are never part of some work list.
  WIR_RegisterSet regSet;
  regSet.insert( vanishingReg );
  if ( isPrecolored( vanishingReg ) )
    regSet.insert( remainingReg );

  for ( WIR_BaseRegister &r : regSet ) {
    WIR_BaseRegister &key = igraph.getUnaliasedReg( r ).getRoot();

    if ( key.isVirtual() ) {
      WIR_VirtualRegister &vkey = dynamic_cast<WIR_VirtualRegister &>( key );

      if ( mWorkListFreeze.count( vkey ) ) {
        DEBUG_FREEZE_WL_WEAK_INVARIANT( vkey, igraph );
        mWorkListFreeze.erase( vkey );
        DEBUG_REG_RM( key, "mWorkListFreeze" );
      } else

      if ( mWorkListSpill.count( vkey ) ) {
        DEBUG_SPILL_WL_INVARIANT( vkey, igraph );
        mWorkListSpill.erase( vkey );
        DEBUG_REG_RM( key, "mWorkListSpill" );
      }
    }
  }

  // Merge all move operations related to nodes r1 and r2 so that the coalesced
  // node still carries the correct information whether it is still
  // move-related. During this merge, we also have to consider all moves of the
  // coalescing partners and of leafs in the case of hierarchical registers.
  WIR_OperationSet nodeMoves1;

  WIR_RegisterSet regsToConsider;
  WIR_RegisterSet hierRegsToConsider;

  // regsToConsider := {r1,r2} U
  //                   getCoalescedAliases( r1 ) U getCoalescedAliases( r2 )
  regsToConsider.insert( const_cast<WIR_BaseRegister &>( r1 ) );
  regsToConsider.insert( const_cast<WIR_BaseRegister &>( r2 ) );

  auto aliases = igraph.getCoalescedAliases( r1 );
  regsToConsider.insert( aliases.begin(), aliases.end() );
  aliases = igraph.getCoalescedAliases( r2 );
  regsToConsider.insert( aliases.begin(), aliases.end() );

  for ( WIR_BaseRegister &r : regsToConsider ) {
    // Iterate over r1, r2, and their coalesced partners.
    list<std::reference_wrapper<WIR_BaseRegister>> workList;
    workList.push_back( r.getRoot() );

    do {
      // Iterate over all child registers of r.
      WIR_BaseRegister &currentReg = workList.front();
      workList.pop_front();
      hierRegsToConsider.insert( currentReg );

      // Assign all move operations related to the current child register to
      // set nodeMoves1.
      WIR_OperationSet moves = mMoveList[ currentReg.getID() ];

      // nodeMoves1 := nodeMoves1 U mMoveList[ currentReg ]
      nodeMoves1.insert( moves.begin(), moves.end() );

      // Add all children of the current register to the work list.
      if ( currentReg.isVirtual() ) {
        WIR_VirtualRegister &vreg =
          dynamic_cast<WIR_VirtualRegister &>( currentReg );
        copy( vreg.begin(), vreg.end(), back_inserter( workList ) );
      } else {
        WIR_PhysicalRegister &preg =
          dynamic_cast<WIR_PhysicalRegister &>( currentReg );
        copy( preg.begin(), preg.end(), back_inserter( workList ) );
      }
    } while ( !workList.empty() );
  }

  // Finally, assign all move operations collected above to mMoveList of all the
  // involved registers.
  for ( const WIR_BaseRegister &r : hierRegsToConsider )
    mMoveList[ r.getID() ] = nodeMoves1;

  // Propagate pre-color information between registers r1 and r2. We also must
  // consider all coalescing partners and register hierarchies here.
  if ( isPrecolored( r1 ) || isPrecolored( r2 ) ) {
    // First, find the common roots of the register hierarchies of r1, r2 and
    // their physical counterpart.
    WIR_BaseRegister *root1 = const_cast<WIR_BaseRegister *>( &r1 );
    WIR_BaseRegister *root2 = const_cast<WIR_BaseRegister *>( &r2 );
    WIR_PhysicalRegister *physRoot = nullptr;

    if ( isPrecolored( r1 ) )
      physRoot =
        &(mPrecolored.at( const_cast<WIR_BaseRegister &>( r1 ) ).get());
    else
      physRoot =
        &(mPrecolored.at( const_cast<WIR_BaseRegister &>( r2 ) ).get());

    while ( root1->isChild() && root2->isChild() ) {
      root1 = &(root1->getParent());
      root2 = &(root2->getParent());
      physRoot = &(physRoot->getParent());
    }

    // We now apply a worklist algorithm that propagates the pre-color
    // information into the register hierarchies rooted by root1 and root2, and
    // into their corresponding coalescing aliases, if existing.
    list<std::reference_wrapper<WIR_BaseRegister>> workList1;
    list<std::reference_wrapper<WIR_BaseRegister>> workList2;
    list<std::reference_wrapper<WIR_PhysicalRegister>> physWorkList;

    workList1.push_back( *root1 );
    workList2.push_back( *root2 );
    physWorkList.push_back( *physRoot );

    do {
      WIR_BaseRegister &reg1 = workList1.front().get();
      workList1.pop_front();
      WIR_BaseRegister &reg2 = workList2.front().get();
      workList2.pop_front();
      WIR_PhysicalRegister &physReg = physWorkList.front().get();
      physWorkList.pop_front();

      WIR_RegisterSet regsToUpdate;
      regsToUpdate.insert( reg1 );
      regsToUpdate.insert( reg2 );

      // Consider aliases here.
      if ( ( !reg1.isChild() && reg1.isVirtual() ) || !reg1.hasChilds() ) {
        auto aliases1 = igraph.getCoalescedAliases( reg1 );
        regsToUpdate.insert( aliases1.begin(), aliases1.end() );
      }

      if ( ( !reg2.isChild() && reg2.isVirtual() ) || !reg2.hasChilds() ) {
        auto aliases1 = igraph.getCoalescedAliases( reg2 );
        regsToUpdate.insert( aliases1.begin(), aliases1.end() );
      }

      // Now update the pre-color information for each individual virtual
      // register to consider.
      for ( WIR_BaseRegister &r : regsToUpdate )
        if ( r.isVirtual() ) {
          if ( !isPrecolored( r ) ) {
            mPrecolored.insert( { r, physReg } );
            DOUT(
              "mPrecolored[ " << r.getName() << " ] = " <<
              physReg.getName() << endl );
            if ( !r.hasChilds() )
              igraph.setColor( r, *(igraph.getColors( physReg ).begin()) );

            // Propagate pre-color information of r into its entire register
            // hierarchy, if any.
            WIR_BaseRegister *rRoot = &r;
            WIR_BaseRegister *pRoot = &physReg;

            while ( rRoot->isChild() && pRoot->isChild() ) {
              rRoot = &(rRoot->getParent());
              pRoot = &(pRoot->getParent());
            }

            list<std::reference_wrapper<WIR_BaseRegister>> rWL;
            list<std::reference_wrapper<WIR_PhysicalRegister>> pWL;

            rWL.push_back( *rRoot );
            pWL.push_back( dynamic_cast<WIR_PhysicalRegister &>( *pRoot ) );

            do {
              WIR_BaseRegister &rReg = rWL.front().get();
              rWL.pop_front();
              WIR_PhysicalRegister &pReg = pWL.front().get();
              pWL.pop_front();

              if ( !isPrecolored( rReg ) ) {
                mPrecolored.insert( { rReg, pReg } );
                DOUT(
                  "mPrecolored[ " << rReg.getName() << " ] = " <<
                  pReg.getName() << endl );
                if ( !rReg.hasChilds() )
                  igraph.setColor( rReg, *(igraph.getColors( pReg ).begin()) );
              } else
                ufAssert(
                  mPrecolored.at( rReg ).get().getName() == pReg.getName() );

              if ( rReg.isVirtual() )
                copy(
                  dynamic_cast<WIR_VirtualRegister &>( rReg ).begin(),
                  dynamic_cast<WIR_VirtualRegister &>( rReg ).end(),
                  back_inserter( rWL ) );
              else
                copy(
                  dynamic_cast<WIR_PhysicalRegister &>( rReg ).begin(),
                  dynamic_cast<WIR_PhysicalRegister &>( rReg ).end(),
                  back_inserter( rWL ) );
              copy( pReg.begin(), pReg.end(), back_inserter( pWL ) );
            } while ( !rWL.empty() );
          } else
            ufAssert(
              mPrecolored.at( r ).get().getName() == physReg.getName() );
        }

      // Set up work lists for child registers.
      if ( reg1.isVirtual() )
        copy(
          dynamic_cast<WIR_VirtualRegister &>( reg1 ).begin(),
          dynamic_cast<WIR_VirtualRegister &>( reg1 ).end(),
          back_inserter( workList1 ) );
      else
        copy(
          dynamic_cast<WIR_PhysicalRegister &>( reg1 ).begin(),
          dynamic_cast<WIR_PhysicalRegister &>( reg1 ).end(),
          back_inserter( workList1 ) );

      if ( reg2.isVirtual() )
        copy(
          dynamic_cast<WIR_VirtualRegister &>( reg2 ).begin(),
          dynamic_cast<WIR_VirtualRegister &>( reg2 ).end(),
          back_inserter( workList2 ) );
      else
        copy(
          dynamic_cast<WIR_PhysicalRegister &>( reg2 ).begin(),
          dynamic_cast<WIR_PhysicalRegister &>( reg2 ).end(),
          back_inserter( workList2 ) );

      copy( physReg.begin(), physReg.end(), back_inserter( physWorkList ) );
    } while ( !workList1.empty() );
  }

  enableMoves( vanishingReg, igraph );

  // Now apply the actual coalescing to the interference graph.
  igraph.coalesceNodes( r1, r2 );

  {
    DSTART(
      "WIR_BaseRegister& WIR_GraphColoring::combine(const WIR_BaseRegister&, const WIR_BaseRegister&, WIR_InterferenceGraph&).visualize" );
    DACTION( igraph.visualize(); );
  }

  // Decrement the degree of all neighbors of the vanished node.
  for ( const WIR_VirtualRegister &n : neighbors )
    if ( !n.isChild() )
      decrementDegree( n, igraph );

  // Add the remaining node to the correct work list.
  auto &key = igraph.getUnaliasedReg( remainingReg ).getRoot();

  if ( key.isVirtual() &&
       ( igraph.getDegree( key ) >= igraph.getAvailableColors() ) &&
       mWorkListFreeze.count( dynamic_cast<WIR_VirtualRegister &>( key ) ) ) {
    auto &vKey = dynamic_cast<WIR_VirtualRegister &>( key );
    mWorkListFreeze.erase( vKey );
    DEBUG_REG_RM( vKey, "mWorkListFreeze" );

    mWorkListSpill.insert( vKey );
    DEBUG_SPILL_WL_INVARIANT( vKey, igraph );
    DEBUG_REG_ADD( vKey, "mWorkListSpill" );

    for ( const WIR_BaseRegister &r : igraph.getCoalescedAliases( key ) )
      if ( r.getRoot().isVirtual() &&
           mWorkListFreeze.count(
            dynamic_cast<WIR_VirtualRegister &>( r.getRoot() ) ) ) {
        mWorkListFreeze.erase(
          dynamic_cast<WIR_VirtualRegister &>( r.getRoot() ) );
        DEBUG_REG_RM( r.getRoot(), "mWorkListFreeze" );
      }
  }

  return( remainingReg );
};


/*
  freeze takes an interference graph node and freezes all moves in which this
  node is involved.

  If neither simplify nor coalesce applies, we look for a move-related node of
  low degree. We freeze the moves in which this node is involved: that is, we
  give up hope of coalescing these moves. This causes the node (and perhaps
  other nodes related to the frozen moves) to be considered non-move-related,
  which should enable more simplification. Now, simplify and coalesce are
  resumed.

  This function implements procedure Freeze() from A. W. Appel, page 255:

  procedure Freeze()
    let u in freezeWorklist
    freezeWorklist <- freezeWorklist \ {u}
    simplifyWorklist <- simplifyWorklist U {u}
    FreezeMoves(u)
*/
void WIR_GraphColoring::freeze( WIR_InterferenceGraph &igraph )
{
  DSTART( "void WIR_GraphColoring::freeze(WIR_InterferenceGraph&)" );

  WIR_VirtualRegister &r = mWorkListFreeze.begin()->get();
  DOUT( "Freezing node '" + r.getName() << "'." << endl );
  DEBUG_FREEZE_WL_INVARIANT( r, igraph );
  mWorkListFreeze.erase( mWorkListFreeze.begin() );

  freezeMoves( r, igraph );

  if ( !mSpillAll || isPriorityRegister( r ) ) {
    mWorkListSimplify.insert( r );
    DEBUG_REG_MOV( r, "mWorkListFreeze", "mWorkListSimplify" );
    DEBUG_SIMPLIFY_WL_INVARIANT( r, igraph );
  } else {
    mWorkListSpill.insert( r );
    DEBUG_REG_MOV( r, "mWorkListFreeze", "mWorkListSpill" );
  }
};


/*
  assignColors finally assigns colors to the interference graph nodes.

  This function implements procedure AssignColors() from A. W, Appel, page
  256:

  procedure AssignColors()
    while SelectStack not empty
      let n = pop(SelectStack)
      okColors <- {0, ..., K-1}
      forall w in adjList[n]
        if GetAlias(w) in (coloredNodes U precolored) then
          okColors <- okColors \ {color[GetAlias(w)]}
      if okColors = {} then
        spilledNodes <- spilledNodes U {n}
      else
        coloredNodes <- coloredNodes U {n}
        let c in okColors
        color[n] <- c
    forall n in coalescedNodes
      color[n] <- color[GetAlias(n)]
*/
void WIR_GraphColoring::assignColors( WIR_InterferenceGraph &igraph )
{
  DSTART( "void WIR_GraphColoring::assignColors(WIR_InterferenceGraph&)" );

  mRegisterMapping.clear();
  mSpillAliases.clear();
  mSpilledNodes.clear();
  mTotalSpillCosts = 0;

  if ( mVerbosity )
    ufProgrMsg << ufFile() << "Assigning colors." << endl;

  // Restore the original interference graph by popping nodes from its stack.
  while ( !igraph.isStackEmpty() ) {
    // Fetch a node from the stack.
    WIR_VirtualRegister &r = igraph.popNode();
    DEBUG_REG_POP( r );

    ufAssert( !igraph.isColored( r ) );

    auto leafs = r.getLeafs();
    WIR_ColorMap colorMap;
    if ( !mSpillAll || mSpillClones.count( r.getID() ) ||
         isPriorityRegister( r ) )
      colorMap = selectColors( leafs, igraph );

    DEBUG_COLOR_MAP_INVARIANTS1( r, colorMap, leafs, igraph );

    // Create the full alias information for r, i.e., which other registers are
    // coalesced into r, for all levels of a potential complex register
    // hierarchy.
    map<WIR_id_t, WIR_RegisterSet> aliases;
    list<std::reference_wrapper<WIR_VirtualRegister>> workList;
    set<WIR_id_t> regAddedToWorkList;

    for ( WIR_VirtualRegister &reg : leafs ) {
      workList.push_back( reg );
      aliases[ reg.getID() ] = igraph.getCoalescedAliases( reg );
    }

    // Propagate aliases bottom-up through the register hierarchy.
    do {
      WIR_VirtualRegister &reg = workList.front();
      workList.pop_front();
      WIR_VirtualRegister &parent = reg.getParent();

      if ( parent != reg ) {
        for ( const WIR_BaseRegister &alias : aliases[ reg.getID() ] )
          if ( alias.isChild() )
            aliases[ parent.getID() ].insert( alias.getParent() );

        if ( !regAddedToWorkList.count( parent.getID() ) ) {
          workList.push_back( parent );
          regAddedToWorkList.insert( parent.getID() );
        }
      }
    } while ( !workList.empty() );

    // Finally, propagate aliases top-down through the register hierarchy.
    workList.push_back( r );

    do {
      WIR_VirtualRegister &reg = workList.front();
      workList.pop_front();

      for ( WIR_BaseRegister &alias : aliases[ reg.getID() ] ) {
        ufAssert(
          reg.getChilds().size() ==
          ( alias.isVirtual() ?
              dynamic_cast<WIR_VirtualRegister &>( alias ).getChilds().size() :
              dynamic_cast<WIR_PhysicalRegister &>(
                alias ).getChilds().size() ) );

        auto regIt = reg.getChilds().begin();

        if ( alias.isVirtual() ) {
          auto aliasIt =
            dynamic_cast<WIR_VirtualRegister &>( alias ).getChilds().begin();

          for ( ; regIt != reg.getChilds().end(); ++regIt, ++aliasIt )
            aliases[ regIt->get().getID() ].insert( aliasIt->get() );
        } else {
          auto aliasIt =
            dynamic_cast<WIR_PhysicalRegister &>( alias ).getChilds().begin();

          for ( ; regIt != reg.getChilds().end(); ++regIt, ++aliasIt )
            aliases[ regIt->get().getID() ].insert( aliasIt->get() );
        }
      }

      copy( reg.begin(), reg.end(), back_inserter( workList ) );
    } while ( !workList.empty() );

    // If the processor-specific coloring fails to assign a color, r is spilled.
    if ( colorMap.empty() ) {
      // r is an actual spill.

      if ( !mUncoloredActualSpills.count( r ) ) {
        // r is not a clone of some original actually spilled register, r has
        // thus not been generated during live range splitting of actual spills.
        // We now mark r as true actual spill.
        unsigned int spillCosts = igraph.getSpillCosts( r );

        mSpilledNodes.insert( r );
        mTotalSpillCosts += spillCosts;
        igraph.setActualSpill( r );

        DACTION(
          DOUT( "Selecting register " << r.getName() << " (leafs:" );
          for ( const WIR_VirtualRegister &leaf : leafs )
            DOUT( " " << leaf.getName() );
          DOUT(
            ") with spill costs " << spillCosts << " as actual spill." <<
            endl ); );

        // Build mSpillAliases, including child registers.
        workList.push_back( r );

        do {
          WIR_VirtualRegister &reg = workList.front();
          workList.pop_front();

          for ( const WIR_BaseRegister &alias : aliases[ reg.getID() ] ) {
            mSpillAliases.insert( { alias.getID(), reg } );

            DOUT(
              "Selecting aliased register " << alias.getName() <<
              " with spill costs " << spillCosts << " as actual spill." <<
              endl );
          }

          // Add childs to the work list.
          copy( reg.begin(), reg.end(), back_inserter( workList ) );
        } while ( !workList.empty() );
      } else {
        // r is a clone of some original actually spilled register, r has thus
        // been generated during live range splitting of actual spills.
        // We now simply skip r, since spilling of tiny live ranges resulting
        // from the fetches of previously spilled registers makes no sense. All
        // these "very tough" nodes in mUncoloredActualSpills will be allocated
        // in a post-processing phase by method allocateUncoloredActualSpills.
        DOUT( "Skipping cloned register " << r.getName() << "." << endl );
      }
    } else {
      // r is not an actual spill.
      bool wasPotentialSpill = igraph.isPotentialSpill( r );

      // Color the node.
      for ( auto &[ reg, color ] : colorMap )
        igraph.setColor( reg.get(), color );

      if ( mUncoloredActualSpills.count( r ) )
        mUncoloredActualSpills.erase( r );

      // Propagate the color mapping through the entire register hierarchy
      // rooted by r, including all coalescing aliases.

      // First, find the physical register root corresponding to the virtual
      // register root r.
      WIR_VirtualRegister *tmpReg = &(leafs.front().get());
      WIR_PhysicalRegister *tmpPhReg =
        &(igraph.getPhregOfColor( colorMap[ *tmpReg ] ));

      while ( tmpReg->isChild() ) {
        tmpReg = &(tmpReg->getParent());
        ufAssert( tmpPhReg->isChild() );
        tmpPhReg = &(tmpPhReg->getParent());
      }

      ufAssert( *tmpReg == r );

      // Second, apply a worklist algorithm to the entire register hierarchy.
      list<std::reference_wrapper<WIR_VirtualRegister>> workList1;
      list<std::reference_wrapper<WIR_PhysicalRegister>> physWorkList;

      workList1.push_back( r );
      physWorkList.push_back( *tmpPhReg );

      do {
        WIR_VirtualRegister &vreg = workList1.front();
        workList1.pop_front();
        WIR_PhysicalRegister &phreg = physWorkList.front();
        physWorkList.pop_front();

        // Retrieve all aliases of the currently considered vreg.
        WIR_RegisterSet regs = aliases[ vreg.getID() ];
        regs.insert( vreg );

        // Now do the actual register assignment.
        for ( const WIR_BaseRegister &reg : regs )
          if ( reg.isVirtual() ) {
            mRegisterMapping.insert( { reg.getID(), phreg } );
            DOUT(
              "Assigning " << reg.getName() << " (ID " << reg.getID() <<
              ") to " << phreg.getName() << " (ID " << phreg.getID() << ")." <<
              endl );

            DEBUG_COLOR_MAP_INVARIANTS2( reg, phreg, colorMap, igraph );

            // If the current node is marked as potential spill and if we now
            // see that it is colorable though, ensure that potential
            // rematerialization instructions are deleted properly.
            if ( wasPotentialSpill ) {
              WIR_RegisterParameterSet deletedRematParams;

              for ( auto &[ rp, instrs ] : mRematerializationInstructions ) {
                // cppcheck-suppress variableScope
                DACTION(
                  BBPOS( rp.get().getOperation() );
                  DOUT(
                    "Checking rematerialization instructions associated " <<
                    "with parameter " << rp << "/" <<
                    rp.get().getRegister().getName() << " of operation " <<
                    BBID << rp.get().getOperation() << endl ); );

                if ( rp.get().getRegister() == reg ) {
                  DOUT( "Deleting rematerialization instructions" );
                  for ( const auto *i : instrs ) {
                    DOUT( " " << i->getID() );
                    delete( i );
                  }
                  DOUT( endl );

                  instrs.clear();
                  deletedRematParams.insert( rp );
                }
              }

              for ( WIR_RegisterParameter &p : deletedRematParams )
                mRematerializationInstructions.erase( p );
            }
          }

        // Add childs to the work lists.
        auto it1 = vreg.begin();
        auto it2 = phreg.begin();
        for ( ; it1 != vreg.end(); ++it1, ++it2 ) {
          workList1.push_back( *it1 );
          physWorkList.push_back( *it2 );
        }
      } while ( !workList1.empty() );
    }
  }

  // Finally, also add all pre-colors to the register assignment.
  for ( auto &[ reg, precol ] : mPrecolored )
    if ( reg.get().isVirtual() ) {
      mRegisterMapping.insert( { reg.get().getID(), precol } );
      auto &vreg = dynamic_cast<WIR_VirtualRegister &>( reg.get() );

      DOUT(
        "Assigning " << vreg.getName() << " (ID " << reg.get().getID() <<
        ") to " << precol.get().getName() << " (ID " << precol.get().getID() <<
        ")." << endl );

      if ( mUncoloredActualSpills.count( vreg ) )
        mUncoloredActualSpills.erase( vreg );
    }

  mPrecolored.clear();
};


/*
  rewriteProgram transforms the specified %WIR function according to the
  decisions taken by the register allocator.

  Basically, rewriteProgram transforms registers according to the found
  coloring, coalesces moves and inserts spill code.

  This function implements procedure RewriteProgram() from A. W. Appel, page
  256:

  procedure RewriteProgram()
    Allocate memory locations for each v in spilledNodes,
    Create a new temporary v_i for each definition and each use,
    In the program (instructions), insert a store after each definition of a
      v_i, a fetch before each use of a v_i.
    Put all the v_i into a set newTemps.
    spilledNodes <- {}
    initial <- coloredNodes U coalescedNodes U newTemps
    coloredNodes <- {}
    coalescedNodes <- {}
*/
void WIR_GraphColoring::rewriteProgram( WIR_Function &f )
{
  DSTART( "void WIR_GraphColoring::rewriteProgram(WIR_Function&)" );

  if ( mVerbosity )
    ufProgrMsg << ufFile() << "Rewriting WIR." << endl;

  computeStackLocations( f );
  replaceCoalescedSpills( f );
  insertSpillCode( f );
  removeCoalescedMoves();
  replaceColoredRegisters( f );

  // All caller-saved registers alive during function calls identified in this
  // current allocation round are now made permanent by storing them in
  // mCallerSavedRegsAliveAcrossCall.
  for ( auto &[ call, regs ] : mLocalCallerSavedRegsAliveAcrossCall )
    for ( WIR_BaseRegister &r : regs )
      mCallerSavedRegsAliveAcrossCall[ call ].insert( r );
  mLocalCallerSavedRegsAliveAcrossCall.clear();

  // All dummy parameters identified in this current allocation round are now
  // made permanent by storing them in mDummyParameters.
  for ( auto &[ phreg, call ] : mLocalDummyParameters )
    mDummyParameters.push_back(
      call.get().begin()->get().pushBackParameter(
        WIR_RegisterParameter( phreg.get(), WIR_Usage::use, true ) ) );
  mLocalDummyParameters.clear();
};


/*
  For each actual spill in mSpilledNodes, computeStackLocations computes its
  relative place in the specified function's stack frame and stores it in
  mStackByteOffset.
*/
void WIR_GraphColoring::computeStackLocations( const WIR_Function &f )
{
  DSTART(
    "void WIR_GraphColoring::computeStackLocations(const WIR_Function&)" );

  DDECLARE(
    unsigned int newStackSpace = 0; );

  if ( mCoalesceStackLocations ) {
    // Apply intelligent stack placement where any two actual spills that are
    // not simultaneously alive are mapped to the same stack location (cf. A. W.
    // Appel, page 242).

    // First, all registers not being actual spills are removed from the
    // original interference graph.
    // A. W. Appel, page 242:
    // 1. Use liveness information to construct the interference graph for
    //    spilled nodes.

    // First, mark spill-related nodes as actual spills and hide away all nodes
    // that are not spill-related.
    set<WIR_id_t> spilledKeyRegs;

    for ( const WIR_VirtualRegister &vreg : mSpilledNodes )
      if ( mBestIgraph.containsNode( vreg ) ) {
        const WIR_BaseRegister &key =
          mBestIgraph.getUnaliasedReg( vreg ).getRoot();

        DOUT( "Setting node " << key.getName() << " as actual spill." << endl );
        spilledKeyRegs.insert( key.getID() );
        mBestIgraph.setActualSpill(
          const_cast<WIR_VirtualRegister &>(
            dynamic_cast<const WIR_VirtualRegister &>( key ) ) );
      }

    for ( const WIR_VirtualRegister &vreg : f.getVirtualRegisters() )
      if ( mBestIgraph.containsNode( vreg ) ) {
        const WIR_BaseRegister &key =
          mBestIgraph.getUnaliasedReg( vreg ).getRoot();

        if ( !spilledKeyRegs.count( key.getID() ) && key.isVirtual() ) {
          DOUT( "Pushing node " << key.getName() << "." << endl );
          mBestIgraph.pushNode(
            const_cast<WIR_VirtualRegister &>(
              dynamic_cast<const WIR_VirtualRegister &>( key ) ) );
        }
      }

    // A. W. Appel, page 242:
    // 2. While there is any pair of non-interfering spilled nodes connected by
    //    a move instruction, coalesce them.
    for ( auto it1 = mSpilledNodes.begin(); it1 != mSpilledNodes.end();
          ++it1 ) {
      auto it2 = it1;
      ++it2;

      for ( ; it2 != mSpilledNodes.end(); ++it2 ) {
        if ( mBestIgraph.containsNode( *it1 ) &&
             mBestIgraph.containsNode( *it2 ) ) {
          WIR_VirtualRegister &spill1 =
            dynamic_cast<WIR_VirtualRegister &>(
              mBestIgraph.getUnaliasedReg( *it1 ).getRoot() );
          WIR_VirtualRegister &spill2 =
            dynamic_cast<WIR_VirtualRegister &>(
              mBestIgraph.getUnaliasedReg( *it2 ).getRoot() );

          DOUT(
            "Checking pair " << spill1.getName() << "/" << spill2.getName() <<
            " for coalescing." << endl );

          if ( !mBestIgraph.interfere( spill1, spill2 ) &&
               !mBestIgraph.areSameNodes( spill1, spill2 ) ) {
            // If we find two distinct spilled nodes that do not interfere, we
            // coalesce them. In case that both spilled graph nodes have a
            // different number of leafs in their register hierarchy, we call a
            // processor-specific resolution function.
            std::reference_wrapper<WIR_VirtualRegister> coal1 = spill1;
            std::reference_wrapper<WIR_VirtualRegister> coal2 = spill2;

            if ( spill1.getLeafs().size() != spill2.getLeafs().size() )
              tie( coal1, coal2 ) =
                resolveSpillCoalescingConflict( spill1, spill2 );

            DOUT(
              "Coalescing nodes " << coal1.get().getName() << " and " <<
              coal2.get().getName() << "." << endl );

            // Uncolor the nodes to be coalesced just to make sure that no
            // assertion is thrown by the interference graph. Since we just
            // merge actual spills on the stack, the coloring of mBestIgraph
            // does not matter at all here.
            for ( const WIR_VirtualRegister &c : coal1.get().getLeafs() )
              mBestIgraph.setColor( c, 0 );
            for ( const WIR_VirtualRegister &c : coal2.get().getLeafs() )
              mBestIgraph.setColor( c, 0 );
            mBestIgraph.coalesceNodes( coal1.get(), coal2.get() );
          }
        }
      }
    }

    {
      DSTART(
        "void WIR_GraphColoring::computeStackLocations(const WIR_Function&).visualize" );
      DACTION( mBestIgraph.visualize(); );
    }

    // A. W. Appel, page 242:
    // 3. Color the graph. There is no (further) spilling in this coloring;
    //    instead, just pick a node and the first available color, without any
    //    predetermined limit on the number of colors.
    // 4. The colors correspond to activation-record locations for the spilled
    //    variables.

    // Collect all distinct nodes of this coalesced spill graph.
    WIR_RegisterSet spillNodes;

    for ( const WIR_VirtualRegister &vreg : mSpilledNodes )
      spillNodes.insert( mBestIgraph.getUnaliasedReg( vreg ).getRoot() );

    // Assign stack positions to all these spilled nodes.
    for ( WIR_BaseRegister &r : spillNodes ) {
      DOUT( "Assigning color to node " << r.getName() << "." << endl );

      unsigned int occupiedStackSpace = 0;

      // Allocate all top-level registers of this spill node to the current
      // stack position, prepare a worklist for potential child registers.
      WIR_RegisterSet workList = mBestIgraph.getCoalescedAliases( r.getRoot() );
      workList.insert( r );
      WIR_VirtualRegisterSet childWorkList;

      for ( const WIR_BaseRegister &reg : workList ) {
        DOUT( "Processing workList entry " << reg.getName() << "." << endl );

        if ( reg.isVirtual() ) {
          mStackByteOffset[ reg.getID() ] = mAdditionalStackSpace;

          DOUT(
            reg.getName() << " is spilled to stack position " <<
            mStackByteOffset[ reg.getID() ] << "." << endl );

          unsigned int stackSize = getStackSize( reg );
          if ( stackSize > occupiedStackSpace )
            occupiedStackSpace = stackSize;

          for ( auto &c :
                  const_cast<WIR_VirtualRegister &>(
                    dynamic_cast<const WIR_VirtualRegister &>( reg ) ) )
            childWorkList.insert( c );
        }
      }

      // If a hierarchical register is spilled, we also have to store the stack
      // positions of all sub-registers.
      while ( !childWorkList.empty() ) {
        WIR_VirtualRegister &reg = childWorkList.begin()->get();
        childWorkList.erase( reg );

        mStackByteOffset[ reg.getID() ] =
          getStackPosOfSubReg( reg, mAdditionalStackSpace );

        DOUT(
          reg.getName() << " is spilled to stack position " <<
          mStackByteOffset[ reg.getID() ] << "." << endl );

        for ( WIR_BaseRegister &alias : mBestIgraph.getCoalescedAliases( reg ) )
          if ( alias.isVirtual() ) {
            auto &vr = dynamic_cast<WIR_VirtualRegister &>( alias );

            mStackByteOffset[ vr.getID() ] =
              getStackPosOfSubReg( vr, mAdditionalStackSpace );

            DOUT(
              vr.getName() << " is spilled to stack position " <<
              mStackByteOffset[ vr.getID() ] << "." << endl );
          }

        unsigned int stackSize = getStackSize( reg );
        if ( stackSize > occupiedStackSpace )
          occupiedStackSpace = stackSize;

        for ( WIR_VirtualRegister &c : reg )
          childWorkList.insert( c );
      }

      // Finally advance the stack frame by the occupied amount of bytes.
      mAdditionalStackSpace += occupiedStackSpace;
      DACTION(
        newStackSpace += occupiedStackSpace; );
    }
  } else {
    // Apply a simple stack placement where each actual spill gets its very own
    // dedicated storage.

    // Iterate over all actual spills.
    for ( WIR_VirtualRegister &vreg : mSpilledNodes ) {
      mStackByteOffset[ vreg.getID() ] = mAdditionalStackSpace;
      mAdditionalStackSpace += getStackSize( vreg );
      DACTION(
        newStackSpace += getStackSize( vreg ); );

      DOUT(
        vreg.getName() << " is spilled to stack position " <<
        mStackByteOffset[ vreg.getID() ] << "." << endl );

      if ( vreg.isHierarchical() ) {
        // If a hierarchical register is spilled, we also have to store the
        // stack positions of all sub-registers of vreg.
        list<std::reference_wrapper<WIR_VirtualRegister>> workList;
        workList.push_back( vreg );

        do {
          const WIR_VirtualRegister &reg = workList.front();
          workList.pop_front();

          if ( reg != vreg ) {
            mStackByteOffset[ reg.getID() ] =
              getStackPosOfSubReg( reg, mStackByteOffset[ vreg.getID() ] );

            DOUT(
              reg.getName() << " is spilled to stack position " <<
              mStackByteOffset[ reg.getID() ] << "." << endl );
          }

          copy( reg.begin(), reg.end(), back_inserter( workList ) );
        } while ( !workList.empty() );
      }
    }
  }

  DACTION(
    if ( newStackSpace != 0 )
      DOUT(
        "Additional stack space for spilling in function '" << f.getName() <<
        "': " << newStackSpace << "." << endl ); );
};


/*
  replaceCoalescedSpills scans the specified WIR function and replaces all
  occurrences of spilled coalesced virtual registers in mSpillAliases by their
  unaliased registers.
*/
void WIR_GraphColoring::replaceCoalescedSpills( WIR_Function &f )
{
  DSTART( "void WIR_GraphColoring::replaceCoalescedSpills(WIR_Function&)" );

  for ( WIR_BasicBlock &b : f )
    for ( WIR_Instruction &i : b )
      for ( WIR_Operation &o : i ) {
        auto it = o.begin();
        while ( it != o.end() )
          if ( it->get().getType() == WIR_ParameterType::reg ) {
            auto &rp = dynamic_cast<WIR_RegisterParameter &>( it->get() );
            WIR_BaseRegister &r = rp.getRegister();

            if ( mSpillAliases.count( r.getID() ) ) {
              WIR_BaseRegister &unaliasedReg =
                mSpillAliases.at( r.getID() ).get();

              // This lambda is used for the actual parameter replacement and
              // deletion.
              auto replaceParam = [&]( WIR_Operation &o,
                                       list<std::reference_wrapper<WIR_Parameter>>::const_iterator it,
                                       WIR_RegisterParameter &rp,
                                       const WIR_BaseRegister &ur ) {
                auto res = o.getParameters().end();
                bool replaceParameter = true;
                const WIR_RegisterParameter *newParam = nullptr;

                // If the potential new parameter is implicit and is already
                // part of operation o, we do not add the new parameter yet
                // another time, it's redundant.
                if ( rp.isImplicit() )
                  for ( WIR_Parameter &p : o )
                    if ( p.getType() == WIR_ParameterType::reg ) {
                      const auto &implRp =
                        dynamic_cast<const WIR_RegisterParameter &>( p );

                      if ( implRp.isImplicit() &&
                          ( implRp.getRegister() == ur ) &&
                          ( implRp.getUsage() == rp.getUsage() ) ) {
                        replaceParameter = false;
                        newParam = dynamic_cast<WIR_RegisterParameter *>( &p );
                        break;
                      }
                    }

                // Do replacement/deletion of rp at position it.
                if ( replaceParameter ) {
                  // cppcheck-suppress variableScope
                  DACTION(
                    BBPOS( o );
                    DOUT(
                      "Replacing " << rp.getRegister().getName() << " by " <<
                      ur.getName() << " in " << BBID << o << endl ) );

                  res = o.replaceParameter(
                    it,
                    WIR_RegisterParameter(
                      ur, rp.getUsage(), rp.isImplicit() ) );
                  newParam =
                    dynamic_cast<WIR_RegisterParameter *>( &(res->get() ) );
                  ++res;
                } else {
                  // cppcheck-suppress variableScope
                  DACTION(
                    BBPOS( o );
                    DOUT(
                      "Removing redundant parameter " <<
                      rp.getRegister().getName() << " from " << BBID << o <<
                      endl ) );

                  res = o.eraseParameter( it );
                }

                // Finally, update mRematerializationInstructions.
                if ( mRematerializationInstructions.count( rp ) ) {
                  mRematerializationInstructions[
                    *(const_cast<WIR_RegisterParameter *>( newParam)) ] =
                    mRematerializationInstructions[ rp ];
                  mRematerializationInstructions.erase( rp );
                }

                return( res );
              };

              it = replaceParam( o, it, rp, unaliasedReg );

              // Furthermore, check if this parameter replacement also needs to
              // be applied to rematerialization instructions.
              for ( auto mapEntry : mRematerializationInstructions ) {
                for ( WIR_Instruction *instr : mapEntry.second )
                  for ( WIR_Operation &op : *instr ) {
                    auto it1 = op.begin();
                    while ( it1 != op.end() )
                      if ( it1->get().getType() == WIR_ParameterType::reg ) {
                        auto &rparam =
                          dynamic_cast<WIR_RegisterParameter &>( it1->get() );

                        if ( rparam.getRegister() == r )
                          it1 = replaceParam( op, it1, rparam, unaliasedReg );
                        else
                          ++it1;
                      } else
                        ++it1;
                  }
              }
            } else
              ++it;
          } else
            ++it;
      }

  mSpillAliases.clear();
};


/*
  insertSpillCode inserts spill code into the %WIR for all actual spills in
  mSpilledNodes.

  The structure of insertSpillCode corresponds to that of computeSpillCosts
  for obvious reasons.
*/
void WIR_GraphColoring::insertSpillCode( WIR_Function &f )
{
  DSTART( "void WIR_GraphColoring::insertSpillCode(WIR_Function&)" );

  // This lambda is used for cloning some virtual register.
  auto cloneRegister = [&]( const WIR_VirtualRegister &r ) {
    DSTART(
      "WIR_GraphColoring::insertSpillCode(WIR_Function&)::<cloneRegister(const WIR_VirtualRegister&)>" );

    WIR_VirtualRegister clone { r };
    WIR_VirtualRegister *res =
      &(f.pushBackVirtualRegister( std::move( clone ) ));
    mSpillClones.insert( res->getID() );

    DACTION(
      DOUT( "Creating new register '" << res->getName() << "'" );
      for ( auto it = res->begin(); it != res->end(); ++it ) {
        if ( it == res->begin() )
          DOUT( " (" << it->get().getName() );
        else
          DOUT( "/" << it->get().getName() );
        if ( it->get() == res->rbegin()->get() )
          DOUT( ")" );
      }
      DOUT(
        " as clone for '" << r.getName() << "' in function '" << f.getName() <<
        "'." << endl ); );

    return( res );
  };

  WIR_OperationSet movesToBeDeleted;

  for ( WIR_BasicBlock &b : f ) {
    map<WIR_id_t, bool> stillLiveInSameBB;
    map<WIR_id_t, std::reference_wrapper<WIR_VirtualRegister>> newCloneFor;

    for ( auto iit = b.begin(); iit != b.end(); ++iit ) {
      WIR_VirtualRegisterSet trueLivenessInSameBB;
      WIR_VirtualRegisterSet falseLivenessInSameBB;

      for ( WIR_Operation &o : iit->get() )
        for ( auto it = o.begin(); it != o.end(); ++it )
          if ( ( it->get().getType() == WIR_ParameterType::reg ) &&
               dynamic_cast<WIR_RegisterParameter &>(
                 it->get() ).getRegister().isVirtual() ) {
            auto &rp = dynamic_cast<WIR_RegisterParameter &>( it->get() );
            auto &r = dynamic_cast<WIR_VirtualRegister &>( rp.getRegister() );

            if ( mSpilledNodes.count( r.getRoot() ) &&
                 !mCoalescedMoves.count( o ) ) {
              // The current parameter rp refers to a spilled virtual register.
              if ( o.isMove() && !movesToBeDeleted.count( o ) &&
                   !isStackPointer( getUseOfMove( o ) ) &&
                   !isStackPointer( getDefOfMove( o ) ) ) {

                // The current operation is a move where some parameter is
                // spilled.

                auto &use = getUseOfMove( o );
                auto &def = getDefOfMove( o );

                if ( use.isVirtual() && def.isVirtual() && ( def != use ) &&
                     mSpilledNodes.count(
                       dynamic_cast<WIR_VirtualRegister &>( use ).getRoot() ) &&
                     mSpilledNodes.count(
                       dynamic_cast<WIR_VirtualRegister &>(
                         def ).getRoot() ) ) {
                  // This is a move where both operand registers are spilled.
                  // Such a move
                  //
                  //   MOV def, use;
                  //
                  // thus becomes:
                  //
                  //   LD clone, <mem[ use ]>;
                  //   ST <mem[ def ]>, clone;

                  // cppcheck-suppress variableScope
                  DACTION(
                    BBPOS( o );
                    DOUT(
                      endl <<
                      "Spill-move scenario (1), both operands of move " <<
                      "spilled. " << BBID << o << endl ); );

                  WIR_VirtualRegister *clone = nullptr;

                  if ( !stillLiveInSameBB.count( use.getID() ) ||
                       ( stillLiveInSameBB[ use.getID() ] == false ) ) {
                    clone = cloneRegister( r );
                    mUncoloredActualSpills.insert( *clone );
                    mStackByteOffset[ clone->getID() ] =
                      mStackByteOffset[ use.getID() ];

                    if ( mRematerializationInstructions.count( rp ) )
                      insertRematerializationCode( *clone, r, rp, b, iit );
                    else {
                      DOUT(
                        "Inserting { spill-load, " << clone->getName() <<
                        ", " << mStackByteOffset[ use.getID() ] <<
                        " } for used register '" << use.getName() <<
                        "' before the move." << endl );

                      insertSpillLoad(
                        *clone, use, mStackByteOffset[ use.getID() ], b, iit );
                    }
                  } else
                    clone = &(newCloneFor.at( use.getID() ).get());

                  movesToBeDeleted.insert( o );
                  DOUT( "Marking move for deletion." << endl );

                  DOUT(
                    "Inserting { spill-store, " << clone->getName() <<
                    ", " << mStackByteOffset[ def.getID() ] <<
                    " } for defined register '" << def.getName() <<
                    "' after the move." << endl );

                  auto pos = iit;
                  ++pos;
                  insertSpillStore(
                    *clone, def, mStackByteOffset[ def.getID() ], b, pos );

                  // After this current move, previous values of def, its childs
                  // and its parents are no more alive.
                  propagateFalseLiveness(
                    dynamic_cast<WIR_VirtualRegister &>( def ), b,
                    falseLivenessInSameBB );
                } else

                if ( r == use ) {
                  // This is a move where the used operand is spilled.
                  // cppcheck-suppress variableScope
                  DACTION(
                    BBPOS( o );
                    DOUT(
                      endl <<
                      "Spill-move scenario (2), used operand '" <<
                      r.getName() << "' of move spilled. " << BBID << o <<
                      endl ); );

                  if ( !stillLiveInSameBB.count( r.getID() ) ||
                       ( stillLiveInSameBB[ r.getID() ] == false ) ) {
                    // The current parameter is used in a move operation. We
                    // generate a spill-load right before the move which loads
                    // the value from r's position on the stack right into the
                    // def-register of the move.
                    //
                    //   MOV def, r;
                    //
                    // would naively become:
                    //
                    //   LD r, mem<mStackByteOffset[ r ]>;
                    //   MOV def, r;
                    //
                    // which can be optimized to:
                    //
                    //   LD def, mem<mStackByteOffset[ r ]>;

                    if ( mRematerializationInstructions.count( rp ) )
                      insertRematerializationCode( def, r, rp, b, iit );
                    else {
                      DOUT(
                        "Inserting { spill-load, " << def.getName() <<
                        ", " << mStackByteOffset[ r.getID() ] <<
                        " } for used register '" << r.getName() <<
                        "' before the move." << endl );

                      insertSpillLoad(
                        def, r, mStackByteOffset[ r.getID() ], b, iit );
                    }

                    movesToBeDeleted.insert( o );
                    DOUT( "Marking move for deletion." << endl );
                  } else {
                    // If the register is already alive in the current basic
                    // block b, we do not need yet another spill-load. Instead,
                    // we simply re-use the cloned register for r that must
                    // exist.
                    it =
                      o.replaceParameter(
                        it,
                        WIR_RegisterParameter(
                          newCloneFor.at( r.getID() ), rp.getUsage(),
                          rp.isImplicit() ) );

                    DOUT(
                      "Replacing '" << r.getName() << "' by '" <<
                      newCloneFor.at( r.getID() ).get().getName() <<
                      "' in the move." << endl );
                  }
                } else {
                  // This is a move where the defined operand is spilled.
                  // cppcheck-suppress variableScope
                  DACTION(
                    BBPOS( o );
                    DOUT(
                      endl <<
                      "Spill-move scenario (3), defined operand '" <<
                      r.getName() << "' of move spilled. " << BBID << o <<
                      endl ); );

                  // The current parameter is defined by a move operation. We
                  // generate a spill-store right after the move which stores
                  // the value of the use-register of the move to r's position
                  // on the stack.
                  //
                  //   MOV r, use;
                  //
                  // would naively become:
                  //
                  //   MOV r, use;
                  //   ST r, mem<mStackByteOffset[ r ]>;
                  //
                  // which can be optimized to:
                  //
                  //   ST use, mem<mStackByteOffset[ r ]>;

                  movesToBeDeleted.insert( o );
                  DOUT( "Marking move for deletion." << endl );

                  DOUT(
                    "Inserting { spill-store, " << use.getName() << ", " <<
                    mStackByteOffset[ r.getID() ] <<
                    " } for defined register '" << r.getName() <<
                    "' after the move." << endl );

                  auto pos = iit;
                  insertSpillStore(
                    use, r, mStackByteOffset[ r.getID() ], b, ++pos );

                  // After this current definition, previous values of r, its
                  // childs and its parents are no more alive.
                  propagateFalseLiveness( r, b, falseLivenessInSameBB );
                }
              } else

              if ( !movesToBeDeleted.count( o ) ) {

                // The current operation is a non-move where some parameter is
                // spilled.

                if ( rp.getUsage() == WIR_Usage::use ) {
                  // This is a non-move where a used operand is spilled.

                  // cppcheck-suppress variableScope
                  DACTION(
                    BBPOS( o );
                    DOUT(
                      endl << "Spill scenario (1), used operand '" <<
                      r.getName() << "' spilled. " << BBID << o << endl ) );

                  // The current parameter is used. We generate a spill-load
                  // right before the current operation. However, if a spilled
                  // value is used several times in the very same basic block
                  // and the restored value remains live until the last use of
                  // the spill in that block, then only a single load of the
                  // value is needed in the block (cf. S. S. Muchnik, page
                  // 501). So, if r is still alive in the same basic block, we
                  // do not need to generate another spill-load.
                  if ( !stillLiveInSameBB.count( r.getID() ) ||
                       ( stillLiveInSameBB[ r.getID() ] == false ) ) {
                    // If the register is not alive in the current basic block
                    // b, we insert a spill-load. We use a clone of r here for
                    // live range splitting during spilling.
                    WIR_VirtualRegister *clone = cloneRegister( r );
                    mUncoloredActualSpills.insert( *clone );
                    mStackByteOffset[ clone->getID() ] =
                      mStackByteOffset[ r.getID() ];

                    if ( mRematerializationInstructions.count( rp ) )
                      insertRematerializationCode( *clone, r, rp, b, iit );
                    else {
                      DOUT(
                        "Inserting { spill-load, " << clone->getName() <<
                        ", " << mStackByteOffset[ r.getID() ] <<
                        " } for used register '" << r.getName() <<
                        "' before the operation." << endl );

                      insertSpillLoad(
                        *clone, r, mStackByteOffset[ r.getID() ], b, iit );
                    }

                    it =
                      o.replaceParameter(
                        it,
                        WIR_RegisterParameter(
                          *clone, rp.getUsage(), rp.isImplicit() ) );

                    DOUT(
                      "Replacing '" << r.getName() << "' by '" <<
                      clone->getName() << "' in the operation." << endl );

                    // The register and all its childs stay alive after the
                    // current use.
                    list<std::reference_wrapper<WIR_VirtualRegister>> workList;
                    list<std::reference_wrapper<WIR_VirtualRegister>>
                      cloneWorkList;

                    workList.push_back( r );
                    cloneWorkList.push_back( *clone );

                    do {
                      WIR_VirtualRegister &reg = workList.front();
                      workList.pop_front();
                      WIR_VirtualRegister &cl = cloneWorkList.front();
                      cloneWorkList.pop_front();

                      trueLivenessInSameBB.insert( reg );
                      newCloneFor.insert( { reg.getID(), cl } );
                      DOUT(
                        "Marking '" << reg.getName() << "' as live within '" <<
                        b.getName() << "'." << endl );

                      copy( reg.begin(), reg.end(), back_inserter( workList ) );
                      copy(
                        cl.begin(), cl.end(), back_inserter( cloneWorkList ) );
                    } while ( !workList.empty() );
                  } else {
                    // If the register is already alive in the current basic
                    // block b, we do not need yet another spill-load. Instead,
                    // we simply re-use the cloned register for r that must
                    // exist.
                    it =
                      o.replaceParameter(
                        it,
                        WIR_RegisterParameter(
                          newCloneFor.at( r.getID() ), rp.getUsage(),
                          rp.isImplicit() ) );

                    DOUT(
                      "Replacing '" << r.getName() << "' by '" <<
                      newCloneFor.at( r.getID() ).get().getName() <<
                      "' in the operation." << endl );
                  }
                } else

                if ( rp.getUsage() == WIR_Usage::def ) {
                  // This is a non-move where a defined operand is spilled.

                  // cppcheck-suppress variableScope
                  DACTION(
                    BBPOS( o );
                    DOUT(
                      endl << "Spill scenario (2), defined operand '" <<
                      r.getName() << "' spilled. " << BBID << o << endl ) );

                  // The current parameter is defined. We generate a spill-store
                  // right after the current instruction. We use a clone of r
                  // here for live range splitting during spilling.
                  WIR_VirtualRegister *clone = cloneRegister( r );
                  mUncoloredActualSpills.insert( *clone );
                  mStackByteOffset[ clone->getID() ] =
                    mStackByteOffset[ r.getID() ];

                  it =
                    o.replaceParameter(
                      it,
                      WIR_RegisterParameter(
                        *clone, rp.getUsage(), rp.isImplicit() ) );

                  DOUT(
                    "Inserting { spill-store, " << clone->getName() << ", " <<
                    mStackByteOffset[ r.getID() ] <<
                    " } for defined register '" << r.getName() <<
                    "' after the operation." << endl << "Replacing '" <<
                    r.getName() << "' by '" << clone->getName() <<
                    "' in the operation." << endl );

                  auto pos = iit;
                  insertSpillStore(
                    *clone, r, mStackByteOffset[ r.getID() ], b, ++pos );

                  // After this current definition, previous values of r, its
                  // childs and its parents are no more alive.
                  propagateFalseLiveness( r, b, falseLivenessInSameBB );
                } else {
                  // This is a non-move where a def-used operand is spilled.

                  // cppcheck-suppress variableScope
                  DACTION(
                    BBPOS( o );
                    DOUT(
                      endl << "Spill scenario (3), def-used operand '" <<
                      r.getName() << "' spilled. " << BBID << o << endl ) );

                  // The current parameter is def-used. In principle, we have to
                  // generate a spill-load before and a spill-store after the
                  // current instruction, using a clone of r for live range
                  // splitting. However, if r is still alive in the same basic
                  // block, we can omit the generation of the spill-load.
                  WIR_VirtualRegister *clone = nullptr;

                  if ( !stillLiveInSameBB.count( r.getID() ) ||
                       ( stillLiveInSameBB[ r.getID() ] == false ) ) {
                    // r is not alive within b.
                    clone = cloneRegister( r );
                    mUncoloredActualSpills.insert( *clone );
                    mStackByteOffset[ clone->getID() ] =
                      mStackByteOffset[ r.getID() ];

                    if ( mRematerializationInstructions.count( rp ) )
                      insertRematerializationCode( *clone, r, rp, b, iit );
                    else {
                      DOUT(
                        "Inserting { spill-load, " << clone->getName() <<
                        ", " << mStackByteOffset[ r.getID() ] <<
                        " } for used register '" << r.getName() <<
                        "' before the operation." << endl );

                      insertSpillLoad(
                        *clone, r, mStackByteOffset[ r.getID() ], b, iit );
                    }
                  } else
                    // r is still alive within b.
                    clone = &(newCloneFor.at( r.getID() ).get());

                  it =
                    o.replaceParameter(
                      it,
                      WIR_RegisterParameter(
                        *clone, rp.getUsage(), rp.isImplicit() ) );

                  // Generate the spill-store.
                  DOUT(
                    "Inserting { spill-store, " << clone->getName() << ", " <<
                    mStackByteOffset[ r.getID() ] <<
                    " } for def-used register '" << r.getName() <<
                    "' after the operation." << endl << "Replacing '" <<
                    r.getName() << "' by '" << clone->getName() <<
                    "' in the operation." << endl );

                  auto pos = iit;
                  insertSpillStore(
                    *clone, r, mStackByteOffset[ r.getID() ], b, ++pos );

                  // After this current def-use, previous values of r, its
                  // childs and its parents are no more alive.
                  propagateFalseLiveness( r, b, falseLivenessInSameBB );
                }
              }
            }
          }

      // Update whether registers are still alive or not in this same basic
      // block after this current instruction.
      for ( auto &r : trueLivenessInSameBB ) {
        list<std::reference_wrapper<WIR_VirtualRegister>> workList;
        workList.push_back( r );

        do {
          const WIR_VirtualRegister &reg = workList.front();
          workList.pop_front();

          stillLiveInSameBB[ reg.getID() ] = true;

          copy( reg.begin(), reg.end(), back_inserter( workList ) );
        } while ( !workList.empty() );
      }

      for ( auto &r : falseLivenessInSameBB ) {
        list<std::reference_wrapper<WIR_VirtualRegister>> workList;
        workList.push_back( r );

        do {
          const WIR_VirtualRegister &reg = workList.front();
          workList.pop_front();

          stillLiveInSameBB[ reg.getID() ] = false;
          newCloneFor.erase( reg.getID() );

          copy( reg.begin(), reg.end(), back_inserter( workList ) );
        } while ( !workList.empty() );
      }
    }
  }

  // Remove all detected spilling-related moves.
  for ( WIR_Operation &o : movesToBeDeleted ) {
    WIR_Instruction &i = o.getInstruction();
    WIR_BasicBlock &b = i.getBasicBlock();
    mCoalescedMoves.erase( o );

    // cppcheck-suppress variableScope
    DACTION(
      BBPOS( o );
      DOUT(
        "Removing spilling-related MOVE operation " << BBID << o << endl ) );

    if ( i.getOperations().size() == 1 )
      // The current move operation o is the only operation in i. We can thus
      // remove i entirely.
      b.eraseInstruction( b.findInstruction( i ) );
    else
      // Here, we simply remove the current move operation o and keep its
      // instruction i.
      i.eraseOperation( i.findOperation( o ) );
  }

  // Remove finally remaining rematerialization code.
  for ( auto mapEntry : mRematerializationInstructions ) {
    DOUT( "Deleting rematerialization instructions" );
    for ( auto i : mapEntry.second ) {
      DOUT( " " << i->getID() );
      delete( i );
    }
    DOUT( endl );
  }
  mRematerializationInstructions.clear();
  mSpilledNodes.clear();
};


/*
  insertRematerializationCode inserts code to rematerialize a register into the
  WIR.

  insertRematerializationCode is responsible to add all generated
  rematerialization instructions to map mRematerializedSpills.
*/
void WIR_GraphColoring::insertRematerializationCode( const WIR_BaseRegister &clone,
                                                     const WIR_VirtualRegister &r,
                                                     WIR_RegisterParameter &rp,
                                                     WIR_BasicBlock &b,
                                                     std::list<std::reference_wrapper<WIR_Instruction>>::const_iterator pos )
{
  DSTART(
    "void WIR_GraphColoring::insertRematerializationCode(const WIR_BaseRegister&, const WIR_VirtualRegister&, WIR_RegisterParameter&, WIR_BasicBlock&, list<reference_wrapper<WIR_Instruction> >::const_iterator)" );

  list<WIR_Instruction *> &l = mRematerializationInstructions[ rp ];

  auto &spillCode = mRematerializedSpills[ r.getRoot().getID() ];
  spillCode.push_back( {} );
  auto &newSpill = spillCode.back();

  // Insert the rematerialization instructions before the instruction pos.
  for ( auto i : l ) {
    auto it = b.insertInstruction( pos, *i );
    mInsertedSpillCode.push_back( it->get() );
    newSpill.push_back( *it );

    if ( mMarkSpillCode )
      it->get().insertContainer(
        WIR_Comment( "RA REMATERIALIZE " + rp.getRegister().getName() ) );

    DOUT(
      "Inserting rematerialization instruction" << endl << it->get() <<
      "for register '" << r.getName() << "' before the operation." << endl );

    // Build a map providing the cloned sub-register for each hierarchical
    // sub-register of r.
    map<WIR_id_t, std::reference_wrapper<const WIR_BaseRegister>> cloneOf;
    list<std::reference_wrapper<const WIR_VirtualRegister>> regWorkList;
    list<std::reference_wrapper<const WIR_BaseRegister>> cloneWorkList;

    regWorkList.push_back( r );
    cloneWorkList.push_back( clone );

    do {
      const WIR_VirtualRegister &reg = regWorkList.front();
      regWorkList.pop_front();
      const WIR_BaseRegister &cl = cloneWorkList.front();
      cloneWorkList.pop_front();

      cloneOf.insert( { reg.getID(), cl } );

      copy( reg.begin(), reg.end(), back_inserter( regWorkList ) );

      if ( cl.isVirtual() )
        copy(
          dynamic_cast<const WIR_VirtualRegister &>( cl ).begin(),
          dynamic_cast<const WIR_VirtualRegister &>( cl ).end(),
          back_inserter( cloneWorkList ) );
      else
        copy(
          dynamic_cast<const WIR_PhysicalRegister &>( cl ).begin(),
          dynamic_cast<const WIR_PhysicalRegister &>( cl ).end(),
          back_inserter( cloneWorkList ) );
    } while ( !regWorkList.empty() );

    // Inspect all parameters of the current rematerialization instruction. If
    // it refers to the current register r, replace it by clone.
    for ( WIR_Operation &o : it->get() )
      for ( auto pit = o.begin(); pit != o.end(); ++pit )
        if ( pit->get().getType() == WIR_ParameterType::reg ) {
          const auto &rparam =
            dynamic_cast<const WIR_RegisterParameter &>( pit->get() );
          if ( cloneOf.count( rparam.getRegister().getID() ) )
            pit =
              o.replaceParameter(
                pit,
                WIR_RegisterParameter(
                  cloneOf.at( rparam.getRegister().getID() ),
                  rparam.getUsage(), rparam.isImplicit() ) );
        }

    delete( i );
  }

  l.clear();
  mRematerializationInstructions.erase( rp );
};


/*
  removeCoalescedMoves removes all operations in set mCoalescedMoves from the
  WIR code.
*/
void WIR_GraphColoring::removeCoalescedMoves( void )
{
  DSTART( "void WIR_GraphColoring::removeCoalescedMoves()" );

  for ( WIR_Operation &o : mCoalescedMoves ) {
    WIR_Instruction &i = o.getInstruction();
    WIR_BasicBlock &b = i.getBasicBlock();

    // cppcheck-suppress variableScope
    DACTION(
      BBPOS( o );
      DOUT(
        "Removing coalesced MOVE operation " << BBID << o << endl ) );

    if ( i.getOperations().size() == 1 )
      // The current move operation o is the only operation in i. We can thus
      // remove i entirely.
      b.eraseInstruction( b.findInstruction( i ) );
    else
      // Here, we simply remove the current move operation o and keep its
      // instruction i.
      i.eraseOperation( i.findOperation( o ) );
  }

  mCoalescedMoves.clear();
  mConstrainedMoves.clear();
  mActiveMoves.clear();
  mFrozenMoves.clear();
};


/*
  replaceColoredRegisters scans the specified WIR function and replaces all
  occurrences of virtual registers in mRegisterMapping by their associated
  physical registers.

  Besides replacing all occurrences of colored virtual registers by their
  physical counterparts, replaceColoredRegisters also deletes unnecessary
  pre-color information from f.
*/
void WIR_GraphColoring::replaceColoredRegisters( WIR_Function &f )
{
  DSTART( "void WIR_GraphColoring::replaceColoredRegisters(WIR_Function&)" );

  for ( WIR_BasicBlock &b : f )
    for ( WIR_Instruction &i : b )
      for ( WIR_Operation &o : i ) {
        for ( auto it = o.begin(); it != o.end(); ++it )
          if ( it->get().getType() == WIR_ParameterType::reg ) {
            const auto &rp =
              dynamic_cast<const WIR_RegisterParameter &>( it->get() );
            const WIR_BaseRegister &r = rp.getRegister();

            if ( r.isVirtual() && mRegisterMapping.count( r.getID() ) ) {
              bool replaceParameter = true;

              // If the new parameter is implicit and is already part of
              // operation o, we do not add the new parameter yet another time,
              // it's redundant.
              if ( rp.isImplicit() )
                for ( const WIR_Parameter &p : o )
                  if ( p.getType() == WIR_ParameterType::reg ) {
                    const auto &implRp =
                      dynamic_cast<const WIR_RegisterParameter &>( p );

                    if ( ( implRp.getRegister() ==
                             mRegisterMapping.at( r.getID() ).get() ) &&
                         ( implRp.getUsage() == rp.getUsage() ) ) {
                      replaceParameter = false;
                      break;
                    }
                  }

              // Do replacement of rp at position it.
              if ( replaceParameter ) {
                DACTION(
                  BBPOS( o );
                  DOUT(
                    "Replacing '" << r.getName() << "' by '" <<
                    mRegisterMapping.at( r.getID() ).get().getName() <<
                    "' in " << BBID << implicitparams << o << endl ); );

                it =
                  o.replaceParameter(
                    it,
                    WIR_RegisterParameter(
                      mRegisterMapping.at( r.getID() ), rp.getUsage(),
                      rp.isImplicit() ) );
              }

              // Remove unnecessary precolor information from f.
              f.erasePrecolor(
                const_cast<WIR_VirtualRegister &>(
                  dynamic_cast<const WIR_VirtualRegister &>( r ) ) );
            }
          }
      }

  mRegisterMapping.clear();
};


/*
  allocateUncoloredActualSpills processes all registers in
  mUncoloredActualSpills and assigns them to physical registers.
*/
void WIR_GraphColoring::allocateUncoloredActualSpills( WIR_Function &f )
{
  DSTART(
    "void WIR_GraphColoring::allocateUncoloredActualSpills(WIR_Function&)" );

  for ( WIR_VirtualRegister &r : mUncoloredActualSpills ) {

    // r is a virtual register created in the course of live range splitting
    // during spilling by method insertSpillCode. In other words, r stands for
    // some actually spilled vreg, and insertSpillCode has already generated
    // the appropriate spill-loads and/or spill-stores for r. However, the
    // graph coloring algorithm finally failed to assign a color to r, because
    // all feasible physical registers are simultaneously alive while r is
    // alive.
    // We thus have to select one currently occupied phreg, temporarily
    // spill-store this phreg onto the stack while r is alive in order to make
    // space in the register file for r, assign r to this phreg, and finally
    // spill-load the original value from the stack into the phreg in order to
    // restore the original register mapping.

    DOUT(
      endl << "Processing uncolored spill '" << r.getName() << "'." << endl );

    // Collect all child registers of r.
    set<WIR_id_t> regs;
    list<std::reference_wrapper<WIR_VirtualRegister>> workList;
    workList.push_back( r );

    do {
      const WIR_VirtualRegister &reg = workList.front();
      workList.pop_front();

      regs.insert( reg.getID() );

      copy( reg.begin(), reg.end(), back_inserter( workList ) );
    } while ( !workList.empty() );

    // According to the scheme of spill code generation, r must only occur in
    // exactly one basic block. We now determine this basic block, and the first
    // and last occurrence of r or its childs in this block.
    auto foundBBit = f.end();
    WIR_Instruction *firstInstr = nullptr;
    WIR_Instruction *lastInstr = nullptr;
    WIR_InstructionSet occurrences;

    for ( auto it = f.begin(); it != f.end(); ++it ) {
      for ( WIR_Instruction &i : it->get() )
        for ( const WIR_Operation &o : i )
          for ( const WIR_Parameter &p : o )
            if ( p.getType() == WIR_ParameterType::reg ) {
              const auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p );

              if ( rp.getRegister().isVirtual() &&
                   regs.count( rp.getRegister().getID() ) ) {
                // OK, we've found an occurrence of r or of a child of it.
                foundBBit = it;
                lastInstr = &i;
                if ( firstInstr == nullptr )
                  firstInstr = &i;
                occurrences.insert( i );
              }
            }

      if ( foundBBit != f.end() )
        break;
    }

    // The following assertions are required for the clang static analyzer.
    ufAssert( firstInstr != nullptr );
    ufAssert( lastInstr != nullptr );

    WIR_Instruction &first = *firstInstr;
    WIR_Instruction &last = *lastInstr;

    WIR_BasicBlock &b = foundBBit->get();
    auto fit = b.findInstruction( first );
    auto lit = b.findInstruction( last );

    // cppcheck-suppress variableScope
    DACTION(
      BBPOS( first.begin()->get() );
      DOUT( "First occurrence: " << BBID << first << endl ); );
    // cppcheck-suppress variableScope
    DACTION(
      BBPOS( last.begin()->get() );
      DOUT( "Last occurrence: " << BBID << last << endl ); );

    // candidatePhregs holds all phregs which could be used here for the
    // allocation of r.
    WIR_PhysicalRegisterSet candidatePhregs = getCandidatePhregs( r );

    DACTION(
      DOUT( "Candidate phregs:" );
      for ( const WIR_PhysicalRegister &phreg : candidatePhregs )
        DOUT( " " << phreg.getName() );
      DOUT( endl ); );

    // From this set candidatePhregs, we remove all phregs which occur in the
    // same instructions as r does. These registers and r are alive in the very
    // same WIR instructions, and we cannot resolve this liveness conflict by
    // inserting other spill instructions.
    WIR_PhysicalRegisterSet candidatesToRemove;
    for ( const WIR_Instruction &i : occurrences )
      for ( const WIR_Operation &o : i )
        for ( const WIR_Parameter &p : o )
          if ( p.getType() == WIR_ParameterType::reg ) {
            const auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p );
            const WIR_BaseRegister &reg = rp.getRegister();

            if ( reg.isPhysical() )
              for ( WIR_PhysicalRegister &c : candidatePhregs )
                if ( c.isChildOf( reg ) || reg.isChildOf( c ) )
                  candidatesToRemove.insert( c );
          }

    DOUT( "Removing candidates:" );
    for ( WIR_PhysicalRegister &c : candidatesToRemove ) {
      DOUT( " " << c.getName() );
      candidatePhregs.erase( c );
    }
    DOUT( endl );
    candidatesToRemove.clear();

    // Next, we basically count how often the remaining registers in
    // candidatePhregs actually occur in the interval between first and last. We
    // temporarily spill-store that register onto the stack which occurs least,
    // of course.
    map<WIR_id_t, unsigned int> additionalSpillsForPhreg;
    list<pair<unsigned int, std::reference_wrapper<WIR_PhysicalRegister>>> l;

    for ( const WIR_PhysicalRegister &c : candidatePhregs ) {
      additionalSpillsForPhreg[ c.getID() ] = 2;

      for ( auto it = fit; it != std::next( lit ); ++it ) {
        bool insertLoad = false;
        bool insertStore = false;

        for ( const WIR_Operation &o : it->get() )
          for ( const WIR_Parameter &p : o )
            if ( p.getType() == WIR_ParameterType::reg ) {
              const auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p );
              const WIR_BaseRegister &reg = rp.getRegister();

              if ( c.isChildOf( reg ) || reg.isChildOf( c ) || ( c == reg ) ) {
                if ( rp.isUsed() || rp.isDefUsed() )
                  insertLoad = true;
                if ( rp.isDefined() || rp.isDefUsed() )
                  insertStore = true;
              }
            }

        if ( insertLoad && insertStore )
          // If the current candidate is defined and used within i, we have to
          // generate 4 additional instructions: spill-store r on the stack,
          // spill-load the candidate, perform i, spill-store the candidate,
          // spill-load r
          additionalSpillsForPhreg[ c.getID() ] += 4;
        else

        if ( insertLoad || insertStore )
          // If the current candidate is only used or only defined within i, we
          // have to generate 3 additional instructions.
          additionalSpillsForPhreg[ c.getID() ] += 3;
      }

      // Put spill costs in a list suitable for sorting.
      l.push_back(
        { additionalSpillsForPhreg[ c.getID() ],
          const_cast<WIR_PhysicalRegister &>( c ) } );
      DOUT(
        "Additional spill instructions when using '" << c.getName() << "': " <<
        additionalSpillsForPhreg[ c.getID() ] << "." << endl );
    }

    // Sort list of pairs according to the spill costs.
    auto cmp = []( pair<unsigned int,
                        std::reference_wrapper<WIR_PhysicalRegister>> p1,
                   pair<unsigned int,
                        std::reference_wrapper<WIR_PhysicalRegister>> p2 ) {
      return( p1.first < p2.first );
    };
    l.sort( cmp );

    // We only keep those phregs in candidatePhregs which have least spill
    // costs.
    candidatePhregs.clear();
    unsigned int leastCost = numeric_limits<unsigned int>::max();
    for ( auto &[ costs, r1 ] : l ) {
      if ( candidatePhregs.empty() ) {
        candidatePhregs.insert( r1 );
        leastCost = costs;
      } else

      if ( costs == leastCost )
        candidatePhregs.insert( r1 );
      else
        break;
    }

    additionalSpillsForPhreg.clear();
    l.clear();

    // candidatePhregs now finally contains all those phregs which can be used
    // for the allocation of r, leading to least additional spill costs. Now,
    // let's select one of these candidates and let's go with spilling.
    ufAssert( candidatePhregs.size() > 0 );
    const WIR_PhysicalRegister &phreg = getCandidatePhreg( candidatePhregs );
    candidatePhregs.clear();

    DOUT(
      "Using phreg '" << phreg.getName() << "' for spilling of '" <<
      r.getName() << "'." << endl );

    // In order to temporarily spill-store the identified candidate on the
    // stack, we need some free stack space.
    unsigned int stackPos = mAdditionalStackSpace;
    mAdditionalStackSpace += getStackSize( phreg );

    DOUT(
      "Additional stack space for spilling in function '" << f.getName() <<
      "': " << mAdditionalStackSpace - stackPos << endl );

    // Insert a spill-store (spill-load, resp.) before the first occurrence of r
    // (after the last one of r, resp.).
    DOUT(
      "Inserting { spill-store, " << phreg.getName() << ", " << stackPos <<
      " } for uncolored spill '" << r.getName() <<
      "' before first occurrence." << endl );

    insertSpillStore( phreg, r, stackPos, b, fit );

    DOUT(
      "Inserting { spill-load, " << phreg.getName() << ", " << stackPos <<
      " } for uncolored spill '" << r.getName() <<
      "' after last occurrence." << endl );

    insertSpillLoad( phreg, r, stackPos, b, std::next( lit ) );

    // We generate a dedicated list of instructions between first and last in
    // its current state here. This is because the following loop inserts new
    // spilling instructions here and there which would otherwise perturb the
    // instruction traversal.
    list<std::reference_wrapper<WIR_Instruction>> instructionsOfBB;
    for ( auto it = fit; it != std::next( lit ); ++it )
      instructionsOfBB.push_back( it->get() );

    // For each occurrence of the candidate register in the interval between
    // first and last, insert the appropriate spill instructions now.
    for ( WIR_Instruction &i : instructionsOfBB ) {

      bool insertLoad = false;
      bool insertStore = false;
      auto posi = b.findInstruction( i );

      for ( const WIR_Operation &o : i )
        for ( const WIR_Parameter &p : o )
          if ( p.getType() == WIR_ParameterType::reg ) {
            const auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p );
            const WIR_BaseRegister &reg = rp.getRegister();

            if ( phreg.isChildOf( reg ) || reg.isChildOf( phreg ) ||
                 ( phreg == reg ) ) {
              if ( rp.isUsed() || rp.isDefUsed() )
                insertLoad = true;
              if ( rp.isDefined() || rp.isDefUsed() )
                insertStore = true;
            }
          }

      if ( insertLoad || insertStore ) {
        // Spill-store r before the current instruction i.
        DOUT(
          "Inserting { spill-store, " << r.getName() << ", " <<
          mStackByteOffset[ r.getID() ] << " } for uncolored spill '" <<
          r.getName() << "'." << endl );

        insertSpillStore( r, r, mStackByteOffset[ r.getID() ], b, posi );

        // Spill-load r after the current instruction i.
        DOUT(
          "Inserting { spill-load, " << r.getName() << ", " <<
          mStackByteOffset[ r.getID() ] << " } for uncolored spill '" <<
          r.getName() << "'." << endl );

        insertSpillLoad(
          r, r, mStackByteOffset[ r.getID() ], b, std::next( posi ) );
      }

      if ( insertLoad ) {
        // If the current candidate is used within i, now generate a spill-load
        // before i.
        DOUT(
          "Inserting { spill-load, " << phreg.getName() << ", " << stackPos <<
          " } for uncolored spill '" << r.getName() << "'." << endl );

        insertSpillLoad( phreg, r, stackPos, b, posi );
      }

      if ( insertStore ) {
        // If the current candidate is defined within i, now generate a
        // spill-store after i.
        DOUT(
          "Inserting { spill-store, " << phreg.getName() << ", " << stackPos <<
          " } for uncolored spill '" << r.getName() << "'." << endl );

        insertSpillStore( phreg, r, stackPos, b, std::next( posi ) );
      }
    }

    // Allocate the currently processed, uncolored spill r and its childs to the
    // identified candidate register.
    workList.push_back( r );
    list<std::reference_wrapper<const WIR_PhysicalRegister>> phWorkList;
    phWorkList.push_back( phreg );

    do {
      WIR_VirtualRegister &r1 = workList.front();
      workList.pop_front();

      const WIR_PhysicalRegister &phreg1 = phWorkList.front();
      phWorkList.pop_front();

      mRegisterMapping.insert(
        { r1.getID(), const_cast<WIR_PhysicalRegister &>( phreg1 ) } );
      DOUT(
        "Assigning " << r1.getName() << " (ID " << r1.getID() << ") to " <<
        phreg1.getName() << " (ID " << phreg.getID() << ")." << endl );

      copy( r1.begin(), r1.end(), back_inserter( workList ) );
      copy( phreg1.begin(), phreg1.end(), back_inserter( phWorkList ) );
    } while ( !workList.empty() );

    // Finally apply the allocation of all the uncolored spills.
    replaceColoredRegisters( f );
  }
};


/*
  computeCallerSavingStackSpace computes how many additional bytes have to be
  allocated in the currently processed function's stack frame to store
  caller-saved registers across calls.

  The result is stored in attribute mCallerSavingStackSpace.
*/
void WIR_GraphColoring::computeCallerSavingStackSpace( const WIR_Function &f )
{
  DSTART(
    "void WIR_GraphColoring::computeCallerSavingStackSpace(const "
    "WIR_Function&)" );

  (void) f;

  mCallerSavingStackSpace = 0;
  for ( const auto &[ call, regs ] : mCallerSavedRegsAliveAcrossCall ) {
    unsigned int requiredSpace = 0;

    requiredSpace =
      accumulate(
        regs.begin(), regs.end(), requiredSpace,
        [&]( unsigned int i,
             const std::reference_wrapper<WIR_BaseRegister> ref ) {
          return( i + getStackSize( ref.get() ) ); } );

    if ( requiredSpace > mCallerSavingStackSpace )
      mCallerSavingStackSpace = requiredSpace;
  }

  mAdditionalStackSpace += mCallerSavingStackSpace;

  DOUT(
    "Additional stack space for caller-saved registers in function '" <<
    f.getName() << "': " << mCallerSavingStackSpace << "." << endl );
};


/*
  removeRedundantSpills removes unnecessary spill code from a WIR function.

  In particular, spill-stores for which no spill-loads exist (due to
  rematerialization) are removed, and immediately following combinations of
  load/store, store/load, load/load and store/store refering to the very same
  register are removed.
*/
void WIR_GraphColoring::removeRedundantSpills( void )
{
  DSTART( "void WIR_GraphColoring::removeRedundantSpills()" );

  // Remove empty entries from maps first.
  for ( auto it = mSpillLoads.begin(); it != mSpillLoads.end(); ) {
    auto &loads = it->second;

    for ( auto lIt = loads.begin(); lIt != loads.end(); )
      lIt = lIt->empty() ? loads.erase( lIt ) : std::next( lIt );

    it = loads.empty() ? mSpillLoads.erase( it ) : std::next( it );
  }

  for ( auto it = mSpillStores.begin(); it != mSpillStores.end(); ) {
    auto &stores = it->second;

    for ( auto sIt = stores.begin(); sIt != stores.end(); )
      sIt = sIt->empty() ? stores.erase( sIt ) : std::next( sIt );

    it = stores.empty() ? mSpillStores.erase( it ) : std::next( it );
  }

  for ( auto it = mRematerializedSpills.begin();
        it != mRematerializedSpills.end(); ) {
    auto &remats = it->second;

    for ( auto rIt = remats.begin(); rIt != remats.end(); )
      rIt = rIt->empty() ? remats.erase( rIt ) : std::next( rIt );

    it = remats.empty() ? mRematerializedSpills.erase( it ) : std::next( it );
  }

  // Check for spill-stores without spill-load first.
  for ( auto it = mSpillStores.begin(); it != mSpillStores.end(); ) {
    if ( !mSpillLoads.count( it->first ) ) {
      DOUT(
        "No spill-loads for spill-stores of register ID " << it->first <<
        " found." << endl );

      for ( auto &store : it->second )
        for ( WIR_Instruction &i : store ) {
          // cppcheck-suppress variableScope
          DACTION(
            BBPOS( i.begin()->get() );
            DOUT( "  Removing redundant spill-store " << BBID << i ); );

          auto &b = i.getBasicBlock();
          b.eraseInstruction( b.findInstruction( i ) );
        }

      DOUT( endl );

      it = mSpillStores.erase( it );
    } else
      ++it;
  }

  // This lambda is used to obtain the IDs of instructions following a given
  // spill.
  auto getNextIDs = [&]( const std::list<std::reference_wrapper<WIR_Instruction>> &spill ) -> set<WIR_id_t> {
    set<WIR_id_t> res, spillIDs;

    for ( const WIR_Instruction &i : spill )
      spillIDs.insert( i.getID() );

    for ( const WIR_Instruction &i : spill ) {
      auto it = std::next( i.getBasicBlock().findInstruction( i ) );
      if ( ( it != i.getBasicBlock().getInstructions().end() ) &&
           !spillIDs.count( it->get().getID() ) )
        res.insert( it->get().getID() );
    }

    return( res );
  };

  // This lambda is used to determine whether a given spill contains
  // instructions with IDs in set nextIDs, i.e., whether one spill follows the
  // other.
  auto isFollowing = [&]( const std::list<std::reference_wrapper<WIR_Instruction>> &spill,
                          const set<WIR_id_t> &nextIDs ) -> bool {
    return(
      any_of(
        spill.begin(), spill.end(),
        [&]( const WIR_Instruction &i ) {
          return( nextIDs.count( i.getID() ) ); } ) );
  };

  // This lambda implements redundant load/store elimination for one given
  // spill-load or rematerialized spill.
  auto redundantLoadStoreElimination = [&]( std::list<std::list<std::reference_wrapper<WIR_Instruction>>> &spills,
                                            bool isRemat = false ) {
    DSTART(
      "WIR_GraphColoring::removeRedundantSpills()::<redundantLoadStoreElimination(list<list<reference_wrapper<WIR_Instruction> > >&, bool)>" );

    bool res = false;

    // Check all spill-loads.
    for ( auto &spillLoad : spills ) {
      auto nextIDs = getNextIDs( spillLoad );
      const WIR_Instruction &loadInstr = spillLoad.front().get();
      auto loadID = loadInstr.getID();

      // Check all spill-stores, whether the store directly follows the spill-
      // load, both load/store the same phreg, and both access the very same
      // stack offset.
      for ( auto sIt = mSpillStores.begin(); sIt != mSpillStores.end(); ) {
        for ( auto storeIt = sIt->second.begin();
              storeIt != sIt->second.end(); ) {
          const WIR_Instruction &storeInstr = storeIt->front().get();
          auto storeID = storeInstr.getID();

          DACTION(
            DOUT(
              "Checking current spill-load" << *(loadInstr.begin()) <<
              " (ID " << loadID << ")" << endl << "  and spill-store" <<
              *(storeInstr.begin()) << " (ID " << storeID << ")" << endl <<
              "  isFollowing( store, load ) = " <<
              string( isFollowing( *storeIt, nextIDs ) ? "true" : "false" ) <<
              endl << "  getPhregOfSpill( " << loadID << " ) = " <<
              getPhregOfSpill( spillLoad ).getName() << endl <<
              "  getPhregOfSpill( " << storeID << " ) = " <<
              getPhregOfSpill( *storeIt ).getName() << endl );

            if ( !isRemat ) {
              DOUT(
                "  mStackOffsetOfSpillInstruction[ " << loadID << " ] = " <<
                mStackOffsetOfSpillInstruction.at( loadID ) << endl <<
                "  mStackOffsetOfSpillInstruction[ " << storeID << " ] = " <<
                mStackOffsetOfSpillInstruction.at( storeID ) << endl );
            } );

          if ( isFollowing( *storeIt, nextIDs ) &&
              ( getPhregOfSpill( spillLoad ) ==
                  getPhregOfSpill( *storeIt ) ) &&
              ( isRemat ||
                ( mStackOffsetOfSpillInstruction.at( loadID ) ==
                    mStackOffsetOfSpillInstruction.at( storeID ) ) ) ) {
            // cppcheck-suppress variableScope
            DACTION(
              BBPOS( storeInstr.begin()->get() );
              DOUT( "  Removing redundant store " << BBID << storeInstr ); );

            for ( const WIR_Instruction &i : *storeIt )
              i.getBasicBlock().eraseInstruction(
                i.getBasicBlock().findInstruction( i ) );

            storeIt->clear();
            storeIt = sIt->second.erase( storeIt );
            nextIDs = getNextIDs( spillLoad );
            res = true;
          } else
            ++storeIt;
        }

        if ( sIt->second.empty() )
          sIt = mSpillStores.erase( sIt );
        else
          ++sIt;
      }
    }

    return( res );
  };

  // This lambda implements redundant store/load elimination for one given
  // spill-store.
  auto redundantStoreLoadElimination = [&]( std::list<std::list<std::reference_wrapper<WIR_Instruction>>> &spills ) {
    DSTART(
      "WIR_GraphColoring::removeRedundantSpills()::<redundantStoreLoadElimination(list<list<reference_wrapper<WIR_Instruction> > >&)>" );

    bool res = false;

    // Check all spill-stores.
    for ( auto &spillStore : spills ) {
      auto nextIDs = getNextIDs( spillStore );
      const WIR_Instruction &storeInstr = spillStore.front().get();
      auto storeID = storeInstr.getID();

      // Check all spill-loads, whether the load directly follows the spill-
      // store, both store/load the same phreg, and both access the very same
      // stack offset.
      for ( auto lIt = mSpillLoads.begin(); lIt != mSpillLoads.end(); ) {
        for ( auto loadIt = lIt->second.begin();
              loadIt != lIt->second.end(); ) {
          const WIR_Instruction &loadInstr = loadIt->front().get();
          auto loadID = loadInstr.getID();

          DOUT(
            "Checking current spill-store" << *(storeInstr.begin()) <<
            " (ID " << storeID << ")" << endl << "  and spill-load" <<
            *(loadInstr.begin()) << " (ID " << loadID << ")" << endl <<
            "  isFollowing( load, store ) = " <<
            string( isFollowing( *loadIt, nextIDs ) ? "true" : "false" ) <<
            endl << "  getPhregOfSpill( " << storeID << " ) = " <<
            getPhregOfSpill( spillStore ).getName() << endl <<
            "  getPhregOfSpill( " << loadID << " ) = " <<
            getPhregOfSpill( *loadIt ).getName() << endl <<
            "  mStackOffsetOfSpillInstruction[ " << storeID << " ] = " <<
            mStackOffsetOfSpillInstruction.at( storeID ) << endl <<
            "  mStackOffsetOfSpillInstruction[ " << loadID << " ] = " <<
            mStackOffsetOfSpillInstruction.at( loadID ) << endl );

          if ( isFollowing( *loadIt, nextIDs ) &&
                ( getPhregOfSpill( spillStore ) ==
                    getPhregOfSpill( *loadIt ) ) &&
                ( mStackOffsetOfSpillInstruction.at( storeID ) ==
                    mStackOffsetOfSpillInstruction.at( loadID ) ) ) {
            // cppcheck-suppress variableScope
            DACTION(
              BBPOS( loadInstr.begin()->get() );
              DOUT( "  Removing redundant load " << BBID << loadInstr ); );

            for ( const WIR_Instruction &i : *loadIt )
              i.getBasicBlock().eraseInstruction(
                i.getBasicBlock().findInstruction( i ) );

            loadIt->clear();
            loadIt = lIt->second.erase( loadIt );
            nextIDs = getNextIDs( spillStore );
            res = true;
          } else
            ++loadIt;
        }

        if ( lIt->second.empty() )
          lIt = mSpillLoads.erase( lIt );
        else
          ++lIt;
      }
    }

    return( res );
  };

  // This lambda implements redundant store/store elimination for one given
  // spill-store.
  auto redundantStoreStoreElimination = [&]( void ) {
    DSTART(
      "WIR_GraphColoring::removeRedundantSpills()::<redundantStoreStoreElimination()>" );

    bool res = false;

    for ( auto sIt1 = mSpillStores.begin(); sIt1 != mSpillStores.end(); ++sIt1 )
      for ( auto storeIt1 = sIt1->second.begin();
            storeIt1 != sIt1->second.end(); ++storeIt1 ) {
        const WIR_Instruction &storeInstr1 = storeIt1->front().get();
        auto storeID1 = storeInstr1.getID();

        // Check all spill-stores, whether the stores directly follow each other
        // and both access the very same stack offset.
        for ( auto sIt2 = mSpillStores.begin(); sIt2 != mSpillStores.end(); ) {
          for ( auto storeIt2 = sIt2->second.begin();
                storeIt2 != sIt2->second.end(); ) {
            auto nextIDs = getNextIDs( *storeIt2 );
            const WIR_Instruction &storeInstr2 = storeIt2->front().get();
            auto storeID2 = storeInstr2.getID();

            if ( storeID1 == storeID2 ) {
              ++storeIt2;
              continue;
            }

            DOUT(
              "Checking current spill-store" << *(storeInstr1.begin()) <<
              " (ID " << storeID1 << ")" << endl << "  and spill-store" <<
              *(storeInstr2.begin()) << " (ID " << storeID2 << ")" << endl <<
              "  isFollowing( store, store ) = " <<
              string( isFollowing( *storeIt1, nextIDs ) ? "true" : "false" ) <<
              endl <<
              "  mStackOffsetOfSpillInstruction[ " << storeID1 << " ] = " <<
              mStackOffsetOfSpillInstruction.at( storeID1 ) << endl <<
              "  mStackOffsetOfSpillInstruction[ " << storeID2 << " ] = " <<
              mStackOffsetOfSpillInstruction.at( storeID2 ) << endl );

            if ( isFollowing( *storeIt1, nextIDs ) &&
                 mStackOffsetOfSpillInstruction.at( storeID1 ) ==
                   mStackOffsetOfSpillInstruction.at( storeID2 ) ) {
              // cppcheck-suppress variableScope
              DACTION(
                BBPOS( storeInstr2.begin()->get() );
                DOUT( "  Removing redundant store " << BBID << storeInstr2 ); );

              for ( const WIR_Instruction &i : *storeIt2 )
                i.getBasicBlock().eraseInstruction(
                  i.getBasicBlock().findInstruction( i ) );

              storeIt2->clear();
              storeIt2 = sIt2->second.erase( storeIt2 );
              res = true;
            } else
              ++storeIt2;
          }

          if ( sIt2->second.empty() )
            sIt2 = mSpillStores.erase( sIt2 );
          else
            ++sIt2;
        }
      }

    return( res );
  };

  // This lambda implements redundant load/load elimination for one given
  // spill-load or rematerialized spill.
  auto redundantLoadLoadElimination = [&]( std::list<std::list<std::reference_wrapper<WIR_Instruction>>> &spills,
                                           bool isRemat = false ) {
    DSTART(
      "WIR_GraphColoring::removeRedundantSpills()::<redundantLoadLoadElimination(list<list<reference_wrapper<WIR_Instruction> > >&, bool)>" );

    bool res = false;

    for ( auto loadIt1 = spills.begin(); loadIt1 != spills.end(); ++loadIt1 ) {
      const WIR_Instruction &loadInstr1 = loadIt1->front().get();
      auto loadID1 = loadInstr1.getID();

      // Check all spill-loads, whether the loads directly follow each other,
      // both load the same phreg, and both access the very same stack offset.
      for ( auto lIt2 = mSpillLoads.begin(); lIt2 != mSpillLoads.end(); ) {
        for ( auto loadIt2 = lIt2->second.begin();
              loadIt2 != lIt2->second.end(); ) {
          auto nextIDs = getNextIDs( *loadIt2 );
          const WIR_Instruction &loadInstr2 = loadIt2->front().get();
          auto loadID2 = loadInstr2.getID();

          if ( loadID1 == loadID2 ) {
            ++loadIt2;
            continue;
          }

          DACTION(
            DOUT(
              "Checking current spill-load" << *(loadInstr1.begin()) <<
              " (ID " << loadID1 << ")" << endl << "  and spill-load" <<
              *(loadInstr2.begin()) << " (ID " << loadID2 << ")" << endl <<
              "  isFollowing( load, load ) = " <<
              string( isFollowing( *loadIt1, nextIDs ) ? "true" : "false" ) <<
              endl << "  getPhregOfSpill( " << loadID1 << " ) = " <<
              getPhregOfSpill( *loadIt1 ).getName() << endl <<
              "  getPhregOfSpill( " << loadID2 << " ) = " <<
              getPhregOfSpill( *loadIt2 ).getName() << endl );

            if ( !isRemat ) {
              DOUT(
                "  mStackOffsetOfSpillInstruction[ " << loadID1 << " ] = " <<
                mStackOffsetOfSpillInstruction.at( loadID1 ) << endl <<
                "  mStackOffsetOfSpillInstruction[ " << loadID2 << " ] = " <<
                mStackOffsetOfSpillInstruction.at( loadID2 ) << endl );
            } );

          if ( isFollowing( *loadIt1, nextIDs ) &&
               ( getPhregOfSpill( *loadIt1 ) ==
                   getPhregOfSpill( *loadIt2 ) ) &&
               ( isRemat ||
                 ( mStackOffsetOfSpillInstruction.at( loadID1 ) ==
                     mStackOffsetOfSpillInstruction.at( loadID2 ) ) ) ) {
            // cppcheck-suppress variableScope
            DACTION(
              BBPOS( loadInstr2.begin()->get() );
              DOUT( "  Removing redundant load " << BBID << loadInstr2 ); );

            for ( const WIR_Instruction &i : *loadIt2 )
              i.getBasicBlock().eraseInstruction(
                i.getBasicBlock().findInstruction( i ) );

            loadIt2->clear();
            loadIt2 = lIt2->second.erase( loadIt2 );
            nextIDs = getNextIDs( *loadIt1 );
            res = true;
          } else
            ++loadIt2;
        }

        if ( lIt2->second.empty() )
          lIt2 = mSpillLoads.erase( lIt2 );
        else
          ++lIt2;
      }
    }

    return( res );
  };

  bool modified = false;
  do {
    modified = false;

    // Check for redundant spill-load/-stores:
    //   ld         <reg>, mem[stack]c
    //   st         mem[stack]c, <reg>      # gets removed
    for ( auto &p : mSpillLoads )
      modified |= redundantLoadStoreElimination( p.second );
    for ( auto &p : mRematerializedSpills )
      modified |= redundantLoadStoreElimination( p.second, true );

    // Check for redundant spill-store/-loads:
    //   st         mem[stack]c, <reg>
    //   ld         <reg>, mem[stack]c      # gets removed
    for ( auto &p : mSpillStores )
      modified |= redundantStoreLoadElimination( p.second );

    // Check for redundant spill-store/-stores:
    //   st         mem[stack]c, <rega>      # gets removed
    //   st         mem[stack]c, <regb>
    modified |= redundantStoreStoreElimination();

    // Check for redundant spill-load/-loads:
    //   ld         <reg>, mem[stack]c      # gets removed
    //   ld         <reg>, mem[stack]c
    for ( auto &p : mSpillLoads )
      modified |= redundantLoadLoadElimination( p.second );
    for ( auto &p : mRematerializedSpills )
      modified |= redundantLoadLoadElimination( p.second, true );

  } while ( modified );

  mSpillLoads.clear();
  mSpillStores.clear();
  mRematerializedSpills.clear();
};

}       // namespace WIR
