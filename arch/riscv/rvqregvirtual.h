/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2025 - 2026, Heiko Falk.

*/

/*!
  @file rvqregvirtual.h
  @brief This file provides the interface of virtual RISC-V quadruples of
         registers.

  @author Lennard Boysen <Lennard.Boysen@tuhh.de>
*/


#ifndef _RV_QREGVIRTUAL_H
#define _RV_QREGVIRTUAL_H


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
  @brief Class RV_QRegV is the representation of quadruples of virtual RV32I
         registers.

  @author Lennard Boysen <Lennard.Boysen@tuhh.de>
*/
class RV_QRegV final : public WIR_VirtualRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for virtual register quadruples.
      @author Lennard Boysen <Lennard.Boysen@tuhh.de>
    */
    RV_QRegV( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Lennard Boysen <Lennard.Boysen@tuhh.de>
    */
    RV_QRegV( const RV_QRegV & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Lennard Boysen <Lennard.Boysen@tuhh.de>
    */
    RV_QRegV( RV_QRegV && );

    /*!
      @brief Destructor.
      @author Lennard Boysen <Lennard.Boysen@tuhh.de>
    */
    virtual ~RV_QRegV( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Lennard Boysen <Lennard.Boysen@tuhh.de>
    */
    RV_QRegV & operator = ( const RV_QRegV & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be copied.

      @author Lennard Boysen <Lennard.Boysen@tuhh.de>
    */
    RV_QRegV & operator = ( RV_QRegV && );


  protected:

    /*!
      @brief clone creates a copy of a virtual RV32I register quadruple.

      @return A pointer to the newly created virtual RV32I register quadruple.

      @author Lennard Boysen <Lennard.Boysen@tuhh.de>
    */
    virtual RV_QRegV *clone( void ) const;

};

}       // namespace WIR

#endif  // _RV_QREGVIRTUAL_H
