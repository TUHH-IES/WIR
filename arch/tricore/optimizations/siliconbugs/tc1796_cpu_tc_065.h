/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tc1796_cpu_tc_065.h
  @brief This file provides the interface of a peephole optimizer for silicon
         bug TC1796 CPU_TC.065 detection and correction.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_SILICONBUGS_TC1796_CPU_TC_065_H
#define _WIR_SILICONBUGS_TC1796_CPU_TC_065_H


//
// Include section
//

// Include WIR headers
#include <optimizations/siliconbugs/wirsiliconbugs.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_Function;


//
// Header section
//

/*!
  @brief Class TC1796_CPU_TC_065 is a peephole optimizer for Infineon TriCore
         TC1796 silicon bug CPU_TC.065.

  See TC1796, EES-BC, ES-BS, BC Errata Sheet Rel. 2.0, 23.06.2008.

  <B>CPU_TC.065: Error when unconditional loop targets unconditional jump</B>

  An error in the program flow occurs when an unconditional loop
  (<TT>LOOPU</TT>) instruction has as its target an unconditional jump
  instruction, i.e. as the first instruction of the loop. Such unconditional
  jump instructions are <TT>J</TT>, <TT>JA</TT>, <TT>JI</TT>, <TT>JL</TT>,
  <TT>JLA</TT> and <TT>JLI</TT>.

  In this erroneous case the first iteration of the loop executes correctly.
  However, at the point the second loop instruction is executed the interaction
  of the unconditional loop and jump instructions causes the loop instruction to
  be resolved as mis-predicted and the program flow exits the loop incorrectly,
  despite the loop instruction being unconditional.

  <B>Example</B>

  <PRE>
  ...
  loop_start_:                                ; Loop start label
  J jump_label_                               ; Unconditional Jump instruction
  ...
  LOOPU loop_start_
  ...
  </PRE>

  <B>Workaround</B>

  The first instruction of a loop may not be an unconditional jump. If
  necessary, precede this jump instruction with a single <TT>NOP</TT>.

  <PRE>
  ...
  loop_start_:                                ; Loop start label
  NOP
  J jump_label_                               ; Unconditional Jump instruction
  ...
  LOOPU loop_start_
  ...
  </PRE>

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC1796_CPU_TC_065 final : public WIR_SiliconBugs
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor.

      @param[in,out] f A reference to a WIR_Function to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit TC1796_CPU_TC_065( WIR_Function & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC1796_CPU_TC_065( void );


  protected:

    /*!
      @brief matchSiliconBug determines whether the specified peephole matches
             with silicon bug CPU_TC.065.

      @param[in] p A const reference to a peephole.
      @return true if p matches with silicon bug CPU_TC.065, false otherwise.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool matchSiliconBug( const WIR_Peephole::peephole & ) override;

    /*!
      @brief fixSiliconBug fixes silicon bug CPU_TC.065.

      @param[in] p A const reference to a peephole.
      @return The peephole after bug fixing.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_Peephole::peephole fixSiliconBug( const WIR_Peephole::peephole & ) const override;

};

}       // namespace WIR

#endif  // _WIR_SILICONBUGS_TC1796_CPU_TC_065_H
