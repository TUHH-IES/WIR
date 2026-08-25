/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvfregphysical.cc
  @brief This file implements physical RISC-V RV32IF registers.

  @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
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
#include <arch/riscv/rv32if.h>


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
RV_FRegP::~RV_FRegP( void )
{
  DSTART( "virtual RV_FRegP::~RV_FRegP()" );
};


//
// Private class methods
//

/*
  Default constructor for physical RV32IF registers.
*/
RV_FRegP::RV_FRegP( const std::string &__s, bool __sp ) :
  WIR_PhysicalRegister { RV32IF::RegisterType::fReg, __s, __sp }
{
  DSTART( "RV_FRegP::RV_FRegP(const string&, bool)" );
};


/*
  Copy constructor.
*/
RV_FRegP::RV_FRegP( const RV_FRegP &__o ) :
  WIR_PhysicalRegister { __o }
{
  DSTART( "RV_FRegP::RV_FRegP(const RV_FRegP&)" );
};


/*
  Copy-assignment operator.
*/
RV_FRegP & RV_FRegP::operator = ( const RV_FRegP &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_PhysicalRegister::operator = ( __o );

  return( *this );
};


/*
  clone creates a copy of a physical RV32I register.
*/
RV_FRegP *RV_FRegP::clone( void ) const
{
  DSTART( "virtual RV_FRegP* RV_FRegP::clone() const" );

  return( new RV_FRegP( *this ) );
};

}       // namespace WIR
