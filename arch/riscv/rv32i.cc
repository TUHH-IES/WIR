/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32i.cc
  @brief This file implements the specific interface of the RISC-V RV32I Base
         Integer instruction set, version 2.0.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
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
#include <arch/riscv/rv32i.h>
#include <arch/riscv/rv32ic.h>
#include <arch/riscv/rv32if.h>
#include <arch/riscv/rv32im.h>
#include <arch/riscv/rv32ifc.h>
#include <arch/riscv/rv32ifd.h>
#include <arch/riscv/rv32imc.h>
#include <arch/riscv/rv32imf.h>
#include <arch/riscv/rv32ifdc.h>
#include <arch/riscv/rv32imfc.h>
#include <arch/riscv/rv32imfd.h>
#include <arch/riscv/rv32imfdc.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for RV32I processor architectures.
*/
RV32I::RV32I( void ) :
  WIR_Processor<RV32I> {}
{
  DSTART( "RV32I::RV32I()" );

  // Specify the processor architecture modeled by this class.
  setProcessorName( "RISC-V" );
  setISAName( "RV32I V2.0" );

  // Create physical registers bottom-up, i.e., from the leaf registers upwards
  // towards complex hierarchical registers.
  // For the meaning of the individual registers, please refer to file
  // doc/riscv-spec-v2.2.pdf, page 109.
  addPhReg<RV_RegP>( "0" );
  addPhReg<RV_RegP>( "1" );
  addPhReg<RV_RegP>( "2", true );
  addPhReg<RV_RegP>( "3" );
  addPhReg<RV_RegP>( "4" );
  addPhReg<RV_RegP>( "5" );
  addPhReg<RV_RegP>( "6" );
  addPhReg<RV_RegP>( "7" );
  addPhReg<RV_RegP>( "8" );
  addPhReg<RV_RegP>( "9" );
  addPhReg<RV_RegP>( "10" );
  addPhReg<RV_RegP>( "11" );
  addPhReg<RV_RegP>( "12" );
  addPhReg<RV_RegP>( "13" );
  addPhReg<RV_RegP>( "14" );
  addPhReg<RV_RegP>( "15" );
  addPhReg<RV_RegP>( "16" );
  addPhReg<RV_RegP>( "17" );
  addPhReg<RV_RegP>( "18" );
  addPhReg<RV_RegP>( "19" );
  addPhReg<RV_RegP>( "20" );
  addPhReg<RV_RegP>( "21" );
  addPhReg<RV_RegP>( "22" );
  addPhReg<RV_RegP>( "23" );
  addPhReg<RV_RegP>( "24" );
  addPhReg<RV_RegP>( "25" );
  addPhReg<RV_RegP>( "26" );
  addPhReg<RV_RegP>( "27" );
  addPhReg<RV_RegP>( "28" );
  addPhReg<RV_RegP>( "29" );
  addPhReg<RV_RegP>( "30" );
  addPhReg<RV_RegP>( "31" );

  addPhReg<RV_ERegP>( "0" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x0(), x1() );

  addPhReg<RV_ERegP>( "2" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x2(), x3() );

  addPhReg<RV_ERegP>( "4" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x4(), x5() );

  addPhReg<RV_ERegP>( "6" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x6(), x7() );

  addPhReg<RV_ERegP>( "8" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x8(), x9() );

  addPhReg<RV_ERegP>( "10" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x10(), x11() );

  addPhReg<RV_ERegP>( "12" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x12(), x13() );

  addPhReg<RV_ERegP>( "14" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x14(), x15() );

  addPhReg<RV_ERegP>( "16" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x16(), x17() );

  addPhReg<RV_ERegP>( "18" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x18(), x19() );

  addPhReg<RV_ERegP>( "20" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x20(), x21() );

  addPhReg<RV_ERegP>( "22" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x22(), x23() );

  addPhReg<RV_ERegP>( "24" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x24(), x25() );

  addPhReg<RV_ERegP>( "26" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x26(), x27() );

  addPhReg<RV_ERegP>( "28" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x28(), x29() );

  addPhReg<RV_ERegP>( "30" );
  dynamic_cast<RV_ERegP &>(
    mPhRegReferences.back().get() ).addChilds( x30(), x31() );
};


