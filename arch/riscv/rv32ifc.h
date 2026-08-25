/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32ifc.h
  @brief This file provides the specific interface of the RISC-V RV32IFC Base
         Integer instruction set plus the F Standard Extension for single-
         precision Floating-Point Instructions and the C Standard Extension for
         Compressed Instructions, version 2.0.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _RV32IFC_H
#define _RV32IFC_H


//
// Include section
//

// Include standard headers
#include <functional>
#include <list>

// Include WIR headers
#include <arch/riscv/rv32if.h>
#include <arch/riscv/rv32ic.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_Function;
class WIR_Instruction;


/*!
  @brief Class RV32IFC models the RISC-V RC32IMF Base Integer instruction set
         plus the F Standard Extension for single-precision Floating-Point
         Instructions and the C Standard Extension for Compressed Instructions,
         version 2.0.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class RV32IFC : public RV32IF,
                public RV32IC
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for RV32IFC processor architectures.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IFC( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IFC( const RV32IFC & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IFC( RV32IFC && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~RV32IFC( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IFC & operator = ( const RV32IFC & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    RV32IFC & operator = ( RV32IFC && );


    //
    // RV32IFC-specific global initializations.
    //

    /*!
      @brief init performs some global initialization tasks for RV32IFC
             processor architectures.

      This includes setting up the assignment of valid operation formats to
      RV32IFC opcodes.

      @note init shall be called globally by WIR_Init(). It shall only perform
            tasks that cannot be expressed as initializations of static class
            members (since the order of static initialization is unspecified in
            C++) and that thus require execution by active code.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
      */
    // cppcheck-suppress duplInheritedMember
    static void init( void );


    //
    // Data structures used to model the RISC-V RV32IFC V2.0 architecture,
    // inherited from the base classes RV32IF and RV32IC.
    //

    /*!
      @brief The public members of class OpCode model the RV32IFC's opcodes.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    class OpCode : public RV32IF::OpCode,
                   public RV32IC::OpCode
    {

      protected:

        // Inherit the Constructors from RV32IF::OpCode and RV32IC::OpCode.
        using RV32IF::OpCode::OpCode;
        using RV32IC::OpCode::OpCode;

    };

    /*!
      @brief The public members of class OperationFormat model the RV32IFC's
             different formats of machine operations.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    class OperationFormat : public RV32IF::OperationFormat
    {

      protected:

        // Inherit the Constructors from RV32IF::OperationFormat.
        using RV32IF::OperationFormat::OperationFormat;

    };

    /*!
      @brief adjustStack allocates additional space in the specified function's
             stack frame and adjusts all stack-related memory accesses
             accordingly.

      @param[in,out] f A reference to a %WIR function.
      @param[in] size An integer denoting the number of bytes by which the stack
                      frame is extended.
      @param[in] insertedSpillCode A const reference to a list containing all
                                   spill instructions inside f generated so far.
                                   The instructions in insertedSpillCode will
                                   not be stack-adjusted, since it is presumed
                                   that their stack offsets determined during
                                   register allocation are OK.

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
    static void adjustStack( WIR_Function &, int,
                             const std::list<std::reference_wrapper<WIR_Instruction>> & );


  private:

    /*!
      @brief clone creates a copy of an RV32IFC processor.

      @return A pointer to the newly created RV32IFC copy.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_BaseProcessor *clone( void ) const override;

};

}       // namespace WIR

#endif  // _RV32IFC_H
