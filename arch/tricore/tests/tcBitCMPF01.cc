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
  // operation CMP.F.

  TC131 p;
  WIR_CompilationUnit c;
  c.setName( __FILE__ );
  WIR_Function &f = c.pushBackFunction( WIR_Function { "main" } );
  WIR_BasicBlock &b1 = f.pushBackBasicBlock( {} );
  const TC_DRegP &D4 = p.D4();
  auto &dExt = f.pushBackVirtualRegister( TC_DRegV() );
  auto &tmp0 = f.pushBackVirtualRegister( TC_DRegV() );

  // This lambda serves for generating and inserting a TriCore operation.
  auto tcop = [&]( WIR_Operation &&o ) -> WIR_Operation & {
    auto &i = b1.pushBackInstruction( WIR_Instruction { WIR_Operation { o } } );
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

  // CMP.F
  auto &cmpf1 = tcop(
    { TC131::OpCode::CMP_F, TC131::OperationFormat::DDD_1,
      new WIR_RegisterParameter( tmp0, WIR_Usage::def ),
      new WIR_RegisterParameter( tmp0, WIR_Usage::use ),
      new WIR_RegisterParameter( dExt, WIR_Usage::use ) } );

  tcop(
    { TC131::OpCode::MOV_RR, TC131::OperationFormat::DD,
      new WIR_RegisterParameter( D4, WIR_Usage::def ),
      new WIR_RegisterParameter( tmp0, WIR_Usage::use ) } );

  // Do bit-true data flow analysis.
  TC_BitDFA a { f };
  a.analyze();

  // Evaluate analysis results.
  ufAssert( dval( cmpf1, 0 ).extract( 6, 26 ).containsOnlyBit( WIR_L4::b0 ) );
  ufAssert( dval( cmpf1, 0 ).extract( 0, 6 ).containsOnlyBit( WIR_L4::bL ) );

  return( 0 );
}
