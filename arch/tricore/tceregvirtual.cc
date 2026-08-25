/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tceregvirtual.cc
  @brief This file implements virtual TriCore extended data registers.

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
#include <arch/tricore/tc13.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for virtual extended data registers.
*/
TC_ERegV::TC_ERegV( void ) :
  WIR_VirtualRegister { TC13::RegisterType::eReg }
{
  DSTART( "TC_ERegV::TC_ERegV()" );

  // Create two virtual child data registers.
  pushBackChild( TC_DRegV() );
  pushBackChild( TC_DRegV() );
};


/*
  Copy constructor.
*/
TC_ERegV::TC_ERegV( const TC_ERegV &__o ) :
  WIR_VirtualRegister { __o }
{
  DSTART( "TC_ERegV::TC_ERegV(const TC_ERegV&)" );
};


/*
  Move constructor.
*/
TC_ERegV::TC_ERegV( TC_ERegV &&__o ) :
  WIR_VirtualRegister { std::move( __o ) }
{
  DSTART( "TC_ERegV::TC_ERegV(TC_ERegV&&)" );
};


/*
  Destructor.
*/
TC_ERegV::~TC_ERegV( void )
{
  DSTART( "virtual TC_ERegV::~TC_ERegV()" );
};


/*
  Copy-assignment operator.
*/
TC_ERegV & TC_ERegV::operator = ( const TC_ERegV &__o )
{
  DSTART( "TC_ERegV& TC_ERegV::operator=(const TC_ERegV&)" );

  WIR_VirtualRegister::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
TC_ERegV & TC_ERegV::operator = ( TC_ERegV &&__o )
{
  DSTART( "TC_ERegV& TC_ERegV::operator=(TC_ERegV&&)" );

  WIR_VirtualRegister::operator = ( std::move( __o ) );

  return( *this );
};


//
// Protected class methods
//

/*
  clone creates a copy of a virtual TriCore extended data register.
*/
TC_ERegV *TC_ERegV::clone( void ) const
{
  DSTART( "virtual TC_ERegV* TC_ERegV::clone() const" );

  return( new TC_ERegV( *this ) );
};

}       // namespace WIR
