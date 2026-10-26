#include "../Headers/dump.h"

int StackDump(Stack_t *stk)
{
    printf(_YELLOW "\n\nDEBUG |Index|     Pointer      |      Value\n");
    CANARY_PROTECTION(
    printf(_YELLOW "DEBUG |     " _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " arbusL  = %llX\n", &(stk->arbusL), stk->arbusL);
    printf(_YELLOW "DEBUG |     " _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " arbusDefault  = %llX\n", &(stk->arbusDefault), stk->arbusDefault);
    )
    printf(_YELLOW "DEBUG |     " _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " size      = %i\n", &(stk->size), stk->size);
    printf(_YELLOW "DEBUG |     " _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " capacity  = %i\n", &(stk->capacity), stk->capacity);
    printf(_YELLOW "DEBUG |     " _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " poison_v  = %" PRINTF_T "\n", &(stk->poison_v), stk->poison_v);
    CANARY_PROTECTION(
    printf(_YELLOW "DEBUG |     " _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " canaryL  = %p\n", &(stk->canaryL), stk->canaryL);
    printf(_YELLOW "DEBUG |     " _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " canaryR  = %p\n", &(stk->canaryR), stk->canaryR);
    printf(_YELLOW "DEBUG |     " _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " canaryDefault  = %llX\n", &(stk->canaryDefault), stk->canaryDefault);    
    )

    HASH_PROTECTION(printf(_YELLOW "DEBUG |     " _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " hashData  = %llu\n", &(stk->hashData), stk->hashData);
                    printf(_YELLOW "DEBUG |     " _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " hashStack = %llu\n", &(stk->hashStack), stk->hashStack);)

    printf(_YELLOW "DEBUG |-----|------------------|----------------------\n");
    CANARY_PROTECTION(printf(_YELLOW "DEBUG |" _CYAN " [-1]" _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " %llX\n" _CYAN, stk->canaryL, *stk->canaryL);)
    for (int i = 0; i < stk->capacity; i++)
    {
        printf(_YELLOW "DEBUG |" _CYAN " [%i] " _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " %" PRINTF_T  _YELLOW " | " _CYAN, i, &(stk->data[i]), stk->data[i]);
        if(i < stk->size)
            printf(_GREEN "*\n");
        else
            printf(_RED "*\n");
    }
    CANARY_PROTECTION(printf(_YELLOW "DEBUG |" _CYAN " [%i] " _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " %llX\n" _CYAN, stk->capacity, stk->canaryR, *stk->canaryR);)
    printf(_YELLOW "DEBUG |-----|------------------|----------------------\n");
    CANARY_PROTECTION(printf(_YELLOW "DEBUG |" _CYAN "     " _YELLOW "|" _CYAN " %p " _YELLOW "|" _CYAN " arbusR  = %llX\n", &(stk->arbusR), stk->arbusR);
    printf(_YELLOW "DEBUG |-----|------------------|----------------------\n");)
    printf(_YELLOW "DEBUG | " _CYAN "[");
    for (int i = 0; i < stk->capacity; i++)
    {
        printf("%" PRINTF_T, stk->data[i]);
        if(i != stk->capacity - 1)
            printf(", ");
    }
    printf("]\n\n\n");
    printf(_RESET);
    return 1;
}