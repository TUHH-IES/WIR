/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armv5topcodes.cc
  @brief This file declares the ARMv5T-specific opcodes.

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
#include <arch/arm/armv5t.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

// Opcodes
const ARMv5T::OpCode ARMv5T::OpCode::BKPT       { "bkpt",
                                                  WIR_OpCodeType::indirectJump };
const ARMv5T::OpCode ARMv5T::OpCode::BLX        { "blx", WIR_OpCodeType::call };
const ARMv5T::OpCode ARMv5T::OpCode::CDP2       { "cdp2" };
const ARMv5T::OpCode ARMv5T::OpCode::CLZ        { "clz" };
const ARMv5T::OpCode ARMv5T::OpCode::LDC2       { "ldc2",
                                                  WIR_OpCodeType::load };
const ARMv5T::OpCode ARMv5T::OpCode::MCR2       { "mcr2" };
const ARMv5T::OpCode ARMv5T::OpCode::STC2       { "stc2",
                                                  WIR_OpCodeType::store };

}       // namespace WIR
