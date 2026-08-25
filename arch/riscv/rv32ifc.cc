/*

  This source file belongs to the

    WCC Intermediate Representation (WIR) Framework

  Permission to use this software is granted according to the license provided
  with this software distribution, and according to the attribution section in
  the README file found in the top-level directory of this distribution.

  Copyright 2015 - 2026, Heiko Falk.

*/

/*!
  @file rv32ifc.cc
  @brief This file implements the specific interface of the RISC-V RV32IFC Base
         Integer instruction set plus the F Standard Extension for single-
         precision Floating-Point Instructions and the C Standard Extension for
         Compressed Instructions, version 2.0.

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
#include <arch/riscv/rv32ifc.h>


//
// Code Section
//

namespace WIR {


using namespace std;


//
// Public class methods
//

/*
  Default constructor for RV32IFC processor architectures.
*/
RV32IFC::RV32IFC( void ) :
  RV32IF {},
  RV32IC {}
{
  DSTART( "RV32IFC::RV32IFC()" );

  // Specify the processor architecture modeled by this class.
  setISAName( "RV32IFC" );
};


/*
  Copy constructor.
*/
RV32IFC::RV32IFC( const RV32IFC &__o ) :
  RV32I { __o },
  RV32IF { __o },
  RV32IC { __o }
{
  DSTART( "RV32IFC::RV32IFC(const RV32IFC&)" );
};


/*
  Move constructor.
*/
RV32IFC::RV32IFC( RV32IFC &&__o ) :
  RV32IF { __o },
  RV32IC { std::move( __o ) }
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );
};


/*
  Destructor.
*/
RV32IFC::~RV32IFC( void )
{
  DSTART( "virtual RV32IFC::~RV32IFC()" );
};


/*
  Copy-assignment operator.
*/
RV32IFC & RV32IFC::operator = ( const RV32IFC &__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IF::operator = ( __o );
  RV32IC::operator = ( __o );

  return( *this );
};


/*
  Move-assignment operator.
*/
RV32IFC & RV32IFC::operator = ( RV32IFC &&__o )
{
cout << BOOST_CURRENT_FUNCTION << endl;
  DSTART( BOOST_CURRENT_FUNCTION );

  RV32IF::operator = ( __o );
  RV32IC::operator = ( std::move( __o ) );

  return( *this );
};


/*
  init performs some global initialization tasks for RV32IFC processor
  architectures.

  This includes setting up the assignment of valid operation formats to RV32IFC
  opcodes. init, however, only registers the RV32IFC processor, as all OpCodes
  and OperationFormats have already been registerd by its base classes.
*/
// cppcheck-suppress duplInheritedMember
void RV32IFC::init( void )
{
  DSTART( "static void RV32IFC::init()" );

  //
  // Register this current processor model.
  //

  registerProcessor( RV32IFC() );
};


