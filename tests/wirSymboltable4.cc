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
    for ( WIR_Data &d : c.getData() )
      ids.insert( d.getID() );

  ufAssert( sys.getSymbols().size() == ids.size() );
  for ( WIR_Symbol &s : sys.getSymbols() ) {
    WIR_id_t id = s.getData().getID();

    ufAssert( ids.count( id ) == 1 );
  }

  for ( WIR_CompilationUnit &c : sys )
    for ( WIR_Data &d : c.getData() ) {
      ufAssert( sys.containsSymbol( d ) );
      ufAssert( sys.containsSymbol( d.getID() ) );
      ufAssert( sys.findSymbol( d ).getType() == WIR_SymbolType::data );
      ufAssert( sys.findSymbol( d ).getData() == d );
      ufAssert( sys.findSymbol( d.getID() ).getData() == d );
    }
};


int main( void )
{
  WIR_Init();

  WIR_TaskManager t;
  WIR_System sys( "genericmips.sys", t );
  WIR_CompilationUnit &c = sys.pushBackCompilationUnit( {} );

  ufAssert( sys.getSymbols().empty() );

  WIR_Data d1( "bla" );

  // Check validity of symbol table across insertion/removal/replacement of
  // compilation units in systems.
  c.pushBackData( d1 );
  c.pushFrontData( WIR_Data( d1 ) );
  c.pushFrontData( WIR_Data( d1 ) );
  c.pushBackData( WIR_Data( d1 ) );
  checkST( sys );

  c.popBackData();
  c.popFrontData();
  checkST( sys );

  auto it = c.getData().begin();
  ++it;
  it = c.insertData( it, WIR_Data( d1 ) );
  it = c.insertData( it, WIR_Data( d1 ) );
  it = c.insertData( it, WIR_Data( d1 ) );
  it = c.getData().begin();
  ++it;
  ++it;
  ++it;
  c.replaceData( it, WIR_Data( d1 ) );
  checkST( sys );

  it = c.getData().begin();
  ++it;
  ++it;
  c.eraseData( it );
  checkST( sys );

  c.clearData();
  checkST( sys );

  return( 0 );
}
