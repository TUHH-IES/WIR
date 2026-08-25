/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirdataaccess.cc
  @brief This file implements %WIR containers storing data access information.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include standard headers
#include <algorithm>
#include <iostream>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor.
*/
WIR_DataAccess::WIR_DataAccess( WIR_DataSet &&s ) :
  WIR_Container<WIR_DataAccess> {},
  mData { std::move( s ) }
{
  DSTART( "WIR_DataAccess::WIR_DataAccess(WIR_DataSet&&)" );
};


/*
  Destructor.
*/
WIR_DataAccess::~WIR_DataAccess( void )
{
  DSTART( "virtual WIR_DataAccess::~WIR_DataAccess()" );
};


/*
  isUnique returns whether data access containers are unique, i.e., whether at
  most one instance of this container type can be attached to a WIR class.
*/
bool WIR_DataAccess::isUnique( void ) const
{
  DSTART( "virtual bool WIR_DataAccess::isUnique() const" );

  return( true );
};


/*
  addData adds a WIR data object to the set of accessed data objects.
*/
void WIR_DataAccess::addData( const WIR_Data &d )
{
  DSTART( "void WIR_DataAccess::addData(const WIR_Data&)" );

  mData.insert( const_cast<WIR_Data &>( d ) );
};


/*
  getData returns all data objects of a data access container.
*/
const WIR_DataSet &WIR_DataAccess::getData( void ) const
{
  DSTART( "const WIR_DataSet& WIR_DataAccess::getData() const" );

  return( mData );
};


/*
  The << operator dumps a data access container to an output stream.
*/
std::ostream & operator << ( std::ostream &os, const WIR_DataAccess &o )
{
  DSTART( "ostream& operator<<(ostream&, const WIR_DataAccess&)" );

  // Build a comment string.
  string c = "Data Access:";
  c =
    accumulate(
      o.mData.begin(), o.mData.end(), c,
      []( const string &str, const WIR_Data &d ) {
        return( str + " " + d.getName() ); } );

  // Dump the data access container as WIR comment.
  WIR_Registry::getCommentDumper( os.iword( WIR_ProcessorIO() ) )(
    os, WIR_Comment { c } );

  return( os );
};

}       // namespace WIR
