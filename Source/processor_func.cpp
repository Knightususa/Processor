#include "../Headers/processor_func.h"

// include-what-you-use -- linux

const command hlt_  = {.name = "HLT",  .index = hlt_index};

const command push_ = {.name = "PUSH", .index = push_index};//TODO: remove name and add getName
const command pop_  = {.name = "POP",  .index = pop_index};

const command pusr_ = {.name = "PUSR", .index = pusr_index};
const command popr_ = {.name = "POPR", .index = popr_index};

const command add_  = {.name = "ADD",  .index = add_index};
const command sub_  = {.name = "SUB",  .index = sub_index};
const command div_  = {.name = "DIV",  .index = div_index};
const command mul_  = {.name = "MUL",  .index = mul_index};

const command jmp_  = {.name = "JMP",  .index = jmp_index};
const command je_   = {.name = "JE",   .index = je_index};
const command ja_   = {.name = "JA",   .index = ja_index};
const command jae_  = {.name = "JAE",  .index = jae_index};
const command jb_   = {.name = "JB",   .index = jb_index};
const command jbe_  = {.name = "JBE",  .index = jbe_index};

const command notcomm_ = {.name = "NOT_COMM", .index = not_command};

const command commands_[] = 
{
    hlt_, push_, pop_, pusr_, popr_,
    add_, sub_, div_, mul_,
    jmp_, je_, ja_, jae_, jb_, jbe_,
    notcomm_
};

const size_t number_of_commands_ = sizeof(commands_) / sizeof(commands_[0]);


enum ERRORS ProcessorAdd(Stack_t *stk)
{
    enum ERRORS checkStack = CheckStack(*stk);
    if(checkStack != noErr)
    {
        printf(RED " when trying to add\n\n" RESET);
        yaissert(StackDump(stk));
        return checkStack;
    }    
    yaissert(Verificator(stk));

    int size = stk->size;
    if(size < 2)
    {
        printf("Too few elements for adding\n\n" RESET);
        return cantAdd;
    }

    StackElem_t sum = (stk->data[size - 1]) + stk->data[size - 2]; 
    stk->data[size - 2] = sum;
    stk->data[size - 1] = stk->poison_v;
    stk->size -= 1;
    ResizeDown(stk);

    HASH_PROTECTION(stk->hashData = CalculateHashData(*stk);
                    stk->hashStack = CalculateHashStack(*stk);)
        
    yaissert(StackDump(stk));
    return noErr;
}



enum ERRORS ProcessorSub(Stack_t *stk)
{
    enum ERRORS checkStack = CheckStack(*stk);
    if(checkStack != noErr)
    {
        printf(RED " when trying to subtraction\n\n" RESET);
        yaissert(StackDump(stk));
        return checkStack;
    }    
    yaissert(Verificator(stk));

    int size = stk->size;
    if(size < 2)
    {
        printf("Too few elements for subtraction\n\n" RESET);
        return cantAdd;
    }

    StackElem_t sub = (stk->data[size - 2]) - stk->data[size - 1]; 
    stk->data[size - 2] = sub;
    stk->data[size - 1] = stk->poison_v;
    stk->size -= 1;
    ResizeDown(stk);

    HASH_PROTECTION(stk->hashData = CalculateHashData(*stk);
                    stk->hashStack = CalculateHashStack(*stk);)
        
    yaissert(StackDump(stk));
    return noErr;
}



enum ERRORS ProcessorDiv(Stack_t *stk)
{
    enum ERRORS checkStack = CheckStack(*stk);
    if(checkStack != noErr)
    {
        printf(RED " when trying to division\n\n" RESET);
        yaissert(StackDump(stk));
        return checkStack;
    }    
    yaissert(Verificator(stk));

    int size = stk->size;
    if(size < 2)
    {
        printf("Too few elements for division\n\n" RESET);
        return cantAdd;
    }

    StackElem_t div = (stk->data[size - 2]) / stk->data[size - 1]; 
    stk->data[size - 2] = div;
    stk->data[size - 1] = stk->poison_v;
    stk->size -= 1;
    ResizeDown(stk);

    HASH_PROTECTION(stk->hashData = CalculateHashData(*stk);
                    stk->hashStack = CalculateHashStack(*stk);)
        
    yaissert(StackDump(stk));
    return noErr;
}


enum ERRORS ProcessorMul(Stack_t *stk)
{
    enum ERRORS checkStack = CheckStack(*stk);
    if(checkStack != noErr)
    {
        printf(RED " when trying to multiply\n\n" RESET);
        yaissert(StackDump(stk));
        return checkStack;
    }    
    yaissert(Verificator(stk));

    int size = stk->size;
    if(size < 2)
    {
        printf("Too few elements for multiply\n\n" RESET);
        return cantAdd;
    }

    StackElem_t mul = (stk->data[size - 2]) * stk->data[size - 1]; 
    stk->data[size - 2] = mul;
    stk->data[size - 1] = stk->poison_v;
    stk->size -= 1;
    ResizeDown(stk);

    HASH_PROTECTION(stk->hashData = CalculateHashData(*stk);
                    stk->hashStack = CalculateHashStack(*stk);)
        
    yaissert(StackDump(stk));
    return noErr;
}