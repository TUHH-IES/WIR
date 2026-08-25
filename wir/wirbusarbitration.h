/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirbusarbitration.h
  @brief This file provides the basic interface of generic %WIR bus arbitration
         policies.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_BUS_ARBITRATION_H
#define _WIR_BUS_ARBITRATION_H


//
// Header section
//

namespace WIR {

/*!
  @brief Class WIR_BusArbitration is the generic representation of bus
         arbitration policies.

  This class serves as virtual base class from which actual arbitration policies
  (e.g., fixed priority, TDMA, ...) are derived.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_BusArbitration
{

  public:

    //
    // Destructors.
    //

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_BusArbitration( void );


    //
    // Comparison operators.
    //

    /*!
      @brief This operator compares two WIR_BusArbitration for equality.

      @param[in] __o A const reference to another object to be compared.
      @return true iff both operands are equal, false otherwise.

      Since this class serves as abstract base class only, this operator is a
      pure virtual method.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool operator == ( const WIR_BusArbitration &__o ) const = 0;

    /*!
      @brief This operator compares two WIR_BusArbitration for inequality.

      @param[in] __o A const reference to another object to be compared.
      @return true iff both operands are inequal, false otherwise.

      Since this class serves as abstract base class only, this operator is a
      pure virtual method.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool operator != ( const WIR_BusArbitration &__o ) const = 0;

    /*!
      @brief clone creates a copy of a %WIR bus arbiter.

      @return A pointer to the newly created copy of this arbiter.

      Since the implementation details depend on some actual arbiter's
      characteristics, clone is a pure virtual method.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_BusArbitration *clone( void ) const = 0;

};

}       // namespace WIR

#endif  // _WIR_BUS_ARBITRATION_H
