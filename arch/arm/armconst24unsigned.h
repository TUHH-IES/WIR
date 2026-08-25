/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armconst24unsigned.h
  @brief This file provides the interface of unsigned 24 bits-wide immediate
         parameters.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _ARM_CONST24_UNSIGNED_H
#define _ARM_CONST24_UNSIGNED_H


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
  @brief Class ARM_Const24_Unsigned is the representation of unsigned const24
         parameters.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ARM_Const24_Unsigned final : public WIR_UnsignedImmediateParameter<ARM_Const24_Unsigned>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief No standard construction allowed, users must use
             ARM_Const24_Unsigned( unsigned long long ) instead.
    */
    ARM_Const24_Unsigned( void ) = delete;

    /*!
      @brief Default constructor for unsigned const24 parameters.

      @param[in] __i The immediate value.

      The constructor ensures that __i lies in the range of values that can be
      represented with 24 bits (unsigned).

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit ARM_Const24_Unsigned( unsigned long long );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_Const24_Unsigned( const ARM_Const24_Unsigned & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_Const24_Unsigned( ARM_Const24_Unsigned && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~ARM_Const24_Unsigned( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_Const24_Unsigned & operator = ( const ARM_Const24_Unsigned & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_Const24_Unsigned & operator = ( ARM_Const24_Unsigned && );

};

}       // namespace WIR

#endif  // _ARM_CONST24_UNSIGNED_H
