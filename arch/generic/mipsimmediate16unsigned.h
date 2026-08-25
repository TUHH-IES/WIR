/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file mipsimmediate16unsigned.h
  @brief This file provides the interface of unsigned 16-bit immediate
         parameters.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _MIPS_IMMEDIATE16_UNSIGNED_H
#define _MIPS_IMMEDIATE16_UNSIGNED_H


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
  @brief Class MIPS_Immediate16_Unsigned is the representation of unsigned
         16-bit immediate parameters.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class MIPS_Immediate16_Unsigned final : public WIR_UnsignedImmediateParameter<MIPS_Immediate16_Unsigned>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief No standard construction allowed, users must use
             MIPS_Immediate16_Unsigned( unsigned long long ) instead.
    */
    MIPS_Immediate16_Unsigned( void ) = delete;

    /*!
      @brief Default constructor for unsigned 16-bit immediate parameters.

      @param[in] __i The immediate value.

      The constructor ensures that __i lies in the range of values that can be
      represented with 16 bits (unsigned).

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit MIPS_Immediate16_Unsigned( unsigned long long );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    MIPS_Immediate16_Unsigned( const MIPS_Immediate16_Unsigned & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    MIPS_Immediate16_Unsigned( MIPS_Immediate16_Unsigned && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~MIPS_Immediate16_Unsigned( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    MIPS_Immediate16_Unsigned & operator = ( const MIPS_Immediate16_Unsigned & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    MIPS_Immediate16_Unsigned & operator = ( MIPS_Immediate16_Unsigned && );

};

}       // namespace WIR

#endif  // _MIPS_IMMEDIATE16_UNSIGNED_H
