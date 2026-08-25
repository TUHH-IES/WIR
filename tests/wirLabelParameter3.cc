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


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  WIR_Function f( "foo" );

  WIR_LabelParameter p1( f );

  // Trying to access a basic block for a label refering to a function must
  // fail.
  p1.getBasicBlock();

  return( 0 );
}
