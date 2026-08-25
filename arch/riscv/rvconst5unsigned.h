/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvconst5unsigned.h
  @brief This file provides the interface of unsigned 5 bits-wide immediate
         parameters.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
*/


#ifndef _RV_CONST5_UNSIGNED_H
#define _RV_CONST5_UNSIGNED_H


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
  @brief Class RV_Const5_Unsigned is the representation of unsigned const5
         parameters.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
*/
class RV_Const5_Unsigned final : public WIR_UnsignedImmediateParameter<RV_Const5_Unsigned>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief No standard construction allowed, users must use
             RV_Const5_Unsigned( signed long long ) instead.
    */
    RV_Const5_Unsigned( void ) = delete;

    /*!
      @brief Default constructor for unsigned const5 parameters.

      @param[in] __i The immediate value.

      The constructor ensures that __i lies in the range of values that can be
      represented with 5 bits (unsigned).

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    explicit RV_Const5_Unsigned( unsigned long long );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV_Const5_Unsigned( const RV_Const5_Unsigned & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV_Const5_Unsigned( RV_Const5_Unsigned && );

    /*!
      @brief Destructor.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    virtual ~RV_Const5_Unsigned( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV_Const5_Unsigned & operator = ( const RV_Const5_Unsigned & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV_Const5_Unsigned & operator = ( RV_Const5_Unsigned && );

};

}       // namespace WIR

#endif  // _RV_CONST5_UNSIGNED_H
