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

  // Create some WIR list.
  WIR_Function f( "main" );
  MIPS_RegV r1, r2, r3;

  f.pushBackVirtualRegister( r1 );
  f.pushBackVirtualRegister( MIPS_RegV() );
  f.pushFrontVirtualRegister( r1 );
  f.pushFrontVirtualRegister( MIPS_RegV() );

  auto it1 = f.getVirtualRegisters().begin();
  ++it1;
  f.insertVirtualRegister( it1, r3 );
  it1 = f.getVirtualRegisters().end();
  --it1;
  --it1;
  f.insertVirtualRegister( it1, MIPS_RegV() );

  // Check that clear actually destroys the removed element.
  auto it = f.getVirtualRegisters().end();
  --it;
  --it;
  --it;
  WIR_VirtualRegister &reg = it->get();
  f.clearVirtualRegisters();
  cout << reg.getID() << reg.getName() << endl;

  return( 0 );
}
