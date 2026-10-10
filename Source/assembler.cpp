#include "../Headers/global.h"
#include "../Headers/assembler.h"
#include "../Headers/stack.h"
#include "../Headers/processor_func.h"
#include "../Headers/common.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


command *ReadFile(char *fileName, int *nCommands)
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

    command *takeCommands = (command*) safe_calloc(nlines, sizeof(command)); //TODO: safecalloc
    for(int i = 0; i < nlines; i++)
        takeCommands[i] = TakeCommand(pointerArr[i]);
        
    fclose(pFile);
    *nCommands = nlines;
    return takeCommands;
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


// void WriteAssemblerCode(FILE *inPipe, FILE *outPipe)
// {
//     command readCommand = ReadCommand(inPipe);
// }
//TODO: Write code to file


command TakeCommand(char *pointerToComm)
{
    command command;
    char inputCommand[CommandMaxLen + 1] = "";//TODO: magicnumber
    char inputArg[ArgumentMaxLen] = "";
    DivideToPiece(pointerToComm, inputCommand, inputArg);
    for(int i = 0; i < number_of_commands_ - 1; i++)
        if(strcmp(inputCommand, commands_[i].name) == 0)//TODO: add strcmp for int
        {
            command = commands_[i];
            if(command.index == pusr_index || command.index == popr_index)
            {
                if(inputArg[1] == 'X')
                    if('A' <= inputArg[0] && inputArg[0] <= 'D')
                        command.argument = (int) (inputArg[0] - 'A' + 1);
                else
                {
                    printf("INCORRECT INPUT NAME REGISTER\n");
                    return notcomm_;
                }
            }

            else if(command.index == push_index || command.index == pop_index)
            {
                int arg = 0;
                if(inputArg[0] != '0')
                {
                    arg = atoi(inputArg);
                    if(arg == 0)
                    {
                        printf("INCORRECT INPUT ARGUMENT\n");
                        return notcomm_;
                    }
                    command.argument = arg;
                }
                else 
                    command.argument = 0;
            }
            return command;
        }
    return notcomm_;
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
        case pop_index:  return "POP";
        
        case pusr_index: return "PUSR";
        case popr_index: return "POPR";
        
        case add_index:  return "ADD";
        case sub_index:  return "SUB";
        case div_index:  return "DIV";
        case mul_index:  return "MUL";
        
        case jmp_index:  return "JMP";
        case je_index:   return "JE";
        case ja_index:   return "JA";
        case jae_index:  return "JAE";
        case jb_index:   return "JB";
        case jbe_index:  return "JBE";
        
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