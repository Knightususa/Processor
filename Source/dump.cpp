#include "../Headers/dump.h"


#ifndef STACK_T_STRUCT
//CI - CHAR or INT
bool IsValuePoisonCI(Stack_t stk, StackElem_t value)
{
    if(value == stk.poison_v)
        return true;
    return false;
}

//D - DOUBLE
bool IsValuePoisonD(Stack_t stk, StackElem_t value)
{
    if(isnan(stk.poison_v) && isnan(value))
        return true;
    else if(stk.poison_v == value)
        return true;
    return false;
}

//CID - CHAR or INT or DOUBLE
int StackDumpCID(Stack_t *stk)
{
    printf(YELLOW "DEBUG |Index|     Pointer      |      Value\n");
    CANARY_PROTECTION(
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " arbusL  = %X\n", &(stk->arbusL), stk->arbusL);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " arbusDefault  = %X\n", &(stk->arbusDefault), stk->arbusDefault);
    )
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " size      = %i\n", &(stk->size), stk->size);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " capacity  = %i\n", &(stk->capacity), stk->capacity);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " poison_v  = %" PRINTF_T "\n", &(stk->poison_v), stk->poison_v);
    CANARY_PROTECTION(
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " canaryL  = %p\n", &(stk->canaryL), stk->canaryL);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " canaryR  = %p\n", &(stk->canaryR), stk->canaryR);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " canaryDefault  = %X\n", &(stk->canaryDefault), stk->canaryDefault);    
    )

    HASH_PROTECTION(printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " hashData  = %llu\n", &(stk->hashData), stk->hashData);
                    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " hashStack = %llu\n", &(stk->hashStack), stk->hashStack);)

    printf(YELLOW "DEBUG |-----|------------------|----------------------\n");
    CANARY_PROTECTION(printf(YELLOW "DEBUG |" CYAN " [-1]" YELLOW "|" CYAN " %p " YELLOW "|" CYAN " %X\n" CYAN, stk->canaryL, *stk->canaryL);)
    for (int i = 0; i < stk->capacity; i++)
    {
        printf(YELLOW "DEBUG |" CYAN " [%i] " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " %" PRINTF_T  YELLOW " | " CYAN, i, &(stk->data[i]), stk->data[i]);
        if(i < stk->size)
            printf(GREEN "*\n");
        else
            printf(RED "*\n");
    }
    CANARY_PROTECTION(printf(YELLOW "DEBUG |" CYAN " [%i] " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " %X\n" CYAN, stk->capacity, stk->canaryR, *stk->canaryR);)
    printf(YELLOW "DEBUG |-----|------------------|----------------------\n");
    CANARY_PROTECTION(printf(YELLOW "DEBUG |" CYAN "     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " arbusR  = %X\n", &(stk->arbusR), stk->arbusR);
    printf(YELLOW "DEBUG |-----|------------------|----------------------\n");)
    printf(YELLOW "DEBUG | " CYAN "[");
    for (int i = 0; i < stk->capacity; i++)
    {
        printf("%" PRINTF_T, stk->data[i]);
        if(i != stk->capacity - 1)
            printf(", ");
    }
    printf("]\n");
    printf(RESET);
    return 1;
}
#endif 
//end #ifndef STACK_T_STRUCT



//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------



#ifdef STACK_T_STRUCT
//S - STRUCT
bool IsValuePoisonS(Stack_t stk, StackElem_t value)
{
    if((isnan(stk.poison_v.double_v) && isnan(value.double_v)) || (stk.poison_v.double_v == value.double_v))
    {
        if(stk.poison_v.int_v == value.int_v)
        {
            if(stk.poison_v.char_v == value.char_v)
                return true;
        }
    }
    return false;
}

//S - STRUCT
int StackDumpS(Stack_t *stk)
{
    printf(YELLOW "DEBUG |Index|     Pointer      |      Value\n");
    CANARY_PROTECTION(
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " arbusL  = %X\n", &(stk->arbusL), stk->arbusL);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " arbusDefault  = %X\n", &(stk->arbusDefault), stk->arbusDefault);
    )
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " size      = %i\n", &(stk->size), stk->size);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " capacity  = %i\n", &(stk->capacity), stk->capacity);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " poison_v.int_v    = %i\n",  &(stk->poison_v.int_v), stk->poison_v.int_v);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " poison_v.char_v   = %c\n",  &(stk->poison_v.char_v), stk->poison_v.char_v);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " poison_v.double_v = %lg\n", &(stk->poison_v.double_v), stk->poison_v.double_v);
    CANARY_PROTECTION(
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " canaryL  = %p\n", &stk->canaryL, stk->canaryL);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " canaryR  = %p\n", &stk->canaryR, stk->canaryR);
    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " canaryDefault  = %X\n", &(stk->canaryDefault), stk->canaryDefault);    
    )

    HASH_PROTECTION(printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " hashData  = %llu\n", &(stk->hashData), stk->hashData);
                    printf(YELLOW "DEBUG |     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " hashStack = %llu\n", &(stk->hashStack), stk->hashStack);)

    printf(YELLOW "DEBUG |-----|------------------|----------------------\n");
    CANARY_PROTECTION(printf(YELLOW "DEBUG |" CYAN " [-1]" YELLOW "|" CYAN " %p " YELLOW "|" CYAN " %X\n" CYAN, stk->canaryL, *stk->canaryL);)
    for (int i = 0; i < stk->capacity; i++)
    {
        printf(YELLOW "DEBUG |" CYAN " [%i] " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " %i, %c, %lg " YELLOW "|" CYAN " ", i, &(stk->data[i]), stk->data[i].int_v, stk->data[i].char_v, stk->data[i].double_v);
        if(i < stk->size)
            printf(GREEN "*\n");
        else
            printf(RED "*\n");
    }
    CANARY_PROTECTION(printf(YELLOW "DEBUG |" CYAN " [%i] " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " %X\n" CYAN, stk->capacity, stk->canaryR, *stk->canaryR);)
    printf(YELLOW "DEBUG |-----|------------------|----------------------\n");
    CANARY_PROTECTION(printf(YELLOW "DEBUG |" CYAN "     " YELLOW "|" CYAN " %p " YELLOW "|" CYAN " arbusR  = %X\n", &(stk->arbusR), stk->arbusR);
    printf(YELLOW "DEBUG |-----|------------------|----------------------\n");)
    printf(YELLOW "DEBUG | " CYAN "[");
    for (int i = 0; i < stk->capacity; i++)
    {
        printf("(%i, %c, %lg)", stk->data[i].int_v, stk->data[i].char_v, stk->data[i].double_v);
        if(i != stk->capacity - 1)
            printf(", ");
    }
    printf("]\n");
    printf(RESET);
    return 1;
}
#endif 
//end #ifdef STACK_T_STRUCT