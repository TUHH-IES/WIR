/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirflowrestriction.cc
  @brief This file implements %WIR flow restrictions.

  @author Til Mauersberger <Til.Mauersberger@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <algorithm>

// Include libuseful headers
#include <libuseful/debugmacros.h>
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor creating an empty flow restriction.

  The individual summands can be added using methods
  - addToLHS
  - addToRHS

  This flow fact is not automatically inserted into a WIR system.
*/
WIR_FlowRestriction::WIR_FlowRestriction( void ) :
  WIR_FlowFact {}
{
  DSTART( "WIR_FlowRestriction::WIR_FlowRestriction()" );
};


/*
  Constructor creating a non-empty flow restriction.

  This flow fact is not automatically inserted into a WIR system.
*/
WIR_FlowRestriction::WIR_FlowRestriction( const std::list<std::pair<int, std::reference_wrapper<const WIR_BasicBlock>>> &lhs,
                                          const std::list<std::pair<int, std::reference_wrapper<const WIR_BasicBlock>>> &rhs ) :
  WIR_FlowFact {}
{
  DSTART(
    "WIR_FlowRestriction::WIR_FlowRestriction(const list<pair<int, "
    "reference_wrapper<const WIR_BasicBlock> > >&, const list<pair<int, "
    "reference_wrapper<const WIR_BasicBlock> > >&)" );

  // Add all summands to left-hand side.
  for ( auto &[factor, bb] : lhs ) {
    // Check data for validity, discard flow restriction if errors occur.
    if ( factor < 0 ) {
      ufWarnMsg << ufFile()
                << "Invalid factor " << factor << " found for basic "
                << "block '" + bb.get().getName() + "',"
                << " clearing entire flow restriction." << endl;

      mLHS.clear();
      mRHS.clear();

      return;
    }

    addToLHS( factor, bb.get() );
  }

  // Add all summands to right-hand side.
  for ( auto &[factor, bb] : rhs ) {
    // Check data for validity, discard flow restriction if errors occur.
    if ( factor < 0 ) {
      ufWarnMsg << ufFile()
                << "Invalid factor " << factor << " found for basic "
                << "block '" + bb.get().getName() + "',"
                << " clearing entire flow restriction." << endl;

      mLHS.clear();
      mRHS.clear();

      return;
    }

    addToRHS( factor, bb.get() );
  }
};


/*
  Copy constructor.

  The copy will not be inserted in any WIR_System or referenced by any
  WIR_FlowFactRefs.
*/
WIR_FlowRestriction::WIR_FlowRestriction( const WIR_FlowRestriction &__o ) :
  WIR_FlowFact { __o },
  mLHS { __o.mLHS },
  mRHS { __o.mRHS }
{
  DSTART(
    "WIR_FlowRestriction::WIR_FlowRestriction(const WIR_FlowRestriction&)" );
};


/*
  Destructor.
*/
WIR_FlowRestriction::~WIR_FlowRestriction( void )
{
  DSTART( "virtual WIR_FlowRestriction::~WIR_FlowRestriction()" );

  if ( !isInserted() )
    return;

  // Remove this flow restriction from all relevant FlowfactRefs.
  auto removeRef = [this](
    const pair<int, reference_wrapper<const WIR_BasicBlock>> &s ) {
    eraseReference( s.second );
  };

  for_each( mLHS.begin(), mLHS.end(), removeRef );
  for_each( mRHS.begin(), mRHS.end(), removeRef );
};


/*
  Copy-assignment operator.

  The copy will not be inserted in any WIR_System or referenced by any
  WIR_FlowFactRefs.
*/
WIR_FlowRestriction & WIR_FlowRestriction::operator = ( const WIR_FlowRestriction &__o )
{
  DSTART(
    "WIR_FlowRestriction& WIR_FlowRestriction::operator=(const "
    "WIR_FlowRestriction&)" );

  WIR_FlowFact::operator = ( __o );

  mLHS = __o.mLHS;
  mRHS = __o.mRHS;

  return( *this );
};


