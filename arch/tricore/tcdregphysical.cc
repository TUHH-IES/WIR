/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcdregphysical.cc
  @brief This file implements physical TriCore data registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
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
  Destructor.
*/
TC_DRegP::~TC_DRegP( void )
{
  DSTART( "virtual TC_DRegP::~TC_DRegP()" );
};


//
// Private class methods
//

/*
  Default constructor for physical data registers.
*/
TC_DRegP::TC_DRegP( const std::string &__s, bool __sp ) :
  WIR_PhysicalRegister { TC13::RegisterType::dReg, __s, __sp }
{
  DSTART( "TC_DRegP::TC_DRegP(const string&, bool)" );
};


/*
  Copy constructor.
*/
TC_DRegP::TC_DRegP( const TC_DRegP &__o ) :
  WIR_PhysicalRegister { __o }
{
  DSTART( "TC_DRegP::TC_DRegP(const TC_DRegP&)" );
};


/*
  Copy-assignment operator.
*/
TC_DRegP & TC_DRegP::operator = ( const TC_DRegP &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_PhysicalRegister::operator = ( __o );

  return( *this );
};


/*
  clone creates a copy of a physical TriCore data register.
*/
TC_DRegP *TC_DRegP::clone( void ) const
{
  DSTART( "virtual TC_DRegP* TC_DRegP::clone() const" );

  return( new TC_DRegP( *this ) );
};

}       // namespace WIR
