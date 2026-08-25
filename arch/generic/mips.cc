/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file mips.cc
  @brief This file implements the specific interface of the MIPS/SPIM
         architecture.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
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
#include <arch/generic/mips.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for MIPS/SPIM processor architectures.
*/
MIPS::MIPS( void ) :
  WIR_Processor<MIPS> {}
{
  DSTART( "MIPS::MIPS()" );

  // Specify the processor architecture modeled by this class.
  setProcessorName( "MIPS/SPIM" );
  setISAName( "WIR generic" );

  // Create physical registers bottom-up, i.e., from the leaf registers upwards
  // towards complex hierarchical registers.
  // For the meaning of the individual registers, please refer to file
  // doc/MIPS_Reference.pdf.
  addPhReg<MIPS_RegP>( "0" );
  addPhReg<MIPS_RegP>( "1" );
  addPhReg<MIPS_RegP>( "2" );
  addPhReg<MIPS_RegP>( "3" );
  addPhReg<MIPS_RegP>( "4" );
  addPhReg<MIPS_RegP>( "5" );
  addPhReg<MIPS_RegP>( "6" );
  addPhReg<MIPS_RegP>( "7" );
  addPhReg<MIPS_RegP>( "8" );
  addPhReg<MIPS_RegP>( "9" );
  addPhReg<MIPS_RegP>( "10" );
  addPhReg<MIPS_RegP>( "11" );
  addPhReg<MIPS_RegP>( "12" );
  addPhReg<MIPS_RegP>( "13" );
  addPhReg<MIPS_RegP>( "14" );
  addPhReg<MIPS_RegP>( "15" );
  addPhReg<MIPS_RegP>( "16" );
  addPhReg<MIPS_RegP>( "17" );
  addPhReg<MIPS_RegP>( "18" );
  addPhReg<MIPS_RegP>( "19" );
  addPhReg<MIPS_RegP>( "20" );
  addPhReg<MIPS_RegP>( "21" );
  addPhReg<MIPS_RegP>( "22" );
  addPhReg<MIPS_RegP>( "23" );
  addPhReg<MIPS_RegP>( "24" );
  addPhReg<MIPS_RegP>( "25" );
  addPhReg<MIPS_RegP>( "26" );
  addPhReg<MIPS_RegP>( "27" );
  addPhReg<MIPS_RegP>( "28" );
  addPhReg<MIPS_RegP>( "29", true );
  addPhReg<MIPS_RegP>( "30" );
  addPhReg<MIPS_RegP>( "31" );
};


/*
  Copy constructor.
*/
MIPS::MIPS( const MIPS &__o ) :
  WIR_Processor<MIPS> { __o }
{
  DSTART( "MIPS::MIPS(const MIPS&)" );
};


