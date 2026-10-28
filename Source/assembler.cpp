#include "../Headers/assembler.h"
#include "../Headers/common.h"

main(int argc, char **argv)
{
    OptionsAsm Options;
    Options.filename = NULL;
    GetOptionsAsm(argc, argv, &Options);   
    DelNameExt(&Options);
    char *filenametxt = CreateName(Options.filename, "txt");
    char *filenamebin = CreateName(Options.filename, "bin");
    ByteCodeStruct byteCodeStruct = ProcReadFile(filenametxt);
    WriteByteCode(filenamebin, byteCodeStruct);
}

void GetOptionsAsm(int argc, char **argv, OptionsAsm *pOptions)
{
    yaissert(argv != NULL);

    int opt = 0;
    while ((opt = getopt(argc, argv, "i:")) != -1)
    {
        switch (opt)
        {       
            case 'i':
                pOptions -> filename = optarg;
                break;
            
            default:
                warning(0, "Wrong flag");
                break;
        }
    }
    if(pOptions->filename == NULL)
        printf(_RED "Enter flag -i and filename\n" _RESET);
}

void DelNameExt(OptionsAsm *pOptions)
{   
    yaissert(pOptions != NULL);
    yaissert(pOptions->filename != NULL);

    char *f = pOptions->filename;
    int lenf = (int)strlen(f);
    int i = lenf - 1;

    while(i >= 0 && f[i] != '.')
        i--;

    if(i <= 0)
        return;

    for(int j = 0; i + j < lenf; j++)
        (pOptions->filename)[i + j] = '\0';
}

char *CreateName(char *filename, const char *ext)
{
    yaissert(filename != NULL);

    char *nameOut = (char *)safe_calloc(strlen(filename) + strlen("./Code_/.") + 2 * strlen(ext) + 1, sizeof(char));
    strcpy(nameOut, "./Code_");
    strcat(nameOut, ext);
    strcat(nameOut, "/");
    strcat(nameOut, filename);
    strcat(nameOut, ".");
    strcat(nameOut, ext);
    return nameOut;
}

ByteCodeStruct ProcReadFile(char *fileName)
{
    yaissert(fileName != NULL);

    char mode[] = "rb";
    FILE *pFile = safe_fopen(fileName, mode);

    yaissert(pFile != NULL);

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
    fclose(pFile);
    ByteCodeStruct byteCodeStruct = ParsTextToBytecode(nlines, pointerArr);
    return byteCodeStruct;
}

ByteCodeStruct ParsTextToBytecode(size_t nlines, char **pointerArr)
{
    yaissert(pointerArr != NULL);

    ByteCodeStruct byteCodeStruct;
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
        {
            if(pointerArr[i] == NULL)
                continue;
            ParserCmdFromStr(pointerArr[i], byteCodeStruct.byteCode, &indexByteCode, labels);
        }
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
    yaissert(stringText != NULL);
    yaissert(fileSize != 0);

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

        while(charInArr != '\n' && charInArr != '\r' && charInArr != '\0' && i < fileSize)
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

        else if(charInArr == '\0' || i == fileSize - 1)
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
    yaissert(fileName != NULL);

    char mode[] = "wb";
    FILE *pFile = safe_fopen(fileName, mode);

    write(fileno(pFile), byteCodeStruct.byteCode, (unsigned int)byteCodeStruct.size);

    fclose(pFile);
}




void ParserCmdFromStr(char *pointerToStrWithCmd, unsigned char *byteCode, size_t *indexByteCode, Label *labels)
{
    warning(pointerToStrWithCmd != NULL, "pointerToStrWithCmd is NULL");
    if(pointerToStrWithCmd == NULL)
    {
        printf("indexByteCode = %lli\n", *indexByteCode);
        return;
    }
    yaissert(byteCode != NULL);
    yaissert(indexByteCode != NULL);
    yaissert(labels != NULL);

    // printf("\nstr = <%s>  ", pointerToStrWithCmd);

    char inputCommand[CommandMaxLen + 1] = "";
    char inputArg[ArgumentMaxLen + 1] = "";
    inputCommand[CommandMaxLen] = '\0';
    inputArg[ArgumentMaxLen] = '\0';

    DivideStrToPiece(pointerToStrWithCmd, inputCommand, inputArg);
    if(inputCommand[0] == '/')
        return;

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
        if(strcmp(inputCommand, commands_global_[i].name) == 0)
        {
            cmdInfo->indexInCmdArr = i;
            WriteCmdToBytecode(cmdInfo);
            return;
        }
    byteCode[(*indexByteCode)++] = (unsigned char)notcmd_index;
}




