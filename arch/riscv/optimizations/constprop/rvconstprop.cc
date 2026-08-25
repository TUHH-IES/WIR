/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2025 - 2026, Heiko Falk.

*/

/*!
  @file rvconstprop.cc
  @brief This file implements a RISC-V-specific constant propagation
         optimization.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <sstream>

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32imfdc.h>
#include <arch/riscv/analyses/bit/rv32bitdfa.h>

// Include local headers
#include "rvconstprop.h"


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
RV_ConstProp::RV_ConstProp( WIR_System &s ) :
  WIR_Optimization { s },
  WIR_ConstProp { s }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for compilation unit-level optimization.
*/
RV_ConstProp::RV_ConstProp( WIR_CompilationUnit &c ) :
  WIR_Optimization { c },
  WIR_ConstProp { c }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for function-level optimization.
*/
RV_ConstProp::RV_ConstProp( WIR_Function &f ) :
  WIR_Optimization { f },
  WIR_ConstProp { f }
{
  DSTART( "RV_ConstProp::RV_ConstProp(WIR_Function&)" );
};


/*
  Destructor.
*/
RV_ConstProp::~RV_ConstProp( void )
{
  DSTART( "virtual RV_ConstProp::~RV_ConstProp()" );
};


//
// Protected class methods
//

/*
  runOptimization propagates constants in the given function.
*/
void RV_ConstProp::runOptimization( WIR_Function &f )
{
  DSTART( "virtual void RV_ConstProp::runOptimization(WIR_Function&)" );

  RV32_BitDFA analyzer { f };
  setDFA( analyzer );

  WIR_ConstProp::runOptimization( f );
};


/*
  doConstProp does the actual RISC-V-specific propagation of constants for a
  given operation.

  doConstProp does not actually modify the currently examined WIR operation o.
  Instead, new instructions realizing the constant folding of o are added to map
  mNewInstructions.
*/
bool RV_ConstProp::doConstProp( const WIR_Operation &o,
                                const std::map<
                                        WIR_id_t, WIR_UpDownValue> &inValue )
{
  DSTART(
    "virtual bool RV_ConstProp::doConstProp(const WIR_Operation&, const "
    "map<long long unsigned int, WIR_UpDownValue>&)" );

  //
  // Propagate operation formats RRR_1 into RRC12_1.
  //

  // Do copy propagation for "classical" signed 3-address operations.
  if ( ( ( o.getOpCode() == RV32I::OpCode::ADD ) ||
         ( o.getOpCode() == RV32I::OpCode::AND ) ||
         ( o.getOpCode() == RV32I::OpCode::OR ) ||
         ( o.getOpCode() == RV32I::OpCode::SLT ) ||
         ( o.getOpCode() == RV32I::OpCode::XOR ) ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRR_1 ) &&
       lastVal( o, inValue ).isInteger() ) {
    auto instrs = prop_RRR_RRC12( o, lastVal( o, inValue ) );
    if ( !instrs.empty() ) {
      mNewInstructions.emplace(
        const_cast<WIR_Operation &>( o ), std::move( instrs ) );
      return( true );
    }
  }

  // Do copy propagation for R-type shift operations (SLL, SRA, SRL).
  if ( ( ( o.getOpCode() == RV32I::OpCode::SLL ) ||
         ( o.getOpCode() == RV32I::OpCode::SRA ) ||
         ( o.getOpCode() == RV32I::OpCode::SRL ) ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRR_1 ) &&
       lastVal( o, inValue ).isInteger() ) {
    auto instrs = prop_RRR_RRC5( o, lastVal( o, inValue ) );
    if ( !instrs.empty() ) {
      mNewInstructions.emplace(
        const_cast<WIR_Operation &>( o ), std::move( instrs ) );
      return( true );
    }
  }

  // Do copy propagation for compressed instructions like CADD and CAND.
  if ( ( ( o.getOpCode() == RV32IC::OpCode::CADD ) ||
         ( o.getOpCode() == RV32IC::OpCode::CAND ) ) &&
       ( o.getOperationFormat() == RV32IC::OperationFormat::SRR_2 ) &&
       lastVal( o, inValue ).isInteger() ) {
    auto instrs = prop_SRR_SRC6( o, lastVal( o, inValue ) );
    if ( !instrs.empty() ) {
      mNewInstructions.emplace(
        const_cast<WIR_Operation &>( o ), std::move( instrs ) );
      return( true );
    }
  }

  // Exploit commutativity of some of the above operations.
  if ( ( ( o.getOpCode() == RV32I::OpCode::ADD ) ||
         ( o.getOpCode() == RV32I::OpCode::AND ) ||
         ( o.getOpCode() == RV32I::OpCode::OR ) ||
         ( o.getOpCode() == RV32I::OpCode::XOR ) ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRR_1 ) &&
       nthVal( o, inValue, 1 ).isInteger() ) {
    auto instrs = prop_RRR_RRC12_2( o, nthVal( o, inValue, 1 ) );
    if ( !instrs.empty() ) {
      mNewInstructions.emplace(
        const_cast<WIR_Operation &>( o ), std::move( instrs ) );
      return( true );
    }
  }

  return( false );
};


