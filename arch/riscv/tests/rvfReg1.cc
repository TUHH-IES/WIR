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
#include <arch/riscv/rv32if.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  // This declaration of an RV32IF processor is needed, since this internally
  // initializes the width of the floating-point registers to 32 bits.
  RV32IF p1;

  stringstream str;

  RV_FRegV fr1;
  ufAssert( fr1.getType() == RV32IF::RegisterType::fReg );
  ufAssert( fr1.getBitWidth() == 32 );
  ufAssert( fr1.isVirtual() );
  ufAssert( !fr1.isPhysical() );
  ufAssert( !fr1.isInserted() );
  str.str( "" );
  str << RV32IF::RegisterType::fReg.getPrefixes()[ 1 ] << fr1.getID()
      << RV32IF::RegisterType::fReg.getSuffixes()[ 1 ];
  ufAssert( fr1.getName() == str.str() );

  // Test the copy constructor.
  RV_FRegV fr2 = fr1;

  ufAssert( fr2.getType() == RV32IF::RegisterType::fReg );
  ufAssert( fr2.getBitWidth() == 32 );
  ufAssert( fr2.isVirtual() );
  ufAssert( !fr2.isPhysical() );
  ufAssert( !fr2.isInserted() );
  str.str( "" );
  str << RV32IF::RegisterType::fReg.getPrefixes()[ 1 ] << fr2.getID()
      << RV32IF::RegisterType::fReg.getSuffixes()[ 1 ];
  ufAssert( fr2.getName() == str.str() );

  // Test the copy assignment operator.
  RV_FRegV fr3;
  fr3 = fr1;

  ufAssert( fr3.getType() == RV32IF::RegisterType::fReg );
  ufAssert( fr3.getBitWidth() == 32 );
  ufAssert( fr3.isVirtual() );
  ufAssert( !fr3.isPhysical() );
  ufAssert( !fr3.isInserted() );
  str.str( "" );
  str << RV32IF::RegisterType::fReg.getPrefixes()[ 1 ] << fr3.getID()
      << RV32IF::RegisterType::fReg.getSuffixes()[ 1 ];
  ufAssert( fr3.getName() == str.str() );

  // Test the move constructor.
  RV_FRegV fr4 = std::move( fr1 );

  ufAssert( fr4.getType() == RV32IF::RegisterType::fReg );
  ufAssert( fr4.getBitWidth() == 32 );
  ufAssert( fr4.isVirtual() );
  ufAssert( !fr4.isPhysical() );
  ufAssert( !fr4.isInserted() );
  str.str( "" );
  str << RV32IF::RegisterType::fReg.getPrefixes()[ 1 ] << fr4.getID()
      << RV32IF::RegisterType::fReg.getSuffixes()[ 1 ];
  ufAssert( fr4.getName() == str.str() );

  // Test the move assignment operator.
  RV_FRegV fr5;
  fr5 = std::move( fr2 );

  ufAssert( fr5.getType() == RV32IF::RegisterType::fReg );
  ufAssert( fr5.getBitWidth() == 32 );
  ufAssert( fr5.isVirtual() );
  ufAssert( !fr5.isPhysical() );
  ufAssert( !fr5.isInserted() );
  str.str( "" );
  str << RV32IF::RegisterType::fReg.getPrefixes()[ 1 ] << fr5.getID()
      << RV32IF::RegisterType::fReg.getSuffixes()[ 1 ];
  ufAssert( fr5.getName() == str.str() );

  return( 0 );
}
