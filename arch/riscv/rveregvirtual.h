/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rveregvirtual.h
  @brief This file provides the interface of virtual RISC-V pairs of registers.

  @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
*/


#ifndef _RV_EREGVIRTUAL_H
#define _RV_EREGVIRTUAL_H


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
  @brief Class RV_ERegV is the representation of pairs of virtual RV32I
         registers.

  @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
*/
class RV_ERegV final : public WIR_VirtualRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for virtual register pairs.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    RV_ERegV( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    RV_ERegV( const RV_ERegV & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    RV_ERegV( RV_ERegV && );

    /*!
      @brief Destructor.
      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    virtual ~RV_ERegV( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    RV_ERegV & operator = ( const RV_ERegV & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be copied.

      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    RV_ERegV & operator = ( RV_ERegV && );


  protected:

    /*!
      @brief clone creates a copy of a virtual RV32I register pair.

      @return A pointer to the newly created virtual RV32I register pair.

      @author Tobias Heinicke <Tobias.Heinicke@tuhh.de>
    */
    virtual RV_ERegV *clone( void ) const;

};

}       // namespace WIR

#endif  // _RV_EREGVIRTUAL_H
