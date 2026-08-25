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

// Include standard headers
#include <sstream>
#include <string>

// Include WIR headers
#include <wir/wir.h>
#include <arch/tricore/tc131.h>
#include <arch/tricore/asmparser/tcasmargument.h>
#include <arch/tricore/asmparser/tcasmparser.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  WIR_TaskManager t;
  WIR_System sys( "tc1797.sys", t );

  WIR_CompilationUnit &c = sys.pushBackCompilationUnit( {} );
  WIR_Function &f = c.pushBackFunction( WIR_Function( "main" ) );
  WIR_BasicBlock &b1 = f.pushBackBasicBlock( {} );

  // String to be rejected by the parser.
  vector<unique_ptr<TC_AsmArgument>> dummy;
  TC_AsmParser parser;
  parser.run( "mov %a0, %d4", dummy, b1, "tcAsmParser5.cc" );

  return( 0 );
}
