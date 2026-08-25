/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirconstprop.cc
  @brief This file implements a constant propagation optimization.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <string>

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>
#include <libuseful/io.h>

// Include WIR headers
#include <analyses/bit/wirbitdfa.h>
#include <wir/wir.h>

// Include local headers
#include "wirconstprop.h"


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
WIR_ConstProp::WIR_ConstProp( WIR_System &s ) :
  WIR_BitOpt { s }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for compilation unit-level optimization.
*/
WIR_ConstProp::WIR_ConstProp( WIR_CompilationUnit &c ) :
  WIR_BitOpt { c }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for function-level optimization.
*/
WIR_ConstProp::WIR_ConstProp( WIR_Function &f ) :
  WIR_BitOpt { f }
{
  DSTART( "WIR_ConstProp::WIR_ConstProp(WIR_Function&)" );
};


/*
  Destructor.
*/
WIR_ConstProp::~WIR_ConstProp( void )
{
  DSTART( "virtual WIR_ConstProp::~WIR_ConstProp()" );
};


//
// Protected class methods
//

/*
  runOptimization propagates constants in the given system.
*/
void WIR_ConstProp::runOptimization( WIR_System &s )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  for ( WIR_CompilationUnit &c : s )
    if ( !c.getDontOptimize() )
      runOptimization( c );
};


/*
  runOptimization propagates constants in the given compilation unit.
*/
void WIR_ConstProp::runOptimization( WIR_CompilationUnit &c )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  for ( WIR_Function &f : c )
    if ( !f.getDontOptimize() )
      runOptimization( f );
};


/*
  runOptimization propagates constants in the given function.
*/
void WIR_ConstProp::runOptimization( WIR_Function &f )
{
  DSTART( "virtual void WIR_ConstProp::runOptimization(WIR_Function&)" );

  // Perform bit-true data and value flow analysis first.
  if ( mRunDFA && ( mBitDFA != nullptr ) )
    mBitDFA->analyze();

  DOUT( "Processing function '" << f.getName() << "'." << endl );
  verifyLocations( f );

  for ( const WIR_BasicBlock &b : f ) {
    if ( b.getDontOptimize() )
      continue;

    // Check all instructions of b.
    for ( auto iIt = b.begin(); iIt != b.end(); ) {
      // Set up an iterator pointing to the next instruction after the current
      // one.
      auto nextInsIt = next( iIt );

      const WIR_Instruction &i = iIt->get();

      if ( i.getDontOptimize() ) {
        iIt = nextInsIt;
        continue;
      }

      bool instructionModified = false;

      // Check all operations of i.
      for ( const WIR_Operation &o : i ) {
        bool dontOptimizeParams =
          any_of(
            o.begin(), o.end(),
            [&]( const WIR_Parameter &p ) { return( p.getDontOptimize() ); } );

        // Propagate constants if possible.
        if ( !o.getDontOptimize() && !dontOptimizeParams && constProp( o ) ) {
          // Store the current instruction iterator for later efficient use by
          // addNewInstructions.
          mIterator[ o.getID() ] = iIt;

          instructionModified = true;
        }
      }

      // Add new instructions to the current WIR function.
      if ( instructionModified ) {
        addNewInstructions();

        // Update locations in up/down values if necessary.
        updateLocations( f );

        eraseInstructions();
      }

      iIt = nextInsIt;
    }
  }

  verifyLocations( f );
};


/*
  replace replaces all 'X' bits in the given up/down value by the specified L4
  value.
*/
signed long long WIR_ConstProp::replace( const WIR_UpDownValue &v,
                                         WIR_L4 b, bool s ) const
{
  DSTART(
    "long long int WIR_ConstProp::replace(const WIR_UpDownValue&, WIR_L4, "
    "bool) const" );

  auto val = v;
  val.setSignedness( s );

  for ( unsigned int i = 0; i < val.getBitWidth(); ++i )
    if ( val.at( i ) == WIR_L4::bX )
      val.setBit( i, b );

  return( val.getSignedValue() );
};


