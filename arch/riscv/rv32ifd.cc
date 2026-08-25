/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32ifd.cc
  @brief This file implements the specific interface of the RISC-V RV32IFD
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
  Default constructor for RV32IFD processor architectures.
*/
RV32IFD::RV32IFD( void ) :
  RV32IF {}
{
  DSTART( "RV32IFD::RV32IFD()" );

  // Specify the processor architecture modeled by this class.
  setISAName( "RV32IFD" );

  // Set bit width of floating-point registers to 64.
  RV32IFD::RegisterType::fReg.setBitWidth( 64 );
};


/*
  Copy constructor.
*/
RV32IFD::RV32IFD( const RV32IFD &__o ) :
  RV32I { __o },
  RV32IF { __o }
{
  DSTART( "RV32IFD::RV32IFD(const RV32IFD&)" );
};


/*
  Move constructor.
*/
RV32IFD::RV32IFD( RV32IFD &&__o ) :
  RV32IF { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
RV32IFD::~RV32IFD( void )
{
  DSTART( "virtual RV32IFD::~RV32IFD()" );
};


/*
  Copy-assignment operator.
*/
RV32IFD & RV32IFD::operator = ( const RV32IFD &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IF::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV32IFD & RV32IFD::operator = ( RV32IFD &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IF::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for RV32IFD processor
  architectures.

  This includes setting up the assignment of valid operation formats to
  RV32IFD opcodes.

  init shall be called globally by WIR_Init(). It shall only perform tasks that
  cannot be expressed as initializations of static class members (since the
  order of static initialization is unspecified in C++) and that thus require
  execution by active code.
*/
// cppcheck-suppress duplInheritedMember
void RV32IFD::init( void )
{
  DSTART( "static void RV32IFD::init()" );

  //
  // RV32IFD opcode to operation format mapping.
  //

  registerOpCode( OpCode::FADD_D, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FCLASS_D, OperationFormat::RF_1 );
  registerOpCode( OpCode::FCVT_D_S, OperationFormat::FF_1 );
  registerOpCode( OpCode::FCVT_D_W, OperationFormat::FR_1 );
  registerOpCode( OpCode::FCVT_D_WU, OperationFormat::FR_1 );
  registerOpCode( OpCode::FCVT_S_D, OperationFormat::FF_1 );
  registerOpCode( OpCode::FCVT_W_D, OperationFormat::RF_1 );
  registerOpCode( OpCode::FCVT_WU_D, OperationFormat::RF_1 );
  registerOpCode( OpCode::FDIV_D, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FEQ_D, OperationFormat::RFF_1 );
  registerOpCode( OpCode::FLD, OperationFormat::FC12R_1 );
  registerOpCode( OpCode::FLE_D, OperationFormat::RFF_1 );
  registerOpCode( OpCode::FLT_D, OperationFormat::RFF_1 );
  registerOpCode( OpCode::FMADD_D, OperationFormat::FFFF_1 );
  registerOpCode( OpCode::FMAX_D, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FMIN_D, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FMSUB_D, OperationFormat::FFFF_1 );
  registerOpCode( OpCode::FMUL_D, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FMV_D, OperationFormat::FF_1 );
  registerOpCode( OpCode::FNMADD_D, OperationFormat::FFFF_1 );
  registerOpCode( OpCode::FNMSUB_D, OperationFormat::FFFF_1 );
  registerOpCode( OpCode::FSD, OperationFormat::FC12R_1 );
  registerOpCode( OpCode::FSGNJ_D, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FSGNJN_D, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FSGNJX_D, OperationFormat::FFF_1 );
  registerOpCode( OpCode::FSQRT_D, OperationFormat::FF_1 );
  registerOpCode( OpCode::FSUB_D, OperationFormat::FFF_1 );


  //
  // Register this current processor model.
  //

  registerProcessor( RV32IFD() );
};


//
// Private class methods
//

/*
  clone creates a copy of an RV32IFD processor.
*/
WIR_BaseProcessor *RV32IFD::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* RV32IFD::clone() const" );

  return( new RV32IFD( *this ) );
};

}       // namespace WIR
