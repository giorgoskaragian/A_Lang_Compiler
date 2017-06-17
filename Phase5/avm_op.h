#ifndef AVM_OP_H
#define AVM_OP_H

struct AVM;
struct AVMInstruction;
struct AVMMemoryCell;

#define AVM_STACK_ENV_SIZE            4
#define AVM_NUM_FUNCTION_ARGS_OFFSET  4
#define AVM_SAVED_PC_OFFSET           3
#define AVM_SAVED_TOP_OFFSET          2
#define AVM_SAVED_TOPSP_OFFSET        1

unsigned int avmGetTotalFunctionArguments(AVM* avm);
AVMMemoryCell* avmGetFunctionArgument(AVM* avm, unsigned int index);
void avmCallSaveEnv(AVM* avm);

void avmOpExecute_assign(AVM* avm, AVMInstruction* instr);
void avmOpExecute_BinaryArithmetic(AVM* avm, AVMInstruction* instr);
void avmOpExecute_jmp(AVM* avm, AVMInstruction* instr);
void avmOpExecute_jeq(AVM* avm, AVMInstruction* instr);
void avmOpExecute_jne(AVM* avm, AVMInstruction* instr);
void avmOpExecute_jle(AVM* avm, AVMInstruction* instr);
void avmOpExecute_jge(AVM* avm, AVMInstruction* instr);
void avmOpExecute_jlt(AVM* avm, AVMInstruction* instr);
void avmOpExecute_jgt(AVM* avm, AVMInstruction* instr);
void avmOpExecute_call(AVM* avm, AVMInstruction* instr);
void avmOpExecute_pusharg(AVM* avm, AVMInstruction* instr);
void avmOpExecute_funcenter(AVM* avm, AVMInstruction* instr);
void avmOpExecute_funcexit(AVM* avm, AVMInstruction* instr);
void avmOpExecute_tablecreate(AVM* avm, AVMInstruction* instr);
void avmOpExecute_tablegetelem(AVM* avm, AVMInstruction* instr);
void avmOpExecute_tablesetelem(AVM* avm, AVMInstruction* instr);
void avmOpExecute_nop(AVM* avm, AVMInstruction* instr);

#endif
