/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32icoperationformats.cc
  @brief This file declares the RISC-V RV32IC's operation formats.

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
#include <arch/riscv/rv32ic.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

const RV32IC::OperationFormat RV32IC::OperationFormat::SL_1 { 16, "SL_1" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SNULL_1 { 16, "SNULL_1" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SR_1 { 16, "SR_1" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SRC5_1 { 16, "SRC5_1" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SRC7R_1 { 16, "SRC7R_1" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SRC7R_2 { 16, "SRC7R_2" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SRC6_1 { 16, "SRC6_1" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SRC6_2 { 16, "SRC6_2" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SRC8R_1 { 16, "SRC8R_1" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SRC8R_2 { 16, "SRC8R_2" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SRC10_1 { 16, "SRC10_1" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SRL_1 { 16, "SRL_1" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SRR_1 { 16, "SRR_1" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SRR_2 { 16, "SRR_2" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SRRC10_1 { 16, "SRRC10_1" };
const RV32IC::OperationFormat RV32IC::OperationFormat::SRC20_1 { 16, "SRC20_1" };

}       // namespace WIR
