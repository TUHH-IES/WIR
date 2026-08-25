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

// Include WIR headers
#include <wir/wir.h>
#include <arch/generic/mips.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  WIR_Function f( "main" );
  WIR_VirtualRegister &r1 =
    f.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );
  WIR_VirtualRegister &r2 =
    f.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );
  WIR_RegisterParameter p1( r1, WIR_Usage::def );
  WIR_RegisterParameter p2( r2, WIR_Usage::use );
  MIPS_Immediate16_Signed p3( 42 );

  WIR_Operation o(
    MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI, p1, p2, p3 );

  WIR_Parameter &p = o.getParameters().front();
  o.setDontOptimize();
  p.setImplicit();

  return( 0 );
}
