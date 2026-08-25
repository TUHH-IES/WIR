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
#include <arch/arm/armv4.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  ARMv4 p1;

  // Check names of all physical registers.
  ufAssert( p1.R0().getName() == "r0" );
  ufAssert( p1.R1().getName() == "r1" );
  ufAssert( p1.R2().getName() == "r2" );
  ufAssert( p1.R3().getName() == "r3" );
  ufAssert( p1.R4().getName() == "r4" );
  ufAssert( p1.R5().getName() == "r5" );
  ufAssert( p1.R6().getName() == "r6" );
  ufAssert( p1.R7().getName() == "r7" );
  ufAssert( p1.R8().getName() == "r8" );
  ufAssert( p1.R9().getName() == "r9" );
  ufAssert( p1.R10().getName() == "r10" );
  ufAssert( p1.R11().getName() == "r11" );
  ufAssert( p1.R12().getName() == "r12" );
  ufAssert( p1.R13().getName() == "r13" );
  ufAssert( p1.R14().getName() == "r14" );
  ufAssert( p1.R15().getName() == "r15" );

  ufAssert( p1.SP().getName() == "r13" );
  ufAssert( p1.LR().getName() == "r14" );
  ufAssert( p1.PC().getName() == "r15" );

  // Check virtual/physical property.
  ufAssert( p1.R0().isPhysical() );
  ufAssert( p1.R1().isPhysical() );
  ufAssert( p1.R2().isPhysical() );
  ufAssert( p1.R3().isPhysical() );
  ufAssert( p1.R4().isPhysical() );
  ufAssert( p1.R5().isPhysical() );
  ufAssert( p1.R6().isPhysical() );
  ufAssert( p1.R7().isPhysical() );
  ufAssert( p1.R8().isPhysical() );
  ufAssert( p1.R9().isPhysical() );
  ufAssert( p1.R10().isPhysical() );
  ufAssert( p1.R11().isPhysical() );
  ufAssert( p1.R12().isPhysical() );
  ufAssert( p1.R13().isPhysical() );
  ufAssert( p1.R14().isPhysical() );
  ufAssert( p1.R15().isPhysical() );

  ufAssert( !p1.R0().isVirtual() );
  ufAssert( !p1.R1().isVirtual() );
  ufAssert( !p1.R2().isVirtual() );
  ufAssert( !p1.R3().isVirtual() );
  ufAssert( !p1.R4().isVirtual() );
  ufAssert( !p1.R5().isVirtual() );
  ufAssert( !p1.R6().isVirtual() );
  ufAssert( !p1.R7().isVirtual() );
  ufAssert( !p1.R8().isVirtual() );
  ufAssert( !p1.R9().isVirtual() );
  ufAssert( !p1.R10().isVirtual() );
  ufAssert( !p1.R11().isVirtual() );
  ufAssert( !p1.R12().isVirtual() );
  ufAssert( !p1.R13().isVirtual() );
  ufAssert( !p1.R14().isVirtual() );
  ufAssert( !p1.R15().isVirtual() );

  // Check register type.
  ufAssert( p1.R0().getType() == ARMv4::RegisterType::lo );
  ufAssert( p1.R1().getType() == ARMv4::RegisterType::lo );
  ufAssert( p1.R2().getType() == ARMv4::RegisterType::lo );
  ufAssert( p1.R3().getType() == ARMv4::RegisterType::lo );
  ufAssert( p1.R4().getType() == ARMv4::RegisterType::lo );
  ufAssert( p1.R5().getType() == ARMv4::RegisterType::lo );
  ufAssert( p1.R6().getType() == ARMv4::RegisterType::lo );
  ufAssert( p1.R7().getType() == ARMv4::RegisterType::lo );
  ufAssert( p1.R8().getType() == ARMv4::RegisterType::hi );
  ufAssert( p1.R9().getType() == ARMv4::RegisterType::hi );
  ufAssert( p1.R10().getType() == ARMv4::RegisterType::hi );
  ufAssert( p1.R11().getType() == ARMv4::RegisterType::hi );
  ufAssert( p1.R12().getType() == ARMv4::RegisterType::hi );
  ufAssert( p1.R13().getType() == ARMv4::RegisterType::hi );
  ufAssert( p1.R14().getType() == ARMv4::RegisterType::hi );
  ufAssert( p1.R15().getType() == ARMv4::RegisterType::hi );

  // Check insertion.
  ufAssert( p1.R0().isInserted() );
  ufAssert( p1.R1().isInserted() );
  ufAssert( p1.R2().isInserted() );
  ufAssert( p1.R3().isInserted() );
  ufAssert( p1.R4().isInserted() );
  ufAssert( p1.R5().isInserted() );
  ufAssert( p1.R6().isInserted() );
  ufAssert( p1.R7().isInserted() );
  ufAssert( p1.R8().isInserted() );
  ufAssert( p1.R9().isInserted() );
  ufAssert( p1.R10().isInserted() );
  ufAssert( p1.R11().isInserted() );
  ufAssert( p1.R12().isInserted() );
  ufAssert( p1.R13().isInserted() );
  ufAssert( p1.R14().isInserted() );
  ufAssert( p1.R15().isInserted() );

  // Check bit widths.
  ufAssert( ARMv4::RegisterType::reg.getBitWidth() == 32 );
  ufAssert( ARMv4::RegisterType::lo.getBitWidth() == 32 );
  ufAssert( ARMv4::RegisterType::hi.getBitWidth() == 32 );

  ufAssert( p1.R0().getBitWidth() == 32 );
  ufAssert( p1.R1().getBitWidth() == 32 );
  ufAssert( p1.R2().getBitWidth() == 32 );
  ufAssert( p1.R3().getBitWidth() == 32 );
  ufAssert( p1.R4().getBitWidth() == 32 );
  ufAssert( p1.R5().getBitWidth() == 32 );
  ufAssert( p1.R6().getBitWidth() == 32 );
  ufAssert( p1.R7().getBitWidth() == 32 );
  ufAssert( p1.R8().getBitWidth() == 32 );
  ufAssert( p1.R9().getBitWidth() == 32 );
  ufAssert( p1.R10().getBitWidth() == 32 );
  ufAssert( p1.R11().getBitWidth() == 32 );
  ufAssert( p1.R12().getBitWidth() == 32 );
  ufAssert( p1.R13().getBitWidth() == 32 );
  ufAssert( p1.R14().getBitWidth() == 32 );
  ufAssert( p1.R15().getBitWidth() == 32 );

  // Check stack pointer property.
  ufAssert( !p1.R0().isStackPointer() );
  ufAssert( !p1.R1().isStackPointer() );
  ufAssert( !p1.R2().isStackPointer() );
  ufAssert( !p1.R3().isStackPointer() );
  ufAssert( !p1.R4().isStackPointer() );
  ufAssert( !p1.R5().isStackPointer() );
  ufAssert( !p1.R6().isStackPointer() );
  ufAssert( !p1.R7().isStackPointer() );
  ufAssert( !p1.R8().isStackPointer() );
  ufAssert( !p1.R9().isStackPointer() );
  ufAssert( !p1.R10().isStackPointer() );
  ufAssert( !p1.R11().isStackPointer() );
  ufAssert( !p1.R12().isStackPointer() );
  ufAssert( p1.R13().isStackPointer() );
  ufAssert( !p1.R14().isStackPointer() );
  ufAssert( !p1.R15().isStackPointer() );

  // Check vectors of physical registers.
  ufAssert( p1.getPhRegs().size() == 16 );

  auto v1( p1.getPhRegs() );
  auto it1 = v1.begin();
  ufAssert( (*it1++).get().getName() == "r0" );
  ufAssert( (*it1++).get().getName() == "r1" );
  ufAssert( (*it1++).get().getName() == "r2" );
  ufAssert( (*it1++).get().getName() == "r3" );
  ufAssert( (*it1++).get().getName() == "r4" );
  ufAssert( (*it1++).get().getName() == "r5" );
  ufAssert( (*it1++).get().getName() == "r6" );
  ufAssert( (*it1++).get().getName() == "r7" );
  ufAssert( (*it1++).get().getName() == "r8" );
  ufAssert( (*it1++).get().getName() == "r9" );
  ufAssert( (*it1++).get().getName() == "r10" );
  ufAssert( (*it1++).get().getName() == "r11" );
  ufAssert( (*it1++).get().getName() == "r12" );
  ufAssert( (*it1++).get().getName() == "r13" );
  ufAssert( (*it1++).get().getName() == "r14" );
  ufAssert( (*it1++).get().getName() == "r15" );

  auto v2( p1.getPhRegs( ARMv4::RegisterType::reg ) );
  ufAssert( v2.size() == 0 );

  auto v3( p1.getPhRegs( ARMv4::RegisterType::lo ) );
  auto it3 = v3.begin();
  ufAssert( v3.size() == 8 );
  ufAssert( (*it3++).get().getName() == "r0" );
  ufAssert( (*it3++).get().getName() == "r1" );
  ufAssert( (*it3++).get().getName() == "r2" );
  ufAssert( (*it3++).get().getName() == "r3" );
  ufAssert( (*it3++).get().getName() == "r4" );
  ufAssert( (*it3++).get().getName() == "r5" );
  ufAssert( (*it3++).get().getName() == "r6" );
  ufAssert( (*it3++).get().getName() == "r7" );

  auto v4( p1.getPhRegs( ARMv4::RegisterType::hi ) );
  auto it4 = v4.begin();
  ufAssert( v4.size() == 8 );
  ufAssert( (*it4++).get().getName() == "r8" );
  ufAssert( (*it4++).get().getName() == "r9" );
  ufAssert( (*it4++).get().getName() == "r10" );
  ufAssert( (*it4++).get().getName() == "r11" );
  ufAssert( (*it4++).get().getName() == "r12" );
  ufAssert( (*it4++).get().getName() == "r13" );
  ufAssert( (*it4++).get().getName() == "r14" );
  ufAssert( (*it4++).get().getName() == "r15" );

  // Check parent/child relationships.
  ufAssert( !p1.R0().isChild() );
  ufAssert( !p1.R0().hasChilds() );
  ufAssert( !p1.R1().isChild() );
  ufAssert( !p1.R1().hasChilds() );
  ufAssert( !p1.R2().isChild() );
  ufAssert( !p1.R2().hasChilds() );
  ufAssert( !p1.R3().isChild() );
  ufAssert( !p1.R3().hasChilds() );
  ufAssert( !p1.R4().isChild() );
  ufAssert( !p1.R4().hasChilds() );
  ufAssert( !p1.R5().isChild() );
  ufAssert( !p1.R5().hasChilds() );
  ufAssert( !p1.R6().isChild() );
  ufAssert( !p1.R6().hasChilds() );
  ufAssert( !p1.R7().isChild() );
  ufAssert( !p1.R7().hasChilds() );
  ufAssert( !p1.R8().isChild() );
  ufAssert( !p1.R8().hasChilds() );
  ufAssert( !p1.R9().isChild() );
  ufAssert( !p1.R9().hasChilds() );
  ufAssert( !p1.R10().isChild() );
  ufAssert( !p1.R10().hasChilds() );
  ufAssert( !p1.R11().isChild() );
  ufAssert( !p1.R11().hasChilds() );
  ufAssert( !p1.R12().isChild() );
  ufAssert( !p1.R12().hasChilds() );
  ufAssert( !p1.R13().isChild() );
  ufAssert( !p1.R13().hasChilds() );
  ufAssert( !p1.R14().isChild() );
  ufAssert( !p1.R14().hasChilds() );
  ufAssert( !p1.R15().isChild() );
  ufAssert( !p1.R15().hasChilds() );

  return( 0 );
}
