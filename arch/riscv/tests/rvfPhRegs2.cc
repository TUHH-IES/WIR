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

//
// Include section
//

// Include libuseful headers
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32ifd.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  RV32IFD p1;

  // Check names of all physical registers.
  ufAssert( p1.f0().getName() == "f0" );
  ufAssert( p1.f1().getName() == "f1" );
  ufAssert( p1.f2().getName() == "f2" );
  ufAssert( p1.f3().getName() == "f3" );
  ufAssert( p1.f4().getName() == "f4" );
  ufAssert( p1.f5().getName() == "f5" );
  ufAssert( p1.f6().getName() == "f6" );
  ufAssert( p1.f7().getName() == "f7" );
  ufAssert( p1.f8().getName() == "f8" );
  ufAssert( p1.f9().getName() == "f9" );
  ufAssert( p1.f10().getName() == "f10" );
  ufAssert( p1.f11().getName() == "f11" );
  ufAssert( p1.f12().getName() == "f12" );
  ufAssert( p1.f13().getName() == "f13" );
  ufAssert( p1.f14().getName() == "f14" );
  ufAssert( p1.f15().getName() == "f15" );
  ufAssert( p1.f16().getName() == "f16" );
  ufAssert( p1.f17().getName() == "f17" );
  ufAssert( p1.f18().getName() == "f18" );
  ufAssert( p1.f19().getName() == "f19" );
  ufAssert( p1.f20().getName() == "f20" );
  ufAssert( p1.f21().getName() == "f21" );
  ufAssert( p1.f22().getName() == "f22" );
  ufAssert( p1.f23().getName() == "f23" );
  ufAssert( p1.f24().getName() == "f24" );
  ufAssert( p1.f25().getName() == "f25" );
  ufAssert( p1.f26().getName() == "f26" );
  ufAssert( p1.f27().getName() == "f27" );
  ufAssert( p1.f28().getName() == "f28" );
  ufAssert( p1.f29().getName() == "f29" );
  ufAssert( p1.f30().getName() == "f30" );
  ufAssert( p1.f31().getName() == "f31" );

  // Check virtual/physical property.
  ufAssert( p1.f0().isPhysical() );
  ufAssert( p1.f1().isPhysical() );
  ufAssert( p1.f2().isPhysical() );
  ufAssert( p1.f3().isPhysical() );
  ufAssert( p1.f4().isPhysical() );
  ufAssert( p1.f5().isPhysical() );
  ufAssert( p1.f6().isPhysical() );
  ufAssert( p1.f7().isPhysical() );
  ufAssert( p1.f8().isPhysical() );
  ufAssert( p1.f9().isPhysical() );
  ufAssert( p1.f10().isPhysical() );
  ufAssert( p1.f11().isPhysical() );
  ufAssert( p1.f12().isPhysical() );
  ufAssert( p1.f13().isPhysical() );
  ufAssert( p1.f14().isPhysical() );
  ufAssert( p1.f15().isPhysical() );
  ufAssert( p1.f16().isPhysical() );
  ufAssert( p1.f17().isPhysical() );
  ufAssert( p1.f18().isPhysical() );
  ufAssert( p1.f19().isPhysical() );
  ufAssert( p1.f20().isPhysical() );
  ufAssert( p1.f21().isPhysical() );
  ufAssert( p1.f22().isPhysical() );
  ufAssert( p1.f23().isPhysical() );
  ufAssert( p1.f24().isPhysical() );
  ufAssert( p1.f25().isPhysical() );
  ufAssert( p1.f26().isPhysical() );
  ufAssert( p1.f27().isPhysical() );
  ufAssert( p1.f28().isPhysical() );
  ufAssert( p1.f29().isPhysical() );
  ufAssert( p1.f30().isPhysical() );
  ufAssert( p1.f31().isPhysical() );

  ufAssert( !p1.f0().isVirtual() );
  ufAssert( !p1.f1().isVirtual() );
  ufAssert( !p1.f2().isVirtual() );
  ufAssert( !p1.f3().isVirtual() );
  ufAssert( !p1.f4().isVirtual() );
  ufAssert( !p1.f5().isVirtual() );
  ufAssert( !p1.f6().isVirtual() );
  ufAssert( !p1.f7().isVirtual() );
  ufAssert( !p1.f8().isVirtual() );
  ufAssert( !p1.f9().isVirtual() );
  ufAssert( !p1.f10().isVirtual() );
  ufAssert( !p1.f11().isVirtual() );
  ufAssert( !p1.f12().isVirtual() );
  ufAssert( !p1.f13().isVirtual() );
  ufAssert( !p1.f14().isVirtual() );
  ufAssert( !p1.f15().isVirtual() );
  ufAssert( !p1.f16().isVirtual() );
  ufAssert( !p1.f17().isVirtual() );
  ufAssert( !p1.f18().isVirtual() );
  ufAssert( !p1.f19().isVirtual() );
  ufAssert( !p1.f20().isVirtual() );
  ufAssert( !p1.f21().isVirtual() );
  ufAssert( !p1.f22().isVirtual() );
  ufAssert( !p1.f23().isVirtual() );
  ufAssert( !p1.f24().isVirtual() );
  ufAssert( !p1.f25().isVirtual() );
  ufAssert( !p1.f26().isVirtual() );
  ufAssert( !p1.f27().isVirtual() );
  ufAssert( !p1.f28().isVirtual() );
  ufAssert( !p1.f29().isVirtual() );
  ufAssert( !p1.f30().isVirtual() );
  ufAssert( !p1.f31().isVirtual() );

  // Check register type.
  ufAssert( p1.f0().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f1().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f2().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f3().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f4().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f5().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f6().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f7().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f8().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f9().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f10().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f11().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f12().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f13().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f14().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f15().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f16().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f17().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f18().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f19().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f20().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f21().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f22().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f23().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f24().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f25().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f26().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f27().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f28().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f29().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f30().getType() == RV32IF::RegisterType::fReg );
  ufAssert( p1.f31().getType() == RV32IF::RegisterType::fReg );

  // Check insertion.
  ufAssert( p1.f0().isInserted() );
  ufAssert( p1.f1().isInserted() );
  ufAssert( p1.f2().isInserted() );
  ufAssert( p1.f3().isInserted() );
  ufAssert( p1.f4().isInserted() );
  ufAssert( p1.f5().isInserted() );
  ufAssert( p1.f6().isInserted() );
  ufAssert( p1.f7().isInserted() );
  ufAssert( p1.f8().isInserted() );
  ufAssert( p1.f9().isInserted() );
  ufAssert( p1.f10().isInserted() );
  ufAssert( p1.f11().isInserted() );
  ufAssert( p1.f12().isInserted() );
  ufAssert( p1.f13().isInserted() );
  ufAssert( p1.f14().isInserted() );
  ufAssert( p1.f15().isInserted() );
  ufAssert( p1.f16().isInserted() );
  ufAssert( p1.f17().isInserted() );
  ufAssert( p1.f18().isInserted() );
  ufAssert( p1.f19().isInserted() );
  ufAssert( p1.f20().isInserted() );
  ufAssert( p1.f21().isInserted() );
  ufAssert( p1.f22().isInserted() );
  ufAssert( p1.f23().isInserted() );
  ufAssert( p1.f24().isInserted() );
  ufAssert( p1.f25().isInserted() );
  ufAssert( p1.f26().isInserted() );
  ufAssert( p1.f27().isInserted() );
  ufAssert( p1.f28().isInserted() );
  ufAssert( p1.f29().isInserted() );
  ufAssert( p1.f30().isInserted() );
  ufAssert( p1.f31().isInserted() );

  // Check bit widths.
  ufAssert( RV32IF::RegisterType::fReg.getBitWidth() == 64  );

  ufAssert( p1.f0().getBitWidth() == 64 );
  ufAssert( p1.f1().getBitWidth() == 64 );
  ufAssert( p1.f2().getBitWidth() == 64 );
  ufAssert( p1.f3().getBitWidth() == 64 );
  ufAssert( p1.f4().getBitWidth() == 64 );
  ufAssert( p1.f5().getBitWidth() == 64 );
  ufAssert( p1.f6().getBitWidth() == 64 );
  ufAssert( p1.f7().getBitWidth() == 64 );
  ufAssert( p1.f8().getBitWidth() == 64 );
  ufAssert( p1.f9().getBitWidth() == 64 );
  ufAssert( p1.f10().getBitWidth() == 64 );
  ufAssert( p1.f11().getBitWidth() == 64 );
  ufAssert( p1.f12().getBitWidth() == 64 );
  ufAssert( p1.f13().getBitWidth() == 64 );
  ufAssert( p1.f14().getBitWidth() == 64 );
  ufAssert( p1.f15().getBitWidth() == 64 );
  ufAssert( p1.f16().getBitWidth() == 64 );
  ufAssert( p1.f17().getBitWidth() == 64 );
  ufAssert( p1.f18().getBitWidth() == 64 );
  ufAssert( p1.f19().getBitWidth() == 64 );
  ufAssert( p1.f20().getBitWidth() == 64 );
  ufAssert( p1.f21().getBitWidth() == 64 );
  ufAssert( p1.f22().getBitWidth() == 64 );
  ufAssert( p1.f23().getBitWidth() == 64 );
  ufAssert( p1.f24().getBitWidth() == 64 );
  ufAssert( p1.f25().getBitWidth() == 64 );
  ufAssert( p1.f26().getBitWidth() == 64 );
  ufAssert( p1.f27().getBitWidth() == 64 );
  ufAssert( p1.f28().getBitWidth() == 64 );
  ufAssert( p1.f29().getBitWidth() == 64 );
  ufAssert( p1.f30().getBitWidth() == 64 );
  ufAssert( p1.f31().getBitWidth() == 64 );

  // Check vectors of physical registers.
  ufAssert( p1.getPhRegs().size() == 80 );

  auto v1( p1.getPhRegs() );
  auto it1 = v1.begin();
  ufAssert( (*it1++).get().getName() == "x0" );
  ufAssert( (*it1++).get().getName() == "x1" );
  ufAssert( (*it1++).get().getName() == "x2" );
  ufAssert( (*it1++).get().getName() == "x3" );
  ufAssert( (*it1++).get().getName() == "x4" );
  ufAssert( (*it1++).get().getName() == "x5" );
  ufAssert( (*it1++).get().getName() == "x6" );
  ufAssert( (*it1++).get().getName() == "x7" );
  ufAssert( (*it1++).get().getName() == "x8" );
  ufAssert( (*it1++).get().getName() == "x9" );
  ufAssert( (*it1++).get().getName() == "x10" );
  ufAssert( (*it1++).get().getName() == "x11" );
  ufAssert( (*it1++).get().getName() == "x12" );
  ufAssert( (*it1++).get().getName() == "x13" );
  ufAssert( (*it1++).get().getName() == "x14" );
  ufAssert( (*it1++).get().getName() == "x15" );
  ufAssert( (*it1++).get().getName() == "x16" );
  ufAssert( (*it1++).get().getName() == "x17" );
  ufAssert( (*it1++).get().getName() == "x18" );
  ufAssert( (*it1++).get().getName() == "x19" );
  ufAssert( (*it1++).get().getName() == "x20" );
  ufAssert( (*it1++).get().getName() == "x21" );
  ufAssert( (*it1++).get().getName() == "x22" );
  ufAssert( (*it1++).get().getName() == "x23" );
  ufAssert( (*it1++).get().getName() == "x24" );
  ufAssert( (*it1++).get().getName() == "x25" );
  ufAssert( (*it1++).get().getName() == "x26" );
  ufAssert( (*it1++).get().getName() == "x27" );
  ufAssert( (*it1++).get().getName() == "x28" );
  ufAssert( (*it1++).get().getName() == "x29" );
  ufAssert( (*it1++).get().getName() == "x30" );
  ufAssert( (*it1++).get().getName() == "x31" );
  ufAssert( (*it1++).get().getName() == "x0" );
  ufAssert( (*it1++).get().getName() == "x2" );
  ufAssert( (*it1++).get().getName() == "x4" );
  ufAssert( (*it1++).get().getName() == "x6" );
  ufAssert( (*it1++).get().getName() == "x8" );
  ufAssert( (*it1++).get().getName() == "x10" );
  ufAssert( (*it1++).get().getName() == "x12" );
  ufAssert( (*it1++).get().getName() == "x14" );
  ufAssert( (*it1++).get().getName() == "x16" );
  ufAssert( (*it1++).get().getName() == "x18" );
  ufAssert( (*it1++).get().getName() == "x20" );
  ufAssert( (*it1++).get().getName() == "x22" );
  ufAssert( (*it1++).get().getName() == "x24" );
  ufAssert( (*it1++).get().getName() == "x26" );
  ufAssert( (*it1++).get().getName() == "x28" );
  ufAssert( (*it1++).get().getName() == "x30" );
  ufAssert( (*it1++).get().getName() == "f0" );
  ufAssert( (*it1++).get().getName() == "f1" );
  ufAssert( (*it1++).get().getName() == "f2" );
  ufAssert( (*it1++).get().getName() == "f3" );
  ufAssert( (*it1++).get().getName() == "f4" );
  ufAssert( (*it1++).get().getName() == "f5" );
  ufAssert( (*it1++).get().getName() == "f6" );
  ufAssert( (*it1++).get().getName() == "f7" );
  ufAssert( (*it1++).get().getName() == "f8" );
  ufAssert( (*it1++).get().getName() == "f9" );
  ufAssert( (*it1++).get().getName() == "f10" );
  ufAssert( (*it1++).get().getName() == "f11" );
  ufAssert( (*it1++).get().getName() == "f12" );
  ufAssert( (*it1++).get().getName() == "f13" );
  ufAssert( (*it1++).get().getName() == "f14" );
  ufAssert( (*it1++).get().getName() == "f15" );
  ufAssert( (*it1++).get().getName() == "f16" );
  ufAssert( (*it1++).get().getName() == "f17" );
  ufAssert( (*it1++).get().getName() == "f18" );
  ufAssert( (*it1++).get().getName() == "f19" );
  ufAssert( (*it1++).get().getName() == "f20" );
  ufAssert( (*it1++).get().getName() == "f21" );
  ufAssert( (*it1++).get().getName() == "f22" );
  ufAssert( (*it1++).get().getName() == "f23" );
  ufAssert( (*it1++).get().getName() == "f24" );
  ufAssert( (*it1++).get().getName() == "f25" );
  ufAssert( (*it1++).get().getName() == "f26" );
  ufAssert( (*it1++).get().getName() == "f27" );
  ufAssert( (*it1++).get().getName() == "f28" );
  ufAssert( (*it1++).get().getName() == "f29" );
  ufAssert( (*it1++).get().getName() == "f30" );
  ufAssert( (*it1++).get().getName() == "f31" );

  auto v2( p1.getPhRegs( RV32IF::RegisterType::fReg ) );
  auto it2 = v2.begin();
  ufAssert( v2.size() == 32 );
  ufAssert( (*it2++).get().getName() == "f0" );
  ufAssert( (*it2++).get().getName() == "f1" );
  ufAssert( (*it2++).get().getName() == "f2" );
  ufAssert( (*it2++).get().getName() == "f3" );
  ufAssert( (*it2++).get().getName() == "f4" );
  ufAssert( (*it2++).get().getName() == "f5" );
  ufAssert( (*it2++).get().getName() == "f6" );
  ufAssert( (*it2++).get().getName() == "f7" );
  ufAssert( (*it2++).get().getName() == "f8" );
  ufAssert( (*it2++).get().getName() == "f9" );
  ufAssert( (*it2++).get().getName() == "f10" );
  ufAssert( (*it2++).get().getName() == "f11" );
  ufAssert( (*it2++).get().getName() == "f12" );
  ufAssert( (*it2++).get().getName() == "f13" );
  ufAssert( (*it2++).get().getName() == "f14" );
  ufAssert( (*it2++).get().getName() == "f15" );
  ufAssert( (*it2++).get().getName() == "f16" );
  ufAssert( (*it2++).get().getName() == "f17" );
  ufAssert( (*it2++).get().getName() == "f18" );
  ufAssert( (*it2++).get().getName() == "f19" );
  ufAssert( (*it2++).get().getName() == "f20" );
  ufAssert( (*it2++).get().getName() == "f21" );
  ufAssert( (*it2++).get().getName() == "f22" );
  ufAssert( (*it2++).get().getName() == "f23" );
  ufAssert( (*it2++).get().getName() == "f24" );
  ufAssert( (*it2++).get().getName() == "f25" );
  ufAssert( (*it2++).get().getName() == "f26" );
  ufAssert( (*it2++).get().getName() == "f27" );
  ufAssert( (*it2++).get().getName() == "f28" );
  ufAssert( (*it2++).get().getName() == "f29" );
  ufAssert( (*it2++).get().getName() == "f30" );
  ufAssert( (*it2++).get().getName() == "f31" );

  return( 0 );
}
