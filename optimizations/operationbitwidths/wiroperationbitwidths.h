/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2025 - 2026, Heiko Falk.

*/

/*!
  @file wiroperationbitwidths.h
  @brief This file provides the interface of a base class for optimizations
         reducing the bit widths of machine operations.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_OPERATIONBITWIDTHS_H
#define _WIR_OPERATIONBITWIDTHS_H


//
// Include section
//

// Include standard headers
#include <functional>
#include <iterator>
#include <list>

// Include WIR headers
#include <optimizations/generic/wiroptimization.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_CompilationUnit;
class WIR_Function;
class WIR_Instruction;
class WIR_Operation;
class WIR_System;


/*!
  @brief Class WIR_OperationBitWidths is a generic optimization that reduces the
         bit widths of machine operations.
*/
class WIR_OperationBitWidths : public WIR_Optimization
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
    explicit WIR_OperationBitWidths( WIR_System & );

    /*!
      @brief Default constructor for compilation unit-level optimization.

      @param[in] c A reference to a WIR_CompilationUnit to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit WIR_OperationBitWidths( WIR_CompilationUnit & );

    /*!
      @brief Default constructor for function-level optimization.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit WIR_OperationBitWidths( WIR_Function & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_OperationBitWidths( void );


  protected:

    /*!
      @brief runOptimization reduces the bit widths of machine operations in the
             given system.

      @param[in] s A reference to a WIR_System to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOptimization( WIR_System & ) override;

    /*!
      @brief runOptimization reduces the bit widths of machine operations in the
             given compilation unit.

      @param[in] c A reference to a WIR_CompilationUnit to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOptimization( WIR_CompilationUnit & ) override;

    /*!
      @brief runOptimization reduces the bit widths of machine operations in the
             given function.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOptimization( WIR_Function & ) override;

    /*!
      @brief reduce reduces the bit width of a given machine operation.

      @param[in,out] i A reference to a %WIR instruction to be optimized.
      @param[in] pos An iterator pointing to a %WIR operation inside i to be
                     optimized.
      @return An iterator pointing to the newly created operation inside the
              given instruction, or the end() iterator if no reduction was done.

      Since the actual task of reducing an operation's bit width is processor-
      specific, this method is purely virtual.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual std::list<
              std::reference_wrapper<
                WIR_Operation>>::iterator reduce( WIR_Instruction &i,
                                                  std::list<
                                                    std::reference_wrapper<
                                                      WIR_Operation>>::const_iterator pos ) const = 0;

    /*!
      @brief replace replaces the first operation by the second.

      @param[in] pos An iterator pointing to a %WIR operation inside i to be
                     optimized.
      @param[in] n An R-value reference to a %WIR operation being the
                   replacement.
      @return An iterator to the newly inserted operation.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    std::list<
      std::reference_wrapper<
        WIR_Operation>>::iterator replace( std::list<
                                             std::reference_wrapper<
                                               WIR_Operation>>::const_iterator,
                                           WIR_Operation && ) const;

};

}       // namespace WIR

#endif  // _WIR_OPERATIONBITWIDTHS_H
