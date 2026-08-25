/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32iopcodes.cc
  @brief This file declares the RISC-V RV32I V2.0's opcodes.

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
#include <arch/riscv/rv32i.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

const RV32I::OpCode RV32I::OpCode::ADD    { "add" };
const RV32I::OpCode RV32I::OpCode::ADDI   { "addi" };
const RV32I::OpCode RV32I::OpCode::AND    { "and" };
const RV32I::OpCode RV32I::OpCode::ANDI   { "andi" };
const RV32I::OpCode RV32I::OpCode::AUIPC  { "auipc" };
const RV32I::OpCode RV32I::OpCode::BEQ    { "beq", WIR_OpCodeType::condJump };
const RV32I::OpCode RV32I::OpCode::BGE    { "bge", WIR_OpCodeType::condJump };
const RV32I::OpCode RV32I::OpCode::BGEU   { "bgeu", WIR_OpCodeType::condJump };
const RV32I::OpCode RV32I::OpCode::BLT    { "blt", WIR_OpCodeType::condJump };
const RV32I::OpCode RV32I::OpCode::BLTU   { "bltu", WIR_OpCodeType::condJump };
const RV32I::OpCode RV32I::OpCode::BNE    { "bne", WIR_OpCodeType::condJump };
const RV32I::OpCode RV32I::OpCode::CSRRC  { "csrrc",
                                            WIR_OpCodeType::sideEffect };
const RV32I::OpCode RV32I::OpCode::CSRRCI { "csrrci",
                                            WIR_OpCodeType::sideEffect };
const RV32I::OpCode RV32I::OpCode::CSRRS  { "csrrs",
                                            WIR_OpCodeType::sideEffect };
const RV32I::OpCode RV32I::OpCode::CSRRSI { "csrrsi",
                                            WIR_OpCodeType::sideEffect };
const RV32I::OpCode RV32I::OpCode::CSRRW  { "csrrw",
                                            WIR_OpCodeType::sideEffect };
const RV32I::OpCode RV32I::OpCode::CSRRWI { "csrrwi",
                                            WIR_OpCodeType::sideEffect };
const RV32I::OpCode RV32I::OpCode::EBREAK { "ebreak",
                                            WIR_OpCodeType::sideEffect };
const RV32I::OpCode RV32I::OpCode::ECALL  { "ecall",
                                            WIR_OpCodeType::sideEffect };

// J is a pseudo-operation de facto realized by a JAL:
//   jal x0, <tgt>
const RV32I::OpCode RV32I::OpCode::J      { "jal", WIR_OpCodeType::uncondJump };
const RV32I::OpCode RV32I::OpCode::JAL    { "jal", WIR_OpCodeType::call };
const RV32I::OpCode RV32I::OpCode::JALR   { "jalr",
                                            WIR_OpCodeType::indirectCall };
const RV32I::OpCode RV32I::OpCode::JR     { "jalr",
                                            WIR_OpCodeType::indirectJump };
const RV32I::OpCode RV32I::OpCode::LB     { "lb", WIR_OpCodeType::load };
const RV32I::OpCode RV32I::OpCode::LBU    { "lbu", WIR_OpCodeType::load };
const RV32I::OpCode RV32I::OpCode::LH     { "lh", WIR_OpCodeType::load };
const RV32I::OpCode RV32I::OpCode::LHU    { "lhu", WIR_OpCodeType::load };
const RV32I::OpCode RV32I::OpCode::LUI    { "lui" };
const RV32I::OpCode RV32I::OpCode::LW     { "lw", WIR_OpCodeType::load };

// MOV is a pseudo-operation de facto realized by an ADDI:
//   addi $tgt, $src, 0
const RV32I::OpCode RV32I::OpCode::MOV    { "addi", WIR_OpCodeType::move };
const RV32I::OpCode RV32I::OpCode::OR     { "or" };
const RV32I::OpCode RV32I::OpCode::ORI    { "ori" };

// RET is a pseudo-operation de facto realized by a JALR:
//   jalr x0, x1, 0
const RV32I::OpCode RV32I::OpCode::RET    { "jalr", WIR_OpCodeType::ret };
const RV32I::OpCode RV32I::OpCode::SB     { "sb", WIR_OpCodeType::store };
const RV32I::OpCode RV32I::OpCode::SH     { "sh", WIR_OpCodeType::store };
const RV32I::OpCode RV32I::OpCode::SLL    { "sll" };
const RV32I::OpCode RV32I::OpCode::SLLI   { "slli" };
const RV32I::OpCode RV32I::OpCode::SLT    { "slt" };
const RV32I::OpCode RV32I::OpCode::SLTI   { "slti" };
const RV32I::OpCode RV32I::OpCode::SLTIU  { "sltiu" };
const RV32I::OpCode RV32I::OpCode::SLTU   { "sltu" };
const RV32I::OpCode RV32I::OpCode::SRA    { "sra" };
const RV32I::OpCode RV32I::OpCode::SRAI   { "srai" };
const RV32I::OpCode RV32I::OpCode::SRL    { "srl" };
const RV32I::OpCode RV32I::OpCode::SRLI   { "srli" };
const RV32I::OpCode RV32I::OpCode::SUB    { "sub" };
const RV32I::OpCode RV32I::OpCode::SW     { "sw", WIR_OpCodeType::store };
const RV32I::OpCode RV32I::OpCode::XOR    { "xor" };
const RV32I::OpCode RV32I::OpCode::XORI   { "xori" };

}       // namespace WIR
