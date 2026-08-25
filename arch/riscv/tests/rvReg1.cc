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

// Include libuseful headers
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32i.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  stringstream str;

  RV_RegV r1;
  ufAssert( r1.getType() == RV32I::RegisterType::reg );
  ufAssert( r1.getBitWidth() == 32 );
  ufAssert( r1.isVirtual() );
  ufAssert( !r1.isPhysical() );
  ufAssert( !r1.isInserted() );
  str.str( "" );
  str << RV32I::RegisterType::reg.getPrefixes()[ 1 ] << r1.getID()
      << RV32I::RegisterType::reg.getSuffixes()[ 1 ];
  ufAssert( r1.getName() == str.str() );

  // Test the copy constructor.
  RV_RegV r2 = r1;

  ufAssert( r2.getType() == RV32I::RegisterType::reg );
  ufAssert( r2.getBitWidth() == 32 );
  ufAssert( r2.isVirtual() );
  ufAssert( !r2.isPhysical() );
  ufAssert( !r2.isInserted() );
  str.str( "" );
  str << RV32I::RegisterType::reg.getPrefixes()[ 1 ] << r2.getID()
      << RV32I::RegisterType::reg.getSuffixes()[ 1 ];
  ufAssert( r2.getName() == str.str() );

  // Test the copy assignment operator.
  RV_RegV r3;
  r3 = r1;

  ufAssert( r3.getType() == RV32I::RegisterType::reg );
  ufAssert( r3.getBitWidth() == 32 );
  ufAssert( r3.isVirtual() );
  ufAssert( !r3.isPhysical() );
  ufAssert( !r3.isInserted() );
  str.str( "" );
  str << RV32I::RegisterType::reg.getPrefixes()[ 1 ] << r3.getID()
      << RV32I::RegisterType::reg.getSuffixes()[ 1 ];
  ufAssert( r3.getName() == str.str() );

  // Test the move constructor.
  RV_RegV r4 = std::move( r1 );

  ufAssert( r4.getType() == RV32I::RegisterType::reg );
  ufAssert( r4.getBitWidth() == 32 );
  ufAssert( r4.isVirtual() );
  ufAssert( !r4.isPhysical() );
  ufAssert( !r4.isInserted() );
  str.str( "" );
  str << RV32I::RegisterType::reg.getPrefixes()[ 1 ] << r4.getID()
      << RV32I::RegisterType::reg.getSuffixes()[ 1 ];
  ufAssert( r4.getName() == str.str() );

  // Test the move assignment operator.
  RV_RegV r5;
  r5 = std::move( r2 );

  ufAssert( r5.getType() == RV32I::RegisterType::reg );
  ufAssert( r5.getBitWidth() == 32 );
  ufAssert( r5.isVirtual() );
  ufAssert( !r5.isPhysical() );
  ufAssert( !r5.isInserted() );
  str.str( "" );
  str << RV32I::RegisterType::reg.getPrefixes()[ 1 ] << r5.getID()
      << RV32I::RegisterType::reg.getSuffixes()[ 1 ];
  ufAssert( r5.getName() == str.str() );

  return( 0 );
}
