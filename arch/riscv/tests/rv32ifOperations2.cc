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
#include <arch/riscv/rv32if.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  RV32IF p;
  const RV_FRegP &f1 = p.f1(), &f2 = p.f12(), &f3 = p.f2(), &f4 = p.f4();
  const RV_RegP &x1 = p.x10();

  cout.iword( WIR_Indentation() ) = 8;
  cout << riscv << comment;

  // This lambda serves for generating a RISC-V operation, adding a comment to
  // it and finally dumping it.
  auto rvop = []( WIR_Operation &&o ) {
    o.insertContainer(
      WIR_Comment( "Operation Format: " + o.getOperationFormat().getName() ) );
    cout << o << endl;
  };

  // The following operations must be accepted according to the RISC-V RV32IF
  // ISA.
  rvop(
    { RV32IF::OpCode::FADD_S, RV32IF::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FCLASS_S, RV32IF::OperationFormat::RF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_W, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rne" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_W, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rtz" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_W, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rdn" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_W, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rup" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_W, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rmm" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_WU, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rne" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_WU, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rtz" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_WU, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rdn" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_WU, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rup" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_WU, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rmm" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_W_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      WIR_StringParameter( "rne" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_W_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      WIR_StringParameter( "rtz" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_W_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      WIR_StringParameter( "rdn" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_W_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      WIR_StringParameter( "rup" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_W_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      WIR_StringParameter( "rmm" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_WU_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_StringParameter( "rne" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_WU_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_StringParameter( "rtz" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_WU_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_StringParameter( "rdn" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_WU_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_StringParameter( "rup" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_WU_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_StringParameter( "rmm" ) } );

  rvop(
    { RV32IF::OpCode::FDIV_S, RV32IF::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FEQ_S, RV32IF::OperationFormat::RFF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FLE_S, RV32IF::OperationFormat::RFF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FLT_S, RV32IF::OperationFormat::RFF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FLW, RV32IF::OperationFormat::FC12R_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FMADD_S, RV32IF::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FMAX_S, RV32IF::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FMIN_S, RV32IF::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FMSUB_S, RV32IF::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FMUL_S, RV32IF::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FMV_S, RV32IF::OperationFormat::FF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FMV_W_X, RV32IF::OperationFormat::FR_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FMV_X_W, RV32IF::OperationFormat::RF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FNMADD_S, RV32IF::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FNMSUB_S, RV32IF::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FSGNJ_S, RV32IF::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FSGNJN_S, RV32IF::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FSGNJX_S, RV32IF::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FSQRT_S, RV32IF::OperationFormat::FF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FSUB_S, RV32IF::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IF::OpCode::FSW, RV32IF::OperationFormat::FC12R_2,
      new WIR_RegisterParameter( f1, WIR_Usage::use ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ) } );

  return( 0 );
}