/*
  Copy constructor.
*/
RV32I::RV32I( const RV32I &__o ) :
  WIR_Processor<RV32I> { __o }
{
  DSTART( "RV32I::RV32I(const RV32I&)"  );
};


/*
  Move constructor.
*/
RV32I::RV32I( RV32I &&__o ) :
  WIR_Processor<RV32I> { std::move( __o ) }
{
  DSTART( "RV32I::RV32I(RV32I&&)" );
};


/*
  Destructor.
*/
RV32I::~RV32I( void )
{
  DSTART( "RV32I::~RV32I()" );
};


/*
  Copy-assignment operator.
*/
RV32I & RV32I::operator = ( const RV32I &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_Processor<RV32I>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV32I & RV32I::operator = ( RV32I &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_Processor<RV32I>::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for RV32I processor
  architectures.

  This includes setting up the assignment of valid operation formats to RV32I
  opcodes.

  init shall be called globally by WIR_Init(). It shall only perform tasks that
  cannot be expressed as initializations of static class members (since the
  order of static initialization is unspecified in C++) and that thus require
  execution by active code.
*/
void RV32I::init( void )
{
  DSTART( "static void RV32I::init()" );

  //
  // Register RV32I I/O functions.
  //

  registerBasicBlockDumper( getProcessorTypeID(), dumpRVBasicBlock );
  registerCompilationUnitDumper( getProcessorTypeID(), dumpRVCompilationUnit );
  registerDataDumper( getProcessorTypeID(), dumpRVData );
  registerDataSectionDumper( getProcessorTypeID(), dumpRVDataSection );
  registerFunctionDumper( getProcessorTypeID(), dumpRVFunction );
  registerLdScriptDumper( getProcessorTypeID(), dumpRVLdScript );
  registerLdScriptSectionDumper( getProcessorTypeID(), dumpRVLdScriptSection );
  registerLdScriptBetweenSectionDumper(
    getProcessorTypeID(), dumpRVLDScriptBetweenSections );
  registerOperationDumper( getProcessorTypeID(), dumpRVOperation );
  registerRegisterParameterDumper(
    getProcessorTypeID(), dumpRVRegisterParameter );
  registerCommentDumper( getProcessorTypeID(), dumpRVComment );
  registerFileInfoDumper( getProcessorTypeID(), dumpRVFileInfo );


  //
  // RV32I operation formats.
  //

  WIR_BasicBlock b;
  const RV_RegV *regV = new RV_RegV;

  registerOperationFormat(
    OperationFormat::L_1,
    { new WIR_LabelParameter( b ) } );

  registerOperationFormat(
    OperationFormat::NULL_1, {} );

  registerOperationFormat(
    OperationFormat::R_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RC12R_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new RV_Const12_Signed( 0 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RC12R_2,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new RV_Const12_Signed( 0 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RC20_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new RV_Const20_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::RL_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_LabelParameter( b ) } );

  registerOperationFormat(
    OperationFormat::RL_2,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_LabelParameter( b ) } );

  registerOperationFormat(
    OperationFormat::RLR_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_LabelParameter( b ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RLR_2,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_LabelParameter( b ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RRC5_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new RV_Const5_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::RRC12_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new RV_Const12_Signed( 0 ) } );

  registerOperationFormat(
    OperationFormat::RRL_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  registerOperationFormat(
    OperationFormat::RRL_2,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  registerOperationFormat(
    OperationFormat::RR_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RRR_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RSC5_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_StringParameter( "frm" ),
      new RV_Const5_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::RSR_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_StringParameter( "frm" ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );


  //
  // RV32I opcode to operation format mapping.
  //

  registerOpCode( OpCode::ADD, OperationFormat::RRR_1 );
  registerOpCode( OpCode::ADDI, OperationFormat::RRC12_1 );
  registerOpCode( OpCode::ADDI, OperationFormat::RRL_2 );
  registerOpCode( OpCode::AND, OperationFormat::RRR_1 );
  registerOpCode( OpCode::ANDI, OperationFormat::RRC12_1 );
  registerOpCode( OpCode::ANDI, OperationFormat::RRL_2 );
  registerOpCode( OpCode::AUIPC, OperationFormat::RC20_1 );
  registerOpCode( OpCode::AUIPC, OperationFormat::RL_2 );
  registerOpCode( OpCode::BEQ, OperationFormat::RRL_1 );
  registerOpCode( OpCode::BGE, OperationFormat::RRL_1 );
  registerOpCode( OpCode::BGEU, OperationFormat::RRL_1 );
  registerOpCode( OpCode::BLT, OperationFormat::RRL_1 );
  registerOpCode( OpCode::BLTU, OperationFormat::RRL_1 );
  registerOpCode( OpCode::BNE, OperationFormat::RRL_1 );
  registerOpCode( OpCode::CSRRC, OperationFormat::RSR_1 );
  registerOpCode( OpCode::CSRRCI, OperationFormat::RSC5_1 );
  registerOpCode( OpCode::CSRRS, OperationFormat::RSR_1 );
  registerOpCode( OpCode::CSRRSI, OperationFormat::RSC5_1 );
  registerOpCode( OpCode::CSRRW, OperationFormat::RSR_1 );
  registerOpCode( OpCode::CSRRWI, OperationFormat::RSC5_1 );
  registerOpCode( OpCode::EBREAK, OperationFormat::NULL_1 );
  registerOpCode( OpCode::ECALL, OperationFormat::NULL_1 );
  registerOpCode( OpCode::J, OperationFormat::L_1 );
  registerOpCode( OpCode::JAL, OperationFormat::RL_1 );
  registerOpCode( OpCode::JALR, OperationFormat::RC12R_1 );
  registerOpCode( OpCode::JALR, OperationFormat::RLR_1 );
  registerOpCode( OpCode::JALR, OperationFormat::RRC12_1 );
  registerOpCode( OpCode::JALR, OperationFormat::RRL_2 );
  registerOpCode( OpCode::JR, OperationFormat::R_1 );
  registerOpCode( OpCode::LB, OperationFormat::RC12R_1 );
  registerOpCode( OpCode::LB, OperationFormat::RLR_1 );
  registerOpCode( OpCode::LBU, OperationFormat::RC12R_1 );
  registerOpCode( OpCode::LBU, OperationFormat::RLR_1 );
  registerOpCode( OpCode::LH, OperationFormat::RC12R_1 );
  registerOpCode( OpCode::LH, OperationFormat::RLR_1 );
  registerOpCode( OpCode::LHU, OperationFormat::RC12R_1 );
  registerOpCode( OpCode::LHU, OperationFormat::RLR_1 );
  registerOpCode( OpCode::LUI, OperationFormat::RC20_1 );
  registerOpCode( OpCode::LUI, OperationFormat::RL_2 );
  registerOpCode( OpCode::LW, OperationFormat::RC12R_1 );
  registerOpCode( OpCode::LW, OperationFormat::RLR_1 );
  registerOpCode( OpCode::MOV, OperationFormat::RR_1 );
  registerOpCode( OpCode::OR, OperationFormat::RRR_1 );
  registerOpCode( OpCode::ORI, OperationFormat::RRC12_1 );
  registerOpCode( OpCode::ORI, OperationFormat::RRL_2 );
  registerOpCode( OpCode::RET, OperationFormat::NULL_1 );
  registerOpCode( OpCode::SB, OperationFormat::RC12R_2 );
  registerOpCode( OpCode::SB, OperationFormat::RLR_2 );
  registerOpCode( OpCode::SH, OperationFormat::RC12R_2 );
  registerOpCode( OpCode::SH, OperationFormat::RLR_2 );
  registerOpCode( OpCode::SLL, OperationFormat::RRR_1 );
  registerOpCode( OpCode::SLLI, OperationFormat::RRC5_1 );
  registerOpCode( OpCode::SLT, OperationFormat::RRR_1 );
  registerOpCode( OpCode::SLTI, OperationFormat::RRC12_1 );
  registerOpCode( OpCode::SLTI, OperationFormat::RRL_2 );
  registerOpCode( OpCode::SLTIU, OperationFormat::RRC12_1 );
  registerOpCode( OpCode::SLTIU, OperationFormat::RRL_2 );
  registerOpCode( OpCode::SLTU, OperationFormat::RRR_1 );
  registerOpCode( OpCode::SRA, OperationFormat::RRR_1 );
  registerOpCode( OpCode::SRAI, OperationFormat::RRC5_1 );
  registerOpCode( OpCode::SRL, OperationFormat::RRR_1 );
  registerOpCode( OpCode::SRLI, OperationFormat::RRC5_1 );
  registerOpCode( OpCode::SUB, OperationFormat::RRR_1 );
  registerOpCode( OpCode::SW, OperationFormat::RC12R_2 );
  registerOpCode( OpCode::SW, OperationFormat::RLR_2 );
  registerOpCode( OpCode::XOR, OperationFormat::RRR_1 );
  registerOpCode( OpCode::XORI, OperationFormat::RRC12_1 );
  registerOpCode( OpCode::XORI, OperationFormat::RRL_2 );


  //
  // Register this current processor model.
  //

  registerProcessor( RV32I() );


  //
  // Finally, initialize derived processor models.
  //

  RV32IM::init();
  RV32IC::init();
  RV32IF::init();
  RV32IFC::init();
  RV32IFD::init();
  RV32IMC::init();
  RV32IMF::init();
  RV32IFDC::init();
  RV32IMFC::init();
  RV32IMFD::init();
  RV32IMFDC::init();
};


/*
  Access to physical general-purpose register x0.
*/
const RV_RegP &RV32I::x0( void ) const
{
  DSTART( "const RV_RegP& RV32I::x0() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 0 ).get() ) );
};


/*
  Access to physical general-purpose register x1.
*/
const RV_RegP &RV32I::x1( void ) const
{
  DSTART( "const RV_RegP& RV32I::x1() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 1 ).get() ) );
};


/*
  Access to physical general-purpose register x2.
*/
const RV_RegP &RV32I::x2( void ) const
{
  DSTART( "const RV_RegP& RV32I::x2() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 2 ).get() ) );
};


/*
  Access to physical general-purpose register x3.
*/
const RV_RegP &RV32I::x3( void ) const
{
  DSTART( "const RV_RegP& RV32I::x3() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 3 ).get() ) );
};


/*
  Access to physical general-purpose register x4.
*/
const RV_RegP &RV32I::x4( void ) const
{
  DSTART( "const RV_RegP& RV32I::x4() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 4 ).get() ) );
};


/*
  Access to physical general-purpose register x5.
*/
const RV_RegP &RV32I::x5( void ) const
{
  DSTART( "const RV_RegP& RV32I::x5() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 5 ).get() ) );
};


/*
  Access to physical general-purpose register x6.
*/
const RV_RegP &RV32I::x6( void ) const
{
  DSTART( "const RV_RegP& RV32I::x6() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 6 ).get() ) );
};


/*
  Access to physical general-purpose register x7.
*/
const RV_RegP &RV32I::x7( void ) const
{
  DSTART( "const RV_RegP& RV32I::x7() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 7 ).get() ) );
};


/*
  Access to physical general-purpose register x8.
*/
const RV_RegP &RV32I::x8( void ) const
{
  DSTART( "const RV_RegP& RV32I::x8() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 8 ).get() ) );
};


/*
  Access to physical general-purpose register x9.
*/
const RV_RegP &RV32I::x9( void ) const
{
  DSTART( "const RV_RegP& RV32I::x9() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 9 ).get() ) );
};


/*
  Access to physical general-purpose register x10.
*/
const RV_RegP &RV32I::x10( void ) const
{
  DSTART( "const RV_RegP& RV32I::x10() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 10 ).get() ) );
};


/*
  Access to physical general-purpose register x11.
*/
const RV_RegP &RV32I::x11( void ) const
{
  DSTART( "const RV_RegP& RV32I::x11() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 11 ).get() ) );
};


/*
  Access to physical general-purpose register x12.
*/
const RV_RegP &RV32I::x12( void ) const
{
  DSTART( "const RV_RegP& RV32I::x12() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 12 ).get() ) );
};


/*
  Access to physical general-purpose register x13.
*/
const RV_RegP &RV32I::x13( void ) const
{
  DSTART( "const RV_RegP& RV32I::x13() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 13 ).get() ) );
};


/*
  Access to physical general-purpose register x14.
*/
const RV_RegP &RV32I::x14( void ) const
{
  DSTART( "const RV_RegP& RV32I::x14() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 14 ).get() ) );
};


/*
  Access to physical general-purpose register x15.
*/
const RV_RegP &RV32I::x15( void ) const
{
  DSTART( "const RV_RegP& RV32I::x15() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 15 ).get() ) );
};


/*
  Access to physical general-purpose register x16.
*/
const RV_RegP &RV32I::x16( void ) const
{
  DSTART( "const RV_RegP& RV32I::x16() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 16 ).get() ) );
};


/*
  Access to physical general-purpose register x17.
*/
const RV_RegP &RV32I::x17( void ) const
{
  DSTART( "const RV_RegP& RV32I::x17() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 17 ).get() ) );
};


/*
  Access to physical general-purpose register x18.
*/
const RV_RegP &RV32I::x18( void ) const
{
  DSTART( "const RV_RegP& RV32I::x18() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 18 ).get() ) );
};


/*
  Access to physical general-purpose register x19.
*/
const RV_RegP &RV32I::x19( void ) const
{
  DSTART( "const RV_RegP& RV32I::x19() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 19 ).get() ) );
};


/*
  Access to physical general-purpose register x20.
*/
const RV_RegP &RV32I::x20( void ) const
{
  DSTART( "const RV_RegP& RV32I::x20() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 20 ).get() ) );
};

/*
  Access to physical general-purpose register x21.
*/
const RV_RegP &RV32I::x21( void ) const
{
  DSTART( "const RV_RegP& RV32I::x21() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 21 ).get() ) );
};


/*
  Access to physical general-purpose register x22.
*/
const RV_RegP &RV32I::x22( void ) const
{
  DSTART( "const RV_RegP& RV32I::x22() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 22 ).get() ) );
};


/*
  Access to physical general-purpose register x23.
*/
const RV_RegP &RV32I::x23( void ) const
{
  DSTART( "const RV_RegP& RV32I::x23() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 23 ).get() ) );
};


/*
  Access to physical general-purpose register x24.
*/
const RV_RegP &RV32I::x24( void ) const
{
  DSTART( "const RV_RegP& RV32I::x24() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 24 ).get() ) );
};


/*
  Access to physical general-purpose register x25.
*/
const RV_RegP &RV32I::x25( void ) const
{
  DSTART( "const RV_RegP& RV32I::x25() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 25 ).get() ) );
};


/*
  Access to physical general-purpose register x26.
*/
const RV_RegP &RV32I::x26( void ) const
{
  DSTART( "const RV_RegP& RV32I::x26() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 26 ).get() ) );
};


/*
  Access to physical general-purpose register x27.
*/
const RV_RegP &RV32I::x27( void ) const
{
  DSTART( "const RV_RegP& RV32I::x27() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 27 ).get() ) );
};


/*
  Access to physical general-purpose register x28.
*/
const RV_RegP &RV32I::x28( void ) const
{
  DSTART( "const RV_RegP& RV32I::x28() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 28 ).get() ) );
};


/*
  Access to physical general-purpose register x29.
*/
const RV_RegP &RV32I::x29( void ) const
{
  DSTART( "const RV_RegP& RV32I::x29() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 29 ).get() ) );
};


/*
  Access to physical general-purpose register x30.
*/
const RV_RegP &RV32I::x30( void ) const
{
  DSTART( "const RV_RegP& RV32I::x30() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 30 ).get() ) );
};


/*
  Access to physical general-purpose register x31.
*/
const RV_RegP &RV32I::x31( void ) const
{
  DSTART( "const RV_RegP& RV32I::x31() const" );

  return( dynamic_cast<RV_RegP &>( mPhRegReferences.at( 31 ).get() ) );
};


/*
  Access to physical general-purpose register pair e0 (x0 and x1).
*/
const RV_ERegP &RV32I::e0( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e0() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 32 ).get() ) );
};


/*
  Access to physical general-purpose register pair e2 (x2 and x3).
*/
const RV_ERegP &RV32I::e2( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e2() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 33 ).get() ) );
};


/*
  Access to physical general-purpose register pair e4 (x4 and x5).
*/
const RV_ERegP &RV32I::e4( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e4() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 34 ).get() ) );
};


/*
  Access to physical general-purpose register pair e6 (x6 and x7).
*/
const RV_ERegP &RV32I::e6( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e6() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 35 ).get() ) );
};


/*
  Access to physical general-purpose register pair e8 (x8 and x9).
*/
const RV_ERegP &RV32I::e8( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e8() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 36 ).get() ) );
};


/*
  Access to physical general-purpose register pair e10 (x10 and x11).
*/
const RV_ERegP &RV32I::e10( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e10() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 37 ).get() ) );
};


/*
  Access to physical general-purpose register pair e12 (x12 and x13).
*/
const RV_ERegP &RV32I::e12( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e12() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 38 ).get() ) );
};


/*
  Access to physical general-purpose register pair e14 (x14 and x15).
*/
const RV_ERegP &RV32I::e14( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e14() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 39 ).get() ) );
};


/*
  Access to physical general-purpose register pair e16 (x16 and x17).
*/
const RV_ERegP &RV32I::e16( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e16() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 40 ).get() ) );
};


/*
  Access to physical general-purpose register pair e18 (x18 and x19).
*/
const RV_ERegP &RV32I::e18( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e18() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 41 ).get() ) );
};


/*
  Access to physical general-purpose register pair e20 (x20 and x21).
*/
const RV_ERegP &RV32I::e20( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e20() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 42 ).get() ) );
};


/*
  Access to physical general-purpose register pair e22 (x22 and x23).
*/
const RV_ERegP &RV32I::e22( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e22() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 43 ).get() ) );
};


/*
  Access to physical general-purpose register pair e24 (x24 and x25).
*/
const RV_ERegP &RV32I::e24( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e24() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 44 ).get() ) );
};


