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
#include <vector>

// Include WIR headers
#include <wir/wir.h>
#include <arch/arm/armv5tej.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  ARMv5TEJ p;
  const ARM_HiRegP &r8 = p.R8();

  cout.iword( WIR_Indentation() ) = 8;
  cout << arm;

  const vector<ARMv4::Condition> conditions {
    ARMv4::Condition::eq,
    ARMv4::Condition::ne,
    ARMv4::Condition::hs,
    ARMv4::Condition::lo,
    ARMv4::Condition::mi,
    ARMv4::Condition::pl,
    ARMv4::Condition::vs,
    ARMv4::Condition::vc,
    ARMv4::Condition::hi,
    ARMv4::Condition::ls,
    ARMv4::Condition::ge,
    ARMv4::Condition::lt,
    ARMv4::Condition::gt,
    ARMv4::Condition::le,
    ARMv4::Condition::al };

  // The following operations must be accepted for the ARMv5TEJ ISA.
  for ( auto &cond : conditions ) {
    WIR_Operation o(
      ARMv5TEJ::OpCode::BXJ, ARMv4T::OperationFormat::CR_3,
      WIR_ConditionFieldParameter( cond ),
      WIR_RegisterParameter( r8, WIR_Usage::use ) );
    cout << o << endl;
  }

  return( 0 );
}
