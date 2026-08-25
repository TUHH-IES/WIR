/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcasmtemplateregister.cc
  @brief This file implements template register arguments.

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
#include <arch/tricore/tc131.h>

// Include local headers
#include "tcasmregisterinitializer.h"
#include "tcasmtemplateregister.h"


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor creating an assembly argument for a given WIR register and
  type.

  This constructor takes over the ownership of the given pointer. The object
  pointed to will automatically be deleted during destruction of this template
  register argument.
*/
TC_AsmTemplateRegister::TC_AsmTemplateRegister( TC_AsmRegisterInitializer *init ) :
  TC_AsmRegister {},
  mTemplateRegisterData { new templateRegisterData( init ) }
{
  DSTART(
    "TC_AsmTemplateRegister::TC_AsmTemplateRegister(TC_AsmRegisterInitializer*)" );

  setType( Type::EREG );
};


/*
  Copy constructor.
*/
TC_AsmTemplateRegister::TC_AsmTemplateRegister( const TC_AsmTemplateRegister &r ) :
  TC_AsmRegister { r },
  mTemplateRegisterData { r.mTemplateRegisterData }
{
  DSTART(
    "TC_AsmTemplateRegister::TC_AsmTemplateRegister(const TC_AsmTemplateRegister&)" );
};


/*
  Destructor.
*/
TC_AsmTemplateRegister::~TC_AsmTemplateRegister( void )
{
  DSTART( "virtual TC_AsmTemplateRegister::~TC_AsmTemplateRegister()" );
};


/*
  Copy-assignment operator.
*/
TC_AsmTemplateRegister & TC_AsmTemplateRegister::operator = ( const TC_AsmTemplateRegister &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  TC_AsmRegister::operator = ( __o );

  mTemplateRegisterData = __o.mTemplateRegisterData;

  return( *this );
};


/*
  isCompatible returns whether a template register is compatible with a given
  argument type.
*/
bool TC_AsmTemplateRegister::isCompatible( Type t ) const
{
  DSTART(
    "virtual bool TC_AsmTemplateRegister::isCompatible(TC_AsmArgument::Type) const" );

  auto tVal = static_cast<unsigned long long>( t );
  auto getVal = static_cast<unsigned long long>( getType() );

  if ( mTemplateRegisterData->mIsEReg &&
       ( getVal & static_cast<unsigned long long>( Type::EREG ) ) )
    return( static_cast<unsigned long long>( Type::EREG ) & tVal );
  else

  if ( !mTemplateRegisterData->mIsEReg &&
       ( getVal & static_cast<unsigned long long>( Type::EREG ) ) )
    return( static_cast<unsigned long long>( Type::ANYDREG ) & tVal );

  return( TC_AsmRegister::isCompatible( t ) );
};


/*
  getRegister returns the WIR register represented by a template assembly
  argument.
*/
const WIR_BaseRegister &TC_AsmTemplateRegister::getRegister( void ) const
{
  DSTART(
    "virtual const WIR_BaseRegister& TC_AsmTemplateRegister::getRegister() const" );

  if ( mTemplateRegisterData->mRegister == nullptr ) {
    if ( mTemplateRegisterData->mIsEReg )
      mTemplateRegisterData->mRegister =
        const_cast<TC_ERegV *>(
          &(mTemplateRegisterData->mRegisterInitializer->getEReg()) );
    else
      mTemplateRegisterData->mRegister =
        const_cast<TC_DRegV *>(
          &(mTemplateRegisterData->mRegisterInitializer->getDReg()) );
  }

  return( *(mTemplateRegisterData->mRegister) );
};


/*
  setEReg specifies that the WIR register backing this template argument is an
  extended register.
*/
void TC_AsmTemplateRegister::setEReg( bool e )
{
  DSTART( "void TC_AsmTemplateRegister::setEReg(bool)" );

  mTemplateRegisterData->mIsEReg = e;
};


/*
  getChildRegister determines the ith child register of this template register
  argument.
*/
TC_AsmRegister *TC_AsmTemplateRegister::getChildRegister( unsigned int i )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  // Retrieve the actual WIR extended register.
  setEReg();
  auto &eReg = mTemplateRegisterData->mRegisterInitializer->getEReg();
  mTemplateRegisterData->mRegister = const_cast<TC_ERegV *>( &eReg );

  ufAssert( ( i == 0 ) || ( i == 1 ) );

  return(
    new TC_AsmRegister(
      i == 0 ? eReg.begin()->get() : eReg.rbegin()->get(), Type::DREG ) );
};


//
// Protected class methods
//

/*
  clone creates a copy of a template register argument.
*/
TC_AsmTemplateRegister *TC_AsmTemplateRegister::clone( void ) const
{
  DSTART(
    "virtual TC_AsmTemplateRegister* TC_AsmTemplateRegister::clone() const" );

  return( new TC_AsmTemplateRegister( *this ) );
};


//
// Private class methods
//

/*
  Default constructor for template register data using a given register
  initializer.

  This constructor takes over the ownership of the given pointer. The object
  pointed to will automatically be deleted during destruction of this struct.
*/
TC_AsmTemplateRegister::templateRegisterData::templateRegisterData( TC_AsmRegisterInitializer *init ) :
  mIsEReg { false },
  mRegister { nullptr },
  mRegisterInitializer { init }
{
  DSTART(
    "TC_AsmTemplateRegister::templateRegisterData::templateRegisterData(TC_AsmRegisterInitializer*)" );
};

}       // namespace WIR
