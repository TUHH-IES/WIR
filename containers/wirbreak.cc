/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirbreak.cc
  @brief This file implements a %WIR container marking branches resulting from
         ANSI-C break statements.

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
WIR_Break::WIR_Break( void ) :
  WIR_Container<WIR_Break> {}
{
  DSTART( "WIR_Break::WIR_Break()" );
};


/*
  Destructor.
*/
WIR_Break::~WIR_Break( void )
{
  DSTART( "virtual WIR_Break::~WIR_Break()" );
};


/*
  isUnique returns whether break containers are unique, i.e., whether at most
  one instance of this container type can be attached to a WIR class.
*/
bool WIR_Break::isUnique( void ) const
{
  DSTART( "virtual bool WIR_Break::isUnique() const" );

  return( true );
};

}       // namespace WIR
