/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file wircomment.cc
  @brief This file implements %WIR containers representing comments.

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

  Any newline characters in the comment text are removed, since this class
  models single-line comments only.
*/
WIR_Comment::WIR_Comment( const std::string &s ) :
  WIR_Container<WIR_Comment> {},
  mCommentText { s }
{
  DSTART( "WIR_Comment::WIR_Comment(const string&)" );

  mCommentText.erase(
    remove( mCommentText.begin(), mCommentText.end(), '\n'),
    mCommentText.end() );
};


/*
  Default constructor.

  Any newline characters in the comment text are removed, since this class
  models single-line comments only.
*/
WIR_Comment::WIR_Comment( std::string &&s ) :
  WIR_Container<WIR_Comment> {},
  mCommentText { std::move( s ) }
{
  DSTART( "WIR_Comment::WIR_Comment(string&&)" );

  mCommentText.erase(
    remove( mCommentText.begin(), mCommentText.end(), '\n'),
    mCommentText.end() );
  s.clear();
};


/*
  Copy constructor.
*/
WIR_Comment::WIR_Comment( const WIR_Comment &__o ) :
  WIR_Container<WIR_Comment> { __o },
  mCommentText { __o.mCommentText }
{
  DSTART( "WIR_Comment::WIR_Comment(const WIR_Comment&)" );
};


/*
  Move constructor.
*/
WIR_Comment::WIR_Comment( WIR_Comment &&__o ) :
  WIR_Container<WIR_Comment> { std::move( __o ) },
  mCommentText { std::move( __o.mCommentText ) }
{
  DSTART( "WIR_Comment::WIR_Comment(WIR_Comment&&)" );

  __o.mCommentText.clear();
};


/*
  Destructor.
*/
WIR_Comment::~WIR_Comment( void )
{
  DSTART( "virtual WIR_Comment::~WIR_Comment()" );
};


/*
  Copy-assignment operator.
*/
WIR_Comment & WIR_Comment::operator = ( const WIR_Comment &__o )
{
  DSTART( "WIR_Comment& WIR_Comment::operator=(const WIR_Comment&)" );

  WIR_Container<WIR_Comment>::operator = ( __o );

  mCommentText = __o.mCommentText;

  return( *this );
};


/*
  Move-assignment operator.
*/
WIR_Comment & WIR_Comment::operator = ( WIR_Comment &&__o )
{
  DSTART( "WIR_Comment& WIR_Comment::operator=(WIR_Comment&&)" );

  WIR_Container<WIR_Comment>::operator = ( std::move( __o ) );

  mCommentText = std::move( __o.mCommentText );
  __o.mCommentText.clear();

  return( *this );
};


/*
  isUnique returns whether comments are unique, i.e., whether at most one
  instance of this container type can be attached to a WIR class.
*/
bool WIR_Comment::isUnique( void ) const
{
  DSTART( "virtual bool WIR_Comment::isUnique() const" );

  return( false );
};


/*
  setText sets a comment's text string.

  Any newline characters in the comment text are removed, since this class
  models single-line comments only.
*/
void WIR_Comment::setText( const std::string &s )
{
  DSTART( "void WIR_Comment::setText(const string&)" );

  mCommentText = s;
  mCommentText.erase(
    remove( mCommentText.begin(), mCommentText.end(), '\n'),
    mCommentText.end() );
};


/*
  setText sets a comment's text string.

  Any newline characters in the comment text are removed, since this class
  models single-line comments only.
*/
void WIR_Comment::setText( std::string &&s )
{
  DSTART( "void WIR_Comment::setText(string&&)" );

  mCommentText = std::move( s );
  mCommentText.erase(
    remove( mCommentText.begin(), mCommentText.end(), '\n'),
    mCommentText.end() );

  s.clear();
};


/*
  getText returns a comment's text.
*/
std::string WIR_Comment::getText( void ) const
{
  DSTART( "string WIR_Comment::getText() const" );

  return( mCommentText );
};


/*
  The << operator dumps a WIR comment to an output stream.

  By applying processor-specific I/O manipulators to the output stream
  beforehand, this << operator can flexibly emit valid assembly output for
  arbitrary processor architectures.
*/
std::ostream & operator << ( std::ostream &os, const WIR_Comment &o )
{
  DSTART( "ostream& operator<<(ostream&, const WIR_Comment&)" );

  WIR_Registry::getCommentDumper( os.iword( WIR_ProcessorIO() ) )( os, o );

  return( os );
};

}       // namespace WIR
