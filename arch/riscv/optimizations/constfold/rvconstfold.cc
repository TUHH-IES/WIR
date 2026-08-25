/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvconstfold.cc
  @brief This file implements a RISC-V-specific constant folding optimization.

  @author Yom Ismail Tan <Yom.Tan@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <iterator>
#include <set>
#include <utility>

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32imfdc.h>
#include <arch/riscv/analyses/bit/rv32bitdfa.h>

// Include local headers
#include "rvconstfold.h"


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
RV_ConstFold::RV_ConstFold( WIR_System &s ) :
  WIR_Optimization { s },
  WIR_ConstFold { s },
  mCurrentFunction { nullptr }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for compilation unit-level optimization.
*/
RV_ConstFold::RV_ConstFold( WIR_CompilationUnit &c ) :
  WIR_Optimization { c },
  WIR_ConstFold { c },
  mCurrentFunction { nullptr }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for function-level optimization.
*/
RV_ConstFold::RV_ConstFold( WIR_Function &f ) :
  WIR_Optimization { f },
  WIR_ConstFold { f },
  mCurrentFunction { nullptr }
{
  DSTART( "RV_ConstFold::RV_ConstFold(WIR_Function&)" );
};


/*
  Destructor.
*/
RV_ConstFold::~RV_ConstFold( void )
{
  DSTART( "virtual RV_ConstFold::~RV_ConstFold()" );
};


//
// Protected class methods
//

/*
  runOptimization folds constants in the given function.
*/
void RV_ConstFold::runOptimization( WIR_Function &f )
{
  DSTART( "virtual void RV_ConstFold::runOptimization(WIR_Function&)" );

  mCurrentFunction = &f;

  RV32_BitDFA analyzer { f };
  setDFA( analyzer );

  WIR_ConstFold::runOptimization( f );

  mCurrentFunction = nullptr;
};


/*
  For an operation identified to be constant, doConstFolding does the actual
  RISC-V-specific folding.

  doConstFolding does not actually modify the currently examined WIR operation
  o. Instead, new instructions realizing the constant folding of o are added to
  map mNewInstructions.
*/
bool RV_ConstFold::doConstFolding( const WIR_Operation &o,
                                   const std::map<
                                     WIR_id_t, WIR_UpDownValue> &outValue,
                                   const std::map<
                                     WIR_id_t, WIR_UpDownValue> &inValue )
{
	DSTART(
    "virtual bool RV_ConstFold::doConstFolding(const WIR_Operation&, const "
    "map<long long unsigned int, WIR_UpDownValue>&, const "
    "map<long long unsigned int, WIR_UpDownValue>&)" );

  (void) inValue;

  // A small lambda to check whether an operation is an ADDI with x0 as second
  // parameter.
  auto isADDIx0 = [&]( const WIR_Operation &op ) -> bool {
    if ( ( op.getOpCode() == RV32I::OpCode::ADDI ) &&
         ( op.getOperationFormat() == RV32I::OperationFormat::RRC12_1 ) )
      return( RV32I::isX0( nthReg( op, 1 ) ) );

    return( false );
  };

  // A small lambda to check whether an operation is an ADDI with that gets
  // input by just one LUI.
  auto isADDILUI = [&]( const WIR_Operation &op ) -> bool {
    if ( ( op.getOpCode() == RV32I::OpCode::ADDI ) &&
         ( op.getOperationFormat() == RV32I::OperationFormat::RRC12_1 ) ) {
      auto &cont =
        nthParam( op, 1 ).getContainers<WIR_BitValues>().begin()->get();

      if ( cont.getInValues().size() == 1 ) {
        auto &srcOp = cont.getInValues().front().rp->getOperation();
        if ( ( srcOp.getOpCode() == RV32I::OpCode::LUI ) &&
             ( srcOp.getOperationFormat() == RV32I::OperationFormat::RC20_1 ) )
          return( true );
      }
    }

    return( false );
  };

  static set<pair<WIR_BaseProcessor::OpCode,
                  WIR_BaseProcessor::OperationFormat>> getMOVOps = {
    { RV32I::OpCode::LUI, RV32I::OperationFormat::RC20_1 },
    { RV32IC::OpCode::CLI, RV32IC::OperationFormat::SRC6_1 },
    { RV32IC::OpCode::CLUI, RV32IC::OperationFormat::SRC20_1 } };

  // Handle operations that produce a constant value in a REG as very first
  // parameter. Of course, those operations that getMOV produces must be
  // excluded here.
  if ( ( o.getExplicitParameters().size() > 0 ) &&
       ( firstParam( o ).getType() == WIR_ParameterType::reg ) &&
       ( firstRP( o ).isDefined() || firstRP( o ).isDefUsed() ) &&
       // Exclude operations listed in getMOVOps from folding, i.e., LUI.
       !getMOVOps.count( { o.getOpCode(), o.getOperationFormat() } ) &&
       // Exclude ADDIs with x0 as second parameter from folding.
       !isADDIx0( o ) &&
       // Exclude ADDIs receiving input from just one LUI from folding.
       !isADDILUI( o ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ),
      getMOV( firstParam( o ), outValue.at( firstID( o ) ) ) );

    return( true );
  }

  return( false );
};