/*
  Move constructor.
*/
MIPS::MIPS( MIPS &&__o ) :
  WIR_Processor<MIPS> { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
MIPS::~MIPS( void )
{
  DSTART( "virtual MIPS::~MIPS()" );
};


/*
  Copy-assignment operator.
*/
MIPS & MIPS::operator = ( const MIPS &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_Processor<MIPS>::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
MIPS & MIPS::operator = ( MIPS &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_Processor<MIPS>::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for MIPS processor
  architectures.

  This includes setting up the MIPS machine operation formats and the assignment
  of valid operation formats to MIPS opcodes.

  init shall be called globally by WIR_Init(). It shall only perform tasks that
  cannot be expressed as initializations of static class members (since the
  order of static initialization is unspecified in C++) and that thus require
  execution by active code.
*/
void MIPS::init( void )
{
  DSTART( "static void MIPS::init()" );

  //
  // Register MIPS/SPIM I/O functions.
  //

  registerOperationDumper(
    getProcessorTypeID(), dumpMIPSOperation );


  //
  // MIPS operation formats.
  //

  WIR_BasicBlock b;
  const MIPS_RegV *regV = new MIPS_RegV;

  registerOperationFormat(
    OperationFormat::L,
    { new WIR_LabelParameter( b ) } );

  registerOperationFormat(
    OperationFormat::R_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ) } );

  registerOperationFormat(
    OperationFormat::R_2,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RIR_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new MIPS_Immediate16_Signed( 0 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RIR_2,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new MIPS_Immediate16_Signed( 0 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RIU,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new MIPS_Immediate16_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::RIUR,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new MIPS_Immediate16_Unsigned( 0 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RR_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RR_2,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RRI,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new MIPS_Immediate16_Signed( 0 ) } );

  registerOperationFormat(
    OperationFormat::RRIU,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new MIPS_Immediate16_Unsigned( 0 ) } );

  registerOperationFormat(
    OperationFormat::RRL,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  registerOperationFormat(
    OperationFormat::RRR,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RRS,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new MIPS_Immediate5_Shamt( 0 ) } );


  //
  // MIPS opcode to operation format mapping.
  //

  registerOpCode( OpCode::ADD, OperationFormat::RRR );
  registerOpCode( OpCode::ADDI, OperationFormat::RRI );
  registerOpCode( OpCode::ADDIU, OperationFormat::RRIU );
  registerOpCode( OpCode::ADDU, OperationFormat::RRR );
  registerOpCode( OpCode::AND, OperationFormat::RRR );
  registerOpCode( OpCode::ANDI, OperationFormat::RRIU );
  registerOpCode( OpCode::BEQ, OperationFormat::RRL );
  registerOpCode( OpCode::BNE, OperationFormat::RRL );
  registerOpCode( OpCode::DIV, OperationFormat::RR_2 );
  registerOpCode( OpCode::DIVU, OperationFormat::RR_2 );
  registerOpCode( OpCode::J, OperationFormat::L );
  registerOpCode( OpCode::JAL, OperationFormat::L );
  registerOpCode( OpCode::JALR, OperationFormat::R_2 );
  registerOpCode( OpCode::JR, OperationFormat::R_2 );
  registerOpCode( OpCode::LB, OperationFormat::RIR_1 );
  registerOpCode( OpCode::LBU, OperationFormat::RIUR );
  registerOpCode( OpCode::LH, OperationFormat::RIR_1 );
  registerOpCode( OpCode::LHU, OperationFormat::RIUR );
  registerOpCode( OpCode::LUI, OperationFormat::RIU );
  registerOpCode( OpCode::LW, OperationFormat::RIR_1 );
  registerOpCode( OpCode::MFCO, OperationFormat::RR_1 );
  registerOpCode( OpCode::MFHI, OperationFormat::R_1 );
  registerOpCode( OpCode::MFLO, OperationFormat::R_1 );
  registerOpCode( OpCode::MULT, OperationFormat::RR_2 );
  registerOpCode( OpCode::MULTU, OperationFormat::RR_2 );
  registerOpCode( OpCode::NOR, OperationFormat::RRR );
  registerOpCode( OpCode::OR, OperationFormat::RRR );
  registerOpCode( OpCode::ORI, OperationFormat::RRIU );
  registerOpCode( OpCode::SB, OperationFormat::RIR_2 );
  registerOpCode( OpCode::SH, OperationFormat::RIR_2 );
  registerOpCode( OpCode::SLL, OperationFormat::RRS );
  registerOpCode( OpCode::SLLV, OperationFormat::RRR );
  registerOpCode( OpCode::SLT, OperationFormat::RRR );
  registerOpCode( OpCode::SLTI, OperationFormat::RRI );
  registerOpCode( OpCode::SLTIU, OperationFormat::RRIU );
  registerOpCode( OpCode::SLTU, OperationFormat::RRR );
  registerOpCode( OpCode::SRA, OperationFormat::RRS );
  registerOpCode( OpCode::SRAV, OperationFormat::RRR );
  registerOpCode( OpCode::SRL, OperationFormat::RRS );
  registerOpCode( OpCode::SRLV, OperationFormat::RRR );
  registerOpCode( OpCode::SUB, OperationFormat::RRR );
  registerOpCode( OpCode::SUBU, OperationFormat::RRR );
  registerOpCode( OpCode::SW, OperationFormat::RIR_2 );
  registerOpCode( OpCode::XOR, OperationFormat::RRR );
  registerOpCode( OpCode::XORI, OperationFormat::RRIU );


  //
  // Register this current processor model.
  //

  registerProcessor( MIPS() );
};


/*
  Access to physical integer register $0 (zero).
*/
const MIPS_RegP &MIPS::r0( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r0() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 0 ).get() ) );
};


/*
  Access to physical integer register $1 (assembler temporary).
*/
const MIPS_RegP &MIPS::r1( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r1() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 1 ).get() ) );
};


/*
  Access to physical integer register $2 (function result).
*/
const MIPS_RegP &MIPS::r2( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r2() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 2 ).get() ) );
};


/*
  Access to physical integer register $3 (function result).
*/
const MIPS_RegP &MIPS::r3( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r3() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 3 ).get() ) );
};


