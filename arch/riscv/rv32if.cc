/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32if.cc
  @brief This file implements the specific interface of the RISC-V RV32IF
         Floating Point instruction set, version 2.0.

  @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
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
#include <arch/riscv/rv32if.h>
#include <arch/riscv/rv32ifd.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for RV32IF processor architectures.
*/
RV32IF::RV32IF( void ) :
  RV32I {}
{
  DSTART( "RV32IF::RV32IF()" );

  // Specify the processor architecture modeled by this class.
  setISAName( "RV32IF V2.0" );

  // Set bit width of floating-point registers to 32.
  RV32IF::RegisterType::fReg.setBitWidth( 32 );

  // Create physical registers for all 32 floating-point registers.
  addPhReg<RV_FRegP>( "0" );
  addPhReg<RV_FRegP>( "1" );
  addPhReg<RV_FRegP>( "2" );
  addPhReg<RV_FRegP>( "3" );
  addPhReg<RV_FRegP>( "4" );
  addPhReg<RV_FRegP>( "5" );
  addPhReg<RV_FRegP>( "6" );
  addPhReg<RV_FRegP>( "7" );
  addPhReg<RV_FRegP>( "8" );
  addPhReg<RV_FRegP>( "9" );
  addPhReg<RV_FRegP>( "10" );
  addPhReg<RV_FRegP>( "11" );
  addPhReg<RV_FRegP>( "12" );
  addPhReg<RV_FRegP>( "13" );
  addPhReg<RV_FRegP>( "14" );
  addPhReg<RV_FRegP>( "15" );
  addPhReg<RV_FRegP>( "16" );
  addPhReg<RV_FRegP>( "17" );
  addPhReg<RV_FRegP>( "18" );
  addPhReg<RV_FRegP>( "19" );
  addPhReg<RV_FRegP>( "20" );
  addPhReg<RV_FRegP>( "21" );
  addPhReg<RV_FRegP>( "22" );
  addPhReg<RV_FRegP>( "23" );
  addPhReg<RV_FRegP>( "24" );
  addPhReg<RV_FRegP>( "25" );
  addPhReg<RV_FRegP>( "26" );
  addPhReg<RV_FRegP>( "27" );
  addPhReg<RV_FRegP>( "28" );
  addPhReg<RV_FRegP>( "29" );
  addPhReg<RV_FRegP>( "30" );
  addPhReg<RV_FRegP>( "31" );
};


/*
  Copy constructor.
*/
RV32IF::RV32IF( const RV32IF &__o ) :
  RV32I { __o }
{
  DSTART( "RV32IF::RV32IF(const RV32IF&)" );
};


