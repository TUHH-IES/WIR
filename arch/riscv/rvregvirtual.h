/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvregvirtual.h
  @brief This file provides the interface of virtual RISC-V RV32I registers.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
*/


#ifndef _RV_REGVIRTUAL_H
#define _RV_REGVIRTUAL_H


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
  @brief Class RV_RegV is the representation of virtual RV32I registers.

  @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
*/
class RV_RegV final : public WIR_VirtualRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for virtual registers.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV_RegV( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV_RegV( const RV_RegV & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV_RegV( RV_RegV && );

    /*!
      @brief Destructor.
      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    virtual ~RV_RegV( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV_RegV & operator = ( const RV_RegV & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be copied.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    RV_RegV & operator = ( RV_RegV && );


  protected:

    /*!
      @brief clone creates a copy of a virtual RV32I register.

      @return A pointer to the newly created virtual RV32I register.

      @author Jonas Oltmanns <Jonas.Oltmanns@tuhh.de>
    */
    virtual RV_RegV *clone( void ) const;

};

}       // namespace WIR

#endif  // _RV_REGVIRTUAL_H