/*
  Access to physical general-purpose register pair e26 (x26 and x27).
*/
const RV_ERegP &RV32I::e26( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e26() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 45 ).get() ) );
};


/*
  Access to physical general-purpose register pair e28 (x28 and x29).
*/
const RV_ERegP &RV32I::e28( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e28() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 46 ).get() ) );
};


/*
  Access to physical general-purpose register pair e30 (x30 and x31).
*/
const RV_ERegP &RV32I::e30( void ) const
{
  DSTART( "const RV_ERegP& RV32I::e30() const" );

  return( dynamic_cast<RV_ERegP &>( mPhRegReferences.at( 47 ).get() ) );
};


/*
  Access to return address pointer.
*/
const RV_RegP &RV32I::RA( void ) const
{
  DSTART( "const RV_RegP& RV32I::RA() const" );

  return( x1() );
};


/*
  Access to stack pointer.
*/
const RV_RegP &RV32I::SP( void ) const
{
  DSTART( "const RV_RegP& RV32I::SP() const" );

  return( x2() );
};


/*
  isX0 checks whether the specified register is the RISC-V's x0 register which
  is always zero.
*/
bool RV32I::isX0( const WIR_BaseRegister &r )
{
  DSTART( "static bool RV32I::isX0(const WIR_BaseRegister&)" );

  if ( r.isVirtual() ) {
    auto &vr = dynamic_cast<const WIR_VirtualRegister &>( r );

    if ( vr.isPrecolored() && ( vr.getPrecolor().getName() == "x0" ) )
      return( true );
  } else
    return( r.getName() == "x0" );

  return( false );
};


