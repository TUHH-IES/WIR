/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file tc13opcodes.cc
  @brief This file declares the Infineon TriCore V1.3's opcodes.

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
#include <arch/tricore/tc13.h>


//
// Code section
//

namespace WIR {


using namespace std;


//
// Global initializations
//

const TC13::OpCode TC13::OpCode::ABS        { "abs" };
const TC13::OpCode TC13::OpCode::ABS_B      { "abs.b" };
const TC13::OpCode TC13::OpCode::ABS_H      { "abs.h" };
const TC13::OpCode TC13::OpCode::ABSDIF     { "absdif" };
const TC13::OpCode TC13::OpCode::ABSDIF_B   { "absdif.b" };
const TC13::OpCode TC13::OpCode::ABSDIF_H   { "absdif.h" };
const TC13::OpCode TC13::OpCode::ABSDIFS    { "absdifs" };
const TC13::OpCode TC13::OpCode::ABSDIFS_H  { "absdifs.h" };
const TC13::OpCode TC13::OpCode::ABSS       { "abss" };
const TC13::OpCode TC13::OpCode::ABSS_H     { "abss.h" };
const TC13::OpCode TC13::OpCode::ADD        { "add" };
const TC13::OpCode TC13::OpCode::ADD_A      { "add.a" };
const TC13::OpCode TC13::OpCode::ADD_B      { "add.b" };
const TC13::OpCode TC13::OpCode::ADD_F      { "add.f" };
const TC13::OpCode TC13::OpCode::ADD_H      { "add.h" };
const TC13::OpCode TC13::OpCode::ADDC       { "addc" };
const TC13::OpCode TC13::OpCode::ADDI       { "addi" };
const TC13::OpCode TC13::OpCode::ADDIH      { "addih" };
const TC13::OpCode TC13::OpCode::ADDIH_A    { "addih.a" };
const TC13::OpCode TC13::OpCode::ADDS       { "adds" };
const TC13::OpCode TC13::OpCode::ADDS_H     { "adds.h" };
const TC13::OpCode TC13::OpCode::ADDS_HU    { "adds.hu" };
const TC13::OpCode TC13::OpCode::ADDS_U     { "adds.u" };
const TC13::OpCode TC13::OpCode::ADDSC_A    { "addsc.a" };
const TC13::OpCode TC13::OpCode::ADDSC_AT   { "addsc.at" };
const TC13::OpCode TC13::OpCode::ADDX       { "addx" };
const TC13::OpCode TC13::OpCode::AND        { "and" };
const TC13::OpCode TC13::OpCode::AND_AND_T  { "and.and.t" };
const TC13::OpCode TC13::OpCode::AND_ANDN_T { "and.andn.t" };
const TC13::OpCode TC13::OpCode::AND_EQ     { "and.eq" };
const TC13::OpCode TC13::OpCode::AND_GE     { "and.ge" };
const TC13::OpCode TC13::OpCode::AND_GE_U   { "and.ge.u" };
const TC13::OpCode TC13::OpCode::AND_LT     { "and.lt" };
const TC13::OpCode TC13::OpCode::AND_LT_U   { "and.lt.u" };
const TC13::OpCode TC13::OpCode::AND_NE     { "and.ne" };
const TC13::OpCode TC13::OpCode::AND_NOR_T  { "and.nor.t" };
const TC13::OpCode TC13::OpCode::AND_OR_T   { "and.or.t" };
const TC13::OpCode TC13::OpCode::AND_T      { "and.t" };
const TC13::OpCode TC13::OpCode::ANDN       { "andn" };
const TC13::OpCode TC13::OpCode::ANDN_T     { "andn.t" };
const TC13::OpCode TC13::OpCode::BISR       { "bisr",
                                              WIR_OpCodeType::implicitMemoryAccess,
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::BMERGE     { "bmerge" };
const TC13::OpCode TC13::OpCode::BSPLIT     { "bsplit" };
const TC13::OpCode TC13::OpCode::CACHEA_I   { "cachea.i",
                                              WIR_OpCodeType::implicitMemoryAccess,
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::CACHEA_W   { "cachea.w",
                                              WIR_OpCodeType::implicitMemoryAccess,
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::CACHEA_WI  { "cachea.wi",
                                              WIR_OpCodeType::implicitMemoryAccess,
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::CADD       { "cadd" };
const TC13::OpCode TC13::OpCode::CADDN      { "caddn" };
const TC13::OpCode TC13::OpCode::CALL       { "call",
                                              WIR_OpCodeType::implicitMemoryAccess,
                                              WIR_OpCodeType::call };
const TC13::OpCode TC13::OpCode::CALLA      { "calla",
                                              WIR_OpCodeType::implicitMemoryAccess,
                                              WIR_OpCodeType::call };
const TC13::OpCode TC13::OpCode::CALLI      { "calli",
                                              WIR_OpCodeType::implicitMemoryAccess,
                                              WIR_OpCodeType::indirectCall };
const TC13::OpCode TC13::OpCode::CLO        { "clo" };
const TC13::OpCode TC13::OpCode::CLO_H      { "clo.h" };
const TC13::OpCode TC13::OpCode::CLS        { "cls" };
const TC13::OpCode TC13::OpCode::CLS_H      { "cls.h" };
const TC13::OpCode TC13::OpCode::CLZ        { "clz" };
const TC13::OpCode TC13::OpCode::CLZ_H      { "clz.h" };
const TC13::OpCode TC13::OpCode::CMOV       { "cmov" };
const TC13::OpCode TC13::OpCode::CMOVN      { "cmovn" };
const TC13::OpCode TC13::OpCode::CMP_F      { "cmp.f" };
const TC13::OpCode TC13::OpCode::CSUB       { "csub" };
const TC13::OpCode TC13::OpCode::CSUBN      { "csubn" };
const TC13::OpCode TC13::OpCode::DEBUG      { "debug",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::DEXTR      { "dextr" };
const TC13::OpCode TC13::OpCode::DISABLE    { "disable",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::DIV_F      { "div.f" };
const TC13::OpCode TC13::OpCode::DSYNC      { "dsync",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::DVADJ      { "dvadj" };
const TC13::OpCode TC13::OpCode::DVINIT     { "dvinit" };
const TC13::OpCode TC13::OpCode::DVINIT_B   { "dvinit.b" };
const TC13::OpCode TC13::OpCode::DVINIT_BU  { "dvinit.bu" };
const TC13::OpCode TC13::OpCode::DVINIT_H   { "dvinit.h" };
const TC13::OpCode TC13::OpCode::DVINIT_HU  { "dvinit.hu" };
const TC13::OpCode TC13::OpCode::DVINIT_U   { "dvinit.u" };
const TC13::OpCode TC13::OpCode::DVSTEP     { "dvstep" };
const TC13::OpCode TC13::OpCode::DVSTEP_U   { "dvstep.u" };
const TC13::OpCode TC13::OpCode::ENABLE     { "enable",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::EQ         { "eq" };
const TC13::OpCode TC13::OpCode::EQ_A       { "eq.a" };
const TC13::OpCode TC13::OpCode::EQ_B       { "eq.b" };
const TC13::OpCode TC13::OpCode::EQ_H       { "eq.h" };
const TC13::OpCode TC13::OpCode::EQ_W       { "eq.w" };
const TC13::OpCode TC13::OpCode::EQANY_B    { "eqany.b" };
const TC13::OpCode TC13::OpCode::EQANY_H    { "eqany.h" };
const TC13::OpCode TC13::OpCode::EQZ_A      { "eqz.a" };
const TC13::OpCode TC13::OpCode::EXTR       { "extr" };
const TC13::OpCode TC13::OpCode::EXTR_U     { "extr.u" };
const TC13::OpCode TC13::OpCode::FTOI       { "ftoi" };
const TC13::OpCode TC13::OpCode::FTOQ31     { "ftoq31" };
const TC13::OpCode TC13::OpCode::FTOU       { "ftou" };
const TC13::OpCode TC13::OpCode::GE         { "ge" };
const TC13::OpCode TC13::OpCode::GE_A       { "ge.a" };
const TC13::OpCode TC13::OpCode::GE_U       { "ge.u" };
const TC13::OpCode TC13::OpCode::IMASK      { "imask" };
const TC13::OpCode TC13::OpCode::INS_T      { "ins.t" };
const TC13::OpCode TC13::OpCode::INSERT     { "insert" };
const TC13::OpCode TC13::OpCode::INSN_T     { "insn.t" };
const TC13::OpCode TC13::OpCode::ISYNC      { "isync",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::ITOF       { "itof" };
const TC13::OpCode TC13::OpCode::IXMAX      { "ixmax" };
const TC13::OpCode TC13::OpCode::IXMAX_U    { "ixmax.u" };
const TC13::OpCode TC13::OpCode::IXMIN      { "ixmin" };
const TC13::OpCode TC13::OpCode::IXMIN_U    { "ixmin.u" };
const TC13::OpCode TC13::OpCode::J          { "j",
                                              WIR_OpCodeType::uncondJump };
const TC13::OpCode TC13::OpCode::JA         { "ja",
                                              WIR_OpCodeType::uncondJump };
const TC13::OpCode TC13::OpCode::JEQ        { "jeq",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JEQ_A      { "jeq.a",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JGE        { "jge",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JGE_U      { "jge.u",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JGEZ       { "jgez",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JGTZ       { "jgtz",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JI         { "ji",
                                              WIR_OpCodeType::uncondJump,
                                              WIR_OpCodeType::indirectJump };
const TC13::OpCode TC13::OpCode::JL         { "jl", WIR_OpCodeType::call };
const TC13::OpCode TC13::OpCode::JLA        { "jla", WIR_OpCodeType::call };
const TC13::OpCode TC13::OpCode::JLEZ       { "jlez",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JLI        { "jli",
                                              WIR_OpCodeType::indirectCall };
const TC13::OpCode TC13::OpCode::JLT        { "jlt",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JLT_U      { "jlt.u",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JLTZ       { "jltz",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JNE        { "jne",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JNE_A      { "jne.a",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JNED       { "jned",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JNEI       { "jnei",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JNZ        { "jnz",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JNZ_A      { "jnz.a",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JNZ_T      { "jnz.t",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JZ         { "jz",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JZ_A       { "jz.a",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::JZ_T       { "jz.t",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::LD_A       { "ld.a", WIR_OpCodeType::load };
const TC13::OpCode TC13::OpCode::LD_B       { "ld.b", WIR_OpCodeType::load };
const TC13::OpCode TC13::OpCode::LD_BU      { "ld.bu", WIR_OpCodeType::load };
const TC13::OpCode TC13::OpCode::LD_D       { "ld.d", WIR_OpCodeType::load };
const TC13::OpCode TC13::OpCode::LD_DA      { "ld.da", WIR_OpCodeType::load };
const TC13::OpCode TC13::OpCode::LD_H       { "ld.h", WIR_OpCodeType::load };
const TC13::OpCode TC13::OpCode::LD_HU      { "ld.hu", WIR_OpCodeType::load };
const TC13::OpCode TC13::OpCode::LD_Q       { "ld.q", WIR_OpCodeType::load };
const TC13::OpCode TC13::OpCode::LD_W       { "ld.w", WIR_OpCodeType::load };
const TC13::OpCode TC13::OpCode::LDLCX      { "ldlcx", WIR_OpCodeType::load,
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::LDMST      { "ldmst", WIR_OpCodeType::load,
                                              WIR_OpCodeType::store };
const TC13::OpCode TC13::OpCode::LDUCX      { "lducx", WIR_OpCodeType::load,
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::LEA        { "lea" };
const TC13::OpCode TC13::OpCode::LOOP       { "loop",
                                              WIR_OpCodeType::condJump };
const TC13::OpCode TC13::OpCode::LOOPU      { "loopu",
                                              WIR_OpCodeType::uncondJump };
const TC13::OpCode TC13::OpCode::LT         { "lt" };
const TC13::OpCode TC13::OpCode::LT_A       { "lt.a" };
const TC13::OpCode TC13::OpCode::LT_B       { "lt.b" };
const TC13::OpCode TC13::OpCode::LT_BU      { "lt.bu" };
const TC13::OpCode TC13::OpCode::LT_H       { "lt.h" };
const TC13::OpCode TC13::OpCode::LT_HU      { "lt.hu" };
const TC13::OpCode TC13::OpCode::LT_U       { "lt.u" };
const TC13::OpCode TC13::OpCode::LT_W       { "lt.w" };
const TC13::OpCode TC13::OpCode::LT_WU      { "lt.wu" };
const TC13::OpCode TC13::OpCode::MADD       { "madd" };
const TC13::OpCode TC13::OpCode::MADD_F     { "madd.f" };
const TC13::OpCode TC13::OpCode::MADD_H     { "madd.h" };
const TC13::OpCode TC13::OpCode::MADD_Q     { "madd.q" };
const TC13::OpCode TC13::OpCode::MADD_U     { "madd.u" };
const TC13::OpCode TC13::OpCode::MADDM_H    { "maddm.h" };
const TC13::OpCode TC13::OpCode::MADDMS_H   { "maddms.h" };
const TC13::OpCode TC13::OpCode::MADDR_H    { "maddr.h" };
const TC13::OpCode TC13::OpCode::MADDR_Q    { "maddr.q" };
const TC13::OpCode TC13::OpCode::MADDRS_H   { "maddrs.h" };
const TC13::OpCode TC13::OpCode::MADDRS_Q   { "maddrs.q" };
const TC13::OpCode TC13::OpCode::MADDS      { "madds" };
const TC13::OpCode TC13::OpCode::MADDS_H    { "madds.h" };
const TC13::OpCode TC13::OpCode::MADDS_Q    { "madds.q" };
const TC13::OpCode TC13::OpCode::MADDS_U    { "madds.u" };
const TC13::OpCode TC13::OpCode::MADDSU_H   { "maddsu.h" };
const TC13::OpCode TC13::OpCode::MADDSUM_H  { "maddsum.h" };
const TC13::OpCode TC13::OpCode::MADDSUMS_H { "maddsums.h" };
const TC13::OpCode TC13::OpCode::MADDSUR_H  { "maddsur.h" };
const TC13::OpCode TC13::OpCode::MADDSURS_H { "maddsurs.h" };
const TC13::OpCode TC13::OpCode::MADDSUS_H  { "maddsus.h" };
const TC13::OpCode TC13::OpCode::MAX        { "max" };
const TC13::OpCode TC13::OpCode::MAX_B      { "max.b" };
const TC13::OpCode TC13::OpCode::MAX_BU     { "max.bu" };
const TC13::OpCode TC13::OpCode::MAX_H      { "max.h" };
const TC13::OpCode TC13::OpCode::MAX_HU     { "max.hu" };
const TC13::OpCode TC13::OpCode::MAX_U      { "max.u" };
const TC13::OpCode TC13::OpCode::MFCR       { "mfcr" };
const TC13::OpCode TC13::OpCode::MIN        { "min" };
const TC13::OpCode TC13::OpCode::MIN_B      { "min.b" };
const TC13::OpCode TC13::OpCode::MIN_BU     { "min.bu" };
const TC13::OpCode TC13::OpCode::MIN_H      { "min.h" };
const TC13::OpCode TC13::OpCode::MIN_HU     { "min.hu" };
const TC13::OpCode TC13::OpCode::MIN_U      { "min.u" };
const TC13::OpCode TC13::OpCode::MOV        { "mov" };
const TC13::OpCode TC13::OpCode::MOV_RR     { "mov", WIR_OpCodeType::move };
const TC13::OpCode TC13::OpCode::MOV_A      { "mov.a" };
const TC13::OpCode TC13::OpCode::MOV_AA     { "mov.aa", WIR_OpCodeType::move };
const TC13::OpCode TC13::OpCode::MOV_D      { "mov.d" };
const TC13::OpCode TC13::OpCode::MOV_U      { "mov.u" };
const TC13::OpCode TC13::OpCode::MOVH       { "movh" };
const TC13::OpCode TC13::OpCode::MOVH_A     { "movh.a" };
const TC13::OpCode TC13::OpCode::MSUB       { "msub" };
const TC13::OpCode TC13::OpCode::MSUB_F     { "msub.f" };
const TC13::OpCode TC13::OpCode::MSUB_H     { "msub.h" };
const TC13::OpCode TC13::OpCode::MSUB_Q     { "msub.q" };
const TC13::OpCode TC13::OpCode::MSUB_U     { "msub.u" };
const TC13::OpCode TC13::OpCode::MSUBAD_H   { "msubad.h" };
const TC13::OpCode TC13::OpCode::MSUBADM_H  { "msubadm.h" };
const TC13::OpCode TC13::OpCode::MSUBADMS_H { "msubadms.h" };
const TC13::OpCode TC13::OpCode::MSUBADR_H  { "msubadr.h" };
const TC13::OpCode TC13::OpCode::MSUBADRS_H { "msubadrs.h" };
const TC13::OpCode TC13::OpCode::MSUBADS_H  { "msubads.h" };
const TC13::OpCode TC13::OpCode::MSUBM_H    { "msubm.h" };
const TC13::OpCode TC13::OpCode::MSUBMS_H   { "msubms.h" };
const TC13::OpCode TC13::OpCode::MSUBR_H    { "msubr.h" };
const TC13::OpCode TC13::OpCode::MSUBR_Q    { "msubr.q" };
const TC13::OpCode TC13::OpCode::MSUBRS_H   { "msubrs.h" };
const TC13::OpCode TC13::OpCode::MSUBRS_Q   { "msubrs.q" };
const TC13::OpCode TC13::OpCode::MSUBS      { "msubs" };
const TC13::OpCode TC13::OpCode::MSUBS_H    { "msubs.h" };
const TC13::OpCode TC13::OpCode::MSUBS_Q    { "msubs.q" };
const TC13::OpCode TC13::OpCode::MSUBS_U    { "msubs.u" };
const TC13::OpCode TC13::OpCode::MTCR       { "mtcr",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::MUL        { "mul" };
const TC13::OpCode TC13::OpCode::MUL_F      { "mul.f" };
const TC13::OpCode TC13::OpCode::MUL_H      { "mul.h" };
const TC13::OpCode TC13::OpCode::MUL_Q      { "mul.q" };
const TC13::OpCode TC13::OpCode::MUL_U      { "mul.u" };
const TC13::OpCode TC13::OpCode::MULM_H     { "mulm.h" };
const TC13::OpCode TC13::OpCode::MULR_H     { "mulr.h" };
const TC13::OpCode TC13::OpCode::MULR_Q     { "mulr.q" };
const TC13::OpCode TC13::OpCode::MULS       { "muls" };
const TC13::OpCode TC13::OpCode::MULS_U     { "muls.u" };
const TC13::OpCode TC13::OpCode::NAND       { "nand" };
const TC13::OpCode TC13::OpCode::NAND_T     { "nand.t" };
const TC13::OpCode TC13::OpCode::NE         { "ne" };
const TC13::OpCode TC13::OpCode::NE_A       { "ne.a" };
const TC13::OpCode TC13::OpCode::NEZ_A      { "nez.a" };
const TC13::OpCode TC13::OpCode::NOP        { "nop" };
const TC13::OpCode TC13::OpCode::NOR        { "nor" };
const TC13::OpCode TC13::OpCode::NOR_T      { "nor.t" };
const TC13::OpCode TC13::OpCode::NOT        { "not" };
const TC13::OpCode TC13::OpCode::OR         { "or" };
const TC13::OpCode TC13::OpCode::OR_AND_T   { "or.and.t" };
const TC13::OpCode TC13::OpCode::OR_ANDN_T  { "or.andn.t" };
const TC13::OpCode TC13::OpCode::OR_EQ      { "or.eq" };
const TC13::OpCode TC13::OpCode::OR_GE      { "or.ge" };
const TC13::OpCode TC13::OpCode::OR_GE_U    { "or.ge.u" };
const TC13::OpCode TC13::OpCode::OR_LT      { "or.lt" };
const TC13::OpCode TC13::OpCode::OR_LT_U    { "or.lt.u" };
const TC13::OpCode TC13::OpCode::OR_NE      { "or.ne" };
const TC13::OpCode TC13::OpCode::OR_NOR_T   { "or.nor.t" };
const TC13::OpCode TC13::OpCode::OR_OR_T    { "or.or.t" };
const TC13::OpCode TC13::OpCode::OR_T       { "or.t" };
const TC13::OpCode TC13::OpCode::ORN        { "orn" };
const TC13::OpCode TC13::OpCode::ORN_T      { "orn.t" };
const TC13::OpCode TC13::OpCode::PACK       { "pack" };
const TC13::OpCode TC13::OpCode::PARITY     { "parity" };
const TC13::OpCode TC13::OpCode::Q31TOF     { "q31tof" };
const TC13::OpCode TC13::OpCode::QSEED_F    { "qseed.f" };
const TC13::OpCode TC13::OpCode::RET        { "ret",
                                              WIR_OpCodeType::implicitMemoryAccess,
                                              WIR_OpCodeType::ret };
const TC13::OpCode TC13::OpCode::RFE        { "rfe",
                                              WIR_OpCodeType::implicitMemoryAccess,
                                              WIR_OpCodeType::ret };
const TC13::OpCode TC13::OpCode::RFM        { "rfm",
                                              WIR_OpCodeType::implicitMemoryAccess,
                                              WIR_OpCodeType::ret };
const TC13::OpCode TC13::OpCode::RSLCX      { "rslcx", WIR_OpCodeType::load,
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::RSTV       { "rstv",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::RSUB       { "rsub" };
const TC13::OpCode TC13::OpCode::RSUBS      { "rsubs" };
const TC13::OpCode TC13::OpCode::RSUBS_U    { "rsubs.u" };
const TC13::OpCode TC13::OpCode::SAT_B      { "sat.b" };
const TC13::OpCode TC13::OpCode::SAT_BU     { "sat.bu" };
const TC13::OpCode TC13::OpCode::SAT_H      { "sat.h" };
const TC13::OpCode TC13::OpCode::SAT_HU     { "sat.hu" };
const TC13::OpCode TC13::OpCode::SEL        { "sel" };
const TC13::OpCode TC13::OpCode::SELN       { "seln" };
const TC13::OpCode TC13::OpCode::SH         { "sh" };
const TC13::OpCode TC13::OpCode::SH_AND_T   { "sh.and.t" };
const TC13::OpCode TC13::OpCode::SH_ANDN_T  { "sh.andn.t" };
const TC13::OpCode TC13::OpCode::SH_EQ      { "sh.eq" };
const TC13::OpCode TC13::OpCode::SH_GE      { "sh.ge" };
const TC13::OpCode TC13::OpCode::SH_GE_U    { "sh.ge.u" };
const TC13::OpCode TC13::OpCode::SH_H       { "sh.h" };
const TC13::OpCode TC13::OpCode::SH_LT      { "sh.lt" };
const TC13::OpCode TC13::OpCode::SH_LT_U    { "sh.lt.u" };
const TC13::OpCode TC13::OpCode::SH_NAND_T  { "sh.nand.t" };
const TC13::OpCode TC13::OpCode::SH_NE      { "sh.ne" };
const TC13::OpCode TC13::OpCode::SH_NOR_T   { "sh.nor.t" };
const TC13::OpCode TC13::OpCode::SH_OR_T    { "sh.or.t" };
const TC13::OpCode TC13::OpCode::SH_ORN_T   { "sh.orn.t" };
const TC13::OpCode TC13::OpCode::SH_XNOR_T  { "sh.xnor.t" };
const TC13::OpCode TC13::OpCode::SH_XOR_T   { "sh.xor.t" };
const TC13::OpCode TC13::OpCode::SHA        { "sha" };
const TC13::OpCode TC13::OpCode::SHA_H      { "sha.h" };
const TC13::OpCode TC13::OpCode::SHAS       { "shas" };
const TC13::OpCode TC13::OpCode::ST_A       { "st.a", WIR_OpCodeType::store };
const TC13::OpCode TC13::OpCode::ST_B       { "st.b", WIR_OpCodeType::store };
const TC13::OpCode TC13::OpCode::ST_D       { "st.d", WIR_OpCodeType::store };
const TC13::OpCode TC13::OpCode::ST_DA      { "st.da", WIR_OpCodeType::store };
const TC13::OpCode TC13::OpCode::ST_H       { "st.h", WIR_OpCodeType::store };
const TC13::OpCode TC13::OpCode::ST_Q       { "st.q", WIR_OpCodeType::store };
const TC13::OpCode TC13::OpCode::ST_T       { "st.t", WIR_OpCodeType::load,
                                              WIR_OpCodeType::store };
const TC13::OpCode TC13::OpCode::ST_W       { "st.w", WIR_OpCodeType::store };
const TC13::OpCode TC13::OpCode::STLCX      { "stlcx", WIR_OpCodeType::store };
const TC13::OpCode TC13::OpCode::STUCX      { "stucx", WIR_OpCodeType::store };
const TC13::OpCode TC13::OpCode::SUB        { "sub" };
const TC13::OpCode TC13::OpCode::SUB_A      { "sub.a" };
const TC13::OpCode TC13::OpCode::SUB_B      { "sub.b" };
const TC13::OpCode TC13::OpCode::SUB_F      { "sub.f" };
const TC13::OpCode TC13::OpCode::SUB_H      { "sub.h" };
const TC13::OpCode TC13::OpCode::SUBC       { "subc" };
const TC13::OpCode TC13::OpCode::SUBS       { "subs" };
const TC13::OpCode TC13::OpCode::SUBS_H     { "subs.h" };
const TC13::OpCode TC13::OpCode::SUBS_HU    { "subs.hu" };
const TC13::OpCode TC13::OpCode::SUBS_U     { "subs.u" };
const TC13::OpCode TC13::OpCode::SUBX       { "subx" };
const TC13::OpCode TC13::OpCode::SVLCX      { "svlcx", WIR_OpCodeType::store,
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::SWAP_W     { "swap.w", WIR_OpCodeType::load,
                                              WIR_OpCodeType::store };
const TC13::OpCode TC13::OpCode::SYSCALL    { "syscall",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::TLBDEMAP   { "tlbdemap",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::TLBFLUSH_A { "tlbflush.a",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::TLBFLUSH_B { "tlbflush.b",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::TLBMAP     { "tlbmap",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::TLBPROBE_A { "tlbprobe.a",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::TLBPROBE_I { "tlbprobe.i",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::TRAPSV     { "trapsv",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::TRAPV      { "trapv",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::UNPACK     { "unpack" };
const TC13::OpCode TC13::OpCode::UPDFL      { "updfl",
                                              WIR_OpCodeType::sideEffect };
const TC13::OpCode TC13::OpCode::UTOF       { "utof" };
const TC13::OpCode TC13::OpCode::XNOR       { "xnor" };
const TC13::OpCode TC13::OpCode::XNOR_T     { "xnor.t" };
const TC13::OpCode TC13::OpCode::XOR        { "xor" };
const TC13::OpCode TC13::OpCode::XOR_EQ     { "xor.eq" };
const TC13::OpCode TC13::OpCode::XOR_GE     { "xor.ge" };
const TC13::OpCode TC13::OpCode::XOR_GE_U   { "xor.ge.u" };
const TC13::OpCode TC13::OpCode::XOR_LT     { "xor.lt" };
const TC13::OpCode TC13::OpCode::XOR_LT_U   { "xor.lt.u" };
const TC13::OpCode TC13::OpCode::XOR_NE     { "xor.ne" };
const TC13::OpCode TC13::OpCode::XOR_T      { "xor.t" };

}       // namespace WIR
