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
#include <arch/arm/armv4t.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  ARMv4T p;

  // The following must assert since TR1LR accepts the link register only.
  WIR_Operation o1(
    ARMv4T::OpCode::PUSH, ARMv4T::OperationFormat::TR1LR,
    WIR_RegisterParameter( p.R1(), WIR_Usage::use ),
    WIR_RegisterParameter( p.SP(), WIR_Usage::use ) );

  // Fallback for disabled failsafe mode.
  auto it = o1.end();
  --it;
  auto &regP = dynamic_cast<WIR_RegisterParameter &>( it->get() );
  ufAssertT(
    regP.getRegister().getName() == p.LR().getName(),
    "Parameters incompatible with operation format '" <<
    o1.getOperationFormat().getName() << "'." );

  return( 0 );
}
