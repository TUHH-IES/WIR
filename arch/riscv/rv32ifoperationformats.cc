/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32ifopcodes.cc
  @brief This file declares the RISC-V RV32IF V2.0's opcodes.

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

const RV32IF::OperationFormat RV32IF::OperationFormat::FC12R_1 { 32, "FC12R_1" };
const RV32IF::OperationFormat RV32IF::OperationFormat::FC12R_2 { 32, "FC12R_2" };
const RV32IF::OperationFormat RV32IF::OperationFormat::FF_1 { 32, "FF_1" };
const RV32IF::OperationFormat RV32IF::OperationFormat::FFF_1 { 32, "FFF_1" };
const RV32IF::OperationFormat RV32IF::OperationFormat::FFFF_1 { 32, "FFFF_1" };
const RV32IF::OperationFormat RV32IF::OperationFormat::FLR_1 { 32, "FLR_1" };
const RV32IF::OperationFormat RV32IF::OperationFormat::FLR_2 { 32, "FLR_2" };
const RV32IF::OperationFormat RV32IF::OperationFormat::FR_1 { 32, "FR_1" };
const RV32IF::OperationFormat RV32IF::OperationFormat::FRS_1 { 32, "FRS_1" };
const RV32IF::OperationFormat RV32IF::OperationFormat::RF_1 { 32, "RF_1" };
const RV32IF::OperationFormat RV32IF::OperationFormat::RFF_1 { 32, "RFF_1" };
const RV32IF::OperationFormat RV32IF::OperationFormat::RFS_1 { 32, "RFS_1" };

}       // namespace WIR
