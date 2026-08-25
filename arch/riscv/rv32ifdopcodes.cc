/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32ifdopcodes.cc
  @brief This file declares the RISC-V RV32IFD V2.0's opcodes.

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
#include <arch/riscv/rv32ifd.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

const RV32IFD::OpCode RV32IFD::OpCode::FADD_D    { "fadd.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FCLASS_D  { "fclass.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FCVT_D_S  { "fcvt.d.s" };
const RV32IFD::OpCode RV32IFD::OpCode::FCVT_D_W  { "fcvt.d.w" };
const RV32IFD::OpCode RV32IFD::OpCode::FCVT_D_WU { "fcvt.d.wu" };
const RV32IFD::OpCode RV32IFD::OpCode::FCVT_S_D  { "fcvt.s.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FCVT_W_D  { "fcvt.w.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FCVT_WU_D { "fcvt.wu.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FDIV_D    { "fdiv.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FEQ_D     { "feq.d",
                                                   WIR_OpCodeType::sideEffect };
const RV32IFD::OpCode RV32IFD::OpCode::FLD       { "fld",
                                                   WIR_OpCodeType::load };
const RV32IFD::OpCode RV32IFD::OpCode::FLE_D     { "fle.d",
                                                   WIR_OpCodeType::sideEffect };
const RV32IFD::OpCode RV32IFD::OpCode::FLT_D     { "flt.d",
                                                   WIR_OpCodeType::sideEffect };
const RV32IFD::OpCode RV32IFD::OpCode::FMADD_D   { "fmadd.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FMAX_D    { "fmax.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FMIN_D    { "fmin.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FMSUB_D   { "fmsub.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FMUL_D    { "fmul.d" };

// FMV.D is a pseudo-operation de facto realized by an FSGNJ.D:
//   fsgnj.d $tgt, $src, $src
const RV32IFD::OpCode RV32IFD::OpCode::FMV_D     { "fsgnj.d",
                                                   WIR_OpCodeType::move };
const RV32IFD::OpCode RV32IFD::OpCode::FNMADD_D  { "fnmadd.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FNMSUB_D  { "fnmsub.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FSD       { "fsd",
                                                   WIR_OpCodeType::store };
const RV32IFD::OpCode RV32IFD::OpCode::FSGNJ_D   { "fsgnj.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FSGNJN_D  { "fsgnjn.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FSGNJX_D  { "fsgnjx.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FSQRT_D   { "fsqrt.d" };
const RV32IFD::OpCode RV32IFD::OpCode::FSUB_D    { "fsub.d" };

}       // namespace WIR
