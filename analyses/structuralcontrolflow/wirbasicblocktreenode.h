/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirbasicblocktreenode.h
  @brief This file provides the basic properties of tree nodes representing
         individual basic blocks.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_BASICBLOCKTREENODE_H
#define _WIR_BASICBLOCKTREENODE_H


//
// Include section
//

// Include standard headers
#include <fstream>
#include <functional>
#include <list>

// Include WIR headers
#include <analyses/structuralcontrolflow/wircontroltreenode.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_BasicBlock;


/*!
  @brief Class WIR_BasicBlockTreeNode represents individual basic blocks within
         the %WIR control tree.

  - Cyclic: No
  - Number of Entries: 1
  - Number of Exits: 1

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_BasicBlockTreeNode : public WIR_ControlTreeNode
{

  public:

    /*!
      @brief Default constructor.

      @param[in] b A reference to a basic block associated with the node.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit WIR_BasicBlockTreeNode( WIR_BasicBlock & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_BasicBlockTreeNode( void );


    //
    // Type handling.
    //

    /*!
      @brief getType returns the type of a basic block node.

      @return WIR_CTNodeType::bb

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_CTNodeType getType( void ) const override;

    /*!
      @brief isCyclic returns whether a basic block node is cyclic or not.

      @return Always false.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool isCyclic( void ) const override;


    //
    // Control Tree hierarchy.
    //

    /*!
      @brief getEntry returns a basic block node's unique entry child node.

      @return A const reference to itself.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual const WIR_ControlTreeNode &getEntry( void ) const override;


    //
    // Handling of basic blocks.
    //

    /*!
      @brief getBasicBlockBlock returns the basic block represented by this
             control region.

      @return A const reference to the basic block mBasicBlock.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_BasicBlock &getBasicBlock( void ) const;


  protected:

    //
    // Visualization.
    //

    /*!
      @brief visualize dumps a basic block node into a given DOT file.

      @param[in,out] dotFile A reference to a DOT file opened for writing.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void visualize( std::fstream & ) const override;


  private:

    //
    // Attributes.
    //

    /*!
      @brief mBasicBlock refers to the single basic block represented by a
             control tree node.
    */
    WIR_BasicBlock &mBasicBlock;

};

}       // namespace WIR

#endif  // _WIR_BASICBLOCKTREENODE_H
