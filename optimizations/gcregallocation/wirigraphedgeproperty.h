/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirigraphedgeproperty.h
  @brief This file provides the basic properties of interference graph edges.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_IGRAPH_EDGE_PROPERTY_H
#define _WIR_IGRAPH_EDGE_PROPERTY_H


//
// Header section
//

namespace WIR {

/*!
  @brief Struct IGraphEdgeProperty represents the information attached to each
         edge of the %WIR interference graph.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
struct IGraphEdgeProperty
{

  public:

    /*!
      @brief Default constructur for interference graph edge information.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    IGraphEdgeProperty() :
      mIsPushed( false )
    {
    };

    /*!
      @brief mIsPushed marks an interference graph edge as being removed from
             the interference graph and pushed onto the stack.
    */
    bool mIsPushed;

};

}       // namespace WIR

#endif  // _WIR_IGRAPH_EDGE_PROPERTY_H
