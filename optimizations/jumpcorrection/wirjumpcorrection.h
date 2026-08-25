/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirjumpcorrection.h
  @brief This file provides the interface of a generic optimimzation detecting
         and correcting jump instructions with too large displacements.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_JUMPCORRECTION_H
#define _WIR_JUMPCORRECTION_H


//
// Include section
//

// Include standard headers
#include <list>
#include <map>

// Include WIR headers
#include <wir/wirtypes.h>
#include <optimizations/generic/wiroptimization.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_BasicBlock;
class WIR_CompilationUnit;
class WIR_Function;
class WIR_Operation;
class WIR_Symbol;
class WIR_System;


/*!
  @brief Class WIR_JumpCorrection models a generic jump-correcting optimization.

  Actual processor-specific variants are created by inheriting from this virtual
  base class.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_JumpCorrection : public WIR_Optimization
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
    explicit WIR_JumpCorrection( WIR_System & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_JumpCorrection( void );


    //
    // Optimization management.
    //

    /*!
      @brief setSinglePass sets whether jump correction is applied once or if it
             executes until a fixed point is reached.

      @param[in] b A Boolean defaulting to true that specifies whether jump
                   correction applies once or until a fixed point is reached.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void setSinglePass( bool = true );

    /*!
      @brief getSinglePass returns whether jump correction is applied once or if
             it executes until a fixed point is reached.

      @return true if jump correction executes only once, false otherwise.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    bool getSinglePass( void ) const;


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

      Since the computation of jump correction costs is processor-specific, this
      method is purely virtual.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual int getJumpCostsSize( const WIR_BasicBlock &s,
                                  const WIR_BasicBlock &t, bool addToSrc ) = 0;

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

      Since the computation of jump correction costs is processor-specific, this
      method is purely virtual.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual int getJumpCostsCycles( const WIR_BasicBlock &s,
                                    const WIR_BasicBlock &t,
                                    bool addToSrc ) = 0;


  protected:

    /*!
      @brief struct MemLayoutInfo contains information about start addresses of
             basic blocks, alignments and section ends etc.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    struct MemLayoutInfo
    {

      public:

        /*!
          @brief block points to the %WIR basic block represented by this
                 struct.
        */
        const WIR_BasicBlock *block;

        //! addr stores a start address in memory.
        unsigned long long address;

        /*!
          @brief alignment stores the required alignment for a start address.

          The alignment denotes the number of least-significant zero bits that
          the address must have. An alignment of 3 means that the
          least-significant 3 bits are '0', i.e., that the resulting relocation
          counter is a multiple of 8.
        */
        unsigned int alignment;

        //! fixedAddress stores whether an address is fix and does not change.
        bool fixedAddress;

        /*!
          @brief Constructor initializing a memory layout info with the given
                 address and alignment 0.

          @param[in] b A const reference to a %WIR basic block.
          @param[in] adr An unsigned long denoting the given start address.

          @author Heiko Falk <Heiko.Falk@tuhh.de>
        */
        MemLayoutInfo( const WIR_BasicBlock &, unsigned long long );

    };

    /*!
      @brief struct InvalidJump is a container storing useful information about
             invalid jumps that have to be corrected.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    struct InvalidJump
    {
      //! A const reference to the invalid jump operation.
      const WIR_Operation &op;

      //! A const reference to the symbol of the invalid jump's source.
      const WIR_Symbol &srcSym;

      //! A const reference to the symbol of the invalid jump's target.
      const WIR_Symbol &tgtSym;

      //! The displacement of the invalid jump.
      const WIR_disp_t disp;

      //! A Boolean flag denoting whether the invalid jump is implicit or not.
      const bool isImplicit;

      /*!
        @brief Default constructor for invalid jump containers.

        @param[in] o A const reference to the invalid jump operation.
        @param[in] s A const reference to the symbol of the invalid jump's
                     source.
        @param[in] t A const reference to the symbol of the invalid jump's
                     target.
        @param[in] d The invalid jump's displacement.
        @param[in] i A Boolean flag specifying whether o is an implicit jump or
                     not.

        @author Heiko Falk <Heiko.Falk@tuhh.de>
      */
      InvalidJump( const WIR_Operation &, const WIR_Symbol &,
                   const WIR_Symbol &, const WIR_disp_t, const bool );
    };


    /*!
      @brief runOptimization performs jump correction in the given system.

      @param[in] s A reference to a %WIR_System to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOptimization( WIR_System & ) override;

    /*!
      @brief runOptimization performs jump correction in the given compilation
             unit.

      @param[in] c A reference to a %WIR_CompilationUnit to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOptimization( WIR_CompilationUnit & ) override;

    /*!
      @brief runOptimization performs jump correction in the given function.

      @param[in] f A reference to a %WIR_Function to be optimized.

      Since the actual task of jump correction within a function is processor-
      specific, this method is purely virtual.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOptimization( WIR_Function & ) override;

    /*!
      @brief runOpt performs jump correction in the given basic block.

      @param[in] b A reference to a %WIR_BasicBlock to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOpt( WIR_BasicBlock &b ) = 0;

    /*!
      @brief initializeMemoryLayout sets up the jump correction's internal data
             structures representing the system's memory layout.

      This method is virtual and thus can be overloaded if required for
      processor-specific initializations.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void initializeMemoryLayout( void );

    /*!
      @brief updateMemoryLayout incrementally updates the jump correction's
             internal memory layout data structures from the position of the
             specified basic block on in memory.

      @param[in] b A const reference to a basic block from where on start
                   addresses in the memory layout need to be updated.

      This method is virtual and thus can be overloaded if required for
      processor-specific updates.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void updateMemoryLayout( const WIR_BasicBlock & );

    /*!
      @brief updateMemoryLayout incrementally updates the jump correction's
             internal memory layout data structures for the complete specified
             function.

      @param[in] f A const reference to a function whose start addresses in the
                   memory layout need to be updated.

      This method is virtual and thus can be overloaded if required for
      processor-specific updates.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void updateMemoryLayout( const WIR_Function & );

    /*!
      @brief verifyMemoryLayout verifies that the jump correction's internal
             memory layout is fully coherent with the overall system's memory
             layout.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void verifyMemoryLayout( void ) const;

    /*!
      @brief computeDisplacement determines the displacement between the source
             basic block and the given symbol.

      @param[in] src A const reference to the source basic block of a jump.
      @param[in] tgtSym A const reference to a symbol denoting a jump target.
      @param[in] o A const reference to the jump operation in basic block src.
      @return The signed distance in memory between source and target.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_disp_t computeDisplacement( const WIR_BasicBlock &,
                                    const WIR_Symbol &,
                                    const WIR_Operation & ) const;

    /*!
      @brief isPhysicalSuccessor determines whether one basic blocks is a direct
             successor of another basic block in the address space.

      @param[in] b1 A const reference to the first basic block.
      @param[in] b2 A const reference to the second basic block.
      @return true if b2 is physical successor of b1, false otherwise.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    bool isPhysicalSuccessor( const WIR_BasicBlock &,
                              const WIR_BasicBlock & ) const;

    /*!
      @brief getAddress determines the start address of a basic block.

      @param[in] b A const reference to a %WIR basic block.
      @return An unsigned long value denoting the block's start address.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    unsigned long long getAddress( const WIR_BasicBlock & ) const;


    //
    // Attributes
    //

    //! mSystem holds a reference to the %WIR system to be optimized.
    WIR_System &mSystem;

    /*!
      @brief mPhysicalWIR stores whether jump correction is applied to physical
             or virtual %WIR code.
    */
    bool mPhysicalWIR;

    /*!
      @brief mSinglePass stores whether jump correction is applied once or if it
             executes until a fixed point is reached.
    */
    bool mSinglePass;

    //! mCorrectedJumps stores the number of corrected jump instructions.
    unsigned int mCorrectedJumps;

    /*!
      @brief mMemLayout is a condensed data structure representing the memory
             layout of the program under optimization.

      mMemLayout contains a list of layout structs per physical memory region.
      The memory regions are represented by their IDs which serve as search key
      in the map. The list of structs is sorted (ascending) by their memory
      addresses.

      The purpose of this data structure is to avoid costly and time-consuming
      full-fledged re-computations of the whole memory layout upon each
      correction of a jump operation.
    */
    std::map<WIR_id_t, std::list<MemLayoutInfo>> mMemLayout;

    /*!
      @brief mBBPosition maps the ID of a basic block to its position within the
             lists of mMemLayout.

      mBBPosition allows a very fast lookup of a basic block in the jump
      correction's memory layout data structure.
    */
    std::map<WIR_id_t, std::list<MemLayoutInfo>::iterator> mBBPosition;


  private:

    /*!
      @brief doMemoryLayoutVerification performs the actual verification of the
             jump correction's internal memory layout.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void doMemoryLayoutVerification( void ) const;

};

}       // namespace WIR

#endif  // _WIR_JUMPCORRECTION_H
