/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirloopexit.cc
  @brief This file implements a %WIR container marking branches that are regular
         exits of ANSI-C for and while-do loops.

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
  Default constructor.
*/
WIR_LoopExit::WIR_LoopExit( bool b ) :
  WIR_Container<WIR_LoopExit> {},
  mExplicitSuccIsExit { b }
{
  DSTART( "WIR_LoopExit::WIR_LoopExit(bool)" );
};


/*
  Destructor.
*/
WIR_LoopExit::~WIR_LoopExit( void )
{
  DSTART( "virtual WIR_LoopExit::~WIR_LoopExit()" );
};


/*
  isUnique returns whether loop exit containers are unique, i.e., whether at
  most one instance of this container type can be attached to a WIR class.
*/
bool WIR_LoopExit::isUnique( void ) const
{
  DSTART( "virtual bool WIR_LoopExit::isUnique() const" );

  return( true );
};


/*
  brief explicitSuccessorIsExit returns whether the explicit or the implicit
  successor of a branch is the loop exit.
*/
bool WIR_LoopExit::explicitSuccessorIsExit( void ) const
{
  DSTART( "bool WIR_LoopExit::explicitSuccessorIsExit() const" );

  return( mExplicitSuccIsExit );
};

}       // namespace WIR
