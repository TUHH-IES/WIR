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

  // Check the entire class hierarchy from WIR_Instruction down to
  // WIR_Parameter.

  WIR_Function f( "foobar" );
  WIR_VirtualRegister &r1 =
    f.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );
  WIR_VirtualRegister &r2 =
    f.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );

  WIR_Operation o(
    MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI,
    WIR_RegisterParameter( r1, WIR_Usage::def ),
    WIR_RegisterParameter( r2, WIR_Usage::use ),
    MIPS_Immediate16_Signed( 42 ) );
  WIR_Instruction i;

  i.pushBackOperation( o );
  i.pushBackOperation(
    { MIPS::OpCode::SW, MIPS::OperationFormat::RIR_2,
      WIR_RegisterParameter( r1, WIR_Usage::use ),
      MIPS_Immediate16_Signed( 112 ),
      WIR_RegisterParameter( r2, WIR_Usage::use ) } );

  // Traverse the entire hierarchy and check the IDs.
  ufAssert( !i.isInserted() );

  for ( WIR_Operation &o : i ) {
    ufAssert( o.isInserted() );
    ufAssert( o.getInstruction().getID() == i.getID() );

    for ( WIR_Parameter &p : o ) {
      ufAssert( p.isInserted() );
      ufAssert( p.getOperation().getID() == o.getID() );
    }
  }

  // Test the copy constructors.
  WIR_Instruction i1( i );

  ufAssert( !i1.isInserted() );

  for ( WIR_Operation &o : i1 ) {
    ufAssert( o.isInserted() );
    ufAssert( o.getInstruction().getID() == i1.getID() );

    for ( WIR_Parameter &p : o ) {
      ufAssert( p.isInserted() );
      ufAssert( p.getOperation().getID() == o.getID() );
    }
  }

  // Test the move constructors.
  WIR_Instruction i2( std::move( i ) );

  ufAssert( !i2.isInserted() );

  for ( WIR_Operation &o : i2 ) {
    ufAssert( o.isInserted() );
    ufAssert( o.getInstruction().getID() == i2.getID() );

    for ( WIR_Parameter &p : o ) {
      ufAssert( p.isInserted() );
      ufAssert( p.getOperation().getID() == o.getID() );
    }
  }

  // Test the copy assignment operator.
  WIR_Instruction i3;
  i3 = i1;

  ufAssert( !i3.isInserted() );

  for ( WIR_Operation &o : i3 ) {
    ufAssert( o.isInserted() );
    ufAssert( o.getInstruction().getID() == i3.getID() );

    for ( WIR_Parameter &p : o ) {
      ufAssert( p.isInserted() );
      ufAssert( p.getOperation().getID() == o.getID() );
    }
  }

  // Test the move assignment operator.
  WIR_Instruction i4;
  i4 = std::move( i1 );

  ufAssert( !i4.isInserted() );

  for ( WIR_Operation &o : i4 ) {
    ufAssert( o.isInserted() );
    ufAssert( o.getInstruction().getID() == i4.getID() );

    for ( WIR_Parameter &p : o ) {
      ufAssert( p.isInserted() );
      ufAssert( p.getOperation().getID() == o.getID() );
    }
  }

  return( 0 );
}
