/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirifthentreenode.h
  @brief This file provides the basic properties of if-then tree nodes.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_IFTHENTREENODE_H
#define _WIR_IFTHENTREENODE_H


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
  @brief Class WIR_IfThenTreeNode represents if-then regions of the %WIR control
         tree.

  - Cyclic: No
  - Number of Entries: 1
  - Number of Exits: 2

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_IfThenTreeNode : public WIR_ControlTreeNode
{

  public:

    /*!
      @brief Default constructor.

      @param[in] c A const reference to an if-then's condition node.
      @param[in] b A const reference to the control tree node representing the
                   then-branch of an if-then.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_IfThenTreeNode( const WIR_ControlTreeNode &,
                        const WIR_ControlTreeNode & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_IfThenTreeNode( void );


    //
    // Type handling.
    //

    /*!
      @brief getType returns the type of an if-then region node.

      @return WIR_CTNodeType::ifthen

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_CTNodeType getType( void ) const override;

    /*!
      @brief isCyclic returns whether an if-then region is cyclic or not.

      @return Always false.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool isCyclic( void ) const override;


    //
    // Control Tree hierarchy.
    //

    /*!
      @brief getEntry returns an if-then region's unique entry child node.

      @return A const reference to an if-then's condition node.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual const WIR_ControlTreeNode &getEntry( void ) const override;


    //
    // Handling of if-then nodes.
    //

    /*!
      @brief getCondition returns an if-then's condition node.

      @return A const reference to mCondition.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    const WIR_ControlTreeNode& getCondition( void ) const;

    /*!
      @brief getBranch returns the control tree node representing the then-
             branch of an if-then.

      @return A const reference to mBranch.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    const WIR_ControlTreeNode& getBranch( void ) const;


  protected:

    //
    // Visualization.
    //

    /*!
      @brief visualize dumps an if-then region node into a given DOT file.

      @param[in,out] dotFile A reference to a DOT file opened for writing.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void visualize( std::fstream & ) const override;


  private:

    /*!
      @brief No standard construction allowed, users must use the public
             constructor above instead.
    */
    WIR_IfThenTreeNode( void ) = delete;


    //
    // Attributes.
    //

    //! mCondition refers to an if-then's condition node.
    const WIR_ControlTreeNode &mCondition;

    /*!
      @brief mBranch refers to the control tree node representing the then-
             branch of an if-then.
    */
    const WIR_ControlTreeNode &mBranch;

};

}       // namespace WIR

#endif  // _WIR_IFTHENTREENODE_H
