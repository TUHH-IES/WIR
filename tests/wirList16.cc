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

  // Create some WIR pointer list.
  WIR_Function f( "test" );
  WIR_VirtualRegister r1( MIPS::RegisterType::reg );
  WIR_VirtualRegister r2( MIPS::RegisterType::reg );
  WIR_VirtualRegister r3( MIPS::RegisterType::reg );

  f.pushBackVirtualRegister( r1 );
  f.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );
  f.pushFrontVirtualRegister( r2 );
  f.pushFrontVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );

  auto it1 = f.getVirtualRegisters().begin();
  it1++;
  f.insertVirtualRegister( it1, r3 );
  it1 = f.getVirtualRegisters().end();
  it1--;
  it1--;
  f.insertVirtualRegister(
    it1, WIR_VirtualRegister( MIPS::RegisterType::reg ) );

  // Check that replace actually destroys the removed element.
  it1 = f.getVirtualRegisters().begin();
  it1++;
  it1++;
  WIR_VirtualRegister &reg = it1->get();
  WIR_VirtualRegister r4( MIPS::RegisterType::reg );
  f.replaceVirtualRegister( it1, r4 );
  cout << reg.getID() << reg.getName() << endl;

  return( 0 );
}
