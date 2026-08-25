/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvgraphcoloring.h
  @brief This file provides the basic interface of a RISC-V-specific
         graph-coloring based register allocator.
*/


#ifndef _RV_GRAPHCOLORING_H
#define _RV_GRAPHCOLORING_H


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
class WIR_CompilationUnit;
class WIR_Function;
class WIR_Instruction;
class WIR_InterferenceGraph;
class WIR_Operation;
class WIR_Parameter;
class WIR_PhysicalRegister;
class WIR_RegisterParameter;
class WIR_System;
class WIR_VirtualRegister;


/*!
  @brief Class RV_GraphColoring performs graph coloring-based register
         allocation for the RISC-V architecture.

  @author Ben Bahe <Ben.Bahe@tuhh.de>
*/
class RV_GraphColoring : public WIR_GraphColoring
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

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    explicit RV_GraphColoring( WIR_System &, bool = false );

    /*!
      @brief Default constructor for compilation unit-level optimization.

      @param[in] c A reference to a WIR_CompilationUnit to be optimized.
      @param[in] verbosity A Boolean defaulting to false that denotes whether
                           verbose messages shall be dumped or not.

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    explicit RV_GraphColoring( WIR_CompilationUnit &, bool = false );

    /*!
      @brief Default constructor for function-level optimization.

      @param[in] f A reference to a WIR_Function to be optimized.
      @param[in] verbosity A Boolean defaulting to false that denotes whether
                           verbose messages shall be dumped or not.

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    explicit RV_GraphColoring( WIR_Function &, bool = false );

    /*!
      @brief Destructor.

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    virtual ~RV_GraphColoring( void );

    /*!
      @brief setRV32IF sets whether register allocation shall be performed
             for RV32IF compatible architectures featuring 32 bits wide
             floating-point registers.

      @param[in] f A Boolean defaulting to true that denotes whether RV32IF
                   shall be considered or not.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void setRV32IF( bool = true );

    /*!
      @brief setRV32ID sets whether register allocation shall be performed
             for RV32ID compatible architectures featuring 64 bits wide
             floating-point registers.

      @param[in] d A Boolean defaulting to true that denotes whether RV32ID
                   shall be considered or not.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void setRV32ID( bool = true );


  protected:

    //
    // Methods for optimization management.
    //

    /*!
      @brief runOptimization allocates registers in the given function.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Simon Kopischke <Simon.Kopischke@tuhh.de>
    */
    virtual void runOptimization( WIR_Function & ) override;


    //
    // Methods for RISC-V-specific physical registers.
    //

    /*!
      @brief createPhregs sets up the lists mPhregs and
             mPhregsForPrecoloringOnly of all RISC-V-specific physical
             registers contained in the specified %WIR function.

      @param[in] f A reference to a %WIR function.

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    virtual void createPhregs( WIR_Function & ) override;

    /*!
      @brief isCallerSaved checks whether the specified physical register is
             caller-saved.

      @param[in] r A const reference to a physical register to be checked.
      @return true iff the physical register is caller-saved, false otherwise.

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    virtual bool isCallerSaved( const WIR_PhysicalRegister & ) const override;

    /*!
      @brief checkCallerSavedRegsAliveAcrossCall checks whether the two
             specified virtual leaf registers are mapped to a caller-saved
             physical register and whether they are alive across a function
             call.

      @param[in] phreg A const reference to a physical RISC-V register.
      @param[in] vregs_c1 A const reference to a set of registers denoting the
                          first virtual leaf register to be checked, including
                          its coalescing aliases.
      @param[in] vregs_c2 A const reference to a set of registers denoting the
                          second virtual leaf register to be checked, including
                          its coalescing aliases.

      If so, the respective function call and the physical register are stored
      in mLocalCallerSavedRegsAliveAcrossCall.

      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    void checkCallerSavedRegsAliveAcrossCall( const WIR_PhysicalRegister &,
                                              const WIR_RegisterSet &,
                                              const WIR_RegisterSet & );


    //
    // Interference graph construction.
    //

    /*!
      @brief buildProcessorSpecificInterferences adds edges to the interference
             graph expressing RISC-V-specific interferences.

      @param[in] f A reference to a %WIR function.
      @param[in,out] igraph A reference to the interference graph.

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    virtual void buildProcessorSpecificInterferences( WIR_Function &,
                                                      WIR_InterferenceGraph & ) override;

    /*!
      @brief isFunctionReturnMove checks whether a given move operation stores
             the result of a function call somewhere.

      @param[in] o A const reference to a move operation.
      @return isFunctionReturnMove returns true iff
              - the register used by the move is x10 or x11, and
              - the move immediately follows a call instruction.

      @author Christian Sühl <Christian.Suehl@tuhh.de>
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

      For the RISC-V register allocator, high-priority registers are kept in set
      mHighPriorityRegs. Thus, this method only checks whether r is in
      mHighPriorityRegs or not.

      @author Simon Kopischke <Simon.Kopischke@tuhh.de>
    */
    virtual bool isPriorityRegister( const WIR_VirtualRegister & ) const override;

    /*!
      @brief getSpillLoadCosts returns the RISC-V-specific costs of one single
             spill-load for the specified register parameter.

      @param[in] p A const reference to a %WIR register parameter.
      @return Some RISC-V-specific cost measure for a spill-load of register
              parameter p.

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    virtual unsigned int getSpillLoadCosts( const WIR_RegisterParameter & ) const override;

    /*!
      @brief getSpillStoreCosts returns the RISC-V-specific costs of one single
             spill-store for the specified register parameter.

      @param[in] p A const reference to a %WIR register parameter.
      @return Some RISC-V-specific cost measure for a spill-store of register
              parameter p.

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    virtual unsigned int getSpillStoreCosts( const WIR_RegisterParameter & ) const override;

    /*!
      @brief getMoveCosts returns the RISC-V-specific costs of one single move
             operation that can be omitted due to spilling.

      @param[in] o A const reference to a move operation.
      @return Some RISC-V-specific cost measure for a move operation.

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    virtual unsigned int getMoveCosts( const WIR_Operation & ) const override;


    //
    // Coalescing.
    //

    /*!
      @brief getUseOfMove returns the used register of the specified RISC-V %WIR
             move operation.

      @param[in] o A const reference to a move operation.
      @return A reference to the %WIR register used by the move operation.

      @author Simon Kopischke <Simon.Kopischke@tuhh.de>
    */
    virtual WIR_BaseRegister &getUseOfMove( const WIR_Operation & ) const override;

    /*!
      @brief getDefOfMove returns the defined register of the specified RISC-V
             %WIR move operation.

      @param[in] o A const reference to a move operation.
      @return A reference to the %WIR register defined by the move operation.

      @author Simon Kopischke <Simon.Kopischke@tuhh.de>
    */
    virtual WIR_BaseRegister &getDefOfMove( const WIR_Operation & ) const override;


    //
    // Coloring.
    //

    /*!
      @brief selectColors assigns actual colors to the RISC-V leaf registers in
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

      The registers are prioritized according to mOrderedPhregs.

      @author Simon Kopischke <Simon.Kopischke@tuhh.de>
    */
    virtual WIR_GraphColoring::WIR_ColorMap selectColors( const std::vector<std::reference_wrapper<WIR_VirtualRegister>> &,
                                                          const WIR_InterferenceGraph & )  override;


    //
    // Code transformation.
    //

    /*!
      @brief getStackPosOfSubReg returns the stack position of some child
             register, if the root of the entire register hierarchy is located
             in the specified stack position.

      @param[in] r A const reference to a virtual %WIR child register.
      @param[in] rootPos The position of r's root register on the stack.
      @return The according stack position of r on the stack.

      @note: RISC-V registers are not really hierarchical, but the notion of
             hierarchical registers is used to conveniently implement pairs of
             registers for storing data types with double word width. Thus, this
             method returns the offset for the child register accordingly.

      @author Simon Kopischke <Simon.Kopischke@tuhh.de>
    */
    virtual unsigned int getStackPosOfSubReg( const WIR_VirtualRegister &,
                                              unsigned int ) const override;

    /*!
      @brief insertSpillLoad inserts RISC-V code for a spill-load of a register
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

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    virtual void insertSpillLoad( const WIR_BaseRegister &,
                                  const WIR_BaseRegister &,
                                  int, WIR_BasicBlock &,
                                  std::list<std::reference_wrapper<WIR_Instruction>>::const_iterator ) override;

    /*!
      @brief insertSpillStore inserts RISC-V code for a spill-store of a
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

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    virtual void insertSpillStore( const WIR_BaseRegister &,
                                   const WIR_BaseRegister &,
                                   int, WIR_BasicBlock &,
                                   std::list<std::reference_wrapper<WIR_Instruction>>::const_iterator ) override;

    /*!
      @brief insertSpillCode inserts RISC-V code for a spill-load or spill-store
             of a register into the %WIR.

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

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    virtual WIR_BasicBlock &insertSpillCode( const WIR_BaseRegister &,
                                             const WIR_BaseRegister &,
                                             int, WIR_BasicBlock &,
                                             std::list<std::reference_wrapper<WIR_Instruction>>::const_iterator,
                                             bool );

    /*!
      @brief isAdjustedLoadOrStoreInstruction returns whether the given iterator
             refers to a load or a store instruction with a stack pointer
             relative addressing mode which is surrounded by ADDI instructions
             that adjust the stack pointer.

      @param[in] b A const reference to a %WIR basic block whose instructions
                   shall be checked.
      @param[in] pos A const iterator refering to the position of an instruction
                     to be checked.
      @return true iff pos refers to an adjusted stack access, false otherwise.

     isAdjustedLoadOrStoreInstruction checks if the code looks like

       addi   x2, x2, const1               # with const1 > 0
       l__    __, __(x2)                   # pos
       addi   x2, x2, -const2              # with const2 > 0

     or

       addi   x2, x2, const1               # with const1 > 0
       s__    __, __(x2)                   # pos
       addi   x2, x2, -const2              # with const2 > 0

     TODO: Since the current implementation of handling offsets larger than 12
           bits violates the RISC-V ABI, this needs to be rewritten once that
           the implementation is changed to be ABI-compatible.

      @author Simon Kopischke <Simon.Kopischke@tuhh.de>
    */
    bool isAdjustedLoadOrStoreInstruction( const WIR_BasicBlock &,
                                           std::list<std::reference_wrapper<WIR_Instruction>>::const_iterator ) const;

    /*!
      @brief getCandidatePhregs returns a set of physical registers that could
             be used for the specified virtual register according to the
             RISC-V's ISA.

      @param[in] r A const reference to a virtual %WIR register.
      @return A set of physical RISC-V registers.

      @author Simon Kopischke <Simon.Kopischke@tuhh.de>
    */
    virtual WIR_PhysicalRegisterSet getCandidatePhregs( const WIR_VirtualRegister & ) override;

    /*!
      @brief getCandidatePhreg returns one element from the specified set of
             registers that will finally be used within
             allocateUncoloredActualSpills for spilling.

      @param[in] candidates A const reference to a set of potential candidate
                            phregs for spilling.
      @return A reference to one of the registers in set candidates.

      @author Simon Kopischke <Simon.Kopischke@tuhh.de>
    */
    virtual const WIR_PhysicalRegister &getCandidatePhreg( const WIR_PhysicalRegisterSet & ) override;

    /*!
      @brief postProcessingHook allows to perform RISC-V-specific actions after
             having done register allocation for a function, using e.g., the set
             of inserted spill operations mInsertedSpillCode.

      @param[in,out] f A reference to a %WIR function.

      Here, postProcessingHook is used to realize the RISC-V-specific calling
      conventions afterwards.

      @author Christian Sühl <Christian.Suehl@tuhh.de>
    */
    virtual void postProcessingHook( WIR_Function & ) override;

    /*!
      @brief adjustStack allocates additional space in the specified function's
             stack frame and adjusts all stack-related memory accesses
             accordingly.

      @param[in,out] f A reference to a %WIR function.

      According to the RISC-V ABI (section 2.1, Integer Calling Convention), the
      stack grows downwards (towards lower addresses) and the stack pointer
      shall be aligned to a 128-bit boundary upon procedure entry. The first
      argument passed on the stack is located at offset zero of the stack
      pointer on function entry; following arguments are stored at
      correspondingly higher addresses.

      In the standard ABI, the stack pointer must remain aligned throughout
      procedure execution. [...]

      Procedures must not rely upon the persistence of stack-allocated data
      whose addresses lie below the stack pointer.

      @verbatim
      (Stack
       growing
       direction)
           |
           |   +-------------------------+      (high address)
           |   | Local Variables Func 1  |
           |   +-------------------------+
           |   | Argument Area for func- |
           |   | tions called by Func 1  |      (first argument passed on stack)
           |   +-------------------------+
           |   | Local Variables Func 2  |
           |   +-------------------------+
           |   | Argument Area for func- |
           |   | tions called by Func 2  |
           |   +-------------------------+ <--- Stack Pointer (SP) at entry
           V   | Local Variables Func 3  |      (CALL) to Function 3
               +-------------------------+
               | Argument Area for func- |
               | tions called by Func 3  |
               +-------------------------+ <--- Stack Pointer (SP) after stack
               |                         |      allocation of Function 3
               |           ...           |
               +-------------------------+      (low address)
      @endverbatim

      @author Simon Kopischke <Simon.Kopischke@tuhh.de>
    */
    virtual void adjustStack( WIR_Function & ) override;

    /*!
      @brief postRACleanup allows to perform very final RISC-V-specific cleanup
             actions, particularly after stack frame reorganization.

      @param[in] f A reference to a %WIR function.

      Here, postRACleanup is used to remove redundant MOV instructions.
      Furthermore, don't optimize flags of parameters (indicating accesses to
      the RISC-V's argument overflow stack region) are reset.

      @author Christian Sühl <Christian.Suehl@tuhh.de>
    */
    virtual void postRACleanup( WIR_Function & ) override;

    /*!
      @brief For a list of instructions implementing one spill-load or -store,
             getPhregOfSpill determines that physical register that is actually
             spill-loaded or -stored.

      @param[in] spill A const reference to a list of spill instructions.

      @author Ben Bahe <Ben.Bahe@tuhh.de>
    */
    virtual WIR_PhysicalRegister &getPhregOfSpill( const std::list<
                                                           std::reference_wrapper<
                                                             WIR_Instruction>> & ) override;


    //
    // Attributes.
    //

    /*!
      @brief mHighPriorityRegs contains the IDs of all RISC-V registers that
             shall be colored with high priority so that they are likely not to
             be spilled.
    */
    std::set<WIR_id_t> mHighPriorityRegs;

    /*!
      @brief mUncoloredSpill points to a virtual register that was artificially
             created during spill code generation.
    */
    WIR_VirtualRegister *mUncoloredSpill;

    /*!
      @brief mHaveRV32IF stores whether register allocation shall be performed
             for RV32IF compatible architectures featuring 32 bits wide
             floating-point registers.
    */
    bool mHaveRV32IF;

    /*!
      @brief mHaveRV32ID stores whether register allocation shall be performed
             for RV32ID compatible architectures featuring 64 bits wide
             floating-point registers.
    */
    bool mHaveRV32ID;


  private:

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
      @brief mOrderedPhregs holds all RISC-V physical registers in the
             precedence order in which they should be used for coloring.
    */
    std::vector<std::reference_wrapper<WIR_PhysicalRegister>> mOrderedPhregs;

    /*!
      @brief mOrderedPhregsPairs holds all RISC-V physical register pairs in the
             precedence order in which they should be used for coloring. That
             is, the caller gets an even/odd pair of registers according to the
             same precedence as defined in mOrderedPhregs for getting just one
             simple register.
    */
    std::vector<
      std::reference_wrapper<WIR_PhysicalRegister>> mOrderedPhregPairs;

    /*!
      @brief mOrderedREGsAliveAcrossCall holds all RISC-V registers in the
             precedence order in which they should be used for coloring of VREGs
             alive across a function call.
    */
    std::vector<
      std::reference_wrapper<WIR_PhysicalRegister>> mOrderedREGsAliveAcrossCall;

};

}       // namespace WIR

#endif  // _RV_GRAPHCOLORING_H
