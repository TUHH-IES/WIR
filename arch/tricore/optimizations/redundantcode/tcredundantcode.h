/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcredundantcode.h
  @brief This file provides the interface of a TriCore-specific optimization
         eliminating redundant code that computes bit-wise equivalent results.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_REDUNDANTCODE_H
#define _TC_REDUNDANTCODE_H


//
// Include section
//

// Include WIR headers
#include <wir/wirtypes.h>
#include <optimizations/redundantcode/wirredundantcode.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_CompilationUnit;
class WIR_Function;
class WIR_RegisterParameter;
class WIR_System;
class WIR_UpDownValue;


/*!
  @brief Class TC_RedundantCode is a TriCore-specific optimization that removes
         redundant code that computes bit-wise equivalent results from %WIR
         functions.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_RedundantCode final : public WIR_RedundantCode
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for system-level optimization.

      @param[in] s A reference to a WIR_System to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit TC_RedundantCode( WIR_System & );

    /*!
      @brief Default constructor for compilation unit-level optimization.

      @param[in] c A reference to a WIR_CompilationUnit to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit TC_RedundantCode( WIR_CompilationUnit & );

    /*!
      @brief Default constructor for function-level optimization.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit TC_RedundantCode( WIR_Function & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_RedundantCode( void );


  protected:

    /*!
      @brief runOptimization eliminates redundant code in the given function.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOptimization( WIR_Function & ) override;

    /*!
      @brief areRedundant checks whether the outgoing bit-value of a defined
             register parameter and the incoming bit-value of a used one are
             redundant for some actual processor architecture.

      @param[in] def A const reference to a defined register parameter.
      @param[in] outValue A const reference to the outgoing up-value of the
                          defined register parameter.
      @param[in] use A const reference to a used register parameter.
      @param[in] inValue A const reference to the incoming up-value of the used
                         register parameter.
      @return true iff the involved register parameters and their bit-values are
              suited for redundant code elimination for the TriCore
              architecture, false otherwise.

      For the TriCore architecture, areRedundant returns false if the defined
      register parameter is an implicit parameter of a function call, as a
      subsequent optimization would change the lifeness of this particular
      definition such that it could span other function calls which would render
      the optimization invalid.

      Likewise, if the defined register is used for argument passing for a
      function call, both def and use are also not redundant.

      Furthermore, the obeyance of particular TriCore operation formats that
      require the presence of particular physical registers (i.e., A10, A15 and
      D15) is ensured.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool areRedundant( const WIR_RegisterParameter &,
                               const WIR_UpDownValue &,
                               const WIR_RegisterParameter &,
                               const WIR_UpDownValue & ) const override;

};

}       // namespace WIR

#endif  // _TC_REDUNDANTCODE_H
