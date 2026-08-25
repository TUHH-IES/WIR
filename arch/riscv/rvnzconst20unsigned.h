/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2026, Heiko Falk.

*/

/*!
  @file rvnzconst20unsigned.h
  @brief This file provides the interface of unsigned, non-zero 20 bits-wide
         immediate parameters from the subintervals [0x1, 0x1f] and
         [0xfffe0, 0xfffff].

  @author Heiko Falk<Heiko.Falk.de>
*/


#ifndef _RV_NZCONST20_UNSIGNED_H
#define _RV_NZCONST20_UNSIGNED_H


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
  @brief Class RV_NZConst20_Unsigned is the representation of unsigned, non-zero
         const20 parameters from the subintervals [0x1, 0x1f] and
         [0xfffe0, 0xfffff].

  See also:
  https://github.com/riscv-non-isa/riscv-asm-manual/pull/66/commits/6196c03643af1ea2dff103b58565e38776b0c34d

  @author Heiko Falk <Heiko.Falk.de>
*/
class RV_NZConst20_Unsigned final : public WIR_UnsignedImmediateParameter<RV_NZConst20_Unsigned>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief No standard construction allowed, users must use
             RV_NZConst20_Unsigned( unsigned long long ) instead.
    */
    RV_NZConst20_Unsigned( void ) = delete;

    /*!
      @brief Default constructor for unsigned non-zero const20 parameters.

      @param[in] __i The immediate value.

      The constructor ensures that __i lies in the range of values that can be
      represented with 20 bits (unsigned) from the subintervals [0x1, 0x1f] and
      [0xfffe0, 0xfffff].

      See also:
      https://github.com/riscv-non-isa/riscv-asm-manual/pull/66/commits/6196c03643af1ea2dff103b58565e38776b0c34d

      @author Heiko Falk <Heiko.Falk.de>
    */
    explicit RV_NZConst20_Unsigned( unsigned long long );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk.de>
    */
    RV_NZConst20_Unsigned( const RV_NZConst20_Unsigned & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk.de>
    */
    RV_NZConst20_Unsigned( RV_NZConst20_Unsigned && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk.de>
    */
    virtual ~RV_NZConst20_Unsigned( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk.de>
    */
    RV_NZConst20_Unsigned & operator = ( const RV_NZConst20_Unsigned & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk.de>
    */
    RV_NZConst20_Unsigned & operator = ( RV_NZConst20_Unsigned && );

};

}       // namespace WIR

#endif  // _RV_NZCONST6_UNSIGNED_H
