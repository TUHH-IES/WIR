/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirifthenelsetreenode.h
  @brief This file provides the basic properties of if-then-else tree nodes.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_IFTHENELSETREENODE_H
#define _WIR_IFTHENELSETREENODE_H


//
// Include section
//

// Include standard headers
#include <fstream>

// Include WIR headers
#include <analyses/structuralcontrolflow/wircontroltreenode.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class WIR_IfThenElseTreeNode represents if-then-else regions of the
         %WIR control tree.

  - Cyclic: No
  - Number of Entries: 1
  - Number of Exits: 2

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_IfThenElseTreeNode : public WIR_ControlTreeNode
{

  public:

    /*!
      @brief Default constructor.

      @param[in] c A const reference to an if-then-else's condition node.
      @param[in] b1 A const reference to the control tree node representing the
                    first branch of an if-then-else.
      @param[in] b2 A const reference to the control tree node representing the
                    second branch of an if-then-else.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_IfThenElseTreeNode( const WIR_ControlTreeNode &,
                            const WIR_ControlTreeNode &,
                            const WIR_ControlTreeNode & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_IfThenElseTreeNode( void );


    //
    // Type handling.
    //

    /*!
      @brief getType returns the type of an if-then-else region node.

      @return WIR_CTNodeType::ifthenelse

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_CTNodeType getType( void ) const override;

    /*!
      @brief isCyclic returns whether an if-then-else region is cyclic or not.

      @return Always false.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool isCyclic( void ) const override;


    //
    // Control Tree hierarchy.
    //

    /*!
      @brief getEntry returns an if-then-else region's unique entry child node.

      @return A const reference to an if-then-else's condition node.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual const WIR_ControlTreeNode &getEntry( void ) const override;


    //
    // Handling of if-then-else nodes.
    //

    /*!
      @brief getCondition returns an if-then-else's condition node.

      @return A const reference to mCondition.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    const WIR_ControlTreeNode& getCondition( void ) const;

    /*!
      @brief getBranch1 returns the control tree node representing the first
             branch of an if-then-else.

      @return A const reference to mBranch1.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    const WIR_ControlTreeNode& getBranch1( void ) const;

    /*!
      @brief getBranch2 returns the control tree node representing the second
             branch of an if-then-else.

      @return A const reference to mBranch2.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    const WIR_ControlTreeNode& getBranch2( void ) const;


  protected:

    //
    // Visualization.
    //

    /*!
      @brief visualize dumps an if-then-else region node into a given DOT file.

      @param[in,out] dotFile A reference to a DOT file opened for writing.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void visualize( std::fstream & ) const override;


  private:

    /*!
      @brief No standard construction allowed, users must use the public
             constructor above instead.
    */
    WIR_IfThenElseTreeNode( void ) = delete;


    //
    // Attributes.
    //

    //! mCondition refers to an if-then-else's condition node.
    const WIR_ControlTreeNode &mCondition;

    /*!
      @brief mBranch1 refers to the control tree node representing the first
             branch of an if-then-else.
    */
    const WIR_ControlTreeNode &mBranch1;

    /*!
      @brief mBranch2 refers to the control tree node representing the second
             branch of an if-then-else.
    */
    const WIR_ControlTreeNode &mBranch2;

};

}       // namespace WIR

#endif  // _WIR_IFTHENELSETREENODE_H
