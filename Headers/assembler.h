#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdbool.h>
#include "./global.h"
#include "../Stack_lib/Headers/stack.h"


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

typedef struct
{
    char *filename;
}OptionsAsm;

const int number_of_labels = 100;

void GetOptionsAsm(int argc, char **argv, OptionsAsm *pOptions);

void DelNameExt(OptionsAsm *pOptions);
char *CreateName(char *filename, const char *ext);

ByteCodeStruct ProcReadFile(char *fileName);
size_t ReplaceRNto0(char *stringText, size_t fileSize);
void SetPointerArr(char *stringText, size_t fileSize, char **pointerArr);
ByteCodeStruct ParsTextToBytecode(size_t nlines, char **pointerArr);


void ParserCmdFromStr(char *pointerToCmd, unsigned char *byteCode, size_t *indexByteCode, Label *labels);
void DivideStrToPiece(char *pointerToCmd, char *inputCommand, char *inputArg);
void WriteArgToByteCode(unsigned char *byteCode, size_t *indexByteCode, void *arg, size_t size);

void FillLabel(CmdToWriteInfo *cmdInfo);
void WriteCmdToBytecode(CmdToWriteInfo *cmdInfo);
void WriteIfHaveLabel(CmdToWriteInfo *cmdInfo, unsigned char index);

void WriteOneArgCmd(CmdToWriteInfo *cmdInfo);
void WriteIfHaveFields(CmdToWriteInfo *cmdInfo, unsigned char index);
void WriteIfHaveFields1Arg(View indexP, CmdToWriteInfo* cmdInfo, char *firstStr, bool is_VRAM);
void WriteIfHaveFields2Arg(View indexP, CmdToWriteInfo* cmdInfo, char *firstStr, char *secondStr, bool is_VRAM);


bool IsIndexEqual(unsigned char index1, unsigned char index2);

void WriteByteCode(char *fileName, ByteCodeStruct byteCodeStruct);

void CheckCmdInfo(CmdToWriteInfo *cmdInfo);

#endif
//end #ifndef ASSEMBLER_H
