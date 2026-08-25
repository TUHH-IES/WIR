/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcpregvirtual.h
  @brief This file provides the interface of virtual TriCore extended address
         registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_PREGVIRTUAL_H
#define _TC_PREGVIRTUAL_H


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
  @brief Class TC_PRegV is the representation of virtual TriCore extended
         address registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_PRegV final : public WIR_VirtualRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for virtual extended address registers.
    */
    TC_PRegV( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_PRegV( const TC_PRegV & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_PRegV( TC_PRegV && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_PRegV( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_PRegV & operator = ( const TC_PRegV & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_PRegV & operator = ( TC_PRegV && );


  protected:

    /*!
      @brief clone creates a copy of a virtual TriCore extended address
             register.

      @return A pointer to the newly created virtual TriCore extended register.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual TC_PRegV *clone( void ) const;

};

}       // namespace WIR

#endif  // _TC_PREGVIRTUAL_H
