#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdbool.h>
#include "./global.h"
#include "../Stack_lib/Headers/stack.h"


typedef struct
{
    unsigned char *byteCode;
    size_t nCommands;
    size_t size;
}ByteCodeStruct;

typedef struct
{
    char *name;
    size_t pointer;
}Label;

typedef struct
{
    unsigned char *byteCode;
    size_t *indexByteCode;
    size_t indexInCmdArr;
    Label *labels;
    char *inputCommand;
    char *inputArg;
}CmdToWriteInfo;


const int number_of_labels = 100;

ByteCodeStruct ProcReadFile(char *fileName);
size_t ReplaceRNto0(char *stringText, size_t fileSize);
void SetPointerArr(char *stringText, size_t fileSize, char **pointerArr);
ByteCodeStruct ParsTextToBytecode(size_t nlines, char **pointerArr);


void ParserCmdFromStr(char *pointerToCmd, unsigned char *byteCode, size_t *indexByteCode, Label *labels);
void DivideStrToPiece(char *pointerToCmd, char *inputCommand, char *inputArg);
void WriteArgToByteCode(unsigned char *byteCode, size_t *indexByteCode, void *arg, size_t size);

void FillLabel(CmdToWriteInfo *cmdInfo);
void WriteCmdToBytecode(CmdToWriteInfo *cmdInfo);
void WriteRegCmd(CmdToWriteInfo *cmdInfo);
void WriteOneArgCmd(CmdToWriteInfo *cmdInfo);


void WriteByteCode(char *fileName, ByteCodeStruct byteCodeStruct);

#endif
//end #ifndef ASSEMBLER_H