//
// Private class methods
//

/*
  prop_RRR_RRC12 propagates constants for a RISC-V operation of format RRR_1.

  If the given up value does not fit into a 12 bits signed constant,
  prop_RRR_RRC12 returns an empty instruction list.
*/
list<WIR_Instruction> RV_ConstProp::prop_RRR_RRC12( const WIR_Operation &o,
                                                    const WIR_UpDownValue &v )
{
  DSTART(
    "list<WIR_Instruction> RV_ConstProp::prop_RRR_RRC12(const WIR_Operation&, "
    "const WIR_UpDownValue&)" );

  list<WIR_Instruction> res;

  auto c0 = replace( v, WIR_L4::b0, true );
  auto c1 = replace( v, WIR_L4::b1, true );

  // Determine whether c0 or c1 are 12 bits signed constants.
  bool c0fits =
    ( c0 >= RV_Const12_Signed::getMinValue( 12 ) ) &&
    ( c0 <= RV_Const12_Signed::getMaxValue( 12 ) );
  bool c1fits =
    ( c1 >= RV_Const12_Signed::getMinValue( 12 ) ) &&
    ( c1 <= RV_Const12_Signed::getMaxValue( 12 ) );

  if ( c0fits || c1fits ) {
    // Do the actual constant propagation of the last register parameter of
    // format RRR_1 to RRC12_1, resp.
    auto it = o.getExplicitParameters().begin();
    const auto &p1o = it->get();

    auto *p1 =
      new WIR_RegisterParameter(
        dynamic_cast<const WIR_RegisterParameter &>( (it++)->get() ) );
    auto *p2 =
      new WIR_RegisterParameter(
        dynamic_cast<const WIR_RegisterParameter &>( it->get() ) );
    const auto &p2o = it->get();
    auto *p3 = new RV_Const12_Signed( c0fits ? c0 : c1 );

    // Patch incoming bit-values for all uses of p1.
    patchDefinedParameter( *p1, p1o );

    if ( p1->isDefUsed() )
      // Patch outgoing bit-values for all definitions of p1, if so.
      patchUsedParameter( *p1, p1o );

    // Patch outgoing bit-values for all definitions of p2.
    patchUsedParameter( *p2, p2o );

    // Add bit-value container for new immediate parameter p3.
    patchImmediateParameter( *p3 );

    // ADD rd, rs1, rs2=<const> -> ADDI rd, rs1, <const>
    if ( o.getOpCode() == RV32I::OpCode::ADD )
      res.push_back(
        { { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1,
            p1, p2, p3 } } );

    // AND rd, rs1, rs2=<const> -> ANDI rd, rs1, <const>
    if ( o.getOpCode() == RV32I::OpCode::AND )
      res.push_back(
        { { RV32I::OpCode::ANDI, RV32I::OperationFormat::RRC12_1,
            p1, p2, p3 } } );

    // OR rd, rs1, rs2=<const> -> ORI rd, rs1, <const>
    if ( o.getOpCode() == RV32I::OpCode::OR )
      res.push_back(
        { { RV32I::OpCode::ORI, RV32I::OperationFormat::RRC12_1,
            p1, p2, p3 } } );

    // SLT rd, rs1, rs2=<const> -> SLTI rd, rs1, <const>
    if ( o.getOpCode() == RV32I::OpCode::SLT )
      res.push_back(
        { { RV32I::OpCode::SLTI, RV32I::OperationFormat::RRC12_1,
            p1, p2, p3 } } );

    // XOR rd, rs1, rs2=<const> -> XORI rd, rs1, <const>
    if ( o.getOpCode() == RV32I::OpCode::XOR )
      res.push_back(
        { { RV32I::OpCode::XORI, RV32I::OperationFormat::RRC12_1,
            p1, p2, p3 } } );

    checkSelfEdges( res.back().begin()->get(), o );
    copyContainers( res.back().begin()->get(), o );
  }

  return( res );
};