/*
  getType returns the type of a %WIR flow fact, i.e., whether it is an entry
  point, flow restriction or loop bound.
*/
WIR_FlowFactType WIR_FlowRestriction::getType( void ) const
{
  DSTART( "virtual WIR_FlowFactType WIR_FlowRestriction::getType() const" );

  return( WIR_FlowFactType::flowrestriction );
};


/*
  The << operator dumps a WIR flow restriction to an output stream.
*/
std::ostream & operator << ( std::ostream &os, const WIR_FlowRestriction &f )
{
  DSTART( "ostream& operator<<(ostream&, const WIR_FlowRestriction&)" );

  // Write preamble.
  os << "Flow restriction: ";

  // Write left-hand side.
  for ( auto it = f.mLHS.begin(); it != f.mLHS.end(); ++it ) {
    if ( it != f.mLHS.begin() )
      os << " + ";
    os << it->first << " * " << it->second.get().getName();
  }

  // Write comparator.
  os << " <= ";

  // Write right-hand side.
  for ( auto it = f.mRHS.begin(); it != f.mRHS.end(); ++it ) {
    if ( it != f.mRHS.begin() )
      os << " + ";
    os << it->first << " * " << it->second.get().getName();
  }

  // Flush output stream.
  os << endl;

  return( os );
};


/*
  addToLeq adds a summand to a flow restriction's left-hand side.

  The basic block with the given integer factor (negative factors add the
  summand to the right-hand side) is added to the left-hand side. If factor is
  zero, nothing changes internally and the summand will not be regarded.

  This is the only method manipulating the internal data structures (and by this
  invokes the update mechanisms of class FlowFactRef).
*/
void WIR_FlowRestriction::addToLHS( int factor, const WIR_BasicBlock &b )
{
  DSTART( "void WIR_FlowRestriction::addToLHS(int, const WIR_BasicBlock&)" );

  // Check data for validity.
  if ( factor == 0 ) {
    ufWarnMsg << ufFile()
              << "Invalid attempt to add basic block '" + b.getName() + "'"
              << " with factor " << dec << factor << " to flow restriction, "
              << "ignoring." << endl;
    return;
  }

  DOUT( "BB ID is: " << b.getID() << endl );

  for ( auto lhsIt = mLHS.begin(); lhsIt != mLHS.end(); ++lhsIt ) {
    // cppcheck-suppress variableScope
    auto &[lhsFactor, lhsBB] = *lhsIt;

    if ( b == lhsBB ) {
      DOUT( "Found BB " << b.getID() << " on left-hand side." << endl );

      // The given bb was found, now change its factor.
      lhsFactor += factor;

      // Analyze situation.
      if ( lhsFactor < 0 ) {
        mRHS.push_back( make_pair( -lhsFactor, lhsBB ) );
        mLHS.erase( lhsIt );
      } else

      if ( lhsFactor == 0 ) {
        eraseReference( b );
        mLHS.erase( lhsIt );
      }

      return;
    }
  }

  for ( auto rhsIt = mRHS.begin(); rhsIt != mRHS.end(); ++rhsIt ) {
    // cppcheck-suppress variableScope
    auto &[rhsFactor, rhsBB] = *rhsIt;

    if ( b == rhsBB ) {
      DOUT( "Found BB " << b.getID() << " on right-hand side." << endl );

      // The given bb was found, now change its factor.
      rhsFactor -= factor;

      // Analyze situation.
      if ( rhsFactor < 0 ) {
        mLHS.push_back( make_pair( -rhsFactor, rhsBB ) );
        mRHS.erase( rhsIt );
      } else

      if ( rhsFactor == 0 ) {
        eraseReference( b );
        mRHS.erase( rhsIt );
      }

      return;
    }
  }

  // Basic block was not found in lists, add new summand to this flow
  // restriction.
  if ( factor > 0 ) {
    DOUT(
      "Creating new pair on left-hand side: " << factor << " , " << b.getID() <<
      endl );
    mLHS.push_back( make_pair( factor, cref( b ) ) );
  } else {
    // Negative factor means to add to right-hand side.
    DOUT(
      "Creating new pair on right-hand side: " << -factor << " , " <<
      b.getID() << endl );
    mRHS.push_back( make_pair( -factor, cref( b ) ) );
  }

  addReference( b );
};


