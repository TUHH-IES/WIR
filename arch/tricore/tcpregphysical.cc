/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcpregphysical.cc
  @brief This file implements physical TriCore extended address registers.

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
#include <libuseful/io.h>

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
TC_PRegP::~TC_PRegP( void )
{
  DSTART( "virtual TC_PRegP::~TC_PRegP()" );
};


//
// Private class methods
//

/*
  Default constructor for physical extended address registers.
*/
TC_PRegP::TC_PRegP( const std::string &__s, bool __sp ) :
  WIR_PhysicalRegister { TC13::RegisterType::pReg, __s, __sp }
{
  DSTART( "TC_PRegP::TC_PRegP(const string&, bool)" );
};


/*
  Copy constructor.
*/
TC_PRegP::TC_PRegP( const TC_PRegP &__o ) :
  WIR_PhysicalRegister { __o }
{
  DSTART( "TC_PRegP::TC_PRegP(const TC_PRegP&)" );
};


/*
  Copy-assignment operator.
*/
TC_PRegP & TC_PRegP::operator = ( const TC_PRegP &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_PhysicalRegister::operator = ( __o );

  return( *this );
};


/*
  Upon creation of TriCore physical register pairs, addChilds adds the two
  specified physical registers as childs.
*/
void TC_PRegP::addChilds( const WIR_PhysicalRegister &c1,
                          const WIR_PhysicalRegister &c2 )
{
  DSTART(
    "void TC_PRegP::addChilds(const WIR_PhysicalRegister&, const WIR_PhysicalRegister&)" );

  pushBackChild( const_cast<WIR_PhysicalRegister &>( c1 ) );
  pushBackChild( const_cast<WIR_PhysicalRegister &>( c2 ) );
};


/*
  clone creates a copy of a physical TriCore extended address register.
*/
TC_PRegP *TC_PRegP::clone( void ) const
{
  DSTART( "virtual TC_PRegP* TC_PRegP::clone() const" );

  return( new TC_PRegP( *this ) );
};

}       // namespace WIR
