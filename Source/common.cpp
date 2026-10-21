#include "../Headers/common.h"



void *safe_calloc(size_t count, size_t size)
{
    void *pointer = calloc(count, size);
    warning(pointer != NULL, "Pointer than return calloc is NULL");
    yaissert(pointer != NULL);
    return pointer;
}

void *safe_malloc(size_t size)
{
    void *pointer = malloc(size);
    yaissert(pointer != NULL);
    memset(pointer, 0, size);
    for (size_t i = 0; i < size; i++)
    {
        warning (*(char *)((size_t)pointer + i) == 0, "Pointer than return malloc is NULL");
        yaissert(*(char *)((size_t)pointer + i) == 0);
    }
    return pointer;
}

void safe_free(void *pointer, size_t size)
{
    warning (pointer != NULL, "Pointer to data to free is NULL");
    yaissert(pointer != NULL);
    
    if(pointer != NULL)
        memset(pointer, 0, size);
    free(pointer);
    pointer = NULL;
}

FILE *safe_fopen(char *fileName, char *mode)
{
    warning (fileName != NULL, "Pointer to fileName is NULL");
    yaissert(fileName != NULL);

    warning (mode != NULL, "Pointer to mode is NULL");
    yaissert(mode != NULL);
    FILE *pfile = fopen(fileName, mode);

    warning (pfile != NULL, "Pointer to file is NULL");
    yaissert(pfile != NULL);

    return pfile;
}


size_t safe_fread(void *stringText, size_t elemSize, size_t fileSize, FILE *pFile)
{
    warning (stringText != NULL, "Pointer to stringText is NULL");
    yaissert(stringText != NULL);

    warning (elemSize != 0, "elemSize equal 0");
    yaissert(elemSize != 0);

    warning (fileSize != 0, "fileSize equal 0");
    yaissert(fileSize != 0);

    warning (pFile != NULL, "Pointer to file is NULL");
    yaissert(pFile != NULL);

    size_t nReadBytes = fread(stringText, elemSize, fileSize, pFile);

    return nReadBytes;
}