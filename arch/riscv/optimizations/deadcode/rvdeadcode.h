/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvdeadcode.h
  @brief This file provides the interface of a RISC-V-specific dead code
         elimination.

  @author Yom Ismail Tan <Yom.Tan@tuhh.de>
*/


#ifndef _RV_DEADCODE_H
#define _RV_DEADCODE_H


//
// Include section
//

// Include WIR headers
#include <optimizations/deadcode/wirdeadcode.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_CompilationUnit;
class WIR_Function;
class WIR_System;


/*!
  @brief Class RV_DeadCode is a RISC-V-specific optimization that eliminates
         dead code in %WIR functions.

  @author Yom Ismail Tan <Yom.Tan@tuhh.de>
*/
class RV_DeadCode final : public WIR_DeadCode
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for system-level optimization.

      @param[in] s A reference to a WIR_System to be optimized.

      @author Yom Ismail Tan <Yom.Tan@tuhh.de>
    */
    explicit RV_DeadCode( WIR_System & );

    /*!
      @brief Default constructor for compilation unit-level optimization.

      @param[in] c A reference to a WIR_CompilationUnit to be optimized.

      @author Yom Ismail Tan <Yom.Tan@tuhh.de>
    */
    explicit RV_DeadCode( WIR_CompilationUnit & );

    /*!
      @brief Default constructor for function-level optimization.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Yom Ismail Tan <Yom.Tan@tuhh.de>
    */
    explicit RV_DeadCode( WIR_Function & );

    /*!
      @brief Destructor.
      @author Yom Ismail Tan <Yom.Tan@tuhh.de>
    */
    virtual ~RV_DeadCode( void );


  protected:

    /*!
      @brief runOptimization eliminates dead code in the given function.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Yom Ismail Tan <Yom.Tan@tuhh.de>
    */
    virtual void runOptimization( WIR_Function & ) override;

};

}       // namespace WIR

#endif  // _RV_DEADCODE_H
