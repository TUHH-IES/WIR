/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32if.h
  @brief This file provides the specific interface of the RISC-V RV32IF Floating
         Point instruction set, version 2.0.

  @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
*/


#ifndef _RV32IF_H
#define _RV32IF_H


//
// Include section
//

// Include WIR headers
#include <wir/wirprocessor.h>

#include <arch/riscv/rv32i.h>
#include <arch/riscv/rvfregphysical.h>
#include <arch/riscv/rvfregvirtual.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class RV32IF models the RISC-V RV32IF Floating Point instruction set
         architecture, version 2.0.

  RISC-V variant RV32IFD is derived from this class by inheritance.

  @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
*/
class RV32IF : virtual public RV32I
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for RV32IF processor architectures.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV32IF( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV32IF( const RV32IF & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV32IF( RV32IF && );

    /*!
      @brief Destructor.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    virtual ~RV32IF( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV32IF & operator = ( const RV32IF & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV32IF & operator = ( RV32IF && );


    //
    // RV32IF-specific global initializations.
    //

    /*!
      @brief init performs some global initialization tasks for RV32IF processor
             architectures.

      This includes setting up the assignment of valid operation formats to
      RV32IF opcodes.

      @note init shall be called globally by WIR_Init(). It shall only perform
            tasks that cannot be expressed as initializations of static class
            members (since the order of static initialization is unspecified in
            C++) and that thus require execution by active code.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    // cppcheck-suppress duplInheritedMember
    static void init( void );


    //
    // Data structures used to model the RISC-V RV32IF V2.0 architecture.
    //

    /*!
      @brief The public members of class OpCode model the RV32IF's opcodes.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    class OpCode : public RV32I::OpCode
    {

      public:

        //! Floating Point Addition, single precision
        static const OpCode FADD_S;

        //! Floating Point Classify, single precision
        static const OpCode FCLASS_S;

        //! Floating Point Conversion int to float, single precision
        static const OpCode FCVT_S_W;

        //! Floating Point Conversion unsigned int to float, single precision
        static const OpCode FCVT_S_WU;

        //! Floating Point Conversion float to int, single precision
        static const OpCode FCVT_W_S;

        //! Floating Point Conversion float to unsigned int, single precision
        static const OpCode FCVT_WU_S;

        //! Floating Point Division, single precision
        static const OpCode FDIV_S;

        //! Floating Point Equality Comparison, single precision
        static const OpCode FEQ_S;

        //! Floating Point Less-Equal Comparison, single precision
        static const OpCode FLE_S;

        //! Floating Point Less-Than Comparison, single precision
        static const OpCode FLT_S;

        //! Floating Point Load Word, single precision
        static const OpCode FLW;

        //! Floating Point Fused Multiply-Add, single precision
        static const OpCode FMADD_S;

        //! Floating Point Maximum, single precision
        static const OpCode FMAX_S;

        //! Floating Point Minimum, single precision
        static const OpCode FMIN_S;

        //! Floating Point Fused Multiply-Sub, single precision
        static const OpCode FMSUB_S;

        //! Floating Point Multiplication, single precision
        static const OpCode FMUL_S;

        //! Floating Point Move, single precision
        static const OpCode FMV_S;

        //! Floating Point Move From Float To Integer Register, single precision
        static const OpCode FMV_W_X;

        //! Floating Point Move From Integer To Float Register, single precision
        static const OpCode FMV_X_W;

        //! Floating Point Negated Fused Multiply-Add, single precision
        static const OpCode FNMADD_S;

        //! Floating Point Negated Fused Multiply-Sub, single precision
        static const OpCode FNMSUB_S;

        //! Floating Point Sign Injection, single precision
        static const OpCode FSGNJ_S;

        //! Floating Point Negated Sign Injection, single precision
        static const OpCode FSGNJN_S;

        //! Floating Point XORed Sign Injection, single precision
        static const OpCode FSGNJX_S;

        //! Floating Point Square-Root, single precision
        static const OpCode FSQRT_S;

        //! Floating Point Subtraction, single precision
        static const OpCode FSUB_S;

        //! Floating Point Store Word, single precision
        static const OpCode FSW;


      protected:

        // Inherit the Constructors from RV32I::OpCode.
        using RV32I::OpCode::OpCode;

    };

    /*!
      @brief The public members of class RegisterType model the different types
             of RISC-V registers.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    class RegisterType : public RV32I::RegisterType
    {

      public:

        //! All general-purpose floating point registers f0 - f31.
        static RegisterType fReg;


      protected:

        // Inherit the constructors from WIR_Processor::RegisterType.
        using RV32I::RegisterType::RegisterType;

    };

    /*!
      @brief The public members of class OperationFormat model the RV32IF's
             different formats of machine operations.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    class OperationFormat : public RV32I::OperationFormat
    {

      public:

        //! 32-bit fReg (def), signed const12, reg (use)
        static const OperationFormat FC12R_1;

        //! 32-bit fReg (use), signed const12, reg (use)
        static const OperationFormat FC12R_2;

        //! 32-bit fReg (def), fReg (use)
        static const OperationFormat FF_1;

        //! 32-bit fReg (def), fReg (use), fReg (use)
        static const OperationFormat FFF_1;

         //! 32-bit fReg (def), fReg (use), fReg (use), fReg (use)
        static const OperationFormat FFFF_1;

        //! 32-bit fReg (def), label, reg (use)
        static const OperationFormat FLR_1;

        //! 32-bit fReg (use), label, reg (use)
        static const OperationFormat FLR_2;

        //! 32-bit fReg (def), reg (use)
        static const OperationFormat FR_1;

        //! 32-bit fReg (def), reg (use), string
        static const OperationFormat FRS_1;

        //! 32-bit reg (def), fReg (use)
        static const OperationFormat RF_1;

         //! 32-bit reg (def), fReg (use), fReg (use)
        static const OperationFormat RFF_1;

         //! 32-bit reg (def), fReg (use), string
        static const OperationFormat RFS_1;


      protected:

        // Inherit the constructors from WIR_Processor::OperationFormat.
        using RV32I::OperationFormat::OperationFormat;

    };

    /*!
      @brief Access to physical floating-point register f0.
      @return A const reference to f0.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f0( void ) const;

    /*!
      @brief Access to physical floating-point register f1.
      @return A const reference to f1.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f1( void ) const;

    /*!
      @brief Access to physical floating-point register f2.
      @return A const reference to f2.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f2( void ) const;

    /*!
      @brief Access to physical floating-point register f3.
      @return A const reference to f3.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f3( void ) const;

    /*!
      @brief Access to physical floating-point register f4.
      @return A const reference to f4.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f4( void ) const;

    /*!
      @brief Access to physical floating-point register f5.
      @return A const reference to f5.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f5( void ) const;

    /*!
      @brief Access to physical floating-point register f6.
      @return A const reference to f6.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f6( void ) const;

    /*!
      @brief Access to physical floating-point register f7.
      @return A const reference to f7.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f7( void ) const;

    /*!
      @brief Access to physical floating-point register f8.
      @return A const reference to f8.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f8( void ) const;

    /*!
      @brief Access to physical floating-point register f9.
      @return A const reference to f9.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f9( void ) const;

    /*!
      @brief Access to physical floating-point register f10.
      @return A const reference to f10.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f10( void ) const;

    /*!
      @brief Access to physical floating-point register f11.
      @return A const reference to f11.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f11( void ) const;

    /*!
      @brief Access to physical floating-point register f12.
      @return A const reference to f12.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f12( void ) const;

    /*!
      @brief Access to physical floating-point register f13.
      @return A const reference to f13.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f13( void ) const;

    /*!
      @brief Access to physical floating-point register f14.
      @return A const reference to f14.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f14( void ) const;

    /*!
      @brief Access to physical floating-point register f15.
      @return A const reference to f15.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f15( void ) const;

    /*!
      @brief Access to physical floating-point register f16.
      @return A const reference to f16.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f16( void ) const;

    /*!
      @brief Access to physical floating-point register f17.
      @return A const reference to f17.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f17( void ) const;

    /*!
      @brief Access to physical floating-point register f18.
      @return A const reference to f18.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f18( void ) const;

    /*!
      @brief Access to physical floating-point register f19.
      @return A const reference to f19.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f19( void ) const;

    /*!
      @brief Access to physical floating-point register f20.
      @return A const reference to f20.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f20( void ) const;

    /*!
      @brief Access to physical floating-point register f21.
      @return A const reference to f21.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f21( void ) const;

    /*!
      @brief Access to physical floating-point register f22.
      @return A const reference to f22.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f22( void ) const;

    /*!
      @brief Access to physical floating-point register f23.
      @return A const reference to f23.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f23( void ) const;

    /*!
      @brief Access to physical floating-point register f24.
      @return A const reference to f24.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f24( void ) const;

    /*!
      @brief Access to physical floating-point register f25.
      @return A const reference to f25.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f25( void ) const;

    /*!
      @brief Access to physical floating-point register f26.
      @return A const reference to f26.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f26( void ) const;

    /*!
      @brief Access to physical floating-point register f27.
      @return A const reference to f27.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f27( void ) const;

    /*!
      @brief Access to physical floating-point register f28.
      @return A const reference to f28.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f28( void ) const;

    /*!
      @brief Access to physical floating-point register f29.
      @return A const reference to f29.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f29( void ) const;

    /*!
      @brief Access to physical floating-point register f30.
      @return A const reference to f30.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f30( void ) const;

    /*!
      @brief Access to physical floating-point register f31.
      @return A const reference to f31.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    const RV_FRegP &f31( void ) const;


  private:

    /*!
      @brief clone creates a copy of an RV32IF processor.

      @return A pointer to the newly created RV32IF copy.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_BaseProcessor *clone( void ) const override;

};

}       // namespace WIR

#endif  // _RV32IF_H
