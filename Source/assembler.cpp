#include "../Headers/common.h"
#include "../Headers/assembler.h"
#include "../Stack_lib/Headers/stack.h"
#include "../Headers/processor_func.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


ByteCodeStruct ProcReadFile(char *fileName)
{
    char mode[] = "rb";
    FILE *pFile = safe_fopen(fileName, mode);

    struct stat fileStat = {};
    fstat(fileno(pFile), &fileStat);
    size_t fileSize = fileStat.st_size;
    
    // printf("Size of file = " RED "%lli bytes\n\n\n" RESET, fileSize);
    char *stringText = (char *)safe_calloc((size_t)fileSize + 1, sizeof(char));
    
    size_t nReadBytes = safe_fread((void *) stringText, sizeof(char), fileSize, pFile);
    warning (nReadBytes == fileSize, "nReadBytes not equals fileSize");
    yaissert(nReadBytes == fileSize);
    
    size_t nlines = ReplaceRNto0(stringText, fileSize);
    
    char **pointerArr = (char **)safe_calloc((size_t)nlines, sizeof(char *));;
    SetPointerArr(stringText, fileSize, pointerArr);
    ByteCodeStruct byteCodeStruct = ParsTextToBytecode(nlines, pointerArr);
    fclose(pFile);
    return byteCodeStruct;
}

ByteCodeStruct ParsTextToBytecode(size_t nlines, char **pointerArr)
{
    ByteCodeStruct byteCodeStruct;
    byteCodeStruct.nCommands = nlines;
    byteCodeStruct.size = (sizeof(StackElem_t) + CommandMaxLen) * nlines;
    byteCodeStruct.byteCode = (unsigned char*) safe_calloc(byteCodeStruct.size, sizeof(unsigned char));
    
    Label *labels = (Label *)safe_calloc(number_of_labels, sizeof(Label));
    for(int i = 0; i < number_of_labels; i++)
    {
        labels[i].name = NULL;
        labels[i].pointer = 0;
    }

    size_t indexByteCode = 0;
    for(int j = 0; j < 2; j++)
    {
        indexByteCode = 0;
        for(size_t i = 0; i < nlines; i++)
            ParserCmdFromStr(pointerArr[i], byteCodeStruct.byteCode, &indexByteCode, labels);
        // for(int i = 0; i < number_of_labels; i++)
        //     if(labels[i].name != NULL) printf("Name = %s, pointer = %lli\n", labels[i].name, labels[i].pointer);
    }

    if(indexByteCode < byteCodeStruct.size)
    {
        byteCodeStruct.size = indexByteCode * sizeof(unsigned char);
        byteCodeStruct.byteCode = (unsigned char *)realloc(byteCodeStruct.byteCode, byteCodeStruct.size);
    }


    for(int i = 0; i < number_of_labels; i++)
        free(labels[i].name);
    safe_free(labels, number_of_labels * sizeof(Label));
    return byteCodeStruct;
}

size_t ReplaceRNto0(char *stringText, size_t fileSize)
{
    size_t i = 0;
    size_t nlines = 0;

    while (true)
    {
        char charInArr = stringText[i];

        while(charInArr == ' ')
        {
            stringText[i++] = '\0';
            charInArr = stringText[i];
        }

        while(charInArr != '\n' && charInArr != '\r' && charInArr != '\0' && i < fileSize + 1)
        {
            i++;
            charInArr = stringText[i];
        }

        if(charInArr == '\r' || charInArr == '\n')
        {
            while(stringText[i] == '\r' || stringText[i] == '\n')
                stringText[i++] = '\0';
            nlines++;
        }

        else if(charInArr == '\0' || i == fileSize)
        {
            nlines++;
            stringText[i] = '\0';
            break;
        }
    }
    return nlines;
}

void SetPointerArr(char *stringText, size_t fileSize, char **pointerArr)
{
    warning (stringText != NULL, "Pointer to stringText is NULL");
    yaissert(stringText != NULL);

    warning (fileSize != 0, "fileSize equal 0");
    yaissert(fileSize != 0);

    warning (pointerArr != NULL, "Pointer to pointerArr is NULL");
    yaissert(pointerArr != NULL);
    size_t i = 0;
    int indexLine = 0;
    pointerArr[indexLine++] = &(stringText[0]);
    
    while (true)
    {
        char charInArr = stringText[i];
        while(charInArr != '\0' && i < fileSize)
        {
            i++;
            charInArr = stringText[i];
        }

        if(i < fileSize)
        {
            while(stringText[i] == '\0' && i < fileSize)
                i++;
            if(i < fileSize)
                pointerArr[indexLine++] = &(stringText[i]);
        }
        if(i >= fileSize)
            break;
    }
}


void WriteByteCode(char *fileName, ByteCodeStruct byteCodeStruct)
{
    char mode[] = "wb";
    FILE *pFile = safe_fopen(fileName, mode);

    write(fileno(pFile), byteCodeStruct.byteCode, (unsigned int)byteCodeStruct.size);

    fclose(pFile);
}




