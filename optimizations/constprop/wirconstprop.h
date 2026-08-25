/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirconstfold.h
  @brief This file provides the interface of a constant propagation
         optimization.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_CONSTPROP_H
#define _WIR_CONSTPROP_H


//
// Include section
//

// Include standard headers
#include <functional>
#include <list>
#include <map>

// Include WIR headers
#include <wir/wirtypes.h>
#include <optimizations/bitopt/wirbitopt.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_BitDFA;
class WIR_BaseImmediateParameter;
class WIR_CompilationUnit;
class WIR_Function;
enum class WIR_L4 : char;
class WIR_Operation;
class WIR_Parameter;
class WIR_RegisterParameter;
class WIR_System;
class WIR_UpDownValue;


/*!
  @brief Class WIR_ConstProp is an optimization that propagates constants in
         %WIR functions.

  According to Steven S. Muchnik (Advanced Compiler Design & Implementation,
  page 362), "constant propagation is a transformation that, given an assignment
  'x := c' for a variable x and a constant c, replaces later uses of x with uses
  of c as long as intervening assignments have not changed the value of x."

  For a specific instruction set architecture, constant propagation actually
  replaces register parameters of operations by immediate parameters, in order
  to make constant operands explicit.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_ConstProp : public WIR_BitOpt
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
    explicit WIR_ConstProp( WIR_System & );

    /*!
      @brief Default constructor for compilation unit-level optimization.

      @param[in] c A reference to a WIR_CompilationUnit to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit WIR_ConstProp( WIR_CompilationUnit & );

    /*!
      @brief Default constructor for function-level optimization.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit WIR_ConstProp( WIR_Function & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_ConstProp( void );


  protected:

    /*!
      @brief runOptimization propagates constants in the given system.

      @param[in] s A reference to a WIR_System to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOptimization( WIR_System & ) override;

    /*!
      @brief runOptimization propagates constants in the given compilation unit.

      @param[in] c A reference to a WIR_CompilationUnit to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOptimization( WIR_CompilationUnit & ) override;

    /*!
      @brief runOptimization propagates constants in the given function.

      @param[in] f A reference to a WIR_Function to be optimized.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void runOptimization( WIR_Function & ) override;

    /*!
      @brief doConstProp does the actual propagation of constants for a given
             operation.

      @param[in] o A const reference to a %WIR operation.
      @param[in] inValue A const reference to a map mapping all used or def-used
                         register parameters to their incoming bit value.
      @return A Boolean denoting whether new instructions were produced for o or
              not.

      Since the actual propagation of constants is processor-specific, this
      method is purely virtual.

      doConstProp does not actually modify the currently examined %WIR operation
      o. Instead, new instructions realizing the constant propagation of o are
      added to map mNewInstructions.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool doConstProp( const WIR_Operation &o,
                              const std::map<
                                      WIR_id_t, WIR_UpDownValue> &inValue ) = 0;

    /*!
      @brief replace replaces all 'X' bits in the given up/down value by the
             specified L4 value.

      @param[in] v A const reference to an up/down value.
      @param[in] b The L4 value to be used as replacement for 'X' bits. L and N
                   must not be used here.
      @param[in] s A Boolean flag denoting whether v shall be considered signed
                   or unsigned.
      @return A signed integer representing the up/down value's numerical value
              after replacement of 'X' bits.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    signed long long replace( const WIR_UpDownValue &, WIR_L4, bool ) const;

    /*!
      @brief patchDefinedParameter updates the bit-values associated with a
             defined parameter of an operation subject to constant propagation.

      @param[in] p A const reference to the defined register parameter of the
                   operation created by constant propagation.
      @param[in] pOrig A const reference to the defined register parameter of
                       the original operation before constant propagation.

      For the targets of all out-edges of p, patchDefinedParameter removes pOrig
      from the set of incoming bit-values and adds p instead.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void patchDefinedParameter( const WIR_RegisterParameter &,
                                const WIR_Parameter & );

    /*!
      @brief patchUsedParameter updates the bit-values associated with a used
             parameter of an operation subject to constant propagation.

      @param[in] p A const reference to the used register parameter of the
                   operation created by constant propagation.
      @param[in] pOrig A const reference to the used register parameter of the
                       original operation before constant propagation.

      For the sources of all in-edges of p, patchUsedParameter adds p to the set
      of outgoing bit-values.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void patchUsedParameter( const WIR_Parameter &,
                             const WIR_Parameter & ) const;

    /*!
      @brief patchImmediateParameter creates a bit-value container for an
             immediate parameter newly created during constant propagation.

      @param[in,out] p A reference to the immediate parameter to be updated.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void patchImmediateParameter( WIR_BaseImmediateParameter & ) const;

    /*!
      @brief checkSelfEdges checkes whether an original operation contains
             DFG edges starting and ending both at some of the operation's
             parameters. Such self-edges are then taken over to a new
             constant-propagated operation.

      @param[in,out] o A reference to a new constant-folded operation.
      @param[in] oOrig A const reference to an original operation.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void checkSelfEdges( WIR_Operation &, const WIR_Operation & ) const;

    /*!
      @brief isSymbol checks whether an up/down value entirely refers to one
             %WIR symbol.

      @param[in] v A const reference to the up/down value to be checked.
      @return true iff the given up/down value refers completely to one symbol,
              false otherwise.

      In order to refer completely to a symbol, all bits of the up/down value
      must be L, all locations must refer to one and the same symbol, and bit
      i in the given up/down value must refer to bit i of the symbol's address.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    bool isSymbol( const WIR_UpDownValue & ) const;

    /*!
      mNewInstructions maps the a constant-propagated operation to a list of new
      instructions implementing the actual propagation.
    */
    std::map<
      std::reference_wrapper<WIR_Operation>, std::list<WIR_Instruction>,
      WIR_Compare<WIR_Operation>> mNewInstructions;


  private:

    /*!
      @brief constProp performs the generic, processor-independent parts of
             constant propagation.

      @param[in] o A const reference to a %WIR operation to be propagated.
      @return A Boolean denoting whether new instructions were produced for o or
              not.

      constProp basically combines the possibly several incoming up/down values
      per parameter into a single up/down value and calls the processor-specific
      virtual methods that check and finally realize propagation.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    bool constProp( const WIR_Operation & );

    /*!
      @brief addNewInstructions processes map mNewInstructions and adds the
             generated constant-propagated instructions immediately behind each
             original constant operation.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void addNewInstructions( void );

    /*!
      @brief eraseInstructions processes list mErasePositions and erases all
             instructions therein.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void eraseInstructions( void );

    /*!
      @brief For an operation ID, mIterator stores an iterator pointing to that
             operation's instruction.
    */
    std::map<
      WIR_id_t,
      std::list<
        std::reference_wrapper<WIR_Instruction>>::const_iterator> mIterator;

    /*!
      @brief mErasePositions stores iterators to WIR instructions to be erased
             after constant propagation.
    */
    std::list<
      std::list<
        std::reference_wrapper<
          WIR_Instruction>>::const_iterator> mErasePositions;

};

}       // namespace WIR

#endif  // _WIR_CONSTPROP_H
