/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32imf.h
  @brief This file provides the specific interface of the RISC-V RV32IMF Base
         Integer instruction set plus the M Standard Extension for Integer
         Multiplication and Division and the F Standard Extension for
         single-precision Floating-Point Instructions, version 2.0.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _RV32IMF_H
#define _RV32IMF_H


//
// Include section
//

// Include WIR headers
#include <arch/riscv/rv32if.h>
#include <arch/riscv/rv32im.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class RV32IMF models the RISC-V RC32IMF Base Integer instruction set
         plus the M Standard Extension for Integer Multiplication and Division
         and the F Standard Extension for single-precision Floating-Point
         Instructions, version 2.0.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class RV32IMF : public RV32IM,
                public RV32IF
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for RV32IMF processor architectures.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMF( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMF( const RV32IMF & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMF( RV32IMF && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~RV32IMF( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMF & operator = ( const RV32IMF & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMF & operator = ( RV32IMF && );


    //
    // RV32IMF-specific global initializations.
    //

    /*!
      @brief init performs some global initialization tasks for RV32IMF
             processor architectures.

      This includes setting up the assignment of valid operation formats to
      RV32IMF opcodes.

      @note init shall be called globally by WIR_Init(). It shall only perform
            tasks that cannot be expressed as initializations of static class
            members (since the order of static initialization is unspecified in
            C++) and that thus require execution by active code.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
      */
    // cppcheck-suppress duplInheritedMember
    static void init( void );


    //
    // Data structures used to model the RISC-V RV32IMF V2.0 architecture,
    // inherited from the base classes RV32IM and RV32IF.
    //

    /*!
      @brief The public members of class OpCode model the RV32IMF's opcodes.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    class OpCode : public RV32IM::OpCode,
                   public RV32IF::OpCode
    {

      protected:

        // Inherit the Constructors from RV32IM::OpCode and RV32IF::OpCode.
        using RV32IM::OpCode::OpCode;
        using RV32IF::OpCode::OpCode;

    };

    /*!
      @brief The public members of class OperationFormat model the RV32IMF's
             different formats of machine operations.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    class OperationFormat : public RV32IF::OperationFormat
    {

      protected:

        // Inherit the Constructors from RV32IF::OperationFormat.
        using RV32IF::OperationFormat::OperationFormat;

    };


  private:

    /*!
      @brief clone creates a copy of an RV32IMF processor.

      @return A pointer to the newly created RV32IMF copy.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_BaseProcessor *clone( void ) const override;

};

}       // namespace WIR

#endif  // _RV32IMF_H
