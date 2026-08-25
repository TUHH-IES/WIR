/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirmobilitypriority.h
  @brief This file provides the interface of an abstract base class computing
         the mobility scheduling priority for %WIR operations.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_MOBILITYPRIORITY_H
#define _WIR_MOBILITYPRIORITY_H


//
// Include section
//

// Include WIR headers
#include <optimizations/scheduling/wirschedulingpriority.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_Operation;


/*!
  @brief Class WIR_MobilityPriority is a base class used to determine scheduling
         priorities based on an operation's mobility.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_MobilityPriority : virtual WIR_SchedulingPriority
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_MobilityPriority( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_MobilityPriority( const WIR_MobilityPriority & );

    /*!
      @brief Destructor.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_MobilityPriority( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_MobilityPriority & operator = ( const WIR_MobilityPriority & );


    //
    // Scheduling priority handling.
    //

    /*!
      @brief getPriority determines an operation's scheduling priority using the
             operation's mobility.

      @param[in] o A const reference to an operation whose scheduling priority
                   is computed.
      @return A signed long long value denoting the operation's scheduling
              priority.

      Operations with only limited mobility receive a high priority.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual long long getPriority( const WIR_Operation & ) const override;

};

}       // namespace WIR

#endif  // _WIR_MOBILITYPRIORITY_H