/*
  patchDefinedParameter updates the bit-values associated with a defined
  parameter of an operation subject to constant propagation.

  For the targets of all out-edges of p, patchDefinedParameter removes pOrig
  from the set of incoming bit-values and adds p instead.
*/
void WIR_ConstProp::patchDefinedParameter( const WIR_RegisterParameter &p,
                                           const WIR_Parameter &pOrig )
{
  DSTART(
    "void WIR_ConstProp::patchDefinedParameter(const WIR_RegisterParameter&, "
    "const WIR_Parameter&)" );

  // Patch incoming bit-values for all uses of p.
  auto &srcContainer = p.getContainers<WIR_BitValues>().begin()->get();

  // Iterate all outgoing edges of p.
  for ( auto &outEdge : srcContainer.getOutValues() ) {
    auto &tgtContainer =
      outEdge.rp->getContainers<WIR_BitValues>().begin()->get();
    auto inEdgeIt = tgtContainer.findInValues( pOrig );

    tgtContainer.insertInValues(
      const_cast<WIR_RegisterParameter &>( p ),
      WIR_UpDownValue( inEdgeIt->downVal ),
      WIR_UpDownValue( inEdgeIt->upVal ) );
  }

  // Ensure that all locations in the bit-wise DFG refering to the old
  // parameter pOrig will be updated for the new parameter p.
  mNewLocation.insert(
    make_pair(
      pOrig.getID(), ref( const_cast<WIR_RegisterParameter &>( p ) ) ) );
};


/*
  patchUsedParameter updates the bit-values associated with a used parameter of
  an operation subject to constant propagation.

  For the sources of all in-edges of p, patchUsedParameter adds p to the set of
  outgoing bit-values.
*/
void WIR_ConstProp::patchUsedParameter( const WIR_Parameter &p,
                                        const WIR_Parameter &pOrig ) const
{
  DSTART(
    "void WIR_ConstProp::patchUsedParameter(const WIR_Parameter&, const "
    "WIR_Parameter&) const" );

  // Patch outgoing bit-values for all definitions of p.
  auto &tgtContainer = p.getContainers<WIR_BitValues>().begin()->get();

  // Patch former self-inedges pOrig -> pOrig first.
  if ( dynamic_cast<const WIR_RegisterParameter &>( p ).isUsed() ) {
    auto it = tgtContainer.findInValues( pOrig );
    if ( it != tgtContainer.getInValues().end() )
      it->rp = &const_cast<WIR_Parameter &>( p );
  }

  // Iterate all incoming edges of p.
  for ( auto &inEdge : tgtContainer.getInValues() ) {
    // Skip self-inedges.
    if ( *(inEdge.rp) == p )
      continue;

    auto &srcContainer =
      inEdge.rp->getContainers<WIR_BitValues>().begin()->get();
    auto outEdgeIt = srcContainer.findOutValues( pOrig );

    srcContainer.insertOutValues(
      const_cast<WIR_Parameter &>( p ), WIR_UpDownValue( outEdgeIt->downVal ),
      WIR_UpDownValue( outEdgeIt->upVal ) );
  }
};


/*
  patchImmediateParameter creates a bit-value container for an immediate
  parameter newly created during constant propagation.
*/
void WIR_ConstProp::patchImmediateParameter( WIR_BaseImmediateParameter &p ) const
{
  DSTART(
    "void WIR_ConstProp::patchImmediateParameter(WIR_BaseImmediateParameter&) "
    "const" );

  auto *cont = new WIR_BitValues();
  p.insertContainer( cont );
  cont->insertInValues( p, WIR_UpDownValue( p ), WIR_UpDownValue( p ) );
};


