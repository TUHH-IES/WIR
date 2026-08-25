/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32ic.cc
  @brief This file implements the specific interface of the RISC-V RV32IC Base
         Integer instruction set plus the C Standard Extension for Compressed
         Instructions, version 2.0.

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
#include <arch/riscv/rv32ic.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for RV32IC processor architectures.
*/
RV32IC::RV32IC( void ) :
  RV32I {}
{
  DSTART( "RV32IC::RV32IC()" );

  // Specify the processor architecture modeled by this class.
  setISAName( "RV32IC" );
};


/*
  Copy constructor.
*/
RV32IC::RV32IC( const RV32IC &__o ) :
  RV32I { __o }
{
  DSTART( "RV32IC::RV32IC(const RV32IC&)" );
};


/*
  Move constructor.
*/
RV32IC::RV32IC( RV32IC &&__o ) :
  RV32I { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
RV32IC::~RV32IC( void )
{
  DSTART( "virtual RV32IC::~RV32IC()" );
};


/*
  Copy-assignment operator.
*/
RV32IC & RV32IC::operator = ( const RV32IC &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32I::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV32IC & RV32IC::operator = ( RV32IC &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32I::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for RV32IC processor
  architectures.

  This includes setting up the assignment of valid operation formats to RV32IC
  opcodes.

  init shall be called globally by WIR_Init(). It shall only perform tasks that
  cannot be expressed as initializations of static class members (since the
  order of static initialization is unspecified in C++) and that thus require
  execution by active code.
*/
// cppcheck-suppress duplInheritedMember
void RV32IC::init( void )
{
  DSTART( "static void RV32IC::init()" );

  //
  // RV32IC operation formats.
  //

  WIR_BasicBlock b;
  const RV_RegV *regV = new RV_RegV;
  const RV_RegP *reg2P = new RV_RegP( "2" );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SL_1,
    { new WIR_LabelParameter( b ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SNULL_1, {} );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SR_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SRC5_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new RV_Const5_Unsigned( 0 ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SRC6_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new RV_Const6_Signed( 0 ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SRC6_2,
    { new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new RV_Const6_Signed( 0 ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SRC7R_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new RV_Const7_Unsigned( 0 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SRC7R_2,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new RV_Const7_Unsigned( 0 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SRC8R_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new RV_Const8_Unsigned( 0 ),
      new WIR_RegisterParameter( *reg2P, WIR_Usage::use ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SRC8R_2,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new RV_Const8_Unsigned( 0 ),
      new WIR_RegisterParameter( *reg2P, WIR_Usage::use ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SRC10_1,
    { new WIR_RegisterParameter( *reg2P, WIR_Usage::defuse ),
      new RV_Const10_Signed( 16 ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SRL_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SRR_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SRR_2,
    { new WIR_RegisterParameter( *regV, WIR_Usage::defuse ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SRRC10_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *reg2P, WIR_Usage::use ),
      new RV_Const10_Unsigned( 1020 ) } );

  WIR_Registry::registerOperationFormat(
    OperationFormat::SRC20_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new RV_NZConst20_Unsigned( 0xFFFE0 ) } );


  //
  // RV32IC opcode to operation format mapping.
  //

  WIR_Registry::registerOpCode( OpCode::CADD, OperationFormat::SRR_2 );
  WIR_Registry::registerOpCode( OpCode::CADDI, OperationFormat::SRC6_2 );
  WIR_Registry::registerOpCode( OpCode::CADDI16SP, OperationFormat::SRC10_1 );
  WIR_Registry::registerOpCode( OpCode::CADDI4SPN, OperationFormat::SRRC10_1 );
  WIR_Registry::registerOpCode( OpCode::CAND, OperationFormat::SRR_2 );
  WIR_Registry::registerOpCode( OpCode::CANDI, OperationFormat::SRC6_2 );
  WIR_Registry::registerOpCode( OpCode::CBEQZ, OperationFormat::SRL_1 );
  WIR_Registry::registerOpCode( OpCode::CBNEZ, OperationFormat::SRL_1 );
  WIR_Registry::registerOpCode( OpCode::CEBREAK, OperationFormat::SNULL_1 );
  WIR_Registry::registerOpCode( OpCode::CJ, OperationFormat::SL_1 );
  WIR_Registry::registerOpCode( OpCode::CJAL, OperationFormat::SL_1 );
  WIR_Registry::registerOpCode( OpCode::CJALR, OperationFormat::SR_1 );
  WIR_Registry::registerOpCode( OpCode::CJR, OperationFormat::SR_1 );
  WIR_Registry::registerOpCode( OpCode::CLI, OperationFormat::SRC6_1 );
  WIR_Registry::registerOpCode( OpCode::CLUI, OperationFormat::SRC20_1 );
  WIR_Registry::registerOpCode( OpCode::CLW, OperationFormat::SRC7R_1 );
  WIR_Registry::registerOpCode( OpCode::CLWSP, OperationFormat::SRC8R_1 );
  WIR_Registry::registerOpCode( OpCode::CMV, OperationFormat::SRR_1 );
  WIR_Registry::registerOpCode( OpCode::CNOP, OperationFormat::SNULL_1);
  WIR_Registry::registerOpCode( OpCode::COR, OperationFormat::SRR_2 );
  WIR_Registry::registerOpCode( OpCode::CSLLI, OperationFormat::SRC5_1 );
  WIR_Registry::registerOpCode( OpCode::CSRAI, OperationFormat::SRC5_1 );
  WIR_Registry::registerOpCode( OpCode::CSRLI, OperationFormat::SRC5_1 );
  WIR_Registry::registerOpCode( OpCode::CSUB, OperationFormat::SRR_2 );
  WIR_Registry::registerOpCode( OpCode::CSW, OperationFormat::SRC7R_2 );
  WIR_Registry::registerOpCode( OpCode::CSWSP, OperationFormat::SRC8R_2 );
  WIR_Registry::registerOpCode( OpCode::CXOR, OperationFormat::SRR_2 );


  //
  // Register this current processor model.
  //

  WIR_Registry::registerProcessor( RV32IC() );
};


/*
  isPopularRegister checks whether the specified register is in the range of the
  RV32IC popular registers (x8 - x15).

  If an uncolored virtual register r is passed to isPopularRegister, this method
  adds interferences between r and all non-popular registers in order to
  maximally exploit compact RV32IC operations.
*/
bool RV32IC::isPopularRegister( const WIR_BaseRegister &r )
{
  DSTART( "bool RV32IC::isPopularRegister(const WIR_BaseRegister&)" );

  if ( r.isVirtual() ) {
    auto &vr = dynamic_cast<const WIR_VirtualRegister &>( r );

    if ( vr.isPrecolored() &&
         ( ( vr.getPrecolor().getName() == "x8" ) ||
           ( vr.getPrecolor().getName() == "x9" ) ||
           ( vr.getPrecolor().getName() == "x10" ) ||
           ( vr.getPrecolor().getName() == "x11" ) ||
           ( vr.getPrecolor().getName() == "x12" ) ||
           ( vr.getPrecolor().getName() == "x13" ) ||
           ( vr.getPrecolor().getName() == "x14" ) ||
           ( vr.getPrecolor().getName() == "x15" ) ) )
      return( true );
    else

    if ( !vr.isPrecolored() ) {
      // Add interferences with all other non-popular registers.
      WIR_Function &f = vr.getFunction();
      const auto &sys = f.getCompilationUnit().getSystem();
      const auto &rv =
        dynamic_cast<RV32IC &>(
          sys.findSymbol( f ).getSection().getProcessor() );

      f.insertInterference( vr, rv.x0() );
      f.insertInterference( vr, rv.x1() );
      f.insertInterference( vr, rv.x2() );
      f.insertInterference( vr, rv.x3() );
      f.insertInterference( vr, rv.x4() );
      f.insertInterference( vr, rv.x5() );
      f.insertInterference( vr, rv.x6() );
      f.insertInterference( vr, rv.x7() );
      f.insertInterference( vr, rv.x16() );
      f.insertInterference( vr, rv.x17() );
      f.insertInterference( vr, rv.x18() );
      f.insertInterference( vr, rv.x19() );
      f.insertInterference( vr, rv.x20() );
      f.insertInterference( vr, rv.x21() );
      f.insertInterference( vr, rv.x22() );
      f.insertInterference( vr, rv.x23() );
      f.insertInterference( vr, rv.x24() );
      f.insertInterference( vr, rv.x25() );
      f.insertInterference( vr, rv.x26() );
      f.insertInterference( vr, rv.x27() );
      f.insertInterference( vr, rv.x28() );
      f.insertInterference( vr, rv.x29() );
      f.insertInterference( vr, rv.x30() );
      f.insertInterference( vr, rv.x31() );

      return( true );
    }
  } else
    return(
      ( r.getName() == "x8" ) || ( r.getName() == "x9" ) ||
      ( r.getName() == "x10" ) || ( r.getName() == "x11" ) ||
      ( r.getName() == "x12" ) || ( r.getName() == "x13" ) ||
      ( r.getName() == "x14" ) || ( r.getName() == "x15" ) );

  return( false );
};


/*
  isStackPointerADDI checks wether the given operation is an ADDI or C.ADDI16SP
  operation that modifies the stack pointer.
*/
bool RV32IC::isStackPointerADDI( const WIR_Operation &o )
{
  DSTART( "static bool RV32IC::isStackPointerADDI(const WIR_Operation&)" );

  return(
    ( ( o.getOpCode() == RV32I::OpCode::ADDI ) &&
      ( o.getOperationFormat() == RV32I::OperationFormat::RRC12_1 ) &&
      isSP(
        dynamic_cast<WIR_RegisterParameter &>(
          o.getExplicitParameter( 1 ) ).getRegister() ) ) ||
    ( o.getOpCode() == RV32IC::OpCode::CADDI16SP ) );
};


/*
  isStackPointerLUIADDIADD checks whether the given operations are the following
  sequence of stack-allocating code:
    LUI  <reg>, const20
    ADDI <reg>, <reg>, const12
    ADD  sp, <reg>, sp
*/
bool RV32IC::isStackPointerLUIADDIADD( const WIR_Operation &o1,
                                       const WIR_Operation &o2,
                                       const WIR_Operation &o3 )
{
  DSTART(
    "static bool RV32IC::isStackPointerLUIADDIADD(const WIR_Operation&, const "
    "WIR_Operation&, const WIR_Operation&)" );

  if ( // o1 is a suitable LUI.
       ( o1.getOpCode() == RV32I::OpCode::LUI ) &&
       ( o1.getOperationFormat() == RV32I::OperationFormat::RC20_1 ) &&

       // o2 is a suitable ADDI.
       ( o2.getOpCode() == RV32I::OpCode::ADDI ) &&
       ( o2.getOperationFormat() == RV32I::OperationFormat::RRC12_1 ) &&

       // o1 and o2 use the same register.
       ( dynamic_cast<WIR_RegisterParameter &>(
           o1.getExplicitParameter( 1 ) ).getRegister() ==
         dynamic_cast<WIR_RegisterParameter &>(
           o2.getExplicitParameter( 1 ) ).getRegister() ) &&
       ( dynamic_cast<WIR_RegisterParameter &>(
           o1.getExplicitParameter( 1 ) ).getRegister() ==
         dynamic_cast<WIR_RegisterParameter &>(
           o2.getExplicitParameter( 2 ) ).getRegister() ) &&

       // o3 is an ADD defining the stack pointer.
       ( o3.getOpCode() == RV32I::OpCode::ADD ) &&
       isSP(
         dynamic_cast<WIR_RegisterParameter &>(
           o3.getExplicitParameter( 1 ) ).getRegister() ) &&

       // o3 and o1 use the same register.
       ( ( isSP(
             dynamic_cast<WIR_RegisterParameter &>(
               o3.getExplicitParameter( 2 ) ).getRegister() ) &&
           ( dynamic_cast<WIR_RegisterParameter &>(
               o1.getExplicitParameter( 1 ) ).getRegister() ==
             dynamic_cast<WIR_RegisterParameter &>(
               o3.getExplicitParameter( 3 ) ).getRegister() ) ) ||
         ( isSP(
             dynamic_cast<WIR_RegisterParameter &>(
               o3.getExplicitParameter( 3 ) ).getRegister() ) &&
           ( dynamic_cast<WIR_RegisterParameter &>(
               o1.getExplicitParameter( 1 ) ).getRegister() ==
             dynamic_cast<WIR_RegisterParameter &>(
               o3.getExplicitParameter( 2 ) ).getRegister() ) ) ) )
    return( true );

  return( false );
};


//
// Private class methods
//

/*
  clone creates a copy of an RV32IC processor.
*/
WIR_BaseProcessor *RV32IC::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* RV32IC::clone() const" );

  return( new RV32IC( *this ) );
};

}       // namespace WIR
