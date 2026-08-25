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

  WIR_BasicBlock b;

  // Insert instructions in DESCENDING order of IDs.
  b.pushBackInstruction( WIR_Instruction() );
  b.pushFrontInstruction( WIR_Instruction() );
  b.pushFrontInstruction( WIR_Instruction() );

  // Copy basic block. The instructions in b1 should also appear in DESCENDING
  // ID order.
  WIR_BasicBlock b1( b );

  ufAssert( b.getInstructions().size() == b1.getInstructions().size() );

  // Check descending order of instruction IDs.
  WIR_id_t prev = nullid;
  for ( WIR_Instruction &i : b1 ) {
    if ( prev == nullid )
      ufAssert( i.getID() > prev );
    else
      ufAssert( i.getID() < prev );
    prev = i.getID();
  }

  return( 0 );
}
