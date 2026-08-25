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

// Include libuseful headers
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32if.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  RV32IF riscv;
  RV_FRegV f1, f2, f3,f4;
  RV_RegV x1;

 // The following operations must be accepted according to the RISC-V ISA.
  WIR_Operation fadd1(
    RV32IF::OpCode::FADD_S, RV32IF::OperationFormat::FFF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fadd1.getSize() == 4 );
  ufAssert(
    !fadd1.isImplicitMemoryAccess() && !fadd1.isMemoryStore() &&
    !fadd1.isMemoryLoad() && !fadd1.isMove() && !fadd1.isCall() &&
    !fadd1.isIndirectCall() && !fadd1.isReturn() && !fadd1.isJump() &&
    !fadd1.isConditionalJump() && !fadd1.isUnconditionalJump() &&
    !fadd1.isIndirectJump() && !fadd1.isAsmDataDirective() &&
    !fadd1.hasSideEffects() );

  WIR_Operation fclass1(
    RV32IF::OpCode::FCLASS_S, RV32IF::OperationFormat::RF_1,
    new WIR_RegisterParameter( x1, WIR_Usage::def ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fclass1.getSize() == 4 );
  ufAssert(
    !fclass1.isImplicitMemoryAccess() && !fclass1.isMemoryStore() &&
    !fclass1.isMemoryLoad() && !fclass1.isMove() && !fclass1.isCall() &&
    !fclass1.isIndirectCall() && !fclass1.isReturn() && !fclass1.isJump() &&
    !fclass1.isConditionalJump() && !fclass1.isUnconditionalJump() &&
    !fclass1.isIndirectJump() && !fclass1.isAsmDataDirective() &&
    !fclass1.hasSideEffects() );

  WIR_Operation fcvtsw1(
    RV32IF::OpCode::FCVT_S_W, RV32IF::OperationFormat::FRS_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( x1, WIR_Usage::use ),
    new WIR_StringParameter( "rtz" ) );
  ufAssert( fcvtsw1.getSize() == 4 );
  ufAssert(
    !fcvtsw1.isImplicitMemoryAccess() && !fcvtsw1.isMemoryStore() &&
    !fcvtsw1.isMemoryLoad() && !fcvtsw1.isMove() && !fcvtsw1.isCall() &&
    !fcvtsw1.isIndirectCall() && !fcvtsw1.isReturn() && !fcvtsw1.isJump() &&
    !fcvtsw1.isConditionalJump() && !fcvtsw1.isUnconditionalJump() &&
    !fcvtsw1.isIndirectJump() && !fcvtsw1.isAsmDataDirective() &&
    !fcvtsw1.hasSideEffects() );

  WIR_Operation fcvtswu1(
    RV32IF::OpCode::FCVT_S_WU, RV32IF::OperationFormat::FRS_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( x1, WIR_Usage::use ),
    new WIR_StringParameter( "rtz" ) );
  ufAssert( fcvtswu1.getSize() == 4 );
  ufAssert(
    !fcvtswu1.isImplicitMemoryAccess() && !fcvtswu1.isMemoryStore() &&
    !fcvtswu1.isMemoryLoad() && !fcvtswu1.isMove() && !fcvtswu1.isCall() &&
    !fcvtswu1.isIndirectCall() && !fcvtswu1.isReturn() && !fcvtswu1.isJump() &&
    !fcvtswu1.isConditionalJump() && !fcvtswu1.isUnconditionalJump() &&
    !fcvtswu1.isIndirectJump() && !fcvtswu1.isAsmDataDirective() &&
    !fcvtswu1.hasSideEffects() );

  WIR_Operation fcvtws1(
    RV32IF::OpCode::FCVT_W_S, RV32IF::OperationFormat::RFS_1,
    new WIR_RegisterParameter( x1, WIR_Usage::def ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ),
    new WIR_StringParameter( "rtz" ) );
  ufAssert( fcvtws1.getSize() == 4 );
  ufAssert(
    !fcvtws1.isImplicitMemoryAccess() && !fcvtws1.isMemoryStore() &&
    !fcvtws1.isMemoryLoad() && !fcvtws1.isMove() && !fcvtws1.isCall() &&
    !fcvtws1.isIndirectCall() && !fcvtws1.isReturn() && !fcvtws1.isJump() &&
    !fcvtws1.isConditionalJump() && !fcvtws1.isUnconditionalJump() &&
    !fcvtws1.isIndirectJump() && !fcvtws1.isAsmDataDirective() &&
    !fcvtws1.hasSideEffects() );

  WIR_Operation fcvtwus1(
    RV32IF::OpCode::FCVT_WU_S, RV32IF::OperationFormat::RFS_1,
    new WIR_RegisterParameter( x1, WIR_Usage::def ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ),
    new WIR_StringParameter( "rtz" ) );
  ufAssert( fcvtwus1.getSize() == 4 );
  ufAssert(
    !fcvtwus1.isImplicitMemoryAccess() && !fcvtwus1.isMemoryStore() &&
    !fcvtwus1.isMemoryLoad() && !fcvtwus1.isMove() && !fcvtwus1.isCall() &&
    !fcvtwus1.isIndirectCall() && !fcvtwus1.isReturn() && !fcvtwus1.isJump() &&
    !fcvtwus1.isConditionalJump() && !fcvtwus1.isUnconditionalJump() &&
    !fcvtwus1.isIndirectJump() && !fcvtwus1.isAsmDataDirective() &&
    !fcvtwus1.hasSideEffects() );

  WIR_Operation fdiv1(
    RV32IF::OpCode::FDIV_S, RV32IF::OperationFormat::FFF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fdiv1.getSize() == 4 );
  ufAssert(
    !fdiv1.isImplicitMemoryAccess() && !fdiv1.isMemoryStore() &&
    !fdiv1.isMemoryLoad() && !fdiv1.isMove() && !fdiv1.isCall() &&
    !fdiv1.isIndirectCall() && !fdiv1.isReturn() && !fdiv1.isJump() &&
    !fdiv1.isConditionalJump() && !fdiv1.isUnconditionalJump() &&
    !fdiv1.isIndirectJump() && !fdiv1.isAsmDataDirective() &&
    !fdiv1.hasSideEffects() );

  WIR_Operation feq1(
    RV32IF::OpCode::FEQ_S, RV32IF::OperationFormat::RFF_1,
    new WIR_RegisterParameter( x1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( feq1.getSize() == 4 );
  ufAssert(
    !feq1.isImplicitMemoryAccess() && !feq1.isMemoryStore() &&
    !feq1.isMemoryLoad() && !feq1.isMove() && !feq1.isCall() &&
    !feq1.isIndirectCall() && !feq1.isReturn() && !feq1.isJump() &&
    !feq1.isConditionalJump() && !feq1.isUnconditionalJump() &&
    !feq1.isIndirectJump() && !feq1.isAsmDataDirective() &&
    feq1.hasSideEffects() );

  WIR_Operation fle1(
    RV32IF::OpCode::FLE_S, RV32IF::OperationFormat::RFF_1,
    new WIR_RegisterParameter( x1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fle1.getSize() == 4 );
  ufAssert(
    !fle1.isImplicitMemoryAccess() && !fle1.isMemoryStore() &&
    !fle1.isMemoryLoad() && !fle1.isMove() && !fle1.isCall() &&
    !fle1.isIndirectCall() && !fle1.isReturn() && !fle1.isJump() &&
    !fle1.isConditionalJump() && !fle1.isUnconditionalJump() &&
    !fle1.isIndirectJump() && !fle1.isAsmDataDirective() &&
    fle1.hasSideEffects() );

  WIR_Operation flt1(
    RV32IF::OpCode::FLT_S, RV32IF::OperationFormat::RFF_1,
    new WIR_RegisterParameter( x1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( flt1.getSize() == 4 );
  ufAssert(
    !flt1.isImplicitMemoryAccess() && !flt1.isMemoryStore() &&
    !flt1.isMemoryLoad() && !flt1.isMove() && !flt1.isCall() &&
    !flt1.isIndirectCall() && !flt1.isReturn() && !flt1.isJump() &&
    !flt1.isConditionalJump() && !flt1.isUnconditionalJump() &&
    !flt1.isIndirectJump() && !flt1.isAsmDataDirective() &&
    flt1.hasSideEffects() );

  WIR_Operation flw1(
    RV32IF::OpCode::FLW, RV32IF::OperationFormat::FC12R_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new RV_Const12_Signed( -1023 ),
    new WIR_RegisterParameter( x1, WIR_Usage::use ) );
  ufAssert( flw1.getSize() == 4 );
  ufAssert(
    !flw1.isImplicitMemoryAccess() && !flw1.isMemoryStore() &&
    flw1.isMemoryLoad() && !flw1.isMove() && !flw1.isCall() &&
    !flw1.isIndirectCall() && !flw1.isReturn() && !flw1.isJump() &&
    !flw1.isConditionalJump() && !flw1.isUnconditionalJump() &&
    !flw1.isIndirectJump() && !flw1.isAsmDataDirective() &&
    !flw1.hasSideEffects() );

  WIR_Operation fmadd1(
    RV32IF::OpCode::FMADD_S, RV32IF::OperationFormat::FFFF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ),
    new WIR_RegisterParameter( f4, WIR_Usage::use ) );
  ufAssert( fmadd1.getSize() == 4 );
  ufAssert(
    !fmadd1.isImplicitMemoryAccess() && !fmadd1.isMemoryStore() &&
    !fmadd1.isMemoryLoad() && !fmadd1.isMove() && !fmadd1.isCall() &&
    !fmadd1.isIndirectCall() && !fmadd1.isReturn() && !fmadd1.isJump() &&
    !fmadd1.isConditionalJump() && !fmadd1.isUnconditionalJump() &&
    !fmadd1.isIndirectJump() && !fmadd1.isAsmDataDirective() &&
    !fmadd1.hasSideEffects() );

  WIR_Operation fmax1(
    RV32IF::OpCode::FMAX_S, RV32IF::OperationFormat::FFF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fmax1.getSize() == 4 );
  ufAssert(
    !fmax1.isImplicitMemoryAccess() && !fmax1.isMemoryStore() &&
    !fmax1.isMemoryLoad() && !fmax1.isMove() && !fmax1.isCall() &&
    !fmax1.isIndirectCall() && !fmax1.isReturn() && !fmax1.isJump() &&
    !fmax1.isConditionalJump() && !fmax1.isUnconditionalJump() &&
    !fmax1.isIndirectJump() && !fmax1.isAsmDataDirective() &&
    !fmax1.hasSideEffects() );

  WIR_Operation fmin1(
    RV32IF::OpCode::FMIN_S, RV32IF::OperationFormat::FFF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fmin1.getSize() == 4 );
  ufAssert(
    !fmin1.isImplicitMemoryAccess() && !fmin1.isMemoryStore() &&
    !fmin1.isMemoryLoad() && !fmin1.isMove() && !fmin1.isCall() &&
    !fmin1.isIndirectCall() && !fmin1.isReturn() && !fmin1.isJump() &&
    !fmin1.isConditionalJump() && !fmin1.isUnconditionalJump() &&
    !fmin1.isIndirectJump() && !fmin1.isAsmDataDirective() &&
    !fmin1.hasSideEffects() );

  WIR_Operation fmsub1(
    RV32IF::OpCode::FMSUB_S, RV32IF::OperationFormat::FFFF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ),
    new WIR_RegisterParameter( f4, WIR_Usage::use ) );
  ufAssert( fmsub1.getSize() == 4 );
  ufAssert(
    !fmsub1.isImplicitMemoryAccess() && !fmsub1.isMemoryStore() &&
    !fmsub1.isMemoryLoad() && !fmsub1.isMove() && !fmsub1.isCall() &&
    !fmsub1.isIndirectCall() && !fmsub1.isReturn() && !fmsub1.isJump() &&
    !fmsub1.isConditionalJump() && !fmsub1.isUnconditionalJump() &&
    !fmsub1.isIndirectJump() && !fmsub1.isAsmDataDirective() &&
    !fmsub1.hasSideEffects() );

  WIR_Operation fmul1(
    RV32IF::OpCode::FMUL_S, RV32IF::OperationFormat::FFF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fmul1.getSize() == 4 );
  ufAssert(
    !fmul1.isImplicitMemoryAccess() && !fmul1.isMemoryStore() &&
    !fmul1.isMemoryLoad() && !fmul1.isMove() && !fmul1.isCall() &&
    !fmul1.isIndirectCall() && !fmul1.isReturn() && !fmul1.isJump() &&
    !fmul1.isConditionalJump() && !fmul1.isUnconditionalJump() &&
    !fmul1.isIndirectJump() && !fmul1.isAsmDataDirective() &&
    !fmul1.hasSideEffects() );

  WIR_Operation fmv1(
    RV32IF::OpCode::FMV_S, RV32IF::OperationFormat::FF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fmv1.getSize() == 4 );
  ufAssert(
    !fmv1.isImplicitMemoryAccess() && !fmv1.isMemoryStore() &&
    !fmv1.isMemoryLoad() && fmv1.isMove() && !fmv1.isCall() &&
    !fmv1.isIndirectCall() && !fmv1.isReturn() && !fmv1.isJump() &&
    !fmv1.isConditionalJump() && !fmv1.isUnconditionalJump() &&
    !fmv1.isIndirectJump() && !fmv1.isAsmDataDirective() &&
    !fmv1.hasSideEffects() );

  WIR_Operation fmvwx1(
    RV32IF::OpCode::FMV_W_X, RV32IF::OperationFormat::FR_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( x1, WIR_Usage::use ) );
  ufAssert( fmvwx1.getSize() == 4 );
  ufAssert(
    !fmvwx1.isImplicitMemoryAccess() && !fmvwx1.isMemoryStore() &&
    !fmvwx1.isMemoryLoad() && !fmvwx1.isMove() && !fmvwx1.isCall() &&
    !fmvwx1.isIndirectCall() && !fmvwx1.isReturn() && !fmvwx1.isJump() &&
    !fmvwx1.isConditionalJump() && !fmvwx1.isUnconditionalJump() &&
    !fmvwx1.isIndirectJump() && !fmvwx1.isAsmDataDirective() &&
    !fmvwx1.hasSideEffects() );

  WIR_Operation fmvxw1(
    RV32IF::OpCode::FMV_X_W, RV32IF::OperationFormat::RF_1,
    new WIR_RegisterParameter( x1, WIR_Usage::def ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fmvxw1.getSize() == 4 );
  ufAssert(
    !fmvxw1.isImplicitMemoryAccess() && !fmvxw1.isMemoryStore() &&
    !fmvxw1.isMemoryLoad() && !fmvxw1.isMove() && !fmvxw1.isCall() &&
    !fmvxw1.isIndirectCall() && !fmvxw1.isReturn() && !fmvxw1.isJump() &&
    !fmvxw1.isConditionalJump() && !fmvxw1.isUnconditionalJump() &&
    !fmvxw1.isIndirectJump() && !fmvxw1.isAsmDataDirective() &&
    !fmvxw1.hasSideEffects() );

  WIR_Operation fnmadd1(
    RV32IF::OpCode::FNMADD_S, RV32IF::OperationFormat::FFFF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ),
    new WIR_RegisterParameter( f4, WIR_Usage::use ) );
  ufAssert( fnmadd1.getSize() == 4 );
  ufAssert(
    !fnmadd1.isImplicitMemoryAccess() && !fnmadd1.isMemoryStore() &&
    !fnmadd1.isMemoryLoad() && !fnmadd1.isMove() && !fnmadd1.isCall() &&
    !fnmadd1.isIndirectCall() && !fnmadd1.isReturn() && !fnmadd1.isJump() &&
    !fnmadd1.isConditionalJump() && !fnmadd1.isUnconditionalJump() &&
    !fnmadd1.isIndirectJump() && !fnmadd1.isAsmDataDirective() &&
    !fnmadd1.hasSideEffects() );

  WIR_Operation fnmsub1(
    RV32IF::OpCode::FNMSUB_S, RV32IF::OperationFormat::FFFF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ),
    new WIR_RegisterParameter( f4, WIR_Usage::use ) );
  ufAssert( fnmsub1.getSize() == 4 );
  ufAssert(
    !fnmsub1.isImplicitMemoryAccess() && !fnmsub1.isMemoryStore() &&
    !fnmsub1.isMemoryLoad() && !fnmsub1.isMove() && !fnmsub1.isCall() &&
    !fnmsub1.isIndirectCall() && !fnmsub1.isReturn() && !fnmsub1.isJump() &&
    !fnmsub1.isConditionalJump() && !fnmsub1.isUnconditionalJump() &&
    !fnmsub1.isIndirectJump() && !fnmsub1.isAsmDataDirective() &&
    !fnmsub1.hasSideEffects() );

  WIR_Operation fsgnj1(
    RV32IF::OpCode::FSGNJ_S, RV32IF::OperationFormat::FFF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fsgnj1.getSize() == 4 );
  ufAssert(
    !fsgnj1.isImplicitMemoryAccess() && !fsgnj1.isMemoryStore() &&
    !fsgnj1.isMemoryLoad() && !fsgnj1.isMove() && !fsgnj1.isCall() &&
    !fsgnj1.isIndirectCall() && !fsgnj1.isReturn() && !fsgnj1.isJump() &&
    !fsgnj1.isConditionalJump() && !fsgnj1.isUnconditionalJump() &&
    !fsgnj1.isIndirectJump() && !fsgnj1.isAsmDataDirective() &&
    !fsgnj1.hasSideEffects() );

  WIR_Operation fsgnjn1(
    RV32IF::OpCode::FSGNJN_S, RV32IF::OperationFormat::FFF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fsgnjn1.getSize() == 4 );
  ufAssert(
    !fsgnjn1.isImplicitMemoryAccess() && !fsgnjn1.isMemoryStore() &&
    !fsgnjn1.isMemoryLoad() && !fsgnjn1.isMove() && !fsgnjn1.isCall() &&
    !fsgnjn1.isIndirectCall() && !fsgnjn1.isReturn() && !fsgnjn1.isJump() &&
    !fsgnjn1.isConditionalJump() && !fsgnjn1.isUnconditionalJump() &&
    !fsgnjn1.isIndirectJump() && !fsgnjn1.isAsmDataDirective() &&
    !fsgnjn1.hasSideEffects() );

  WIR_Operation fsgnjx1(
    RV32IF::OpCode::FSGNJX_S, RV32IF::OperationFormat::FFF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fsgnjx1.getSize() == 4 );
  ufAssert(
    !fsgnjx1.isImplicitMemoryAccess() && !fsgnjx1.isMemoryStore() &&
    !fsgnjx1.isMemoryLoad() && !fsgnjx1.isMove() && !fsgnjx1.isCall() &&
    !fsgnjx1.isIndirectCall() && !fsgnjx1.isReturn() && !fsgnjx1.isJump() &&
    !fsgnjx1.isConditionalJump() && !fsgnjx1.isUnconditionalJump() &&
    !fsgnjx1.isIndirectJump() && !fsgnjx1.isAsmDataDirective() &&
    !fsgnjx1.hasSideEffects() );

  WIR_Operation fsqrt1(
    RV32IF::OpCode::FSQRT_S, RV32IF::OperationFormat::FF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fsqrt1.getSize() == 4 );
  ufAssert(
    !fsqrt1.isImplicitMemoryAccess() && !fsqrt1.isMemoryStore() &&
    !fsqrt1.isMemoryLoad() && !fsqrt1.isMove() && !fsqrt1.isCall() &&
    !fsqrt1.isIndirectCall() && !fsqrt1.isReturn() && !fsqrt1.isJump() &&
    !fsqrt1.isConditionalJump() && !fsqrt1.isUnconditionalJump() &&
    !fsqrt1.isIndirectJump() && !fsqrt1.isAsmDataDirective() &&
    !fsqrt1.hasSideEffects() );

  WIR_Operation fsub1(
    RV32IF::OpCode::FSUB_S, RV32IF::OperationFormat::FFF_1,
    new WIR_RegisterParameter( f1, WIR_Usage::def ),
    new WIR_RegisterParameter( f2, WIR_Usage::use ),
    new WIR_RegisterParameter( f3, WIR_Usage::use ) );
  ufAssert( fsub1.getSize() == 4 );
  ufAssert(
    !fsub1.isImplicitMemoryAccess() && !fsub1.isMemoryStore() &&
    !fsub1.isMemoryLoad() && !fsub1.isMove() && !fsub1.isCall() &&
    !fsub1.isIndirectCall() && !fsub1.isReturn() && !fsub1.isJump() &&
    !fsub1.isConditionalJump() && !fsub1.isUnconditionalJump() &&
    !fsub1.isIndirectJump() && !fsub1.isAsmDataDirective() &&
    !fsub1.hasSideEffects() );

  WIR_Operation fsw1(
    RV32IF::OpCode::FSW, RV32IF::OperationFormat::FC12R_2,
    new WIR_RegisterParameter( f1, WIR_Usage::use ),
    new RV_Const12_Signed( -1023 ),
    new WIR_RegisterParameter( x1, WIR_Usage::use ) );
  ufAssert( flw1.getSize() == 4 );
  ufAssert(
    !fsw1.isImplicitMemoryAccess() && fsw1.isMemoryStore() &&
    !fsw1.isMemoryLoad() && !fsw1.isMove() && !fsw1.isCall() &&
    !fsw1.isIndirectCall() && !fsw1.isReturn() && !fsw1.isJump() &&
    !fsw1.isConditionalJump() && !fsw1.isUnconditionalJump() &&
    !fsw1.isIndirectJump() && !fsw1.isAsmDataDirective() &&
    !fsw1.hasSideEffects() );

  return( 0 );
}
