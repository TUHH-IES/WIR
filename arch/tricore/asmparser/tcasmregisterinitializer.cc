/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcasmregisterinitializer.cc
  @brief This file implements a generic %WIR register initializing class.

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

// Include local headers
#include "tcasmregisterinitializer.h"


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Destructor.
*/
TC_AsmRegisterInitializer::~TC_AsmRegisterInitializer( void )
{
  DSTART( "virtual TC_AsmRegisterInitializer::~TC_AsmRegisterInitializer()" );
};

}       // namespace WIR
