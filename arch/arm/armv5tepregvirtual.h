/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armv5tepregvirtual.h
  @brief This file provides the interface of virtual ARMv5TE register pairs.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _ARMV5TE_PREGVIRTUAL_H
#define _ARMV5TE_PREGVIRTUAL_H


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
  @brief Class ARMv5TE_PRegV is the representation of virtual ARMv5TE register
         pairs.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ARMv5TE_PRegV : public WIR_VirtualRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for virtual register pairs.
    */
    ARMv5TE_PRegV( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARMv5TE_PRegV( const ARMv5TE_PRegV & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARMv5TE_PRegV( ARMv5TE_PRegV && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~ARMv5TE_PRegV( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARMv5TE_PRegV & operator = ( const ARMv5TE_PRegV & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARMv5TE_PRegV & operator = ( ARMv5TE_PRegV && );


  protected:

    /*!
      @brief clone creates a copy of a virtual ARMv5TE register pair.

      @return A pointer to the newly created virtual ARMv5TE register pair.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ARMv5TE_PRegV *clone( void ) const;

};

}       // namespace WIR

#endif  // _ARMV5TE_PREGVIRTUAL_H