/*
  For an operation identified to be constant, doBranchFolding performs RISC-V-
  specific folding of conditional branches.

  If the result of a jump condition is statically known, the conditional branch
  gets folded either to an unconditional branch or gets removed completely.

  doBranchFolding does not actually modify the currently examined WIR operation
  o. Instead, new instructions realizing the constant folding of o are added to
  map mNewInstructions.
*/
bool RV_ConstFold::doBranchFolding( const WIR_Operation &o,
                                    const std::map<
                                      WIR_id_t, WIR_UpDownValue> &inValue )
{
  DSTART(
    "virtual bool RV_ConstFold::doBranchFolding(const WIR_Operation&, const "
    "map<long long unsigned int, WIR_UpDownValue>&)" );

  // BEQ rs1, rs1, offset -> J offset
  // BGE rs1, rs1, offset -> J offset
  // BGEU rs1, rs1, offset -> J offset
   if ( ( ( o.getOpCode() == RV32I::OpCode::BEQ ) ||
          ( o.getOpCode() == RV32I::OpCode::BGE ) ||
          ( o.getOpCode() == RV32I::OpCode::BGEU ) ) &&
        ( o.getOperationFormat() == RV32I::OperationFormat::RRL_1 ) &&
        ( firstReg( o ) == nthReg( o, 1 ) ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getJ( lastLabel( o ) ) );
    return( true );
  }

  // BEQ rs1, rs2, offset -> J offset
  if ( ( o.getOpCode() == RV32I::OpCode::BEQ ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRL_1 ) &&
       ( (WIR_L4)( firstVal( o, inValue ).extend( 32 ) ==
           nthVal( o, inValue, 1 ) ) == WIR_L4::b1 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getJ( lastLabel( o ) ) );
    return( true );
  }

  // BEQ rs1, rs2, offset -> <nothing>
  if ( ( o.getOpCode() == RV32I::OpCode::BEQ ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRL_1 ) &&
       ( ( firstVal( o, inValue ) == nthVal( o, inValue, 1 ) ) ==
             WIR_L4::b0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), list<WIR_Instruction> {} );
    return( true );
  }

  // BGE rs1, rs2, offset -> J offset
  // BGEU rs1, rs2, offset -> J offset
  if ( ( ( o.getOpCode() == RV32I::OpCode::BGE ) ||
         ( o.getOpCode() == RV32I::OpCode::BGEU ) ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRL_1 ) &&
       ( (WIR_L4)( firstVal( o, inValue ) >= nthVal( o, inValue, 1 ) ) ==
           WIR_L4::b1 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getJ( lastLabel( o ) ) );
    return( true );
  }

  // BGE rs1, rs2, offset -> <nothing>
  // BGEU rs1, rs2, offset -> <nothing>
  if ( ( ( o.getOpCode() == RV32I::OpCode::BGE ) ||
         ( o.getOpCode() == RV32I::OpCode::BGEU ) ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRL_1 ) &&
       ( (WIR_L4)( firstVal( o, inValue ) >= nthVal( o, inValue, 1 ) ) ==
           WIR_L4::b0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), list<WIR_Instruction> {} );
    return( true );
  }

  // BNE rs1, rs1, offset -> <nothing>
  // BLT rs1, rs1, offset -> <nothing>
  // BLTU rs1, rs1, offset -> <nothing>
  if ( ( ( o.getOpCode() == RV32I::OpCode::BNE ) ||
         ( o.getOpCode() == RV32I::OpCode::BLT ) ||
         ( o.getOpCode() == RV32I::OpCode::BLTU ) ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRL_1 ) &&
       ( firstReg( o ) == nthReg( o, 1 ) ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), list<WIR_Instruction> {} );
    return( true );
  }

  // BLT rs1, rs2, offset -> J offset
  // BLTU rs1, rs2, offset -> J offset
  if ( ( ( o.getOpCode() == RV32I::OpCode::BLT ) ||
         ( o.getOpCode() == RV32I::OpCode::BLTU ) ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRL_1 ) &&
       ( (WIR_L4)( firstVal( o, inValue ) < nthVal( o, inValue, 1 ) ) ==
           WIR_L4::b1 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getJ( lastLabel( o ) ) );
    return( true );
  }

  // BLT rs1, rs2, offset -> <nothing>
  // BLTU rs1, rs2, offset -> <nothing>
  if ( ( ( o.getOpCode() == RV32I::OpCode::BLT ) ||
         ( o.getOpCode() == RV32I::OpCode::BLTU ) ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRL_1 ) &&
       ( ( firstVal( o, inValue ) < nthVal( o, inValue, 1 ) ) ==
             WIR_L4::b0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), list<WIR_Instruction> {} );
    return( true );
  }

  // BNE rs1, rs2, offset -> J offset
  if ( ( o.getOpCode() == RV32I::OpCode::BNE ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRL_1 ) &&
       ( (WIR_L4)( firstVal( o, inValue ) != nthVal( o, inValue, 1 ) ) ==
           WIR_L4::b1 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getJ( lastLabel( o ) ) );
    return( true );
  }

  // BNE rs1, rs2, offset -> <nothing>
  if ( ( o.getOpCode() == RV32I::OpCode::BNE ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRL_1 ) &&
       ( ( firstVal( o, inValue ) != nthVal( o, inValue, 1 ) ) ==
             WIR_L4::b0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), list<WIR_Instruction> {} );
    return( true );
  }

  // C.BEQZ rs1', offset -> J offset
  if ( ( o.getOpCode() == RV32IC::OpCode::CBEQZ ) &&
       ( (WIR_L4)( firstVal( o, inValue ) ==
             WIR_UpDownValue( WIR_L4::b0, 32, true ) ) == WIR_L4::b1 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getCJ( lastLabel( o ) ) );
    return( true );
  }

  // C.BEQZ rs1', offset -> <nothing>
  if ( ( o.getOpCode() == RV32IC::OpCode::CBEQZ ) &&
       ( ( firstVal( o, inValue ) ==
             WIR_UpDownValue( WIR_L4::b0, 32, true ) ) == WIR_L4::b0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), list<WIR_Instruction> {} );
    return( true );
  }

  // C.BNEZ rs1', offset -> J offset
  if ( ( o.getOpCode() == RV32IC::OpCode::CBNEZ ) &&
       ( (WIR_L4)( firstVal( o, inValue ) !=
             WIR_UpDownValue( WIR_L4::b0, 32, true ) ) == WIR_L4::b1 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getCJ( lastLabel( o ) ) );
    return( true );
  }

  // C.BNEZ rs1', offset -> <nothing>
  if ( ( o.getOpCode() == RV32IC::OpCode::CBNEZ ) &&
       ( ( firstVal( o, inValue ) !=
             WIR_UpDownValue( WIR_L4::b0, 32, true ) ) == WIR_L4::b0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), list<WIR_Instruction> {} );
    return( true );
  }

  return( false );
};


