/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armregphysical.h
  @brief This file provides the interface of physical ARM general-purpose
         registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _ARM_REGPHYSICAL_H
#define _ARM_REGPHYSICAL_H


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
  @brief Class ARM_RegP is the representation of physical ARM general-purpose
         registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ARM_RegP final : public WIR_PhysicalRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~ARM_RegP( void );


  private:

    friend class WIR_BaseProcessor;
    friend class ARMv4;

    /*!
      @brief No standard construction allowed, users must use
             ARM_RegP( const std::string & ) instead.
    */
    ARM_RegP( void ) = delete;

    /*!
      @brief Default constructor for physical general-purpose registers.

      @param[in] __s A const reference to a string that holds the physical
                     register's name (without its specific pre-/suffix as
                     determined by the register's type).
      @param[in] __sp A Boolean denoting whether the physical register is the
                      stack pointer.

      This constructor is private so that folks cannot create and mess around
      with additional physical registers. Construction of physical registers is
      only allowed for class ARMv4.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit ARM_RegP( const std::string &, bool = false );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_RegP( const ARM_RegP & );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARM_RegP & operator = ( const ARM_RegP & );

    /*!
      @brief clone creates a copy of a physical ARM general-purpose register.

      @return A pointer to the newly created physical ARM register.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ARM_RegP *clone( void ) const;

};

}       // namespace WIR

#endif  // _ARM_REGPHYSICAL_H
