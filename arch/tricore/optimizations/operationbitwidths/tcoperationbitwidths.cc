/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tcoperationbitwidths.cc
  @brief This file implements a TriCore-specific optimization reducing the bit
         widths of machine operations.

  @author Heiko Falk <Heiko.Falk@tuhh.de>
*/


//
// Include section
//

#ifdef HAVE_CONFIG_H
#include <config_wir.h>
#endif

// Include boost headers
#include <boost/current_function.hpp>

// Include libuseful headers
#include <libuseful/debugmacros.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/tricore/tc131.h>

// Include local headers
#include "tcoperationbitwidths.h"


//
// Code section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for system-level optimization.
*/
TC_OperationBitWidths::TC_OperationBitWidths( WIR_System &s ) :
  WIR_OperationBitWidths { s }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for compilation unit-level optimization.
*/
TC_OperationBitWidths::TC_OperationBitWidths( WIR_CompilationUnit &c ) :
  WIR_OperationBitWidths { c }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Default constructor for function-level optimization.
*/
TC_OperationBitWidths::TC_OperationBitWidths( WIR_Function &f ) :
  WIR_OperationBitWidths { f }
{
  DSTART( "TC_OperationBitWidths::TC_OperationBitWidths(WIR_Function&)" );
};


/*
  Destructor.
*/
TC_OperationBitWidths::~TC_OperationBitWidths( void )
{
  DSTART( "virtual TC_OperationBitWidths::~TC_OperationBitWidths()" );
};


//
// Protected class methods
//

