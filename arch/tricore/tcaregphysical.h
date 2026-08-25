/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcaregphysical.h
  @brief This file provides the interface of physical TriCore address registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_AREGPHYSICAL_H
#define _TC_AREGPHYSICAL_H


//
// Include section
//

// Include standard headers
#include <string>

// Include WIR headers
#include <wir/wirphysicalregister.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class TC_ARegP is the representation of physical TriCore address
         registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_ARegP final : public WIR_PhysicalRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_ARegP( void );


  private:

    friend class WIR_BaseProcessor;
    friend class TC13;

    /*!
      @brief No standard construction allowed, users must use
             TC_ARegP( const std::string & ) instead.
    */
    TC_ARegP( void ) = delete;

    /*!
      @brief Default constructor for physical address registers.

      @param[in] __s A const reference to a string that holds the physical
                     register's name (without its specific pre-/suffix as
                     determined by the register's type).
      @param[in] __sp A Boolean denoting whether the physical register is the
                      stack pointer.

      This constructor is private so that folks cannot create and mess around
      with additional physical registers. Construction of physical registers is
      only allowed for class TC13.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit TC_ARegP( const std::string &, bool = false );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_ARegP( const TC_ARegP & );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_ARegP & operator = ( const TC_ARegP & );

    /*!
      @brief clone creates a copy of a physical TriCore address register.

      @return A pointer to the newly created physical TriCore register.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual TC_ARegP *clone( void ) const;

};

}       // namespace WIR

#endif  // _TC_AREGPHYSICAL_H
