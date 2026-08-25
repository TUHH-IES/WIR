/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcconst3unsigned.h
  @brief This file provides the interface of unsigned 3 bits-wide immediate
         parameters.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_CONST3_UNSIGNED_H
#define _TC_CONST3_UNSIGNED_H


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
  @brief Class TC_Const3_Unsigned is the representation of unsigned const3
         parameters.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_Const3_Unsigned final : public WIR_UnsignedImmediateParameter<TC_Const3_Unsigned>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief No standard construction allowed, users must use
             TC_Const3_Unsigned( signed long long ) instead.
    */
    TC_Const3_Unsigned( void ) = delete;

    /*!
      @brief Default constructor for unsigned const3 parameters.

      @param[in] __i The immediate value.

      The constructor ensures that __i lies in the range of values that can be
      represented with 3 bits (unsigned).

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit TC_Const3_Unsigned( unsigned long long );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const3_Unsigned( const TC_Const3_Unsigned & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const3_Unsigned( TC_Const3_Unsigned && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_Const3_Unsigned( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const3_Unsigned & operator = ( const TC_Const3_Unsigned & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const3_Unsigned & operator = ( TC_Const3_Unsigned && );

};

}       // namespace WIR

#endif  // _TC_CONST3_UNSIGNED_H
