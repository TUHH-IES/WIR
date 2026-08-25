/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvconst6signed.h
  @brief This file provides the interface of signed 6 bits-wide immediate
         parameters.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
*/


#ifndef _RV_CONST6_SIGNED_H
#define _RV_CONST6_SIGNED_H


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
  @brief Class RV_Const6_Signed is the representation of signed const6
         parameters.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
*/
class RV_Const6_Signed final : public WIR_SignedImmediateParameter<RV_Const6_Signed>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief No standard construction allowed, users must use
             RV_Const6_Signed( signed long long ) instead.
    */
    RV_Const6_Signed( void ) = delete;

    /*!
      @brief Default constructor for signed const6 parameters.

      @param[in] __i The immediate value.

      The constructor ensures that __i lies in the range of values that can be
      represented with 6 bits, assuming two's-complement as underlying data
      format.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    explicit RV_Const6_Signed( signed long long );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV_Const6_Signed( const RV_Const6_Signed & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV_Const6_Signed( RV_Const6_Signed && );

    /*!
      @brief Destructor.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    virtual ~RV_Const6_Signed( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV_Const6_Signed & operator = ( const RV_Const6_Signed & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV_Const6_Signed & operator = ( RV_Const6_Signed && );

};

}       // namespace WIR

#endif  // _RV_CONST6_SIGNED_H
