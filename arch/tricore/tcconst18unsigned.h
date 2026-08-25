/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcconst18unsigned.h
  @brief This file provides the interface of unsigned 18 bits-wide absolute
         address parameters.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_CONST18_UNSIGNED_H
#define _TC_CONST18_UNSIGNED_H


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
  @brief Class TC_Const18_Unsigned is the representation of 18 bits-wide
         absolute address parameters.

  According to the TriCore ISA, machine operations using absolute addressing
  receive an 18-bit wide immediate operand which is modeled by this class.
  However, the interpretation of these immediate operands is not like a regular
  18-bit unsigned binary number. Instead, the 18 bits of the immediate operand
  are expanded to a full 32 bits-wide address according to the following scheme:

  const18[17:14], "00000000000000", const18[13:0]

  Due to this, this class TC_Const18_Unsigned internally models such absolute
  address parameters with their full bit width, i.e., with full 32 bits. The
  constructor thus accepts values of arbitrary width, including values wider
  than 18 bits, but rejects those values that do not adhere to the above 32 bits
  wide format with 14 0-bits in the middle.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_Const18_Unsigned final : public WIR_UnsignedImmediateParameter<TC_Const18_Unsigned>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief No standard construction allowed, users must use
             TC_Const18_Unsigned( signed long long ) instead.
    */
    TC_Const18_Unsigned( void ) = delete;

    /*!
      @brief Default constructor for 18 bits-wide absolute address parameters.

      @param[in] __i The immediate value.

      The constructor ensures that __i lies in the range of values that are
      valid according to the TriCore ISA.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit TC_Const18_Unsigned( unsigned long long );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const18_Unsigned( const TC_Const18_Unsigned & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const18_Unsigned( TC_Const18_Unsigned && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_Const18_Unsigned( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const18_Unsigned & operator = ( const TC_Const18_Unsigned & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const18_Unsigned & operator = ( TC_Const18_Unsigned && );


    //
    // Value handling.
    //

    /*!
      @brief setValue sets the actual value of a TC_Const18_Unsigned.

      @param[in] i The parameter's new immediate value.

      setValue ensures that __i lies in the range of values that are valid
      according to the TriCore ISA.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void setValue( unsigned long long );

};

}       // namespace WIR

#endif  // _TC_CONST18_UNSIGNED_H
