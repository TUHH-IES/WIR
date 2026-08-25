/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armv5tejopcodes.cc
  @brief This file declares the ARMv5TEJ-specific opcodes.

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
#include <arch/arm/armv5tej.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

// Opcodes
const ARMv5TEJ::OpCode ARMv5TEJ::OpCode::BXJ    { "bxj",
                                                  WIR_OpCodeType::indirectJump };

}       // namespace WIR
