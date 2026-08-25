/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rveregvirtual.cc
  @brief This file implements virtual RISC-V pairs of registers.

  @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
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
  Default constructor for virtual register pairs.
*/
RV_ERegV::RV_ERegV( void ) :
  WIR_VirtualRegister { RV32I::RegisterType::eReg }
{
  DSTART( "RV_ERegV::RV_ERegV()" );

  // Create two virtual child RV32I registers.
  pushBackChild( RV_RegV() );
  pushBackChild( RV_RegV() );
};


/*
  Copy constructor.
*/
RV_ERegV::RV_ERegV( const RV_ERegV &__o ) :
  WIR_VirtualRegister { __o }
{
  DSTART( "RV_ERegV::RV_ERegV(const RV_ERegV&)" );
};


/*
  Move constructor.
*/
RV_ERegV::RV_ERegV( RV_ERegV &&__o ) :
  WIR_VirtualRegister { std::move( __o ) }
{
  DSTART( "RV_ERegV::RV_ERegV(RV_ERegV&&)" );
};


/*
  Destructor.
*/
RV_ERegV::~RV_ERegV( void )
{
  DSTART( "virtual RV_ERegV::~RV_ERegV()" );
};


/*
  Copy-assignment operator.
*/
RV_ERegV & RV_ERegV ::operator = ( const RV_ERegV &__o )
{
  DSTART( "RV_ERegV& RV_ERegV::operator=(const RV_ERegV&)" );

  WIR_VirtualRegister::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV_ERegV & RV_ERegV::operator = ( RV_ERegV &&__o )
{
  DSTART( "RV_ERegV& RV_ERegV::operator=(RV_ERegV&&)" );

  WIR_VirtualRegister::operator = ( std::move( __o ) );

  return( *this );
};


//
// Protected class methods
//

/*
  clone creates a copy of a virtual RV32I register pair.
*/
RV_ERegV *RV_ERegV::clone( void ) const
{
  DSTART( "virtual RV_ERegV* RV_ERegV::clone() const" );

  return( new RV_ERegV( *this ) );
};

}       // namespace WIR
