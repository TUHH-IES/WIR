/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirunusedvregs.cc
  @brief This file implements an optimization removing unused virtual registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <set>

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>

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
  Default constructor for system-level optimization.
*/
WIR_UnusedVRegs::WIR_UnusedVRegs( WIR_System &s ) :
  WIR_Optimization { s }
{
  DSTART( "WIR_UnusedVRegs::WIR_UnusedVRegs(WIR_System&)" );
};


/*
  Default constructor for compilation unit-level optimization.
*/
WIR_UnusedVRegs::WIR_UnusedVRegs( WIR_CompilationUnit &c ) :
  WIR_Optimization { c }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for function-level optimization.
*/
WIR_UnusedVRegs::WIR_UnusedVRegs( WIR_Function &f ) :
  WIR_Optimization { f }
{
  DSTART( "WIR_UnusedVRegs::WIR_UnusedVRegs(WIR_Function&)" );
};


/*
  Destructor.
*/
WIR_UnusedVRegs::~WIR_UnusedVRegs( void )
{
  DSTART( "virtual WIR_UnusedVRegs::~WIR_UnusedVRegs()" );
};


//
// Protected class methods
//

/*
  runOptimization removes unused virtual registers in the given system.
*/
void WIR_UnusedVRegs::runOptimization( WIR_System &s )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  for ( WIR_CompilationUnit &c : s )
    runOptimization( c );
};


/*
  runOptimization removes unused virtual registers in the given compilation
  unit.
*/
void WIR_UnusedVRegs::runOptimization( WIR_CompilationUnit &c )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  for ( WIR_Function &f : c )
    runOptimization( f );
};


/*
  runOptimization removes unused virtual registers in the given function.
*/
void WIR_UnusedVRegs::runOptimization( WIR_Function &f )
{
  DSTART( "virtual void WIR_UnusedVRegs::runOptimization(WIR_Function&)" );

  set<WIR_id_t> usedVregs;

  // Collect all virtual registers occurring in the function.
  for ( WIR_BasicBlock &b : f )
    for ( WIR_Instruction &i : b )
      for ( WIR_Operation &o : i )
        for ( WIR_Parameter &p : o )
          if ( p.getType() == WIR_ParameterType::reg ) {
            WIR_BaseRegister &r =
              dynamic_cast<WIR_RegisterParameter &>( p ).getRegister();

            if ( r.isVirtual() ) {
              WIR_VirtualRegister &vreg =
                dynamic_cast<WIR_VirtualRegister &>( r );

              usedVregs.insert( vreg.getRoot().getID() );
            }
          }

  auto it = f.getVirtualRegisters().begin();
  while ( it != f.getVirtualRegisters().end() ) {
    if ( !usedVregs.count( (*it).get().getRoot().getID() ) )
      // The iterator points to an unused virtual register, remove it.
      it = f.eraseVirtualRegister( it );
    else
      ++it;
  }
};

}       // namespace WIR
