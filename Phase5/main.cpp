#include "avm.h"
#include <stdio.h>

#define AVM_STACK_SIZE 1024

void printUsage(const char* programPath)
{
	fprintf(stdout, "%s <program.abc>\n", programPath);
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		printUsage(argv[0]);
		return 0;
	}

	fprintf(stdout, "Initializing Alpha VM...\n");
	AVM* avm = avmCreate(AVM_STACK_SIZE);
	if (!avm) {
		fprintf(stderr, "(x) Failed to create Alpha VM\n");
		return 1;
	}

	fprintf(stdout, "Loading %s into VM...\n", argv[1]);
	if (!avmLoadProgram(avm, argv[1])) {
		fprintf(stderr, "(x) Failed to load %s as program into Alpha VM.\n", argv[1]);
		avmDestroy(avm);
		return 1;
	}

	fprintf(stdout, "Executing program...\n");
	fprintf(stdout, "--------------------\n");
	avmRun(avm);
	fprintf(stdout, "\n--------------------\n");

	fprintf(stdout, "Destroying Alpha VM...\n");
	avmDestroy(avm);

	return 0;
}