/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcjumpcorrection.h
  @brief This file provides the interface of a TriCore-specific optimimzation
         detecting and correcting jump instructions with too large
         displacements.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_JUMPCORRECTION_H
#define _TC_JUMPCORRECTION_H


//
// Include section
//

// Include standard headers
#include <functional>
#include <utility>

// Include WIR headers
#include <optimizations/jumpcorrection/wirjumpcorrection.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_BaseRegister;
class WIR_BasicBlock;
class WIR_CompilationUnit;
class WIR_Operation;
class WIR_Symbol;
class WIR_System;
class WIR_Function;


/*!
  @brief Class TC_JumpCorrection corrects TriCore jump instructions having too
         large displacements.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_JumpCorrection : public WIR_JumpCorrection
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for system-level optimization.

      @param[in,out] s A reference to a WIR_System to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit TC_JumpCorrection( WIR_System & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_JumpCorrection( void );


    //
    // Jump correction costs.
    //

    /*!
      @brief getJumpCostsSize computes the size costs of a jump correction.

      @param[in] s A const reference to the source basic block of a jump.
      @param[in] t A const reference to the target basic block of a jump.
      @param[in] addToSrc A Boolean flag indicating whether the additional size
                          costs attributed to the source basic block shall be
                          computed (true) or those attributed to the target
                          block (false).
      @return An integer denoting the number of additionally required bytes due
              to jump correction.

      This method computes the amount of bytes that are introduced to a basic
      block due to jump correction code if a jump from basic block s to t is
      corrected. Depending on addToSrc, the number of additional bytes for block
      s or t is computed, resp.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual int getJumpCostsSize( const WIR_BasicBlock &,
                                  const WIR_BasicBlock &, bool ) override;

    /*!
      @brief getJumpCostsCycles computes the timing costs of a jump correction.

      @param[in] s A const reference to the source basic block of a jump.
      @param[in] t A const reference to the target basic block of a jump.
      @param[in] addToSrc A Boolean flag indicating whether the additional
                          timing costs attributed to the source basic block
                          shall be computed (true) or those attributed to the
                          target block (false).
      @return An integer denoting the number of additionally required clock
              cycles due to jump correction.

      This method computes the amount of clock cycles that are introduced to a
      basic block due to jump correction code if a jump from basic block s to t
      is corrected. Depending on addToSrc, the number of additional clock cycles
      for block s or t is computed, resp.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual int getJumpCostsCycles( const WIR_BasicBlock &,
                                    const WIR_BasicBlock &, bool ) override;


  protected:

    /*!
      @brief runOptimization performs jump correction in the given function.

      @param[in] f A reference to a %WIR_Function to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOptimization( WIR_Function & ) override;

    /*!
      @brief runOpt performs jump correction in the given basic block.

      @param[in] b A reference to a %WIR_BasicBlock to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOpt( WIR_BasicBlock & ) override;


  private:

    /*!
      @brief adjustConditionalJump corrects invalid conditional jumps.

      @param[in,out] o A reference to the conditional jump to be corrected.
      @return An unsigned integer denoting the number of effectively corrected
              operations.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    unsigned int adjustConditionalJump( WIR_Operation & );

    /*!
      @brief adjustImplicitJump corrects invalid implicit jumps.

      @param[in,out] o A reference to the implicit jump to be corrected.
      @return An unsigned integer denoting the number of effectively corrected
              operations.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    unsigned int adjustImplicitJump( WIR_Operation & );

    /*!
      @brief adjustUnconditionalJump corrects invalid unconditional jumps.

      @param[in,out] o A reference to the unconditional jump to be corrected.
      @return An unsigned integer denoting the number of effectively corrected
              operations.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    unsigned int adjustUnconditionalJump( WIR_Operation & );

    /*!
      @brief generateLongJump generates a code fragment for long-distance jumps.

      @param[in] jmp A const reference to a struct storing data of an invalid
                     jump.
      @param[in,out] srcBB A reference to a basic block in which to insert the
                           code for a long jump.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void generateLongJump( const InvalidJump &, WIR_BasicBlock & );

    /*!
      @brief eraseTrampoline checks if the specified basic block is a trampoline
             (i.e., only contains one single jump operation) that is superfluous
             and removes it.

      @param[in,out] b A reference to the trampoline basic block to be checked.
      @return true if the trampoline block is removed successfully, false
              otherwise.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    bool eraseTrampoline( WIR_BasicBlock & );

    /*!
      @brief determineAddressRegister tries to determine a free TriCore adddress
             register for indirect jumps.

      @param[in] f A reference to the %WIR function where a free address
                   register is required.
      @param[in] i A const reference to the %WIR instruction inside f where a
                   free address register is required.
      @return A pair containing a (wrapped) reference to the determined register
              plus a Boolean flag indicating wheter spilling is required or not.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    std::pair<
      std::reference_wrapper<WIR_BaseRegister>,
      bool> determineAddressRegister( WIR_Function &,
                                      const WIR_Instruction & ) const;

    /*!
      @brief getJumpTarget determines the symbol of the target of the given jump
             operation.

      @param[in] o A const reference to a jump operation.
      @return A reference to the jump target's symbol.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_Symbol &getJumpTarget( const WIR_Operation & ) const;

    /*!
      @brief getDisplacementWidth computes the maximal bit width of jump
             displacements for the given operation and its operation format.

      @param[in] o A const reference to a jump operation whose TriCore-specific
                   jump displacement width shall be determined.
      @return An unsigned integer containing the maximal bit width of o's jump
              displacement.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    unsigned int getDisplacementWidth( const WIR_Operation & ) const;

    /*!
      @brief fixInvalidCondition tries to correct the given invalid conditional
             jump by inverting its test condition.

      @param[in] jmp A const reference to a struct storing data of an invalid
                     conditional jump.
      @return true if the invalid jump could be corrected successfully, false
              otherwise.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    bool fixInvalidCondition( const InvalidJump & );

    /*!
      @brief fixInvalidDisplacement tries to correct the given invalid jump by
             replacing a 16-bit operation by a 32-bit operation allowing for
             larger displacements.

      @param[in] jmp A const reference to a struct storing data of an invalid
                     jump.
      @return true if the invalid jump could be corrected successfully, false
              otherwise.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    bool fixInvalidDisplacement( const InvalidJump & );

    /*!
      @brief redirectJump examines the specified operation and if it is a jump,
             it will be redirected to the given basic block.

      @param[in,out] o A reference to a %WIR operation to be examined.
      @param[in] b A const reference to the new jump target.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void redirectJump( WIR_Operation &, const WIR_BasicBlock & ) const;

    /*!
      @brief isTCReturn checks whether the given operation is a TriCore-specific
             function return.

      @param[in] o A const reference to the operation to be checked.
      @return true if o is a function return according to the TriCore ISA, false
              otherwise.

      isTCReturn checks whether o is classified as return operation or whether
      it is an indirect jump using physical register A11.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    bool isTCReturn( const WIR_Operation & ) const;


    //
    // Attributes
    //

    /*!
      When allocating additional stack space for extra spill variables that
      might be neccessary during jump correction, the TriCore stack pointer must
      be 8-byte aligned. Nevertheless, we only need 4 bytes for each single
      spill. Therefore, this variable indicates whether there is some remaining
      space on the stack that we have allocated but not yet used (if the
      variable is >= 0), or whether there is no unused stack space (variable <
      0). In the first case, the variable holds the address of the free space
      relative to the stack pointer of the function.
    */
    int mFreeSpillAddress;

};

}       // namespace WIR

#endif  // _TC_JUMPCORRECTION_H
