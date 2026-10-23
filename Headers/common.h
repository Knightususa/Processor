#ifndef COMMON_H
#define COMMON_H

#include "./global.h"
#include "./colors.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define $(variable, typeInprintf) printf("\n" #variable " = <%" #typeInprintf ">\n", variable)

#ifdef ON_DEBUG
#define yaissert(usl)                                                                                                  \
    if (!(usl))                                                                                                        \
    {                                                                                                                  \
        printf("Assertion failed: " #usl ", function %s, file %s:%i\n", __func__, __FILE_NAME__, __LINE__);            \
        abort();                                                                                                       \
    }
#else
#define yaissert(usl)
#endif

#define warning(usl, text)                                                                                          \
{                                                                                                                   \
    if (!(usl))                                                                                                     \
    {                                                                                                               \
        printf("\n" _RED text _RESET "\n%s:%i\n", __FILE__, __LINE__);                                                \
    }                                                                                                               \
}


void *safe_calloc(size_t count, size_t size);
void *safe_malloc(size_t size);
void  safe_free(void *pointer, size_t size);
FILE *safe_fopen(char *fileName, char *mode);
size_t safe_fread(void *stringText, size_t elemSize, size_t fileSize, FILE *pFile);


#endif