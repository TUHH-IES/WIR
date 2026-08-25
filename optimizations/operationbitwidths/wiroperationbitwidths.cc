/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2025 - 2026, Heiko Falk.

*/

/*!
  @file wiroperationbitwidths.cc
  @brief This file implements a base class for optimizations reducing the bit
         widths of machine operations.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <sstream>

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>

// Include local headers
#include "wiroperationbitwidths.h"


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for system-level optimization.
*/
WIR_OperationBitWidths::WIR_OperationBitWidths( WIR_System &s ) :
  WIR_Optimization { s }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for compilation unit-level optimization.
*/
WIR_OperationBitWidths::WIR_OperationBitWidths( WIR_CompilationUnit &c ) :
  WIR_Optimization { c }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for function-level optimization.
*/
WIR_OperationBitWidths::WIR_OperationBitWidths( WIR_Function &f ) :
  WIR_Optimization { f }
{
  DSTART( "WIR_OperationBitWidths::WIR_OperationBitWidths(WIR_Function&)" );
};


/*
  Destructor.
*/
WIR_OperationBitWidths::~WIR_OperationBitWidths( void )
{
  DSTART( "virtual WIR_OperationBitWidths::~WIR_OperationBitWidths()" );
};


//
// Protected class methods
//

/*
  runOptimization reduces the bit widths of machine operations in the given
  system.
*/
void WIR_OperationBitWidths::runOptimization( WIR_System &s )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  for ( WIR_CompilationUnit &c : s )
    runOptimization( c );
};


/*
  runOptimization reduces the bit widths of machine operations in the given
  compilation unit.
*/
void WIR_OperationBitWidths::runOptimization( WIR_CompilationUnit &c )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  for ( WIR_Function &f : c )
    runOptimization( f );
};


/*
  runOptimization reduces the bit widths of machine operations in the given
  function.
*/
void WIR_OperationBitWidths::runOptimization( WIR_Function &f )
{
  DSTART(
    "virtual void WIR_OperationBitWidths::runOptimization(WIR_Function&)" );

  if ( !f.getDontOptimize() )
    for ( WIR_BasicBlock &b : f )
      if ( !b.getDontOptimize() )
        for ( WIR_Instruction &i : b )
          if ( !i.getDontOptimize() )
            for ( auto it = i.begin(); it != i.end(); ++it )
              if ( !( it->get().getDontOptimize() ) ) {
                auto res = reduce( i, it );
                if ( res !=
                       const_cast<list<reference_wrapper<WIR_Operation>> &>(
                         i.getOperations() ).end() )
                  it = res;
              }
};


/*
  replace replaces the first operation by the second.
*/
std::list<
  std::reference_wrapper<
    WIR_Operation>>::iterator WIR_OperationBitWidths::replace( std::list<
                                                                 std::reference_wrapper<
                                                                   WIR_Operation>>::const_iterator pos,
                                                               WIR_Operation &&n ) const
{
  auto &o = pos->get();

  DDECLARE( stringstream str );
  DACTION(
    stringstream str1;
    str1 << o;
    str << "Replacing '" << str1.str().substr( 8 ) << "' by '"; );

  // Copy implicit parameters first.
  for ( WIR_Parameter &p : o )
    if ( p.isImplicit() )
      n.pushBackParameter( p );

  // Copy containers.
  copyContainers( n, o );

  auto res = o.getInstruction().replaceOperation( pos, std::move( n ) );

  DACTION(
    stringstream str1;
    str1 << res->get();
    str << str1.str().substr( 8 ) << "'." << endl;
    DOUT( str.str() ) );

  return( res );
};

}       // namespace WIR
