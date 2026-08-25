/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armopcodes.cc
  @brief This file declares the ARM opcodes common to all architecture versions.

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
#include <arch/arm/armv4.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

// Opcodes
const ARMv4::OpCode ARMv4::OpCode::ADC    { "adc" };
const ARMv4::OpCode ARMv4::OpCode::ADD    { "add" };
const ARMv4::OpCode ARMv4::OpCode::AND    { "and" };
const ARMv4::OpCode ARMv4::OpCode::B      { "b", WIR_OpCodeType::condJump,
                                            WIR_OpCodeType::uncondJump };
const ARMv4::OpCode ARMv4::OpCode::BL     { "bl", WIR_OpCodeType::call };
const ARMv4::OpCode ARMv4::OpCode::BIC    { "bic" };
const ARMv4::OpCode ARMv4::OpCode::CDP    { "cdp" };
const ARMv4::OpCode ARMv4::OpCode::CMN    { "cmn" };
const ARMv4::OpCode ARMv4::OpCode::CMP    { "cmp" };
const ARMv4::OpCode ARMv4::OpCode::EOR    { "eor" };
const ARMv4::OpCode ARMv4::OpCode::LDC    { "ldc", WIR_OpCodeType::load };
const ARMv4::OpCode ARMv4::OpCode::LDM    { "ldm", WIR_OpCodeType::load };
const ARMv4::OpCode ARMv4::OpCode::LDR    { "ldr", WIR_OpCodeType::load };
const ARMv4::OpCode ARMv4::OpCode::LDRB   { "ldrb", WIR_OpCodeType::load };
const ARMv4::OpCode ARMv4::OpCode::LDRBT  { "ldrbt", WIR_OpCodeType::load };
const ARMv4::OpCode ARMv4::OpCode::LDRH   { "ldrh", WIR_OpCodeType::load };
const ARMv4::OpCode ARMv4::OpCode::LDRSB  { "ldrsb", WIR_OpCodeType::load };
const ARMv4::OpCode ARMv4::OpCode::LDRSH  { "ldrsh", WIR_OpCodeType::load };
const ARMv4::OpCode ARMv4::OpCode::LDRT   { "ldrt", WIR_OpCodeType::load };
const ARMv4::OpCode ARMv4::OpCode::MCR    { "mcr" };
const ARMv4::OpCode ARMv4::OpCode::MLA    { "mla" };
const ARMv4::OpCode ARMv4::OpCode::MOV    { "mov" };
const ARMv4::OpCode ARMv4::OpCode::MRC    { "mrc" };
const ARMv4::OpCode ARMv4::OpCode::MRS    { "mrs" };
const ARMv4::OpCode ARMv4::OpCode::MSR    { "msr" };
const ARMv4::OpCode ARMv4::OpCode::MUL    { "mul" };
const ARMv4::OpCode ARMv4::OpCode::MVN    { "mvn" };
const ARMv4::OpCode ARMv4::OpCode::ORR    { "orr" };
const ARMv4::OpCode ARMv4::OpCode::RSB    { "rsb" };
const ARMv4::OpCode ARMv4::OpCode::RSC    { "rsc" };
const ARMv4::OpCode ARMv4::OpCode::SBC    { "sbc" };
const ARMv4::OpCode ARMv4::OpCode::SMLAL  { "smlal" };
const ARMv4::OpCode ARMv4::OpCode::SMULL  { "smull" };
const ARMv4::OpCode ARMv4::OpCode::STC    { "stc", WIR_OpCodeType::store };
const ARMv4::OpCode ARMv4::OpCode::STM    { "stm", WIR_OpCodeType::store };
const ARMv4::OpCode ARMv4::OpCode::STR    { "str", WIR_OpCodeType::store };
const ARMv4::OpCode ARMv4::OpCode::STRB   { "strb", WIR_OpCodeType::store };
const ARMv4::OpCode ARMv4::OpCode::STRBT  { "strbt", WIR_OpCodeType::store };
const ARMv4::OpCode ARMv4::OpCode::STRH   { "strh", WIR_OpCodeType::store };
const ARMv4::OpCode ARMv4::OpCode::STRT   { "strt", WIR_OpCodeType::store };
const ARMv4::OpCode ARMv4::OpCode::SUB    { "sub" };
const ARMv4::OpCode ARMv4::OpCode::SWI    { "swi",
                                            WIR_OpCodeType::indirectCall };
const ARMv4::OpCode ARMv4::OpCode::SWP    { "swp", WIR_OpCodeType::load,
                                            WIR_OpCodeType::store };
const ARMv4::OpCode ARMv4::OpCode::SWPB   { "swpb", WIR_OpCodeType::load,
                                            WIR_OpCodeType::store };
const ARMv4::OpCode ARMv4::OpCode::TEQ    { "teq" };
const ARMv4::OpCode ARMv4::OpCode::TST    { "tst" };
const ARMv4::OpCode ARMv4::OpCode::UMLAL  { "umlal" };
const ARMv4::OpCode ARMv4::OpCode::UMULL  { "umull" };

}       // namespace WIR
