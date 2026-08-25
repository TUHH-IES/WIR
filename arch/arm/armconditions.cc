/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armconditions.cc
  @brief This file declares the ARM's condition codes.

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
#include <arch/arm/armv4.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

// Condition codes
const ARMv4::Condition ARMv4::Condition::eq { "eq" };
const ARMv4::Condition ARMv4::Condition::ne { "ne" };
const ARMv4::Condition ARMv4::Condition::hs { "hs" };
const ARMv4::Condition ARMv4::Condition::lo { "lo" };
const ARMv4::Condition ARMv4::Condition::mi { "mi" };
const ARMv4::Condition ARMv4::Condition::pl { "pl" };
const ARMv4::Condition ARMv4::Condition::vs { "vs" };
const ARMv4::Condition ARMv4::Condition::vc { "vc" };
const ARMv4::Condition ARMv4::Condition::hi { "hi" };
const ARMv4::Condition ARMv4::Condition::ls { "ls" };
const ARMv4::Condition ARMv4::Condition::ge { "ge" };
const ARMv4::Condition ARMv4::Condition::lt { "lt" };
const ARMv4::Condition ARMv4::Condition::gt { "gt" };
const ARMv4::Condition ARMv4::Condition::le { "le" };
const ARMv4::Condition ARMv4::Condition::al { "al" };


/*
  getProcessorTypeName returns a string containing the C++-mangled name of the
  ARM class to which a condition belongs.

  This method is only used for comparing addressing modes of different processor
  architectures in WIR_Operation::checkParameters().
*/
std::string ARMv4::Condition::getProcessorTypeName( void ) const
{
  DSTART( BOOST_CURRENT_FUNCTION );

  return( string( typeid( ARMv4 ).name() ) );
};

}       // namespace WIR
