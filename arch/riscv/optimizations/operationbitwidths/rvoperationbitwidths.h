/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2025 - 2026, Heiko Falk.

*/

/*!
  @file rvoperationbitwidths.h
  @brief This file provides the interface of a RISC-V-specific optimization
         reducing the bit widths of machine operations.

  @author Sasha Tosta <Sasha.Tosta@tuhh.de>
*/


#ifndef _RV_OPERATIONBITWIDTHS_H
#define _RV_OPERATIONBITWIDTHS_H


//
// Include section
//

// Include standard headers
#include <functional>
#include <list>

// Include WIR headers
#include <optimizations/operationbitwidths/wiroperationbitwidths.h>


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
  @brief Class RV_OperationBitWidths is a RISC-V-specific optimization that
         reduces the bit widths of machine operations.
*/
class RV_OperationBitWidths final : public WIR_OperationBitWidths
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
    explicit RV_OperationBitWidths( WIR_System & );

    /*!
      @brief Default constructor for compilation unit-level optimization.

      @param[in] c A reference to a WIR_CompilationUnit to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit RV_OperationBitWidths( WIR_CompilationUnit & );

    /*!
      @brief Default constructor for function-level optimization.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit RV_OperationBitWidths( WIR_Function & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~RV_OperationBitWidths( void );


  protected:

    /*!
      @brief reduce reduces the bit width of a given RISC-V machine operation.

      @param[in,out] i A reference to a %WIR instruction to be optimized.
      @param[in] pos An iterator pointing to a %WIR operation inside i to be
                     optimized.
      @return An iterator pointing to the newly created operation inside the
              given instruction, or the end() iterator if no reduction was done.

      Bit widths of branches are reduced whenever possible and without proper
      consideration of jump displacements. The correction of branches with
      incorrect displacements thus has to be done elsewhere afterwards.

      @author Sasha Tosta <Sasha.Tosta@tuhh.de>
    */
    virtual std::list<
              std::reference_wrapper<
                WIR_Operation>>::iterator reduce( WIR_Instruction &,
                                                  std::list<
                                                    std::reference_wrapper<
                                                      WIR_Operation>>::const_iterator ) const override;

};

}       // namespace WIR

#endif  // _RV_OPERATIONBITWIDTHS_H
