#include "../Headers/common.h"
#include "../Headers/assembler.h"
#include "../Headers/stack.h"
#include "../Headers/processor_func.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


ByteCodeStruct ReadFile(char *fileName)
{
    char mode[] = "rb";
    FILE *pFile = safe_fopen(fileName, mode);//TODO: safe open

    struct stat fileStat = {};
    fstat(fileno(pFile), &fileStat);
    size_t fileSize = fileStat.st_size;

    printf("Size of file = " RED "%lli bytes\n\n\n" RESET, fileSize);
    char *stringText = (char *)safe_calloc((size_t)fileSize + 1, sizeof(char));

    size_t nReadBytes = safe_fread((void *) stringText, sizeof(char), fileSize, pFile);//TODO: safe fread 
    warning (nReadBytes == fileSize, "nReadBytes not equals fileSize");
    yaissert(nReadBytes == fileSize);

    size_t nlines = ReplaceRNto0(stringText, fileSize);
    
    char **pointerArr = (char **)safe_calloc((size_t)nlines, sizeof(char *));;
    SetPointerArr(stringText, fileSize, pointerArr);
//---------------------------------------------- 
//TODO: divide to funcs
//TODO: array of sstruct
    ByteCodeStruct byteCodeStruct;
    byteCodeStruct.nCommands = nlines;
    byteCodeStruct.size = (sizeof(StackElem_t) + CommandMaxLen) * nlines;
    byteCodeStruct.byteCode = (unsigned char*) safe_calloc(byteCodeStruct.size, sizeof(unsigned char)); //TODO: Elem_t
    size_t indexByteCode = 0;
    for(size_t i = 0; i < nlines; i++)
        TakeCommand(pointerArr[i], byteCodeStruct.byteCode, &indexByteCode);
        

    fclose(pFile);
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
{//TODO null pointerarr
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
//TODO: Write code to file


void TakeCommand(char *pointerToComm, unsigned char *byteCode, size_t *indexByteCode)
{
    char inputCommand[CommandMaxLen + 1] = "";//TODO: magicnumber
    char inputArg[ArgumentMaxLen] = "";
    DivideToPiece(pointerToComm, inputCommand, inputArg);
    for(size_t i = 0; i < number_of_commands_ - 1; i++)
        if(strcmp(inputCommand, GetCommandName(commands_[i])) == 0)//TODO: add strcmp for int
        {
            COMMAND_INDEX index = commands_[i];
            if(index == pusr_index || index == popr_index)
            {
                if(inputArg[1] == 'X')
                {
                    if('A' <= inputArg[0] && inputArg[0] <= 'D')
                    {
                        byteCode[(*indexByteCode)++] = (unsigned char)index;

                        unsigned char registerIndex = (unsigned char) (inputArg[0] - 'A' + 1);
                        byteCode[(*indexByteCode)++] = registerIndex;
                        return;
                    }
                }
                byteCode[(*indexByteCode)++] = (unsigned char)notcomm_index;
                return;
            }

            else if (IsCommandHaveArg(index))
            {
                byteCode[(*indexByteCode)++] = (unsigned char)index;

                StackElem_t arg = 0;
                sscanf(inputArg, "%" PRINTF_T, &arg);
                WriteArgToByteCode(byteCode, indexByteCode, &arg);
                return;
            }
            byteCode[(*indexByteCode)++] = (unsigned char)index;
            return;
        }
    byteCode[(*indexByteCode)++] = (unsigned char)notcomm_index;
}

bool IsCommandHaveArg(COMMAND_INDEX index)
{
    for(size_t i = 0; i < number_of_arg_commands_; i++)
    {
        if(index == arg_commands_[i])
            return true;
    }
    return false;
}

void WriteArgToByteCode(unsigned char *byteCode, size_t *indexByteCode, StackElem_t *arg)
{
    for(size_t i = 0; i < sizeof(StackElem_t); i++)
        byteCode[(*indexByteCode)++] = (unsigned char)*((unsigned char *)arg + i);
}

void DivideToPiece(char *pointerToComm, char *inputCommand, char *inputArg)
{
    int i = 0;
    char tempc = pointerToComm[i];
    while(tempc != ' ' && i < 5)
    {
        inputCommand[i] = tempc;
        tempc = pointerToComm[++i];
    }

    inputCommand[i++] = '\0';
    int j = 0;
    tempc = pointerToComm[i];
    while(tempc != '\0')
    {
        inputArg[j] = tempc;
        tempc = pointerToComm[++i];
        j++;
    }
    inputArg[j] = '\0';
}

const char *GetCommandName(COMMAND_INDEX commandIndex)
{
    switch (commandIndex)
    {
        case hlt_index:  return "HLT";

        case push_index: return "PUSH";
        case out_index:  return "OUT";
        
        case pusr_index: return "PUSR";
        case popr_index: return "POPR";
        
        case add_index:  return "ADD";
        case sub_index:  return "SUB";
        case div_index:  return "DIV";
        case mul_index:  return "MUL";
        case sqrt_index:  return "SQRT";
        
        case jmp_index:  return "JMP";
        case je_index:   return "JE";
        case ja_index:   return "JA";
        case jae_index:  return "JAE";
        case jb_index:   return "JB";
        case jbe_index:  return "JBE";
        
        case notcomm_index: return "NOT_COMM";
        default: return "NOT_COMM";
    }
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