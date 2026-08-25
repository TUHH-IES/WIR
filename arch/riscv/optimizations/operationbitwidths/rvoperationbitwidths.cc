/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2025 - 2026, Heiko Falk.

*/

/*!
  @file rvoperationbitwidths.cc
  @brief This file implements a RISC-V-specific optimization reducing the bit
         widths of machine operations.

  @author Sasha Tosta <Sasha.Tosta@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32imfdc.h>

// Include local headers
#include "rvoperationbitwidths.h"


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
RV_OperationBitWidths::RV_OperationBitWidths( WIR_System &s ) :
  WIR_OperationBitWidths { s }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for compilation unit-level optimization.
*/
RV_OperationBitWidths::RV_OperationBitWidths( WIR_CompilationUnit &c ) :
  WIR_OperationBitWidths { c }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for function-level optimization.
*/
RV_OperationBitWidths::RV_OperationBitWidths( WIR_Function &f ) :
  WIR_OperationBitWidths { f }
{
  DSTART( "RV_OperationBitWidths::RV_OperationBitWidths(WIR_Function&)" );
};


/*
  Destructor.
*/
RV_OperationBitWidths::~RV_OperationBitWidths( void )
{
  DSTART( "virtual RV_OperationBitWidths::~RV_OperationBitWidths()" );
};


//
// Protected class methods
//

