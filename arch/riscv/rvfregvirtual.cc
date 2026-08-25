/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvfregvirtual.cc
  @brief This file implements virtual RISC-V RV32IF registers.

  @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
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
  Default constructor for virtual registers.
*/
RV_FRegV::RV_FRegV( void ) :
  WIR_VirtualRegister { RV32IF::RegisterType::fReg }
{
  DSTART( "RV_FRegV::RV_FRegV()" );
};


/*
  Copy constructor.
*/
RV_FRegV::RV_FRegV( const RV_FRegV &__o ) :
  WIR_VirtualRegister { __o }
{
  DSTART( "RV_FRegV::RV_FRegV(const RV_FRegV&)" );
};


/*
  Move constructor.
*/
RV_FRegV::RV_FRegV( RV_FRegV &&__o ) :
  WIR_VirtualRegister { std::move( __o ) }
{
  DSTART( "RV_FRegV::RV_FRegV(RV_FRegV&&)" );
};


/*
  Destructor.
*/
RV_FRegV::~RV_FRegV( void )
{
  DSTART( "virtual RV_FRegV::~RV_FRegV()" );
};


/*
  Copy-assignment operator.
*/
RV_FRegV & RV_FRegV ::operator = ( const RV_FRegV &__o )
{
  DSTART( "RV_FRegV& RV_FRegV::operator=(const RV_FRegV&)" );

  WIR_VirtualRegister::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV_FRegV & RV_FRegV::operator = ( RV_FRegV &&__o )
{
  DSTART( "RV_FRegV& RV_FRegV::operator=(RV_FRegV&&)" );

  WIR_VirtualRegister::operator = ( std::move( __o ) );

  return( *this );
};


//
// Protected class methods
//

/*
  clone creates a copy of a virtual RV32IF register.
*/
RV_FRegV *RV_FRegV::clone( void ) const
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  return( new RV_FRegV( *this ) );
};

}       // namespace WIR
