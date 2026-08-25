/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file mipsregvirtual.h
  @brief This file provides the interface of virtual MIPS integer registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _MIPS_REGVIRTUAL_H
#define _MIPS_REGVIRTUAL_H


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
  @brief Class MIPS_RegV is the representation of virtual MIPS integer
         registers.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class MIPS_RegV : public WIR_VirtualRegister
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for virtual integer registers.
    */
    MIPS_RegV( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    MIPS_RegV( const MIPS_RegV & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    MIPS_RegV( MIPS_RegV && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~MIPS_RegV( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    MIPS_RegV & operator = ( const MIPS_RegV & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    MIPS_RegV & operator = ( MIPS_RegV && );


  protected:

    /*!
      @brief clone creates a copy of a virtual MIPS integer register.

      @return A pointer to the newly created virtual MIPS register.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual MIPS_RegV *clone( void ) const;

};

}       // namespace WIR

#endif  // _MIPS_REGVIRTUAL_H
