/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armconst3coprocessoropcode.h
  @brief This file provides the interface of unsigned 3 bits-wide immediate
         parameters for coprocessor opcodes.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _ARM_CONST3_COPROCESSOROPCODE_H
#define _ARM_CONST3_COPROCESSOROPCODE_H


//
// Include section
//

// Include WIR headers
#include <wir/wirunsignedimmediateparameter.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class ARM_Const3_CoprocessorOpcode is the representation of unsigned
         const3 coprocessor opcodes.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ARM_Const3_CoprocessorOpcode final : public WIR_UnsignedImmediateParameter<ARM_Const3_CoprocessorOpcode>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief No standard construction allowed, users must use
             ARM_Const3_CoprocessorOpcode( unsigned long long ) instead.
    */
    ARM_Const3_CoprocessorOpcode( void ) = delete;

    /*!
      @brief Default constructor for unsigned const3 parameters.

      @param[in] __i The immediate value.

      The constructor ensures that __i lies in the range of values that can be
      represented with 3 bits (unsigned).

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit ARM_Const3_CoprocessorOpcode( unsigned long long );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_Const3_CoprocessorOpcode( const ARM_Const3_CoprocessorOpcode & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_Const3_CoprocessorOpcode( ARM_Const3_CoprocessorOpcode && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~ARM_Const3_CoprocessorOpcode( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_Const3_CoprocessorOpcode & operator = ( const ARM_Const3_CoprocessorOpcode & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_Const3_CoprocessorOpcode & operator = ( ARM_Const3_CoprocessorOpcode && );

};

}       // namespace WIR

#endif  // _ARM_CONST3_COPROCESSOROPCODE_H
