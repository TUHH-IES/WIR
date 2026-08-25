/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32iregistertypes.cc
  @brief This file declares the RISC-V RV32I's register types.

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

// Register types
const RV32I::RegisterType RV32I::RegisterType::reg { "x", "x_", "", "", 32 };
const RV32I::RegisterType RV32I::RegisterType::eReg { "x", "e_", "", "", 64 };
const RV32I::RegisterType RV32I::RegisterType::qReg { "x", "q_", "", "", 128 };

}       // namespace WIR
