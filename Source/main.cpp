#include "../Headers/stack.h"
#include "../Headers/assembler.h"
#include "../Headers/processor_func.h"


main()
{
    Stack_t stk =
    {
        CANARY_PROTECTION(.arbusL = 0,
                          .arbusDefault = ArbusDefault,)
        .size = -1,
        .capacity = 1,
        .data = NULL,
        .poison_v = POISON_V
        CANARY_PROTECTION(,
                          .canaryL = NULL,
                          .canaryR = NULL,
                          .canaryDefault = CanaryDefault)
        HASH_PROTECTION(,
                        .hashData = 0,
                        .hashStack = 0)
        CANARY_PROTECTION(,
                          .arbusR = 0,)
    };

    StackInit(&stk);
    printf("Input command(and if its necessary argument also)\n"\
            "PUSH, ADD, SUB, DIV, MUL\n");
    char fileName[] = "program1.txt";
    int nCommands = 0;
    command *command1 = ReadFile(fileName, &nCommands);
    for(int i = 0; i < nCommands; i++)
    {
        printf("%s ", command1[i].name);
        printf("%i\n", command1[i].argument);
    }
    /*
    switch(command1.index)
    {
        case(push_index):
            StackPush(&stk, 1);
            break;
            
        case (add_index):
            ProcessorAdd(&stk);
            break;
            
        case (sub_index):
            ProcessorSub(&stk);
            break;
            
        case (div_index):
            ProcessorDiv(&stk);
            break;
            
        case (mul_index):
            ProcessorMul(&stk);
            break;
        default:
            printf("sfsdfs");
            break;
            
    }
    printf("name = %s, index = %i, argument = %i\n\n", command1.name, command1.index, command1.argument);
    */
}