void ParserCmdFromStr(char *pointerToCmd, unsigned char *byteCode, size_t *indexByteCode, Label *labels)
{
    char inputCommand[CommandMaxLen + 1] = "";
    char inputArg[ArgumentMaxLen + 1] = "";
    inputCommand[CommandMaxLen] = '\0';
    inputArg[ArgumentMaxLen] = '\0';

    DivideStrToPiece(pointerToCmd, inputCommand, inputArg);

    // printf("Command name = %s\n", inputCommand);
    // printf("arg name = %s\n\n", inputArg);
    CmdToWriteInfo cmdInfoValue = 
    {
        .byteCode      = byteCode,
        .indexByteCode = indexByteCode,
        .labels        = labels,
        .inputCommand  = inputCommand,
        .inputArg      = inputArg,
    };
    CmdToWriteInfo *cmdInfo = &cmdInfoValue;
          
    if(inputCommand[0] == ':')
    {
        FillLabel(cmdInfo);
        return;
    }
    for(size_t i = 0; i < number_of_commands_ - 1; i++)
        if(strcmp(inputCommand, commands_[i].name) == 0)//TODO: add strcmp for int
        {
            cmdInfo->indexInCmdArr = i;
            WriteCmdToBytecode(cmdInfo);
            return;
        }
    byteCode[(*indexByteCode)++] = (unsigned char)notcmd_index;
}




void WriteArgToByteCode(unsigned char *byteCode, size_t *indexByteCode, void *arg, size_t size)
{
    for(size_t i = 0; i < size; i++)
        byteCode[(*indexByteCode)++] = (unsigned char)*((unsigned char *)arg + i);
}

void DivideStrToPiece(char *pointerToCmd, char *inputCommand, char *inputArg)
{
    sscanf(pointerToCmd, "%" MAKEVALUE(CommandMaxLen) "[^ \n] %" MAKEVALUE(ArgumentMaxLen) "[^ \n]", inputCommand, inputArg);
}

void FillLabel(CmdToWriteInfo *cmdInfo)
{
    char *inputCommand1 = cmdInfo->inputCommand + 1;
    for(int i = 0; i < number_of_labels; i++)
    {
        if(cmdInfo->labels[i].name == NULL)
        {
            cmdInfo->labels[i].name = strdup(inputCommand1); 
            cmdInfo->labels[i].pointer = *cmdInfo->indexByteCode;
            return;
        }

        if(strcmp(cmdInfo->labels[i].name, inputCommand1) == 0)
            return;

        if(i == number_of_labels - 1 && cmdInfo->labels[i].name != NULL)
        {
            printf("Entered %s, but it cant be a label because its already maximum of labels\n", inputCommand1);
            return;
        }
    }
}

void WriteCmdToBytecode(CmdToWriteInfo *cmdInfo)
{
    COMMAND_INDEX index = commands_[cmdInfo->indexInCmdArr].index;
    if(index == pusr_index || index == popr_index)
    {
        WriteRegCmd(cmdInfo);
        return;
    }

    else if (commands_[cmdInfo->indexInCmdArr].args == 1)
    {
        WriteOneArgCmd(cmdInfo);
        return;
    }
    cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = (unsigned char)index;
}

void WriteRegCmd(CmdToWriteInfo *cmdInfo)
{
    COMMAND_INDEX index = commands_[cmdInfo->indexInCmdArr].index;
    if(cmdInfo->inputArg[1] == 'X')
    {
        if('A' <= cmdInfo->inputArg[0] && cmdInfo->inputArg[0] <= 'D')
        {
            cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = (unsigned char)index;

            unsigned char registerIndex = (unsigned char) (cmdInfo->inputArg[0] - 'A' + 1);
            cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = registerIndex;
            return;
        }
    }
    cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = (unsigned char)notcmd_index;
}

void WriteOneArgCmd(CmdToWriteInfo *cmdInfo)
{
    COMMAND_INDEX index = commands_[cmdInfo->indexInCmdArr].index;
    if(cmdInfo->inputArg[0] == ':')
    {
        char *inputArg1 = cmdInfo->inputArg + 1;
        for(int i = 0; i < number_of_labels; i++)
        {
            if(cmdInfo->labels[i].name != NULL && strcmp(cmdInfo->labels[i].name, inputArg1) == 0)
            {
                size_t arg = cmdInfo->labels[i].pointer;
                cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = (unsigned char)index;
                WriteArgToByteCode(cmdInfo->byteCode, cmdInfo->indexByteCode, (void *)&arg, sizeof(size_t));
                return;
            }
        }
        size_t arg = 0;
        cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = (unsigned char)index;
        WriteArgToByteCode(cmdInfo->byteCode, cmdInfo->indexByteCode, (void *)&arg, sizeof(size_t));
        return;
    }
    StackElem_t arg = 0;
    if(sscanf(cmdInfo->inputArg, "%" PRINTF_T, &arg) != 1)
    {
        printf("Invalid numeric argument: <%s>\n", cmdInfo->inputArg);
        cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = (unsigned char)notcmd_index;
        WriteArgToByteCode(cmdInfo->byteCode, cmdInfo->indexByteCode, &arg, sizeof(StackElem_t));
        return;
    }
    cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = (unsigned char)index;
    WriteArgToByteCode(cmdInfo->byteCode, cmdInfo->indexByteCode, &arg, sizeof(StackElem_t));
}

// int StrCmpInt(char *firstString, char *SecondString)//TODO: Finish it
// {
//     char *as = firstString;
//     char *bs = SecondString;

//     int ai = 0;
//     int bi = 0;

//     int i = 0;
//     for(i < CommandMaxLen)
//     {
//         ai += 
//     }
//     return a[i] - 
// }