void WriteArgToByteCode(unsigned char *byteCode, size_t *indexByteCode, void *arg, size_t size)
{
    yaissert(byteCode != NULL);
    yaissert(indexByteCode != NULL);
    yaissert(arg != NULL);
    yaissert(size != 0);

    for(size_t i = 0; i < size; i++)
        byteCode[(*indexByteCode)++] = (unsigned char)*((unsigned char *)arg + i);
}

void DivideStrToPiece(char *pointerToStrWithCmd, char *inputCommand, char *inputArg)
{
    yaissert(pointerToStrWithCmd != NULL);
    yaissert(inputCommand != NULL);
    yaissert(inputArg != NULL);

    sscanf(pointerToStrWithCmd, "%" MAKEVALUE(CommandMaxLen) "[^[ \n] %" MAKEVALUE(ArgumentMaxLen) "[^\n]", inputCommand, inputArg);
}

void FillLabel(CmdToWriteInfo *cmdInfo)
{
    CheckCmdInfo(cmdInfo);

    char *inputCommand1 = cmdInfo->inputCommand + 1;
    char *inputCommand2 = (char *)safe_calloc(strlen(inputCommand1) + 1, sizeof(char));
    sscanf(inputCommand1, "%[^ ]", inputCommand2);

    for(int i = 0; i < number_of_labels; i++)
    {
        if(cmdInfo->labels[i].name == NULL)
        {
            cmdInfo->labels[i].name = (char *)safe_calloc(strlen(inputCommand2) + 1, sizeof(char));
            sscanf(inputCommand2, "%[^ ]", cmdInfo->labels[i].name);
            cmdInfo->labels[i].pointer = *cmdInfo->indexByteCode;
            return;
        }

        if(strcmp(cmdInfo->labels[i].name, inputCommand2) == 0)
            return;

        if(i == number_of_labels - 1 && cmdInfo->labels[i].name != NULL)
        {
            printf("Entered %s, but it cant be a label because its already maximum of labels\n", inputCommand2);
            return;
        }
    }
}

void WriteCmdToBytecode(CmdToWriteInfo *cmdInfo)
{
    CheckCmdInfo(cmdInfo);

    COMMAND_INDEX index = commands_global_[cmdInfo->indexInCmdArr].index;
    if (commands_global_[cmdInfo->indexInCmdArr].args == 1)
    {
        WriteOneArgCmd(cmdInfo);
        return;
    }
    cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = (unsigned char)index;
}

void WriteOneArgCmd(CmdToWriteInfo *cmdInfo)
{
    CheckCmdInfo(cmdInfo);

    COMMAND_INDEX index = notcmd_index;
    if(cmdInfo->inputArg[0] == ':')
    {
        WriteIfHaveLabel(cmdInfo, index);
        return;
    }

    index = commands_global_[cmdInfo->indexInCmdArr].index;
    if(IsIndexEqual(index, push_index))
    {
        WriteIfHaveFields(cmdInfo, push_index);
        return;
    }

    if(IsIndexEqual(index, pop_index))
    {
        WriteIfHaveFields(cmdInfo, pop_index);
        return;
    }

    if(index == delay_index || index == pow_index)
    {
        WriteIfHaveFields(cmdInfo, index);
        return;
    }


    index = notcmd_index;
    StackElem_t arg = 0;
    if(sscanf(cmdInfo->inputArg, "%" PRINTF_T, &arg) == 1)
        index = commands_global_[cmdInfo->indexInCmdArr].index;

    cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = (unsigned char)index;
    WriteArgToByteCode(cmdInfo->byteCode, cmdInfo->indexByteCode, &arg, sizeof(StackElem_t));
}

void WriteIfHaveLabel(CmdToWriteInfo *cmdInfo, unsigned char index)
{
    char *inputArg1 = cmdInfo->inputArg + 1;
    char *inputArg2 = (char *)safe_calloc(strlen(inputArg1) + 1, sizeof(char));
    sscanf(inputArg1, "%[^ ]", inputArg2);
    size_t arg = 0;
    for(int i = 0; i < number_of_labels; i++)
    {
        if(cmdInfo->labels[i].name != NULL && strcmp(cmdInfo->labels[i].name, inputArg2) == 0)
        {
            index = commands_global_[cmdInfo->indexInCmdArr].index;
            arg = cmdInfo->labels[i].pointer;
            break;
        }
    }
    cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = (unsigned char)index;
    WriteArgToByteCode(cmdInfo->byteCode, cmdInfo->indexByteCode, (void *)&arg, sizeof(size_t));
    return;
}

