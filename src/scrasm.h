#ifndef SCRASM_H_
#define SCRASM_H_

enum asm_instr_type
{
    OP_SECT = 1,
    OP_MOV,
    OP_ADD,
    OP_CALL,
};

enum asm_reg
{
    REG_A = 1,
    REG_B,
};

enum asm_operand_type
{
    OPER_REG = 1,
    OPER_FUNC,
};

struct asm_operand
{
    enum asm_operand_type type;
    union
    {
        enum asm_reg reg;
        char* func;
        char* section;
    };
};

struct asm_instr
{
    enum asm_instr_type instr;
    struct asm_operand oper_a;
    struct asm_operand oper_b;
};

#endif
