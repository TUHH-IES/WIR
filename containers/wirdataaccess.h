/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirdataaccess.h
  @brief This file provides the interface of a %WIR container storing data
         data access information.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_DATAACCESS_H
#define _WIR_DATAACCESS_H


//
// Include section
//

// Include standard headers
#include <functional>
#include <set>

// Include WIR headers
#include <wir/wircontainer.h>
#include <wir/wirtypes.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//

class WIR_Data;


/*!
  @brief Class WIR_DataAccess models sets of data objects that are (potentially)
         accessed by a %WIR instruction.

  WIR_DataAccess containers are attached to WIR_Operation objects during code
  selection.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_DataAccess : public WIR_Container<WIR_DataAccess>
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor.

      @param[in] s An R-value reference to a set of accessed data objects to be
                   moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit WIR_DataAccess( WIR_DataSet && );

    /*!
      @brief Destructor.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_DataAccess( void );

    /*!
      @brief isUnique returns whether data access containers are unique, i.e.,
             whether at most one instance of this container type can be attached
             to a %WIR class.

      @return Always true, data access containers are unique.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual bool isUnique( void ) const override;


    //
    // Data object handling.
    //

    /*!
      @brief addData adds a %WIR data object to the set of accessed data
             objects.

      @param[in] d A const reference to a data object to be added.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void addData( const WIR_Data & );

    /*!
      @brief getData returns all data objects of a data access container.

      @return A const reference to the set of all accessed data objects.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    const WIR_DataSet &getData( void ) const;


    //
    // Stream I/O.
    //

    /*!
      @brief The << operator dumps a data access container to an output stream.

      @param[in] os A reference to an output stream.
      @param[in] o A const reference to the data access container to be dumped.
      @return A reference to the same output stream.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    friend std::ostream & operator << ( std::ostream &,
                                        const WIR_DataAccess & );


  private:

    /*!
      @brief mData stores (wrapped) references to all data objects managed by
             this class.
    */
    WIR_DataSet mData;

};

}       // namespace WIR

#endif  // _WIR_DATAACCESS_H
