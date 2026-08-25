/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2025 - 2026, Heiko Falk.

*/

/*!
  @file rvredundantcode.cc
  @brief This file implements a RISC-V-specific optimization eliminating
         redundant code that computes bit-wise equivalent results.

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
#include <string>

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32imfdc.h>
#include <arch/riscv/analyses/bit/rv32bitdfa.h>

// Include local headers
#include "rvredundantcode.h"


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
RV_RedundantCode::RV_RedundantCode( WIR_System &s ) :
  WIR_Optimization { s },
  WIR_RedundantCode { s }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for compilation unit-level optimization.
*/
RV_RedundantCode::RV_RedundantCode( WIR_CompilationUnit &c ) :
  WIR_Optimization { c },
  WIR_RedundantCode { c }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for function-level optimization.
*/
RV_RedundantCode::RV_RedundantCode( WIR_Function &f ) :
  WIR_Optimization { f },
  WIR_RedundantCode { f }
{
  DSTART( "RV_RedundantCode::RV_RedundantCode(WIR_Function&)" );
};


/*
  Destructor.
*/
RV_RedundantCode::~RV_RedundantCode( void )
{
  DSTART( "virtual RV_RedundantCode::~RV_RedundantCode()" );
};


//
// Protected class methods
//

/*
  runOptimization eliminates redundant code in the given function.
*/
void RV_RedundantCode::runOptimization( WIR_Function &f )
{
  DSTART( "virtual void RV_RedundantCode::runOptimization(WIR_Function&)" );

  RV32_BitDFA analyzer { f };
  setDFA( analyzer );

  WIR_RedundantCode::runOptimization( f );
};


/*
  areRedundant checks whether the outgoing bit-value of a defined register
  parameter and the incoming bit-value of a used one are redundant for some
  actual processor architecture.

  For the RISC-V architecture, areRedundant returns false if the defined
  register parameter is an implicit parameter of a function call, as a
  subsequent optimization would change the lifeness of this particular
  definition such that it could span other function calls which would render
  the optimization invalid.

  Likewise, if the defined register is used for argument passing for a function
  call, both def and use are also not redundant.

  Furthermore, the obeyance of particular RISC-V operation formats that require
  the presence of particular physical registers is ensured.
*/
bool RV_RedundantCode::areRedundant( const WIR_RegisterParameter &def,
                                     const WIR_UpDownValue &outValue,
                                     const WIR_RegisterParameter &use,
                                     const WIR_UpDownValue &inValue) const
{
  DSTART(
    "virtual bool RV_RedundantCode::areRedundant(const WIR_RegisterParameter&, "
    "const WIR_UpDownValue&, const WIR_RegisterParameter&, const "
    "WIR_UpDownValue&) const");

  (void) outValue;
  (void) inValue;

  // Check implicit defines of RISC-V function calls.
  if ( def.isImplicit() &&
       ( def.getOperation().isCall() || def.getOperation().isIndirectCall() ) )
    return( false );

  // Check defines from RISC-V moves that serve for argument passing of a
  // function call.
  static const set<string> argumentPassingRegs {
    "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17" };

  bool defIsArgumentPassingReg = false;
  if ( def.getRegister().isPhysical() &&
       argumentPassingRegs.count( def.getRegister().getName() ) )
    defIsArgumentPassingReg = true;
  if ( def.getRegister().isVirtual() ) {
    const auto &vreg =
      dynamic_cast<const WIR_VirtualRegister &>( def.getRegister() );
    if ( vreg.isPrecolored() &&
         argumentPassingRegs.count( vreg.getPrecolor().getName() ) )
      defIsArgumentPassingReg = true;
  }

  if ( defIsArgumentPassingReg ) {
    // OK, the def refers to a physical register used for function argument
    // passing. So, let's check whether the very last operation of the def's
    // basic block actually is a function call.
    auto &oDef = def.getOperation();
    auto &defBB = oDef.getInstruction().getBasicBlock();
    auto &lastOp = defBB.rbegin()->get().begin()->get();

    if ( lastOp.isCall() || lastOp.isIndirectCall() )
      return( false );
  }

  // Check RISC-V-specific operation formats.
  const auto &oUse = use.getOperation();
  int paramPos = -1;
  int i = 0;
  for ( const WIR_Parameter &p : oUse.getExplicitParameters() ) {
    if ( p == use ) {
      paramPos = i;
      break;
    }
    ++i;
  }

  // Operation formats featuring the stack pointer.
  if ( ( ( oUse.getOperationFormat() == RV32IC::OperationFormat::SRC8R_1 ) &&
         ( paramPos == 2 ) ) ||
       ( ( oUse.getOperationFormat() == RV32IC::OperationFormat::SRC8R_2 ) &&
         ( paramPos == 2 ) ) ||
       ( ( oUse.getOperationFormat() == RV32IC::OperationFormat::SRC10_1 ) &&
         ( paramPos == 0 ) ) ||
       ( ( oUse.getOperationFormat() == RV32IC::OperationFormat::SRRC10_1 ) &&
         ( paramPos == 1 ) ) )
    return( RV32I::isSP( def.getRegister() ) );

  return( true );
};

} // namespace WIR