/*
  Access to physical integer register $4 (function argument).
*/
const MIPS_RegP &MIPS::r4( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r4() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 4 ).get() ) );
};


/*
  Access to physical integer register $5 (function argument).
*/
const MIPS_RegP &MIPS::r5( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r5() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 5 ).get() ) );
};


/*
  Access to physical integer register $6 (function argument).
*/
const MIPS_RegP &MIPS::r6( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r6() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 6 ).get() ) );
};


/*
  Access to physical integer register $7 (function argument).
*/
const MIPS_RegP &MIPS::r7( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r7() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 7 ).get() ) );
};


/*
  Access to physical integer register $8 (temporaries).
*/
const MIPS_RegP &MIPS::r8( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r8() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 8 ).get() ) );
};


/*
  Access to physical integer register $9 (temporaries).
*/
const MIPS_RegP &MIPS::r9( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r9() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 9 ).get() ) );
};


/*
  Access to physical integer register $10 (temporaries).
*/
const MIPS_RegP &MIPS::r10( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r10() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 10 ).get() ) );
};


/*
  Access to physical integer register $11 (temporaries).
*/
const MIPS_RegP &MIPS::r11( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r11() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 11 ).get() ) );
};


/*
  Access to physical integer register $12 (temporaries).
*/
const MIPS_RegP &MIPS::r12( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r12() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 12 ).get() ) );
};


/*
  Access to physical integer register $13 (temporaries).
*/
const MIPS_RegP &MIPS::r13( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r13() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 13 ).get() ) );
};


/*
  Access to physical integer register $14 (temporaries).
*/
const MIPS_RegP &MIPS::r14( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r14() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 14 ).get() ) );
};


/*
  Access to physical integer register $15 (temporaries).
*/
const MIPS_RegP &MIPS::r15( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r15() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 15 ).get() ) );
};


/*
  Access to physical integer register $16 (saved temporaries).
*/
const MIPS_RegP &MIPS::r16( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r16() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 16 ).get() ) );
};


/*
  Access to physical integer register $17 (saved temporaries).
*/
const MIPS_RegP &MIPS::r17( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r17() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 17 ).get() ) );
};


/*
  Access to physical integer register $18 (saved temporaries).
*/
const MIPS_RegP &MIPS::r18( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r18() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 18 ).get() ) );
};


/*
  Access to physical integer register $19 (saved temporaries).
*/
const MIPS_RegP &MIPS::r19( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r19() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 19 ).get() ) );
};


/*
  Access to physical integer register $20 (saved temporaries).
*/
const MIPS_RegP &MIPS::r20( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r20() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 20 ).get() ) );
};


/*
  Access to physical integer register $21 (saved temporaries).
*/
const MIPS_RegP &MIPS::r21( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r21() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 21 ).get() ) );
};


/*
  Access to physical integer register $22 (saved temporaries).
*/
const MIPS_RegP &MIPS::r22( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r22() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 22 ).get() ) );
};


/*
  Access to physical integer register $23 (saved temporaries).
*/
const MIPS_RegP &MIPS::r23( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r23() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 23 ).get() ) );
};


/*
  Access to physical integer register $24 (temporaries).
*/
const MIPS_RegP &MIPS::r24( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r24() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 24 ).get() ) );
};


/*
  Access to physical integer register $25 (temporaries).
*/
const MIPS_RegP &MIPS::r25( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r25() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 25 ).get() ) );
};


/*
  Access to physical integer register $26 (reserved OS kernel).
*/
const MIPS_RegP &MIPS::r26( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r26() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 26 ).get() ) );
};


/*
  Access to physical integer register $27 (reserved OS kernel).
*/
const MIPS_RegP &MIPS::r27( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r27() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 27 ).get() ) );
};


/*
  Access to physical integer register $28 (global pointer).
*/
const MIPS_RegP &MIPS::r28( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r28() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 28 ).get() ) );
};


/*
  Access to physical integer register $29 (stack pointer).
*/
const MIPS_RegP &MIPS::r29( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r29() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 29 ).get() ) );
};


/*
  Access to physical integer register $30 (frame pointer).
*/
const MIPS_RegP &MIPS::r30( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r30() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 30 ).get() ) );
};


/*
  Access to physical integer register $31 (return address).
*/
const MIPS_RegP &MIPS::r31( void ) const
{
  DSTART( "const MIPS_RegP& MIPS::r31() const" );

  return( dynamic_cast<MIPS_RegP &>( mPhRegReferences.at( 31 ).get() ) );
};

}       // namespace WIR
