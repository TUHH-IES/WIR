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

  WIR_TaskManager t;
  WIR_System s( "genericmips.sys", t );
  WIR_CompilationUnit &c = s.pushBackCompilationUnit( {} );
  WIR_Function &f = c.pushBackFunction( WIR_Function( "foo" ) );
  WIR_VirtualRegister &r1 =
    f.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );
  WIR_VirtualRegister &r2 =
    f.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );

  f.pushBackBasicBlock(
    { { { MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI,
          WIR_RegisterParameter( r1, WIR_Usage::def ),
          WIR_RegisterParameter( r2, WIR_Usage::use ),
          MIPS_Immediate16_Signed( 42 ) } } } );

  s.setDontOptimize();
  s.popFrontCompilationUnit();

  return( 0 );
}
