/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armv6opcodes.cc
  @brief This file declares the ARMv6-specific opcodes.

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
#include <arch/arm/armv6.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

// Opcodes
const ARMv6::OpCode ARMv6::OpCode::CPS          { "cps" };
const ARMv6::OpCode ARMv6::OpCode::CPSID        { "cpsid" };
const ARMv6::OpCode ARMv6::OpCode::CPSIE        { "cpsie" };
const ARMv6::OpCode ARMv6::OpCode::CPY          { "cpy", WIR_OpCodeType::move };
const ARMv6::OpCode ARMv6::OpCode::LDREX        { "ldrex",
                                                  WIR_OpCodeType:: load };
const ARMv6::OpCode ARMv6::OpCode::MCRR2        { "mcrr2" };
const ARMv6::OpCode ARMv6::OpCode::MRRC2        { "mrrc2" };
const ARMv6::OpCode ARMv6::OpCode::PKHBT        { "pkhbt" };
const ARMv6::OpCode ARMv6::OpCode::PKHTB        { "pkhtb" };
const ARMv6::OpCode ARMv6::OpCode::QADD16       { "qadd16" };
const ARMv6::OpCode ARMv6::OpCode::QADD8        { "qadd8" };
const ARMv6::OpCode ARMv6::OpCode::QADDSUBX     { "qaddsubx" };
const ARMv6::OpCode ARMv6::OpCode::QSUB16       { "qsub16" };
const ARMv6::OpCode ARMv6::OpCode::QSUB8        { "qsub8" };
const ARMv6::OpCode ARMv6::OpCode::QSUBADDX     { "qsubaddx" };
const ARMv6::OpCode ARMv6::OpCode::REV          { "rev" };
const ARMv6::OpCode ARMv6::OpCode::REV16        { "rev16" };
const ARMv6::OpCode ARMv6::OpCode::REVSH        { "revsh" };
const ARMv6::OpCode ARMv6::OpCode::RFE          { "rfe", WIR_OpCodeType:: ret };
const ARMv6::OpCode ARMv6::OpCode::SADD16       { "sadd16" };
const ARMv6::OpCode ARMv6::OpCode::SADD8        { "sadd8" };
const ARMv6::OpCode ARMv6::OpCode::SADDSUBX     { "saddsubx" };
const ARMv6::OpCode ARMv6::OpCode::SEL          { "sel" };
const ARMv6::OpCode ARMv6::OpCode::SETEND       { "setend" };
const ARMv6::OpCode ARMv6::OpCode::SHADD16      { "shadd16" };
const ARMv6::OpCode ARMv6::OpCode::SHADD8       { "shadd8" };
const ARMv6::OpCode ARMv6::OpCode::SHADDSUBX    { "shaddsubx" };
const ARMv6::OpCode ARMv6::OpCode::SHSUB16      { "shsub16" };
const ARMv6::OpCode ARMv6::OpCode::SHSUB8       { "shsub8" };
const ARMv6::OpCode ARMv6::OpCode::SHSUBADDX    { "shsubaddx" };
const ARMv6::OpCode ARMv6::OpCode::SMLAD        { "smlad" };
const ARMv6::OpCode ARMv6::OpCode::SMLADX       { "smladx" };
const ARMv6::OpCode ARMv6::OpCode::SMLALD       { "smlald" };
const ARMv6::OpCode ARMv6::OpCode::SMLALDX      { "smlaldx" };
const ARMv6::OpCode ARMv6::OpCode::SMLSD        { "smlsd" };
const ARMv6::OpCode ARMv6::OpCode::SMLSDX       { "smlsdx" };
const ARMv6::OpCode ARMv6::OpCode::SMLSLD       { "smlsld" };
const ARMv6::OpCode ARMv6::OpCode::SMLSLDX      { "smlsldx" };
const ARMv6::OpCode ARMv6::OpCode::SMMLA        { "smmla" };
const ARMv6::OpCode ARMv6::OpCode::SMMLAR       { "smmlar" };
const ARMv6::OpCode ARMv6::OpCode::SMMLS        { "smmls" };
const ARMv6::OpCode ARMv6::OpCode::SMMLSR       { "smmlsr" };
const ARMv6::OpCode ARMv6::OpCode::SMMUL        { "smmul" };
const ARMv6::OpCode ARMv6::OpCode::SMMULR       { "smmulr" };
const ARMv6::OpCode ARMv6::OpCode::SMUAD        { "smuad" };
const ARMv6::OpCode ARMv6::OpCode::SMUADX       { "smuadx" };
const ARMv6::OpCode ARMv6::OpCode::SMUSD        { "smusd" };
const ARMv6::OpCode ARMv6::OpCode::SMUSDX       { "smusdx" };
const ARMv6::OpCode ARMv6::OpCode::SRS          { "srs",
                                                  WIR_OpCodeType:: store };
