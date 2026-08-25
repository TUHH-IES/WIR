/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tc131.h
  @brief This file provides the specific interface of the Infineon TriCore
         V1.3.1 architecture.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC131_H
#define _TC131_H


//
// Include section
//

// Include WIR headers
#include <arch/tricore/tc13.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class TC131 models the Infineon TriCore V1.3.1 instruction set architecture.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC131 : public TC13
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for TC131 processor architectures.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC131( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC131( const TC131 & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC131( TC131 && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC131( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC131 & operator = ( const TC131 & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC131 & operator = ( TC131 && );


    //
    // TC131-specific global initializations.
    //

    /*!
      @brief init performs some global initialization tasks for TC131 processor
             architectures.

      This includes setting up the assignment of valid operation formats to
      TC131 opcodes.

      @note init shall be called globally by WIR_Init(). It shall only perform
            tasks that cannot be expressed as initializations of static class
            members (since the order of static initialization is unspecified in
            C++) and that thus require execution by active code.
    */
    // cppcheck-suppress duplInheritedMember
    static void init( void );


    //
    // Data structures used to model the Infineon TriCore V1.3.1 architecture.
    //

    /*!
      @brief The public members of class OpCode model the TC131's opcodes.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    class OpCode : public TC13::OpCode
    {

      public:

        //! Cache Index, Writeback
        static const OpCode CACHEI_W;

        //! Cache Index, Writeback, Invalidate
        static const OpCode CACHEI_WI;

        //! Float to Integer, Round towards Zero
        static const OpCode FTOIZ;

        //! Float to Fraction, Round towards Zero
        static const OpCode FTOQ31Z;

        //! Float to Unsigned, Round towards Zero
        static const OpCode FTOUZ;


      protected:

        // Inherit the Constructors from TC13::OpCode.
        using TC13::OpCode::OpCode;

    };


  private:

    /*!
      @brief clone creates a copy of a TC131 processor.

      @return A pointer to the newly created TC131 copy.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_BaseProcessor *clone( void ) const override;

};

}       // namespace WIR

#endif  // _TC131_H