/*
  addToGeq adds a summand to a flow restriction's right-hand side.

  The basic block with the given integer as factor (negative factors will add
  the summand to the left-hand side) is added to the right-hand side.
*/
void WIR_FlowRestriction::addToRHS( int factor, const WIR_BasicBlock &b )
{
  DSTART( "void WIR_FlowRestriction::addToRHS(int, const WIR_BasicBlock&)" );

  addToLHS( -factor, b );
};


/*
  eraseSummand erases a summand from a flow restriction.

  A whole summand is removed from a flow restriction.
*/
int WIR_FlowRestriction::eraseSummand( const WIR_BasicBlock &b )
{
  DSTART( "int WIR_FlowRestriction::eraseSummand(const WIR_BasicBlock&)" );

  // Iterate left-hand side.
  for ( const auto &[factor, lhsBB] : mLHS )
    // Check if bb is found.
    if ( b == lhsBB ) {
      int ret = factor;

      // Negate factor to remove the whole summand.
      addToLHS( -factor, lhsBB );
      return( ret );
    }

  // Iterate right-hand side.
  for ( const auto &[factor, rhsBB] : mRHS )
    // Check if bb is found.
    if ( b == rhsBB ) {
      int ret = factor;

      // Negate factor to remove the whole summand.
      addToRHS( -factor, rhsBB );
      return( ret );
    }

  return( 0 );
};


/*
  replaceSummand replaces a summand in a flow restriction.

  The factor of the summand remains unchanged (no differentiation between left-
  and right-hand side).
*/
void WIR_FlowRestriction::replaceSummand( const WIR_BasicBlock &bOld,
                                          const WIR_BasicBlock &bNew )
{
  DSTART(
    "void WIR_FlowRestriction::replaceSummand(const WIR_BasicBlock&, "
    "const WIR_BasicBlock&)" );

  // Remember from which side of the inequation the summand was.
  bool wasLHS = isPartOfLHS( bOld );

  // Erase old summand, remember its factor.
  int factor = eraseSummand( bOld );

  DOUT(
    "The BB to exchange was on " << string( wasLHS ? "left" : "right" ) <<
    "-hand side" << " with factor " << factor << "." << endl );

  // Add new summand.
  if ( wasLHS )
    addToLHS( factor, bNew );
  else
    addToRHS( factor, bNew );

  return;
};


/*
  getLHS returns the left-hand side of a flow restriction.
*/
const list<pair<int, reference_wrapper<const WIR_BasicBlock>>> &WIR_FlowRestriction::getLHS( void ) const
{
  DSTART(
    "const list<pair<int, reference_wrapper<const WIR_BasicBlock> > >& "
    "WIR_FlowRestriction::getLHS() const" );

  return( mLHS );
};


/*
  getRHS returns the right-hand side of a flow restriction.
*/
const list<pair<int, reference_wrapper<const WIR_BasicBlock>>> &WIR_FlowRestriction::getRHS( void ) const
{
  DSTART(
    "const list<pair<int, reference_wrapper<const WIR_BasicBlock> > >& "
    "WIR_FlowRestriction::getRHS() const" );

  return( mRHS );
};


/*
  isPartOfLHS tests whether a basic block belongs to the left-hand side.
*/
bool WIR_FlowRestriction::isPartOfLHS( const WIR_BasicBlock &b ) const
{
  DSTART(
    "bool WIR_FlowRestriction::isPartOfLHS(const WIR_BasicBlock&) const" );

  return(
    any_of(
      mLHS.begin(), mLHS.end(),
      [&]( const pair<int, reference_wrapper<const WIR_BasicBlock>> &summand ) {
        return( b == summand.second.get() ); } ) );
};


/*
  isPartOfRHS tests whether a basic block belongs to the right-hand side.
*/
bool WIR_FlowRestriction::isPartOfRHS( const WIR_BasicBlock &b ) const
{
  DSTART(
    "bool WIR_FlowRestriction::isPartOfRHS(const WIR_BasicBlock&) const" );

  return(
    any_of(
      mRHS.begin(), mRHS.end(),
      [&]( const pair<int, reference_wrapper<const WIR_BasicBlock>> &summand ) {
        return( b == summand.second.get() ); } ) );
};


