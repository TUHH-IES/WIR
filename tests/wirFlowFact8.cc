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
#include <list>
#include <utility>

// Include libuseful headers
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/generic/mips.h>


using namespace std;
using namespace WIR;


/*
  This file tests class WIR_Flowrestriction, it should exit normally, no
  assertion should fail.
*/

int main( void )
{
  WIR_Init();

  WIR_BasicBlock b1, b2, b3, b4, b5;

  // Test default constructor.
  WIR_FlowRestriction fr1;

  ufAssert( fr1.getType() == WIR_FlowFactType::flowrestriction );

  ufAssert( fr1.getLHS().size() == 0 );
  ufAssert( fr1.getRHS().size() == 0 );

  // This will also test isPartOfLeq and isPartOfGeq.
  ufAssert( fr1.isPartOfFlowFact( b1 ) == false );
  ufAssert( fr1.isSignificant() == false );

  // Test addToGeq.
  fr1.addToRHS( 1, b1 );
  ufAssert( fr1.isPartOfFlowFact( b1 ) == true );
  ufAssert( fr1.isPartOfRHS( b1 ) == true );
  ufAssert( fr1.isSignificant() == false );

  // Test addToLeq.
  fr1.addToLHS( 2, b2 );
  ufAssert( fr1.isPartOfFlowFact( b2 ) == true );
  ufAssert( fr1.isPartOfLHS( b2 ) == true );
  ufAssert( fr1.isSignificant() == true );

  // Test list getter-functions and internal data.
  ufAssert( fr1.getLHS().size() == 1 );
  ufAssert( fr1.getRHS().size() == 1 );

  auto leq1 = fr1.getLHS();
  auto geq1 = fr1.getRHS();
  ufAssert( geq1.front().second.get().getID() == b1.getID() );
  ufAssert( geq1.front().first == 1 );
  ufAssert( leq1.front().second.get().getID() == b2.getID() );
  ufAssert( leq1.front().first == 2 );

  // Add to existing summand via addToLeq.
  fr1.addToLHS( 3, b2 );
  leq1 = fr1.getLHS();
  ufAssert( leq1.size() == 1 );
  ufAssert( leq1.front().second.get().getID() == b2.getID() );
  ufAssert( leq1.front().first == 5 );

  // Test removal of a summand.
  ufAssert( fr1.eraseSummand( b2 ) == 5 );
  ufAssert( fr1.isPartOfFlowFact( b2 ) == false );
  ufAssert( fr1.isSignificant() == false );

  // Test exchange of a summand.
  ufAssert( fr1.isPartOfRHS( b1 ) == true );
  ufAssert( fr1.isPartOfLHS( b1 ) == false );
  fr1.replaceSummand( b1, b2 );
  ufAssert( fr1.isPartOfRHS( b1 ) == false );
  ufAssert( fr1.isPartOfRHS( b2 ) == true );
  geq1 = fr1.getRHS();
  ufAssert( geq1.size() == 1 );
  ufAssert( geq1.front().second.get().getID() == b2.getID() );
  ufAssert( geq1.front().first == 1 );

  // Test list initializers.
  list<pair<int, reference_wrapper<const WIR_BasicBlock>>> leq2, geq2;

  leq2.push_back( make_pair( 1, cref( b1 ) ) );
  leq2.push_back( make_pair( 2, cref( b2 ) ) );
  leq2.push_back( make_pair( 3, cref( b3 ) ) );
  geq2.push_back( make_pair( 4, cref( b4 ) ) );
  geq2.push_back( make_pair( 5, cref( b5 ) ) );

  // Test list copy constructor.
  WIR_FlowRestriction fr2( leq2, geq2 );

  // Test proper internal assignment.
  ufAssert(
    fr2.isPartOfLHS( b1 ) && fr2.isPartOfLHS( b2 ) && fr2.isPartOfLHS( b3 ) );
  ufAssert( fr2.isPartOfRHS( b4 ) && fr2.isPartOfRHS( b5 ) );

  // Test internal data structures.
  leq2 = fr2.getLHS();
  geq2 = fr2.getRHS();

  ufAssert( leq2.size() == 3 );
  auto leq2_it = leq2.begin();
  ufAssert( leq2_it->second.get().getID() == b1.getID() );
  ufAssert( leq2_it->first == 1 );
  ++leq2_it;
  ufAssert( leq2_it->second.get().getID() == b2.getID() );
  ufAssert( leq2_it->first == 2 );
  ++leq2_it;
  ufAssert( leq2_it->second.get().getID() == b3.getID() );
  ufAssert( leq2_it->first == 3 );

  auto geq2_it = geq2.begin();
  ufAssert( geq2_it->second.get().getID() == b4.getID() );
  ufAssert( geq2_it->first == 4 );
  ++geq2_it;
  ufAssert( geq2_it->second.get().getID() == b5.getID() );
  ufAssert( geq2_it->first == 5 );

  // Test list move constructor.
  list<pair<int, reference_wrapper<const WIR_BasicBlock>>> leq3( leq2 );
  list<pair<int, reference_wrapper<const WIR_BasicBlock>>> geq3( geq2 );

  WIR_FlowRestriction fr3( std::move( leq3 ), std::move( geq3 ) );

  // Test proper internal assignment.
  ufAssert(
    fr3.isPartOfLHS( b1 ) && fr3.isPartOfLHS( b2 ) && fr3.isPartOfLHS( b3 ) );
  ufAssert( fr3.isPartOfRHS( b4 ) && fr3.isPartOfRHS( b5 ) );

  // Test internal data structures.
  leq3 = fr3.getLHS();
  geq3 = fr3.getRHS();

  ufAssert( leq3.size() == 3 );
  auto leq3_it = leq3.begin();
  ufAssert( leq3_it->second.get().getID() == b1.getID() );
  ufAssert( leq3_it->first == 1 );
  ++leq3_it;
  ufAssert( leq3_it->second.get().getID() == b2.getID() );
  ufAssert( leq3_it->first == 2 );
  ++leq3_it;
  ufAssert( leq3_it->second.get().getID() == b3.getID() );
  ufAssert( leq3_it->first == 3 );

  auto geq3_it = geq3.begin();
  ufAssert( geq3_it->second.get().getID() == b4.getID() );
  ufAssert( geq3_it->first == 4 );
  ++geq3_it;
  ufAssert( geq3_it->second.get().getID() == b5.getID() );
  ufAssert( geq3_it->first == 5 );

  // Test equality operator.
  ufAssert( fr2.getID() != fr3.getID() );
  ufAssert( fr2.isEqual( fr3 ) );

  // Test bad input data.
  WIR_FlowRestriction fr4;

  fr4.addToRHS( 4, b4 );
  fr4.addToLHS( 0, b1 );
  ufAssert( fr4.isSignificant() == false );
  ufAssert( fr4.getLHS().size() == 0 );

  list<pair<int, reference_wrapper<const WIR_BasicBlock>>> leq_zero, geq_zero;

  leq_zero.push_back( make_pair( -1, cref( b1 ) ) );
  geq_zero.push_back( make_pair( 4, cref( b4 ) ) );

  WIR_FlowRestriction fr5( leq_zero, geq_zero );

  ufAssert( fr5.isSignificant() == false );
  ufAssert( fr5.getLHS().size() == 0 );
  ufAssert( fr5.getRHS().size() == 0 );

  WIR_FlowRestriction fr6( std::move( leq_zero ), std::move( geq_zero ) );

  ufAssert( fr5.isSignificant() == false );
  ufAssert( fr5.getLHS().size() == 0 );
  ufAssert( fr5.getRHS().size() == 0 );

  // Test copy constructor.
  WIR_FlowRestriction fr7( fr2 );

  // Test proper internal assignment.
  ufAssert(
    fr7.isPartOfLHS( b1 ) && fr7.isPartOfLHS( b2 ) && fr7.isPartOfLHS( b3 ) );
  ufAssert( fr7.isPartOfRHS( b4 ) && fr7.isPartOfRHS( b5 ) );

  // Test internal data structures.
  auto leq7 = fr7.getLHS();
  auto geq7 = fr7.getRHS();

  ufAssert( leq7.size() == 3 );
  auto leq7_it = leq7.begin();
  ufAssert( leq7_it->second.get().getID() == b1.getID() );
  ufAssert( leq7_it->first == 1 );
  ++leq7_it;
  ufAssert( leq7_it->second.get().getID() == b2.getID() );
  ufAssert( leq7_it->first == 2 );
  ++leq7_it;
  ufAssert( leq7_it->second.get().getID() == b3.getID() );
  ufAssert( leq7_it->first == 3 );

  auto geq7_it = geq7.begin();
  ufAssert( geq7_it->second.get().getID() == b4.getID() );
  ufAssert( geq7_it->first == 4 );
  ++geq7_it;
  ufAssert( geq7_it->second.get().getID() == b5.getID() );
  ufAssert( geq7_it->first == 5 );

  return( 0 );
};
