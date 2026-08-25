/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcaregvirtual.h
  @brief This file provides the interface of virtual TriCore address registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_AREGVIRTUAL_H
#define _TC_AREGVIRTUAL_H


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
  @brief Class TC_ARegV is the representation of virtual TriCore address
         registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_ARegV final : public WIR_VirtualRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for virtual address registers.
    */
    TC_ARegV( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_ARegV( const TC_ARegV & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_ARegV( TC_ARegV && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_ARegV( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_ARegV & operator = ( const TC_ARegV & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_ARegV & operator = ( TC_ARegV && );


  protected:

    /*!
      @brief clone creates a copy of a virtual TriCore address register.

      @return A pointer to the newly created virtual TriCore register.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual TC_ARegV *clone( void ) const;

};

}       // namespace WIR

#endif  // _TC_AREGVIRTUAL_H
