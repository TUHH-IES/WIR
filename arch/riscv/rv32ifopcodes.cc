/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32ifopcodes.cc
  @brief This file declares the RISC-V RV32IF V2.0's opcodes.

  @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32if.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

const RV32IF::OpCode RV32IF::OpCode::FADD_S    { "fadd.s" };
const RV32IF::OpCode RV32IF::OpCode::FCLASS_S  { "fclass.s" };
const RV32IF::OpCode RV32IF::OpCode::FCVT_S_W  { "fcvt.s.w" };
const RV32IF::OpCode RV32IF::OpCode::FCVT_S_WU { "fcvt.s.wu" };
const RV32IF::OpCode RV32IF::OpCode::FCVT_W_S  { "fcvt.w.s" };
const RV32IF::OpCode RV32IF::OpCode::FCVT_WU_S { "fcvt.wu.s" };
const RV32IF::OpCode RV32IF::OpCode::FDIV_S    { "fdiv.s" };
const RV32IF::OpCode RV32IF::OpCode::FEQ_S     { "feq.s",
                                                 WIR_OpCodeType::sideEffect };
const RV32IF::OpCode RV32IF::OpCode::FLE_S     { "fle.s",
                                                 WIR_OpCodeType::sideEffect };
const RV32IF::OpCode RV32IF::OpCode::FLT_S     { "flt.s",
                                                 WIR_OpCodeType::sideEffect };
const RV32IF::OpCode RV32IF::OpCode::FLW       { "flw", WIR_OpCodeType::load };
const RV32IF::OpCode RV32IF::OpCode::FMADD_S   { "fmadd.s" };
const RV32IF::OpCode RV32IF::OpCode::FMAX_S    { "fmax.s" };
const RV32IF::OpCode RV32IF::OpCode::FMIN_S    { "fmin.s" };
const RV32IF::OpCode RV32IF::OpCode::FMSUB_S   { "fmsub.s" };
const RV32IF::OpCode RV32IF::OpCode::FMUL_S    { "fmul.s" };

// FMV.S is a pseudo-operation de facto realized by an FSGNJ.S:
//   fsgnj.s $tgt, $src, $src
const RV32IF::OpCode RV32IF::OpCode::FMV_S     { "fsgnj.s",
                                                 WIR_OpCodeType::move };
const RV32IF::OpCode RV32IF::OpCode::FMV_W_X   { "fmv.w.x" };
const RV32IF::OpCode RV32IF::OpCode::FMV_X_W   { "fmv.x.w" };
const RV32IF::OpCode RV32IF::OpCode::FNMADD_S  { "fnmadd.s" };
const RV32IF::OpCode RV32IF::OpCode::FNMSUB_S  { "fnmsub.s" };
const RV32IF::OpCode RV32IF::OpCode::FSGNJ_S   { "fsgnj.s" };
const RV32IF::OpCode RV32IF::OpCode::FSGNJN_S  { "fsgnjn.s" };
const RV32IF::OpCode RV32IF::OpCode::FSGNJX_S  { "fsgnjx.s" };
const RV32IF::OpCode RV32IF::OpCode::FSQRT_S   { "fsqrt.s" };
const RV32IF::OpCode RV32IF::OpCode::FSUB_S    { "fsub.s" };
const RV32IF::OpCode RV32IF::OpCode::FSW       { "fsw", WIR_OpCodeType::store };

}       // namespace WIR
