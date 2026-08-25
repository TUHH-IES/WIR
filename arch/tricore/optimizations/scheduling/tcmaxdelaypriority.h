/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcmaxdelaypriority.h
  @brief This file provides the interface of a class computing the maximum delay
         scheduling priority for TriCore operations.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _TC_MAXDELAYPRIORITY_H
#define _TC_MAXDELAYPRIORITY_H


//
// Include section
//

// Include WIR headers
#include <optimizations/scheduling/wirmaxdelaypriority.h>
#include <arch/tricore/optimizations/scheduling/tcschedulingpriority.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_Operation;


/*!
  @brief Class TC_MaxDelayPriority determines maximum delay scheduling
         priorities for TriCore operations.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class TC_MaxDelayPriority : public WIR_MaxDelayPriority,
                            public TC_SchedulingPriority
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_MaxDelayPriority( void );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_MaxDelayPriority( const TC_MaxDelayPriority & );

    /*!
      @brief Destructor.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~TC_MaxDelayPriority( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    TC_MaxDelayPriority & operator = ( const TC_MaxDelayPriority & );


  protected:

    /*!
      @brief clone creates a copy of a TriCore maximum delay scheduling
             priority.

      @return A pointer to the newly created copy of this priority object.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_SchedulingPriority *clone( void ) const override;

};

}       // namespace WIR

#endif  // _TC_MAXDELAYPRIORITY_H
