/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tc1796_cpu_tc_072.cc
  @brief This file implements a peephole optimizer for silicon bug TC1796
         CPU_TC.072 detection and correction.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <vector>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/tricore/tc13.h>

// Include local headers
#include "tc1796_cpu_tc_072.h"


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
TC1796_CPU_TC_072::TC1796_CPU_TC_072( WIR_Function &f ) :
  WIR_Optimization { f },
  WIR_SiliconBugs { f, "TC1796 CPU_TC.072", 2, true }
{
  DSTART( "TC1796_CPU_TC_072::TC1796_CPU_TC_072(WIR_Function&)" );
};


/*
  Destructor.
*/
TC1796_CPU_TC_072::~TC1796_CPU_TC_072( void )
{
  DSTART( "virtual TC1796_CPU_TC_072::~TC1796_CPU_TC_072()" );
};


//
// Protected class methods
//

/*
  matchSiliconBug determines whether the specified peephole matches with silicon
  bug CPU_TC.072.
*/
bool TC1796_CPU_TC_072::matchSiliconBug( const WIR_Peephole::peephole &p )
{
  DSTART( "virtual bool TC1796_CPU_TC_072::matchSiliconBug(const peephole&)" );

  // A small lambda to determine the registers defined by an operation.
  auto defRegs = [&]( const WIR_Operation &o, set<WIR_id_t> &regSet ) {
    for ( const WIR_Parameter &p : o.getExplicitParameters() )
      if ( p.getType() == WIR_ParameterType::reg ) {
        const auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p );
        if ( rp.isDefined() || rp.isDefUsed() ) {
          regSet.insert( rp.getRegister().getID() );
          for ( const WIR_BaseRegister &c : rp.getRegister().getLeafs() )
            regSet.insert( c.getID() );
        }
      }
  };

  vector<WIR_Operation *> ops;

  for ( auto it : p )
    for ( WIR_Operation &o : it->get() )
      if ( ops.size() < 3 )
        ops.push_back( &o );

  set<WIR_id_t> definedRegs0;
  defRegs( *ops[ 0 ], definedRegs0 );

  set<WIR_id_t> definedRegs1;
  defRegs( *ops[ 1 ], definedRegs1 );

  if ( ( ops[ 1 ]->getOpCode() == TC13::OpCode::LOOP ) &&
       definedRegs0.count(
         dynamic_cast<WIR_RegisterParameter &>(
           ops[ 1 ]->getExplicitParameters().begin()->get() ).getRegister().getID() ) )
    return( true );

  if ( ( ops.size() == 3 ) && ( ops[ 2 ]->getOpCode() == TC13::OpCode::LOOP ) &&
       definedRegs1.count(
         dynamic_cast<WIR_RegisterParameter &>(
           ops[ 2 ]->getExplicitParameters().begin()->get() ).getRegister().getID() ) )
    return( true );

  return( false );
};


/*
  fixSiliconBug fixes silicon bug CPU_TC.072.
*/
WIR_Peephole::peephole TC1796_CPU_TC_072::fixSiliconBug( const WIR_Peephole::peephole &p ) const
{
  DSTART(
    "virtual WIR_Peephole::peephole TC1796_CPU_TC_072::fixSiliconBug(const peephole&) const" );

  WIR_Peephole::peephole res;

  auto &b = p[ 1 ]->get().getBasicBlock();

  vector<WIR_Operation *> ops;

  for ( auto it : p )
    for ( WIR_Operation &o : it->get() )
      if ( ops.size() < 3 )
        ops.push_back( &o );

  res.push_back( p[ 0 ] );

  res.push_back(
    b.insertInstruction(
      p[ 1 ],
      { { TC13::OpCode::NOP,
          m16BitOperations ?
            TC13::OperationFormat::S : TC13::OperationFormat::SYS } } ) );
  markInstruction( res[ 0 ]->get() );

  if ( ops[ 1 ]->getOpCode() == TC13::OpCode::LOOP ) {
    if ( ops[ 1 ]->getInstruction() != ops[ 0 ]->getInstruction() )
      res.push_back( p[ 1 ] );
    else {
      res.push_back( b.insertInstruction( p[ 1 ], {} ) );
      res[ 1 ]->get().moveOperation( *ops[ 1 ] );
    }
  } else
    res.push_back( p[ 1 ] );

  b.insertContainer(
    WIR_SchedulingConstraint {
      WIR_SchedulingConstraintType::sequential,
      res[ 1 ]->get(), res[ 2 ]->get() } );

  return( res );
};

}       // namespace WIR
