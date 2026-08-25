/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirblockschedulingregion.cc
  @brief This file implements a class representing basic block regions in which
         scheduling is performed.

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

// Include local headers
#include "wirblockschedulingregion.h"


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor.
*/
WIR_BlockSchedulingRegion::WIR_BlockSchedulingRegion( WIR_BasicBlock &b,
                                                      bool verbosity,
                                                      bool keepTmpFiles ) :
  WIR_SchedulingRegion { b, verbosity, keepTmpFiles }
{
  DSTART(
    "WIR_BlockSchedulingRegion::WIR_BlockSchedulingRegion(WIR_BasicBlock&, bool, bool)" );
};


/*
  Destructor.
*/
WIR_BlockSchedulingRegion::~WIR_BlockSchedulingRegion( void )
{
  DSTART( "virtual WIR_BlockSchedulingRegion::~WIR_BlockSchedulingRegion()" );
};

}       // namespace WIR
