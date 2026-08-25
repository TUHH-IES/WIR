/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tc1796_cpu_tc_071.cc
  @brief This file implements a peephole optimizer for silicon bug TC1796
         CPU_TC.071 detection and correction.

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
#include <arch/tricore/tc13.h>

// Include local headers
#include "tc1796_cpu_tc_071.h"


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
TC1796_CPU_TC_071::TC1796_CPU_TC_071( WIR_Function &f ) :
  WIR_Optimization { f },
  WIR_SiliconBugs { f, "TC1796 CPU_TC.071", 2, true }
{
  DSTART( "TC1796_CPU_TC_071::TC1796_CPU_TC_071(WIR_Function&)" );
};


/*
  Destructor.
*/
TC1796_CPU_TC_071::~TC1796_CPU_TC_071( void )
{
  DSTART( "virtual TC1796_CPU_TC_071::~TC1796_CPU_TC_071()" );
};


//
// Protected class methods
//

/*
  matchSiliconBug determines whether the specified peephole matches with silicon
  bug CPU_TC.071.
*/
bool TC1796_CPU_TC_071::matchSiliconBug( const WIR_Peephole::peephole &p )
{
  DSTART( "virtual bool TC1796_CPU_TC_071::matchSiliconBug(const peephole&)" );

  WIR_Operation &o0 = p[ 0 ]->get().rbegin()->get();
  const WIR_Operation &o1 = p[ 1 ]->get().begin()->get();

  if ( ( o0.getOpCode() == TC13::OpCode::LOOP ) &&
       ( o1.getOpCode() == TC13::OpCode::LOOPU ) &&
       ( dynamic_cast<WIR_LabelParameter &>(
           o0.getExplicitParameters().rbegin()->get() ).getBasicBlock() ==
         o1.getInstruction().getBasicBlock() ) )
    return( true );

  return( false );
};


/*
  fixSiliconBug fixes silicon bug CPU_TC.071.
*/
WIR_Peephole::peephole TC1796_CPU_TC_071::fixSiliconBug( const WIR_Peephole::peephole &p ) const
{
  DSTART(
    "virtual WIR_Peephole::peephole TC1796_CPU_TC_071::fixSiliconBug(const peephole&) const" );

  WIR_BasicBlock &b = p[ 1 ]->get().getBasicBlock();

  WIR_Peephole::peephole res;
  res.push_back( p[ 0 ] );

  res.push_back(
    b.insertInstruction(
      p[ 1 ],
      { { TC13::OpCode::NOP,
          m16BitOperations ?
            TC13::OperationFormat::S : TC13::OperationFormat::SYS } } ) );
  markInstruction( res[ 1 ]->get() );

  res.push_back( p[ 1 ] );

  b.insertContainer(
    WIR_SchedulingConstraint {
      WIR_SchedulingConstraintType::sequential,
      res[ 1 ]->get(), res[ 2 ]->get() } );

  return( res );
};

}       // namespace WIR
