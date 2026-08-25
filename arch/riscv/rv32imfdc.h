/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32imfdc.h
  @brief This file provides the specific interface of the RISC-V RV32IMFDC Base
         Integer instruction set plus the M Standard Extension for Integer
         Multiplication and Division plus the F and D Standard Extensions for
         single- and double-precision Floating-Point Instructions and the C
         Standard Extension for Compressed Extension for Compressed
         Instructions, version 2.0.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _RV32IMFDC_H
#define _RV32IMFDC_H


//
// Include section
//

// Include WIR headers
#include <arch/riscv/rv32ic.h>
#include <arch/riscv/rv32im.h>
#include <arch/riscv/rv32ifd.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class RV32IMFDC models the RISC-V RV32IMFDC Base Integer instruction
         set plus the M Standard Extension for Integer Multiplication and
         Division plus the F and D Standard Extensions for single- and double-
         precision Floating-Point Instructions and the C Standard Extension for
         Compressed Extension for Compressed Instructions, version 2.0.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class RV32IMFDC : public RV32IM,
                  public RV32IFD,
                  public RV32IC
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for RV32IMFDC processor architectures.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFDC( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFDC( const RV32IMFDC & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFDC( RV32IMFDC && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~RV32IMFDC( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFDC & operator = ( const RV32IMFDC & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFDC & operator = ( RV32IMFDC && );


    //
    // RV32IMFDC-specific global initializations.
    //

    /*!
      @brief init performs some global initialization tasks for RV32IMFDC
             processor architectures.

      This includes setting up the assignment of valid operation formats to
      RV32IMFDC opcodes.

      @note init shall be called globally by WIR_Init(). It shall only perform
            tasks that cannot be expressed as initializations of static class
            members (since the order of static initialization is unspecified in
            C++) and that thus require execution by active code.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    // cppcheck-suppress duplInheritedMember
    static void init( void );


    //
    // Data structures used to model the RISC-V RV32IMFDC V2.0 architecture,
    // inherited from the base classes RV32IM, RV32IFD and RV32IC.
    //

    /*!
      @brief The public members of class OpCode model the RV32IMFDC's opcodes.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    class OpCode : public RV32IM::OpCode,
                   public RV32IFD::OpCode,
                   public RV32IC::OpCode
    {

      protected:

        // Inherit the Constructors from RV32IM::OpCode, RV32IFD::OpCode and
        // RV32IC::OpCode.
        using RV32IM::OpCode::OpCode;
        using RV32IFD::OpCode::OpCode;
        using RV32IC::OpCode::OpCode;

    };

    /*!
      @brief The public members of class OperationFormat model the RV32IMFDC's
             different formats of machine operations.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    class OperationFormat : public RV32IFD::OperationFormat,
                            public RV32IC::OperationFormat
    {

      protected:

        // Inherit the Constructors from RV32IFD::OperationFormat.
        using RV32IFD::OperationFormat::OperationFormat;

    };


  private:

    /*!
      @brief clone creates a copy of an RV32IMFDC processor.

      @return A pointer to the newly created RV32IMFDC copy.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_BaseProcessor *clone( void ) const override;

};

}       // namespace WIR

#endif  // _RV32IMFDC_H