/*
  For an operation identified to be constant, doMOVFolding performs RISC-V-
  specific folding into register MOV operations.

  If the computations of an operation are statically known to have no
  arithmetical effect, doMOVFolding folds the operation into a register MOV.

  doMOVFolding does not actually modify the currently examined WIR operation o.
  Instead, new instructions realizing the constant folding of o are added to map
  mNewInstructions.
*/
bool RV_ConstFold::doMOVFolding( const WIR_Operation &o,
                                 const std::map<
                                   WIR_id_t, WIR_UpDownValue> &inValue )
{
  DSTART(
    "virtual bool RV_ConstFold::doMOVFolding(const WIR_Operation&, const "
    "map<long long unsigned int, WIR_UpDownValue>&)" );

  // A small lambda to check whether an operation is an ADDI with x0 as second
  // parameter.
  auto isADDIx0 = [&]( const WIR_Operation &op ) -> bool {
    if ( ( op.getOpCode() == RV32I::OpCode::ADDI ) &&
         ( op.getOperationFormat() == RV32I::OperationFormat::RRC12_1 ) )
      return( RV32I::isX0( nthReg( op, 1 ) ) );

    return( false );
  };

  // ADD rd, rs1, <0> -> MOV rd, rs1
  if ( ( o.getOpCode() == RV32I::OpCode::ADD ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       lastVal( o, inValue ).isInteger() &&
       ( replace( lastVal( o, inValue ), WIR_L4::b0 ) == 0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // ADDI rd, rs1, 0 -> MOV rd, rs1
  if ( !isADDIx0( o ) && ( o.getOpCode() == RV32I::OpCode::ADDI ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::imm ) &&
       ( dynamic_cast<const WIR_BaseImmediateParameter &>(
           lastParam( o ) ).getSignedValue() == 0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // ADD rd, <0>, rs2 -> MOV rd, rs2
  if ( ( o.getOpCode() == RV32I::OpCode::ADD ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       ( o.getExplicitParameters().size() == 3 ) &&
       nthVal( o, inValue, 1 ).isInteger() &&
       ( replace( nthVal( o, inValue ,1 ), WIR_L4::b0 ) == 0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), lastRP( o ) ) );
    return( true );
  }

  // SUB rd, rs1, <0> -> MOV rd, rs1
  if ( ( o.getOpCode() == RV32I::OpCode::SUB ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       lastVal( o, inValue ).isInteger() &&
       ( replace( lastVal( o, inValue ), WIR_L4::b0 ) == 0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // CSUB rd, <0> -> <nothing>
  if ( ( o.getOpCode() == RV32IC::OpCode::CSUB ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       lastVal( o, inValue ).isInteger() &&
       ( replace( lastVal( o, inValue ), WIR_L4::b0 ) == 0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), list<WIR_Instruction> {} );
    return( true );
  }

  // MUL rd, rs1, <1> -> MOV rd, rs1
  // MUL.H rd, rs1, <1> -> MOV rd, rs1
  // MUL.HU rd, rs1, <1> -> MOV rd, rs1
  // MUL.HSU rd, rs1, <1> -> MOV rd, rs1
  if ( ( ( o.getOpCode() == RV32IM::OpCode::MUL ) ||
         ( o.getOpCode() == RV32IM::OpCode::MULH ) ||
         ( o.getOpCode() == RV32IM::OpCode::MULHU ) ||
         ( o.getOpCode() == RV32IM::OpCode::MULHSU ) ) &&
       ( firstReg( o ).getBitWidth() == 32 ) &&
       ( ( ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
           lastVal( o, inValue ).isInteger() &&
           ( replace( lastVal( o, inValue ), WIR_L4::b0 ) == 1 ) ) ||
         ( ( lastParam( o ).getType() == WIR_ParameterType::imm ) &&
           ( dynamic_cast<const WIR_BaseImmediateParameter &>(
               lastParam( o ) ).getSignedValue() == 1 ) ) ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // MUL rd, <1>, rs2 -> MOV rd, rs2
  // MUL.H rd, <1>, rs2  -> MOV rd, rs2
  // MUL.HU rd, <1>, rs2  -> MOV rd, rs2
  // MUL.HSU rd, <1>, rs2  -> MOV rd, rs2
  if ( ( ( o.getOpCode() == RV32IM::OpCode::MUL ) ||
         ( o.getOpCode() == RV32IM::OpCode::MULH ) ||
         ( o.getOpCode() == RV32IM::OpCode::MULHU ) ||
         ( o.getOpCode() == RV32IM::OpCode::MULHSU ) ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       ( o.getExplicitParameters().size() == 3 ) &&
       nthVal( o, inValue, 1 ).isInteger() &&
       ( replace( nthVal( o, inValue, 1 ), WIR_L4::b0 ) == 1 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), lastRP( o ) ) );
    return( true );
  }

  // DIV rd, rs1, <1> -> MOV rd, rs1
  // DIV.U rd, rs1, <1> -> MOV rd, rs1
  if ( ( ( o.getOpCode() == RV32IM::OpCode::DIV ) ||
         ( o.getOpCode() == RV32IM::OpCode::DIVU ) ) &&
       ( firstReg( o ).getBitWidth() == 32 ) &&
       ( ( ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
           lastVal( o, inValue ).isInteger() &&
           ( replace( lastVal( o, inValue ), WIR_L4::b0 ) == 1 ) ) ||
         ( ( lastParam( o ).getType() == WIR_ParameterType::imm ) &&
           ( dynamic_cast<const WIR_BaseImmediateParameter &>(
               lastParam( o ) ).getSignedValue() == 1 ) ) ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // REM rd, rs1, <0> -> MOV rd, rs1
  // REMU rd, rs1, <0> -> MOV rd, rs1
  if ( ( ( o.getOpCode() == RV32IM::OpCode::REM ) ||
         ( o.getOpCode() == RV32IM::OpCode::REMU ) ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       lastVal( o, inValue ).isInteger() &&
       ( replace( lastVal( o, inValue ), WIR_L4::b0 ) == 0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // REM rd, rs1, rs2 -> rs2>rs1 -> MOV rd, rs1
  // REMU rd, rs1, rs2 -> rs2>rs1 -> MOV rd, rs1
  if ( ( ( o.getOpCode() == RV32IM::OpCode::REM ) ||
         ( o.getOpCode() == RV32IM::OpCode::REMU ) ) &&
       ( o.getOperationFormat() == RV32IM::OperationFormat::RRR_1 ) &&
       ( (WIR_L4)( nthVal( o, inValue, 1 ) < lastVal( o, inValue  ) ) ==
           WIR_L4::b1 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // AND rd, rs1, rs1 -> MOV rd, rs1
  if ( ( o.getOpCode() == RV32I::OpCode::AND ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       ( o.getExplicitParameters().size() == 3 ) &&
       ( nthReg( o, 1 ) == lastReg( o ) ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // AND rd, rs1, <val of rs1> -> MOV rd, rs1
  if ( ( o.getOpCode() == RV32I::OpCode::AND ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       ( o.getExplicitParameters().size() == 3 ) &&
       ( (WIR_L4)( nthVal( o, inValue, 1 ) ==
           lastVal( o, inValue ).extend( 32 ) ) == WIR_L4::b1 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // CAND rd, rs1=<val of rd> -> <nothing>
  if ( ( o.getOpCode() == RV32IC::OpCode::CAND ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       ( o.getExplicitParameters().size() == 2 ) &&
       ( (WIR_L4)( firstVal( o, inValue ) ==
           lastVal( o, inValue ).extend( 32 ) ) == WIR_L4::b1 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), list<WIR_Instruction> {} );
    return( true );
  }

  // ANDI rd, rs1, <immediate val of rs1> -> MOV rd, rs1
  if ( ( o.getOpCode() == RV32I::OpCode::ANDI ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::imm ) &&
       ( o.getExplicitParameters().size() == 3 ) &&
       ( dynamic_cast<const WIR_BaseImmediateParameter &>(
           lastParam( o ) ).isUnsigned() ) &&
       ( replace( nthVal( o, inValue, 1 ), WIR_L4::b0 ) ==
           (long long) dynamic_cast<const WIR_BaseImmediateParameter &>(
               lastParam( o ) ).getUnsignedValue() ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // CANDI rd, <immediate val of rd> -> <nothing>
  if ( ( o.getOpCode() == RV32IC::OpCode::CANDI ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::imm ) &&
       ( o.getExplicitParameters().size() == 2 ) &&
       ( dynamic_cast<const WIR_BaseImmediateParameter &>(
           lastParam( o ) ).isUnsigned() ) &&
       ( replace( firstVal( o, inValue ), WIR_L4::b0 ) ==
           (long long) dynamic_cast<const WIR_BaseImmediateParameter &>(
             lastParam( o ) ).getUnsignedValue() ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), list<WIR_Instruction> {} );
    return( true );
  }

  // OR rd, rs1, rs1 -> MOV rd, rs1
  if ( ( o.getOpCode() == RV32I::OpCode::OR ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       ( o.getExplicitParameters().size() == 3 ) &&
       ( nthReg( o, 1 ) == lastReg( o ) ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // OR rd, rs1, <0> -> MOV rd, rs1
  // XOR rd, rs1, <0> -> MOV rd, rs1
  if ( ( ( o.getOpCode() == RV32I::OpCode::OR ) ||
         ( o.getOpCode() == RV32I::OpCode::XOR ) ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       lastVal( o, inValue ).isInteger() &&
       ( replace( lastVal( o, inValue ), WIR_L4::b0 ) == 0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // COR rd, <0> -> <nothing>
  // CXOR rd, <0> -> <nothing>
  if ( ( ( o.getOpCode() == RV32IC::OpCode::COR ) ||
         ( o.getOpCode() == RV32IC::OpCode::CXOR ) ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       lastVal( o, inValue ).isInteger() &&
       ( replace( lastVal( o, inValue ), WIR_L4::b0 ) == 0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), list<WIR_Instruction> {} );
    return( true );
  }

  // OR rd, <0>, rs2 -> MOV rd, rs2
  // XOR rd, <0>, rs2 -> MOV rd, rs2
  if ( ( ( o.getOpCode() == RV32I::OpCode::OR ) ||
         ( o.getOpCode() == RV32I::OpCode::XOR ) ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       ( o.getExplicitParameters().size() == 3 ) &&
       nthVal( o, inValue, 1 ).isInteger() &&
       ( replace( nthVal( o, inValue, 1 ), WIR_L4::b0 ) == 0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), lastRP( o ) ) );
    return( true );
  }

  // OR rd, rs1, <Val of rs1> -> MOV rd, rs1
  if ( ( o.getOpCode() == RV32I::OpCode::OR ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       ( o.getExplicitParameters().size() == 3 ) &&
       ( (WIR_L4)( nthVal( o, inValue, 1 ) ==
           lastVal( o, inValue ).extend( 32 ) ) == WIR_L4::b1 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // COR rd, rs1=<val of rd> -> <nothing>
  if ( ( o.getOpCode() == RV32IC::OpCode::COR ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       ( o.getExplicitParameters().size() == 2 ) &&
       ( (WIR_L4)( firstVal( o, inValue ) ==
           lastVal( o, inValue ).extend( 32 ) ) == WIR_L4::b1 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), list<WIR_Instruction> {} );
    return( true );
  }

  // ORI rd, rs1, 0 -> MOV rd, rs1
  // XORI rd, rs1, 0 -> MOV rd, rs1
  if ( ( ( o.getOpCode() == RV32I::OpCode::ORI ) ||
         ( o.getOpCode() == RV32I::OpCode::XORI ) ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::imm ) &&
       ( dynamic_cast<const WIR_BaseImmediateParameter &>(
           lastParam( o ) ).isUnsigned() ) &&
       ( dynamic_cast<const WIR_BaseImmediateParameter &>(
           lastParam( o ) ).getUnsignedValue() == 0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // ORI rd, rs1, <immediate val of rs1> -> MOV rd, rs1
  if ( ( o.getOpCode() == RV32I::OpCode::ORI ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::imm ) &&
       ( o.getExplicitParameters().size() == 3 ) &&
       ( dynamic_cast<const WIR_BaseImmediateParameter &>(
           lastParam( o ) ).isUnsigned() ) &&
       ( replace( nthVal( o, inValue, 1 ), WIR_L4::b0 ) ==
           (long long) dynamic_cast<const WIR_BaseImmediateParameter &>(
             lastParam( o ) ).getUnsignedValue() ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // SLL rd, rs1, <0> -> MOV rd, rs1
  // SRL rd, rs1, <0> -> MOV rd, rs1
  // SRA rd, rs1, <0> -> MOV rd, rs1
  if ( ( ( o.getOpCode() == RV32I::OpCode::SLL ) ||
         ( o.getOpCode() == RV32I::OpCode::SRL ) ||
         ( o.getOpCode() == RV32I::OpCode::SRA ) ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::reg ) &&
       lastVal( o, inValue ).isInteger() &&
       ( replace( lastVal( o, inValue ), WIR_L4::b0 ) == 0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  // SLLI rd, rs1, 0 -> MOV rd, rs1
  // SRLI rd, rs1, 0 -> MOV rd, rs1
  // SRAI rd, rs1, <0> -> MOV rd, rs1
  if ( ( ( o.getOpCode() == RV32I::OpCode::SLLI ) ||
         ( o.getOpCode() == RV32I::OpCode::SRLI ) ||
         ( o.getOpCode() == RV32I::OpCode::SRAI ) ) &&
       ( lastParam( o ).getType() == WIR_ParameterType::imm ) &&
       ( dynamic_cast<const WIR_BaseImmediateParameter &>(
           lastParam( o ) ).getUnsignedValue() == 0 ) ) {
    mNewInstructions.emplace(
      const_cast<WIR_Operation &>( o ), getMOV( firstRP( o ), nthRP( o, 1 ) ) );
    return( true );
  }

  return( false );
};


//
// Private class methods
//

/*
  getMOV generates RISC-V instructions loading a register with a constant.
*/
list<WIR_Instruction> RV_ConstFold::getMOV( const WIR_Parameter &p,
                                            const WIR_UpDownValue &v ) const
{
  DSTART(
    "list<WIR_Instruction> RV_ConstFold::getMOV(const WIR_Parameter&, const "
    "WIR_UpDownValue&) const" );


  list<WIR_Instruction> res;
  const auto &r =
    dynamic_cast<const WIR_RegisterParameter &>( p ).getRegister();
  auto &rp = insertMOV( r, v, res );

  // Copy bit-value container to the finally produced register parameter.
  auto *cont = new WIR_BitValues();
  rp.insertContainer( cont );

  for ( auto &outEdge :
          p.getContainers<WIR_BitValues>().begin()->get().getOutValues() ) {
    cont->insertOutValues(
      *(outEdge.rp), std::move( outEdge.downVal ), std::move( outEdge.upVal ) );

    // Patch target container so that it receives input data from rp.
    auto &tgtContainer =
      outEdge.rp->getContainers<WIR_BitValues>().begin()->get();
    auto inEdgeIt = tgtContainer.findInValues( p );
    inEdgeIt->rp = &rp;
  }

  for ( WIR_Instruction &i : res )
    for ( WIR_Operation &o : i )
      copyContainers( o, p.getOperation() );

  return( res );
};


/*
  getMOV generates RISC-V instructions performing a register-register MOV.
*/
list<WIR_Instruction> RV_ConstFold::getMOV( const WIR_RegisterParameter &t,
                                            const WIR_RegisterParameter &s )
{
  DSTART(
    "list<WIR_Instruction> RV_ConstFold::getMOV(const WIR_RegisterParameter&, "
    "const WIR_RegisterParameter&)" );

  list<WIR_Instruction> res;

  // Determine the used RISC-V processor core.
  auto &sys = mCurrentFunction->getCompilationUnit().getSystem();
  auto &p = sys.findSymbol( *mCurrentFunction ).getSection().getProcessor();

  // Determine whether we can generate 16-bit RV32IC operations.
  bool generateIC = m16BitOperations;
  auto *rv32ic = dynamic_cast<RV32IC *>( &p );
  if ( !rv32ic )
    generateIC = false;

  auto *rp1 = new WIR_RegisterParameter( t.getRegister(), WIR_Usage::def );
  auto *rp2 = new WIR_RegisterParameter( s.getRegister(), WIR_Usage::use );

  if ( generateIC )
    res.push_back(
      { { RV32IC::OpCode::CMV, RV32IC::OperationFormat::SRR_1, rp1, rp2 } } );
  else
    res.push_back(
      { { RV32I::OpCode::MOV, RV32I::OperationFormat::RR_1, rp1, rp2 } } );

  // Ensure that all locations in the bit-wise DFG refering to the old parameter
  // t will be updated for the new parameter rp1.
  mNewLocation.insert( make_pair( t.getID(), ref( *rp1 ) ) );

  // Copy bit-value container to the finally produced register parameter.
  auto *cont1 = new WIR_BitValues();
  rp1->insertContainer( cont1 );

  for ( auto &outEdge :
          t.getContainers<WIR_BitValues>().begin()->get().getOutValues() ) {
    cont1->insertOutValues(
      *(outEdge.rp), std::move( outEdge.downVal ), std::move( outEdge.upVal ) );

    // Patch target container so that it receives input data from rp1.
    auto &tgtContainer =
      outEdge.rp->getContainers<WIR_BitValues>().begin()->get();
    auto inEdgeIt = tgtContainer.findInValues( t );
    inEdgeIt->rp = rp1;
  }

  // Ensure that all locations in the bit-wise DFG refering to the old parameter
  // s will be updated for the new parameter rp2.
  mNewLocation.insert( make_pair( s.getID(), ref( *rp2 ) ) );

  // Copy bit-value container to the finally used register parameter.
  auto *cont2 = new WIR_BitValues();
  rp2->insertContainer( cont2 );

  for ( auto &inEdge :
          s.getContainers<WIR_BitValues>().begin()->get().getInValues() ) {
    // It can be that an in-edge of parameter s refers to itself, i.e., is a
    // self-loop. In this case, make sure that a similar self-loop is created
    // for rp2.
    if ( *(inEdge.rp) == s )
      cont2->insertInValues(
        *rp2, std::move( inEdge.downVal ), std::move( inEdge.upVal ) );
    else
      cont2->insertInValues(
        *(inEdge.rp), std::move( inEdge.downVal ), std::move( inEdge.upVal ) );

    // Patch source container so that it provides input data to rp2.
    auto &srcContainer =
      inEdge.rp->getContainers<WIR_BitValues>().begin()->get();
    auto outEdgeIt = srcContainer.findOutValues( s );
    outEdgeIt->rp = rp2;
  }

  for ( WIR_Instruction &i : res )
    for ( WIR_Operation &o : i )
      copyContainers( o, t.getOperation() );

  return( res );
};


/*
  getJ generates RISC-V instructions unconditionally jumping to the given label.
*/
std::list<WIR_Instruction> RV_ConstFold::getJ( const WIR_LabelParameter &p ) const
{
  DSTART(
    "list<WIR_Instruction> RV_ConstFold::getJ(const WIR_LabelParameter&) "
    "const" );

  list<WIR_Instruction> res;

  // Use B for standard 32-bit operations.
  res.push_back(
    { { RV32I::OpCode::J, RV32I::OperationFormat::L_1,
        new WIR_LabelParameter( p ) } } );

  for ( WIR_Instruction &i : res )
    for ( WIR_Operation &o : i )
      copyContainers( o, p.getOperation() );

  return( res );
};


/*
  getCJ generates compressed RISC-V instructions unconditionally jumping to the
  given label.
*/
std::list<WIR_Instruction> RV_ConstFold::getCJ( const WIR_LabelParameter &p ) const
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  list<WIR_Instruction> res;

  // Use C.J for compressed 16-bit operations.
  res.push_back(
    { { RV32IC::OpCode::CJ, RV32IC::OperationFormat::SL_1,
        new WIR_LabelParameter( p ) } } );

  for ( WIR_Instruction &i : res )
    for ( WIR_Operation &o : i )
      copyContainers( o, p.getOperation() );

  return( res );
};


/*
  insertMOV generates the actual RISC-V instructions to load a register with a
  constant.
*/
WIR_RegisterParameter &RV_ConstFold::insertMOV( const WIR_BaseRegister &r,
                                                const WIR_UpDownValue &v,
                                                std::list<WIR_Instruction> &l ) const
{
  DSTART(
    "WIR_RegisterParameter& RV_ConstFold::insertMOV(const WIR_BaseRegister&, "
    "const WIR_UpDownValue&, list<WIR_Instruction>&) const" );

  auto *rp = new WIR_RegisterParameter( r, WIR_Usage::def );
  auto c0 = replace( v, WIR_L4::b0 );
  auto c1 = replace( v, WIR_L4::b1 );

  // Determine the processor core so that we can access special register x0.
  auto &sys = mCurrentFunction->getCompilationUnit().getSystem();
  auto &p = sys.findSymbol( *mCurrentFunction ).getSection().getProcessor();
  auto &rv = dynamic_cast<RV32I &>( p );

  // Determine whether we can generate 16-bit RV32IC operations.
  bool generateIC = m16BitOperations;
  auto *rv32ic = dynamic_cast<RV32IC *>( &p );
  if ( !rv32ic )
    generateIC = false;

  if ( generateIC &&
       ( c0 >= RV_Const6_Signed::getMinValue( 6 ) ) &&
       ( c0 <= RV_Const6_Signed::getMaxValue( 6 ) ) ) {
    // Generate 16-bit CLI.
    auto *imm = new RV_Const6_Signed( c0 );
    auto *cont = new WIR_BitValues();
    imm->insertContainer( cont );
    cont->insertInValues(
      *imm, WIR_UpDownValue( *imm ), WIR_UpDownValue( *imm ) );

    l.push_back(
      { { RV32IC::OpCode::CLI, RV32IC::OperationFormat::SRC6_1, rp, imm } } );
  } else

  if ( generateIC &&
       ( c1 >= RV_Const6_Signed::getMinValue( 6 ) ) &&
       ( c1 <= RV_Const6_Signed::getMaxValue( 6 ) ) ) {
    // Generate 16-bit CLI.
    auto *imm = new RV_Const6_Signed( c1 );
    auto *cont = new WIR_BitValues();
    imm->insertContainer( cont );
    cont->insertInValues(
      *imm, WIR_UpDownValue( *imm ), WIR_UpDownValue( *imm ) );

    l.push_back(
      { { RV32IC::OpCode::CLI, RV32IC::OperationFormat::SRC6_1, rp, imm } } );
  } else

  if ( ( c0 >= RV_Const12_Signed::getMinValue( 12 ) ) &&
       ( c0 <= RV_Const12_Signed::getMaxValue( 12 ) ) ) {
    // Generate 32-bit ADDI.
    auto *imm = new RV_Const12_Signed( c0 );
    auto *cont = new WIR_BitValues();
    imm->insertContainer( cont );
    cont->insertInValues(
      *imm, WIR_UpDownValue( *imm ), WIR_UpDownValue( *imm ) );

    auto *pNull = new WIR_RegisterParameter( rv.x0(), WIR_Usage::use );
    cont = new WIR_BitValues();
    pNull->insertContainer( cont );
    cont->insertInValues(
      *pNull, WIR_UpDownValue( WIR_L4::b0, 32, true ),
      WIR_UpDownValue( WIR_L4::b0, 32, true ) );

    l.push_back(
      { { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1, rp, *pNull,
          imm } } );
  } else

  if ( ( c1 >= RV_Const12_Signed::getMinValue( 12 ) ) &&
       ( c1 <= RV_Const12_Signed::getMaxValue( 12 ) ) ) {
    // Generate 32-bit ADDI.
    auto *imm = new RV_Const12_Signed( c1 );
    auto *cont = new WIR_BitValues();
    imm->insertContainer( cont );
    cont->insertInValues(
      *imm, WIR_UpDownValue( *imm ), WIR_UpDownValue( *imm ) );

    auto *pNull = new WIR_RegisterParameter( rv.x0(), WIR_Usage::use );
    cont = new WIR_BitValues();
    pNull->insertContainer( cont );
    cont->insertInValues(
      *pNull, WIR_UpDownValue( WIR_L4::b0, 32, true ),
      WIR_UpDownValue( WIR_L4::b0, 32, true ) );

    l.push_back(
      { { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1, rp, *pNull,
        imm } } );
  } else {
    int low, high;

    // Extract lowest 12 bits from constant, use 2-complement representation.
    low = c0 & 0xFFF;

    if ( low > RV_Const12_Signed::getMaxValue( 12 ) )
      low =
        RV_Const12_Signed::getMinValue( 12 ) +
        ( low - RV_Const12_Signed::getMaxValue( 12 ) ) - 1;

    // Extract upper part of constant.
    high = c0 - low;

    // We don't use '>> 12' here, since C does not specify whether >> performs
    // shifting with or without sign extension.
    for ( int i = 0; i < 12; ++i )
      high /= 2;

    if ( high < 0 )
      high += RV_Const20_Unsigned::getMaxValue( 20 ) + 1;

    // Generate LUI instruction.
    WIR_Instruction *lui = nullptr;
    if ( high != 0 ) {
      // Load value high in some up/down value in order to check whether a
      // 16-bit C.LUI is feasible.
      auto tmpVal = WIR_UpDownValue { RV_Const20_Unsigned( high ) };
      // auto tmpMSBs = tmpVal.extract( 5, 15 );

      if ( generateIC &&
           // According to the RISC-V RV32IC ISA, C.LUI should accept non-zero
           // 6-bit immediates which are internally sign-extended. However, the
           // RISC-V GNU assembler supports immediates from the subintervals
           // [0x1, 0x1f] and [0xfffe0, 0xfffff] (see also:
           // https://github.com/riscv-non-isa/riscv-asm-manual/pull/66/commits/6196c03643af1ea2dff103b58565e38776b0c34d).
           //
           // We thus put the following condition in the code that checks these
           // subintervals in order to make GNU-as happy.
           ( ( ( high >= 0x1 ) && ( high <= 0x1F ) ) ||
             ( ( high >= 0xFFFE0 ) && ( high <= 0xFFFFF ) ) ) ) {
        // Generate 16-bit CLUI.
        auto *imm = new RV_NZConst20_Unsigned( high );
        auto *cont = new WIR_BitValues();
        imm->insertContainer( cont );
        cont->insertInValues(
          *imm, WIR_UpDownValue( *imm ), WIR_UpDownValue( *imm ) );

        l.push_back(
          { { RV32IC::OpCode::CLUI, RV32IC::OperationFormat::SRC20_1,
              low != 0 ? new WIR_RegisterParameter( r, WIR_Usage::def ) : rp,
              imm } } );
      } else {
        // Generate 32-bit LUI.
        auto *imm = new RV_Const20_Unsigned( high );
        auto *cont = new WIR_BitValues();
        imm->insertContainer( cont );
        cont->insertInValues(
          *imm, WIR_UpDownValue( *imm ), WIR_UpDownValue( *imm ) );

        l.push_back(
          { { RV32I::OpCode::LUI, RV32I::OperationFormat::RC20_1,
              low != 0 ? new WIR_RegisterParameter( r, WIR_Usage::def ) : rp,
              imm } } );
      }
      lui = &(l.back());
    }

    // Generate ADDI instruction.
    if ( low != 0 ) {
      if ( !lui ) {
        if ( generateIC &&
             ( low >= RV_Const6_Signed::getMinValue( 6 ) ) &&
             ( low <= RV_Const6_Signed::getMaxValue( 6 ) ) ) {
          // Generate 16-bit CLI.
          auto *imm = new RV_Const6_Signed( low );
          auto *cont = new WIR_BitValues();
          imm->insertContainer( cont );
          cont->insertInValues(
            *imm, WIR_UpDownValue( *imm ), WIR_UpDownValue( *imm ) );

          l.push_back(
            { { RV32IC::OpCode::CLI, RV32IC::OperationFormat::SRC6_1, rp,
                imm } } );
        } else {
          // Generate 32-bit ADDI.
          auto *imm = new RV_Const12_Signed( low );
          auto *cont1 = new WIR_BitValues();
          imm->insertContainer( cont1 );
          cont1->insertInValues(
            *imm, WIR_UpDownValue( *imm ), WIR_UpDownValue( *imm ) );

          auto *pNull = new WIR_RegisterParameter( rv.x0(), WIR_Usage::use );
          auto *cont = new WIR_BitValues();
          pNull->insertContainer( cont );
          cont->insertInValues(
            *pNull, WIR_UpDownValue( WIR_L4::b0, 32, true ),
            WIR_UpDownValue( WIR_L4::b0, 32, true ) );

          l.push_back(
            { { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1, rp,
                *pNull, imm } } );
        }
      } else {
        // Generate 32-bit ADDI.
        auto *imm = new RV_Const12_Signed( low );
        auto *cont1 = new WIR_BitValues();
        imm->insertContainer( cont1 );
        cont1->insertInValues(
          *imm, WIR_UpDownValue( *imm ), WIR_UpDownValue( *imm ) );

        l.push_back(
          { { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1, rp,
              new WIR_RegisterParameter( r, WIR_Usage::use ), imm } } );
        const auto &addi = l.back();

        // Add bit-true data flow from LUI to ADDI.
        WIR_UpDownValue v1 { WIR_L4::b0, 32 };
        insert( v1, WIR_UpDownValue( RV_Const20_Unsigned( high ) ), 12 );

        WIR_Parameter &src = lui->begin()->get().begin()->get();
        WIR_Parameter &tgt = std::next( addi.begin()->get().begin() )->get();

        src.insertContainer( new WIR_BitValues() );
        tgt.insertContainer( new WIR_BitValues() );

        auto &srcContainer = src.getContainers<WIR_BitValues>().begin()->get();
        auto &tgtContainer = tgt.getContainers<WIR_BitValues>().begin()->get();

        srcContainer.insertOutValues(
          tgt, WIR_UpDownValue( v1 ), WIR_UpDownValue( v1 ) );
        tgtContainer.insertInValues(
          src, WIR_UpDownValue( v1 ), WIR_UpDownValue( v1 ) );
      }
    }
  }

  return( *rp );
};

}       // namespace WIR
