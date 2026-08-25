/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armloregvirtual.h
  @brief This file provides the interface of low virtual ARM registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _ARM_LOREGVIRTUAL_H
#define _ARM_LOREGVIRTUAL_H


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
  @brief Class ARM_LoRegV is the representation of low virtual ARM registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ARM_LoRegV : public WIR_VirtualRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for low virtual registers.
    */
    ARM_LoRegV( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_LoRegV( const ARM_LoRegV & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_LoRegV( ARM_LoRegV && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~ARM_LoRegV( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_LoRegV & operator = ( const ARM_LoRegV & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_LoRegV & operator = ( ARM_LoRegV && );


  protected:

    /*!
      @brief clone creates a copy of a low virtual ARM register.

      @return A pointer to the newly created virtual ARM register.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ARM_LoRegV *clone( void ) const;

};

}       // namespace WIR

#endif  // _ARM_LOREGVIRTUAL_H