/*
  Move constructor.
*/
RV32IF::RV32IF( RV32IF &&__o ) :
  RV32I { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
RV32IF::~RV32IF( void )
{
  DSTART( "virtual RV32IF::~RV32IF()" );
};


/*
  Copy-assignment operator.
*/
RV32IF & RV32IF::operator = ( const RV32IF &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32I::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV32IF & RV32IF::operator = ( RV32IF &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32I::operator = ( std::move( __o ) );

  return( *this );
};

/*
  init performs some global initialization tasks for RV32IF processor
  architectures.

  This includes setting up the assignment of valid operation formats to RV32IF
  opcodes.

  init shall be called globally by WIR_Init(). It shall only perform tasks that
  cannot be expressed as initializations of static class members (since the
  order of static initialization is unspecified in C++) and that thus require
  execution by active code.
*/
// cppcheck-suppress duplInheritedMember
void RV32IF::init( void )
{
  DSTART( "static void RV32IF::init()" );

  //
  // RV32IF operation formats.
  //

  WIR_BasicBlock b;
  const RV_FRegV *fRegV = new RV_FRegV;
  const RV_RegV *regV = new RV_RegV;

  registerOperationFormat(
    OperationFormat::FC12R_1,
    { new WIR_RegisterParameter( *fRegV, WIR_Usage::def ),
      new RV_Const12_Signed( 0 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::FC12R_2,
    { new WIR_RegisterParameter( *fRegV, WIR_Usage::use ),
      new RV_Const12_Signed( 0 ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::FF_1,
    { new WIR_RegisterParameter( *fRegV, WIR_Usage::def ),
      new WIR_RegisterParameter( *fRegV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::FFF_1,
    { new WIR_RegisterParameter( *fRegV, WIR_Usage::def ),
      new WIR_RegisterParameter( *fRegV, WIR_Usage::use ),
      new WIR_RegisterParameter( *fRegV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::FFFF_1,
    { new WIR_RegisterParameter( *fRegV, WIR_Usage::def ),
      new WIR_RegisterParameter( *fRegV, WIR_Usage::use ),
      new WIR_RegisterParameter( *fRegV, WIR_Usage::use ),
      new WIR_RegisterParameter( *fRegV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::FLR_1,
    { new WIR_RegisterParameter( *fRegV, WIR_Usage::def ),
      new WIR_LabelParameter( b ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::FLR_2,
    { new WIR_RegisterParameter( *fRegV, WIR_Usage::use ),
      new WIR_LabelParameter( b ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::FR_1,
    { new WIR_RegisterParameter( *fRegV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::FRS_1,
    { new WIR_RegisterParameter( *fRegV, WIR_Usage::def ),
      new WIR_RegisterParameter( *regV, WIR_Usage::use ),
      new WIR_StringParameter( "rtz" ) } );

  registerOperationFormat(
    OperationFormat::RF_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *fRegV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RFF_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *fRegV, WIR_Usage::use ),
      new WIR_RegisterParameter( *fRegV, WIR_Usage::use ) } );

  registerOperationFormat(
    OperationFormat::RFS_1,
    { new WIR_RegisterParameter( *regV, WIR_Usage::def ),
      new WIR_RegisterParameter( *fRegV, WIR_Usage::use ),
      new WIR_StringParameter( "rtz" ) } );


  //
  // RV32IF opcode to operation format mapping.
  //

  registerOpCode( OpCode::FADD_S, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FCLASS_S, OperationFormat::RF_1 );
  registerOpCode( OpCode::FCVT_S_W, OperationFormat::FRS_1 );
  registerOpCode( OpCode::FCVT_S_WU, OperationFormat::FRS_1 );
  registerOpCode( OpCode::FCVT_W_S, OperationFormat::RFS_1 );
  registerOpCode( OpCode::FCVT_WU_S, OperationFormat::RFS_1 );
  registerOpCode( OpCode::FDIV_S, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FEQ_S, OperationFormat::RFF_1 );
  registerOpCode( OpCode::FLE_S, OperationFormat::RFF_1 );
  registerOpCode( OpCode::FLT_S, OperationFormat::RFF_1 );
  registerOpCode( OpCode::FLW, OperationFormat::FC12R_1 );
  registerOpCode( OpCode::FLW, OperationFormat::FLR_1 );
  registerOpCode( OpCode::FMADD_S, OperationFormat::FFFF_1 );
  registerOpCode( OpCode::FMAX_S, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FMIN_S, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FMSUB_S, OperationFormat::FFFF_1 );
  registerOpCode( OpCode::FMUL_S, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FMV_S, OperationFormat::FF_1 );
  registerOpCode( OpCode::FMV_W_X, OperationFormat::FR_1 );
  registerOpCode( OpCode::FMV_X_W, OperationFormat::RF_1 );
  registerOpCode( OpCode::FNMADD_S, OperationFormat::FFFF_1 );
  registerOpCode( OpCode::FNMSUB_S, OperationFormat::FFFF_1 );
  registerOpCode( OpCode::FSGNJ_S, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FSGNJN_S, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FSGNJX_S, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FSQRT_S, OperationFormat::FF_1 );
  registerOpCode( OpCode::FSUB_S, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FSW, OperationFormat::FC12R_2 );
  registerOpCode( OpCode::FSW, OperationFormat::FLR_2 );


  //
  // Register this current processor model.
  //

  registerProcessor( RV32IF() );


  //
  // Finally, initialize derived processor models.
  //

  RV32IFD::init();
};


/*
  Access to physical floating-point register f0.
*/
const RV_FRegP &RV32IF::f0( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f0() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 48 ).get() ) );
};


/*
  Access to physical floating-point register f1.
*/
const RV_FRegP &RV32IF::f1( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f1() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 49 ).get() ) );
};


/*
  Access to physical floating-point register f2.
*/
const RV_FRegP &RV32IF::f2( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f2() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 50 ).get() ) );
};


/*
  Access to physical floating-point register f3.
*/
const RV_FRegP &RV32IF::f3( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f3() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 51 ).get() ) );
};


/*
  Access to physical floating-point register f4.
*/
const RV_FRegP &RV32IF::f4( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f4() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 52 ).get() ) );
};


/*
  Access to physical floating-point register f5.
*/
const RV_FRegP &RV32IF::f5( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f5() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 53 ).get() ) );
};


