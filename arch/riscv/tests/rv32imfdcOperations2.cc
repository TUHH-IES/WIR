/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/


//
// Include section
//

// Include libuseful headers
#include <libuseful/io.h>

// Include WIR headers
#include <wir/wir.h>
#include <arch/riscv/rv32imfdc.h>


using namespace std;
using namespace WIR;


void ITests( void )
{
  RV32IMFDC p;
  const RV_RegP &x1 = p.x12(), &x2 = p.x12(), &x3 = p.x2();

  WIR_BasicBlock b;
  WIR_Function f( "main" );

  cout.iword( WIR_Indentation() ) = 8;
  cout << riscv << comment;

  // This lambda serves for generating a RISC-V operation, adding a comment to
  // it and finally dumping it.
  auto rvop = []( WIR_Operation &&o ) {
    o.insertContainer(
      WIR_Comment( "Operation Format: " + o.getOperationFormat().getName() ) );
    cout << o << endl;
  };

  // The following operations must be accepted according to the RISC-V RV32I
  // ISA.
  rvop(
    { RV32IMFDC::OpCode::ADD, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::ADDI, RV32IMFDC::OperationFormat::RRC12_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new RV_Const12_Signed( -1023 ) } );

  rvop(
    { RV32IMFDC::OpCode::ADDI, RV32IMFDC::OperationFormat::RRL_2,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::AND, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::ANDI, RV32IMFDC::OperationFormat::RRC12_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new RV_Const12_Signed( -1023 ) } );

  rvop(
    { RV32IMFDC::OpCode::ANDI, RV32IMFDC::OperationFormat::RRL_2,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::AUIPC, RV32IMFDC::OperationFormat::RC20_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_Const20_Unsigned( 500000 ) } );

  rvop(
    { RV32IMFDC::OpCode::AUIPC, RV32IMFDC::OperationFormat::RL_2,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::BEQ, RV32IMFDC::OperationFormat::RRL_1,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::BGE, RV32IMFDC::OperationFormat::RRL_1,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::BGEU, RV32IMFDC::OperationFormat::RRL_1,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::BLT, RV32IMFDC::OperationFormat::RRL_1,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::BLTU, RV32IMFDC::OperationFormat::RRL_1,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::BNE, RV32IMFDC::OperationFormat::RRL_1,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRC, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "frm" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRC, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "fcsr" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRC, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "fflags" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRC, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "instret" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRC, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "instreth" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRC, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "cycle" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRC, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "cycleh" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRC, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "time" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRC, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "timeh" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRCI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "frm" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRCI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "fcsr" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRCI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "fflags" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRCI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "instret" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRCI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "instreth" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRCI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "cycle" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRCI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "cycleh" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRCI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "time" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRCI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "timeh" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRS, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "frm" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRS, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "fcsr" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRS, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "fflags" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRS, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "instret" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRS, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "instreth" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRS, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "cycle" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRS, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "cycleh" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRS, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "time" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRS, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "timeh" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRSI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "frm" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRSI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "fcsr" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRSI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "fflags" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRSI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "instret" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRSI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "instreth" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRSI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "cycle" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRSI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "cycleh" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRSI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "time" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRSI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "timeh" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRW, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "frm" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRW, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "fcsr" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRW, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "fflags" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRW, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "instret" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRW, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "instreth" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRW, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "cycle" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRW, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "cycleh" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRW, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "time" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRW, RV32IMFDC::OperationFormat::RSR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "timeh" ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRWI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "frm" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRWI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "fcsr" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRWI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "fflags" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRWI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "instret" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRWI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "instreth" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRWI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "cycle" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRWI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "cycleh" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRWI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "time" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRRWI, RV32IMFDC::OperationFormat::RSC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_StringParameter( "timeh" ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::EBREAK, RV32IMFDC::OperationFormat::NULL_1 } );

  rvop(
    { RV32IMFDC::OpCode::ECALL, RV32IMFDC::OperationFormat::NULL_1 } );

  rvop(
    { RV32IMFDC::OpCode::J, RV32IMFDC::OperationFormat::L_1,
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::JAL, RV32IMFDC::OperationFormat::RL_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::JALR, RV32IMFDC::OperationFormat::RC12R_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_Const12_Signed( -4 ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::JALR, RV32IMFDC::OperationFormat::RLR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_LabelParameter( b ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::JALR, RV32IMFDC::OperationFormat::RRC12_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new RV_Const12_Signed( 0 ) } );

  rvop(
    { RV32IMFDC::OpCode::JALR, RV32IMFDC::OperationFormat::RRL_2,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32I::OpCode::JR, RV32I::OperationFormat::R_1,
      new WIR_RegisterParameter( x1, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::LB, RV32IMFDC::OperationFormat::RC12R_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::LB, RV32IMFDC::OperationFormat::RLR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_LabelParameter( b ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::LBU, RV32IMFDC::OperationFormat::RC12R_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::LBU, RV32IMFDC::OperationFormat::RLR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_LabelParameter( b ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::LH, RV32IMFDC::OperationFormat::RC12R_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::LH, RV32IMFDC::OperationFormat::RLR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_LabelParameter( b ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::LHU, RV32IMFDC::OperationFormat::RC12R_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::LHU, RV32IMFDC::OperationFormat::RLR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_LabelParameter( b ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::LUI, RV32IMFDC::OperationFormat::RC20_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_Const20_Unsigned( 500000 ) } );

  rvop(
    { RV32IMFDC::OpCode::LUI, RV32IMFDC::OperationFormat::RL_2,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::LW, RV32IMFDC::OperationFormat::RC12R_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::LW, RV32IMFDC::OperationFormat::RLR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_LabelParameter( b ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::MOV, RV32IMFDC::OperationFormat::RR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::OR, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::ORI, RV32IMFDC::OperationFormat::RRC12_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new RV_Const12_Signed( -1023 ) } );

  rvop(
    { RV32IMFDC::OpCode::ORI, RV32IMFDC::OperationFormat::RRL_2,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::RET, RV32IMFDC::OperationFormat::NULL_1 } );

  rvop(
    { RV32IMFDC::OpCode::SB, RV32IMFDC::OperationFormat::RC12R_2,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::SB, RV32IMFDC::OperationFormat::RLR_2,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_LabelParameter( b ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::SH, RV32IMFDC::OperationFormat::RC12R_2,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::SH, RV32IMFDC::OperationFormat::RLR_2,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_LabelParameter( b ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::SLL, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::SLLI, RV32IMFDC::OperationFormat::RRC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::SLT, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::SLTI, RV32IMFDC::OperationFormat::RRC12_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new RV_Const12_Signed( -1023 ) } );

  rvop(
    { RV32IMFDC::OpCode::SLTI, RV32IMFDC::OperationFormat::RRL_2,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::SLTIU, RV32IMFDC::OperationFormat::RRC12_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new RV_Const12_Signed( -1023 ) } );

  rvop(
    { RV32IMFDC::OpCode::SLTIU, RV32IMFDC::OperationFormat::RRL_2,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  rvop(
    { RV32IMFDC::OpCode::SLTU, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::SRA, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::SRAI, RV32IMFDC::OperationFormat::RRC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::SRL, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::SRLI, RV32IMFDC::OperationFormat::RRC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new RV_Const5_Unsigned( 15 ) } );

  rvop(
    { RV32IMFDC::OpCode::SUB, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::SW, RV32IMFDC::OperationFormat::RC12R_2,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::SW, RV32IMFDC::OperationFormat::RLR_2,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_LabelParameter( b ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::XOR, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::XORI, RV32IMFDC::OperationFormat::RRC12_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new RV_Const12_Signed( -1023 ) } );

  rvop(
    { RV32IMFDC::OpCode::XORI, RV32IMFDC::OperationFormat::RRL_2,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_LabelParameter( b ) } );

  return;
};


void ICTests( void )
{
  RV32IMFDC p;
  const RV_RegP &x1 = p.x12(), &x2 = p.x12(), &x3 = p.x2();

  WIR_BasicBlock b;
  WIR_Function f( "main" );

  cout.iword( WIR_Indentation() ) = 8;
  cout << riscv << comment;

  // This lambda serves for generating a RISC-V operation, adding a comment to
  // it and finally dumping it.
  auto rvop = []( WIR_Operation &&o ) {
    o.insertContainer(
      WIR_Comment( "Operation Format: " + o.getOperationFormat().getName() ) );
    cout << o << endl;
  };

  // The following operations must be accepted according to the RISC-V RV32IC
  // ISA.
  rvop(
    { RV32IMFDC::OpCode::CADD, RV32IMFDC::OperationFormat::SRR_2,
      new WIR_RegisterParameter( x1, WIR_Usage::defuse ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CADDI, RV32IMFDC::OperationFormat::SRC6_2,
      new WIR_RegisterParameter( x1, WIR_Usage::defuse ),
      new RV_Const6_Signed( -10 ) } );

  rvop(
    { RV32IMFDC::OpCode::CADDI16SP, RV32IMFDC::OperationFormat::SRC10_1,
      new WIR_RegisterParameter( x3, WIR_Usage::defuse ),
      new RV_Const10_Signed( -16 ) } );

  rvop(
    { RV32IMFDC::OpCode::CADDI4SPN, RV32IMFDC::OperationFormat::SRRC10_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ),
      new RV_Const10_Unsigned( 4 ) } );

  rvop(
    { RV32IMFDC::OpCode::CAND, RV32IMFDC::OperationFormat::SRR_2,
      new WIR_RegisterParameter( x1, WIR_Usage::defuse ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CANDI, RV32IMFDC::OperationFormat::SRC6_2,
      new WIR_RegisterParameter( x1, WIR_Usage::defuse ),
      new RV_Const6_Signed( -10 ) } );

  rvop(
    { RV32IMFDC::OpCode::CBEQZ, RV32IMFDC::OperationFormat::SRL_1,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_LabelParameter ( b ) } );

  rvop(
    { RV32IMFDC::OpCode::CBNEZ, RV32IMFDC::OperationFormat::SRL_1,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_LabelParameter ( b ) } );

  rvop(
    { RV32IMFDC::OpCode::CEBREAK, RV32IMFDC::OperationFormat::SNULL_1 } );

  rvop(
    { RV32IMFDC::OpCode::CJ, RV32IMFDC::OperationFormat::SL_1,
      new WIR_LabelParameter ( b ) } );

  rvop(
    { RV32IMFDC::OpCode::CJAL, RV32IMFDC::OperationFormat::SL_1,
      new WIR_LabelParameter ( b ) } );

  rvop(
    { RV32IMFDC::OpCode::CJALR, RV32IMFDC::OperationFormat::SR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CJR, RV32IMFDC::OperationFormat::SR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CLI, RV32IMFDC::OperationFormat::SRC6_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_Const6_Signed( -10 ) } );

  rvop(
    { RV32IC::OpCode::CLUI, RV32IC::OperationFormat::SRC20_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_NZConst20_Unsigned( 0x1 ) } );

  rvop(
    { RV32IC::OpCode::CLUI, RV32IC::OperationFormat::SRC20_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_NZConst20_Unsigned( 0x1F ) } );

  rvop(
    { RV32IC::OpCode::CLUI, RV32IC::OperationFormat::SRC20_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_NZConst20_Unsigned( 0xFFFE0 ) } );

  rvop(
    { RV32IC::OpCode::CLUI, RV32IC::OperationFormat::SRC20_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_NZConst20_Unsigned( 0xFFFFF ) } );

  rvop(
    { RV32IMFDC::OpCode::CLW, RV32IMFDC::OperationFormat::SRC7R_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new RV_Const7_Unsigned( 4 ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CLWSP, RV32IMFDC::OperationFormat::SRC8R_1,
     new WIR_RegisterParameter( x1, WIR_Usage::def ),
     new RV_Const8_Unsigned( 24 ),
     new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CMV, RV32IMFDC::OperationFormat::SRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CNOP, RV32IMFDC::OperationFormat::SNULL_1 } );

  rvop(
    { RV32IMFDC::OpCode::COR, RV32IMFDC::OperationFormat::SRR_2,
      new WIR_RegisterParameter( x1, WIR_Usage::defuse ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSLLI, RV32IMFDC::OperationFormat::SRC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::defuse ),
      new RV_Const5_Unsigned( 10 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRAI, RV32IMFDC::OperationFormat::SRC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::defuse ),
      new RV_Const5_Unsigned( 10 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSRLI, RV32IMFDC::OperationFormat::SRC5_1,
      new WIR_RegisterParameter( x1, WIR_Usage::defuse ),
      new RV_Const5_Unsigned( 10 ) } );

  rvop(
    { RV32IMFDC::OpCode::CSUB, RV32IMFDC::OperationFormat::SRR_2,
      new WIR_RegisterParameter( x1, WIR_Usage::defuse ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSW, RV32IMFDC::OperationFormat::SRC7R_2,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new RV_Const7_Unsigned( 4 ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CSWSP, RV32IMFDC::OperationFormat::SRC8R_2,
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new RV_Const8_Unsigned( 16 ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::CXOR, RV32IMFDC::OperationFormat::SRR_2,
      new WIR_RegisterParameter( x1, WIR_Usage::defuse ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ) } );

  return;
};


void IMTests( void )
{
  RV32IMFDC p;
  const RV_RegP &x1 = p.x1(), &x2 = p.x2(), &x3 = p.x3();

  WIR_Function f( "main" );

  cout.iword( WIR_Indentation() ) = 8;
  cout << riscv << comment;

  // This lambda serves for generating a RISC-V operation, adding a comment to
  // it and finally dumping it.
  auto rvop = []( WIR_Operation &&o ) {
    o.insertContainer(
      WIR_Comment( "Operation Format: " + o.getOperationFormat().getName() ) );
    cout << o << endl;
  };

  // The following operations must be accepted according to the RISC-V RV32IM
  // ISA.
  rvop(
    { RV32IMFDC::OpCode::DIV, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::DIVU, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::MUL, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::MULH, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::MULHSU, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::MULHU, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::REM, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::REMU, RV32IMFDC::OperationFormat::RRR_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( x2, WIR_Usage::use ),
      new WIR_RegisterParameter( x3, WIR_Usage::use ) } );

  return;
};


void IFTests( void )
{
  RV32IMFDC p;
  const RV_FRegP &f1 = p.f1(), &f2 = p.f12(), &f3 = p.f2(), &f4 = p.f4();
  const RV_RegP &x1 = p.x10();

  cout.iword( WIR_Indentation() ) = 8;
  cout << riscv << comment;

  // This lambda serves for generating a RISC-V operation, adding a comment to
  // it and finally dumping it.
  auto rvop = []( WIR_Operation &&o ) {
    o.insertContainer(
      WIR_Comment( "Operation Format: " + o.getOperationFormat().getName() ) );
    cout << o << endl;
  };

  // The following operations must be accepted according to the RISC-V RV32IMFDC
  // ISA.
  rvop(
    { RV32IMFDC::OpCode::FADD_S, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FCLASS_S, RV32IMFDC::OperationFormat::RF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IF::OpCode::FCVT_S_W, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rne" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_W, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rtz" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_W, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rdn" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_W, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rup" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_W, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rmm" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_WU, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rne" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_WU, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rtz" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_WU, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rdn" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_WU, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rup" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_S_WU, RV32IF::OperationFormat::FRS_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ),
      new WIR_StringParameter( "rmm" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_W_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      WIR_StringParameter( "rne" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_W_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      WIR_StringParameter( "rtz" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_W_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      WIR_StringParameter( "rdn" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_W_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      WIR_StringParameter( "rup" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_W_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      WIR_StringParameter( "rmm" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_WU_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_StringParameter( "rne" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_WU_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_StringParameter( "rtz" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_WU_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_StringParameter( "rdn" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_WU_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_StringParameter( "rup" ) } );

  rvop(
    { RV32IF::OpCode::FCVT_WU_S, RV32IF::OperationFormat::RFS_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_StringParameter( "rmm" ) } );

  rvop(
    { RV32IMFDC::OpCode::FDIV_S, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FEQ_S, RV32IMFDC::OperationFormat::RFF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FLE_S, RV32IMFDC::OperationFormat::RFF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FLT_S, RV32IMFDC::OperationFormat::RFF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FLW, RV32IMFDC::OperationFormat::FC12R_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FMADD_S, RV32IMFDC::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FMAX_S, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FMIN_S, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FMSUB_S, RV32IMFDC::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FMUL_S, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FMV_S, RV32IMFDC::OperationFormat::FF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FMV_W_X, RV32IMFDC::OperationFormat::FR_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FMV_X_W, RV32IMFDC::OperationFormat::RF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FNMADD_S, RV32IMFDC::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FNMSUB_S, RV32IMFDC::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FSGNJ_S, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FSGNJN_S, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FSGNJX_S, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FSQRT_S, RV32IMFDC::OperationFormat::FF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FSUB_S, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FSW, RV32IMFDC::OperationFormat::FC12R_2,
      new WIR_RegisterParameter( f1, WIR_Usage::use ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ) } );
};


void IDTests( void )
{
  RV32IMFDC p;
  const RV_FRegP &f1 = p.f1(), &f2 = p.f12(), &f3 = p.f2(), &f4 = p.f4();
  const RV_RegP &x1 = p.x10();

  cout.iword( WIR_Indentation() ) = 8;
  cout << riscv << comment;

  // This lambda serves for generating a RISC-V operation, adding a comment to
  // it and finally dumping it.
  auto rvop = []( WIR_Operation &&o ) {
    o.insertContainer(
      WIR_Comment( "Operation Format: " + o.getOperationFormat().getName() ) );
    cout << o << endl;
  };

  // The following operations must be accepted according to the RISC-V RV32IMFDC
  // ISA.
  rvop(
    { RV32IMFDC::OpCode::FADD_D, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FCLASS_D, RV32IMFDC::OperationFormat::RF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FCVT_D_S, RV32IMFDC::OperationFormat::FF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FCVT_D_W, RV32IMFDC::OperationFormat::FR_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FCVT_D_WU, RV32IMFDC::OperationFormat::FR_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( x1, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FCVT_S_D, RV32IMFDC::OperationFormat::FF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FCVT_W_D, RV32IMFDC::OperationFormat::RF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FCVT_WU_D, RV32IMFDC::OperationFormat::RF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FDIV_D, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FEQ_D, RV32IMFDC::OperationFormat::RFF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FLD, RV32IMFDC::OperationFormat::FC12R_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FLE_D, RV32IMFDC::OperationFormat::RFF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FLT_D, RV32IMFDC::OperationFormat::RFF_1,
      new WIR_RegisterParameter( x1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FMADD_D, RV32IMFDC::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FMAX_D, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FMIN_D, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FMSUB_D, RV32IMFDC::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FMUL_D, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FMV_D, RV32IMFDC::OperationFormat::FF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FNMADD_D, RV32IMFDC::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FNMSUB_D, RV32IMFDC::OperationFormat::FFFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ),
      new WIR_RegisterParameter( f4, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FSD, RV32IMFDC::OperationFormat::FC12R_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new RV_Const12_Signed( -1023 ),
      new WIR_RegisterParameter( x1, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FSGNJ_D, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FSGNJN_D, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FSGNJX_D, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );

  rvop(
    { RV32IMFDC::OpCode::FSQRT_D, RV32IMFDC::OperationFormat::FF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f3, WIR_Usage::use )} );

  rvop(
    { RV32IMFDC::OpCode::FSUB_D, RV32IMFDC::OperationFormat::FFF_1,
      new WIR_RegisterParameter( f1, WIR_Usage::def ),
      new WIR_RegisterParameter( f2, WIR_Usage::use ),
      new WIR_RegisterParameter( f3, WIR_Usage::use ) } );
};


int main( void )
{
  WIR_Init();

  // The tests are split into seperate functions to avoid conflicts due to
  // matching variable names.

  // RV32I tests.
  ITests();

  // RV32IC tests.
  ICTests();

  // RV32IM tests.
  IMTests();

  // RV32IF tests.
  IFTests();

  // RV32ID tests.
  IDTests();

  return( 0 );
};
