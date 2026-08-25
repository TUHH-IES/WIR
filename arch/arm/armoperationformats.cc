/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armoperationformats.cc
  @brief This file declares the ARM's general operation formats.

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
#include <arch/arm/armv4.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

// 32-bit ARM operation formats.
const ARMv4::OperationFormat ARMv4::OperationFormat::CAAAAC8RA_1 { 32, "CAAAAC8RA_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CAAAAC8RA_2 { 32, "CAAAAC8RA_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CAAAAR_1 { 32, "CAAAAR_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CAAAAR_2 { 32, "CAAAAR_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CAORSSO_1 { 32, "CAORSSO_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CAORSSO_2 { 32, "CAORSSO_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CAOSSSO { 32, "CAOSSSO" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR1_1 { 32, "CARR1_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR1_2 { 32, "CARR1_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR1_3 { 32, "CARR1_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR1_4 { 32, "CARR1_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR1_5 { 32, "CARR1_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR1_6 { 32, "CARR1_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR1_7 { 32, "CARR1_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR1_8 { 32, "CARR1_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR2_1 { 32, "CARR2_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR2_2 { 32, "CARR2_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR2_3 { 32, "CARR2_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR2_4 { 32, "CARR2_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR2_5 { 32, "CARR2_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR2_6 { 32, "CARR2_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR2_7 { 32, "CARR2_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR2_8 { 32, "CARR2_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR3_1 { 32, "CARR3_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR3_2 { 32, "CARR3_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR3_3 { 32, "CARR3_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR3_4 { 32, "CARR3_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR3_5 { 32, "CARR3_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR3_6 { 32, "CARR3_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR3_7 { 32, "CARR3_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR3_8 { 32, "CARR3_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR4_1 { 32, "CARR4_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR4_2 { 32, "CARR4_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR4_3 { 32, "CARR4_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR4_4 { 32, "CARR4_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR4_5 { 32, "CARR4_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR4_6 { 32, "CARR4_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR4_7 { 32, "CARR4_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR4_8 { 32, "CARR4_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR5_1 { 32, "CARR5_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR5_2 { 32, "CARR5_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR5_3 { 32, "CARR5_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR5_4 { 32, "CARR5_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR5_5 { 32, "CARR5_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR5_6 { 32, "CARR5_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR5_7 { 32, "CARR5_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR5_8 { 32, "CARR5_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR6_1 { 32, "CARR6_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR6_2 { 32, "CARR6_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR6_3 { 32, "CARR6_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR6_4 { 32, "CARR6_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR6_5 { 32, "CARR6_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR6_6 { 32, "CARR6_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR6_7 { 32, "CARR6_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR6_8 { 32, "CARR6_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR7_1 { 32, "CARR7_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR7_2 { 32, "CARR7_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR7_3 { 32, "CARR7_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR7_4 { 32, "CARR7_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR7_5 { 32, "CARR7_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR7_6 { 32, "CARR7_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR7_7 { 32, "CARR7_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR7_8 { 32, "CARR7_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR8_1 { 32, "CARR8_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR8_2 { 32, "CARR8_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR8_3 { 32, "CARR8_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR8_4 { 32, "CARR8_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR8_5 { 32, "CARR8_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR8_6 { 32, "CARR8_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR8_7 { 32, "CARR8_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR8_8 { 32, "CARR8_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR9_1 { 32, "CARR9_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR9_2 { 32, "CARR9_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR9_3 { 32, "CARR9_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR9_4 { 32, "CARR9_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR9_5 { 32, "CARR9_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR9_6 { 32, "CARR9_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR9_7 { 32, "CARR9_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR9_8 { 32, "CARR9_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR10_1 { 32, "CARR10_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR10_2 { 32, "CARR10_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR10_3 { 32, "CARR10_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR10_4 { 32, "CARR10_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR10_5 { 32, "CARR10_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR10_6 { 32, "CARR10_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR10_7 { 32, "CARR10_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR10_8 { 32, "CARR10_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR11_1 { 32, "CARR11_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR11_2 { 32, "CARR11_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR11_3 { 32, "CARR11_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR11_4 { 32, "CARR11_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR11_5 { 32, "CARR11_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR11_6 { 32, "CARR11_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR11_7 { 32, "CARR11_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR11_8 { 32, "CARR11_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR12_1 { 32, "CARR12_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR12_2 { 32, "CARR12_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR12_3 { 32, "CARR12_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR12_4 { 32, "CARR12_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR12_5 { 32, "CARR12_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR12_6 { 32, "CARR12_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR12_7 { 32, "CARR12_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR12_8 { 32, "CARR12_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR13_1 { 32, "CARR13_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR13_2 { 32, "CARR13_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR13_3 { 32, "CARR13_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR13_4 { 32, "CARR13_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR13_5 { 32, "CARR13_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR13_6 { 32, "CARR13_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR13_7 { 32, "CARR13_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR13_8 { 32, "CARR13_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR14_1 { 32, "CARR14_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR14_2 { 32, "CARR14_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR14_3 { 32, "CARR14_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR14_4 { 32, "CARR14_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR14_5 { 32, "CARR14_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR14_6 { 32, "CARR14_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR14_7 { 32, "CARR14_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR14_8 { 32, "CARR14_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR15_1 { 32, "CARR15_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR15_2 { 32, "CARR15_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR15_3 { 32, "CARR15_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR15_4 { 32, "CARR15_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR15_5 { 32, "CARR15_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR15_6 { 32, "CARR15_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR15_7 { 32, "CARR15_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR15_8 { 32, "CARR15_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR16_1 { 32, "CARR16_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR16_2 { 32, "CARR16_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR16_3 { 32, "CARR16_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR16_4 { 32, "CARR16_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR16_6 { 32, "CARR16_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR16_7 { 32, "CARR16_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CARR16_8 { 32, "CARR16_8" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CASARAC10_1 { 32, "CASARAC10_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CASARAC10_2 { 32, "CASARAC10_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CASRAC10_1 { 32, "CASRAC10_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CASRAC10_2 { 32, "CASRAC10_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CASRC8_1 { 32, "CASRC8_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CASRC8_2 { 32, "CASRC8_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CC24 { 32, "CC24" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CL { 32, "CL" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CR_1 { 32, "CR_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CR_2 { 32, "CR_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARAC12_1 { 32, "CRARAC12_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARAC12_2 { 32, "CRARAC12_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARAC8_1 { 32, "CRARAC8_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARAC8_2 { 32, "CRARAC8_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARAR_1 { 32, "CRARAR_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARAR_2 { 32, "CRARAR_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARAR_3 { 32, "CRARAR_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARAR_4 { 32, "CRARAR_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARARAC60_1 { 32, "CRARARAC60_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARARAC60_2 { 32, "CRARARAC60_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARARC50_1 { 32, "CRARARC50_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARARC50_2 { 32, "CRARARC50_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARARC5_1 { 32, "CRARARC5_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRARARC5_2 { 32, "CRARARC5_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRC8RA_1 { 32, "CRC8RA_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRC8RA_2 { 32, "CRC8RA_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRC8RA_3 { 32, "CRC8RA_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRR_1 { 32, "CRR_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRR_2 { 32, "CRR_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRR_3 { 32, "CRR_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRR_4 { 32, "CRR_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRR_5 { 32, "CRR_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRR_6 { 32, "CRR_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAC12_1 { 32, "CRRAC12_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAC12_2 { 32, "CRRAC12_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAC60_1 { 32, "CRRAC60_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAC60_2 { 32, "CRRAC60_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAC60_3 { 32, "CRRAC60_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAC8_1 { 32, "CRRAC8_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAC8_2 { 32, "CRRAC8_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAR_1 { 32, "CRRAR_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAR_2 { 32, "CRRAR_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAR_3 { 32, "CRRAR_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAR_4 { 32, "CRRAR_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAR_5 { 32, "CRRAR_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAR_6 { 32, "CRRAR_6" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRAR_7 { 32, "CRRAR_7" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRARAC60_1 { 32, "CRRARAC60_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRARAC60_2 { 32, "CRRARAC60_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRARC50_1 { 32, "CRRARC50_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRARC50_2 { 32, "CRRARC50_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRARC5_1 { 32, "CRRARC5_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRARC5_2 { 32, "CRRARC5_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRC50_1 { 32, "CRRC50_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRC50_2 { 32, "CRRC50_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRC50_3 { 32, "CRRC50_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRC5_1 { 32, "CRRC5_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRC5_2 { 32, "CRRC5_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRC5_3 { 32, "CRRC5_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRC8RA_1 { 32, "CRRC8RA_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRC8RA_2 { 32, "CRRC8RA_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRR_1 { 32, "CRRR_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRR_2 { 32, "CRRR_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRR_3 { 32, "CRRR_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRR_4 { 32, "CRRR_4" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRR_5 { 32, "CRRR_5" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRRAC60_1 { 32, "CRRRAC60_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRRAC60_2 { 32, "CRRRAC60_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRRAR_1 { 32, "CRRRAR_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRRAR_2 { 32, "CRRRAR_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRRC50_1 { 32, "CRRRC50_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRRC50_2 { 32, "CRRRC50_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRRC5_1 { 32, "CRRRC5_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRRC5_2 { 32, "CRRRC5_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRRR_1 { 32, "CRRRR_1" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRRR_2 { 32, "CRRRR_2" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRRR_3 { 32, "CRRRR_3" };
const ARMv4::OperationFormat ARMv4::OperationFormat::CRRRR_4 { 32, "CRRRR_4" };

}       // namespace WIR
