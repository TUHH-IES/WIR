/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcconst10unsigned.h
  @brief This file provides the interface of unsigned 10 bits-wide immediate
         parameters that are multiples of 4.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_CONST10_UNSIGNED_H
#define _TC_CONST10_UNSIGNED_H


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
  @brief Class TC_Const10_Unsigned is the representation of unsigned const10
         parameters that are multiples of 4.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_Const10_Unsigned final : public WIR_UnsignedImmediateParameter<TC_Const10_Unsigned>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief No standard construction allowed, users must use
             TC_Const10_Unsigned( unsigned long long ) instead.
    */
    TC_Const10_Unsigned( void ) = delete;

    /*!
      @brief Default constructor for unsigned const10 parameters that are
             multiples of 4.

      @param[in] __i The immediate value.

      The constructor ensures that __i lies in the range of values that can be
      represented with 10 bits (unsigned) and that the given value is a multiple
      of 4.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit TC_Const10_Unsigned( unsigned long long );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const10_Unsigned( const TC_Const10_Unsigned & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const10_Unsigned( TC_Const10_Unsigned && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_Const10_Unsigned( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const10_Unsigned & operator = ( const TC_Const10_Unsigned & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_Const10_Unsigned & operator = ( TC_Const10_Unsigned && );


    //
    // Value handling.
    //

    /*!
      @brief setValue sets the actual value of a TC_Const10_Unsigned.

      @param[in] i The parameter's new immediate value.

      setValue ensures that __i lies in the range of values that are valid
      according to the TriCore ISA.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void setValue( unsigned long long );

};

}       // namespace WIR

#endif  // _TC_CONST10_UNSIGNED_H