/*
  isX1 checks whether the specified register is the RISC-V's x1 register which
  is the link register.
*/
bool RV32I::isX1( const WIR_BaseRegister &r )
{
  DSTART( "static bool RV32I::isX1(const WIR_BaseRegister&)" );

  if ( r.isVirtual() ) {
    auto &vr = dynamic_cast<const WIR_VirtualRegister &>( r );

    if ( vr.isPrecolored() && ( vr.getPrecolor().getName() == "x1" ) )
      return( true );
  } else
    return( r.getName() == "x1" );

  return( false );
};


/*
  isFP checks whether the specified register is the RISC-V's frame pointer.
*/
bool RV32I::isFP( const WIR_BaseRegister &r )
{
  DSTART( "static bool RV32I::isFP(const WIR_BaseRegister&)" );

  if ( r.isVirtual() ) {
    auto &vr = dynamic_cast<const WIR_VirtualRegister &>( r );

    if ( vr.isPrecolored() && ( vr.getPrecolor().getName() == "x8" ) )
      return( true );
  } else
    return( r.getName() == "x8" );

  return( false );
};


/*
  isSP checks whether the specified register is the RISC-V's stack pointer.
*/
bool RV32I::isSP( const WIR_BaseRegister &r )
{
  DSTART( "static bool RV32I::isSP(const WIR_BaseRegister&)" );

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


//
// Private class methods
//

/*
  clone creates a copy of an RV32I processor.
*/
WIR_BaseProcessor *RV32I::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* RV32I::clone()" );

  return( new RV32I( *this ) );
};

}       // namespace WIR
