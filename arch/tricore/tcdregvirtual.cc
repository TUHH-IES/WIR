/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcdregvirtual.cc
  @brief This file implements virtual TriCore data registers.

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
  Default constructor for virtual data registers.
*/
TC_DRegV::TC_DRegV( void ) :
  WIR_VirtualRegister { TC13::RegisterType::dReg }
{
  DSTART( "TC_DRegV::TC_DRegV()" );
};


/*
  Copy constructor.
*/
TC_DRegV::TC_DRegV( const TC_DRegV &__o ) :
  WIR_VirtualRegister { __o }
{
  DSTART( "TC_DRegV::TC_DRegV(const TC_DRegV&)" );
};


/*
  Move constructor.
*/
TC_DRegV::TC_DRegV( TC_DRegV &&__o ) :
  WIR_VirtualRegister { std::move( __o ) }
{
  DSTART( "TC_DRegV::TC_DRegV(TC_DRegV&&)" );
};


/*
  Destructor.
*/
TC_DRegV::~TC_DRegV( void )
{
  DSTART( "virtual TC_DRegV::~TC_DRegV()" );
};


/*
  Copy-assignment operator.
*/
TC_DRegV & TC_DRegV::operator = ( const TC_DRegV &__o )
{
  DSTART( "TC_DRegV& TC_DRegV::operator=(const TC_DRegV&)" );

  WIR_VirtualRegister::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
TC_DRegV & TC_DRegV::operator = ( TC_DRegV &&__o )
{
  DSTART( "TC_DRegV& TC_DRegV::operator=(TC_DRegV&&)" );

  WIR_VirtualRegister::operator = ( std::move( __o ) );

  return( *this );
};


//
// Protected class methods
//

/*
  clone creates a copy of a virtual TriCore data register.
*/
TC_DRegV *TC_DRegV::clone( void ) const
{
  DSTART( "virtual TC_DRegV* TC_DRegV::clone() const" );

  return( new TC_DRegV( *this ) );
};

}       // namespace WIR
