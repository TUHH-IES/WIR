/*

  This header file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wirstringparameter.h
  @brief This file provides the interface of parameters that contain simple,
         unstructured strings.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


#ifndef _WIR_STRINGPARAMETER_H
#define _WIR_STRINGPARAMETER_H


//
// Include section
//

// Include standard headers
#include <ostream>
#include <string>

// Include WIR headers
#include <wir/wirparameter.h>


//
// Header section
//

namespace WIR {

//
// Class forward declarations
//


/*!
  @brief Class WIR_StringParameter is the representation of unstructured string
         parameters.

  @note The use of such unstructured parameters should be avoided whenever
        possible, since any information stored in a string will not be
        considered by %WIR analyses and optimizations.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/
class WIR_StringParameter : public WIR_Parameter
{

  public:

    //
    // Constructors and destructors.
    //

    /*!
      @brief Default constructor for string parameters.

      @param[in] s A const reference to the parameter's string.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit WIR_StringParameter( const std::string & );

    /*!
      @brief Default constructor for string parameters.

      @param[in] s An R-value reference to the parameter's string.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    explicit WIR_StringParameter( std::string && );

    /*!
      @brief Copy constructor.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_StringParameter( const WIR_StringParameter & );

    /*!
      @brief Move constructor.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_StringParameter( WIR_StringParameter && );

    /*!
      @brief Destructor.
      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual ~WIR_StringParameter( void );

    /*!
      @brief Copy-assignment operator.

      @param[in] __o A const reference to another object to be copied.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_StringParameter & operator = ( const WIR_StringParameter & );

    /*!
      @brief Move-assignment operator.

      @param[in] __o An R-value reference to another object to be moved.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    WIR_StringParameter & operator = ( WIR_StringParameter && );


    //
    // Generic type handling.
    //

    /*!
      @brief getType returns the type of a %WIR parameter, i.e., that it is a
             string parameter.

      @return WIR_ParameterType::str

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_ParameterType getType( void ) const override;


    //
    // String value handling.
    //

    /*!
      @brief setString sets a parameter's string.

      @param[in] s A const reference to the parameter's new string value.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void setString( const std::string & );

    /*!
      @brief setString sets a parameter's string.

      @param[in] s An R-value reference to the parameter's new string value.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    void setString( std::string && );

    /*!
      @brief getString returns a parameter's string.

      @return The parameter's string.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    std::string getString( void ) const;


    //
    // Stream I/O.
    //

    /*!
      @brief The << operator dumps a %WIR string parameter to an output stream.

      @param[in] os A reference to an output stream.
      @param[in] p A const reference to the %WIR string parameter to be dumped.
      @return A reference to the same output stream.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    friend std::ostream & operator << ( std::ostream &,
                                        const WIR_StringParameter & );


  protected:

    /*!
      @brief clone creates a copy of a string parameter.

      @return A pointer to the newly created copy of this parameter.

      Clone just calls the corresponding copy constructor.

      @author Heiko Falk <Heiko.Falk@tuhh.de>
    */
    virtual WIR_Parameter *clone( void ) const override;


  private:

    /*!
      @brief No standard construction allowed, users must use
             WIR_StringParameter( const std::string & ) instead.
    */
    WIR_StringParameter( void ) = delete;

    //! mString stores the parameter's string.
    std::string mString;

};

}       // namespace WIR

#endif  // _WIR_STRINGPARAMETER_H
