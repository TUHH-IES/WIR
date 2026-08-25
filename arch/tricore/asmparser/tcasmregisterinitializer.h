/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcasmregisterinitializer.h
  @brief This file provides the interface of a generic %WIR register
         initializing class.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_ASMREGISTERINITIALIZER_H
#define _TC_ASMREGISTERINITIALIZER_H


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class TC_DRegV;
class TC_ERegV;


/*!
  @brief Class TC_AsmRegisterInitializer provides a minimal API to initialize
         and retrieve %WIR registers for template register arguments within GNU
         inline assembly.

  Using input and output operands in GNU inline assembly, the assembly code can
  be connected with variables inside the sorrounding C source code. These input
  and output operands thus act as the interface between C and assembly.

  Since %WIR in general and this assembly code parser in particular are
  completely unaware of C source code due to lacking integration of a high-level
  IR like, e.g., ICD-C, it is impossible to fully implement this link between C
  and assembly here. Thus, this class only provides a generic API so that the
  assembly code parser can retrieve the proper %WIR registers to be used for the
  given input and output operands. By inheriting from this class, the lacking
  functionality (i.e., the link to a high-level IR and the connection between C
  source code variables and these %WIR registers) has to be implemented outside
  the assembly code parser, e.g., inside a compiler's code selector.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_AsmRegisterInitializer
{

  public:

    //
    // Destructor.
    //

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_AsmRegisterInitializer( void );


    //
    // Register management.
    //

    /*!
      @brief getDReg returns the virtual data register associated with this
             object.

      @return A const reference to a TriCore virtual data register.

      Depending on the register's context, getDReg also generates assembly code
      to properly initialize the data register. Since these actions depend on
      the register's context within a GNU inline assembly statement, getDReg is
      purely virtual here.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual const TC_DRegV &getDReg( void ) = 0;

    /*!
      @brief getEReg returns the virtual extended register associated with this
             object.

      @return A const reference to a virtual TriCore extended register.

      Depending on the register's context, getEReg also generates assembly code
      to properly initialize the extended register. Since these actions depend
      on the register's context within a GNU inline assembly statement, getEReg
      is purely virtual here.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual const TC_ERegV &getEReg( void ) = 0;

};

}       // namespace WIR

#endif  // _TC_ASMREGISTERINITIALIZER_H
