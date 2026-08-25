/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armv5teopcodes.cc
  @brief This file declares the ARMv5TE-specific opcodes.

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
#include <arch/arm/armv5te.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

// Opcodes
const ARMv5TE::OpCode ARMv5TE::OpCode::LDRD     { "ldrd",
                                                  WIR_OpCodeType::load };
const ARMv5TE::OpCode ARMv5TE::OpCode::MCRR     { "mcrr" };
const ARMv5TE::OpCode ARMv5TE::OpCode::MRRC     { "mrrc" };
const ARMv5TE::OpCode ARMv5TE::OpCode::PLD      { "pld" };
const ARMv5TE::OpCode ARMv5TE::OpCode::QADD     { "qadd" };
const ARMv5TE::OpCode ARMv5TE::OpCode::QDADD    { "qdadd" };
const ARMv5TE::OpCode ARMv5TE::OpCode::QDSUB    { "qdsub" };
const ARMv5TE::OpCode ARMv5TE::OpCode::QSUB     { "qsub" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMLABB   { "smlabb" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMLABT   { "smlabt" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMLATB   { "smlatb" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMLATT   { "smlatt" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMLALBB  { "smlalbb" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMLALBT  { "smlalbt" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMLALTB  { "smlaltb" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMLALTT  { "smlaltt" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMLAWB   { "smlawb" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMLAWT   { "smlawt" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMULBB   { "smulbb" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMULBT   { "smulbt" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMULTB   { "smultb" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMULTT   { "smultt" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMULWB   { "smulwb" };
const ARMv5TE::OpCode ARMv5TE::OpCode::SMULWT   { "smulwt" };
const ARMv5TE::OpCode ARMv5TE::OpCode::STRD     { "strd",
                                                  WIR_OpCodeType::store };

}       // namespace WIR
