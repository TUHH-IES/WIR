/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirselflooptreenode.h
  @brief This file provides the basic properties of self-loop tree nodes.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_SELFLOOPTREENODE_H
#define _WIR_SELFLOOPTREENODE_H


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
  @brief Class WIR_SelfLoopTreeNode represents self-loop regions of the %WIR
         control tree.

  - Cyclic: Yes
  - Number of Entries: 1
  - Number of Exits: 1

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_SelfLoopTreeNode : public WIR_ControlTreeNode
{

  public:

    /*!
      @brief Default constructor.

      @param[in] n A const reference to a self-loop's entry node.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit WIR_SelfLoopTreeNode( const WIR_ControlTreeNode & );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_SelfLoopTreeNode( void );


    //
    // Type handling.
    //

    /*!
      @brief getType returns the type of a self-loop region node.

      @return WIR_CTNodeType::selfloop

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_CTNodeType getType( void ) const override;

    /*!
      @brief isCyclic returns whether a self-loop region is cyclic or not.

      @return Always true.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool isCyclic( void ) const override;


    //
    // Control Tree hierarchy.
    //

    /*!
      @brief getEntry returns a self-loop's unique entry child node.

      @return A const reference to mEntry.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual const WIR_ControlTreeNode &getEntry( void ) const override;


  protected:

    //
    // Visualization.
    //

    /*!
      @brief visualize dumps a self-loop region node into a given DOT file.

      @param[in,out] dotFile A reference to a DOT file opened for writing.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual void visualize( std::fstream & ) const override;


  private:

    /*!
      @brief No standard construction allowed, users must use the public
             constructor above instead.
    */
    WIR_SelfLoopTreeNode( void ) = delete;


    //
    // Attributes.
    //

    //! mEntry refers to a self-loop's entry node.
    const WIR_ControlTreeNode &mEntry;

};

}       // namespace WIR

#endif  // _WIR_SELFLOOPTREENODE_H
