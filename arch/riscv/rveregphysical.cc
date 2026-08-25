/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rveregphysical.cc
  @brief This file implements physical RISC-V pairs of registers.

  @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
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
RV_ERegP::~RV_ERegP( void )
{
  DSTART( "virtual RV_ERegP::~RV_ERegP()" );
};


//
// Private class methods
//

/*
  Default constructor for physical RV32I register pairs.
*/
RV_ERegP::RV_ERegP( const std::string &__s, bool __sp ) :
  WIR_PhysicalRegister { RV32I::RegisterType::eReg, __s, __sp }
{
  DSTART( "RV_ERegP::RV_ERegP(const string&, bool)" );

  ufAssertT(
    !__sp, "Register pair declared as stack pointer does not make sense." );
};


/*
  Copy constructor.
*/
RV_ERegP::RV_ERegP( const RV_ERegP &__o ) :
  WIR_PhysicalRegister { __o }
{
  DSTART( "RV_ERegP::RV_ERegP(const RV_ERegP&)" );
};


/*
  Copy-assignment operator.
*/
RV_ERegP & RV_ERegP::operator = ( const RV_ERegP &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  WIR_PhysicalRegister::operator = ( __o );

  return( *this );
};


/*
  Upon creation of RISC-V physical register pairs, addChilds adds the two
  specified physical registers as childs.
*/
void RV_ERegP::addChilds( const WIR_PhysicalRegister &c1,
                          const WIR_PhysicalRegister &c2 )
{
  DSTART(
    "void TC_ERegP::addChilds(const WIR_PhysicalRegister&, const "
    "WIR_PhysicalRegister&)" );

  pushBackChild( const_cast<WIR_PhysicalRegister &>( c1 ) );
  pushBackChild( const_cast<WIR_PhysicalRegister &>( c2 ) );
};


/*
  clone creates a copy of a physical RV32I register pair.
*/
RV_ERegP *RV_ERegP::clone( void ) const
{
  DSTART( "virtual RV_ERegP* RV_ERegP::clone() const" );

  return( new RV_ERegP( *this ) );
};

}       // namespace WIR
