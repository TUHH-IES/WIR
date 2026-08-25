/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armhiregvirtual.h
  @brief This file provides the interface of high virtual ARM registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _ARM_HIREGVIRTUAL_H
#define _ARM_HIREGVIRTUAL_H


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
  @brief Class ARM_HiRegV is the representation of high virtual ARM registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ARM_HiRegV : public WIR_VirtualRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for high virtual registers.
    */
    ARM_HiRegV( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_HiRegV( const ARM_HiRegV & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_HiRegV( ARM_HiRegV && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~ARM_HiRegV( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_HiRegV & operator = ( const ARM_HiRegV & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_HiRegV & operator = ( ARM_HiRegV && );


  protected:

    /*!
      @brief clone creates a copy of a high virtual ARM register.

      @return A pointer to the newly created virtual ARM register.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ARM_HiRegV *clone( void ) const;

};

}       // namespace WIR

#endif  // _ARM_HIREGVIRTUAL_H
