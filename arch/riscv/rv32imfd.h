/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32imfd.h
  @brief This file provides the specific interface of the RISC-V RV32IMFD Base
         Integer instruction set plus the M Standard Extension for Integer
         Multiplication and Division plus the F and D Standard Extensions for
         single- and double-precision Floating-Point Instructions, version 2.0.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _RV32IMFD_H
#define _RV32IMFD_H


//
// Include section
//

// Include WIR headers
#include <arch/riscv/rv32im.h>
#include <arch/riscv/rv32ifd.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class RV32IMFD models the RISC-V RC32IMF Base Integer instruction set
         plus the M Standard Extension for Integer Multiplication and Division
         and the F Standard Extension for single-precision Floating-Point
         Instructions, version 2.0.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class RV32IMFD : public RV32IM,
                 public RV32IFD
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for RV32IMFD processor architectures.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFD( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFD( const RV32IMFD & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFD( RV32IMFD && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~RV32IMFD( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFD & operator = ( const RV32IMFD & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IMFD & operator = ( RV32IMFD && );


    //
    // RV32IMFD-specific global initializations.
    //

    /*!
      @brief init performs some global initialization tasks for RV32IMFD
             processor architectures.

      This includes setting up the assignment of valid operation formats to
      RV32IMFD opcodes.

      @note init shall be called globally by WIR_Init(). It shall only perform
            tasks that cannot be expressed as initializations of static class
            members (since the order of static initialization is unspecified in
            C++) and that thus require execution by active code.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
      */
    // cppcheck-suppress duplInheritedMember
    static void init( void );


    //
    // Data structures used to model the RISC-V RV32IMFD V2.0 architecture,
    // inherited from the base classes RV32IM and RV32IFD.
    //

    /*!
      @brief The public members of class OpCode model the RV32IMFD's opcodes.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    class OpCode : public RV32IM::OpCode,
                   public RV32IFD::OpCode
    {

      protected:

        // Inherit the Constructors from RV32IM::OpCode and RV32IFD::OpCode.
        using RV32IM::OpCode::OpCode;
        using RV32IFD::OpCode::OpCode;

    };

    /*!
      @brief The public members of class OperationFormat model the RV32IMFD's
             different formats of machine operations.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    class OperationFormat : public RV32IFD::OperationFormat
    {

      protected:

        // Inherit the Constructors from RV32IFD::OperationFormat.
        using RV32IFD::OperationFormat::OperationFormat;

    };


  private:

    /*!
      @brief clone creates a copy of an RV32IMFD processor.

      @return A pointer to the newly created RV32IMFD copy.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_BaseProcessor *clone( void ) const override;

};

}       // namespace WIR

#endif  // _RV32IMFD_H
