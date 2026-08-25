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
#include <set>

// Include libuseful headers
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/generic/mips.h>


using namespace std;
using namespace WIR;


void checkST( const WIR_System &sys )
{
  set<WIR_id_t> ids;

  for ( WIR_CompilationUnit &c : sys )
    for ( WIR_Function &f : c ) {
      ids.insert( f.getID() );
      for ( WIR_BasicBlock &b : f )
        ids.insert( b.getID() );
    }

  ufAssert( sys.getSymbols().size() == ids.size() );
  for ( WIR_Symbol &s : sys.getSymbols() ) {
    WIR_id_t id = nullid;

    if ( s.getType() == WIR_SymbolType::function )
      id = s.getFunction().getID();
    else
      id = s.getBasicBlock().getID();

    ufAssert( ids.count( id ) == 1 );
  }

  for ( WIR_CompilationUnit &c : sys )
    for ( WIR_Function &f : c ) {
      ufAssert( sys.containsSymbol( f ) );
      ufAssert( sys.containsSymbol( f.getID() ) );
      ufAssert( sys.findSymbol( f ).getType() == WIR_SymbolType::function );
      ufAssert( sys.findSymbol( f ).getFunction() == f );
      ufAssert( sys.findSymbol( f.getID() ).getFunction() == f );

      for ( WIR_BasicBlock &b : f ) {
        ufAssert( sys.containsSymbol( b ) );
        ufAssert( sys.containsSymbol( b.getID() ) );
        ufAssert( sys.findSymbol( b ).getType() == WIR_SymbolType::block );
        ufAssert( sys.findSymbol( b ).getBasicBlock() == b );
        ufAssert( sys.findSymbol( b.getID() ).getBasicBlock() == b );
      }
    }
};


int main( void )
{
  WIR_Init();

  WIR_Function f( "foo" );
  WIR_BasicBlock b;
  WIR_CompilationUnit c1;
  WIR_TaskManager t;
  WIR_System sys( "genericmips.sys", t );

  ufAssert( sys.getSymbols().empty() );

  f.pushBackBasicBlock( b );
  f.pushBackBasicBlock( WIR_BasicBlock() );
  c1.pushBackFunction( f );
  c1.pushBackFunction( WIR_Function( "bar" ) );

  // Check validity of symbol table across insertion/removal/replacement of
  // compilation units in systems.
  sys.pushBackCompilationUnit( c1 );
  sys.pushFrontCompilationUnit( WIR_CompilationUnit( c1 ) );
  sys.pushFrontCompilationUnit( WIR_CompilationUnit( c1 ) );
  sys.pushBackCompilationUnit( WIR_CompilationUnit( c1 ) );
  checkST( sys );

  sys.popBackCompilationUnit();
  sys.popFrontCompilationUnit();
  checkST( sys );

  auto it = sys.getCompilationUnits().begin();
  ++it;
  it = sys.insertCompilationUnit( it, WIR_CompilationUnit( c1 ) );
  it = sys.insertCompilationUnit( it, WIR_CompilationUnit( c1 ) );
  it = sys.insertCompilationUnit( it, WIR_CompilationUnit( c1 ) );
  it = sys.getCompilationUnits().begin();
  ++it;
  ++it;
  ++it;
  sys.replaceCompilationUnit( it, WIR_CompilationUnit( c1 ) );
  checkST( sys );

  it = sys.getCompilationUnits().begin();
  ++it;
  ++it;
  sys.eraseCompilationUnit( it );
  checkST( sys );

  sys.clearCompilationUnits();
  checkST( sys );

  return( 0 );
}
