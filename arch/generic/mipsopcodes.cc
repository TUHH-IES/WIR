/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file mipsopcodes.cc
  @brief This file declares the MIPS opcodes.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/generic/mips.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

// Opcodes
const MIPS::OpCode MIPS::OpCode::ADD            { "add" };
const MIPS::OpCode MIPS::OpCode::ADDI           { "addi" };
const MIPS::OpCode MIPS::OpCode::ADDIU          { "addiu" };
const MIPS::OpCode MIPS::OpCode::ADDU           { "addu" };
const MIPS::OpCode MIPS::OpCode::AND            { "and" };
const MIPS::OpCode MIPS::OpCode::ANDI           { "andi" };
const MIPS::OpCode MIPS::OpCode::BEQ            { "beq",
                                                  WIR_OpCodeType::condJump };
const MIPS::OpCode MIPS::OpCode::BNE            { "bne",
                                                  WIR_OpCodeType::condJump };
const MIPS::OpCode MIPS::OpCode::DIV            { "div" };
const MIPS::OpCode MIPS::OpCode::DIVU           { "divu" };
const MIPS::OpCode MIPS::OpCode::J              { "j",
                                                  WIR_OpCodeType::uncondJump };
const MIPS::OpCode MIPS::OpCode::JAL            { "jal", WIR_OpCodeType::call };
const MIPS::OpCode MIPS::OpCode::JR             { "jr",
                                                  WIR_OpCodeType::ret,
                                                  WIR_OpCodeType::uncondJump,
                                                  WIR_OpCodeType::indirectJump };
const MIPS::OpCode MIPS::OpCode::JALR           { "jalr",
                                                  WIR_OpCodeType::call,
                                                  WIR_OpCodeType::indirectCall };
const MIPS::OpCode MIPS::OpCode::LB             { "lb", WIR_OpCodeType::load };
const MIPS::OpCode MIPS::OpCode::LBU            { "lbu", WIR_OpCodeType::load };
const MIPS::OpCode MIPS::OpCode::LH             { "lh", WIR_OpCodeType::load };
const MIPS::OpCode MIPS::OpCode::LHU            { "lhu", WIR_OpCodeType::load };
const MIPS::OpCode MIPS::OpCode::LUI            { "lui" };
const MIPS::OpCode MIPS::OpCode::LW             { "lw", WIR_OpCodeType::load };
const MIPS::OpCode MIPS::OpCode::MFCO           { "mfco" };
const MIPS::OpCode MIPS::OpCode::MFHI           { "mfhi" };
const MIPS::OpCode MIPS::OpCode::MFLO           { "mflo" };
const MIPS::OpCode MIPS::OpCode::MULT           { "mult" };
const MIPS::OpCode MIPS::OpCode::MULTU          { "multu" };
const MIPS::OpCode MIPS::OpCode::NOR            { "nor" };
const MIPS::OpCode MIPS::OpCode::OR             { "or" };
const MIPS::OpCode MIPS::OpCode::ORI            { "ori" };
const MIPS::OpCode MIPS::OpCode::SB             { "sb", WIR_OpCodeType::store };
const MIPS::OpCode MIPS::OpCode::SH             { "sh", WIR_OpCodeType::store };
const MIPS::OpCode MIPS::OpCode::SLL            { "sll" };
const MIPS::OpCode MIPS::OpCode::SLLV           { "sllv" };
const MIPS::OpCode MIPS::OpCode::SLT            { "slt" };
const MIPS::OpCode MIPS::OpCode::SLTI           { "slti" };
const MIPS::OpCode MIPS::OpCode::SLTIU          { "sltiu" };
const MIPS::OpCode MIPS::OpCode::SLTU           { "sltu" };
const MIPS::OpCode MIPS::OpCode::SRA            { "sra" };
const MIPS::OpCode MIPS::OpCode::SRAV           { "srav" };
const MIPS::OpCode MIPS::OpCode::SRL            { "srl" };
const MIPS::OpCode MIPS::OpCode::SRLV           { "srlv" };
const MIPS::OpCode MIPS::OpCode::SUB            { "sub" };
const MIPS::OpCode MIPS::OpCode::SUBU           { "subu" };
const MIPS::OpCode MIPS::OpCode::SW             { "sw", WIR_OpCodeType::store };
const MIPS::OpCode MIPS::OpCode::XOR            { "xor" };
const MIPS::OpCode MIPS::OpCode::XORI           { "xori" };

}       // namespace WIR
