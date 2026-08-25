/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32icopcodes.cc
  @brief This file declares the RISC-V RV32IC V2.0's opcodes.

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
#include <arch/riscv/rv32ic.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

const RV32IC::OpCode RV32IC::OpCode::CADD      { "c.add" };
const RV32IC::OpCode RV32IC::OpCode::CADDI     { "c.addi" };
const RV32IC::OpCode RV32IC::OpCode::CADDI16SP { "c.addi16sp" };
const RV32IC::OpCode RV32IC::OpCode::CADDI4SPN { "c.addi4spn" };
const RV32IC::OpCode RV32IC::OpCode::CAND      { "c.and" };
const RV32IC::OpCode RV32IC::OpCode::CANDI     { "c.andi" };
const RV32IC::OpCode RV32IC::OpCode::CBEQZ     { "c.beqz",
                                                 WIR_OpCodeType::condJump };
const RV32IC::OpCode RV32IC::OpCode::CBNEZ     { "c.bnez",
                                                 WIR_OpCodeType::condJump };
const RV32IC::OpCode RV32IC::OpCode::CEBREAK   { "c.ebreak",
                                                 WIR_OpCodeType::sideEffect };
const RV32IC::OpCode RV32IC::OpCode::CJ        { "c.j",
                                                 WIR_OpCodeType::uncondJump };
const RV32IC::OpCode RV32IC::OpCode::CJAL      { "c.jal",
                                                 WIR_OpCodeType::call };
const RV32IC::OpCode RV32IC::OpCode::CJALR     { "c.jalr",
                                                 WIR_OpCodeType::indirectCall };
const RV32IC::OpCode RV32IC::OpCode::CJR       { "c.jr",
                                                 WIR_OpCodeType::indirectJump };
const RV32IC::OpCode RV32IC::OpCode::CLI       { "c.li" };
const RV32IC::OpCode RV32IC::OpCode::CLUI      { "c.lui" };
const RV32IC::OpCode RV32IC::OpCode::CLW       { "c.lw", WIR_OpCodeType::load };
const RV32IC::OpCode RV32IC::OpCode::CLWSP     { "c.lwsp", WIR_OpCodeType::load };
const RV32IC::OpCode RV32IC::OpCode::CMV       { "c.mv", WIR_OpCodeType::move };
const RV32IC::OpCode RV32IC::OpCode::CNOP      { "c.nop" };
const RV32IC::OpCode RV32IC::OpCode::COR       { "c.or" };
const RV32IC::OpCode RV32IC::OpCode::CSLLI     { "c.slli" };
const RV32IC::OpCode RV32IC::OpCode::CSRAI     { "c.srai" };
const RV32IC::OpCode RV32IC::OpCode::CSRLI     { "c.srli" };
const RV32IC::OpCode RV32IC::OpCode::CSUB      { "c.sub" };
const RV32IC::OpCode RV32IC::OpCode::CSW       { "c.sw",
                                                 WIR_OpCodeType::store };
const RV32IC::OpCode RV32IC::OpCode::CSWSP     { "c.swsp",
                                                 WIR_OpCodeType::store };
const RV32IC::OpCode RV32IC::OpCode::CXOR      { "c.xor" };

}       // namespace WIR
