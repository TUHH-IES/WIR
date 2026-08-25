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

// Include standard headers
#include <sstream>

// Include WIR headers
#include <wir/wir.h>
#include <arch/tricore/tc131.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  stringstream str;

  TC_DRegV r1;
  ufAssert( r1.getType() == TC131::RegisterType::dReg );
  ufAssert( r1.getBitWidth() == 32 );
  ufAssert( r1.isVirtual() );
  ufAssert( !r1.isPhysical() );
  ufAssert( !r1.isInserted() );
  str.str( "" );
  str << TC131::RegisterType::dReg.getPrefixes()[ 1 ] << r1.getID()
      << TC131::RegisterType::dReg.getSuffixes()[ 1 ];
  ufAssert( r1.getName() == str.str() );

  // Test the copy constructor.
  TC_DRegV r2 = r1;

  ufAssert( r2.getType() == TC131::RegisterType::dReg );
  ufAssert( r2.getBitWidth() == 32 );
  ufAssert( r2.isVirtual() );
  ufAssert( !r2.isPhysical() );
  ufAssert( !r2.isInserted() );
  str.str( "" );
  str << TC131::RegisterType::dReg.getPrefixes()[ 1 ] << r2.getID()
      << TC131::RegisterType::dReg.getSuffixes()[ 1 ];
  ufAssert( r2.getName() == str.str() );

  // Test the copy assignment operator.
  TC_DRegV r3;
  r3 = r1;

  ufAssert( r3.getType() == TC131::RegisterType::dReg );
  ufAssert( r3.getBitWidth() == 32 );
  ufAssert( r3.isVirtual() );
  ufAssert( !r3.isPhysical() );
  ufAssert( !r3.isInserted() );
  str.str( "" );
  str << TC131::RegisterType::dReg.getPrefixes()[ 1 ] << r3.getID()
      << TC131::RegisterType::dReg.getSuffixes()[ 1 ];
  ufAssert( r3.getName() == str.str() );

  // Test the move constructor.
  TC_DRegV r4 = std::move( r1 );

  ufAssert( r4.getType() == TC131::RegisterType::dReg );
  ufAssert( r4.getBitWidth() == 32 );
  ufAssert( r4.isVirtual() );
  ufAssert( !r4.isPhysical() );
  ufAssert( !r4.isInserted() );
  str.str( "" );
  str << TC131::RegisterType::dReg.getPrefixes()[ 1 ] << r4.getID()
      << TC131::RegisterType::dReg.getSuffixes()[ 1 ];
  ufAssert( r4.getName() == str.str() );

  // Test the move assignment operator.
  TC_DRegV r5;
  r5 = std::move( r2 );

  ufAssert( r5.getType() == TC131::RegisterType::dReg );
  ufAssert( r5.getBitWidth() == 32 );
  ufAssert( r5.isVirtual() );
  ufAssert( !r5.isPhysical() );
  ufAssert( !r5.isInserted() );
  str.str( "" );
  str << TC131::RegisterType::dReg.getPrefixes()[ 1 ] << r5.getID()
      << TC131::RegisterType::dReg.getSuffixes()[ 1 ];
  ufAssert( r5.getName() == str.str() );

  return( 0 );
}
