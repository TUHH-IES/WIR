/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirlivenessanalysis.h
  @brief This file provides the interface of the liveness analysis.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_LIVENESSANALYSIS_H
#define _WIR_LIVENESSANALYSIS_H


//
// Include section
//

// Include standard headers
#include <map>
#include <set>

// Include WIR headers
#include <wir/wirtypes.h>
#include <analyses/generic/wiranalysis.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_BaseRegister;
class WIR_BitVector;
class WIR_Function;


/*!
  @brief Class WIR_LivenessAnalysis is the %WIR liveness analysis according to
         Andrew W. Appel, Modern Compiler Implementation in C, page 221,
         algorithm 10.4.

  Analysis results are stored in WIR_LiveOut containers that are attached to
  %WIR instructions.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_LivenessAnalysis : public WIR_Analysis
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
    explicit WIR_LivenessAnalysis( WIR_Function & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_LivenessAnalysis( void );


  protected:

    /*!
      @brief runAnalysis performs liveness analysis by iteration of the given
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
      @brief init initializes internal data structures by collecting information
             about register definitions/uses of %WIR instructions and of
             predecessor/successor relations between basic blocks.

      @param[in] f A reference to a WIR_Function to be analyzed.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void init( WIR_Function & );

    /*!
      @brief propagateIns2BB propagates instruction-level def/use information to
             basic block-level where liveness analysis is actually done (Appel,
             pages 394-395).

      @param[in] f A const reference to a WIR_Function to be analyzed.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void propagateIns2BB( const WIR_Function & );

    /*!
      @brief propagateBB2Ins propagates basic block-level analysis results to
             instruction-level.

      @param[in] f A reference to a WIR_Function to be analyzed.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void propagateBB2Ins( WIR_Function & );

    //! mProblemSize stores how many registers are subject to liveness analysis.
    size_t mProblemSize;

    /*!
      @brief mHash maps the ID of each register subject to liveness analysis to
             a unique position in the bit vectors used during iterative
             analysis.
    */
    std::map<WIR_id_t, size_t> mHash;

    /*!
      @brief mReverseHash reverse-maps each bit vector position to its
             corresponding register subject to liveness analysis.
    */
    std::map<size_t, WIR_BaseRegister *> mReverseHash;

    /*!
      @brief mInstrDefs maps an instruction's ID to a bit vector of all
             registers it defines.
    */
    std::map<WIR_id_t, WIR_BitVector> mInstrDefs;

    /*!
      @brief mInstrUses maps an instruction's ID to a bit vector of all
             registers it uses.
    */
    std::map<WIR_id_t, WIR_BitVector> mInstrUses;

    /*!
      @brief mBlockDefs maps a basic block's ID to a bit vector of all registers
             it defines.
    */
    std::map<WIR_id_t, WIR_BitVector> mBlockDefs;

    /*!
      @brief mBlockLiveIn maps a basic block's ID to a bit vector of live-in
             registers.
    */
    std::map<WIR_id_t, WIR_BitVector> mBlockLiveIn;

    /*!
      @brief mBlockLiveIn maps a basic block's ID to a bit vector of live-out
             registers.
    */
    std::map<WIR_id_t, WIR_BitVector> mBlockLiveOut;

    /*!
      @brief mPredecessors maps a basic block's ID to all its non-empty
             predecessors.
    */
    std::map<WIR_id_t, WIR_BasicBlockSet> mPredecessors;

    /*!
      @brief mSuccessors maps a basic block's ID to all its non-empty
             successors.
    */
    std::map<WIR_id_t, WIR_BasicBlockSet> mSuccessors;

    /*!
      @brief mUpdateBlock stores whether a basic block ID has to be re-evaluated
             during iterative data flow analysis.
    */
    std::map<WIR_id_t, bool> mUpdateBlock;

};

}       // namespace WIR

#endif  // _WIR_LIVENESSANALYSIS_H
