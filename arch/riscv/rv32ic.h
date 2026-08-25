/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32ic.h
  @brief This file provides the specific interface of the RISC-V RV32IC Base
         Integer instruction set plus the C Standard Extension for Compressed
         Instructions, version 2.0.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
*/


#ifndef _RV32IC_H
#define _RV32IC_H


//
// Include section
//

// Include WIR headers
#include <arch/riscv/rv32i.h>
#include <arch/riscv/rvconst6signed.h>
#include <arch/riscv/rvconst6unsigned.h>
#include <arch/riscv/rvconst7unsigned.h>
#include <arch/riscv/rvconst8unsigned.h>
#include <arch/riscv/rvconst10signed.h>
#include <arch/riscv/rvconst10unsigned.h>
#include <arch/riscv/rvnzconst20unsigned.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class RV32IC models the RISC-V RV32IC Base Integer instruction set plus
         the C Standard Extension for Compressed Instructions, version 2.0.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
*/
class RV32IC : virtual public RV32I
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for RV32IC processor architectures.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV32IC( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV32IC( const RV32IC & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV32IC( RV32IC && );

    /*!
      @brief Destructor.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    virtual ~RV32IC( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV32IC & operator = ( const RV32IC & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV32IC & operator = ( RV32IC && );


    //
    // RV32IC-specific global initializations.
    //

    /*!
      @brief init performs some global initialization tasks for RV32IC processor
             architectures.

      This includes setting up the assignment of valid operation formats to
      RV32IC opcodes.

      @note init shall be called globally by WIR_Init(). It shall only perform
            tasks that cannot be expressed as initializations of static class
            members (since the order of static initialization is unspecified in
            C++) and that thus require execution by active code.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    // cppcheck-suppress duplInheritedMember
    static void init( void );


    //
    // Data structures used to model the RISC-V RV32IC V2.0 architecture.
    //

    /*!
      @brief The public members of class OpCode model the RV32IC's opcodes.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    class OpCode : public RV32I::OpCode
    {

      public:

        //! Compressed Addition
        static const OpCode CADD;

        //! Compressed Add Immediate
        static const OpCode CADDI;

        //! Compressed Stack Pointer Adjustment
        static const OpCode CADDI16SP;

        //! Compressed Pointer Generation
        static const OpCode CADDI4SPN;

        //! Compressed Logical AND
        static const OpCode CAND;

        //! Compressed Logical AND Immediate
        static const OpCode CANDI;

        //! Compressed Branch if Equal Zero
        static const OpCode CBEQZ;

        //! Compressed Branch if Not Equal Zero
        static const OpCode CBNEZ;

        //! Compressed Environment Break
        static const OpCode CEBREAK;

        //! Compressed Jump
        static const OpCode CJ;

        //! Compressed Jump And Link
        static const OpCode CJAL;

        //! Compressed Jump And Link Register
        static const OpCode CJALR;

        //! Compressed Jump Register
        static const OpCode CJR;

        //! Compressed Load Immediate
        static const OpCode CLI;

        //! Compressed Load Upper Immediate
        static const OpCode CLUI;

        //! Compressed Load Word
        static const OpCode CLW;

        //! Compressed Load Word from Stack
        static const OpCode CLWSP;

        //! Compressed Move
        static const OpCode CMV;

        //! No Operation
        static const OpCode CNOP;

        //! Compressed Logical OR
        static const OpCode COR;

        //! Compressed Shift Left Logical Immediate
        static const OpCode CSLLI;

        //! Compressed Shift Right Arithmetical Immediate
        static const OpCode CSRAI;

        //! Compressed Shift Right Logical Immediate
        static const OpCode CSRLI;

        //! Compressed Subtraction
        static const OpCode CSUB;

        //! Compressed Store Word
        static const OpCode CSW;

        //! Compressed Store Word to Stack
        static const OpCode CSWSP;

        //! Compressed Logical XOR
        static const OpCode CXOR;


      protected:

        // Inherit the Constructors from RV32I::OpCode.
        using RV32I::OpCode::OpCode;

    };

    /*!
      @brief The public members of class OperationFormat model the RV32IC's
             different formats of machine operations.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    class OperationFormat : public RV32I::OperationFormat
    {

      public:

        //! 16-bit label
        static const OperationFormat SL_1;

        //! 16-bit without operands
        static const OperationFormat SNULL_1;

        //! 16-bit reg (use)
        static const OperationFormat SR_1;

        //! 16-bit reg (defuse), unsigned const5
        static const OperationFormat SRC5_1;

        //! 16-bit reg (def), signed const6
        static const OperationFormat SRC6_1;

        //! 16-bit reg (defuse), signed const6
        static const OperationFormat SRC6_2;

        //! 16-bit reg (def), unsigned const7, reg (use)
        static const OperationFormat SRC7R_1;

        //! 16-bit reg (use), unsigned const7, reg (use)
        static const OperationFormat SRC7R_2;

        //! 16-bit reg (def), unsigned const8, sp (use)
        static const OperationFormat SRC8R_1;

        //! 16-bit reg (use), unsigned const8, sp (use)
        static const OperationFormat SRC8R_2;

        //! 16-bit sp (defuse), signed const10
        static const OperationFormat SRC10_1;

        //! 16-bit reg (def), unsigned non-zero const20
        static const OperationFormat SRC20_1;

        //! 16-bit reg (def), label
        static const OperationFormat SRL_1;

        //! 16-bit reg (def), reg (use)
        static const OperationFormat SRR_1;

        //! 16-bit reg (defuse), reg (use)
        static const OperationFormat SRR_2;

        //! 16-bit reg (def), sp (use), unsigned const10
        static const OperationFormat SRRC10_1;


      protected:

        // Inherit the Constructors from RV32I::OperationFormat.
        using RV32I::OperationFormat::OperationFormat;

    };

    /*!
      @brief isPopularRegister checks whether the specified register is in the
             range of the RV32IC popular registers (x8 - x15).

      @param[in] r A const reference to a register to be checked.
      @return true if the specified register is physical and is in the range
              or if it is virtual and precolored and in the range, false
              otherwise.

      If an uncolored virtual register r is passed to isPopularRegister, this
      method adds interferences between r and all non-popular registers in order
      to maximally exploit compact RV32IC operations.

      @author Sasha Tosta <Sasha.Tosta@tuhh.de>
    */
    static bool isPopularRegister( const WIR_BaseRegister & );

    /*!
      @brief isStackPointerADDI checks wether the given operation is an ADDI or
             C.ADDI16SP operation that modifies the stack pointer.

      @param[in] o A const reference to a %WIR operation to be examined.
      @return True if o is an ADDI and modifies the stack pointer, false
              otherwise.

      @author Simon Kopischke <Simon.Kopischke@tuhh.de>
    */
    static bool isStackPointerADDI( const WIR_Operation & );

    /*!
      @brief isStackPointerLUIADDIADD checks whether the given operations are
             the following sequence of stack-allocating code:
             @verbatim
               LUI  <reg>, const20
               ADDI <reg>, <reg>, const12
               ADD  sp, <reg>, sp
             @endverbatim

      @param[in] o1 A const reference to a first %WIR operation to be examined.
      @param[in] o2 A const reference to a second %WIR operation to be examined.
      @param[in] o3 A const reference to a third %WIR operation to be examined.
      @return True if o1 to o3 form the above code sequence, false otherwise.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    static bool isStackPointerLUIADDIADD( const WIR_Operation &,
                                          const WIR_Operation &,
                                          const WIR_Operation & );


  private:

    /*!
      @brief clone creates a copy of an RV32IC processor.

      @return A pointer to the newly created RV32IC copy.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    virtual WIR_BaseProcessor *clone( void ) const override;

};

}       // namespace WIR

#endif  // _RV32IC_H
