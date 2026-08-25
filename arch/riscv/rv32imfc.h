/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32imfc.h
  @brief This file provides the specific interface of the RISC-V RV32IMFC Base
         Integer instruction set plus the M Standard Extension for Integer
         Multiplication and Division plus the F Standard Extension for
         single-precision Floating-Point Instructions and the C Standard
         Extension for Compressed Instructions, version 2.0.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _RV32IMFC_H
#define _RV32IMFC_H


//
// Include section
//

// Include WIR headers
#include <arch/riscv/rv32ic.h>
#include <arch/riscv/rv32imf.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class RV32IMFC models the RISC-V RC32IMF Base Integer instruction set
         plus the M Standard Extension for Integer Multiplication and Division
         and the F Standard Extension for single-precision Floating-Point
         Instructions, version 2.0.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class RV32IMFC : public RV32IMF,
                 public RV32IC
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for RV32IMFC processor architectures.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFC( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFC( const RV32IMFC & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFC( RV32IMFC && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~RV32IMFC( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFC & operator = ( const RV32IMFC & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFC & operator = ( RV32IMFC && );


    //
    // RV32IMFC-specific global initializations.
    //

    /*!
      @brief init performs some global initialization tasks for RV32IMFC
             processor architectures.

      This includes setting up the assignment of valid operation formats to
      RV32IMFC opcodes.

      @note init shall be called globally by WIR_Init(). It shall only perform
            tasks that cannot be expressed as initializations of static class
            members (since the order of static initialization is unspecified in
            C++) and that thus require execution by active code.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    // cppcheck-suppress duplInheritedMember
    static void init( void );


    //
    // Data structures used to model the RISC-V RV32IMFC V2.0 architecture,
    // inherited from the base classes RV32IMF and RV32IC.
    //

    /*!
      @brief The public members of class OpCode model the RV32IMFC's opcodes.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    class OpCode : public RV32IMF::OpCode,
                   public RV32IC::OpCode
    {

      protected:

        // Inherit the Constructors from RV32IMF::OpCode and RV32IC::OpCode.
        using RV32IMF::OpCode::OpCode;
        using RV32IC::OpCode::OpCode;

    };

    /*!
      @brief The public members of class OperationFormat model the RV32IMFC's
             different formats of machine operations.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    class OperationFormat : public RV32IMF::OperationFormat,
                            public RV32IC::OperationFormat
    {

      protected:

        // Inherit the Constructors from RV32IMF::OperationFormat and
        // RV32IC::OperationFormat.
        using RV32IMF::OperationFormat::OperationFormat;
        using RV32IC::OperationFormat::OperationFormat;

    };


  private:

    /*!
      @brief clone creates a copy of an RV32IMFC processor.

      @return A pointer to the newly created RV32IMFC copy.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_BaseProcessor *clone( void ) const override;

};

}       // namespace WIR

#endif  // _RV32IMFC_H
