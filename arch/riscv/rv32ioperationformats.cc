/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32ioperationformats.cc
  @brief This file declares the RISC-V RV32I's operation formats.

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

const RV32I::OperationFormat RV32I::OperationFormat::L_1 { 32, "L_1" };
const RV32I::OperationFormat RV32I::OperationFormat::NULL_1 { 32, "NULL_1" };
const RV32I::OperationFormat RV32I::OperationFormat::R_1 { 32, "R_1" };
const RV32I::OperationFormat RV32I::OperationFormat::RC12R_1 { 32, "RC12R_1" };
const RV32I::OperationFormat RV32I::OperationFormat::RC12R_2 { 32, "RC12R_2" };
const RV32I::OperationFormat RV32I::OperationFormat::RC20_1 { 32, "RC20_1" };
const RV32I::OperationFormat RV32I::OperationFormat::RL_1 { 32, "RL_1" };
const RV32I::OperationFormat RV32I::OperationFormat::RL_2 { 32, "RL_2" };
const RV32I::OperationFormat RV32I::OperationFormat::RLR_1 { 32, "RLR_1" };
const RV32I::OperationFormat RV32I::OperationFormat::RLR_2 { 32, "RLR_2" };
const RV32I::OperationFormat RV32I::OperationFormat::RRC5_1 { 32, "RRC5_1" };
const RV32I::OperationFormat RV32I::OperationFormat::RRC12_1 { 32, "RRC12_1" };
const RV32I::OperationFormat RV32I::OperationFormat::RRL_1 { 32, "RRL_1" };
const RV32I::OperationFormat RV32I::OperationFormat::RRL_2 { 32, "RRL_2" };
const RV32I::OperationFormat RV32I::OperationFormat::RR_1 { 32, "RR_1" };
const RV32I::OperationFormat RV32I::OperationFormat::RRR_1 { 32, "RRR_1" };
const RV32I::OperationFormat RV32I::OperationFormat::RSC5_1 { 32, "RSC5_1" };
const RV32I::OperationFormat RV32I::OperationFormat::RSR_1 { 32, "RSR_1" };

}       // namespace WIR
