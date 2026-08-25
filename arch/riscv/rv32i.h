/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32i.h
  @brief This file provides the specific interface of the RISC-V RV32I Base
         Integer instruction set, version 2.0.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
*/


#ifndef _RV32I_H
#define _RV32I_H


//
// Include section
//

// Include WIR headers
#include <wir/wirprocessor.h>

#include <arch/riscv/rvconst5unsigned.h>
#include <arch/riscv/rvconst12signed.h>
#include <arch/riscv/rvconst20unsigned.h>
#include <arch/riscv/rvio.h>
#include <arch/riscv/rvqregvirtual.h>
#include <arch/riscv/rveregphysical.h>
#include <arch/riscv/rveregvirtual.h>
#include <arch/riscv/rvregphysical.h>
#include <arch/riscv/rvregvirtual.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class RV32I models the RISC-V RV32I Base Integer instruction set
         architecture, version 2.0.

  Specific other RISC-V variants like, e.g., the RV32IC or RV32IM ISAs are
  derived from this class by inheritance.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
*/
class RV32I : public WIR_Processor<RV32I>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for RV32I processor architectures.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV32I( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV32I( const RV32I & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV32I( RV32I && );

    /*!
      @brief Destructor.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    virtual ~RV32I( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV32I & operator = ( const RV32I & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV32I & operator = ( RV32I && );


    //
    // RV32I-specific global initializations.
    //

    /*!
      @brief init performs some global initialization tasks for RV32I processor
             architectures.

      This includes setting up the assignment of valid operation formats to
      RV32I opcodes.

      @note init shall be called globally by WIR_Init(). It shall only perform
            tasks that cannot be expressed as initializations of static class
            members (since the order of static initialization is unspecified in
            C++) and that thus require execution by active code.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    static void init( void );


    //
    // Data structures used to model the RISC-V RV32I V2.0 architecture.
    //

    /*!
      @brief The public members of class OpCode model the RV32I's opcodes.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    class OpCode : public WIR_Processor::OpCode
    {

      public:

        //! Addition
        static const OpCode ADD;

        //! Add Immediate
        static const OpCode ADDI;

        //! Logical AND
        static const OpCode AND;

        //! Logical AND Immediate
        static const OpCode ANDI;

        //! Add Upper Immediate to PC
        static const OpCode AUIPC;

        //! Branch if Equal
        static const OpCode BEQ;

        //! Branch if Greater Than or Equal
        static const OpCode BGE;

        //! Branch if Greater Than or Equal Unsigned
        static const OpCode BGEU;

        //! Branch if Less Than
        static const OpCode BLT;

        //! Branch if Less Than Unsigned
        static const OpCode BLTU;

        //! Branch if Not Equal
        static const OpCode BNE;

        //! Atomic Read and Clear Bits in CSR
        static const OpCode CSRRC;

        //! Atomic Read and Clear Bits in CSR Immediate
        static const OpCode CSRRCI;

        //! Atomic Read and Set Bits in CSR
        static const OpCode CSRRS;

        //! Atomic Read and Set Bits in CSR Immediate
        static const OpCode CSRRSI;

        //! Atomic Read/Write CSR
        static const OpCode CSRRW;

        //! Atomic Read/Write CSR Immediate
        static const OpCode CSRRWI;

        //! Environment Break
        static const OpCode EBREAK;

        //! Environment Call
        static const OpCode ECALL;

        // Synchronization between RISC-V harts
        // Since this instruction is not clearly defined in the RISC-V ISA
        // manual, it is not implemented here.
        // static const OpCode FENCE;

        // Synchronization between instruction and data streams
        // Since this instruction is not clearly defined in the RISC-V ISA
        // manual, it is not implemented here.
        // static const OpCode FENCEI;

        //! Unconditional jump pseudo-operation, de facto realized by a JAL.
        static const OpCode J;

        //! Jump And Link (function call)
        static const OpCode JAL;

        //! Jump And Link Register
        static const OpCode JALR;

        //! Jump Register pseudo-operation de facto realized by a JALR.
        static const OpCode JR;

        //! Load Byte
        static const OpCode LB;

        //! Load Byte Unsigned
        static const OpCode LBU;

        //! Load Halfword
        static const OpCode LH;

        //! Load Halfword Unsigned
        static const OpCode LHU;

        //! Load Upper Immediate
        static const OpCode LUI;

        //! Load Word
        static const OpCode LW;

        //! Move pseudo-operation, de facto realized by an ADDI.
        static const OpCode MOV;

        //! Logical OR
        static const OpCode OR;

        //! Logical OR Immediate
        static const OpCode ORI;

        //! Return pseudo-operation, de facto realized by a JALR.
        static const OpCode RET;

        //! Store Byte
        static const OpCode SB;

        //! Store Halfword
        static const OpCode SH;

        //! Shift Left Logical
        static const OpCode SLL;

        //! Shift Left Logical Immediate
        static const OpCode SLLI;

        //! Set Less Than
        static const OpCode SLT;

        //! Set Less Than Immediate
        static const OpCode SLTI;

        //! Set Less Than Immediate Unsigned
        static const OpCode SLTIU;

        //! Set Less Than Unsigned
        static const OpCode SLTU;

        //! Shift Right Arithmetical
        static const OpCode SRA;

        //! Shift Right Arithmetical Immediate
        static const OpCode SRAI;

        //! Shift Right Logical
        static const OpCode SRL;

        //! Shift Right Logical Immediate
        static const OpCode SRLI;

        //! Subtraction
        static const OpCode SUB;

        //! Store Word
        static const OpCode SW;

        //! Logical XOR
        static const OpCode XOR;

        //! Logical XOR Immediate
        static const OpCode XORI;


      protected:

        // Inherit the Constructors from WIR_Processor::OpCode.
        using WIR_Processor::OpCode::OpCode;

    };

    /*!
      @brief The public members of class RegisterType model the different types
             of RISC-V registers.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    class RegisterType : public WIR_Processor::RegisterType
    {

      public:

        //! All general-purpose registers x0 - x31 (x0 hardwired to constant 0).
        static const RegisterType reg;

        //! Pairs of registers.
        static const RegisterType eReg;

        //! Quadruples of registers.
        static const RegisterType qReg;


      protected:

        // Inherit the constructors from WIR_Processor::RegisterType.
        using WIR_Processor::RegisterType::RegisterType;

    };

    /*!
      @brief The public members of class OperationFormat model the RV32I's
             different formats of machine operations.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    class OperationFormat : public WIR_Processor::OperationFormat
    {

      public:

        //! 32-bit label
        static const OperationFormat L_1;

        //! 32-bit without operands
        static const OperationFormat NULL_1;

        //! 32-bit reg (use)
        static const OperationFormat R_1;

        //! 32-bit reg (def), reg (use), signed const12
        static const OperationFormat RC12R_1;

        //! 32-bit reg (use), reg (use), signed const12
        static const OperationFormat RC12R_2;

        //! 32-bit reg (def), unsigned const20
        static const OperationFormat RC20_1;

        //! 32-bit reg (def), label
        static const OperationFormat RL_1;

        //! 32-bit reg (def), %hi(label)
        static const OperationFormat RL_2;

        //! 32-bit reg (def), %lo(label), reg (use)
        static const OperationFormat RLR_1;

        //! 32-bit reg (use), %lo(label), reg (use)
        static const OperationFormat RLR_2;

        //! 32-bit reg (def), reg (use), unsigned const5
        static const OperationFormat RRC5_1;

        //! 32-bit reg (def), reg (use), signed const12
        static const OperationFormat RRC12_1;

        //! 32-bit reg (use), reg(use), label
        static const OperationFormat RRL_1;

        //! 32-bit reg (def), reg (use), %lo(label)
        static const OperationFormat RRL_2;

        //! 32-bit reg (def), reg (use)
        static const OperationFormat RR_1;

        //! 32-bit reg (def), reg (use), reg (use)
        static const OperationFormat RRR_1;

        /*!
          @brief 32-bit reg (def), string, unsigned const5

          The string parameter is allowed to be one of "frm", "fcsr", "fflags",
          "instret", "instreth", "cycle", "cycleh", "time" or "timeh".
        */
        static const OperationFormat RSC5_1;

        /*!
          @brief 32-bit reg (def), string, reg (use)

          The string parameter is allowed to be one of "frm", "fcsr", "fflags",
          "instret", "instreth", "cycle", "cycleh", "time" or "timeh".
        */
        static const OperationFormat RSR_1;


      protected:

        // Inherit the constructors from WIR_Processor::OperationFormat.
        using WIR_Processor::OperationFormat::OperationFormat;

    };

    /*!
      @brief Access to physical general-purpose register x0.
      @return A const reference to x0.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x0( void ) const;

    /*!
      @brief Access to physical general-purpose register x1.
      @return A const reference to x1.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x1( void ) const;

    /*!
      @brief Access to physical general-purpose register x2.
      @return A const reference to x2.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x2( void ) const;

    /*!
      @brief Access to physical general-purpose register x3.
      @return A const reference to x3.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x3( void ) const;

    /*!
      @brief Access to physical general-purpose register x4.
      @return A const reference to x4.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x4( void ) const;

    /*!
      @brief Access to physical general-purpose register x5.
      @return A const reference to x5.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x5( void ) const;

    /*!
      @brief Access to physical general-purpose register x6.
      @return A const reference to x6.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x6( void ) const;

    /*!
      @brief Access to physical general-purpose register x7.
      @return A const reference to x7.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x7( void ) const;

    /*!
      @brief Access to physical general-purpose register x8.
      @return A const reference to x8.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x8( void ) const;

    /*!
      @brief Access to physical general-purpose register x9.
      @return A const reference to x9.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
   const RV_RegP &x9( void ) const;

    /*!
      @brief Access to physical general-purpose register x10.
      @return A const reference to x10.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x10( void ) const;

    /*!
      @brief Access to physical general-purpose register x11.
      @return A const reference to x11.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x11( void ) const;

    /*!
      @brief Access to physical general-purpose register x12.
      @return A const reference to x12.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x12( void ) const;

    /*!
      @brief Access to physical general-purpose register x13.
      @return A const reference to x13.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x13( void ) const;

    /*!
       @brief Access to physical general-purpose register x14.
       @return A const reference to x14.
       @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x14( void ) const;

    /*!
      @brief Access to physical general-purpose register x15.
      @return A const reference to x15.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x15( void ) const;

    /*!
      @brief Access to physical general-purpose register x16.
      @return A const reference to x16.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x16( void ) const;

    /*!
      @brief Access to physical general-purpose register x17.
      @return A const reference to x17.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x17( void ) const;

    /*!
      @brief Access to physical general-purpose register x18.
      @return A const reference to x18.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x18( void ) const;

    /*!
      @brief Access to physical general-purpose register x19.
      @return A const reference to x19.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x19( void ) const;

    /*!
      @brief Access to physical general-purpose register x20.
      @return A const reference to x20.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x20( void ) const;

    /*!
      @brief Access to physical general-purpose register x21.
      @return A const reference to x21.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x21( void ) const;

    /*!
      @brief Access to physical general-purpose register x22.
      @return A const reference to x22.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x22( void ) const;

    /*!
      @brief Access to physical general-purpose register x23.
      @return A const reference to x23.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x23( void ) const;

    /*!
      @brief Access to physical general-purpose register x24.
      @return A const reference to x24.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x24( void ) const;

    /*!
      @brief Access to physical general-purpose register x25.
      @return A const reference to x25.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x25( void ) const;

    /*!
      @brief Access to physical general-purpose register x26.
      @return A const reference to x26.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x26( void ) const;

    /*!
      @brief Access to physical general-purpose register x27.
      @return A const reference to x27.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x27( void ) const;

    /*!
      @brief Access to physical general-purpose register x28.
      @return A const reference to x28.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x28( void ) const;

    /*!
      @brief Access to physical general-purpose register x29.
      @return A const reference to x29.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x29( void ) const;

    /*!
      @brief Access to physical general-purpose register x30.
      @return A const reference to x30.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x30( void ) const;

    /*!
      @brief Access to physical general-purpose register x31.
      @return A const reference to x31.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &x31( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e0 (x0 and x1).
      @return A const reference to p0.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e0( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e2 (x2 and x3).
      @return A const reference to p2.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e2( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e4 (x4 and x5).
      @return A const reference to p4.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e4( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e6 (x6 and x7).
      @return A const reference to p6.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e6( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e8 (x8 and x9).
      @return A const reference to p8.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e8( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e10 (x10 and x11).
      @return A const reference to p10.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e10( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e12 (x12 and x13).
      @return A const reference to p12.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e12( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e14 (x14 and x15).
      @return A const reference to p14.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e14( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e16 (x16 and x17).
      @return A const reference to p16.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e16( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e18 (x18 and x19).
      @return A const reference to p18.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e18( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e20 (x20 and x21).
      @return A const reference to p20.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e20( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e22 (x22 and x23).
      @return A const reference to p22.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e22( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e24 (x24 and x25).
      @return A const reference to p24.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e24( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e26 (x26 and x27).
      @return A const reference to p26.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e26( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e28 (x28 and x29).
      @return A const reference to p28.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e28( void ) const;

    /*!
      @brief Access to physical general-purpose register pair e30 (x30 and x31).
      @return A const reference to p30.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    const RV_ERegP &e30( void ) const;

    /*!
      @brief Access to return address pointer.
      @return A const reference to x1.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &RA( void ) const;

    /*!
      @brief Access to stack pointer.
      @return A const reference to x2.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    const RV_RegP &SP( void ) const;

    /*!
      @brief isX0 checks whether the specified register is the RISC-V's x0
             register which is always zero.

      @param[in] r A const reference to a register to be checked.
      @return true if the specified register is physical and is x0 or if it is
              virtual and precolored with x0, false otherwise.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    static bool isX0( const WIR_BaseRegister & );

    /*!
      @brief isX1 checks whether the specified register is the RISC-V's x1
             register which is the link register.

      @param[in] r A const reference to a register to be checked.
      @return true if the specified register is physical and is x1 or if it is
              virtual and precolored with x1, false otherwise.

      @author Sasha Tosta <Sasha.Tosta@tuhh.de>
    */
    static bool isX1( const WIR_BaseRegister & );

    /*!
      @brief isFP checks whether the specified register is the RISC-V's frame
             pointer.

      @param[in] r A const reference to a register to be checked.
      @return true if the specified register is physical and is the frame
              pointer or if it is virtual and precolored with x8, false
              otherwise.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    static bool isFP( const WIR_BaseRegister & );

    /*!
      @brief isSP checks whether the specified register is the RISC-V's stack
             pointer.

      @param[in] r A const reference to a register to be checked.
      @return true if the specified register is physical and is the stack
              pointer or if it is virtual and precolored with x2, false
              otherwise.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    static bool isSP( const WIR_BaseRegister & );

    /*!
      @brief boolBytes contains the width of a _Bool in bytes for the RV32I
             architecture.

      See RISC-V ABIs Specification, section 4.1.
    */
    static const int boolBytes = ( 8 / 8 );

    /*!
      @brief charBytes contains the width of a char in bytes for the RV32I
             architecture.

      See RISC-V ABIs Specification, section 4.1.
    */
    static const int charBytes = ( 8 / 8 );

    /*!
      @brief shortBytes contains the width of a short in bytes for the RV32I
             architecture.

      See RISC-V ABIs Specification, section 4.1.
    */
    static const int shortBytes = ( 16 / 8 );

    /*!
      @brief intBytes contains the width of an int in bytes for the RV32I
             architecture.

      See RISC-V ABIs Specification, section 4.1.
    */
    static const int intBytes = ( 32 / 8 );

    /*!
      @brief longBytes contains the width of a long in bytes for the RV32I
             architecture.

      See RISC-V ABIs Specification, section 4.1.
    */
    static const int longBytes = ( 32 / 8 );

    /*!
      @brief longLongBytes contains the width of a long long in bytes for the
             RV32I architecture.

      See RISC-V ABIs Specification, section 4.1.
    */
    static const int longLongBytes = ( 64 / 8 );

    /*!
      @brief pointerBytes contains the width of a pointer in bytes for the RV32I
             architecture.

      See RISC-V ABIs Specification, section 4.1.
    */
    static const int pointerBytes = ( 32 / 8 );

    /*!
      @brief floatBytes contains the width of a float in bytes for the RV32I
             architecture.

      See RISC-V ABIs Specification, section 4.1.
    */
    static const int floatBytes = ( 32 / 8 );

    /*!
      @brief doubleBytes contains the width of a double in bytes for the RV32I
             architecture.

      See RISC-V ABIs Specification, section 4.1.
    */
    static const int doubleBytes = ( 64 / 8 );

    /*!
      @brief longDoubleBytes contains the width of a long double in bytes for the
             RV32I architecture.

      See RISC-V ABIs Specification, section 4.1.
    */
    static const int longDoubleBytes = ( 128 / 8 );


  private:

    /*!
      @brief clone creates a copy of an RV32I processor.

      @return A pointer to the newly created RV32I copy.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    virtual WIR_BaseProcessor *clone( void ) const override;

};

}       // namespace WIR

#endif  // _RV32I_H
