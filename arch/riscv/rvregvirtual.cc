/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvregvirtual.cc
  @brief This file implements virtual RISC-V RV32I registers.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
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
  Default constructor for virtual registers.
*/
RV_RegV::RV_RegV( void ) :
  WIR_VirtualRegister { RV32I::RegisterType::reg }
{
  DSTART( "RV_RegV::RV_RegV()" );
};


/*
  Copy constructor.
*/
RV_RegV::RV_RegV( const RV_RegV &__o ) :
  WIR_VirtualRegister { __o }
{
  DSTART( "RV_RegV::RV_RegV(const RV_RegV&)" );
};


/*
  Move constructor.
*/
RV_RegV::RV_RegV( RV_RegV &&__o ) :
  WIR_VirtualRegister { std::move( __o ) }
{
  DSTART( "RV_RegV::RV_RegV(RV_RegV&&)" );
};


/*
  Destructor.
*/
RV_RegV::~RV_RegV( void )
{
  DSTART( "virtual RV_RegV::~RV_RegV()" );
};


/*
  Copy-assignment operator.
*/
RV_RegV & RV_RegV ::operator = ( const RV_RegV &__o )
{
  DSTART( "RV_RegV& RV_RegV::operator=(const RV_RegV&)" );

  WIR_VirtualRegister::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV_RegV & RV_RegV::operator = ( RV_RegV &&__o )
{
  DSTART( "RV_RegV& RV_RegV::operator=(RV_RegV&&)" );

  WIR_VirtualRegister::operator = ( std::move( __o ) );

  return( *this );
};


//
// Protected class methods
//

/*
  clone creates a copy of a virtual RV32I register.
*/
RV_RegV *RV_RegV::clone( void ) const
{
  DSTART( "virtual RV_RegV* RV_RegV::clone() const" );

  return( new RV_RegV( *this ) );
};

}       // namespace WIR
