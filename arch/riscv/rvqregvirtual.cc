/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2025 - 2026, Heiko Falk.

*/

/*!
  @file rvqregvirtual.cc
  @brief This file implements virtual RISC-V quadruples of registers.

  @author Lennard Boysen <Lennard.Boysen@tuhh.de>
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
  Default constructor for virtual register quadruples.
*/
RV_QRegV::RV_QRegV( void ) :
  WIR_VirtualRegister { RV32I::RegisterType::qReg }
{
  DSTART( "RV_QRegV::RV_QRegV()" );

  // Create four virtual child RV32I registers.
  pushBackChild( RV_RegV() );
  pushBackChild( RV_RegV() );
  pushBackChild( RV_RegV() );
  pushBackChild( RV_RegV() );
};


/*
  Copy constructor.
*/
RV_QRegV::RV_QRegV( const RV_QRegV &__o ) :
  WIR_VirtualRegister { __o }
{
  DSTART( "RV_QRegV::RV_QRegV(const RV_QRegV&)" );
};


/*
  Move constructor.
*/
RV_QRegV::RV_QRegV( RV_QRegV &&__o ) :
  WIR_VirtualRegister { move( __o ) }
{
  DSTART( "RV_QRegV::RV_QRegV(RV_QRegV&&)" );
};


/*
  Destructor.
*/
RV_QRegV::~RV_QRegV( void )
{
  DSTART( "virtual RV_QRegV::~RV_QRegV()" );
};


/*
  Copy-assignment operator.
*/
RV_QRegV & RV_QRegV ::operator = ( const RV_QRegV &__o )
{
  DSTART( "RV_QRegV& RV_QRegV::operator=(const RV_QRegV&)" );

  WIR_VirtualRegister::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV_QRegV & RV_QRegV::operator = ( RV_QRegV &&__o )
{
  DSTART( "RV_QRegV& RV_QRegV::operator=(RV_QRegV&&)" );

  WIR_VirtualRegister::operator = ( move( __o ) );

  return( *this );
};


//
// Protected class methods
//

/*
  clone creates a copy of a virtual RV32I register quadruple.
*/
RV_QRegV *RV_QRegV::clone( void ) const
{
  DSTART( "virtual RV_QRegV* RV_QRegV::clone() const" );

  return( new RV_QRegV( *this ) );
};

}       // namespace WIR
