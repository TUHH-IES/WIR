/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file mipsimmediate5shamt.h
  @brief This file provides the interface of 5-bit shift amount parameters.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _MIPS_IMMEDIATE5_SHAMT_H
#define _MIPS_IMMEDIATE5_SHAMT_H


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
  @brief Class MIPS_Immediate5_Shamt is the representation of 5-bit shift
         amount parameters.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class MIPS_Immediate5_Shamt final : public WIR_UnsignedImmediateParameter<MIPS_Immediate5_Shamt>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief No standard construction allowed, users must use
             MIPS_Immediate5_Shamt( unsigned long long ) instead.
    */
    MIPS_Immediate5_Shamt( void ) = delete;

    /*!
      @brief Default constructor for 5-bit shift amount parameters.

      @param[in] __i The immediate value.

      The constructor ensures that __i lies in the range of values that can be
      represented with 5 bits (unsigned).

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit MIPS_Immediate5_Shamt( unsigned long long );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    MIPS_Immediate5_Shamt( const MIPS_Immediate5_Shamt & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    MIPS_Immediate5_Shamt( MIPS_Immediate5_Shamt && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~MIPS_Immediate5_Shamt( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    MIPS_Immediate5_Shamt & operator = ( const MIPS_Immediate5_Shamt & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    MIPS_Immediate5_Shamt & operator = ( MIPS_Immediate5_Shamt && );

};

}       // namespace WIR

#endif  // _MIPS_IMMEDIATE5_SHAMT_H
