#ifndef AVM_LIB_H
#define AVM_LIB_H

struct AVM;

typedef void(*AVMLibraryFuncCallback)(AVM*);

AVMLibraryFuncCallback avmGetLibraryFunctionPointer(const char* name);

void avmCallLibraryFunc(AVM* avm, AVMLibraryFuncCallback f);

#endif
