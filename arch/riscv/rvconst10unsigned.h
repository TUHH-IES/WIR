/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2025 - 2026, Heiko Falk.

*/

/*!
  @file rvconst10unsigned.h
  @brief This file provides the interface of unsigned 10 bits-wide immediate
         parameters.

  @author Sasha Tosta <Sasha.Tosta@tuhh.de>
*/

#ifndef _RV_CONST10_UNSIGNED_H
#define _RV_CONST10_UNSIGNED_H


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
  @brief Class RV_Const10_Unsigned is the representation of unsigned const10
         parameters that are non-zero multiples of 4.

  @author Sasha Tosta <Sasha.Tosta@tuhh.de>
*/
class RV_Const10_Unsigned final : public WIR_UnsignedImmediateParameter<RV_Const10_Unsigned>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief No standard construction allowed, users must use
            RV_Const10_Unsigned( unsigned long long ) instead.
    */
    RV_Const10_Unsigned( void ) = delete;

    /*!
      @brief Default constructor for unsigned const10 parameters.

      @param[in] __i The immediate value.

      The constructor ensures that __i lies in the range of values that can be
      represented with 10 bits (unsigned) and is a non-zero multiple of 4.

      @author Sasha Tosta <Sasha.Tosta@tuhh.de>
    */
    explicit RV_Const10_Unsigned( unsigned long long );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Sasha Tosta <Sasha.Tosta@tuhh.de>
    */
    RV_Const10_Unsigned( const RV_Const10_Unsigned & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Sasha Tosta <Sasha.Tosta@tuhh.de>
    */
    RV_Const10_Unsigned( RV_Const10_Unsigned && );

    /*!
      @brief Destructor.
      @author Sasha Tosta <Sasha.Tosta@tuhh.de>
    */
    virtual ~RV_Const10_Unsigned( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Sasha Tosta <Sasha.Tosta@tuhh.de>
    */
    RV_Const10_Unsigned & operator = ( const RV_Const10_Unsigned & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Sasha Tosta <Sasha.Tosta@tuhh.de>
    */
    RV_Const10_Unsigned & operator = ( RV_Const10_Unsigned && );
};

}       // namespace WIR

#endif  // _RV_CONST10_UNSIGNED_H
