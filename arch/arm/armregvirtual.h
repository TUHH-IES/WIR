/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armregvirtual.h
  @brief This file provides the interface of virtual ARM general-purpose
         registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _ARM_REGVIRTUAL_H
#define _ARM_REGVIRTUAL_H


//
// Include section
//

// Include WIR headers
#include <wir/wirvirtualregister.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class ARM_RegV is the representation of virtual ARM general-purpose
         registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ARM_RegV : public WIR_VirtualRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for virtual general-purpose registers.
    */
    ARM_RegV( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_RegV( const ARM_RegV & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_RegV( ARM_RegV && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~ARM_RegV( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_RegV & operator = ( const ARM_RegV & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_RegV & operator = ( ARM_RegV && );


  protected:

    /*!
      @brief clone creates a copy of a virtual ARM general-purpose register.

      @return A pointer to the newly created virtual ARM register.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ARM_RegV *clone( void ) const;

};

}       // namespace WIR

#endif  // _ARM_REGVIRTUAL_H
