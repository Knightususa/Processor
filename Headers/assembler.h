#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdbool.h>
#include "./global.h"
#include "./stack.h"

typedef struct
{
    unsigned char *byteCode;
    size_t nCommands;
    size_t size;
}ByteCodeStruct;


ByteCodeStruct ReadFile(char *fileName);
size_t ReplaceRNto0(char *stringText, size_t fileSize);
void SetPointerArr(char *stringText, size_t fileSize, char **pointerArr);

void TakeCommand(char *pointerToComm, unsigned char *byteCode, size_t *indexByteCode);
const char *GetCommandName(COMMAND_INDEX commandIndex);
void DivideToPiece(char *pointerToComm, char *inputCommand, char *inputArg);
bool IsCommandHaveArg(COMMAND_INDEX index);
void WriteArgToByteCode(unsigned char *byteCode, size_t *indexByteCode, StackElem_t *arg);

void WriteByteCode(char *fileName, ByteCodeStruct byteCodeStruct);

#endif 
//end #ifndef ASSEMBLER_H