/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rveregphysical.h
  @brief This file provides the interface of physical RISC-V pairs of registers.

  @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
*/


#ifndef _RV_EREGPHYSICAL_H
#define _RV_EREGPHYSICAL_H


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
  @brief Class RV_ERegP is the representation of pairs of physical RV32I
         registers.

  @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
*/
class RV_ERegP final : public WIR_PhysicalRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Destructor.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    virtual ~RV_ERegP( void );


  private:

    friend class WIR_BaseProcessor;
    friend class RV32I;

    /*!
      @brief No standard construction allowed, users must use
             RV_ERegP( const std::string & ) instead.
    */
    RV_ERegP( void ) = delete;

    /*!
      @brief Default constructor for physical RV32I register pairs.

      @param[in] __s A const reference to a string that holds the physical
                     register's name which is to be derived by the actual
                     physical registers that make up the pair. The pairs should
                     consist of even-odd pairs of actual registers; example:
                     the argument registers may yield the following pairs:
                     p10 -> (x10, x11), p12 -> (x12, x13), p14 -> (x14, x15) &
                     p16 -> (x16, x17).
      @param[in] __sp A Boolean denoting whether the physical register is the
                      stack pointer.

      @note Second input parameter (denoting if reg is stack pointer) is kept
            for compatibility, it is however asserted that this parameter is
            left at it's default value false, because denoting a register pair
            as stack pointer does not make sense.

      This constructor is private so that folks cannot create and mess around
      with additional physical registers. Construction of physical registers is
      only allowed for class RV32I.

      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    explicit RV_ERegP( const std::string &, bool = false );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    RV_ERegP( const RV_ERegP & );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    RV_ERegP & operator = ( const RV_ERegP & );

    /*!
      @brief Upon creation of RISC-V physical register pairs, addChilds adds
             the two specified physical registers as childs.

      @param[in] c1 A const reference to the first child.
      @param[in] c2 A const reference to the second child.

      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    void addChilds( const WIR_PhysicalRegister &,
                    const WIR_PhysicalRegister & );

    /*!
      @brief clone creates a copy of a physical RV32I register pair.

      @return A pointer to the newly created physical RV32I register pair.

      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    virtual RV_ERegP *clone( void ) const;

};

}       // namespace WIR

#endif  // _RV_EREGPHYSICAL_H
