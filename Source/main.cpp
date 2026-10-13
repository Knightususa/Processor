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

    char fileName[] = "program1.txt";
    char fileName1[] = "program1.bin";
    ByteCodeStruct byteCodeStruct = ReadFile(fileName);
    WriteByteCode(fileName1, byteCodeStruct);
    StackInit(&stk);
    RunProcessor(byteCodeStruct, &stk);
}