/*
  isPartOfFlowFact tests whether a basic block is part of this flow restriction.
*/
bool WIR_FlowRestriction::isPartOfFlowFact( const WIR_BasicBlock &b ) const
{
  DSTART(
    "bool WIR_FlowRestriction::isPartOfFlowFact(const WIR_BasicBlock&) const" );

  return( isPartOfLHS( b ) || isPartOfRHS( b ) );
};


/*
  isSignificant returns whether a flow restriction is significant for WCET
  calculation or not.

  In some cases, a flow fact may not be significant for WCET calculation, e.g.:
  - A flow restriction with 0 <= SUM.
  In such cases, this method returns false, in all other cases true.
*/
bool WIR_FlowRestriction::isSignificant( void ) const
{
  DSTART( "virtual bool WIR_FlowRestriction::isSignificant() const" );

  // If the left-hand side is empty, this has no significant information,
  // because 0 <= SUM is always true for positive factors and positive decision
  // variables.
  return( !mLHS.empty() );
};


/*
  isEqual returns true iff this flow restriction is equal to the specified one.

  Two flow restrictions r and r' are equal iff
    - each summand of the left-hand side of r is equal to the summand of r' at
      the same position of the left-hand side of r', and
    - each summand of the right-hand side of r is equal to the summand of r' at
      the same position of the right-hand side of r', and
  Two summands s and s' are equal iff
    - they both have the same factor, and
    - the LABELS of the referenced basic block are identical.   (!)
*/
bool WIR_FlowRestriction::isEqual( const WIR_FlowRestriction &__o ) const
{
  DSTART(
    "bool WIR_FlowRestriction::isEqual(const WIR_FlowRestriction&) const" );

  // Create a comparator for summands according to the requirements.
  using Summand =
    std::pair<int, std::reference_wrapper<const WIR::WIR_BasicBlock>>;

  auto cmpSummands = []( const Summand &s1, const Summand &s2 ) -> bool {
    // Compare factors first.
    if ( s1.first != s2.first )
      return( false );

    // Compare basic block labels next.
    if ( s1.second.get().getName() != s2.second.get().getName() )
      return( false );

    return( true );
  };

  // std::equal returns false for different lengths of the input ranges, it thus
  // does the job of comparing our two lists perfectly.
  return (
    equal(
      mLHS.begin(), mLHS.end(), __o.mLHS.begin(), __o.mLHS.end(),
      cmpSummands ) &&
    equal(
      mRHS.begin(), mRHS.end(), __o.mRHS.begin(), __o.mRHS.end(),
      cmpSummands ) );
};


/*
  reorganize adjusts all references to WIR basic blocks stored by a flow fact
  after a deep copy of flow facts.
*/
void WIR_FlowRestriction::reorganize( const std::map<WIR_id_t, WIR_BasicBlock *> &blockIDMap )
{
  DSTART(
    "virtual void WIR_FlowRestriction::reorganize(const map<long long unsigned "
    "int, WIR_BasicBlock*>&)" );

  auto exchangeBBReferences = [&blockIDMap]( auto &summand ) -> void {
    // Look for current basic block in the map.
    auto block_it = blockIDMap.find( summand.second.get().getID() );
    if ( block_it == blockIDMap.end() )
      return;

    // Change the reference to the new basic block provided by the map.
    summand.second = cref( *(block_it->second) );
  };

  for_each( mLHS.begin(), mLHS.end(), exchangeBBReferences );
  for_each( mRHS.begin(), mRHS.end(), exchangeBBReferences );
};


