/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wircfgedgeproperty.h
  @brief This file provides the basic properties of control flow graph edges.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_CFG_EDGE_PROPERTY_H
#define _WIR_CFG_EDGE_PROPERTY_H


//
// Header section
//

namespace WIR {

/*!
  @brief This enum represents different types of %WIR control flow graph edges.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
enum class WIR_CFGEdgeType : char
{
  //! A regular edge.
  regular,

  //! A true-edge of a conditional jump.
  tru,

  //! A function call edge.
  call,

  //! A function return edge.
  ret
};


/*!
  @brief Class WIR_CFGEdgeProperty represents the information attached to each
         edge of the %WIR control flow graph.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_CFGEdgeProperty
{

  public:

    /*!
      @brief Default constructor for regular control flow graph edges.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_CFGEdgeProperty( void ) :
      mType { WIR_CFGEdgeType::regular },
      mIsBackEdge { false }
    {
    };

    //! mType stores a CFG edge's actual type.
    WIR_CFGEdgeType mType;

    //! mIsBackEdge stores whether a CFG edge is a back edge or not.
    bool mIsBackEdge;

};

}       // namespace WIR

#endif  // _WIR_CFG_EDGE_PROPERTY_H
