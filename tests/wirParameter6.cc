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

  WIR_CompilationUnit c;
  WIR_Function &f1 = c.pushBackFunction( WIR_Function( "main" ) );
  WIR_VirtualRegister &r1 =
    f1.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );
  WIR_VirtualRegister &r2 =
    f1.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );

  f1.pushBackBasicBlock(
    { { { MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI,
          WIR_RegisterParameter( r1, WIR_Usage::def ),
          WIR_RegisterParameter( r2, WIR_Usage::use ),
          MIPS_Immediate16_Signed( 42 ) } } } );
  c.setDontOptimize();

  WIR_Parameter &p =
    f1.getBasicBlocks().front().get().getInstructions().front().get().getOperations().front().get().getParameters().front().get();
  p.setImplicit();

  return( 0 );
}