/*
  checkFlowFact inspects a flow restriction in depth and verifies that all flow
  fact-related data structures addressing this flow restriction are in a
  coherent state.
*/
bool WIR_FlowRestriction::checkFlowFact( const WIR_System &s ) const
{
  DSTART(
    "virtual bool WIR_FlowRestriction::checkFlowFact(const WIR_System&) "
    "const" );

  DOUT(
    "Checking flow restriction " << hex << this << dec << " (ID " << getID() <<
    ")." << endl );

  // Do a few general checks that apply to all kinds of flow facts.
  if ( !isInserted() ) {
    DOUT( "  Flow restriction is not inserted into some WIR system." << endl );
    return( false );
  }

  if ( mSystemPointer != &s ) {
    DOUT(
      "  Mismatch of system pointers found: mSystemPointer = " << hex <<
      mSystemPointer << ", &s = " << &s << dec << "." << endl );
    return( false );
  }

  // Do flow restriction-specific checks.
  WIR_BasicBlockSet bbSet;

  DOUT( "  mLHS refers to" );
  for ( auto &p : mLHS ) {
    auto &b = p.second.get();

    DOUT( " '" << b.getName() << "'" );
    bbSet.insert( const_cast<WIR_BasicBlock &>( b ) );
  }
  DOUT( endl );

  DOUT( "  mRHS refers to" );
  for ( auto &p : mRHS ) {
    auto &b = p.second.get();

    DOUT( " '" << b.getName() << "'" );
    bbSet.insert( const_cast<WIR_BasicBlock &>( b ) );
  }
  DOUT( endl );

  // Check that all refered basic blocks contain a reference to this flow
  // restriction.
  for ( const WIR_BasicBlock &b : bbSet ) {
    if ( !b.containsContainers( WIR_FlowFactRef::getContainerTypeID() ) ) {
      DOUT (
        "  '" << b.getName() << "' does not have a flow fact reference." <<
        endl );
      return( false );
    }

    // Get the basic block's FlowFactRef.
    auto &ref = b.getContainers<WIR_FlowFactRef>().begin()->get();
    DOUT(
      "  FlowFactRef associated with '" << b.getName() << "' is " << hex <<
      &ref << dec << " (ID " << ref.getID() << ")." << endl );

    // Check that all these flow fact references refer to this flow restriction
    // exactly once.
    unsigned int cnt =
      count_if(
        ref.getFlowFacts().begin(), ref.getFlowFacts().end(),
        [&]( reference_wrapper<WIR_FlowFact> ref ) {
          return( &(ref.get()) == this ); } );

    if ( cnt != 1 ) {
      DOUT(
        "  Found " << cnt << " references to the current flow restriction " <<
        "in FlowFactRef." << endl );
      return( false );
    }

    // Check that no other FlowFactRef in the whole WIR system refers to this
    // flow restriction at all.
    for ( WIR_CompilationUnit &c : s )
      for ( WIR_Function &f : c )
        for ( WIR_BasicBlock &bb : f )
          if ( !bbSet.count( bb ) &&
               bb.containsContainers(
                 WIR_FlowFactRef::getContainerTypeID() ) ) {
            auto &bRef = bb.getContainers<WIR_FlowFactRef>().begin()->get();

            if ( any_of(
                   bRef.getFlowFacts().begin(), bRef.getFlowFacts().end(),
                   [&]( reference_wrapper<WIR_FlowFact> ref ) {
                     return( &(ref.get()) == this ); } ) ) {
                DOUT(
                  "  Other FlowFactRef " << hex << &bRef << dec << " (ID " <<
                  bRef.getID() << ") also refers to this flow restriction." <<
                  endl );
                return( false );
              }
          }
  }

  return( true );
};


//
// Protected class methods
//

/*
  onInsert is called whenever this flow restriction is added to a WIR_System.

  This method overrides the method defined by the base class to add references
  of itself to all basic block's FlowFactRefs that this flow restriction is made
  up of.
*/
void WIR_FlowRestriction::onInsert( WIR_System *s )
{
  DSTART( "virtual void WIR_FlowRestriction::onInsert(WIR_System*)" );

  // Call base class implementation to set the pointer.
  WIR_FlowFact::onInsert( s );

  auto insertRef = [this]( const auto &s ) {
    addReference( s.second );
  };

  for_each( mLHS.begin(), mLHS.end(), insertRef );
  for_each( mRHS.begin(), mRHS.end(), insertRef );
};


/*
  clone creates a copy of a WIR flow restriction.

  This method only calls the copy constructor and allocates a new WIR flow
  restriction on the heap.
*/
WIR_FlowFact *WIR_FlowRestriction::clone( void ) const
{
  DSTART( "virtual WIR_FlowFact* WIR_FlowRestriction::clone() const" );

  return( new WIR_FlowRestriction( *this ) );
};

}      // namespace WIR
