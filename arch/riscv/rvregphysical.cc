/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvregphysical.cc
  @brief This file implements physical RISC-V RV32I registers.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
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
#include <arch/riscv/rv32i.h>


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
RV_RegP::~RV_RegP( void )
{
  DSTART( "virtual RV_RegP::~RV_RegP()" );
};


//
// Private class methods
//

/*
  Default constructor for physical RV32I registers.
*/
RV_RegP::RV_RegP( const std::string &__s, bool __sp ) :
  WIR_PhysicalRegister { RV32I::RegisterType::reg, __s, __sp }
{
  DSTART( "RV_RegP::RV_RegP(const string&, bool)" );
};


/*
  Copy constructor.
*/
RV_RegP::RV_RegP( const RV_RegP &__o ) :
  WIR_PhysicalRegister { __o }
{
  DSTART( "RV_RegP::RV_RegP(const RV_RegP&)" );
};


/*
  Copy-assignment operator.
*/
RV_RegP & RV_RegP::operator = ( const RV_RegP &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_PhysicalRegister::operator = ( __o );

  return( *this );
};


/*
  clone creates a copy of a physical RV32I register.
*/
RV_RegP *RV_RegP::clone( void ) const
{
  DSTART( "virtual RV_RegP* RV_RegP::clone() const" );

  return( new RV_RegP( *this ) );
};

}       // namespace WIR