/*
  prop_RRR_RRC12_2 propagates constants for a commutative RISC-V operation of
  format RRR_1.

  If the given up value does not fit into a 12 bits signed constant,
  prop_RRR_RRC12_2 returns an empty instruction list.
*/
list<WIR_Instruction> RV_ConstProp::prop_RRR_RRC12_2( const WIR_Operation &o,
                                                      const WIR_UpDownValue &v )
{
  DSTART(
    "list<WIR_Instruction> RV_ConstProp::prop_RRR_RRC12_2(const "
    "WIR_Operation&, const WIR_UpDownValue&)" );

  list<WIR_Instruction> res;

  auto c0 = replace( v, WIR_L4::b0, true );
  auto c1 = replace( v, WIR_L4::b1, true );

  // Determine whether c0 or c1 are 12 bits signed constants.
  bool c0fits =
    ( c0 >= RV_Const12_Signed::getMinValue( 12 ) ) &&
    ( c0 <= RV_Const12_Signed::getMaxValue( 12 ) );
  bool c1fits =
    ( c1 >= RV_Const12_Signed::getMinValue( 12 ) ) &&
    ( c1 <= RV_Const12_Signed::getMaxValue( 12 ) );

  if ( c0fits || c1fits ) {
    // Do the actual constant propagation of the second register parameter of
    // format RRR_1 to RRC12_1, resp.
    auto it = o.getExplicitParameters().begin();
    const auto &p1o = it->get();

    auto *p1 =
      new WIR_RegisterParameter(
        dynamic_cast<const WIR_RegisterParameter &>( (it++)->get() ) );
    ++it;
    auto *p2 =
      new WIR_RegisterParameter(
        dynamic_cast<const WIR_RegisterParameter &>( it->get() ) );
    const auto &p3o = it->get();
    auto *p3 = new RV_Const12_Signed( c0fits ? c0 : c1 );

    // Patch incoming bit-values for all uses of p1.
    patchDefinedParameter( *p1, p1o );

    if ( p1->isDefUsed() )
      // Patch outgoing bit-values for all definitions of p1, if so.
      patchUsedParameter( *p1, p1o );

    // Patch outgoing bit-values for all definitions of p2.
    patchUsedParameter( *p2, p3o );

    // Add bit-value container for new immediate parameter p3.
    patchImmediateParameter( *p3 );

    // ADD rd, rs1=<const>, rs2 -> ADDI rd, rs2, <const>
    if ( o.getOpCode() == RV32I::OpCode::ADD )
      res.push_back(
        { { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1,
            p1, p2, p3 } } );

    // AND rd, rs1=<const>, rs2 -> ANDI rd, rs2, <const>
    if ( o.getOpCode() == RV32I::OpCode::AND )
      res.push_back(
        { { RV32I::OpCode::ANDI, RV32I::OperationFormat::RRC12_1,
            p1, p2, p3 } } );

    // OR rd, rs1=<const>, rs2 -> ORI rd, rs2, <const>
    if ( o.getOpCode() == RV32I::OpCode::OR )
      res.push_back(
        { { RV32I::OpCode::ORI, RV32I::OperationFormat::RRC12_1,
            p1, p2, p3 } } );

    // XOR rd, rs1=<const>, rs2 -> XORI rd, rs2, <const>
    if ( o.getOpCode() == RV32I::OpCode::XOR )
      res.push_back(
        { { RV32I::OpCode::XORI, RV32I::OperationFormat::RRC12_1,
            p1, p2, p3 } } );

    checkSelfEdges( res.back().begin()->get(), o );
    copyContainers( res.back().begin()->get(), o );
  }

  return( res );
};


