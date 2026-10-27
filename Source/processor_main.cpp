#include "../Headers/processor_func.h"
#include "../Headers/draw_func.h"

main(int argc, char **argv)
{
    for(size_t i = 0; i < number_of_commands_; i++)
        commands_processor[i].cmd = commands_global_[i];

    OptionsProc Options;
    Options.filename = NULL;
    GetOptionsProc(argc, argv, &Options);   
    DelNameExt(&Options);
    char *filenamebin = CreateName(Options.filename, "bin");


    ByteCodeStruct byteCodeStruct = ReadFileBytecode(filenamebin);
    ProcessorStruct *proc = ProcessorStructInit(byteCodeStruct);

    SetTraceLogLevel(LOG_ERROR);

    // int line = 0;
    while (proc->ip < byteCodeStruct.size)
    {
        COMMAND_INDEX index = (COMMAND_INDEX)proc->byteCodeStruct.byteCode[proc->ip];
        View indexP;
        indexP.value = index;
        
        // printf("line = %i\n", line++);
        ON_PROC_DEBUG(
            printf(_CYAN "%s\n\n" _RESET, commands_processor[indexP.bits.b0].cmd.name);
            PrintProcessor(proc);
        )
        // StackDump(proc->stk);

        commands_processor[indexP.bits.b0].function(proc);
    }
    while(!WindowShouldClose())
        ;
}