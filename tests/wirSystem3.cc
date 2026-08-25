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

  // Check the entire class hierarchy from WIR_System down to WIR_Parameter.

  WIR_Function f( "foo" );
  WIR_VirtualRegister &r1 =
    f.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );
  WIR_VirtualRegister &r2 =
    f.pushBackVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );
  WIR_RegisterParameter p1( r1, WIR_Usage::def );
  WIR_RegisterParameter p2( r2, WIR_Usage::use );
  MIPS_Immediate16_Signed p3( 42 );

  WIR_Operation o(
    MIPS::OpCode::ADDI, MIPS::OperationFormat::RRI, p1, p2, p3 );
  WIR_Instruction i;
  WIR_BasicBlock b;
  WIR_CompilationUnit c;
  WIR_TaskManager t;
  WIR_System sys( "genericmips.sys", t );

  i.pushBackOperation( o );
  i.pushBackOperation(
    WIR_Operation(
      MIPS::OpCode::SW, MIPS::OperationFormat::RIR_2,
      WIR_RegisterParameter( r1, WIR_Usage::use ),
      MIPS_Immediate16_Signed( 112 ),
      WIR_RegisterParameter( r2, WIR_Usage::use ) ) );
  b.pushFrontInstruction( i );
  b.pushFrontInstruction( WIR_Instruction() );
  f.pushFrontBasicBlock( b );
  f.pushFrontBasicBlock( WIR_BasicBlock() );
  WIR_VirtualRegister r( MIPS::RegisterType::reg );
  f.pushFrontVirtualRegister( r );
  f.pushFrontVirtualRegister( WIR_VirtualRegister( MIPS::RegisterType::reg ) );
  f.pushFrontVirtualRegister( r );
  c.pushFrontFunction( f );
  c.pushFrontFunction( WIR_Function( "bar" ) );
  sys.pushFrontCompilationUnit( c );
  sys.pushFrontCompilationUnit( WIR_CompilationUnit() );

  // Traverse the entire hierarchy and check the IDs.
  ufAssert( sys.getTaskManager().isInserted() );
  ufAssert( sys.getTaskManager().getSystem().getID() == sys.getID() );

  for ( WIR_CompilationUnit &c : sys ) {
    ufAssert( c.isInserted() );
    ufAssert( c.getSystem().getID() == sys.getID() );

    for ( WIR_Function &f : c ) {
      ufAssert( f.isInserted() );
      ufAssert( f.getCompilationUnit().getID() == c.getID() );

      for ( WIR_VirtualRegister &r : f.getVirtualRegisters() ) {
        ufAssert( r.isInserted() );
        ufAssert( r.getFunction().getID() == f.getID() );
      }

      for ( WIR_BasicBlock &b : f ) {
        ufAssert( b.isInserted() );
        ufAssert( b.getFunction().getID() == f.getID() );

        for ( WIR_Instruction &i : b ) {
          ufAssert( i.isInserted() );
          ufAssert( i.getBasicBlock().getID() == b.getID() );

          for ( WIR_Operation &o : i ) {
            ufAssert( o.isInserted() );
            ufAssert( o.getInstruction().getID() == i.getID() );

            for ( WIR_Parameter &p : o ) {
              ufAssert( p.isInserted() );
              ufAssert( p.getOperation().getID() == o.getID() );
            }
          }
        }
      }
    }
  }

  return( 0 );
}