/*
  reduce reduces the bit width of the given RISC-V machine operation.

  Bit widths of branches are reduced whenever possible and without proper
  consideration of jump displacements. The correction of branches with incorrect
  displacements thus has to be done elsewhere afterwards.
*/
std::list<
  std::reference_wrapper<
    WIR_Operation>>::iterator RV_OperationBitWidths::reduce( WIR_Instruction& i,
                                                             std::list<
                                                               std::reference_wrapper<
                                                                 WIR_Operation>>::const_iterator pos ) const
{
  DSTART(
    "virtual list<reference_wrapper<WIR_Operation> >::iterator "
    "RV_OperationBitWidths::reduce(WIR_Instruction&, "
    "list<reference_wrapper<WIR_Operation> >::const_iterator) const" );

  const WIR_Operation &o = pos->get();

  //
  // Load and store operations.
  //

  // Stack pointer-based loads and stores.

  if ( ( o.getOpCode() == RV32I::OpCode::SW ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RC12R_2 ) &&
       RV32I::isSP( nthReg( o, 2 ) ) &&
       ( nthSVal( o, 1 ) >= 0 ) &&
       ( nthSVal( o, 1 ) <=
           (long long) RV_Const8_Unsigned::getMaxValue( 8 ) ) &&
       ( nthSVal( o, 1 ) % 4 == 0 ) )
    // SW rs2, offset[12-Bit, signed](x2) ->
    //   C.SWSP rs2, zxoffset[6-Bit, unsigned](x2)
    return(
      replace(
        pos,
        { RV32IC::OpCode::CSWSP, RV32IC::OperationFormat::SRC8R_2,
          nthParam( o, 0 ), new RV_Const8_Unsigned( nthSVal( o, 1 ) ),
          nthParam( o, 2 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::LW ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RC12R_1 ) &&
       RV32I::isSP( nthReg( o, 2 ) ) &&
       ( nthSVal( o, 1 ) >= 0 ) &&
       ( nthSVal( o, 1 ) <=
           (long long) RV_Const8_Unsigned::getMaxValue( 8 ) ) &&
       ( nthSVal( o, 1 ) % 4 == 0 ) )
    // LW rd, offset[12-Bit, signed](x2) ->
    //   C.LWSP rd, zxoffset[6-Bit, unsigned](x2)
    return(
      replace(
        pos,
        { RV32IC::OpCode::CLWSP, RV32IC::OperationFormat::SRC8R_1,
          nthParam( o, 0 ), new RV_Const8_Unsigned( nthSVal( o, 1 ) ),
          nthParam( o, 2 ) } ) );
  else

  // Register-based loads and stores.

  if ( ( o.getOpCode() == RV32I::OpCode::LW ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RC12R_1 ) &&
       ( nthSVal( o, 1 ) >= 0 ) &&
       ( nthSVal( o, 1 ) <=
           (long long) RV_Const7_Unsigned::getMaxValue( 7 ) ) &&
       ( nthSVal( o, 1 ) % 4 == 0 ) &&
       RV32IC::isPopularRegister( nthReg( o, 0 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 2 ) ) )
    // LW rd’, offset[12-Bit, signed](rs2’) ->
    //   C.LW rd’, zxoffset[5-Bit, unsigned](rs2’)
    return(
      replace(
        pos,
        { RV32IC::OpCode::CLW, RV32IC::OperationFormat::SRC7R_1,
          nthParam( o, 0 ), new RV_Const7_Unsigned( nthSVal( o, 1 ) ),
          nthParam( o, 2 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::SW ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RC12R_2 ) &&
       ( nthSVal( o, 1 ) >= 0 ) &&
       ( nthSVal( o, 1 ) <=
           (long long) RV_Const7_Unsigned::getMaxValue( 7 ) ) &&
       ( nthSVal( o, 1 ) % 4 == 0 ) &&
       RV32IC::isPopularRegister( nthReg( o, 0 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 2 ) ) )
    // SW rs2’, offset[12-Bit, signed](rs1’) ->
    //   C.SW rs2’, zxoffset[5-Bit, unsigned](rs1’)
    return(
      replace(
        pos,
        { RV32IC::OpCode::CSW, RV32IC::OperationFormat::SRC7R_2,
          nthParam( o, 0 ), new RV_Const7_Unsigned( nthSVal( o, 1 ) ),
          nthParam( o, 2 ) } ) );
  else


  //
  // Control transfer operations.
  //

  // Unconditional control transfer.

  if ( ( ( o.getOpCode() == RV32I::OpCode::J ) &&
         ( o.getOperationFormat() == RV32I::OperationFormat::L_1 ) ) ||
       ( ( o.getOpCode() == RV32I::OpCode::JAL ) &&
         ( o.getOperationFormat() == RV32I::OperationFormat::RL_1 ) &&
         RV32I::isX0( nthReg( o, 0 ) ) ) )
    // Note: J is a pseudo-operation, de facto realized by a JAL.
    // JAL x0, sxoffset[20-Bit, signed] (+-1 MiB Range) ->
    //   C.J sxoffset[11-Bit, signed] (+- 2 KiB Range)
    return(
      replace(
        pos,
        { RV32IC::OpCode::CJ, RV32IC::OperationFormat::SL_1,
          nthParam( o, 0 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::JAL ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RL_1 ) &&
       RV32I::isX1( nthReg( o, 0 ) ) )
    // JAL x1, sxoffset[20-Bit, signed] (+-1 MiB Range) ->
    //   C.JAL sxoffset[11-Bit, signed] (+- 2 KiB range)
    return(
      replace(
        pos,
        { RV32IC::OpCode::CJAL, RV32IC::OperationFormat::SL_1,
          nthParam( o, 1 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::JALR ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRC12_1 ) &&
       RV32I::isX0( nthReg( o, 0 ) ) &&
       ( nthSVal( o, 2 ) == 0 ) )
    // JALR x0, rs1, 0 ->
    //   C.JR rs1
    return(
      replace(
        pos,
        { RV32IC::OpCode::CJR, RV32IC::OperationFormat::SR_1,
          nthParam( o, 1 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::JALR ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRC12_1 ) &&
       RV32I::isX1( nthReg( o, 0 ) ) &&
       ( nthSVal( o, 2 ) == 0 ) )
    // JALR x1, rs1, 0 ->
    //   C.JALR rs1
{
cout << "Exit 08" << endl;
exit( 8 );
    return(
      replace(
        pos,
        { RV32IC::OpCode::CJALR, RV32IC::OperationFormat::SR_1,
          nthParam( o, 1 ) } ) );
}
  else

  // Conditional control transfer.

  if ( ( o.getOpCode() == RV32I::OpCode::BEQ ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRL_1 ) &&
       RV32I::isX0( nthReg( o, 1 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 0 ) ) )
    // BEQ rs2’, x0, offset[12-Bit, signed] (+-4 KiB Range) ->
    //   C.BEQZ rs2’, sxoffset[8-Bit, signed] (+-256 B range)
    return(
      replace(
        pos,
        { RV32IC::OpCode::CBEQZ, RV32IC::OperationFormat::SRL_1,
          nthParam( o, 0 ), nthParam( o, 2 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::BNE ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRL_1 ) &&
       RV32I::isX0( nthReg( o, 1 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 0 ) ) )
    // BNE rs2’, x0, offset[12-Bit, signed] (+-4 KiB Range) ->
    //   C.BNEZ rs2’, sxoffset[8-Bit, signed] (+-256 B range)
    return(
      replace(
        pos,
        { RV32IC::OpCode::CBNEZ, RV32IC::OperationFormat::SRL_1,
          nthParam( o, 0 ), nthParam( o, 2 ) } ) );
  else


  //
  // Integer computational operations.
  //

  if ( ( o.getOpCode() == RV32I::OpCode::ADDI ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRC12_1 ) &&
       RV32I::isX0( nthReg( o, 1 ) ) &&
       !RV32I::isX0( nthReg( o, 0 ) ) &&
       ( nthSVal( o, 2 ) >= RV_Const6_Signed::getMinValue( 6 ) ) &&
       ( nthSVal( o, 2 ) <= RV_Const6_Signed::getMaxValue( 6 ) ) )
    // ADDI rd, x0, imm[12-Bit, signed] ->
    //   C.LI rd, imm[6-Bit, signed]
    return(
      replace(
        pos,
        { RV32IC::OpCode::CLI, RV32IC::OperationFormat::SRC6_1,
          nthParam( o, 0 ), new RV_Const6_Signed( nthSVal( o, 2 ) ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::LUI ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RC20_1 ) &&
       !RV32I::isX0( nthReg( o, 0 ) ) &&
       !RV32I::isSP( nthReg( o, 0 ) ) ) {
    // LUI rd, imm[20-Bit, unsigned] -> C.LUI rd, nzuimm[6-Bit, unsigned]
    unsigned int upper15 = nthUVal( o, 1 ) & 0xFFFE0;
    unsigned int lower6 = nthUVal( o, 1 ) & 0x3F;

    if ( ( ( upper15 == 0xFFFE0 ) || ( upper15 == 0x0 ) ) &&
         ( lower6 != 0 ) )
      return(
        replace(
          pos,
          { RV32IC::OpCode::CLUI, RV32IC::OperationFormat::SRC20_1,
            nthParam( o, 0 ),
            new RV_NZConst20_Unsigned( nthUVal( o, 1 ) ) } ) );
  } else

  if ( ( o.getOpCode() == RV32I::OpCode::ADDI ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRC12_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       RV32I::isSP( nthReg( o, 0 ) ) &&
       ( nthSVal( o, 2 ) >= RV_Const10_Signed::getMinValue( 10 ) ) &&
       ( nthSVal( o, 2 ) <= RV_Const10_Signed::getMaxValue( 10 ) ) &&
       ( nthSVal( o, 2 ) != 0 ) &&
       ( nthSVal( o, 2 ) % 16 == 0 ) )
    // ADDI x2, x2, imm[12-Bit, signed] ->
    //   C.ADDI16SP x2, nzimm[6-Bit, signed]
    return(
      replace(
        pos,
        { RV32IC::OpCode::CADDI16SP, RV32IC::OperationFormat::SRC10_1,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          new RV_Const10_Signed( nthSVal( o, 2 ) ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::ADDI ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRC12_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       !RV32I::isX0( nthReg( o, 0 ) ) &&
       ( nthSVal( o, 2 ) >= RV_Const6_Signed::getMinValue( 6 ) ) &&
       ( nthSVal( o, 2 ) <= RV_Const6_Signed::getMaxValue( 6 ) ) &&
       ( nthSVal( o, 2 ) != 0 ) )
    // ADDI rd, rd, imm[12-Bit, signed] ->
    //   C.ADDI rd, nzimm[6-Bit, signed]
    return(
      replace(
        pos,
        { RV32IC::OpCode::CADDI, RV32IC::OperationFormat::SRC6_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          new RV_Const6_Signed( nthSVal( o, 2 ) ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::ADDI ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRC12_1 ) &&
       ( RV32IC::isPopularRegister( nthReg( o, 0 ) ) ) &&
       RV32I::isSP( nthReg( o, 1 ) ) &&
       ( nthSVal( o, 2 ) > 0 ) &&
       ( nthSVal( o, 2 ) <=
           (long long) RV_Const10_Unsigned::getMaxValue( 10 ) ) &&
       ( nthSVal( o, 2 ) % 4 == 0 ) )
    // ADDI rd', x2, imm[12-Bit, signed] ->
    //   C.ADDI4SPN rd', x2, nzuimm[8-Bit, unsigned]
    return(
      replace(
        pos,
        { RV32IC::OpCode::CADDI4SPN, RV32IC::OperationFormat::SRRC10_1,
          nthParam( o, 0 ), nthParam( o, 1 ),
          new RV_Const10_Unsigned( nthSVal( o, 2 ) ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::SLLI ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRC5_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       ( nthUVal( o, 2 ) > 0 ) )
    // SLLI rd, rd, shamt[5-Bit, unsigned] ->
    //   C.SLLI rd, shamt[5-Bit, unsigned]
    return(
      replace(
        pos,
        { RV32IC::OpCode::CSLLI, RV32IC::OperationFormat::SRC5_1,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 2 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::SRLI ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRC5_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       ( RV32IC::isPopularRegister( nthReg( o, 0 ) ) ) &&
       ( nthUVal( o, 2 ) > 0 ) )
    // SRLI rd', rd', shamt[5-Bit, unsigned] ->
    //   C.SRLI rd’, shamt[5-Bit, unsigned]
    return(
      replace(
        pos,
        { RV32IC::OpCode::CSRLI, RV32IC::OperationFormat::SRC5_1,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 2 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::SRAI ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRC5_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       ( RV32IC::isPopularRegister( nthReg( o, 0 ) ) ) &&
       ( nthUVal( o, 2 ) > 0 ) )
    // SRAI rd', rd', shamt[5-Bit, unsigned] ->
    //   C.SRAI rd’, shamt[5-Bit, unsigned]
    return(
      replace(
        pos,
        { RV32IC::OpCode::CSRAI, RV32IC::OperationFormat::SRC5_1,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 2 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::ANDI ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRC12_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       ( RV32IC::isPopularRegister( nthReg( o, 0 ) ) ) &&
       ( nthSVal( o, 2 ) >= RV_Const6_Signed::getMinValue( 6 ) ) &&
       ( nthSVal( o, 2 ) <= RV_Const6_Signed::getMaxValue( 6 ) ) )
    // ANDI rd', rd', imm[12-Bit, signed] ->
    //   C.ANDI rd’, imm[6-Bit, signed]
    return(
      replace(
        pos,
        { RV32IC::OpCode::CANDI, RV32IC::OperationFormat::SRC6_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          new RV_Const6_Signed( nthSVal( o, 2 ) ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::ADD ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRR_1 ) &&
       !RV32I::isX0( nthReg( o, 0 ) ) &&
       RV32I::isX0( nthReg( o, 1 ) ) &&
       !RV32I::isX0( nthReg( o, 2 ) ) )
    // ADD rd, x0, rs2 ->
    //   C.MV rd, rs2
{
cout << "Exit 20" << endl;
exit( 20 );
    return(
      replace(
        pos,
        { RV32IC::OpCode::CMV, RV32IC::OperationFormat::SRR_1,
          nthParam( o, 0 ), nthParam( o, 2 ) } ) );
}
  else

  if ( ( o.getOpCode() == RV32I::OpCode::ADD ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRR_1 ) &&
       !RV32I::isX0( nthReg( o, 0 ) ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       !RV32I::isX0( nthReg( o, 2 ) ) )
    // ADD rd, rd, rs2 ->
    //   C.ADD rd, rs2
    return(
      replace(
        pos,
        { RV32IC::OpCode::CADD, RV32IC::OperationFormat::SRR_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 2 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::ADD ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRR_1 ) &&
       !RV32I::isX0( nthReg( o, 0 ) ) &&
       ( nthReg( o, 0 ) == nthReg( o, 2 ) ) &&
       !RV32I::isX0( nthReg( o, 1 ) ) )
    // ADD rd, rs2, rd ->
    //   C.ADD rd, rs2
    return(
      replace(
        pos,
        { RV32IC::OpCode::CADD, RV32IC::OperationFormat::SRR_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 1 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::AND ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRR_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 0 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 2 ) ) )
    // AND rd', rd', rs2' ->
    //   C.AND rd’, rs2’
    return(
      replace(
        pos,
        { RV32IC::OpCode::CAND, RV32IC::OperationFormat::SRR_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 2 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::AND ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRR_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 2 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 0 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 1 ) ) )
    // AND rd', rs2', rd' ->
    //   C.AND rd’, rs2’
    return(
      replace(
        pos,
        { RV32IC::OpCode::CAND, RV32IC::OperationFormat::SRR_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 1 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::MOV ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RR_1 ) &&
       !RV32I::isX0( nthReg( o, 0 ) ) &&
       !RV32I::isX0( nthReg( o, 1 ) ) )
    // MOV rd, rs1 ->
    //   C.MV rd, rs1
    return(
      replace(
        pos,
        { RV32IC::OpCode::CMV, RV32IC::OperationFormat::SRR_1,
          nthParam( o, 0 ), nthParam( o, 1 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::OR ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRR_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 0 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 2 ) ) )
    // OR rd', rd', rs2' ->
    //   C.OR rd’, rs2’
    return(
      replace(
        pos,
        { RV32IC::OpCode::COR, RV32IC::OperationFormat::SRR_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 2 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::OR ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRR_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 2 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 0 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 1 ) ) )
    // OR rd', rs2', rd' ->
    //   C.OR rd’, rs2’
    return(
      replace(
        pos,
        { RV32IC::OpCode::COR, RV32IC::OperationFormat::SRR_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 1 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::XOR ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRR_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 0 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 2 ) ) )
    // XOR rd', rd', rs2' ->
    //   C.XOR rd’, rs2’
    return(
      replace(
        pos,
        { RV32IC::OpCode::CXOR, RV32IC::OperationFormat::SRR_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 2 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::XOR ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRR_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 2 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 0 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 1 ) ) )
    // XOR rd', rs2', rd' ->
    //   C.XOR rd’, rs2’
    return(
      replace(
        pos,
        { RV32IC::OpCode::CXOR, RV32IC::OperationFormat::SRR_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 1 ) } ) );
  else

  if ( ( o.getOpCode() == RV32I::OpCode::SUB ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::RRR_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 0 ) ) &&
       RV32IC::isPopularRegister( nthReg( o, 2 ) ) )
    // SUB rd', rd', rs2' ->
    //   C.SUB rd', rs2'
    return(
      replace(
        pos,
        { RV32IC::OpCode::CSUB, RV32IC::OperationFormat::SRR_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 2 ) } ) );
  else


  //
  // NOP operation.
  //

  if ( ( ( o.getOpCode() == RV32I::OpCode::ADDI ) ||
         ( o.getOpCode() == RV32I::OpCode::ADD ) ) &&
       RV32I::isX0( nthReg( o, 0 ) ) )
    // ADDI x0, *, * or ADD x0, *, * ->
    //   C.NOP
{
cout << "Exit 26" << endl;
exit( 26 );
    return(
      replace(
        pos, { RV32IC::OpCode::CNOP, RV32IC::OperationFormat::SNULL_1 } ) );
}
  else

  if ( ( o.getOpCode() == RV32I::OpCode::EBREAK ) &&
       ( o.getOperationFormat() == RV32I::OperationFormat::NULL_1 ) )
    // EBREAK -> C.EBREAK
{
cout << "Exit 27" << endl;
exit( 27 );
    return(
      replace(
        pos, { RV32IC::OpCode::CEBREAK, RV32IC::OperationFormat::SNULL_1 } ) );
}

  return(
    const_cast<list<reference_wrapper<WIR_Operation>>&>(
      i.getOperations() ).end() );
};

}       // namespace WIR
