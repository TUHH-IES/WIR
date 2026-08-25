/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirprocessor.h
  @brief This file provides the interface of generic %WIR processors.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_PROCESSOR_H
#define _WIR_PROCESSOR_H


//
// Include section
//

// Include standard headers
#include <iostream>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/API/wiridapi.h>
#include <wir/wirbaseprocessor.h>


//
// Header section
//

namespace WIR {

/*!
  @brief Class WIR_Processor models a generic %WIR processor architecture.

  WIR_Processor adds functionality for statically and uniquely assigning
  processor type IDs. This way, processors of different types can unambiguously
  be distinguished.

  WIR_Processor serves as virtual base class from which actual processors are
  derived. For this purpose, every processor type must inherit from
  WIR_Processor in the following way:

  class TC179x : public WIR_Processor<TC179x>
  {
    ...
  };

  This creates a template instantiation of WIR_Processor that adds a static
  field for the processor type ID of 'TC179x'. Furthermore, static initializers
  are added to TC179x that fill this field with a valid value allocated from
  class WIR_BaseProcessor via a call to method registerNewProcessorType.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
template<typename DerivedProcessorClass>
class WIR_Processor : public WIR_BaseProcessor
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor creating a processor and registering its type.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_Processor( void ) :
      WIR_BaseProcessor {}
    {
      DSTART( "WIR_Processor<DerivedProcessorClass>::WIR_Processor()" );

      // The following call is important, see also comments in
      // WIR_BaseProcessor::Registrator::touch.
      mRegistrator.touch();
    };

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_Processor( const WIR_Processor &__o ) :
      WIR_BaseProcessor { __o }
    {
      DSTART(
        "WIR_Processor<DerivedProcessorClass>::WIR_Processor(const WIR_Processor<DerivedProcessorClass>&)" );
    };

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_Processor( WIR_Processor &&__o ) :
      WIR_BaseProcessor { std::move( __o ) }
    {
      DSTART(
        "WIR_Processor<DerivedProcessorClass>::WIR_Processor("
        "WIR_Processor<DerivedProcessorClass>&&)" );
    };

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_Processor( void )
    {
      DSTART( "WIR_Processor<DerivedProcessorClass>::~WIR_Processor()" );
    };

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_Processor & operator = ( const WIR_Processor &__o )
    {
      DSTART(
        "WIR_Processor<DerivedProcessorClass>& "
        "WIR_Processor<DerivedProcessorClass>::operator=(const "
        "WIR_Processor<DerivedProcessorClass>&)" );

      WIR_BaseProcessor::operator = ( __o );

      return( *this );
    };

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_Processor & operator = ( WIR_Processor &&__o )
    {
      DSTART(
        "WIR_Processor<DerivedProcessorClass>& "
        "WIR_Processor<DerivedProcessorClass>::operator=("
        "WIR_Processor<DerivedProcessorClass>&&)" );

      WIR_BaseProcessor::operator = ( std::move( __o ) );

      return( *this );
    };


    //
    // Processor type handling.
    //

    /*!
      @brief getProcessorTypeID returns the ID of a processor type.

      @return The processor type's ID.

      Since getProcessorTypeID is a static method, it can be used to query types
      for derived processor classes without having actual objects of such a
      processor class.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    static WIR_id_t getProcessorTypeID( void )
    {
      DSTART(
        "static WIR_id_t WIR_Processor<DerivedProcessorClass>::getProcessorTypeID()" );

      return( mProcessorID );
    };

    /*!
      @brief getProcessorType returns the ID of a processor type.

      @return The processor type's ID.

      getProcessorType can be used to query types for actual processor objects.
      Please note the difference to method getProcessorTypeID above that serves
      the same purpose but that is a static method.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_id_t getProcessorType( void ) const override
    {
      DSTART(
        "WIR_id_t WIR_Processor<DerivedProcessorClass>::getProcessorType() "
        "const" );

      return( mProcessorID );
    };


  private:

    /*!
      @brief clone creates a copy of a %WIR processor.

      @return A pointer to the newly created copy of this processor.

      Using the template argument, clone just calls the correct copy constructor
      of the actual processor class.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_BaseProcessor *clone( void ) const override
    {
      DSTART(
        "WIR_BaseProcessor* WIR_Processor<DerivedProcessorClass>::clone() const" );

      return(
        new DerivedProcessorClass(
          *( static_cast<const DerivedProcessorClass *>( this ) ) ) );
    };

    //! mProcessorID holds the statically initialized processor type ID.
    static WIR_id_t mProcessorID;

    //! mRegistrator is used to initialize mProcessorID via its constructor.
    static WIR_BaseProcessor::Registrator mRegistrator;

};

template<typename DerivedProcessorClass>
WIR_id_t WIR_Processor<DerivedProcessorClass>::mProcessorID;

template<typename DerivedProcessorClass>
WIR_BaseProcessor::Registrator WIR_Processor<DerivedProcessorClass>::mRegistrator( DerivedProcessorClass::mProcessorID );

}       // namespace WIR

#endif  // _WIR_PROCESSOR_H
