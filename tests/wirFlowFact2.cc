/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/


//
// Include section
//

// Include libuseful headers
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/generic/mips.h>


using namespace std;
using namespace WIR;


/*
  This file tests class WIR_Loopbound, it should fail an assertion.
*/

int main( void )
{
  WIR_Init();

  // An unintialized loop bound is not bound to a loop. Thus, getLoop would
  // dereference a nullptr and will fail an assertion.
  WIR_LoopBound unbound;

  unbound.getLoop();
};
