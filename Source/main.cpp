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
    // char fileName[]  = "./Code_txt/PrintSquare.txt";
    // char fileName1[] = "./Code_bin/PrintSquare.bin";

    // char fileName[]  = "./Code_txt/Test.txt";
    // char fileName1[] = "./Code_bin/Test.bin";

    char fileName[]  = "./Code_txt/TestLabels.txt";
    char fileName1[] = "./Code_bin/TestLabels.bin";

    ByteCodeStruct byteCodeStruct = ReadFile(fileName);
    WriteByteCode(fileName1, byteCodeStruct);
    StackInit(&stk);
    RunProcessor(byteCodeStruct, &stk);
}