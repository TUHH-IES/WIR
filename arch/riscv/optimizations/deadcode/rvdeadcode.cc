/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvdeadcode.cc
  @brief This file implements a RISC-V-specific dead code elimination.

  @author Yom Ismail Tan <Yom.Tan@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32imfdc.h>
#include <arch/riscv/analyses/bit/rv32bitdfa.h>

// Include local headers
#include "rvdeadcode.h"


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
RV_DeadCode::RV_DeadCode( WIR_System &s ) :
  WIR_Optimization { s },
  WIR_DeadCode { s }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for compilation unit-level optimization.
*/
RV_DeadCode::RV_DeadCode( WIR_CompilationUnit &c ) :
  WIR_Optimization { c },
  WIR_DeadCode { c }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for function-level optimization.
*/
RV_DeadCode::RV_DeadCode( WIR_Function &f ) :
  WIR_Optimization { f },
  WIR_DeadCode { f }
{
  DSTART( "RV_DeadCode::RV_DeadCode(WIR_Function&)" );
};


/*
  Destructor.
*/
RV_DeadCode::~RV_DeadCode( void )
{
  DSTART( "virtual RV_DeadCode::~RV_DeadCode()" );
};


//
// Protected class methods
//

/*
  runOptimization eliminates dead code in the given function.
*/
void RV_DeadCode::runOptimization( WIR_Function &f )
{
  DSTART( "virtual void RV_DeadCode::runOptimization(WIR_Function&)" );

  RV32_BitDFA analyzer { f };
  setDFA( analyzer );

  WIR_DeadCode::runOptimization( f );
};

}       // namespace WIR
