/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armv6operationformats.cc
  @brief This file declares the ARMv6-specific operation formats.

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

// 32-bit ARM operation formats.
const ARMv6::OperationFormat ARMv6::OperationFormat::A_1 { 32, "A_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::A_2 { 32, "A_2" };
const ARMv6::OperationFormat ARMv6::OperationFormat::AC5_1 { 32, "AC5_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::AC5_2 { 32, "AC5_2" };
const ARMv6::OperationFormat ARMv6::OperationFormat::AC5_3 { 32, "AC5_3" };
const ARMv6::OperationFormat ARMv6::OperationFormat::AORRS_1 { 32, "AORRS_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::AORRS_2 { 32, "AORRS_2" };
const ARMv6::OperationFormat ARMv6::OperationFormat::AR_1 { 32, "AR_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::AR_2 { 32, "AR_2" };
const ARMv6::OperationFormat ARMv6::OperationFormat::C5_1 { 32, "C5_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::CRC4R_1 { 32, "CRC4R_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::CRC5SPR_1 { 32, "CRC5SPR_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::CRC60R_1 { 32, "CRC60R_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::CRC5RC5_1 { 32, "CRC5RC5_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::CRC5RC60_1 { 32, "CRC5RC60_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::CRC60RC5_1 { 32, "CRC60RC5_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::CRC60RC60_1 { 32, "CRC60RC60_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::CRR_7 { 32, "CRR_7" };
const ARMv6::OperationFormat ARMv6::OperationFormat::CRRA_1 { 32, "CRRA_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::CRRRA_1 { 32, "CRRRA_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::CRRRC5_3 { 32, "CRRRC5_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::CRRRR_5 { 32, "CRRRR_5" };

// 16-bit THUMB operation formats.
const ARMv6::OperationFormat ARMv6::OperationFormat::TA_1 { 16, "TA_1" };
const ARMv6::OperationFormat ARMv6::OperationFormat::TA_2 { 16, "TA_2" };

}       // namespace WIR
