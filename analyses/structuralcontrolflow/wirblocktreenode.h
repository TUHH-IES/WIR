/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirblocktreenode.h
  @brief This file provides the basic properties of block tree nodes.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_BLOCKTREENODE_H
#define _WIR_BLOCKTREENODE_H


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

/*!
  @brief Class WIR_BlockTreeNode represents block regions of the %WIR control
         tree.

  - Cyclic: No
  - Number of Entries: 1
  - Number of Exits: 1

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_BlockTreeNode : public WIR_ControlTreeNode
{

  public:

    /*!
      @brief Default constructor.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_BlockTreeNode( void );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_BlockTreeNode( void );


    //
    // Type handling.
    //

    /*!
      @brief getType returns the type of a block region node.

      @return WIR_CTNodeType::block

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_CTNodeType getType( void ) const override;

    /*!
      @brief isCyclic returns whether a block region node is cyclic or not.

      @return Always false.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool isCyclic( void ) const override;


    //
    // Control Tree hierarchy.
    //

    /*!
      @brief getEntry returns a block region's unique entry child node.

      @return A const reference to the first entry in the block list.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual const WIR_ControlTreeNode &getEntry( void ) const override;


    //
    // Handling of block nodes.
    //

    /*!
      @brief getBlockList returns the list of stored child nodes in their
             sequential order.

      @return A const reference to the list mBlockList.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    const std::list<std::reference_wrapper<WIR_ControlTreeNode>> &getBlockList( void ) const;


  protected:

    //
    // Visualization.
    //

    /*!
      @brief visualize dumps a block region node into a given DOT file.

      @param[in,out] dotFile A reference to a DOT file opened for writing.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void visualize( std::fstream & ) const override;


  private:

    friend class WIR_StructuralAnalysis;

    //
    // Handling of block nodes.
    //

    /*!
      @brief pushBackBlockNode adds a new node to a block region at the end of
             list mBlockList.

      @param[in] n A reference to the control tree node to be inserted into a
                   block.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void pushBackBlockNode( WIR_ControlTreeNode & );


    //
    // Attributes.
    //

    /*!
      @brief mBlockList holds wrapped references to all stored child nodes in
             their sequential order.
    */
    std::list<std::reference_wrapper<WIR_ControlTreeNode>> mBlockList;

};

}       // namespace WIR

#endif  // _WIR_BLOCKTREENODE_H
