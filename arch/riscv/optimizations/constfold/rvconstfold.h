/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rvconstfold.h
  @brief This file provides the interface of a RISC-V-specific constant folding
         optimization.

  @author Yom Ismail Tan <Yom.Tan@tuhh.de>
*/


#ifndef _RV_CONSTFOLD_H
#define _RV_CONSTFOLD_H


//
// Include section
//

// Include standard headers
#include <list>
#include <map>

// Include WIR headers
#include <wir/wirtypes.h>
#include <optimizations/constfold/wirconstfold.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_BaseRegister;
class WIR_CompilationUnit;
class WIR_Function;
class WIR_Instruction;
class WIR_LabelParameter;
class WIR_Operation;
class WIR_Parameter;
class WIR_RegisterParameter;
class WIR_System;
class WIR_UpDownValue;


/*!
  @brief Class RV_ConstFold is a RISC-V-specific optimization that folds
         constants in %WIR functions.

  @author Yom Ismail Tan <Yom.Tan@tuhh.de>
*/
class RV_ConstFold final : public WIR_ConstFold
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
    explicit RV_ConstFold( WIR_System & );

    /*!
      @brief Default constructor for compilation unit-level optimization.

      @param[in] c A reference to a WIR_CompilationUnit to be optimized.

      @author Yom Ismail Tan <Yom.Tan@tuhh.de>
    */
    explicit RV_ConstFold( WIR_CompilationUnit & );

    /*!
      @brief Default constructor for function-level optimization.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Yom Ismail Tan <Yom.Tan@tuhh.de>
    */
    explicit RV_ConstFold( WIR_Function & );

    /*!
      @brief Destructor.
      @author Yom Ismail Tan <Yom.Tan@tuhh.de>
    */
    virtual ~RV_ConstFold( void );


  protected:

    /*!
      @brief runOptimization folds constants in the given function.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Yom Ismail Tan <Yom.Tan@tuhh.de>
    */
    virtual void runOptimization( WIR_Function & ) override;

    /*!
      @brief For an operation identified to be constant, doConstFolding does the
             actual RISC-V-specific folding.

      @param[in] o A const reference to a %WIR operation identified as constant.
      @param[in] outValue A const reference to a map mapping all defined or
                          def-used register parameters to their outgoing bit
                          value.
      @param[in] inValue A const reference to a map mapping all used or def-used
                         register parameters to their incoming bit value.
      @return A Boolean denoting whether new instructions were produced for o or
              not.

      doConstFolding does not actually modify the currently examined %WIR
      operation o. Instead, new instructions realizing the constant folding of o
      are added to map mNewInstructions.

      @author Yom Ismail Tan <Yom.Tan@tuhh.de>
    */
    virtual bool doConstFolding( const WIR_Operation &,
                                 const std::map<WIR_id_t, WIR_UpDownValue> &,
                                 const std::map<WIR_id_t, WIR_UpDownValue> & ) override;

    /*!
      @brief For an operation identified to be constant, doBranchFolding
             performs RISC-V-specific folding of conditional branches.

      @param[in] o A const reference to a %WIR operation identified as constant.
      @param[in] inValue A const reference to a map mapping all used or def-used
                         register parameters to their incoming bit value.
      @return A Boolean denoting whether new instructions were produced for o or
              not.

      If the result of a jump condition is statically known, the conditional
      branch gets folded either to an unconditional branch or gets removed
      completely.

      doBranchFolding does not actually modify the currently examined %WIR
      operation o. Instead, new instructions realizing the constant folding of o
      are added to map mNewInstructions.

      @author Muthappan Viswanathan <Muthappan.Viswanathan@tuhh.de>
    */
    virtual bool doBranchFolding( const WIR_Operation &,
                                  const std::map<WIR_id_t, WIR_UpDownValue> & ) override;

    /*!
      @brief For an operation identified to be constant, doMOVFolding performs
             RISC-V-specific folding into register MOV operations.

      @param[in] o A const reference to a %WIR operation identified as constant.
      @param[in] inValue A const reference to a map mapping all used or def-used
                         register parameters to their incoming bit value.
      @return A Boolean denoting whether new instructions were produced for o or
              not.

      If the computations of an operation are statically known to have no
      arithmetical effect, doMOVFolding folds the operation into a register MOV.

      doMOVFolding does not actually modify the currently examined %WIR
      operation o. Instead, new instructions realizing the constant folding of o
      are added to map mNewInstructions.

      @author Muthappan Viswanathan <Muthappan.Viswanathan@tuhh.de>
    */
    virtual bool doMOVFolding( const WIR_Operation &,
                               const std::map<WIR_id_t, WIR_UpDownValue> & ) override;


  private:

    /*!
      @brief getMOV generates RISC-V instructions loading a register with a
             constant.

      @param[in] p A const reference to a register parameter whose data register
                   shall be loaded with a constant value.
      @param[in] v A const reference to an up value to be loaded into the
                   register.
      @return A list of RISC-V instructions implementing the desired action.

      @author Yom Ismail Tan <Yom.Tan@tuhh.de>
    */
    std::list<WIR_Instruction> getMOV( const WIR_Parameter &,
                                       const WIR_UpDownValue & ) const;

    /*!
      @brief getMOV generates RISC-V instructions performing a register-register
             MOV.

      @param[in] t A const reference to a register parameter being the MOV's
                   target.
      @param[in] s A const reference to a register parameter being the MOV's
                   source.
      @return A list of RISC-V instructions implementing the desired action.

      @author Muthappan Viswanathan <Muthappan.Viswanathan@tuhh.de>
    */
    std::list<WIR_Instruction> getMOV( const WIR_RegisterParameter &,
                                       const WIR_RegisterParameter & );

    /*!
      @brief getJ generates RISC-V instructions unconditionally jumping to the
             given label.

      @param[in] p A const reference to a label parameter that denotes the jump
                   target.
      @return A list of RISC-V instructions implementing the desired action.

      @author Muthappan Viswanathan <Muthappan.Viswanathan@tuhh.de>
    */
    std::list<WIR_Instruction> getJ( const WIR_LabelParameter & ) const;

    /*!
      @brief getCJ generates compressed RISC-V instructions unconditionally
             jumping to the given label.

      @param[in] p A const reference to a label parameter that denotes the jump
                   target.
      @return A list of RISC-V instructions implementing the desired action.

      @author Muthappan Viswanathan <Muthappan.Viswanathan@tuhh.de>
    */
    std::list<WIR_Instruction> getCJ( const WIR_LabelParameter & ) const;

    /*!
      @brief insertMOV generates the actual RISC-V instructions to load a
             register with a constant.

      @param[in] r A const reference to the register to be loaded.
      @param[in] v A const reference to an up value to be loaded into the
                   register.
      @param[in,out] l A reference to a list of RISC-V instructions
                       implementing the desired constant MOV.
      @return A reference to a defined register parameter that finally holds the
              correctly loaded register.

      @author Yom Ismail Tan <Yom.Tan@tuhh.de>
    */
    WIR_RegisterParameter &insertMOV( const WIR_BaseRegister &,
                                      const WIR_UpDownValue &,
                                      std::list<WIR_Instruction> & ) const;

    //! mCurrentFuction points to the function that is currently optimized.
    WIR_Function *mCurrentFunction;

};

}

#endif // _RV_CONSTFOLD_H
