/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/


//
// Include section
//

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32ifd.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  RV32IFD p;
  const RV_FRegP &f1 = p.f1(), &f2 = p.f12(), &f3 = p.f2(), &f4 = p.f4();
  const RV_RegP  &x1 = p.x10();

  cout.iword( WIR_Indentation() ) = 8;
  cout << riscv << comment;

  // This lambda serves for generating a RISC-V operation, adding a comment to
  // it and finally dumping it.
  auto rvop = []( WIR_Operation &&o ) {
    o.insertContainer(
      WIR_Comment( "Operation Format: " + o.getOperationFormat().getName() ) );
    cout << o << endl;
  };

  // The following operations must be accepted according to the RISC-V RV32IFD
  // ISA.
  rvop(
    { RV32IFD::OpCode::FADD_D, RV32IFD::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FCLASS_D, RV32IFD::OperationFormat::RF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IFD::OpCode::FCVT_D_S, RV32IFD::OperationFormat::FF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use )} );

  rvop(
    { RV32IFD::OpCode::FCVT_D_W, RV32IFD::OperationFormat::FR_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use )} );

  rvop(
    { RV32IFD::OpCode::FCVT_D_WU, RV32IFD::OperationFormat::FR_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use )} );

  rvop(
    { RV32IFD::OpCode::FCVT_S_D, RV32IFD::OperationFormat::FF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use )} );

  rvop(
    { RV32IFD::OpCode::FCVT_W_D, RV32IFD::OperationFormat::RF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IFD::OpCode::FCVT_WU_D, RV32IFD::OperationFormat::RF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IFD::OpCode::FDIV_D, RV32IFD::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FEQ_D, RV32IFD::OperationFormat::RFF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IFD::OpCode::FLD, RV32IFD::OperationFormat::FC12R_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FLE_D, RV32IFD::OperationFormat::RFF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IFD::OpCode::FLT_D, RV32IFD::OperationFormat::RFF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IFD::OpCode::FMADD_D, RV32IFD::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FMAX_D, RV32IFD::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FMIN_D, RV32IFD::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FMSUB_D, RV32IFD::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FMUL_D, RV32IFD::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FMV_D, RV32IFD::OperationFormat::FF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IFD::OpCode::FNMADD_D, RV32IFD::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FNMSUB_D, RV32IFD::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FSD, RV32IFD::OperationFormat::FC12R_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FSGNJ_D, RV32IFD::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FSGNJN_D, RV32IFD::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FSGNJX_D, RV32IFD::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IFD::OpCode::FSQRT_D, RV32IFD::OperationFormat::FF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IFD::OpCode::FSUB_D, RV32IFD::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

   return( 0 );
}
