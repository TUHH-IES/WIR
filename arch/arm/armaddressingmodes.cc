/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armaddressingmodes.cc
  @brief This file declares the ARM's general addressing modes.

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
#include <arch/arm/armv4.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

// Addressing modes
const ARMv4::AddressingMode ARMv4::AddressingMode::lsl     { "lsl" };
const ARMv4::AddressingMode ARMv4::AddressingMode::lsr     { "lsr" };
const ARMv4::AddressingMode ARMv4::AddressingMode::asr     { "asr" };
const ARMv4::AddressingMode ARMv4::AddressingMode::ror     { "ror" };
const ARMv4::AddressingMode ARMv4::AddressingMode::plus    { "+" };
const ARMv4::AddressingMode ARMv4::AddressingMode::minus   { "-" };
const ARMv4::AddressingMode ARMv4::AddressingMode::pre     { "pre" };
const ARMv4::AddressingMode ARMv4::AddressingMode::post    { "post" };
const ARMv4::AddressingMode ARMv4::AddressingMode::ia      { "ia" };
const ARMv4::AddressingMode ARMv4::AddressingMode::ib      { "ib" };
const ARMv4::AddressingMode ARMv4::AddressingMode::da      { "da" };
const ARMv4::AddressingMode ARMv4::AddressingMode::db      { "db" };
const ARMv4::AddressingMode ARMv4::AddressingMode::fd      { "fd" };
const ARMv4::AddressingMode ARMv4::AddressingMode::ed      { "ed" };
const ARMv4::AddressingMode ARMv4::AddressingMode::fa      { "fa" };
const ARMv4::AddressingMode ARMv4::AddressingMode::ea      { "ea" };
const ARMv4::AddressingMode ARMv4::AddressingMode::c       { "c" };
const ARMv4::AddressingMode ARMv4::AddressingMode::x       { "x" };
const ARMv4::AddressingMode ARMv4::AddressingMode::s       { "s" };
const ARMv4::AddressingMode ARMv4::AddressingMode::f       { "f" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p0      { "p0" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p1      { "p1" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p2      { "p2" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p3      { "p3" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p4      { "p4" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p5      { "p5" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p6      { "p6" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p7      { "p7" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p8      { "p8" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p9      { "p9" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p10     { "p10" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p11     { "p11" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p12     { "p12" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p13     { "p13" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p14     { "p14" };
const ARMv4::AddressingMode ARMv4::AddressingMode::p15     { "p15" };
const ARMv4::AddressingMode ARMv4::AddressingMode::cpsra   { "a" };
const ARMv4::AddressingMode ARMv4::AddressingMode::cpsrf   { "f" };
const ARMv4::AddressingMode ARMv4::AddressingMode::cpsri   { "i" };
const ARMv4::AddressingMode ARMv4::AddressingMode::cpsraf  { "af" };
const ARMv4::AddressingMode ARMv4::AddressingMode::cpsrai  { "ai" };
const ARMv4::AddressingMode ARMv4::AddressingMode::cpsrfi  { "fi" };
const ARMv4::AddressingMode ARMv4::AddressingMode::cpsrafi { "afi" };
const ARMv4::AddressingMode ARMv4::AddressingMode::be      { "be" };
const ARMv4::AddressingMode ARMv4::AddressingMode::le      { "le" };
const ARMv4::AddressingMode ARMv4::AddressingMode::ror0    { "ror #0" };
const ARMv4::AddressingMode ARMv4::AddressingMode::ror8    { "ror #8" };
const ARMv4::AddressingMode ARMv4::AddressingMode::ror16   { "ror #16" };
const ARMv4::AddressingMode ARMv4::AddressingMode::ror24   { "ror #24" };


/*
  getProcessorTypeName returns a string containing the C++-mangled name of the
  ARM class to which an addressing mode belongs.

  This method is only used for comparing addressing modes of different processor
  architectures in WIR_Operation::checkParameters().
*/
std::string ARMv4::AddressingMode::getProcessorTypeName( void ) const
{
  DSTART(
    "virtual string ARMv4::AddressingMode::getProcessorTypeName() const" );

  return( string( typeid( ARMv4 ).name() ) );
};

}       // namespace WIR