void WriteIfHaveFields(CmdToWriteInfo *cmdInfo, unsigned char index)
{
    CheckCmdInfo(cmdInfo);

    char *input = cmdInfo->inputArg;
    View indexP;
    indexP.value = index;
    
    bool is_VRAM = false;
    if(cmdInfo->inputArg[0] == '[')
    {
        indexP.bits.b3 = 1;   
        input += 1;
    }
    if(cmdInfo->inputArg[0] == '{')
    {
        indexP.bits.b3 = 1;     
        input += 1;
        is_VRAM = true;
    }  
        
    char *firstStr = (char *)safe_calloc(ArgumentMaxLen + 1, sizeof(char));
    char *secondStr = (char *)safe_calloc(ArgumentMaxLen + 1, sizeof(char));

    int inputN = sscanf(input, "%[^] +]%*[^-0-9]%[^] \n]", firstStr, secondStr);
    // printf("inputN = %i\n", inputN);
    // printf("firstStr = %s\n", firstStr);
    if(inputN == 2)
        WriteIfHaveFields2Arg(indexP, cmdInfo, firstStr, secondStr, is_VRAM);
        
    else if(inputN == 1)
        WriteIfHaveFields1Arg(indexP, cmdInfo, firstStr, is_VRAM);
}

void WriteIfHaveFields2Arg(View indexP, CmdToWriteInfo* cmdInfo, char *firstStr, char *secondStr, bool is_VRAM)
{
    indexP.bits.b1 = 1;
    indexP.bits.b2 = 1;
    cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = (unsigned char)indexP.value;
    

    if(firstStr[1] == 'x')
    {
        if('a' <= firstStr[0] && firstStr[0] <= 'a' + (char)size_registers)
        {
            unsigned char registerIndex = (unsigned char) (firstStr[0] - 'a');
            cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = registerIndex;
        }
    }

    StackElem_t konst = 0;
    int inputN = sscanf(secondStr, "%" PRINTF_T, &konst);
    yaissert(inputN != 0);
    if(is_VRAM)
        konst += (delta_size_RAM);

    WriteArgToByteCode(cmdInfo->byteCode, cmdInfo->indexByteCode, &konst, sizeof(StackElem_t));
    return;
}

void WriteIfHaveFields1Arg(View indexP, CmdToWriteInfo* cmdInfo, char *firstStr, bool is_VRAM)
{
    if(firstStr[1] == 'x')
    {
        indexP.bits.b2 = 1;

        if(is_VRAM)
            indexP.bits.b1 = 1;
        cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = (unsigned char)indexP.value;
        if('a' <= firstStr[0] && firstStr[0] <= 'a' + (char)size_registers)
        {
            unsigned char registerIndex = (unsigned char) (firstStr[0] - 'a');
            cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = registerIndex;

            if(is_VRAM)
            {
                StackElem_t konst = (delta_size_RAM);
                WriteArgToByteCode(cmdInfo->byteCode, cmdInfo->indexByteCode, &konst, sizeof(StackElem_t));
            }
        }
    }
    else
    {
        indexP.bits.b1 = 1;
        cmdInfo->byteCode[(*cmdInfo->indexByteCode)++] = (unsigned char)indexP.value;
        StackElem_t konst = 0;
        int inputN = 0;
        inputN = sscanf(firstStr, "%" PRINTF_T, &konst);
        
        // printf("first = %c\n", firstStr[0]);
        // printf("inputN = %i\n", inputN);
        if(inputN != 1)
            konst = (StackElem_t)firstStr[0];
        if(is_VRAM)
            konst += (delta_size_RAM);

        WriteArgToByteCode(cmdInfo->byteCode, cmdInfo->indexByteCode, &konst, sizeof(StackElem_t));
    }
    return;
}

bool IsIndexEqual(unsigned char index1, unsigned char index2)
{
    View indexP1, indexP2;
    indexP1.value = index1;
    indexP2.value = index2;
    if(indexP1.bits.b0 == indexP2.bits.b0)
        return true;
    return false;
}

void CheckCmdInfo(CmdToWriteInfo *cmdInfo)
{
    yaissert(cmdInfo != NULL);
    yaissert(cmdInfo->byteCode != NULL);
    yaissert(cmdInfo->indexByteCode != NULL);
    yaissert(cmdInfo->inputArg != NULL);
    yaissert(cmdInfo->inputCommand != NULL);
    yaissert(cmdInfo->labels != NULL);
}