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


int main( void )
{
  WIR_Init();

  WIR_Function f( "main" );

  // Insert registers in DESCENDING order of IDs.
  f.pushBackVirtualRegister( MIPS_RegV() );
  f.pushFrontVirtualRegister( MIPS_RegV() );
  f.pushFrontVirtualRegister( MIPS_RegV() );

  // Insert basic blocks in descending order of IDs.
  f.pushBackBasicBlock( WIR_BasicBlock() );
  f.pushFrontBasicBlock( WIR_BasicBlock() );
  f.pushFrontBasicBlock( WIR_BasicBlock() );

  // Copy function. The registers in f1 should also appear in DESCENDING ID
  // order.
  WIR_Function f1( f );

  ufAssert(
    f.getVirtualRegisters().size() == f1.getVirtualRegisters().size() );
  ufAssert( f.getBasicBlocks().size() == f1.getBasicBlocks().size() );

  // Check descending order of register IDs.
  WIR_id_t prev = nullid;
  for ( WIR_VirtualRegister &r : f1.getVirtualRegisters() ) {
    if ( prev == nullid )
      ufAssert( r.getID() > prev );
    else
      ufAssert( r.getID() < prev );
    prev = r.getID();
  }

  // Check descending order of basic block IDs.
  prev = nullid;
  for ( WIR_BasicBlock &b : f1 ) {
    if ( prev == nullid )
      ufAssert( b.getID() > prev );
    else
      ufAssert( b.getID() < prev );
    prev = b.getID();
  }

  return( 0 );
}