/*
  prop_RRR_RRC5 propagates constants for a RISC-V operation of format RRR_1.

  If the given up value does not fit into a 5 bits unsigned constant,
  prop_RRR_RRC5 returns an empty instruction list.
*/
list<WIR_Instruction> RV_ConstProp::prop_RRR_RRC5( const WIR_Operation &o,
                                                   const WIR_UpDownValue &v )
{
  DSTART(
    "list<WIR::WIR_Instruction> RV_ConstProp::prop_RRR_RRC5(const "
    "WIR_Operation&, const WIR_UpDownValue&)" );

  list<WIR_Instruction> res;

  auto c0 = replace( v, WIR_L4::b0, true );
  auto c1 = replace( v, WIR_L4::b1, true );

  // Determine whether c0 or c1 are 5 bits unsigned constants.
  bool c0fits =
    ( c0 >= 0 ) && ( c0 <= (long long) RV_Const5_Unsigned::getMaxValue( 5 ) );
  bool c1fits =
    ( c1 >= 0 ) && ( c1 <= (long long) RV_Const5_Unsigned::getMaxValue( 5 ) );

  if ( c0fits || c1fits ) {
    // Do the actual constant propagation of the last register parameter of
    // format RRR_1 to RRC5_1, resp.
    auto it = o.getExplicitParameters().begin();
    const auto &p1o = it->get();

    auto *p1 =
      new WIR_RegisterParameter(
        dynamic_cast<const WIR_RegisterParameter &>( (it++)->get() ) );
    auto *p2 =
      new WIR_RegisterParameter(
        dynamic_cast<const WIR_RegisterParameter &>( it->get() ) );
    const auto &p2o = it->get();
    auto *p3 = new RV_Const5_Unsigned( c0fits ? c0 : c1 );

    // Patch incoming bit-values for all uses of p1.
    patchDefinedParameter( *p1, p1o );

    if ( p1->isDefUsed() )
      // Patch outgoing bit-values for all definitions of p1, if so.
      patchUsedParameter( *p1, p1o );

    // Patch outgoing bit-values for all definitions of p2.
    patchUsedParameter( *p2, p2o );

    // Add bit-value container for new immediate parameter p3.
    patchImmediateParameter( *p3 );

    // SLL rd, rs1, rs2=<const> -> SLLI rd, rs1, <const>
    if ( o.getOpCode() == RV32I::OpCode::SLL )
      res.push_back(
        { { RV32I::OpCode::SLLI, RV32I::OperationFormat::RRC5_1,
            p1, p2, p3 } } );

    // SRA rd, rs1, rs2=<const> -> SRAI rd, rs1, <const>
    if ( o.getOpCode() == RV32I::OpCode::SRA )
      res.push_back(
        { { RV32I::OpCode::SRAI, RV32I::OperationFormat::RRC5_1,
            p1, p2, p3 } } );

    // SRL rd, rs1, rs2=<const> -> SRLI rd, rs1, <const>
    if ( o.getOpCode() == RV32I::OpCode::SRL )
      res.push_back(
        { { RV32I::OpCode::SRLI, RV32I::OperationFormat::RRC5_1,
            p1, p2, p3 } } );

    checkSelfEdges( res.back().begin()->get(), o );
    copyContainers( res.back().begin()->get(), o );
  }

  return( res );
};


/*
  prop_SRR_SRC6 propagates constants for a RISC-V operation of format SRR_2.

  If the given up value does not fit into a 6 bits signed constant,
  prop_SRR_SRC6 returns an empty instruction list.
*/
list<WIR_Instruction> RV_ConstProp::prop_SRR_SRC6( const WIR_Operation &o,
                                                   const WIR_UpDownValue &v )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  list<WIR_Instruction> res;

  auto c0 = replace( v, WIR_L4::b0, true );
  auto c1 = replace( v, WIR_L4::b1, true );

  // Determine whether c0 or c1 are 6 bits signed constants.
  bool c0fits =
    ( c0 >= RV_Const6_Signed::getMinValue( 6 ) ) &&
    ( c0 <= RV_Const6_Signed::getMaxValue( 6 ) );
  bool c1fits =
    ( c1 >= RV_Const6_Signed::getMinValue( 6 ) ) &&
    ( c1 <= RV_Const6_Signed::getMaxValue( 6 ) );

  if ( c0fits || c1fits ) {
    // Do the actual constant propagation of the last register parameter of
    // format SRR_2 to SRC6_2, resp.
    auto it = o.getExplicitParameters().begin();
    const auto &p1o = it->get();

    auto *p1 =
      new WIR_RegisterParameter(
        dynamic_cast<const WIR_RegisterParameter &>( (it++)->get() ) );
    auto *p2 = new RV_Const6_Signed( c0fits ? c0 : c1 );

    // Patch incoming bit-values for all uses of p1.
    patchDefinedParameter( *p1, p1o );

    if ( p1->isDefUsed() )
      // Patch outgoing bit-values for all definitions of p1, if so.
      patchUsedParameter( *p1, p1o );

    // Add bit-value container for new immediate parameter p2.
    patchImmediateParameter( *p2 );

    // CADD rd, rs1=<const> -> CADDI rd, <const>
    if ( o.getOpCode() == RV32IC::OpCode::CADD )
      res.push_back(
        { { RV32IC::OpCode::CADDI, RV32IC::OperationFormat::SRC6_2,
            p1, p2 } } );

    // CAND rd, rs1=<const> -> CANDI rd, <const>
    if ( o.getOpCode() == RV32IC::OpCode::CAND )
      res.push_back(
        { { RV32IC::OpCode::CANDI, RV32IC::OperationFormat::SRC6_2,
            p1, p2 } } );

    checkSelfEdges( res.back().begin()->get(), o );
    copyContainers( res.back().begin()->get(), o );
  }

  return( res );
};

}       // namespace WIR