/*
  adjustStack allocates additional space in the specified function's stack
  frame and adjusts all stack-related memory accesses accordingly.

  According to the RISC-V ABI (section 2.1, Integer Calling Convention), the
  stack grows downwards (towards lower addresses) and the stack pointer shall be
  aligned to a 128-bit boundary upon procedure entry. The first argument passed
  on the stack is located at offset zero of the stack pointer on function entry;
  following arguments are stored at correspondingly higher addresses.

  In the standard ABI, the stack pointer must remain aligned throughout
  procedure execution. [...]

  Procedures must not rely upon the persistence of stack-allocated data whose
  addresses lie below the stack pointer.

  (Stack
    growing
    direction)
        |
        |   +-------------------------+      (high address)
        |   | Local Variables Func 1  |
        |   +-------------------------+
        |   | Argument Area for func- |
        |   | tions called by Func 1  |      (first argument passed on stack)
        |   +-------------------------+
        |   | Local Variables Func 2  |
        |   +-------------------------+
        |   | Argument Area for func- |
        |   | tions called by Func 2  |
        |   +-------------------------+ <--- Stack Pointer (SP) at entry
        V   | Local Variables Func 3  |      (CALL) to Function 3
            +-------------------------+
            | Argument Area for func- |
            | tions called by Func 3  |
            +-------------------------+ <--- Stack Pointer (SP) after stack
            |                         |      allocation of Function 3
            |           ...           |
            +-------------------------+      (low address)
*/
void RV32IFC::adjustStack( WIR_Function &f, int size,
                           const std::list<std::reference_wrapper<WIR_Instruction>> &insertedSpillCode )
{
  DSTART(
    "static void RV32IFC::adjustStack(WIR_Function&, int, const "
    "list<reference_wrapper<WIR_Instruction> >&)" );

  DOUT(
    "Adjusting stack of function '" << f.getName() << "' by " << size <<
    " bytes." << endl );

  if ( size == 0 )
    return;

  // Align stack pointer to 16 bytes.
  int remainder = size % 16;
  if ( remainder != 0 ) {
    size += 16 - remainder;
    DOUT( "Aligning to next 16-byte boundary of " << size << "." << endl );
  }

  // Determine the involved processor core.
  WIR_System &sys = f.getCompilationUnit().getSystem();
  const WIR_Section &sec = sys.findSymbol( f ).getSection();
  RV32I &rv = dynamic_cast<RV32I &>( sec.getProcessor() );

  auto &stackPointer = rv.SP();

  // Convert list insertedSpillCode into a set of IDs for convenience.
  set<WIR_id_t> spillCode;
  for ( const WIR_Instruction &i : insertedSpillCode )
    spillCode.insert( i.getID() );

  // Determine the first non-empty instruction and operation of f.
  WIR_Instruction *iid = nullptr;

  for ( const WIR_BasicBlock &b : f ) {
    auto it =
      find_if(
        b.begin(), b.end(),
        [&]( reference_wrapper<WIR_Instruction> ref ) {
          return( !ref.get().getOperations().empty() ); } );
    if ( it != b.end() )
      iid = &(it->get());

    if ( iid != nullptr )
      break;
  }

  if ( iid == nullptr )
    return;

  set<WIR_id_t> sacIDs;
  auto &firstIns = *iid;
  WIR_Operation &oldSAC = firstIns.getOperations().front();

  // Determine potential second instruction for stack allocation.
  auto secondOp = firstIns.getOperations().end();
  // auto &secondIns = firstIns;
  bool secondOpFound = false;
  for ( auto it =
          std::next( firstIns.getBasicBlock().findInstruction( firstIns ) );
        it != firstIns.getBasicBlock().getInstructions().end(); ++it )
    if ( !it->get().getOperations().empty() ) {
      secondOp = it->get().getOperations().begin();
      secondOpFound = true;
      break;
    }

  // Determine potential third instruction for stack allocation.
  auto thirdOp = firstIns.getOperations().end();
  bool thirdOpFound = false;
  if ( secondOpFound )
    for ( auto it =
            std::next(
              secondOp->get().getInstruction().getBasicBlock().findInstruction(
                secondOp->get().getInstruction() ) );
          it !=
            secondOp->get().getInstruction().getBasicBlock().getInstructions().end();
          ++it )
      if ( !it->get().getOperations().empty() ) {
        thirdOp = it->get().getOperations().begin();
        thirdOpFound = true;
        break;
      }

  // Test if f already has some stack-allocating code (SAC) and adjust it.
  if ( isStackPointerADDI( oldSAC ) ) {
    DOUT( "Adjusting ADDI stack-allocating code." << endl );

    auto pos = std::prev( oldSAC.getParameters().end() );

    int oldStackFrameSize =
      dynamic_cast<RV_Const12_Signed &>( pos->get() ).getValue();

    oldSAC.replaceParameter(
      pos, RV_Const12_Signed( oldStackFrameSize - size ) );

    sacIDs.insert( oldSAC.getID() );

    // TODO: Add support for CADDI16SP stack-allocating code.
  } else

  if ( thirdOpFound &&
       isStackPointerLUIADDIADD( oldSAC, *secondOp, *thirdOp ) ) {
    DOUT( "Adjusting LUI+ADDI+ADD stack-allocating code." << endl );

    WIR_Operation &lui = oldSAC;
    WIR_Operation &addi = secondOp->get();

    auto lui_pos = std::prev( lui.getParameters().end() );
    unsigned int lui_const =
      dynamic_cast<RV_Const20_Unsigned &>( lui_pos->get() ).getValue();

    auto addi_pos = std::prev( addi.getParameters().end() );
    signed int addi_const =
      dynamic_cast<RV_Const12_Signed &>( addi_pos->get() ).getValue();

    long oldStackFrameSize = ( lui_const * 0x1000 ) + addi_const;
    long newStackFrameSize = oldStackFrameSize - size;

    DACTION(
      if ( oldStackFrameSize & 0x80000000 ) {
        long twosComplement = (~oldStackFrameSize + 1) & 0xFFFFFFFF;
        DOUT(
          "Old stack frame size: " << oldStackFrameSize << " (0x" << hex <<
          oldStackFrameSize << dec << ", -" << twosComplement << ")" << endl );
      } else
        DOUT(
          "Old stack frame size: " << oldStackFrameSize << " (0x" << hex <<
          oldStackFrameSize << dec << ")" << endl );

      if ( newStackFrameSize & 0x80000000 ) {
        long twosComplement = (~newStackFrameSize + 1) & 0xFFFFFFFF;
        DOUT(
          "New stack frame size: " << newStackFrameSize << " (0x" << hex <<
          newStackFrameSize << dec << ", -" << twosComplement << ")" << endl );
      } else
        DOUT(
          "New stack frame size: " << newStackFrameSize << " (0x" << hex <<
          newStackFrameSize << dec << ")" << endl ); );

    unsigned int lui_new_const =
      ( ( newStackFrameSize + 0x800 ) / (unsigned int) 0x1000 ) & 0xFFFFF;
    int addi_new_const = newStackFrameSize & 0xFFF;

    if ( addi_new_const > RV_Const12_Signed::getMaxValue( 12 ) )
      addi_new_const =
        RV_Const12_Signed::getMinValue( 12 ) - 1 +
        ( addi_new_const - RV_Const12_Signed::getMaxValue( 12 ) );
    else

    if ( addi_new_const < RV_Const12_Signed::getMinValue( 12 ) )
      addi_new_const =
        RV_Const12_Signed::getMaxValue( 12 ) +
        ( addi_new_const - RV_Const12_Signed::getMinValue( 12 ) );

    lui.replaceParameter( lui_pos, RV_Const20_Unsigned( lui_new_const ) );
    addi.replaceParameter( addi_pos, RV_Const12_Signed( addi_new_const ) );

    sacIDs.insert( lui.getID() );
    sacIDs.insert( addi.getID() );
    sacIDs.insert( thirdOp->get().getID() );
  } else {
    // No SAC.
    auto &addi1 =
      f.getBasicBlocks().front().get().insertInstruction(
        f.getBasicBlocks().front().get().begin(), {} )->get();
    addi1.pushBackOperation(
      { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1,
        WIR_RegisterParameter( stackPointer, WIR_Usage::def ),
        WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
        RV_Const12_Signed( (int) -size ) } );

    sacIDs.insert( addi1.getOperations().front().get().getID() );
    DOUT( "Inserting ADDI stack-allocating code." << riscv << endl << addi1 );
  }

  // Adjust all old accesses to the stack.
  for ( WIR_BasicBlock &b : f )
    for ( auto it = b.begin(); it != b.end(); ++it ) {
      WIR_Instruction &i = it->get();

      // Adjust stack access only if it's neither a regular spill instruction
      // nor an access to the overflow area used for argument passing. The
      // RISC-V ABI mandates that overflow arguments must lie at stack pointer
      // offset 0 and higher.
      // This bottom of the stack frame thus must not be used for spilling or
      // local data. Spill instructions are already generated by the RISC-V
      // register allocator (methods insertSpillLoad and insertSpillStore) such
      // that their offsets are right above the overflow region.
      // Due to the fact that the register allocator places the spill region
      // between the overflow region (bottom of stack frame) and the local data
      // region (top of stack frame), all accesses to the local data region thus
      // have invalid offsets. We thus need to adjust them in the following.
      //
      // TODO: This adjustment of local data stack accesses could be avoided if
      //       the spill region is put at the top of the stack frame.

      // Check whether the current instruction contains parameters that are
      // marked as don't touch since they access the overflow area.
      bool isOverflowAccess = false;
      for ( WIR_Operation &o : i )
        for ( WIR_Parameter &p : o )
          if ( p.getDontOptimize() ) {
            isOverflowAccess = true;
            p.setDontOptimize( false );
          }

      // Adjust stack access only if it's neither an access to the overflow
      // region nor a regular spill instruction.
      if ( !isOverflowAccess && !spillCode.count( i.getID() ) &&
           !i.getOperations().empty() ) {
        WIR_Operation &o = i.getOperations().front().get();

        DOUT( "Checking operation " << riscv << o << endl );

        // Check for stack pointer-relative offset computations using ADDI or
        // the MOV pseudo-operation.
        bool spOffset =
          ( ( o.getOpCode() == RV32I::OpCode::ADDI ) &&
            ( o.getOperationFormat() == RV32IC::OperationFormat::RRC12_1 ) &&
            isSP(
              dynamic_cast<WIR_RegisterParameter &>(
                o.getExplicitParameter( 2 ) ).getRegister() ) ) ||
          ( ( o.getOpCode() == RV32I::OpCode::MOV ) &&
            ( o.getOperationFormat() == RV32I::OperationFormat::RR_1 ) &&
            isSP(
              dynamic_cast<WIR_RegisterParameter &>(
                o.getExplicitParameter( 2 ) ).getRegister() ) );

        if ( !( o.isImplicitMemoryAccess() || o.isMemoryLoad() ||
                o.isMemoryStore() || spOffset ) ) {
          DOUT( "Skipping current operation. (1)" << endl );
          continue;
        }

        // Don't adjust the original operation for stack frame allocation again!
        if ( sacIDs.count( o.getID() ) ) {
          DOUT( "Skipping current operation. (2)" << endl );
          continue;
        }

        auto &p = o.getExplicitParameters();

        // Adjust any kind of base+offset addressing using the stack pointer.

        // This lambda is used to obtain an unsigned constant argument value.
        auto getReg = [&]( unsigned int i ) -> WIR_BaseRegister & {
          return(
            dynamic_cast<WIR_RegisterParameter &>(
              o.getExplicitParameter( i ) ).getRegister() );
        };

        // The operation formats of load and store store instructions, the MOV
        // pseudo-operation and the instruction to set the new FP need to be
        // checked.
        bool rc12r_1 =
          ( o.getOperationFormat() == RV32I::OperationFormat::RC12R_1 ) &&
          isSP( getReg( 3 ) );
        bool rc12r_2 =
          ( o.getOperationFormat() == RV32I::OperationFormat::RC12R_2 ) &&
          isSP( getReg( 3 ) );
        bool rr_1 =
          ( o.getOperationFormat() == RV32I::OperationFormat::RR_1 ) &&
          isSP( getReg( 2 ) );
        bool src7r_1 =
          ( o.getOperationFormat() == RV32IC::OperationFormat::SRC7R_1 ) &&
          isSP( getReg( 3 ) );
        bool src7r_2 =
          ( o.getOperationFormat() == RV32IC::OperationFormat::SRC7R_2 ) &&
          isSP( getReg( 3 ) );
        bool src8r_1 =
          ( o.getOperationFormat() == RV32IC::OperationFormat::SRC8R_1 ) &&
          isSP( getReg( 3 ) );
        bool src8r_2 =
          ( o.getOperationFormat() == RV32IC::OperationFormat::SRC8R_2 ) &&
          isSP( getReg( 3 ) );
        bool rlr_1 =
          ( o.getOperationFormat() == RV32IC::OperationFormat::RLR_1 ) &&
          isSP( getReg( 3 ) );
        bool rlr_2 =
          ( o.getOperationFormat() == RV32IC::OperationFormat::RLR_2 ) &&
          isSP( getReg( 3 ) );
        bool fc12r_1 =
          ( o.getOperationFormat() == RV32IF::OperationFormat::FC12R_1 ) &&
          isSP( getReg( 3 ) );
        bool fc12r_2 =
          ( o.getOperationFormat() == RV32IF::OperationFormat::FC12R_2 ) &&
          isSP( getReg( 3 ) );

        if ( rc12r_1 || rc12r_2 || rr_1 || src7r_1 || src7r_2 || src8r_1 ||
             src8r_2 || rlr_1 || rlr_2 || fc12r_1 || fc12r_2 || spOffset ) {
          DOUT( "Adjusting stack access in " << o << endl );

          if ( rr_1 ) {
            long long newOffset = size;

            if ( ( newOffset > RV_Const12_Signed::getMinValue( 12 ) ) &&
                 ( newOffset <= RV_Const12_Signed::getMaxValue( 12 ) ) ) {
              DOUT( "Replacing MOV by ADDI." << endl );

              i.replaceOperation(
                i.getOperations().begin(),
                { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1,
                  WIR_RegisterParameter(
                    dynamic_cast<WIR_RegisterParameter &>( p.front().get() ) ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                  RV_Const12_Signed( newOffset ) } );
            } else {
              DOUT( "Replacing MOV by LUI, ADDI and ADD." );

              unsigned int luiConst =
                ( ( newOffset + 0x800 ) / (unsigned int) 0x1000 ) & 0xFFFFF;
              int addiConst = newOffset & 0xFFF;

              if ( addiConst > RV_Const12_Signed::getMaxValue( 12 ) )
                addiConst =
                  RV_Const12_Signed::getMinValue( 12 ) - 1 +
                  ( addiConst - RV_Const12_Signed::getMaxValue( 12 ) );
              else

              if ( addiConst < RV_Const12_Signed::getMinValue( 12 ) )
                addiConst =
                  RV_Const12_Signed::getMaxValue( 12 ) +
                  ( addiConst - RV_Const12_Signed::getMinValue( 12 ) );

              b.insertInstruction( it,
                { { RV32I::OpCode::LUI, RV32I::OperationFormat::RC20_1,
                    WIR_RegisterParameter(
                      dynamic_cast<WIR_RegisterParameter &>(
                        p.front().get() ) ),
                    RV_Const20_Unsigned( luiConst ) } } );

              if ( addiConst != 0 )
                b.insertInstruction( it,
                  { { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1,
                      WIR_RegisterParameter(
                        dynamic_cast<WIR_RegisterParameter &>(
                          p.front().get() ) ),
                      WIR_RegisterParameter(
                        dynamic_cast<WIR_RegisterParameter &>(
                          p.front().get() ).getRegister(),
                        WIR_Usage::use ),
                      RV_Const12_Signed( addiConst ) } } );

              i.replaceOperation(
                i.getOperations().begin(),
                { RV32I::OpCode::ADD, RV32I::OperationFormat::RRR_1,
                  WIR_RegisterParameter(
                    dynamic_cast<WIR_RegisterParameter &>( p.front().get() ) ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                  WIR_RegisterParameter(
                    dynamic_cast<WIR_RegisterParameter &>(
                      p.front().get() ).getRegister(),
                    WIR_Usage::use ) } );
            }

            continue;
          }

          // Determine the immediate parameter and current stack offset.
          auto pos =
            find_if(
              p.begin(), p.end(),
              [&]( reference_wrapper<WIR_Parameter> ref ) {
                return( ref.get().getType() == WIR_ParameterType::imm ); } );

          // cppcheck-suppress constVariableReference
          auto &ip = dynamic_cast<WIR_BaseImmediateParameter &>( pos->get() );
          long long currentOffset =
            ip.isSigned() ? ip.getSignedValue() : ip.getUnsignedValue();

          // TODO: Add proper handling of C.LW, C.LWSP, C.SW, C.SWSP!
          // TODO: Add proper handling of floating-point load/stores!

          if ( currentOffset >= 0 ) {
            long long newOffset = currentOffset + size;

            if ( ( o.getOpCode() == RV32I::OpCode::LW )  &&
                 ( newOffset > RV_Const12_Signed::getMinValue( 12 ) ) &&
                 ( newOffset <= RV_Const12_Signed::getMaxValue( 12 ) ) ) {
              DOUT( "Replacing LW." << endl );
              // TODO: Where are SW's adjusted???

              // Replace LW offset parameter.
              i.replaceOperation(
                i.getOperations().begin(),
                { RV32I::OpCode::LW, RV32I::OperationFormat::RC12R_1,
                  WIR_RegisterParameter(
                    dynamic_cast<WIR_RegisterParameter &>( p.front().get() ) ),
                  RV_Const12_Signed( newOffset ),
                  WIR_RegisterParameter( stackPointer, WIR_Usage::use ) } );
            } else

            if ( newOffset > RV_Const12_Signed::getMaxValue( 12 ) ) {
              // The new offset does not fit into a 12-bits immediate.
              DOUT(
                "Generating two ADDIs before/after the current operation." <<
                endl );

              // TODO: The following manipulation of SP violates the RISC-V ABI
              //       which states that "Procedures must not rely upon the
              //       persistence of stack-allocated data whose addresses lie
              //       below the stack pointer." (RISC-V ABI, Section 2.1, Page
              //       6). In the future, some different register needs to be
              //       identified and used here.
              //
              //       RV_GraphColoring::isAdjustedLoadOrStoreInstruction needs
              //       to be rewritten in accordance with changes here, since it
              //       checks for exactly this case.

              // TODO: What has to happen below if size itself does not fit into
              //       the 12 bits immediates for the two ADDIs?

              // TODO: The code below only adds ADDI instructions but leaves the
              //       stack access ifself, i.e., the LW/SW, unchanged! Is this
              //       correct?
              b.insertInstruction( it,
                { { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1,
                    WIR_RegisterParameter( stackPointer, WIR_Usage::def ),
                    WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                    RV_Const12_Signed( size ) } } );
              b.insertInstruction( std::next( it ),
                { { RV32I::OpCode::ADDI, RV32I::OperationFormat::RRC12_1,
                    WIR_RegisterParameter( stackPointer, WIR_Usage::def ),
                    WIR_RegisterParameter( stackPointer, WIR_Usage::use ),
                    RV_Const12_Signed( -size ) } } );
            } else {
              DOUT( "Replacing stack offset parameter." << endl );

              // The adjusted SP-relative offset is small enough to still fit
              // into the current operation.
              dynamic_cast<RV_Const12_Signed &>( ip ).setValue( newOffset );
            }
          }
        }
      }
    }
};


//
// Private class methods
//

/*
  clone creates a copy of an RV32IFC processor.
*/
WIR_BaseProcessor *RV32IFC::clone( void ) const
{
  DSTART( "virtual WIR_BaseProcessor* RV32IFC::clone() const" );

  return ( new RV32IFC( *this ) );
};

}       // namespace WIR
