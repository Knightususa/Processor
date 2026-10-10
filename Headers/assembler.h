#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdbool.h>
#include "./global.h"
#include "./stack.h"

command *ReadFile(char *fileName, int *nCommands);
command TakeCommand(char *pointerToComm);
size_t ReplaceRNto0(char *stringText, size_t fileSize);
void SetPointerArr(char *stringText, size_t fileSize, char **pointerArr);
void DivideToPiece(char *pointerToComm, char *inputCommand, char *inputArg);
const char *GetCommandName(COMMAND_INDEX commandIndex);


#endif 
//end #ifndef ASSEMBLER_H