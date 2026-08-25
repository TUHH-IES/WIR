/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armv4topcodes.cc
  @brief This file declares the ARMv4T-specific opcodes.

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
#include <arch/arm/armv4t.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

// Opcodes
const ARMv4T::OpCode ARMv4T::OpCode::ASR        { "asr" };
const ARMv4T::OpCode ARMv4T::OpCode::BX         { "bx",
                                                  WIR_OpCodeType::indirectJump };
const ARMv4T::OpCode ARMv4T::OpCode::LDMIA      { "ldmia",
                                                  WIR_OpCodeType::load };
const ARMv4T::OpCode ARMv4T::OpCode::LSL        { "lsl" };
const ARMv4T::OpCode ARMv4T::OpCode::LSR        { "lsr" };
const ARMv4T::OpCode ARMv4T::OpCode::NEG        { "neg" };
const ARMv4T::OpCode ARMv4T::OpCode::POP        { "pop", WIR_OpCodeType::load };
const ARMv4T::OpCode ARMv4T::OpCode::PUSH       { "push",
                                                  WIR_OpCodeType::store };
const ARMv4T::OpCode ARMv4T::OpCode::ROR        { "ror" };
const ARMv4T::OpCode ARMv4T::OpCode::STMIA      { "stmia",
                                                  WIR_OpCodeType::store };

}       // namespace WIR
