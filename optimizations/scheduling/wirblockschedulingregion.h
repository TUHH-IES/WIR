/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirblockschedulingregion.h
  @brief This file provides the interface of a class representing basic block
         regions in which scheduling is performed.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_BLOCKSCHEDULINGREGION_H
#define _WIR_BLOCKSCHEDULINGREGION_H


//
// Include section
//

// Include WIR headers
#include <optimizations/scheduling/wirschedulingregion.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_BasicBlock;


/*!
  @brief Class WIR_BlockSchedulingRegion represents basic block scheduling
         regions.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_BlockSchedulingRegion : public WIR_SchedulingRegion
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor.

      @param[in] b A reference to a %WIR basic block to be considered as
                   scheduling region.
      @param[in] verbosity A Boolean defaulting to false that denotes whether
                           verbose messages shall be dumped.
      @param[in] keepTmpFiles A Boolean defaulting to false that denotes whether
                              temporary files shall be kept.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit WIR_BlockSchedulingRegion( WIR_BasicBlock &, bool = false,
                                        bool = false );

    /*!
      @brief Destructor.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_BlockSchedulingRegion( void );


  protected:


  private:

    /*!
      @brief No standard construction allowed, users must use one of the
             standard constructors above instead.
    */
    WIR_BlockSchedulingRegion( void ) = delete;


    //
    // Attributes
    //

};

}       // namespace WIR

#endif  // _WIR_BLOCKSCHEDULINGREGION_H
