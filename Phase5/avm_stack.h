#ifndef AVM_STACK_H
#define AVM_STACK_H

struct AVM;
struct AVMMemoryCell;

void avmStackPush(AVM* avm, const AVMMemoryCell* cell);
void avmStackPushEnvValue(AVM* avm, unsigned int val);
unsigned int avmStackGetEnvValue(AVM* avm, unsigned int topSPRelativeIndex);

#endif
