/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvfregphysical.h
  @brief This file provides the interface of physical RISC-V RV32IF registers.

  @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
*/


#ifndef _RV_FREGPHYSICAL_H
#define _RV_FREGPHYSICAL_H


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
  @brief Class RV_FRegP is the representation of physical RV32IF registers.

  @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
*/
class RV_FRegP final : public WIR_PhysicalRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Destructor.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    virtual ~RV_FRegP( void );


  private:

    friend class WIR_BaseProcessor;
    friend class RV32IF;
    friend class RV32IFD;

    /*!
      @brief No standard construction allowed, users must use
             RV_RegP( const std::string & ) instead.
    */
    RV_FRegP( void ) = delete;

    /*!
      @brief Default constructor for physical RV32IF registers.

      @param[in] __s A const reference to a string that holds the physical
                     register's name.
      @param[in] __sp A Boolean denoting whether the physical register is the
                      stack pointer.

      This constructor is private so that folks cannot create and mess around
      with additional physical registers. Construction of physical registers is
      only allowed for class RV32IF.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    explicit RV_FRegP( const std::string &, bool = false );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV_FRegP( const RV_FRegP & );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV_FRegP & operator = ( const RV_FRegP & );

    /*!
      @brief clone creates a copy of a physical RV32IF register.

      @return A pointer to the newly created physical RV32IF register.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    virtual RV_FRegP *clone( void ) const;

};

}       // namespace WIR

#endif  // _RV_FREGPHYSICAL_H
