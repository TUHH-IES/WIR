/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32ifd.h
  @brief This file provides the specific interface of the RISC-V RV32IFD
         Floating Point instruction set, version 2.0.

  @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
*/


#ifndef _RV32IFD_H
#define _RV32IFD_H


//
// Include section
//

// Include WIR headers
#include <wir/wirprocessor.h>

#include <arch/riscv/rv32if.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class RV32IFD models the RISC-V RV32IFD Floating Point instruction set
         architecture, version 2.0.

  @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
*/
class RV32IFD : virtual public RV32IF
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for RV32IFD processor architectures.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV32IFD( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV32IFD( const RV32IFD & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV32IFD( RV32IFD && );

    /*!
      @brief Destructor.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    virtual ~RV32IFD( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV32IFD & operator = ( const RV32IFD & );

     /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV32IFD & operator = ( RV32IFD && );


    //
    // RV32IFD-specific global initializations.
    //

    /*!
      @brief init performs some global initialization tasks for RV32IFD
             processor architectures.

      This includes setting up the assignment of valid operation formats to
      RV32IFD opcodes.

      @note init shall be called globally by WIR_Init(). It shall only perform
            tasks that cannot be expressed as initializations of static class
            members (since the order of static initialization is unspecified in
            C++) and that thus require execution by active code.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    // cppcheck-suppress duplInheritedMember
    static void init( void );


    //
    // Data structures used to model the RISC-V RV32IFD V2.0 architecture.
    //

    /*!
      @brief The public members of class OpCode model the RV32IFD's opcodes.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    class OpCode : public RV32IF::OpCode
    {
      public:

        //! Floating Point Addition, double precision
        static const OpCode FADD_D;

        //! Floating Point Classify, double precision
        static const OpCode FCLASS_D;

        //! Floating Point Conversion, double to single precision
        static const OpCode FCVT_D_S;

        //! Floating Point Conversion int to float, double precision
        static const OpCode FCVT_D_W;

        //! Floating Point Conversion unsigned int to float, double precision
        static const OpCode FCVT_D_WU;

        //! Floating Point Conversion, single to double precision
        static const OpCode FCVT_S_D;

        //! Floating Point Conversion float to int, double precision
        static const OpCode FCVT_W_D;

        //! Floating Point Conversion float to unsigned int, double precision
        static const OpCode FCVT_WU_D;

        //! Floating Point Division, double precision
        static const OpCode FDIV_D;

        //! Floating Point Equality Comparison, double precision
        static const OpCode FEQ_D;

        //! Floating Point Load Word, double precision
        static const OpCode FLD;

        //! Floating Point Less-Equal Comparison, double precision
        static const OpCode FLE_D;

        //! Floating Point Less-Than Comparison, double precision
        static const OpCode FLT_D;

        //! Floating Point Fused Multiply-Add, double precision
        static const OpCode FMADD_D;

        //! Floating Point Maximum, double precision
        static const OpCode FMAX_D;

        //! Floating Point Minimum, double precision
        static const OpCode FMIN_D;

        //! Floating Point Fused Multiply-Sub, double precision
        static const OpCode FMSUB_D;

        //! Floating Point Multiplication, double precision
        static const OpCode FMUL_D;

        //! Floating Point Move, double precision
        static const OpCode FMV_D;

        //! Floating Point Negated Fused Multiply-Add, double precision
        static const OpCode FNMADD_D;

        //! Floating Point Negated Fused Multiply-Sub, double precision
        static const OpCode FNMSUB_D;

        //! Floating Point Store Word, double precision
        static const OpCode FSD;

        //! Floating Point Sign Injection, double precision
        static const OpCode FSGNJ_D;

        //! Floating Point Negated Sign Injection, double precision
        static const OpCode FSGNJN_D;

        //! Floating Point XORed Sign Injection, double precision
        static const OpCode FSGNJX_D;

        //! Floating Point Square-Root, double precision
        static const OpCode FSQRT_D;

        //! Floating Point Subtraction, double precision
        static const OpCode FSUB_D;


      protected:

        // Inherit the Constructors from RV32IF::OpCode.
        using RV32IF::OpCode::OpCode;

    };


  private:

    /*!
      @brief clone creates a copy of an RV32IFD processor.

      @return A pointer to the newly created RV32IFD copy.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_BaseProcessor *clone( void ) const override;

};

}       // namespace WIR

#endif  // _RV32IFD_H
