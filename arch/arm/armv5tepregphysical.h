/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file armv5tepregphysical.h
  @brief This file provides the interface of physical ARMv5TE register pairs.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _ARMV5TE_PREGPHYSICAL_H
#define _ARMV5TE_PREGPHYSICAL_H


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
  @brief Class ARMv5TE_PRegP is the representation of physical ARMv5TE register
         pairs.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class ARMv5TE_PRegP final : public WIR_PhysicalRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~ARMv5TE_PRegP( void );


  private:

    friend class WIR_BaseProcessor;
    friend class ARMv5TE;

    /*!
      @brief No standard construction allowed, users must use
             ARMv5TE_PRegP( const std::string & ) instead.
    */
    ARMv5TE_PRegP( void ) = delete;

    /*!
      @brief Default constructor for physical register pairs.

      @param[in] __s A const reference to a string that holds the physical
                     register's name (without its specific pre-/suffix as
                     determined by the register's type).
      @param[in] __sp A Boolean denoting whether the physical register is the
                      stack pointer.

      This constructor is private so that folks cannot create and mess around
      with additional physical registers. Construction of physical registers is
      only allowed for class ARMv5TE.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit ARMv5TE_PRegP( const std::string &, bool = false );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARMv5TE_PRegP( const ARMv5TE_PRegP & );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    ARMv5TE_PRegP & operator = ( const ARMv5TE_PRegP & );

    /*!
      @brief Upon creation of ARM physical register pairs, addChilds adds the
             two specified physical registers as childs.

      @param[in] c1 A const reference to the first child.
      @param[in] c2 A const reference to the second child.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void addChilds( const WIR_PhysicalRegister &,
                    const WIR_PhysicalRegister & );

    /*!
      @brief clone creates a copy of a physical ARMv5TE register pair.

      @return A pointer to the newly created physical ARMv5TE register pair.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ARMv5TE_PRegP *clone( void ) const;

};

}       // namespace WIR

#endif  // _ARMV5TE_PREGPHYSICAL_H
