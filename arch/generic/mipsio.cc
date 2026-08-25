/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file mipsio.cc
  @brief This file implements MIPS/SPIM-specific stream I/O routines for the
         %WIR library.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <iostream>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/generic/mips.h>


//
// Code section
//

namespace WIR {


using namespace std;


/*
  mips is an I/O manipulator that provides a MIPS assembler dump of WIR.
*/
std::ostream &mips( std::ostream &os )
{
  DSTART( "ostream& mips(ostream&)" );

  os.iword( WIR_ProcessorIO() ) = MIPS::getProcessorTypeID();
  return( os );
};


/*
  dumpMIPSOperation dumps a %WIR operation to an output stream in a
  MIPS/SPIM-specific fashion.
*/
void dumpMIPSOperation( std::ostream &os, const WIR_Operation &o )
{
  DSTART( "void dumpMIPSOperation(ostream&, const WIR_Operation&)" );

  // Indent output.
  os << string( os.iword( WIR_Indentation() ), ' ' );

  // Output the operation's opcode.
  os << o.getOpCode().getName();

  // Output parameters.
  auto params = o.getParameters();
  bool firstParam = true;
  unsigned int expParamCount = 0;
  WIR_BaseProcessor::OperationFormat f = o.getOperationFormat();

  for ( auto it = params.begin(); it != params.end(); ++it ) {
    auto &p = (*it).get();
    bool emitParam = p.isExplicit();

    if ( emitParam ) {
      ++expParamCount;

      // Output separator between parameters.
      if ( firstParam ) {
        os << string( 8 - o.getOpCode().getName().size(), ' ' );
        firstParam = false;
      } else
        os << ", ";

      // Catch special operation formats here.
      if ( ( expParamCount == 2 ) &&
           ( ( f == MIPS::OperationFormat::RIR_1 ) ||
             ( f == MIPS::OperationFormat::RIR_2 ) ||
             ( f == MIPS::OperationFormat::RIUR ) ) ) {
        // Base + Offset addressing.
        os << dynamic_cast<WIR_BaseImmediateParameter &>( (*it).get() );
        do {
          ++it;
        } while ( (*it).get().isImplicit() );
        os << "(" << dynamic_cast<WIR_RegisterParameter &>( (*it).get() )
           << ")";
      } else {

        // Output current parameter itself.
        switch ( p.getType() ) {

          default: {
            // For immediate, register and label parameters, no special handling
            // is required.
            os << p;

            break;
          }
        }
      }
    }
  }
};

}       // namespace WIR
