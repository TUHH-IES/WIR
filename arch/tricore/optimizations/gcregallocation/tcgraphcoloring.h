/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcgraphcoloring.h
  @brief This file provides the basic interface of a TriCore-specific
         graph-coloring based register allocator.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_GRAPHCOLORING_H
#define _TC_GRAPHCOLORING_H


//
// Include section
//

// Include standard headers
#include <functional>
#include <list>
#include <map>
#include <set>
#include <utility>
#include <vector>

// Include WIR headers
#include <wir/wirtypes.h>
#include <optimizations/gcregallocation/wirgraphcoloring.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_BaseRegister;
class WIR_BasicBlock;
class WIR_Function;
class WIR_Instruction;
class WIR_InterferenceGraph;
class WIR_Operation;
class WIR_Parameter;
class WIR_PhysicalRegister;
class WIR_RegisterParameter;
class WIR_UpDownValue;
class WIR_VirtualRegister;


/*!
  @brief Class TC_GraphColoring performs graph coloring-based register
         allocation for the TriCore architecture.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_GraphColoring : public WIR_GraphColoring
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for system-level optimization.

      @param[in] s A reference to a WIR_System to be optimized.
      @param[in] verbosity A Boolean defaulting to false that denotes whether
                           verbose messages shall be dumped or not.
      @param[in] uc A Boolean defaulting to false that denotes whether the
                    register allocator shall only use physical registers from
                    the TriCore's upper context or not.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit TC_GraphColoring( WIR_System &, bool = false, bool = false );

    /*!
      @brief Default constructor for compilation unit-level optimization.

      @param[in] c A reference to a WIR_CompilationUnit to be optimized.
      @param[in] verbosity A Boolean defaulting to false that denotes whether
                           verbose messages shall be dumped or not.
      @param[in] uc A Boolean defaulting to false that denotes whether the
                    register allocator shall only use physical registers from
                    the TriCore's upper context or not.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit TC_GraphColoring( WIR_CompilationUnit &, bool = false,
                               bool = false );

    /*!
      @brief Default constructor for function-level optimization.

      @param[in] f A reference to a WIR_Function to be optimized.
      @param[in] verbosity A Boolean defaulting to false that denotes whether
                           verbose messages shall be dumped or not.
      @param[in] uc A Boolean defaulting to false that denotes whether the
                    register allocator shall only use physical registers from
                    the TriCore's upper context or not.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit TC_GraphColoring( WIR_Function &, bool = false, bool = false );

    /*!
      @brief Destructor.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_GraphColoring( void );


    //
    // Configuration switches.
    //

    /*!
      @brief setUseOnlyUC (de-) activates whether the register allocator should
             only use physical registers from the TriCore's upper context, or
             not.

      @param[in] uc A Boolean defaulting to true that specifies whether to use
                    the upper context only, or not.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void setUseOnlyUC( bool = true );

    /*!
      @brief getUseOnlyUC returns whether the register allocator should only use
             physical registers from the TriCore's upper context, or not.

      @return true if only physical registers from the TriCore's upper context
              are used, false otherwise.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    bool getUseOnlyUC( void ) const;


  protected:

    //
    // Methods for optimization management.
    //

    /*!
      @brief runOptimization allocates registers in the given function.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOptimization( WIR_Function & ) override;


    //
    // Methods for TriCore-specific physical registers.
    //

    /*!
      @brief createPhregs sets up the lists mPhregs and
             mPhregsForPrecoloringOnly of all TriCore-specific physical
             registers contained in the specified %WIR function.

      @param[in] f A reference to a %WIR function.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void createPhregs( WIR_Function & ) override;

    /*!
      @brief initializationHook allows to perform processor-specific actions
             before doing some actual coloring or spilling.

      @param[in] f A reference to a %WIR function.

      Here, initializationHook is used to perform the bit-true data flow
      analysis for TriCore before jumping into the actual graph coloring.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void initializationHook( WIR_Function & ) override;

    /*!
      @brief isCallerSaved checks whether the specified physical register is
             caller-saved.

      @param[in] r A const reference to a physical register to be checked.
      @return true iff the physical register is caller-saved, false otherwise.

      For the TriCore architecture, the lower-context registers are caller-
      saved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool isCallerSaved( const WIR_PhysicalRegister & ) const override;

    /*!
      @brief checkCallerSavedRegsAliveAcrossCall checks whether the two
             specified virtual leaf registers are mapped to a caller-saved
             physical register and whether they are alive across a function
             call.

      @param[in] phreg A const reference to a physical TriCore register.
      @param[in] vregs_c1 A const reference to a set of registers denoting the
                          first virtual leaf register to be checked, including
                          its coalescing aliases.
      @param[in] vregs_c2 A const reference to a set of registers denoting the
                          second virtual leaf register to be checked, including
                          its coalescing aliases.

      If so, the respective function call and the physical register are stored
      in mLocalCallerSavedRegsAliveAcrossCall.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void checkCallerSavedRegsAliveAcrossCall( const WIR_PhysicalRegister &,
                                              const WIR_RegisterSet &,
                                              const WIR_RegisterSet & );


    //
    // Interference graph construction.
    //

    /*!
      @brief buildProcessorSpecificInterferences adds edges to the interference
             graph expressing TriCore-specific interferences.

      @param[in] f A reference to a %WIR function.
      @param[in,out] igraph A reference to the interference graph.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void buildProcessorSpecificInterferences( WIR_Function &,
                                                      WIR_InterferenceGraph & ) override;

    /*!
      @brief isFunctionReturnMove checks whether a given move operation stores
             the result of a function call somewhere.

      @param[in] o A const reference to a move operation.
      @return isFunctionReturnMove returns true iff
              - the register used by the move is D2, D3 or E3, and
              - the move immediately follows a call instruction.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool isFunctionReturnMove( const WIR_Operation & ) const override;


    //
    // Spilling.
    //

    /*!
      @brief isPriorityRegister returns whether a given register has high
             priority for color assignment.

      @param[in] r A const reference to a virtual register.
      @return true iff the register has high priority, false otherwise.

      For the TriCore register allocator, high-priority registers are kept in
      set mHighPriorityRegs. Thus, this method only checks whether r is in
      mHighPriorityRegs or not.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool isPriorityRegister( const WIR_VirtualRegister & ) const override;

    /*!
      @brief getSpillLoadCosts returns the TriCore-specific costs of one single
             spill-load for the specified register parameter.

      @param[in] p A const reference to a %WIR register parameter.
      @return Some TriCore-specific cost measure for a spill-load of register
              parameter p.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual unsigned int getSpillLoadCosts( const WIR_RegisterParameter & ) const override;

    /*!
      @brief getSpillStoreCosts returns the TriCore-specific costs of one single
             spill-store for the specified register parameter.

      @param[in] p A const reference to a %WIR register parameter.
      @return Some TriCore-specific cost measure for a spill-store of register
              parameter p.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual unsigned int getSpillStoreCosts( const WIR_RegisterParameter & ) const override;

    /*!
      @brief getMoveCosts returns the TriCore-specific costs of one single move
             operation that can be omitted due to spilling.

      @param[in] o A const reference to a move operation.
      @return Some TriCore-specific cost measure for a move operation.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual unsigned int getMoveCosts( const WIR_Operation & ) const override;


    //
    // Coalescing.
    //

    /*!
      @brief getUseOfMove returns the used register of the specified TriCore
             %WIR move operation.

      @param[in] o A const reference to a move operation.
      @return A reference to the %WIR register used by the move operation.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_BaseRegister &getUseOfMove( const WIR_Operation & ) const override;

    /*!
      @brief getDefOfMove returns the defined register of the specified TriCore
             %WIR move operation.

      @param[in] o A const reference to a move operation.
      @return A reference to the %WIR register defined by the move operation.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_BaseRegister &getDefOfMove( const WIR_Operation & ) const override;


    //
    // Rematerialization.
    //

    /*!
      @brief getRematerializationCosts returns the TriCore-specific costs of one
             single recomputation of the specified used parameter.

      @param[in] p A const reference to a %WIR register parameter.
      @return Some TriCore-specific cost measure for a rematerialization of the
              register in used in p.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual unsigned int getRematerializationCosts( const WIR_RegisterParameter & ) const override;

    /*!
      @brief getRematerializationConstant returns a constant integer value that
             is equivalent to the specified %WIR parameter.

      @param[in] p A const reference to a %WIR register parameter.
      @return A constant value c if p boils down to c according to data flow
              analysis, maxint otherwise.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    int getRematerializationConstant( const WIR_RegisterParameter & ) const;

    /*!
      @brief getRematerializationInstructions returns a list of TriCore
             instructions for one single recomputation of the specified used
             parameter.

      @param[in] p A const reference to a %WIR register parameter.
      @return Some TriCore-specific machine instructions for the
              rematerialization of the register in used in p.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual std::list<WIR_Instruction *> getRematerializationInstructions( const WIR_RegisterParameter & ) const override;


    //
    // Coloring.
    //

    /*!
      @brief selectColors assigns actual colors to the TriCore leaf registers in
             the specified vector.

      @param[in] leafs A const reference to a vector containing all leaf
                       registers of one arbitrary interference graph node.
      @param[in] igraph A const reference to the interference graph.
      @return A map mapping each leaf from vector leafs to its color number.
              Valid color numbers are from the interval
              [1, getAvailableColors()]. If the returned map contains no entry,
              no suitable color for the leafs could be found so that they become
              actual spills.

      This method must not yet assign colors to the interference graph - this is
      done elsewhere. In order to determine feasible colors for the leaf
      registers, this method should make use of
      WIR_InterferenceGraph::getPossibleColors(). It must be ensured that the
      returned map is either empty or contains exactly one entry per leaf. It
      must hold that none of the colors used in this returned map is already
      used for adjacent interference graph nodes.

      This method checks whether the leafs to be processed are TriCore data or
      address registers, and whether they are simple or extended registers. It
      first tries to select the implicit registers D15 and A15, since this is
      beneficial for the generation of 16-bit instructions. If D15/A15 are not
      available, other data/address registers from the TriCore's upper context
      are checked next. This is done, because upper context registers are
      automatically saved across function calls without any additional context
      saving code. If the entire upper context is not available, the registers
      of the lower context are finally checked.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_GraphColoring::WIR_ColorMap selectColors( const std::vector<
                                                                  std::reference_wrapper<
                                                                    WIR_VirtualRegister>> &,
                                                          const WIR_InterferenceGraph & ) override;


    //
    // Code transformation.
    //

    /*!
      @brief resolveSpillCoalescingConflict resolves a conflict when two
             registers with different positions in a register hierarchy shall be
             coalesced during computeStackLocations.

      @param[in] r1 A const reference to a first %WIR virtual register to be
                    coalesced.
      @param[in] r2 A const reference to a second %WIR register to be coalesced.
      @return A pair of registers that have the same number of leafs in their
              respective hierarchies and that can thus be used for coalescing.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual std::pair<
              std::reference_wrapper<WIR_VirtualRegister>,
              std::reference_wrapper<WIR_VirtualRegister>> resolveSpillCoalescingConflict( const WIR_VirtualRegister &,
                                                                                           const WIR_VirtualRegister & ) const override;

    /*!
      @brief getStackPosOfSubReg returns the stack position of some child
             register, if the root of the entire register hierarchy is located
             in the specified stack position.

      @param[in] r A const reference to a virtual %WIR child register.
      @param[in] rootPos The position of r's root register on the stack.
      @return The according stack position of r on the stack.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual unsigned int getStackPosOfSubReg( const WIR_VirtualRegister &,
                                              unsigned int ) const override;

    /*!
      @brief insertSpillLoad inserts TriCore code for a spill-load of a register
             into the %WIR.

      @param[in] clone A const reference to a cloned register created during
                       spilling-related live range splitting. This cloned
                       register will be used by the actually generated
                       spill-load instruction.
      @param[in] r A const reference to the original, un-cloned %WIR register.
      @param[in] stackPos The stack position from which the register is spill-
                          loaded.
      @param[in] b A reference to a %WIR basic block in which to insert the
                   generated spill code.
      @param[in] pos A const iterator refering to the position before which the
                     generated spill code will be inserted.

      insertSpillLoad is responsible to add all generated spill-load
      instructions to map mSpillLoads.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void insertSpillLoad( const WIR_BaseRegister &,
                                  const WIR_BaseRegister &,
                                  int, WIR_BasicBlock &,
                                  std::list<
                                    std::reference_wrapper<
                                      WIR_Instruction>>::const_iterator ) override;

    /*!
      @brief insertSpillStore inserts TriCore code for a spill-store of a
             register into the %WIR.

      @param[in] clone A const reference to a cloned register created during
                       spilling-related live range splitting. This cloned
                       register will be used by the actually generated
                       spill-store instruction.
      @param[in] r A const reference to the original, un-cloned %WIR register.
      @param[in] stackPos The stack position to which the register is spill-
                          stored.
      @param[in] b A reference to a %WIR basic block in which to insert the
                   generated spill code.
      @param[in] pos A const iterator refering to the position before which the
                     generated spill code will be inserted.

      insertSpillStore is responsible to add all generated spill-store
      instructions to map mSpillStores.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void insertSpillStore( const WIR_BaseRegister &,
                                   const WIR_BaseRegister &,
                                   int, WIR_BasicBlock &,
                                   std::list<
                                     std::reference_wrapper<
                                       WIR_Instruction>>::const_iterator ) override;

    /*!
      @brief insertSpillCode inserts TriCore code for a spill-load or
             spill-store of a register into the %WIR.

      @param[in] clone A const reference to a cloned register to be spilled.
      @param[in] r A const reference to the original, un-cloned %WIR register.
      @param[in] stackPos The stack position to/from which the register is
                          spilled.
      @param[in] b A reference to a %WIR basic block in which to insert the
                   generated spill code.
      @param[in] pos A const iterator refering to the position before which the
                     generated spill code will be inserted.
      @param[in] spillStore A Boolean switch that controls whether a spill-store
                            (true) or a spill-load (false) is generated.
      @return A reference to an eventually newly inserted basic block, or a
              reference to b if no new basic block was created.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_BasicBlock &insertSpillCode( const WIR_BaseRegister &,
                                             const WIR_BaseRegister &,
                                             int, WIR_BasicBlock &,
                                             std::list<
                                               std::reference_wrapper<
                                                 WIR_Instruction>>::const_iterator,
                                             bool );

    /*!
      @brief isAdjustedLoadOrStoreInstruction returns whether the given iterator
             refers to a load or a store instruction with a stack pointer
             relative addressing mode which is surrounded by LEA instructions
             that adjust the stack pointer.

      @param[in] b A const reference to a %WIR basic block whose instructions
                   shall be checked.
      @param[in] pos A const iterator refering to the position of an instruction
                     to be checked.
      @return true iff pos refers to an adjusted stack access, false otherwise.

      isAdjustedLoadOrStoreInstruction checks if the code looks like

        lea    A10, A10, const1             # with const1 > 0
        ld__   __, [A10] __                 # pos
        lea    A10, A10, -const2            # with const2 > 0

      or

        lea    A10, A10, const1             # with const1 > 0
        st__   [A10] __, __                 # pos
        lea    A10, A10, -const2            # with const2 > 0

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    bool isAdjustedLoadOrStoreInstruction( const WIR_BasicBlock &,
                                           std::list<
                                             std::reference_wrapper<
                                               WIR_Instruction>>::const_iterator ) const;

    /*!
      @brief getCandidatePhregs returns a set of physical registers that could
             be used for the specified virtual register according to the
             TriCore's ISA.

      @param[in] r A const reference to a virtual %WIR register.
      @return A set of physical TriCore registers.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_PhysicalRegisterSet getCandidatePhregs( const WIR_VirtualRegister & ) override;

    /*!
      @brief getCandidatePhreg returns one element from the specified set of
             registers that will finally be used within
             allocateUncoloredActualSpills for spilling.

      @param[in] candidates A const reference to a set of potential candidate
                            phregs for spilling.
      @return A reference to one of the registers in set candidates.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual const WIR_PhysicalRegister &getCandidatePhreg( const WIR_PhysicalRegisterSet & ) override;

    /*!
      @brief postProcessingHook allows to perform TriCore-specific actions after
             having done register allocation for a function, using e.g., the set
             of inserted spill operations mInsertedSpillCode.

      @param[in,out] f A reference to a %WIR function.

      Here, postProcessingHook is used to realize the TriCore-specific calling
      conventions afterwards.
    */
    virtual void postProcessingHook( WIR_Function & ) override;

    /*!
      @brief adjustStack allocates additional space in the specified function's
             stack frame and adjusts all stack-related memory accesses
             accordingly.

      @param[in,out] f A reference to a %WIR function.

      According to the TriCore EABI (section 2.2.2.1), the stack pointer points
      to the bottom (low address) of the stack frame. The stack pointer
      alignment is 8 bytes. The argument overflow area for outgoing arguments
      must be located at the bottom (low address end) of the frame, with the
      first overflow argument at zero offset from the stack pointer:

      @verbatim
      (Stack
       growing
       direction)
           |
           |   +-------------------------+      (high address)
           |   | Local Variables Frame 1 |
           |   +-------------------------+
           |   | Argument Overflow Area, |
           |   | Function 2 Arguments    |      (first argument passed on stack)
           |   +-------------------------+
           |   | Local Variables Frame 2 |
           |   +-------------------------+
           |   | Argument Overflow Area, |
           |   | Function 3 Arguments    |
           |   +-------------------------+ <--- Stack Pointer (SP) at entry
           V   | Local Variables Frame 3 |      (CALL) to Function 3
               +-------------------------+
               | Argument Overflow Area  |
               +-------------------------+      (low address)
      @endverbatim

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void adjustStack( WIR_Function & ) override;

    /*!
      @brief postRACleanup allows to perform very final TriCore-specific cleanup
             actions, particularly after stack frame reorganization.

      @param[in] f A reference to a %WIR function.

      Here, postRACleanup is used to remove redundant MOV and SWAP.W
      instructions. Furthermore, don't optimize flags of parameters (indicating
      accesses to the TriCore's argument overflow stack region) are reset.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void postRACleanup( WIR_Function & ) override;

    /*!
      @brief For a list of instructions implementing one spill-load or -store,
             getPhregOfSpill determines that physical register that is actually
             spill-loaded or -stored.

      @param[in] spill A const reference to a list of spill instructions.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_PhysicalRegister &getPhregOfSpill( const std::list<
                                                           std::reference_wrapper<
                                                             WIR_Instruction>> & ) override;


    //
    // Attributes.
    //

    /*!
      @brief mHighPriorityRegs contains the IDs of all TriCore registers that
             shall be colored with high priority so that they are likely not to
             be spilled.
    */
    std::set<WIR_id_t> mHighPriorityRegs;

    /*!
      @brief mUncoloredSpill points to a virtual register that was artificially
             created during spill code generation.
    */
    WIR_VirtualRegister *mUncoloredSpill;


  private:

    /*!
      @brief Method deleted, a data flow analysis will always be carried out.

      @param[in] f A Boolean flag defaulting to true that denotes whether a data
                   flow analysis will be carried out (true) or not.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    // cppcheck-suppress duplInheritedMember
    void setRunDFA( bool f = true ) = delete;

    /*!
      @brief mStackAccessLatency stores the memory access latency of the
             physical memory where the stack resides.
    */
    unsigned int mStackAccessLatency;

    /*!
      @brief mBBAccessLatency stores the memory access latency of the physical
             memory where the basic block with the given ID resides.
    */
    std::map<WIR_id_t, unsigned int> mBBAccessLatency;

    /*!
      @brief mUseOnlyUC denotes whether the register allocator should only use
             physical registers from the TriCore's upper context, or not.
    */
    bool mUseOnlyUC;

    /*!
      @brief mOrderedAREGs holds all TriCore AREGs in the precedence order in
             which they should be used for coloring.
    */
    std::vector<std::reference_wrapper<WIR_PhysicalRegister>> mOrderedAREGs;

    /*!
      @brief mOrderedDREGs holds all TriCore DREGs in the precedence order in
             which they should be used for coloring.
    */
    std::vector<std::reference_wrapper<WIR_PhysicalRegister>> mOrderedDREGs;

    /*!
      @brief mOrderedDREGs holds all TriCore EREGs in the precedence order in
             which they should be used for coloring.
    */
    std::vector<std::reference_wrapper<WIR_PhysicalRegister>> mOrderedEREGs;

    /*!
      @brief mOrderedDREGs holds all TriCore PREGs in the precedence order in
             which they should be used for coloring.
    */
    std::vector<std::reference_wrapper<WIR_PhysicalRegister>> mOrderedPREGs;

};

}       // namespace WIR

#endif  // _TC_GRAPHCOLORING_H
