/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tc13registertypes.cc
  @brief This file declares the Infineon TriCore TC13's register types.

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
#include <arch/tricore/tc13.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

// Register types
const TC13::RegisterType TC13::RegisterType::aReg { "a", "a_", "", "", 32 };
const TC13::RegisterType TC13::RegisterType::dReg { "d", "d_", "", "", 32 };
const TC13::RegisterType TC13::RegisterType::eReg { "e", "e_", "", "", 64 };
const TC13::RegisterType TC13::RegisterType::pReg { "p", "p_", "", "", 64 };
const TC13::RegisterType TC13::RegisterType::pswBit { "PSW.", "PSW_", "", "",
                                                      1 };

}       // namespace WIR
