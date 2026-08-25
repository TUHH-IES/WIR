/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirtypes.cc
  @brief This file implements several simple basic data types that are used here
         and there within the %WIR library.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>


//
// Code section
//

namespace WIR {


using namespace std;


/*
  WIR_Compare_Pair is a comparator class that is used to sort sets of pairs of
  WIR classes uniquely.
*/
bool WIR_Compare_Pair::operator()( const std::pair<std::reference_wrapper<WIR_ID_API>,
                                                   std::reference_wrapper<WIR_ID_API>> &lhs,
                                   const std::pair<std::reference_wrapper<WIR_ID_API>,
                                                   std::reference_wrapper<WIR_ID_API>> &rhs ) const
{
  return(
    ( lhs.first.get().getID() < rhs.first.get().getID() ) ? true :
      ( lhs.first.get().getID() > rhs.first.get().getID() ) ? false :
        lhs.second.get().getID() < rhs.second.get().getID() );
};


/*
  This operator performs a less-than comparison of two WIR_MemoryRegions.
*/
bool operator < ( const WIR_MemoryRegion &lhs, const WIR_MemoryRegion &rhs )
{
  DSTART( "bool operator<(const WIR_MemoryRegion&, const WIR_MemoryRegion&)" );

  return( lhs.getBaseAddress() < rhs.getBaseAddress() );
};


/*
  WIR_Compare_MemoryRegions is a comparator class that is used to sort, e.g.,
  sets of WIR memory regions uniquely by their base addresses.
*/
bool WIR_Compare_MemoryRegions::operator()( const reference_wrapper<WIR_MemoryRegion> &lhs,
                                            const reference_wrapper<WIR_MemoryRegion> &rhs ) const
{
  return(
    ( lhs.get().getBaseAddress() < rhs.get().getBaseAddress() ) ? true :
      ( lhs.get().getBaseAddress() > rhs.get().getBaseAddress() ) ? false :
        ( lhs.get().getID() < rhs.get().getID() ) );
};


/*
  This operator compares two WIR_RegisterSets for equality.
*/
bool operator == ( const WIR_RegisterSet &lhs, const WIR_RegisterSet &rhs )
{
  DSTART( "bool operator==(const WIR_RegisterSet&, const WIR_RegisterSet&)" );

  if ( lhs.size() != rhs.size() )
    return( false );

  auto it1 = lhs.begin();
  auto it2 = rhs.begin();
  for ( ; it1 != lhs.end(); ++it1, ++it2 )
    if ( (*it1).get() != (*it2).get() )
      return( false );

  return( true );
};


/*
  This operator compares two WIR_RegisterSets for inequality.
*/
bool operator != ( const WIR_RegisterSet &lhs, const WIR_RegisterSet &rhs )
{
  DSTART( "bool operator!=(const WIR_RegisterSet&, const WIR_RegisterSet&)" );

  return( !( lhs == rhs ) );
};

}       // namespace WIR
