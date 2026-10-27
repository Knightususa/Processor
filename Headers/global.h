#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STRINGIFY(x) #x
#define MAKEVALUE(x) STRINGIFY(x)


enum COMMAND_INDEX
{
    hlt_index     = 0,
   
    push_index    = 1,
    out_index     = 2,
   
    pop_index     = 3,

    add_index     = 4,
    sub_index     = 5,
    div_index     = 6,
    mul_index     = 7,
    sqrt_index    = 8,
   
    jmp_index     = 9,
    je_index      = 10,
    jne_index     = 11,
    ja_index      = 12,
    jae_index     = 13,
    jb_index      = 14,
    jbe_index     = 15,

    in_index      = 16,
    call_index    = 17,
    ret_index     = 18,

    outc_index    = 19,
    draw_index    = 20,
    initw_index   = 21,
    closew_index  = 22,
    rand_index    = 23,

    notcmd_index  = 24
};

typedef struct
{
    COMMAND_INDEX index;
    const char *name;
    int args;
}CommandGlobal;

#define MakeCommandGlobal(nameCmd, argsCmd)             \
    {.index = nameCmd ## _index, .name = #nameCmd, .args = argsCmd}

const CommandGlobal commands_global_[] = 
{
    MakeCommandGlobal(hlt, 0),

    MakeCommandGlobal(push, 1),
    MakeCommandGlobal(out, 0),

    MakeCommandGlobal(pop, 1),

    MakeCommandGlobal(add, 0),
    MakeCommandGlobal(sub, 0),
    MakeCommandGlobal(div, 0),
    MakeCommandGlobal(mul, 0),
    MakeCommandGlobal(sqrt, 0),

    MakeCommandGlobal(jmp, 1),
    MakeCommandGlobal(je, 1),
    MakeCommandGlobal(jne, 1),
    MakeCommandGlobal(ja, 1),
    MakeCommandGlobal(jae, 1),
    MakeCommandGlobal(jb, 1),
    MakeCommandGlobal(jbe, 1),

    MakeCommandGlobal(in, 0),
    MakeCommandGlobal(call, 1),
    MakeCommandGlobal(ret, 0),

    MakeCommandGlobal(outc, 0),
    MakeCommandGlobal(draw, 0),
    MakeCommandGlobal(initw, 0),
    MakeCommandGlobal(closew, 0),
    MakeCommandGlobal(rand, 1),

    MakeCommandGlobal(notcmd, 0),
};
const size_t number_of_commands_ = sizeof(commands_global_) / sizeof(commands_global_[0]);

typedef struct
{
    unsigned char *byteCode;
    size_t size;
}ByteCodeStruct;

typedef struct
{
    unsigned char b0 :5;
    unsigned char b1 :1;
    unsigned char b2 :1;
    unsigned char b3 :1;
}BitField;

typedef union
{
    unsigned char value;
    BitField      bits;
}View;

#define CommandMaxLen  31
#define ArgumentMaxLen 31

const size_t size_VRAM_x = 100;
const size_t size_VRAM_y = 100;
const size_t size_VRAM = size_VRAM_x * size_VRAM_y;

const size_t size_RAM = size_VRAM + 200;
const size_t delta_size_RAM = size_RAM - size_VRAM;

const size_t size_registers = 10;

#endif

//end #ifndef GLOBAL_H