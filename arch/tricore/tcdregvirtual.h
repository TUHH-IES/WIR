/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcdregvirtual.h
  @brief This file provides the interface of virtual TriCore data registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_DREGVIRTUAL_H
#define _TC_DREGVIRTUAL_H


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
  @brief Class TC_DRegV is the representation of virtual TriCore data registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_DRegV final : public WIR_VirtualRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for virtual data registers.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_DRegV( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_DRegV( const TC_DRegV & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_DRegV( TC_DRegV && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_DRegV( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_DRegV & operator = ( const TC_DRegV & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_DRegV & operator = ( TC_DRegV && );


  protected:

    /*!
      @brief clone creates a copy of a virtual TriCore data register.

      @return A pointer to the newly created virtual TriCore register.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual TC_DRegV *clone( void ) const;

};

}       // namespace WIR

#endif  // _TC_DREGVIRTUAL_H
