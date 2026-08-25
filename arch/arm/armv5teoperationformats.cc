/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armv5teoperationformats.cc
  @brief This file declares the ARMv5TE-specific operation formats.

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

// 32-bit ARM operation formats.
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::CAORRS_1 { 32, "CAORRS_1" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::CAORRS_2 { 32, "CAORRS_2" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::CPARAC8_1 { 32, "CPARAC8_1" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::CPARAC8_2 { 32, "CPARAC8_2" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::CPARAR_1 { 32, "CPARAR_1" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::CPARAR_2 { 32, "CPARAR_2" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::CPRAC8_1 { 32, "CPRAC8_1" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::CPRAC8_2 { 32, "CPRAC8_2" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::CPRAR_1 { 32, "CPRAR_1" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::CPRAR_2 { 32, "CPRAR_2" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::RAC12_1 { 32, "RAC12_1" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::RAR_1 { 32, "RAR_1" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::RAR_2 { 32, "RAR_2" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::RARAC60_1 { 32, "RARAC60_1" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::RARC5_1 { 32, "RARC5_1" };
const ARMv5TE::OperationFormat ARMv5TE::OperationFormat::RARC50_1 { 32, "RARC50_1" };

}       // namespace WIR
