/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wircfganalysis.cc
  @brief This file implements generic CFG-based control flow analyses.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

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
  Default constructor for function-level analysis.
*/
WIR_ControlFlowAnalysis::WIR_ControlFlowAnalysis( WIR_Function &f ) :
  WIR_Analysis { f },
  WIR_CFG { f }
{
  DSTART( "WIR_ControlFlowAnalysis::WIR_ControlFlowAnalysis(WIR_Function&)" );
};


/*
  Destructor.
*/
WIR_ControlFlowAnalysis::~WIR_ControlFlowAnalysis( void )
{
  DSTART( "virtual WIR_ControlFlowAnalysis::~WIR_ControlFlowAnalysis()" );
};

}       // namespace WIR
