/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tceregvirtual.h
  @brief This file provides the interface of virtual TriCore extended data
         registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_EREGVIRTUAL_H
#define _TC_EREGVIRTUAL_H


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
  @brief Class TC_ERegV is the representation of virtual TriCore extended
         data registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_ERegV final : public WIR_VirtualRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for virtual extended data registers.
    */
    TC_ERegV( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_ERegV( const TC_ERegV & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_ERegV( TC_ERegV && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_ERegV( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_ERegV & operator = ( const TC_ERegV & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_ERegV & operator = ( TC_ERegV && );


  protected:

    /*!
      @brief clone creates a copy of a virtual TriCore extended data register.

      @return A pointer to the newly created virtual TriCore extended register.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual TC_ERegV *clone( void ) const;

};

}       // namespace WIR

#endif  // _TC_EREGVIRTUAL_H
