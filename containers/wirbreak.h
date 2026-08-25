/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirbreak.h
  @brief This file provides the interface of a %WIR container marking branches
         resulting from ANSI-C break statements.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_BREAK_H
#define _WIR_BREAK_H


//
// Include section
//

// Include WIR headers
#include <wir/wircontainer.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class WIR_Break marks branch operations that result from an ANSI-C
         break statement.

  WIR_Break containers are supposed to be attached to WIR_Operation objects
  during code selection and are exploited by %WIR's structural control flow
  analysis.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_Break : public WIR_Container<WIR_Break>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_Break( void );

    /*!
      @brief Destructor.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_Break( void );

    /*!
      @brief isUnique returns whether break containers are unique, i.e., whether
             at most one instance of this container type can be attached to a
             %WIR class.

      @return Always true, break containers are unique.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool isUnique( void ) const override;

};

}       // namespace WIR

#endif  // _WIR_BREAK_H
