#include "../Stack_lib/Headers/stack.h"
#include "../Headers/assembler.h"
#include "../Headers/processor_func.h"


main()
{
    // char fileName[]  = "./Code_txt/PrintSquare.txt";
    // char fileName1[] = "./Code_bin/PrintSquare.bin";

    // char fileName[]  = "./Code_txt/Test.txt";
    // char fileName1[] = "./Code_bin/Test.bin";

    // char fileName[]  = "./Code_txt/TestLabels.txt";
    // char fileName1[] = "./Code_bin/TestLabels.bin";

    char fileName[]  = "./Code_txt/Fucktorial.txt";
    char fileName1[] = "./Code_bin/Fucktorial.bin";

    // char fileName[]  = "./Code_txt/test1.txt";
    // char fileName1[] = "./Code_bin/test1.bin";

    ByteCodeStruct byteCodeStruct = ProcReadFile(fileName);
    WriteByteCode(fileName1, byteCodeStruct);
    RunProcessor(byteCodeStruct);
}