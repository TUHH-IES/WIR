/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tc13addressingmodes.cc
  @brief This file declares the Infineon TriCore TC13's addressing modes.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
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
#include <arch/tricore/tc13.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

// Addressing modes
const TC13::AddressingMode TC13::AddressingMode::pre  { "preincr" };
const TC13::AddressingMode TC13::AddressingMode::post { "postincr" };


/*
  getProcessorTypeName returns a string containing the C++-mangled name of the
  TC13 class to which an addressing mode belongs.

  This method is only used for comparing addressing modes of different processor
  architectures in WIR_Operation::checkParameters().
*/
std::string TC13::AddressingMode::getProcessorTypeName( void ) const
{
  DSTART( "virtual string TC13::AddressingMode::getProcessorTypeName() const" );

  return( string( typeid( TC13 ).name() ) );
};

}       // namespace WIR