/*
  checkSelfEdges checkes whether an original operation contains DFG edges
  starting and ending both at some of the operation's parameters. Such
  self-edges are then taken over to a new constant-propagated operation.
*/
void WIR_ConstProp::checkSelfEdges( WIR_Operation &o,
                                    const WIR_Operation &oOrig ) const
{
  DSTART(
    "void WIR_ConstProp::checkSelfEdges(WIR_Operation&, const WIR_Operation&) "
    "const" );

  for ( WIR_Parameter &pOrig : oOrig )
    if ( pOrig.getType() == WIR_ParameterType::reg ) {
      auto &rpOrig = dynamic_cast<WIR_RegisterParameter &>( pOrig );

      if ( ( rpOrig.isDefined() || rpOrig.isDefUsed() ) &&
           rpOrig.containsContainers( WIR_BitValues::getContainerTypeID() ) ) {
        // Get the register parameter's bitValue container.
        auto &cont = rpOrig.getContainers<WIR_BitValues>().begin()->get();

        // Iterate all outgoing edges of the curent register parameter.
        for ( auto &e : cont.getOutValues() ) {
          auto &tgtOrig =
            dynamic_cast<const WIR_RegisterParameter &>( *(e.rp) );

          if ( tgtOrig.getOperation() == oOrig ) {
            DACTION(
              stringstream sstr;
              sstr << wir << oOrig;
              DOUT(
                "Found self-edge '" << rpOrig << "' (ID " << rpOrig.getID() <<
                ") -> '" << tgtOrig << "' (ID " << tgtOrig.getID() <<
                ") in operation '" << sstr.str().substr( 8 ) <<
                "', down-value = " << e.downVal << ", up-value = " << e.upVal <<
                endl ); );

            // Find a definition of the same register and of same usage type in
            // the new constant-propagated operation.
            auto itDef = o.end();
            for ( auto it = o.begin(); it != o.end(); ++it )
              if ( it->get().getType() == WIR_ParameterType::reg ) {
                const auto &rp =
                  dynamic_cast<const WIR_RegisterParameter &>( it->get() );

                if ( ( rp.getUsage() == rpOrig.getUsage() ) &&
                     ( rp.getRegister() == rpOrig.getRegister() ) ) {
                  itDef = it;
                  break;
                }
              }

            // Find a use of the same register and of same usage type in
            // the new constant-propagated operation.
            auto itUse = o.end();
            for ( auto it = o.begin(); it != o.end(); ++it )
              if ( it->get().getType() == WIR_ParameterType::reg ) {
                const auto &rp =
                  dynamic_cast<const WIR_RegisterParameter &>( it->get() );

                if ( ( rp.getUsage() == tgtOrig.getUsage() ) &&
                     ( rp.getRegister() == tgtOrig.getRegister() ) ) {
                  itUse = it;
                  break;
                }
              }

            if ( ( itDef != o.end() ) && ( itUse != o.end() ) ) {
              DACTION(
                stringstream sstr;
                sstr << wir << o;
                DOUT(
                  "Adding self-edge '" << itDef->get() << "' (ID " <<
                  itDef->get().getID() << ") -> '" << itUse->get() <<
                  "' (ID " << itUse->get().getID() << ") in operation '" <<
                  sstr.str().substr( 8 ) << "', up-value = " << e.upVal <<
                  ", down-value = " << e.downVal << endl ); );

              auto &cDef =
                itDef->get().getContainers<WIR_BitValues>().begin()->get();
              auto &cUse =
                itUse->get().getContainers<WIR_BitValues>().begin()->get();

              cDef.insertOutValues(
                itUse->get(), WIR_UpDownValue( e.downVal ),
                WIR_UpDownValue( e.upVal ) );
              cUse.insertInValues(
                itDef->get(), WIR_UpDownValue( e.downVal ),
                WIR_UpDownValue( e.upVal ) );
            }
          }
        }
      }
    }
};


/*
  isSymbol checks whether an up/down value entirely refers to one WIR symbol.

  In order to refer completely to a symbol, all bits of the up/down value must
  be L, all locations must refer to one and the same symbol, and bit i in the
  given up/down value must refer to bit i of the symbol's address.
*/
bool WIR_ConstProp::isSymbol( const WIR_UpDownValue &v ) const
{
  DSTART( "bool WIR_ConstProp::isSymbol(const WIR_UpDownValue&) const" );

  WIR_id_t symID = nullid;

  for ( unsigned int i = 0; i < v.getBitWidth(); ++i ) {
    if ( v.at( i ) != WIR_L4::bL )
      return( false );

    auto &loc = v.getLocation( i );
    if ( loc.isRegisterParameter() )
      return( false );

    auto &sym = loc.getSymbol();
    if ( i == 0 )
      symID = sym.getID();
    else

    if ( ( sym.getID() != symID ) || ( loc.getBitPosition() != i ) )
      return( false );
  }

  return( true );
};


//
// Private class methods
//

