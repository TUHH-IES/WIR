/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirimmediatedominationanalysis.h
  @brief This file provides the interface of the %WIR immediate domination
         analysis.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_IMMEDIATEDOMINATIONANALYSIS_H
#define _WIR_IMMEDIATEDOMINATIONANALYSIS_H


//
// Include section
//

// Include WIR headers
#include <analyses/controlflow/wircfg.h>
#include <analyses/generic/wiranalysis.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_Function;


/*!
  @brief Class WIR_ImmediateDominationAnalysis is the %WIR immediate domination
         analysis according Steven S. Muchnick, Advanced Compiler Design and
         Implementation, page 184ff and algorithm 7.15.

  Analysis results are stored in WIR_ImmediateDomination containers that are
  attached to reachable %WIR basic blocks.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_ImmediateDominationAnalysis : public WIR_Analysis
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
    explicit WIR_ImmediateDominationAnalysis( WIR_Function & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_ImmediateDominationAnalysis( void );


  protected:

    /*!
      @brief runAnalysis performs immediate domination analysis of the given
             function.

      @param[in] f A reference to a WIR_Function to be analyzed.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runAnalysis( WIR_Function & ) override;


  private:

    //
    // Private methods.
    //

    /*!
      @brief init initializes data structures by doing a domination analysis and
             attaching fresh containers.

      @param[in] f A reference to a WIR_Function to be analyzed.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void init( WIR_Function & );

    //! mCFG holds the currently analyzed function's control flow graph.
    WIR_CFG mCFG;

};

}       // namespace WIR

#endif  // _WIR_IMMEDIATEDOMINATIONANALYSIS_H
