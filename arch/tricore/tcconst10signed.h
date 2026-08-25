/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcconst10signed.h
  @brief This file provides the interface of signed 10 bits-wide immediate
         parameters.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_CONST10_SIGNED_H
#define _TC_CONST10_SIGNED_H


//
// Include section
//

// Include WIR headers
#include <wir/wirsignedimmediateparameter.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class TC_Const10_Signed is the representation of signed const10
         parameters.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_Const10_Signed final : public WIR_SignedImmediateParameter<TC_Const10_Signed>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief No standard construction allowed, users must use
             TC_Const10_Signed( signed long long ) instead.
    */
    TC_Const10_Signed( void ) = delete;

    /*!
      @brief Default constructor for signed const10 parameters.

      @param[in] __i The immediate value.

      The constructor ensures that __i lies in the range of values that can be
      represented with 10 bits, assuming two's-complement as underlying data
      format.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit TC_Const10_Signed( signed long long );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const10_Signed( const TC_Const10_Signed & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const10_Signed( TC_Const10_Signed && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_Const10_Signed( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const10_Signed & operator = ( const TC_Const10_Signed & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const10_Signed& operator = ( TC_Const10_Signed && );

};

}       // namespace WIR

#endif  // _TC_CONST10_SIGNED_H
