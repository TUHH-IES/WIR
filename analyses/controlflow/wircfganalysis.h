/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wircfganalysis.h
  @brief This file provides the interface of generic CFG-based control flow
         analyses.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_CFGANALYSIS_H
#define _WIR_CFGANALYSIS_H


//
// Include section
//

// Include WIR headers
#include <analyses/generic/wiranalysis.h>
#include <analyses/controlflow/wircfg.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_Function;


/*!
  @brief Class WIR_ControlFlowAnalysis is a generic base class for control flow
         analysis based on the %WIR control flow graph.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_ControlFlowAnalysis : public WIR_Analysis,
                                public WIR_CFG
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for function-level analysis.

      @param[in] f A reference to a WIR_Function to be analyzed.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit WIR_ControlFlowAnalysis( WIR_Function & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_ControlFlowAnalysis( void );

};

}       // namespace WIR

#endif  // _WIR_CFGANALYSIS_H
