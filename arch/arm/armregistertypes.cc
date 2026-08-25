/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armregistertypes.cc
  @brief This file declares the ARM register types.

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

// Register types
const ARMv4::RegisterType ARMv4::RegisterType::reg { "r", "r_", "", "", 32 };
const ARMv4::RegisterType ARMv4::RegisterType::lo { "r", "l_", "", "", 32 };
const ARMv4::RegisterType ARMv4::RegisterType::hi { "r", "h_", "", "", 32 };

}       // namespace WIR
