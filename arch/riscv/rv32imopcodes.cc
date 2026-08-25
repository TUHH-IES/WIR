/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32imopcodes.cc
  @brief This file declares the RISC-V RV32IM V2.0's opcodes.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32im.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

const RV32IM::OpCode RV32IM::OpCode::DIV    { "div" };
const RV32IM::OpCode RV32IM::OpCode::DIVU   { "divu" };
const RV32IM::OpCode RV32IM::OpCode::MUL    { "mul" };
const RV32IM::OpCode RV32IM::OpCode::MULH   { "mulh" };
const RV32IM::OpCode RV32IM::OpCode::MULHSU { "mulhsu" };
const RV32IM::OpCode RV32IM::OpCode::MULHU  { "mulhu" };
const RV32IM::OpCode RV32IM::OpCode::REM    { "rem" };
const RV32IM::OpCode RV32IM::OpCode::REMU   { "remu" };

}       // namespace WIR
