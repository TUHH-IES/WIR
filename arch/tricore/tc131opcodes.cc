/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tc131opcodes.cc
  @brief This file declares the Infineon TriCore V1.3.1's opcodes.

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
#include <arch/tricore/tc131.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

const TC131::OpCode TC131::OpCode::CACHEI_W   { "cachei.w",
                                                WIR_OpCodeType::implicitMemoryAccess,
                                                WIR_OpCodeType::sideEffect };
const TC131::OpCode TC131::OpCode::CACHEI_WI  { "cachei.wi",
                                                WIR_OpCodeType::implicitMemoryAccess,
                                                WIR_OpCodeType::sideEffect };
const TC131::OpCode TC131::OpCode::FTOIZ      { "ftoiz" };
const TC131::OpCode TC131::OpCode::FTOQ31Z    { "ftoq31z" };
const TC131::OpCode TC131::OpCode::FTOUZ      { "ftouz" };

}       // namespace WIR