/*
  Access to physical floating-point register f6.
*/
const RV_FRegP &RV32IF::f6( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f6() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 54 ).get() ) );
};


/*
  Access to physical floating-point register f7.
*/
const RV_FRegP &RV32IF::f7( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f7() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 55 ).get() ) );
};


/*
  Access to physical floating-point register f8.
*/
const RV_FRegP &RV32IF::f8( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f8() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 56 ).get() ) );
};


/*
  Access to physical floating-point register f9.
*/
const RV_FRegP &RV32IF::f9( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f9() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 57 ).get() ) );
};


/*
  Access to physical floating-point register f10.
*/
const RV_FRegP &RV32IF::f10( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f10() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 58 ).get() ) );
};


/*
  Access to physical floating-point register f11.
*/
const RV_FRegP &RV32IF::f11( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f11() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 59 ).get() ) );
};


/*
  Access to physical floating-point register f12.
*/
const RV_FRegP &RV32IF::f12( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f12() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 60 ).get() ) );
};


/*
  Access to physical floating-point register f13.
*/
const RV_FRegP &RV32IF::f13( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f13() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 61 ).get() ) );
};


/*
  Access to physical floating-point register f14.
*/
const RV_FRegP &RV32IF::f14( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f14() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 62 ).get() ) );
};


/*
  Access to physical floating-point register f15.
*/
const RV_FRegP &RV32IF::f15( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f15() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 63 ).get() ) );
};


/*
  Access to physical floating-point register f16.
*/
const RV_FRegP &RV32IF::f16( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f16() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 64 ).get() ) );
};


/*
  Access to physical floating-point register f17.
*/
const RV_FRegP &RV32IF::f17( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f17() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 65 ).get() ) );
};


/*
  Access to physical floating-point register f18.
*/
const RV_FRegP &RV32IF::f18( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f18() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 66 ).get() ) );
};


/*
  Access to physical floating-point register f19.
*/
const RV_FRegP &RV32IF::f19( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f19() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 67 ).get() ) );
};


/*
  Access to physical floating-point register f20.
*/
const RV_FRegP &RV32IF::f20( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f20() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 68 ).get() ) );
};

/*
  Access to physical floating-point register f21.
*/
const RV_FRegP &RV32IF::f21( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f21() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 69 ).get() ) );
};


/*
  Access to physical floating-point register f22.
*/
const RV_FRegP &RV32IF::f22( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f22() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 70 ).get() ) );
};


/*
  Access to physical floating-point register f23.
*/
const RV_FRegP &RV32IF::f23( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f23() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 71 ).get() ) );
};


/*
  Access to physical floating-point register f24.
*/
const RV_FRegP &RV32IF::f24( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f24() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 72 ).get() ) );
};


/*
  Access to physical floating-point register f25.
*/
const RV_FRegP &RV32IF::f25( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f25() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 73 ).get() ) );
};


/*
  Access to physical floating-point register f26.
*/
const RV_FRegP &RV32IF::f26( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f26() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 74 ).get() ) );
};


/*
  Access to physical floating-point register f27.
*/
const RV_FRegP &RV32IF::f27( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f27() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 75 ).get() ) );
};


/*
  Access to physical floating-point register f28.
*/
const RV_FRegP &RV32IF::f28( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f28() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 76 ).get() ) );
};


/*
  Access to physical floating-point register f29.
*/
const RV_FRegP &RV32IF::f29( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f29() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 77 ).get() ) );
};


/*
  Access to physical floating-point register f30.
*/
const RV_FRegP &RV32IF::f30( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f30() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 78 ).get() ) );
};


/*
  Access to physical floating-point register f31.
*/
const RV_FRegP &RV32IF::f31( void ) const
{
  DSTART( "const RV_FRegP& RV32IF::f31() const" );

  return( dynamic_cast<RV_FRegP &>( mPhRegReferences.at( 79 ).get() ) );
};


//
// Private class methods
//

/*
  clone creates a copy of an RV32IF processor.
*/
WIR_BaseProcessor *RV32IF::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* RV32IF::clone()" );

  return( new RV32IF( *this ) );
};

}       // namespace WIR
