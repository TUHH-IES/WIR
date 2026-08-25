/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file mipsio.h
  @brief This file provides MIPS/SPIM-specific stream I/O routines for the %WIR
         library.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _MIPS_IO_H
#define _MIPS_IO_H


//
// Include section
//


//
// Header section
//

namespace WIR {

/*!
  @brief mips is an I/O manipulator that provides a MIPS assembler dump of %WIR.

  @param[in] os A reference to the output stream to be manipulated.
  @return A reference to the manipulated output stream.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
std::ostream &mips( std::ostream &os );

/*!
  @brief dumpMIPSOperation dumps a %WIR operation to an output stream in a
         MIPS/SPIM-specific fashion.

  @param[in] os A reference to an output stream.
  @param[in] o A reference to the %WIR operation to be dumped.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
void dumpMIPSOperation( std::ostream &os, const WIR_Operation &o );

}       // namespace WIR

#endif  // _MIPS_IO_H
