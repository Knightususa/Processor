#include "../Headers/stack.h"

enum ERRORS StackInit(Stack_t *stk)
{
    if (stk->capacity <= 0)
    {
        printf(RED "\nCapacity must be > 0\n" RESET);
        return stackNoInit;
    }
    CANARY_PROTECTION(stk->arbusL =  stk->arbusDefault;
                      stk->arbusR = stk->arbusDefault;)

    stk->data = (StackElem_t *)safe_calloc(stk->capacity, sizeof(StackElem_t) + 16);
    
    if (stk->data == NULL)
    {
        printf(RED "\nCan't create stack(\n\n" RESET);
        return stackNoInit;
    }
    
    CANARY_PROTECTION(stk->canaryL = (unsigned long long *) stk->data;
                      *stk->canaryL = stk->canaryDefault;
                      stk->data    = stk->data + sizeof(unsigned long long) / sizeof(StackElem_t);
                      MakeCanaryEnd(stk);)
    
    stk->size = 0;

    for (int i = 0; i < stk->capacity; i++)
        stk->data[i] = stk->poison_v;

    HASH_PROTECTION(stk->hashData = CalculateHashData(*stk);
                    stk->hashStack = CalculateHashStack(*stk);)

    yaissert(StackDump(stk));
    printf(MAGENTA "\nCreate and initialize stack successfully\n\n" RESET);

    return noErr;
}

enum ERRORS StackDestroy(Stack_t *stk)
{
    enum ERRORS checkStack = CheckStack(*stk);
    if(checkStack != noErr)
    {
        printf(RED " when trying to destroy\n\n" RESET);
        yaissert(StackDump(stk));
        return checkStack;
    }

    yaissert(Verificator(stk));
    
    for (int i = 0; i < stk->capacity; i++)
    stk->data[i] = stk->poison_v;
    // printf("data")
    free(stk->data);
    stk->data = NULL;
    stk->size = -1;
    stk->capacity = -1;
    
    HASH_PROTECTION(stk->hashData = CalculateHashData(*stk);
                    stk->hashStack = CalculateHashStack(*stk);)

    // printf(MAGENTA "\nDestroy stack successfully\n\n" RESET);
    return noErr;
}

enum ERRORS StackPush(Stack_t *stk, StackElem_t elem)
{
    enum ERRORS checkStack = CheckStack(*stk);
    if(checkStack != noErr)
    {
        printf(RED " when trying to push\n\n" RESET);
        yaissert(StackDump(stk));
        return checkStack;
    }
    enum ERRORS resizeUp = noErr;
    
    if (stk->size == stk->capacity)
        resizeUp = ResizeUp(stk);
    
    if(resizeUp == stackOverflow)
        return stackOverflow;
    
    stk->data[stk->size++] = elem;

    HASH_PROTECTION(stk->hashData = CalculateHashData(*stk);
                    stk->hashStack = CalculateHashStack(*stk);)
    // printf(MAGENTA "\nSuccessfully pushed\n\n" RESET);
        
    return resizeUp;
}

enum ERRORS StackPop(Stack_t *stk, StackElem_t *popValue)
{
    enum ERRORS checkStack = CheckStack(*stk);
    if(checkStack != noErr)
    {
        printf(RED " when trying to pop\n\n" RESET);
        yaissert(StackDump(stk));
        return checkStack;
    }

    if (stk->size <= 0)
    {
        printf(RED "\nStack underflow\n\n" RESET);
        stk->size = 0;
        return stackUnderflow;
    }

    stk->size--;
    *popValue = stk->data[stk->size];
    stk->data[stk->size] = stk->poison_v;
    
    HASH_PROTECTION(stk->hashData = CalculateHashData(*stk);
                    stk->hashStack = CalculateHashStack(*stk);)

    // printf(MAGENTA "\nSuccessfully poped\n\n" RESET);

    enum ERRORS resizeDown = ResizeDown(stk);

    return resizeDown;
}

enum ERRORS ResizeUp(Stack_t *stk)
{
    if(stk->data == NULL)
    {
        printf("Pointer to data is null ");
        return stackNoResize;
    }
    int cap = stk->capacity;
    void *dataNew = realloc(stk->canaryL, sizeof(StackElem_t) * (cap * 2) + 16);

    if(dataNew == NULL)
    {
        printf(RED "\nCant resize up stack\n\n" RESET);
        return stackOverflow;
    }

    stk->capacity = stk->capacity * 2;
    stk->data = (StackElem_t *) dataNew; 

    CANARY_PROTECTION(stk->canaryL = (unsigned long long *) stk->data;
                      *stk->canaryL = stk->canaryDefault;
                      stk->data    = stk->data + sizeof(unsigned long long) / sizeof(StackElem_t);
                      MakeCanaryEnd(stk);)

    for(int i = cap; i < cap * 2; i++)
        stk->data[i] = stk->poison_v;

    

    HASH_PROTECTION(stk->hashData = CalculateHashData(*stk);
                    stk->hashStack = CalculateHashStack(*stk);)

