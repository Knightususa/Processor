#include "../Headers/common.h"
#include "../Headers/assembler.h"
#include "../Headers/stack.h"
#include "../Headers/processor_func.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const int number_of_labels = 100;

ByteCodeStruct ReadFile(char *fileName)
{
    char mode[] = "rb";
    FILE *pFile = safe_fopen(fileName, mode);

    struct stat fileStat = {};
    fstat(fileno(pFile), &fileStat);
    size_t fileSize = fileStat.st_size;

    printf("Size of file = " RED "%lli bytes\n\n\n" RESET, fileSize);
    char *stringText = (char *)safe_calloc((size_t)fileSize + 1, sizeof(char));

    size_t nReadBytes = safe_fread((void *) stringText, sizeof(char), fileSize, pFile);
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
    byteCodeStruct.byteCode = (unsigned char*) safe_calloc(byteCodeStruct.size, sizeof(unsigned char));

    size_t *labels = (size_t *)safe_calloc(sizeof(size_t), number_of_labels);
    for(int j = 0; j < 2; j++)
    {
        size_t indexByteCode = 0;
        for(size_t i = 0; i < nlines; i++)
            TakeCommand(pointerArr[i], byteCodeStruct.byteCode, &indexByteCode, labels);
    }

    safe_free(labels, number_of_labels * sizeof(size_t));
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


void TakeCommand(char *pointerToComm, unsigned char *byteCode, size_t *indexByteCode, size_t *labels)
{
    char inputCommand[CommandMaxLen + 1] = "";
    char inputArg[ArgumentMaxLen] = "";
    DivideToPiece(pointerToComm, inputCommand, inputArg);
    if(inputCommand[0] == ':')
    {
        int arg = 0;
        char *inputCommand1 = inputCommand + 1;
        sscanf(inputCommand1, "%i", &arg);
        if(!(0 < arg || arg <= number_of_labels))
        {
            printf("Entered %i, but it cant be a label(less than 0 or more than max(number_of_labels))\n", arg);
            return;
        }
        labels[arg] = *indexByteCode;
        return;
    }
    for(size_t i = 0; i < number_of_commands_ - 1; i++)
        if(strcmp(inputCommand, commands_[i].name) == 0)//TODO: add strcmp for int
        {
            COMMAND_INDEX index = commands_[i].index;
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

            else if (commands_[i].args == 1)
            {
                if(inputArg[0] == ':')
                {
                    int arg = 0;
                    char *inputArg1 = inputArg + 1;
                    sscanf(inputArg1, "%i", &arg);
                    if(!(0 < arg || arg <= number_of_labels))
                    {
                        printf("Entered %i, but it cant be a label(less than 0 or more than max(number_of_labels))\n", arg);
                        return;
                    }
                    byteCode[(*indexByteCode)++] = (unsigned char)index;
                    WriteArgToByteCode(byteCode, indexByteCode, (void *)&labels[arg], sizeof(size_t));
                    return;
                }
                byteCode[(*indexByteCode)++] = (unsigned char)index;

                StackElem_t arg = 0;
                sscanf(inputArg, "%" PRINTF_T, &arg);
                WriteArgToByteCode(byteCode, indexByteCode, &arg, sizeof(StackElem_t));
                return;
            }
            byteCode[(*indexByteCode)++] = (unsigned char)index;
            return;
        }
    byteCode[(*indexByteCode)++] = (unsigned char)notcomm_index;
}

void WriteArgToByteCode(unsigned char *byteCode, size_t *indexByteCode, void *arg, size_t size)
{
    for(size_t i = 0; i < size; i++)
        byteCode[(*indexByteCode)++] = (unsigned char)*((unsigned char *)arg + i);
}

void DivideToPiece(char *pointerToComm, char *inputCommand, char *inputArg)
{
    int i = 0;
    char tempc = pointerToComm[i];
    while(tempc != ' ' && i < CommandMaxLen + 1)
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