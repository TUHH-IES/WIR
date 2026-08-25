/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirinheritableenum.h
  @brief This file provides an API for inheritable enumeration classes.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIRINHERITABLEENUM_H
#define _WIRINHERITABLEENUM_H


//
// Header section
//

namespace WIR {

/*!
  @brief Class WIR_InheritableEnum quasi models enumeration types that can be
         inherited.

  This class serves as base class from which actual enumerations are derived.
  This kind of enumerations is mostly used to model processor-specific features
  like, e.g., mnemonics or addressing modes.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_InheritableEnum
{

  public:

    /*!
      @brief The == operator checks for equality of enumerators.

      @param[in] __o A const reference to another object to be compared.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    bool operator == ( const WIR_InheritableEnum & ) const;

    /*!
      @brief The != operator checks for inequality of enumerators.

      @param[in] __o A const reference to another object to be compared.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    bool operator != ( const WIR_InheritableEnum & ) const;

    /*!
      @brief The < operator checks for less-than of enumerators.

      @param[in] __o A const reference to another object to be compared.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    bool operator < ( const WIR_InheritableEnum & ) const;


  protected:

    friend class WIR_Registry;

    /*!
      @brief Default constructor assigning a new unique ID to an enumerator.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_InheritableEnum( void );

    //! mID holds an enumerator's unique ID.
    const unsigned int mID;

    //! mMaxValue stores the next free ID.
    static unsigned int mMaxValue;

};

}       // namespace WIR

#endif  // _WIRINHERITABLEENUM_H