    // printf(MAGENTA "\nResize up stack\n\n" RESET);
    return noErr;
}

enum ERRORS ResizeDown(Stack_t *stk)
{
    if(stk->capacity <= stk->size * 4)
        return noErr;

    void *dataNew = realloc(stk->canaryL, sizeof(StackElem_t) * (stk->capacity / 2) + 16);
    if(dataNew == NULL)
    {
        printf(RED "\nCant resize down stack\n\n" RESET);
        return stackNoResize;
    }

    stk->data = (StackElem_t *) dataNew;
    stk->capacity /= 2;

    HASH_PROTECTION(stk->hashData = CalculateHashData(*stk);
                    stk->hashStack = CalculateHashStack(*stk);)

    // printf(MAGENTA "\nResize down stack\n\n" RESET);
    return noErr;
}

bool Verificator(Stack_t *stk)
{
    printf(MAGENTA);
    if (stk->data == NULL)
    {
        printf("Pointer to data is NULL\n" RESET);
        return false;
    }

    if (stk->capacity <= 0)
    {
        printf("Capacity of data <= 0\n" RESET);
        StackDump(stk);
        return false;
    }

    if (stk->capacity < stk->size)
    {
        printf("Capacity less than size\n" RESET);
        StackDump(stk);
        return false;
    }

    for (int i = 0; i < stk->size; i++)
    {
        if (IsValuePoison(*stk, stk->data[i]))
        {
            printf("Element of data number %i is poison value\n" RESET, i);
            StackDump(stk);
            return false;
        }
    }

    for (int j = stk->size; j < stk->capacity; j++)
    {
        if (!IsValuePoison(*stk, stk->data[j]))
        {
            printf("Element of data with index %i not a poison value\n" RESET, j);
            StackDump(stk);
            return false;
        }
    }
    printf(RESET);
    return true;
}

enum ERRORS CheckStack(Stack_t stk)
{
    if(stk.data == NULL)
    {
        printf(RED "\nPointer to data is NULL" RESET);
        return dataPointerNull;
    }

    CANARY_PROTECTION(
    else if(*stk.canaryL != stk.canaryDefault)
    {
        printf(RED "\nLeft canary in data is killed(value is edit)" RESET);
        return leftCanaryDataKilled;
    }

    else if(*stk.canaryR != stk.canaryDefault)
    {
        printf(RED "\nRight canary in data is killed(value is edit)" RESET);
        return rightCanaryDataKilled;
    }

    else if(stk.arbusL != stk.arbusDefault)
    {
        printf(RED "\nLeft canary in stack is killed(value is edit)" RESET);
        return leftCanaryStackKilled;
    }

    else if(stk.arbusR != stk.arbusDefault)
    {
        printf(RED "\nRight canary in stack is killed(value is edit)" RESET);
        return rightCanaryStackKilled;
    })//end CANARY_PROTECTION

    HASH_PROTECTION(
    else if(stk.hashData != CalculateHashData(stk))
    {
        printf(RED "\nHash of data incorrect, data may be edited" RESET);
        return hashDataIncorrect;
    }

    else if(stk.hashStack != CalculateHashStack(stk))
    {
        printf(RED "\nHash of stack incorrect, stack may be edited" RESET);
        return hashStackIncorrect;
    })//end HASH_PROTECTION
    return noErr;
}

CANARY_PROTECTION(
void MakeCanaryEnd(Stack_t *stk)
{   
    stk->canaryR = (unsigned long long *) & (stk->data[stk->capacity]);
    for(int i = 0; i < 8; i++)
    {
        unsigned char mask = (unsigned char)(((stk->canaryDefault) >> 8 * i) & 0xFF);
        *(unsigned char *)((size_t)(stk->canaryR) + i) = mask;
    }
})//end CANARY_PROTECTION



void EditHash(unsigned long long *hash, unsigned long long value)
{
    *hash = (*hash << 5) + *hash;
    *hash += value;
}


HASH_PROTECTION(
unsigned long long CalculateHashData(Stack_t stk)
{
    unsigned long long hash = 5381;
    for(int i = 0; i < stk.capacity; i++)
    {
        hash = (hash << 5) + hash;
        hash += (unsigned long long)stk.data[i];
    }
    return hash;
}

unsigned long long CalculateHashStack(Stack_t stk)
{
    unsigned long long hash = 5381;
    EditHash(&hash, (unsigned long long) stk.arbusDefault);
    EditHash(&hash, (unsigned long long) stk.arbusL);
    EditHash(&hash, (unsigned long long) stk.arbusR);
    EditHash(&hash, (unsigned long long) stk.capacity);
    EditHash(&hash, (unsigned long long) stk.data);
    EditHash(&hash, (unsigned long long) stk.hashData);
    EditHash(&hash, (unsigned long long) stk.poison_v);
    EditHash(&hash, (unsigned long long) stk.size);
    return hash;
}
)