/*
  reduce reduces the bit width of the given TriCore machine operation.

  Bit widths of branches are reduced whenever possible and without proper
  consideration of jump displacements. The correction of branches with incorrect
  displacements thus has to be done elsewhere afterwards.
*/
std::list<
  std::reference_wrapper<
    WIR_Operation>>::iterator TC_OperationBitWidths::reduce( WIR_Instruction &i,
                                                             std::list<
                                                               std::reference_wrapper<
                                                                 WIR_Operation>>::const_iterator pos ) const
{
  DSTART(
    "virtual list<reference_wrapper<WIR_Operation> >::iterator "
    "TC_OperationBitWidths::reduce(WIR_Instruction&, "
    "list<reference_wrapper<WIR_Operation> >::const_iterator) const" );

  const WIR_Operation &o = pos->get();

  // This lambda is used to determine whether the i-th parameter of operation o
  // is the post-increment addressing mode.
  auto isPostIncr = [&]( unsigned int i ) {
    const auto &p = nthParam( o, i );
    if ( p.getType() != WIR_ParameterType::addr )
      return( false );
    return(
      dynamic_cast<const WIR_AddressingModeParameter &>(
        p ).getAddressingMode() ==
        TC13::AddressingMode::post );
  };

  if ( ( o.getOpCode() == TC13::OpCode::ADD_A ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::AAA ) ) {
    if ( nthReg( o, 0 ) == nthReg( o, 1 ) )
      // add.a Aa, Aa, Ab         ->      add.a Aa, Ab
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SAA_5,
            new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
            nthParam( o, 2 ) } ) );
    else

    if ( nthReg( o, 0 ) == nthReg( o, 2 ) )
      // add.a Aa, Ab, Aa         ->      add.a Aa, Ab
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SAA_5,
            new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
            nthParam( o, 1 ) } ) );
  } else

  if ( ( ( o.getOpCode() == TC13::OpCode::ADD ) ||
         ( o.getOpCode() == TC13::OpCode::ADDS ) ||
         ( o.getOpCode() == TC13::OpCode::AND ) ||
         ( o.getOpCode() == TC13::OpCode::MUL ) ||
         ( o.getOpCode() == TC13::OpCode::OR ) ||
         ( o.getOpCode() == TC13::OpCode::SUB ) ||
         ( o.getOpCode() == TC13::OpCode::SUBS ) ||
         ( o.getOpCode() == TC13::OpCode::XOR ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DDD_1 ) ) {
    if ( nthReg( o, 0 ) == nthReg( o, 1 ) )
      // op Da, Da, Db            ->      op Da, Db
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SDD_2,
            new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
            nthParam( o, 2 ) } ) );
    else

    if ( ( o.getOpCode() != TC13::OpCode::SUB ) &&
         ( o.getOpCode() != TC13::OpCode::SUBS ) &&
         ( nthReg( o, 0 ) == nthReg( o, 2 ) ) )
      // op Da, Db, Da            ->      op Da, Db
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SDD_2,
            new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
            nthParam( o, 1 ) } ) );
    else

    if ( ( o.getOpCode() == TC13::OpCode::ADD ) &&
         ( TC13::isD15( nthReg( o, 1 ) ) || TC13::isD15( nthReg( o, 2 ) ) ) )
      // add Da, D15, Db            ->      add Da, D15, Db (16)
      // add Da, Db, D15            ->      add Da, D15, Db (16)
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SDID_1,
            nthParam( o, 0 ),
            TC13::isD15( nthReg( o, 1 ) ) ? nthParam( o, 1 ) : nthParam( o, 2 ),
            TC13::isD15( nthReg( o, 1 ) ) ?
              nthParam( o, 2 ) : nthParam( o, 1 ) } ) );
    else

    if ( ( ( o.getOpCode() == TC13::OpCode::ADD ) ||
           ( o.getOpCode() == TC13::OpCode::SUB ) ) &&
         TC13::isD15( nthReg( o, 0 ) ) )
      // op D15, Da, Db             ->      op D15, Da, Db (16)
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SIDD,
            nthParam( o, 0 ), nthParam( o, 1 ), nthParam( o, 2 ) } ) );
  } else

  if ( ( ( o.getOpCode() == TC13::OpCode::ADD ) ||
         ( o.getOpCode() == TC13::OpCode::SH ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DDC9_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       ( nthSVal( o, 2 ) >= TC_Const16_Signed::getMinValue( 4 ) ) &&
       ( nthSVal( o, 2 ) <= TC_Const16_Signed::getMaxValue( 4 ) ) )
    // op Da, Da, c               ->      op Da, c
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SDC4_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          new TC_Const4_Signed( nthSVal( o, 2 ) ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::SHA ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DDC9PSW_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       ( nthSVal( o, 2 ) >= TC_Const16_Signed::getMinValue( 4 ) ) &&
       ( nthSVal( o, 2 ) <= TC_Const16_Signed::getMaxValue( 4 ) ) )
    // op Da, Da, c               ->      op Da, c
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SDC4PSW,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          new TC_Const4_Signed( nthSVal( o, 2 ) ),
          new WIR_RegisterParameter( nthReg( o, 3 ), WIR_Usage::def ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::EQ ) ||
         ( o.getOpCode() == TC13::OpCode::LT ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DDD_1 ) &&
       TC13::isD15( nthReg( o, 0 ) ) )
    // op D15, Da, Db             ->      op D15, Da, Db (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SIDD,
          nthParam( o, 0 ), nthParam( o, 1 ), nthParam( o, 2 ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::ADD ) ||
         ( o.getOpCode() == TC13::OpCode::EQ ) ||
         ( o.getOpCode() == TC13::OpCode::LT ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DDC9_1 ) &&
       TC13::isD15( nthReg( o, 0 ) ) &&
       ( nthSVal( o, 2 ) >= TC_Const16_Signed::getMinValue( 4 ) ) &&
       ( nthSVal( o, 2 ) <= TC_Const16_Signed::getMaxValue( 4 ) ) )
    // op D15, Da, c              ->      op D15, Da, c (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SIDC4,
          nthParam( o, 0 ), nthParam( o, 1 ),
          new TC_Const4_Signed( nthSVal( o, 2 ) ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::ADD ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DDC9_1 ) &&
       TC13::isD15( nthReg( o, 1 ) ) &&
       ( nthSVal( o, 2 ) >= TC_Const16_Signed::getMinValue( 4 ) ) &&
       ( nthSVal( o, 2 ) <= TC_Const16_Signed::getMaxValue( 4 ) ) )
    // add Da, D15, c             ->      add Da, D15, c (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SDIC4_2,
          nthParam( o, 0 ), nthParam( o, 1 ),
          new TC_Const4_Signed( nthSVal( o, 2 ) ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::ADDSC_A ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::AADC2 ) &&
       TC13::isD15( nthReg( o, 2 ) ) )
    // addsc.a Aa, Ab, D15, c     ->      addsc.a Aa, Ab, D15, c (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SAAIC2,
          nthParam( o, 0 ), nthParam( o, 1 ), nthParam( o, 2 ),
          nthParam( o, 3 ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::AND ) ||
         ( o.getOpCode() == TC13::OpCode::OR ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DDC9_2 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       TC13::isD15( nthReg( o, 0 ) ) &&
       ( nthUVal( o, 2 ) <= TC_Const16_Unsigned::getMaxValue( 8 ) ) )
    // op D15, D15, c             ->      op D15, c
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SIC8_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          new TC_Const8_Unsigned( nthUVal( o, 2 ) ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::BISR ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::C9 ) &&
       ( nthUVal( o, 0 ) <= TC_Const16_Unsigned::getMaxValue( 8 ) ) )
    // bisr c                     ->      bisr c (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SC8,
          new TC_Const8_Unsigned( nthUVal( o, 0 ) ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::CADD ) ||
         ( o.getOpCode() == TC13::OpCode::CADDN ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DDDC9_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 2 ) ) && TC13::isD15( nthReg( o, 1 ) ) &&
       ( nthSVal( o, 3 ) >= TC_Const16_Signed::getMinValue( 4 ) ) &&
       ( nthSVal( o, 3 ) <= TC_Const16_Signed::getMaxValue( 4 ) ) )
    // op Da, D15, Da, c          ->      op Da, D15, c
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SDIC4_3,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 1 ),
          new TC_Const4_Signed( nthSVal( o, 3 ) ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::DEBUG ) ||
         ( o.getOpCode() == TC13::OpCode::NOP ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::SYS ) )
    // op                         ->      op (16)
    return(
      replace( pos, { o.getOpCode(), TC13::OperationFormat::S } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::RET ) ||
         ( o.getOpCode() == TC13::OpCode::RFE ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::PSW ) )
    // op                         ->      op (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SPSW,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::def ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::J ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::L ) )
    // j disp24                   ->      j disp8 (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SL, nthParam( o, 0 ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::JEQ ) ||
         ( o.getOpCode() == TC13::OpCode::JNE ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DC4L_1 ) &&
       TC13::isD15( nthReg( o, 0 ) ) && ( nthSVal( o, 1 ) == 0 ) )
    // jeq D15, 0, disp15         ->      jz D15, disp8 (16)
    // jne D15, 0, disp15         ->      jnz D15, disp8 (16)
    return(
      replace(
        pos,
        { ( o.getOpCode() == TC13::OpCode::JEQ ) ?
            TC13::OpCode::JZ : TC13::OpCode::JNZ,
          TC13::OperationFormat::SIL, nthParam( o, 0 ), nthParam( o, 2 ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::JEQ ) ||
         ( o.getOpCode() == TC13::OpCode::JNE ) ) &&
       ( ( o.getOperationFormat() == TC13::OperationFormat::DC4L_1 ) ||
         ( o.getOperationFormat() == TC13::OperationFormat::DDL_1 ) ) &&
       TC13::isD15( nthReg( o, 0 ) ) )
    // jeq D15, Da, disp15        ->      jeq D15, Da, disp4 (16)
    // jeq D15, c, disp15         ->      jeq D15, c, disp4 (16)
    // jne D15, Da, disp15        ->      jne D15, Da, disp4 (16)
    // jne D15, c, disp15         ->      jne D15, c, disp4 (16)
    return(
      replace(
        pos,
        { o.getOpCode(),
          ( o.getOperationFormat() == TC13::OperationFormat::DDL_1 ) ?
            TC13::OperationFormat::SIDL : TC13::OperationFormat::SIC4L,
          nthParam( o, 0 ), nthParam( o, 1 ), nthParam( o, 2 ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::JEQ ) ||
         ( o.getOpCode() == TC13::OpCode::JNE ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DC4L_1 ) &&
       ( nthSVal( o, 1 ) == 0 ) )
    // jeq Da, 0, disp15          ->      jz Da, disp4 (16)
    // jne Da, 0, disp15          ->      jnz Da, disp4 (16)
    return(
      replace(
        pos,
        { ( o.getOpCode() == TC13::OpCode::JEQ ) ?
            TC13::OpCode::JZ : TC13::OpCode::JNZ,
          TC13::OperationFormat::SDL, nthParam( o, 0 ), nthParam( o, 2 ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::JGE ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DC4L_1 ) &&
       ( ( nthSVal( o, 1 ) == 0 ) || ( nthSVal( o, 1 ) == 1 ) ) )
    // jge Da, 0, disp15          ->      jgez Da, disp4 (16)
    // jge Da, 1, disp15          ->      jgtz Da, disp4 (16)
    return(
      replace(
        pos,
        { ( nthSVal( o, 1 ) == 0 ) ? TC13::OpCode::JGEZ : TC13::OpCode::JGTZ,
          TC13::OperationFormat::SDL, nthParam( o, 0 ), nthParam( o, 2 ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::JI ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::A ) )
    // ji Aa                      ->      ji Aa (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SA, nthParam( o, 0 ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::JLT ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DC4L_1 ) &&
       ( ( nthSVal( o, 1 ) == 0 ) || ( nthSVal( o, 1 ) == 1 ) ) )
    // jlt Da, 0, disp15          ->      jltz Da, disp4 (16)
    // jlt Da, 1, disp15          ->      jlez Da, disp4 (16)
    return(
      replace(
        pos,
        { ( nthSVal( o, 1 ) == 0 ) ? TC13::OpCode::JLTZ : TC13::OpCode::JLEZ,
          TC13::OperationFormat::SDL, nthParam( o, 0 ), nthParam( o, 2 ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::JNZ_A ) ||
         ( o.getOpCode() == TC13::OpCode::JZ_A ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::AL_2 ) )
    // jnz.a Aa, disp15           ->      jnz.a Aa, disp4 (16)
    // jz.a Aa, disp15            ->      jz.a Aa, disp4 (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SAL_1,
          nthParam( o, 0 ), nthParam( o, 1 ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::JNZ_T ) ||
         ( o.getOpCode() == TC13::OpCode::JZ_T ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DC5L ) &&
       ( TC13::isD15( nthReg( o, 0 ) ) ) )
    // jnz.t D15, c, disp15       ->      jnz.t D15, c, disp4 (16)
    // jz.t D15, c, disp15        ->      jz.t D15, c, disp4 (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SIC5L,
          nthParam( o, 0 ), nthParam( o, 1 ), nthParam( o, 2 ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::LD_A ) &&
       ( ( o.getOperationFormat() == TC13::OperationFormat::AAC10BOA ) ||
         ( o.getOperationFormat() == TC13::OperationFormat::AAC16BOA ) ) ) {
    if ( nthSVal( o, 2 ) == 0 )
      // ld.a Aa, [Ab]0           ->      ld.a Aa, [Ab]
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SAA_2,
            nthParam( o, 0 ), nthParam( o, 1 ) } ) );
    else

    if ( ( TC13::isA15( nthReg( o, 0 ) ) || TC13::isA15( nthReg( o, 1 ) ) ) &&
         ( nthSVal( o, 2 ) >= 0 ) &&
         ( nthSVal( o, 2 ) <=
             (long long) TC_Const16_Unsigned::getMaxValue( 4 ) ) )
      // ld.a A15, [Aa]c          ->      ld.a A15, [Aa]c (16)
      // ld.a Aa, [A15]c          ->      ld.a Aa, [A15]c (16)
      return(
        replace(
          pos,
          { o.getOpCode(),
            TC13::isA15( nthReg( o, 0 ) ) ?
              TC13::OperationFormat::SIAC4_1 : TC13::OperationFormat::SAIC4,
            nthParam( o, 0 ), nthParam( o, 1 ),
            new TC_Const4_Unsigned( (long long) nthSVal( o, 2 ) ) } ) );
    else

    if ( TC13::isA15( nthReg( o, 0 ) ) && TC13::isSP( nthReg( o, 1 ) ) &&
         ( nthSVal( o, 2 ) >= 0 ) &&
         ( nthSVal( o, 2 ) <=
             (long long) TC_Const16_Unsigned::getMaxValue( 10 ) ) &&
         ( nthSVal( o, 2 ) % 4 == 0 ) )
      // ld.a A15, [A10]c         ->      ld.a A15, [A10]c (16)
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SISPC10_1,
            nthParam( o, 0 ), nthParam( o, 1 ),
            new TC_Const10_Unsigned( (long long) nthSVal( o, 2 ) ) } ) );
  } else

  if ( ( o.getOpCode() == TC13::OpCode::LD_A ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::AAC10PIA ) &&
       isPostIncr( 1 ) && ( nthSVal( o, 3 ) == 4 ) )
    // ld.a Aa, [Ab+]4            ->      ld.a Aa, [Ab+]
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SAA_3,
          nthParam( o, 0 ), nthParam( o, 2 ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::LD_BU ) ||
         ( o.getOpCode() == TC13::OpCode::LD_H ) ||
         ( o.getOpCode() == TC13::OpCode::LD_W ) ) &&
       ( ( o.getOperationFormat() == TC13::OperationFormat::DAC10BOA ) ||
         ( o.getOperationFormat() == TC13::OperationFormat::DAC16BOA ) ) ) {
    if ( nthSVal( o, 2 ) == 0 )
      // ld.x Da, [Ab]0           ->      ld.x Da, [Ab]
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SDA_2,
            nthParam( o, 0 ), nthParam( o, 1 ) } ) );
    else

    if ( ( TC13::isD15( nthReg( o, 0 ) ) || TC13::isA15( nthReg( o, 1 ) ) ) &&
         ( nthSVal( o, 2 ) >= 0 ) &&
         ( nthSVal( o, 2 ) <=
             (long long) TC_Const16_Unsigned::getMaxValue( 4 ) ) )
      // ld.x D15, [Aa]c          ->      ld.x D15, [Aa]c (16)
      // ld.x Da, [A15]c          ->      ld.x Da, [A15]c (16)
      return(
        replace(
          pos,
          { o.getOpCode(),
            TC13::isD15( nthReg( o, 0 ) ) ?
              TC13::OperationFormat::SIAC4_2 : TC13::OperationFormat::SDIC4_1,
            nthParam( o, 0 ), nthParam( o, 1 ),
            new TC_Const4_Unsigned( (long long) nthSVal( o, 2 ) ) } ) );
    else

    if ( ( o.getOpCode() == TC13::OpCode::LD_W ) &&
         TC13::isD15( nthReg( o, 0 ) ) && TC13::isSP( nthReg( o, 1 ) ) &&
         ( nthSVal( o, 2 ) >= 0 ) &&
         ( nthSVal( o, 2 ) <=
             (long long) TC_Const16_Unsigned::getMaxValue( 10 ) ) &&
         ( nthSVal( o, 2 ) % 4 == 0 ) )
      // ld.w D15, [A10]c         ->      ld.w D15, [A10]c (16)
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SISPC10_2,
            nthParam( o, 0 ), nthParam( o, 1 ),
            new TC_Const10_Unsigned( (long long) nthSVal( o, 2 ) ) } ) );
  } else

  if ( ( o.getOperationFormat() == TC13::OperationFormat::DAC10PIA ) &&
       isPostIncr( 1 ) ) {
    if ( ( ( o.getOpCode() == TC13::OpCode::LD_BU ) && nthSVal( o, 3 ) == 1 ) ||
         ( ( o.getOpCode() == TC13::OpCode::LD_H ) && nthSVal( o, 3 ) == 2 ) ||
         ( ( o.getOpCode() == TC13::OpCode::LD_W ) && nthSVal( o, 3 ) == 4 ) )
      // ld.x Da, [Ab+]c          ->      ld.x Da, [Ab+]
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SDA_3,
            nthParam( o, 0 ), nthParam( o, 2 ) } ) );
  } else

  if ( o.getOpCode() == TC13::OpCode::LEA ) {
    if ( ( o.getOperationFormat() == TC13::OperationFormat::AC18ABSA ) &&
         ( nthUVal( o, 1 ) <= TC_Const16_Unsigned::getMaxValue( 4 ) ) )
      // lea Aa, c                ->      mov.a Aa, c
      return(
        replace(
          pos,
          { TC13::OpCode::MOV_A, TC13::OperationFormat::SAC4_1,
            nthParam( o, 0 ), new TC_Const4_Unsigned( nthUVal( o, 1 ) ) } ) );
    else

    if ( ( ( o.getOperationFormat() == TC13::OperationFormat::AAC10BOA ) ||
           ( o.getOperationFormat() == TC13::OperationFormat::AAC16BOA ) ) &&
         TC13::isSP( nthReg( o, 0 ) ) && TC13::isSP( nthReg( o, 1 ) ) &&
         ( nthSVal( o, 2 ) <= 0 ) &&
         ( -nthSVal( o, 2 ) <=
             (long long) TC_Const16_Unsigned::getMaxValue( 8 ) ) )
      // lea A10, [A10]-c         ->      sub.a A10, c
      return(
        replace(
          pos,
          { TC13::OpCode::SUB_A, TC13::OperationFormat::SSPC8,
            new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
            new TC_Const8_Unsigned( -nthSVal( o, 2 ) ) } ) );
    else

    if ( ( ( o.getOperationFormat() == TC13::OperationFormat::AAC10BOA ) ||
           ( o.getOperationFormat() == TC13::OperationFormat::AAC16BOA ) ) &&
         ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
         ( nthSVal( o, 2 ) >= TC_Const16_Signed::getMinValue( 4 ) ) &&
         ( nthSVal( o, 2 ) <= TC_Const16_Signed::getMaxValue( 4 ) ) )
      // lea Aa, [Aa]c            ->      add.a Aa, c
      return(
        replace(
          pos,
          { TC13::OpCode::ADD_A, TC13::OperationFormat::SAC4_2,
            new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
            new TC_Const4_Signed( nthSVal( o, 2 ) ) } ) );
  } else

  if ( ( o.getOpCode() == TC13::OpCode::MOV ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DC16_1 ) ) {
    if ( TC13::isD15( nthReg( o, 0 ) ) &&
         ( nthSVal( o, 1 ) >= 0 ) &&
         ( nthSVal( o, 1 ) <=
             (long long) TC_Const16_Unsigned::getMaxValue( 8 ) ) )
      // mov D15, c               ->      mov D15, c (16)
      return(
        replace(
          pos,
          { TC13::OpCode::MOV, TC13::OperationFormat::SIC8_1,
            nthParam( o, 0 ),
            new TC_Const8_Unsigned( nthSVal( o, 1 ) ) } ) );
    else

    if ( ( nthSVal( o, 1 ) >= TC_Const16_Signed::getMinValue( 4 ) ) &&
         ( nthSVal( o, 1 ) <= TC_Const16_Signed::getMaxValue( 4 ) ) )
      // mov Da, c                ->      mov Da, c (16)
      return(
        replace(
          pos,
          { TC13::OpCode::MOV, TC13::OperationFormat::SDC4_1,
            nthParam( o, 0 ),
            new TC_Const4_Signed( nthSVal( o, 1 ) ) } ) );
  } else

  if ( ( o.getOpCode() == TC13::OpCode::MOV_RR ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DD ) )
    // mov Da, Db                 ->      mov Da, Db (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SDD_1,
          nthParam( o, 0 ), nthParam( o, 1 ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::MOV_A ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::AD ) )
    // mov.a Aa, Db               ->      mov.a Aa, Db (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SAD_1,
          nthParam( o, 0 ), nthParam( o, 1 ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::MOV_AA ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::AA ) )
    // mov.aa Aa, Ab              ->      mov.aa Aa, Ab (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SAA_1,
          nthParam( o, 0 ), nthParam( o, 1 ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::MOV_D ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DA ) )
    // mov.d Da, Ab               ->      mov.d Da, Ab (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SDA_1,
          nthParam( o, 0 ), nthParam( o, 1 ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::RSUB ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DDC9_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) &&
       ( nthSVal( o, 2 ) == 0 ) )
    // rsub Da, Da, 0             ->      rsub Da
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SD,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::SAT_B ) ||
         ( o.getOpCode() == TC13::OpCode::SAT_BU ) ||
         ( o.getOpCode() == TC13::OpCode::SAT_H ) ||
         ( o.getOpCode() == TC13::OpCode::SAT_HU ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DD ) &&
       ( nthReg( o, 0 ) == nthReg( o, 1 ) ) )
    // op Da, Da                  ->      op Da
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SD,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ) } ) );
  else

  if ( ( o.getOpCode() == TC13::OpCode::ST_A ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::AC10ABOA ) ) {
    if ( nthSVal( o, 1 ) == 0 )
      // st.a [Aa]0, Ab           ->      st.a [Aa], Ab
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SAA_4,
            nthParam( o, 0 ), nthParam( o, 2 ) } ) );
    else

    if ( ( TC13::isA15( nthReg( o, 0 ) ) || TC13::isA15( nthReg( o, 2 ) ) ) &&
         ( nthSVal( o, 1 ) >= 0 ) &&
         ( nthSVal( o, 1 ) <=
             (long long) TC_Const16_Unsigned::getMaxValue( 4 ) ) )
      // st.a [A15]c, Aa          ->      st.a [A15]c, Aa (16)
      // st.a [Aa]c, A15          ->      st.a [Aa]c, A15 (16)
      return(
        replace(
          pos,
          { o.getOpCode(),
            TC13::isA15( nthReg( o, 0 ) ) ?
              TC13::OperationFormat::SIC4A : TC13::OperationFormat::SAC4I_1,
            nthParam( o, 0 ),
            new TC_Const4_Unsigned( (long long) nthSVal( o, 1 ) ),
            nthParam( o, 2 ) } ) );
    else

    if ( TC13::isSP( nthReg( o, 0 ) ) && TC13::isA15( nthReg( o, 2 ) ) &&
         ( nthSVal( o, 1 ) >= 0 ) &&
         ( nthSVal( o, 1 ) <=
             (long long) TC_Const16_Unsigned::getMaxValue( 10 ) ) &&
         ( nthSVal( o, 1 ) % 4 == 0 ) )
      // st.a [A10]c, A15         ->      st.a [A10]c, A15 (16)
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SSPC10I_1,
            nthParam( o, 0 ),
            new TC_Const10_Unsigned( (long long) nthSVal( o, 1 ) ),
            nthParam( o, 2 ) } ) );
  } else

  if ( ( o.getOpCode() == TC13::OpCode::ST_A ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::AC10APIA ) &&
       isPostIncr( 0 ) && ( nthSVal( o, 2 ) == 4 ) )
    // st.a [Aa+]4, Ab            ->      st.a [Aa+], Ab
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SAA_6,
          nthParam( o, 1 ), nthParam( o, 3 ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::ST_B ) ||
         ( o.getOpCode() == TC13::OpCode::ST_H ) ||
         ( o.getOpCode() == TC13::OpCode::ST_W ) ) &&
       ( ( o.getOperationFormat() == TC13::OperationFormat::AC10DBOA_1 ) ||
         ( o.getOperationFormat() == TC13::OperationFormat::AC16DBOA ) ) ) {
    if ( nthSVal( o, 1 ) == 0 )
      // st.x [Aa]0, Db           ->      st.x [Aa], Db
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SAD_2,
            nthParam( o, 0 ), nthParam( o, 2 ) } ) );
    else

    if ( ( TC13::isA15( nthReg( o, 0 ) ) || TC13::isD15( nthReg( o, 2 ) ) ) &&
         ( nthSVal( o, 1 ) >= 0 ) &&
         ( nthSVal( o, 1 ) <=
             (long long) TC_Const16_Unsigned::getMaxValue( 4 ) ) )
      // st.x [A15]c, Da          ->      st.x [A15]c, Da (16)
      // st.x [Aa]c, D15          ->      st.x [Aa]c, D15 (16)
      return(
        replace(
          pos,
          { o.getOpCode(),
            TC13::isA15( nthReg( o, 0 ) ) ?
              TC13::OperationFormat::SIC4D : TC13::OperationFormat::SAC4I_2,
            nthParam( o, 0 ),
            new TC_Const4_Unsigned( (long long) nthSVal( o, 1 ) ),
            nthParam( o, 2 ) } ) );
    else

    if ( ( o.getOpCode() == TC13::OpCode::ST_W ) &&
         TC13::isSP( nthReg( o, 0 ) ) && TC13::isD15( nthReg( o, 2 ) ) &&
         ( nthSVal( o, 1 ) >= 0 ) &&
         ( nthSVal( o, 1 ) <=
             (long long) TC_Const16_Unsigned::getMaxValue( 10 ) ) &&
         ( nthSVal( o, 1 ) % 4 == 0 ) )
      // st.w [A10]c, D15         ->      st.w [A10]c, D15 (16)
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SSPC10I_2,
            nthParam( o, 0 ),
            new TC_Const10_Unsigned( (long long) nthSVal( o, 1 ) ),
            nthParam( o, 2 ) } ) );
  } else

  if ( ( o.getOperationFormat() == TC13::OperationFormat::AC10DPIA_1 ) &&
       isPostIncr( 0 ) ) {
    if ( ( ( o.getOpCode() == TC13::OpCode::ST_B ) && nthSVal( o, 2 ) == 1 ) ||
         ( ( o.getOpCode() == TC13::OpCode::ST_H ) && nthSVal( o, 2 ) == 2 ) ||
         ( ( o.getOpCode() == TC13::OpCode::ST_W ) && nthSVal( o, 2 ) == 4 ) )
      // st.x [Aa+]c, Db          ->      st.x [Aa+], Db
      return(
        replace(
          pos,
          { o.getOpCode(), TC13::OperationFormat::SAD_3,
            nthParam( o, 1 ), nthParam( o, 3 ) } ) );
  } else

  if ( ( o.getOpCode() == TC13::OpCode::SUB ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DDD_1 ) &&
       TC13::isD15( nthReg( o, 1 ) ) )
    // sub Da, D15, Db            ->      sub Da, D15, Db (16)
    return(
      replace(
        pos,
        { o.getOpCode(), TC13::OperationFormat::SDID_1,
          nthParam( o, 0 ), nthParam( o, 1 ), nthParam( o, 2 ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::SEL ) ||
         ( o.getOpCode() == TC13::OpCode::SELN ) ) &&
       TC13::isD15( nthReg( o, 1 ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DDDD ) &&
       ( nthReg( o, 0 ) == nthReg( o, 2 ) ) )
    // sel Da, D15, Da, Db        ->      cmovn Da, D15, Db (16)
    // seln Da, D15, Da, Db       ->      cmov Da, D15, Db (16)
    return(
      replace(
        pos,
        { o.getOpCode() == TC13::OpCode::SEL ?
            TC13::OpCode::CMOVN : TC13::OpCode::CMOV,
          TC13::OperationFormat::SDID_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 1 ), nthParam( o, 3 ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::SEL ) ||
         ( o.getOpCode() == TC13::OpCode::SELN ) ) &&
       TC13::isD15( nthReg( o, 1 ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DDDD ) &&
       ( nthReg( o, 0 ) == nthReg( o, 3 ) ) )
    // sel Da, D15, Db, Da        ->      cmov Da, D15, Db (16)
    // seln Da, D15, Db, Da       ->      cmovn Da, D15, Db (16)
    return(
      replace(
        pos,
        { o.getOpCode() == TC13::OpCode::SEL ?
            TC13::OpCode::CMOV : TC13::OpCode::CMOVN,
          TC13::OperationFormat::SDID_2,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 1 ), nthParam( o, 2 ) } ) );
  else

  if ( ( ( o.getOpCode() == TC13::OpCode::SEL ) ||
         ( o.getOpCode() == TC13::OpCode::SELN ) ) &&
       TC13::isD15( nthReg( o, 1 ) ) &&
       ( o.getOperationFormat() == TC13::OperationFormat::DDDC9_1 ) &&
       ( nthReg( o, 0 ) == nthReg( o, 2 ) ) &&
       ( nthSVal( o, 3 ) >= (long long) TC_Const4_Signed::getMinValue( 4 ) ) &&
       ( nthSVal( o, 3 ) <= (long long) TC_Const4_Signed::getMaxValue( 4 ) ) )
    // sel Da, D15, Da, c         ->      cmovn Da, D15, c (16)
    // seln Da, D15, Da, c        ->      cmov Da, D15, c (16)
    return(
      replace(
        pos,
        { o.getOpCode() == TC13::OpCode::SEL ?
            TC13::OpCode::CMOVN : TC13::OpCode::CMOV,
          TC13::OperationFormat::SDIC4_3,
          new WIR_RegisterParameter( nthReg( o, 0 ), WIR_Usage::defuse ),
          nthParam( o, 1 ),
          new TC_Const4_Signed( (long long) nthSVal( o, 3 ) ) } ) );

  return(
    const_cast<list<reference_wrapper<WIR_Operation>> &>(
      i.getOperations() ).end() );
};

}       // namespace WIR
