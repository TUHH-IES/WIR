/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2025 - 2026, Heiko Falk.

*/

/*!
  @file rvconstprop.h
  @brief This file provides the interface of a RISC-V-specific constant
         propagation optimization.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _RV_CONSTPROP_H
#define _RV_CONSTPROP_H


//
// Include section
//

// Include standard headers
#include <list>
#include <map>

// Include WIR headers
#include <wir/wirtypes.h>
#include <optimizations/constprop/wirconstprop.h>


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
class WIR_UpDownValue;


/*!
  @brief Class RV_ConstProp is a RISC-V-specific optimization that propagates
         constants in %WIR functions.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class RV_ConstProp final : public WIR_ConstProp
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
    explicit RV_ConstProp( WIR_System & );

    /*!
      @brief Default constructor for compilation unit-level optimization.

      @param[in] c A reference to a WIR_CompilationUnit to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit RV_ConstProp( WIR_CompilationUnit & );

    /*!
      @brief Default constructor for function-level optimization.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit RV_ConstProp( WIR_Function & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~RV_ConstProp( void );


  protected:

    /*!
      @brief runOptimization propagates constants in the given function.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOptimization( WIR_Function & ) override;

    /*!
      @brief doConstProp does the actual RISC-V-specific propagation of
             constants for a given operation.

      @param[in] o A const reference to a %WIR operation.
      @param[in] inValue A const reference to a map mapping all defined or
                         def-used register parameters to their outgoing bit
                         value.
      @return A Boolean denoting whether new instructions were produced for o or
              not.

      doConstProp does not actually modify the currently examined %WIR operation
      o. Instead, new instructions realizing the constant folding of o are added
      to map mNewInstructions.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool doConstProp( const WIR_Operation &,
                              const std::map<
                                      WIR_id_t, WIR_UpDownValue> & ) override;


  private:

    /*!
      @brief prop_RRR_RRC12 propagates constants for a RISC-V operation of
             format RRR_1.

      @param[in] o A const reference to an operation of format RRR_1
                   whose last explicit parameter is a constant.
      @param[in] v A const reference to the up value of the operation's last
                   explicit parameter.
      @return A list of RISC-V instructions with format RRC12_1, resp.

      If the given up value does not fit into a 12 bits signed constant,
      prop_RRR_RRC12 returns an empty instruction list.

      @author Muthappan Viswanathan <Muthappan.Viswanathan@tuhh.de>
    */
   std::list<WIR_Instruction> prop_RRR_RRC12( const WIR_Operation &,
                                              const WIR_UpDownValue & );

    /*!
      @brief prop_RRR_RRC12_2 propagates constants for a commutative RISC-V
             operation of format RRR_1.

      @param[in] o A const reference to an operation of format RRR_1
                   whose second explicit parameter is a constant.
      @param[in] v A const reference to the up value of the operation's second
                   explicit parameter.
      @return A list of RISC-V instructions with format RRC12_1, resp.

      If the given up value does not fit into a 12 bits signed constant,
      prop_RRR_RRC12 returns an empty instruction list.

      @author Muthappan Viswanathan <Muthappan.Viswanathan@tuhh.de>
    */
   std::list<WIR_Instruction> prop_RRR_RRC12_2( const WIR_Operation &,
                                                const WIR_UpDownValue & );

    /*!
      @brief prop_RRR_RRC5 propagates constants for a RISC-V operation of
             format RRR_1.

      @param[in] o A const reference to an operation of format RRR_1
                   whose last explicit parameter is a constant.
      @param[in] v A const reference to the up value of the operation's last
                   explicit parameter.
      @return A list of RISC-V instructions with format RRC5_1, resp.

      If the given up value does not fit into a 5 bits unsigned constant,
      prop_RRR_RRC5 returns an empty instruction list.

      @author Muthappan Viswanathan <Muthappan.Viswanathan@tuhh.de>
    */
   std::list<WIR_Instruction> prop_RRR_RRC5( const WIR_Operation &,
                                             const WIR_UpDownValue & );

    /*!
      @brief prop_SRR_SRC6 propagates constants for a RISC-V operation of
             format SRR_2.

      @param[in] o A const reference to an operation of format SRR_2
                   whose last explicit parameter is a constant.
      @param[in] v A const reference to the up value of the operation's last
                   explicit parameter.
      @return A list of RISC-V instructions with format SRC6_3, resp.

      If the given up value does not fit into a 6 bits signed constant,
      prop_SRR_SRC6 returns an empty instruction list.

      @author Muthappan Viswanathan <Muthappan.Viswanathan@tuhh.de>
    */
   std::list<WIR_Instruction> prop_SRR_SRC6( const WIR_Operation &,
                                             const WIR_UpDownValue & );

};

}       // namespace WIR

#endif  // _RV_CONSTPROP_H
