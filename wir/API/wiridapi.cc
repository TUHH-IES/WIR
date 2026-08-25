/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wiridapi.cc
  @brief This file implements a base class for managing unique numerical IDs of
         derived classes.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <iostream>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wirregistry.h>

// Include private headers
#include "wiridapi.h"


//
// Code section
//

namespace WIR {


using namespace std;


// mIDCounter contains the next free numerial ID for WIR objects.
WIR_id_t WIR_ID_API::mIDCounter = 1;

}       // namespace WIR
