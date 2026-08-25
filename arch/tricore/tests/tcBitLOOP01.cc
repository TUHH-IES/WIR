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
#include <iterator>

// Include libuseful headers
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/tricore/tc131.h>
#include <arch/tricore/analyses/bit/tcbitdfa.h>


using namespace std;
using namespace WIR;


int main( void )
{
  WIR_Init();

  // This test case tests the bit-true top-down data flow analysis for TriCore
  // operation LOOP.

  TC131 p;
  WIR_CompilationUnit c;
  c.setName( __FILE__ );
  WIR_Function &f = c.pushBackFunction( WIR_Function { "main" } );
  WIR_BasicBlock &b1 = f.pushBackBasicBlock( {} );
  WIR_BasicBlock &b2 = f.pushBackBasicBlock( {} );
  const TC_ARegP &A4 = p.A4();
  auto &a42 = f.pushBackVirtualRegister( TC_ARegV() );

  // This lambda serves for generating and inserting a TriCore operation.
  auto tcop = [&]( WIR_Operation &&o, WIR_BasicBlock &bb ) -> WIR_Operation & {
    auto &i = bb.pushBackInstruction( WIR_Instruction { WIR_Operation { o } } );
    return( i.begin()->get() );
  };

  // This lambda serves to retrieve an operation's outgoing down value.
  auto dval = []( const WIR_Operation &o,
                  unsigned int pos ) -> const WIR_UpDownValue & {
    auto it = o.begin();
    std::advance( it, pos );
    auto &rp = dynamic_cast<WIR_RegisterParameter &>( it->get() );
    auto &c = rp.getContainers<WIR_BitValues>().begin()->get();
    return( c.getOutValues().begin()->downVal );
  };

  // Create WIR code.

  tcop(
    { TC131::OpCode::LEA, TC131::OperationFormat::AC18ABSA,
      new WIR_RegisterParameter( a42, WIR_Usage::def ),
      new TC_Const18_Unsigned( 42 ) }, b1 );

  // LOOP
  auto &loop1 = tcop(
    { TC131::OpCode::LOOP, TC131::OperationFormat::AL_3,
      new WIR_RegisterParameter( a42, WIR_Usage::defuse ),
      new WIR_LabelParameter( b1 ) }, b1 );

  tcop(
    { TC131::OpCode::MOV_AA, TC131::OperationFormat::SAA_1,
      new WIR_RegisterParameter( A4, WIR_Usage::def ),
      new WIR_RegisterParameter( a42, WIR_Usage::use ) }, b2 );

  // Do bit-true data flow analysis.
  TC_BitDFA a { f };
  a.analyze();

  // Evaluate analysis results.
  ufAssert( dval( loop1, 0 ).getSignedValue() == 41 );

  return( 0 );
}
