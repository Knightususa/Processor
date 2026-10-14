#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdbool.h>
#include "./global.h"
#include "../Stack_lib/Headers/stack.h"

typedef struct
{
    unsigned char *byteCode;//TODO const unsigned char*
    size_t nCommands;
    size_t size;
}ByteCodeStruct;

extern const int number_of_labels;

ByteCodeStruct ReadFile(char *fileName);
size_t ReplaceRNto0(char *stringText, size_t fileSize);
void SetPointerArr(char *stringText, size_t fileSize, char **pointerArr);

void TakeCommand(char *pointerToComm, unsigned char *byteCode, size_t *indexByteCode, size_t *labels);
void DivideToPiece(char *pointerToComm, char *inputCommand, char *inputArg);
void WriteArgToByteCode(unsigned char *byteCode, size_t *indexByteCode, void *arg, size_t size);

void WriteByteCode(char *fileName, ByteCodeStruct byteCodeStruct);

#endif 
//end #ifndef ASSEMBLER_H