/*
  constProp performs the generic, processor-independent parts of constant
  propagation.

  constProp basically combines the possibly several incoming up/down values per
  parameter into a single up/down value and calls the processor-specific virtual
  methods that check and finally realize propagation.
*/
bool WIR_ConstProp::constProp( const WIR_Operation &o )
{
  DSTART( "bool WIR_ConstProp::constProp(const WIR_Operation&)" );

  DACTION(
    stringstream sstr;
    sstr << wir << o;
    DOUT(
      "Checking '" << sstr.str().substr( 8 ) << "' (ID " << o.getID() << ")." <<
      endl; ); );

  // An operation's parameter can, depending on the operation's position in the
  // control flow, be reached via several DFG edges. Before the actual
  // optimization of the current operation, the (possibly several) up values per
  // operation parameter have to be combined into one single up value per
  // parameter.
  map<WIR_id_t, WIR_UpDownValue> inValue;

  // Iterate all explicit parameters of o and combine their incoming up values.
  for ( const WIR_Parameter &p : o.getExplicitParameters() )
    if ( p.getType() == WIR_ParameterType::reg ) {
      const auto &rp = dynamic_cast<const WIR_RegisterParameter &>( p );

      combineInValues( rp, inValue );
    }

  // Do the actual constant propagation.
  return( doConstProp( o, inValue ) );
};


/*
  addNewInstructions processes map mNewInstructions and adds the generated
  constant-propagated instructions immediately behind each original constant
  operation.
*/
void WIR_ConstProp::addNewInstructions( void )
{
  DSTART( "void WIR_ConstProp::addNewInstructions()" );

  for ( auto &[op, instrs] : mNewInstructions ) {
    // Retrieve an iterator before which the new instructions will be added.
    auto insertPos = std::next( mIterator[ op.get().getID() ] );

    // Retrieve the basic block to which the new instructions will be added.
    auto &o = op.get();
    auto &i = o.getInstruction();
    auto &b = i.getBasicBlock();
    auto pos = b.findInstruction( i );

    // Insert all new instructions into b at the current position.
    while ( !instrs.empty() ) {
      auto it = b.insertInstruction( insertPos, std::move( instrs.front() ) );
      instrs.pop_front();

      // Copy comments and file-infos.
      for ( WIR_Comment &c : i.getContainers<WIR_Comment>() )
        it->get().insertContainer( c );
      for ( WIR_FileInfo &c : i.getContainers<WIR_FileInfo>() )
        it->get().insertContainer( c );

      // For the very last newly inserted instruction, copy all containers, not
      // only comments and file-infos.
      if ( instrs.empty() )
        copyContainers( it->get(), i );

      DACTION(
        stringstream sstr0;
        sstr0 << wir << it->get();
        stringstream sstr1;
        sstr1 << wir << o;
        DOUT(
          "Adding new instruction '" <<
          sstr0.str().substr( 8, sstr0.str().size() - 9 ) <<
          "' after operation '" << sstr1.str().substr( 8 ) << "' (ID " <<
          o.getID() << ")." << endl; ); );
    }

    // Remove o's parameters properly from all adjacent bit-value containers.
    for ( WIR_Parameter &p1 : o )
      if ( p1.containsContainers( WIR_BitValues::getContainerTypeID() ) ) {
        // Get the parameter's bitValue container.
        auto &c = p1.getContainers<WIR_BitValues>().begin()->get();

        // Iterate all outgoing edges of p1.
        for ( auto &outEdge : c.getOutValues() ) {
          auto &tgtContainer =
            outEdge.rp->getContainers<WIR_BitValues>().begin()->get();
          tgtContainer.eraseInValues( p1 );
        }

        // Iterate all incoming edges of p1.
        for ( auto &inEdge : c.getInValues() ) {
          auto &srcContainer =
            inEdge.rp->getContainers<WIR_BitValues>().begin()->get();
          srcContainer.eraseOutValues( p1 );
        }
      }

    // Add o's instruction to the list of instructions to be erased.
    mErasePositions.push_back( pos );
  }

  mNewInstructions.clear();
  mIterator.clear();
};


/*
  eraseInstructions processes list mErasePositions and erases all instructions
  therein.
*/
void WIR_ConstProp::eraseInstructions( void )
{
  DSTART( "void WIR_ConstProp::eraseInstructions()" );

  while ( !mErasePositions.empty() ) {
    auto pos = mErasePositions.front();
    mErasePositions.pop_front();

    WIR_BasicBlock &b = pos->get().getBasicBlock();

    // Remove instruction from basic block b.
    DACTION(
      stringstream sstr;
      sstr << wir << pos->get();
      DOUT(
        "Removing original instruction '" <<
        sstr.str().substr( 8, sstr.str().size() - 9 ) << "' (ID " <<
        pos->get().getID() << ")." << endl; ); );
    b.eraseInstruction( pos );
  }
};

}       // namespace WIR
