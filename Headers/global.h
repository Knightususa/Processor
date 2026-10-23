#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>

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

    notcmd_index = 21
};

typedef struct
{
    COMMAND_INDEX index;
    const char *name;
    int args;
}CommandGlobal;

#define XXX(idx, name, args) \
    {.index = idx,     .name = name,  .args = args},


const CommandGlobal commands_global_[] = 
{
    {.index = hlt_index,     .name = "hlt",  .args = 0},
    
    {.index = push_index,    .name = "push", .args = 1},
    {.index = out_index,     .name = "out",  .args = 0},

    {.index = pop_index,     .name = "pop",  .args = 1},

    {.index = add_index,     .name = "add",  .args = 0},
    {.index = sub_index,     .name = "sub",  .args = 0},
    {.index = div_index,     .name = "div",  .args = 0},
    {.index = mul_index,     .name = "mul",  .args = 0},
    {.index = sqrt_index,    .name = "sqrt", .args = 0},

    {.index = jmp_index,     .name = "jmp",  .args = 1},
    {.index = je_index,      .name = "je",   .args = 1},
    {.index = jne_index,     .name = "jne",  .args = 1},
    {.index = ja_index,      .name = "ja",   .args = 1},
    {.index = jae_index,     .name = "jae",  .args = 1},
    {.index = jb_index,      .name = "jb",   .args = 1},
    {.index = jbe_index,     .name = "jbe",  .args = 1},

    {.index = in_index,      .name = "in",   .args = 0},
    {.index = call_index,    .name = "call", .args = 1},
    {.index = ret_index,     .name = "ret",  .args = 0},

    {.index = outc_index,    .name = "outc", .args = 0},
    {.index = draw_index,    .name = "draw", .args = 0},

    {.index = notcmd_index, .name = "notc",  .args = 0},
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
    double raw;
    struct
    {
        uint64_t r : 8;
        uint64_t g : 8;
        uint64_t b : 8;
        uint64_t a : 8;
        uint64_t   : 16;
        uint64_t   : 8;
        uint64_t   : 8;
    }bits;
}Pixel64;

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

const size_t size_RAM = size_VRAM + 100;
const size_t delta_size_RAM = size_RAM - size_VRAM;

const size_t size_registers = 10;

#endif
//end #ifndef GLOBAL_H