const ARMv6::OpCode ARMv6::OpCode::SSAT         { "ssat" };
const ARMv6::OpCode ARMv6::OpCode::SSAT16       { "ssat16" };
const ARMv6::OpCode ARMv6::OpCode::SSUB16       { "ssub16" };
const ARMv6::OpCode ARMv6::OpCode::SSUB8        { "ssub8" };
const ARMv6::OpCode ARMv6::OpCode::SSUBADDX     { "ssubaddx" };
const ARMv6::OpCode ARMv6::OpCode::STREX        { "strex",
                                                  WIR_OpCodeType:: store };
const ARMv6::OpCode ARMv6::OpCode::SXTAB        { "sxtab" };
const ARMv6::OpCode ARMv6::OpCode::SXTAB16      { "sxtab16" };
const ARMv6::OpCode ARMv6::OpCode::SXTAH        { "sxtah" };
const ARMv6::OpCode ARMv6::OpCode::SXTB         { "sxtb" };
const ARMv6::OpCode ARMv6::OpCode::SXTB16       { "sxtb16" };
const ARMv6::OpCode ARMv6::OpCode::SXTH         { "sxth" };
const ARMv6::OpCode ARMv6::OpCode::UADD16       { "uadd16" };
const ARMv6::OpCode ARMv6::OpCode::UADD8        { "uadd8" };
const ARMv6::OpCode ARMv6::OpCode::UADDSUBX     { "uaddsubx" };
const ARMv6::OpCode ARMv6::OpCode::UHADD16      { "uhadd16" };
const ARMv6::OpCode ARMv6::OpCode::UHADD8       { "uhadd8" };
const ARMv6::OpCode ARMv6::OpCode::UHADDSUBX    { "uhaddsubx" };
const ARMv6::OpCode ARMv6::OpCode::UHSUB16      { "uhsub16" };
const ARMv6::OpCode ARMv6::OpCode::UHSUB8       { "uhsub8" };
const ARMv6::OpCode ARMv6::OpCode::UHSUBADDX    { "uhsubaddx" };
const ARMv6::OpCode ARMv6::OpCode::UMAAL        { "umaal" };
const ARMv6::OpCode ARMv6::OpCode::UQADD16      { "uqadd16" };
const ARMv6::OpCode ARMv6::OpCode::UQADD8       { "uqadd8" };
const ARMv6::OpCode ARMv6::OpCode::UQADDSUBX    { "uqaddsubx" };
const ARMv6::OpCode ARMv6::OpCode::UQSUB16      { "uqsub16" };
const ARMv6::OpCode ARMv6::OpCode::UQSUB8       { "uqsub8" };
const ARMv6::OpCode ARMv6::OpCode::UQSUBADDX    { "uqsubaddx" };
const ARMv6::OpCode ARMv6::OpCode::USAD8        { "usad8" };
const ARMv6::OpCode ARMv6::OpCode::USADA8       { "usada8" };
const ARMv6::OpCode ARMv6::OpCode::USAT         { "usat" };
const ARMv6::OpCode ARMv6::OpCode::USAT16       { "usat16" };
const ARMv6::OpCode ARMv6::OpCode::USUB16       { "usub16" };
const ARMv6::OpCode ARMv6::OpCode::USUB8        { "usub8" };
const ARMv6::OpCode ARMv6::OpCode::USUBADDX     { "usubaddx" };
const ARMv6::OpCode ARMv6::OpCode::UXTAB        { "uxtab" };
const ARMv6::OpCode ARMv6::OpCode::UXTAB16      { "uxtab16" };
const ARMv6::OpCode ARMv6::OpCode::UXTAH        { "uxtah" };
const ARMv6::OpCode ARMv6::OpCode::UXTB         { "uxtb" };
const ARMv6::OpCode ARMv6::OpCode::UXTB16       { "uxtb16" };
const ARMv6::OpCode ARMv6::OpCode::UXTH         { "uxth" };

}       // namespace WIR
