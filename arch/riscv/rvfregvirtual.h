/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvfregvirtual.h
  @brief This file provides the interface of virtual RISC-V RV32IF registers.

  @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
*/


#ifndef _RV_FREGVIRTUAL_H
#define _RV_FREGVIRTUAL_H


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
  @brief Class RV_FRegV is the representation of virtual RV32IF registers.

  @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
*/
class RV_FRegV final : public WIR_VirtualRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for virtual registers.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV_FRegV( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV_FRegV( const RV_FRegV & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV_FRegV( RV_FRegV && );

    /*!
      @brief Destructor.
      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    virtual ~RV_FRegV( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV_FRegV & operator = ( const RV_FRegV & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be copied.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    RV_FRegV & operator = ( RV_FRegV && );


  protected:

    /*!
      @brief clone creates a copy of a virtual RV32IF register.

      @return A pointer to the newly created virtual RV32IF register.

      @author Abhirami Meera Sreekumar <Abhirami.Meera.Sreekumar@tuhh.de>
    */
    virtual RV_FRegV *clone( void ) const;

};

}       // namespace WIR

#endif  // _RV_FREGVIRTUAL_H
