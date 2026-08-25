/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32ifregistertypes.cc
  @brief This file declares the RISC-V RV32IF's register types.

  @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32if.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

RV32IF::RegisterType RV32IF::RegisterType::fReg { "f", "f_", "", "", 32 };

}       // namespace WIR
