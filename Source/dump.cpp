#include "../Headers/dump.h"

const double EPSILON = 0.0001;

#ifndef STACK_T_DOUBLE
bool IsValuePoisonCI(Stack_t stk, StackElem_t value)
{
    if(value == stk.poison_v)
        return true;
    return false;
}
#endif

#ifdef STACK_T_DOUBLE
bool IsValuePoisonD(Stack_t stk, StackElem_t value)
{
    if(isnan(stk.poison_v) && isnan(value))
        return true;
    else if(IsZero(stk.poison_v, value))
        return true;
    return false;
}

bool IsZero(StackElem_t a, StackElem_t b)
{
    if(a - b < EPSILON || b - a < EPSILON)
    return true;
    return false;
}
#endif

//CID - CHAR or INT or DOUBLE
int StackDumpCID(Stack_t *stk)
{
    printf(YELLOW "\n\nDEBUG |Index|     Pointer      |      Value\n");
    CANARY_PROTECTION(
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " arbusL  = %llX\n", &(stk->arbusL), stk->arbusL);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " arbusDefault  = %llX\n", &(stk->arbusDefault), stk->arbusDefault);
    )
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " size      = %i\n", &(stk->size), stk->size);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " capacity  = %i\n", &(stk->capacity), stk->capacity);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " poison_v  = %" PRINTF_T "\n", &(stk->poison_v), stk->poison_v);
    CANARY_PROTECTION(
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " canaryL  = %p\n", &(stk->canaryL), stk->canaryL);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " canaryR  = %p\n", &(stk->canaryR), stk->canaryR);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " canaryDefault  = %llX\n", &(stk->canaryDefault), stk->canaryDefault);    
    )

    HASH_PROTECTION(printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " hashData  = %llu\n", &(stk->hashData), stk->hashData);
                    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " hashStack = %llu\n", &(stk->hashStack), stk->hashStack);)

    printf(YELLOW "DEBUG |-----|------------------|----------------------\n");
    CANARY_PROTECTION(printf(YELLOW "DEBUG |" CYAN " [-1]" YELLOW "|" CYAN " %p " YELLOW "|" CYAN " %llX\n" CYAN, stk->canaryL, *stk->canaryL);)
    for (int i = 0; i < stk->capacity; i++)
    {
        printf(YELLOW "DEBUG |" CYAN " [%i] " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " %" PRINTF_T  YELLOW " | " CYAN, i, &(stk->data[i]), stk->data[i]);
        if(i < stk->size)
            printf(GREEN "*\n");
        else
            printf(RED "*\n");
    }
    CANARY_PROTECTION(printf(YELLOW "DEBUG |" CYAN " [%i] " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " %llX\n" CYAN, stk->capacity, stk->canaryR, *stk->canaryR);)
    printf(YELLOW "DEBUG |-----|------------------|----------------------\n");
    CANARY_PROTECTION(printf(YELLOW "DEBUG |" CYAN "     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " arbusR  = %llX\n", &(stk->arbusR), stk->arbusR);
    printf(YELLOW "DEBUG |-----|------------------|----------------------\n");)
    printf(YELLOW "DEBUG | " CYAN "[");
    for (int i = 0; i < stk->capacity; i++)
    {
        printf("%" PRINTF_T, stk->data[i]);
        if(i != stk->capacity - 1)
            printf(", ");
    }
    printf("]\n\n\n");
    printf(RESET);
    return 1;
}