typedef u32 instruction;

struct VM {
    instruction* memory;
    int pc;
    
    union {
        struct {
            int RA;
            int RB;
            int RC;
            int RD;
        };
        int registers[4];
    };
    b32 overflow;
    b32 carry;
    b32 positive;
    b32 negative;
    b32 zero;
};

#define MEMORY_SIZE (65535)
#define GPIO (0x4000)
#define NU (0x8000)
#define RAM (0x0)



#define INSTRUCTION_LENGTH (24)
#define OPCODE_LENGTH (6)
#define REGISTER_LENGTH (2)

#define get_opcode(instr) (((instr) >> (INSTRUCTION_LENGTH - 6)) & 0b111111)
#define get_register_x(instr) (((instr) >> (INSTRUCTION_LENGTH - 8)) & 0b11)
#define get_register_y(instr) (((instr) >> (INSTRUCTION_LENGTH - 10)) & 0b11)
#define get_operand(instr) ((instr) & 0xFFFF)
#define get_call_address(instr) ((instr) & 0b11111111111111)
#define get_address(instr) ((instr) & 0b1111111111111111)

enum OpCodes {
    
    OP_MOVE_ABSOLUTE = 0,
    OP_ADD_ABSOLUTE,
    OP_SUB_ABSOLUTE,
    OP_LOAD_ABSOLUTE,
    OP_STORE_ABSOLUTE,
    OP_AND_ABSOLUTE,
    OP_OR_ABSOLUTE,
    OP_XOR_ABSOLUTE,
    
    OP_MOVE_REGISTER  = 0b010000,
    OP_ADD_REGISTER   = 0b010001,
    OP_SUB_REGISTER   = 0b010010,
    OP_LOAD_REGISTER  = 0b010011,
    OP_STORE_REGISTER = 0b010100,
    OP_AND_REGISTER   = 0b010101,
    OP_OR_REGISTER    = 0b010110,
    OP_XOR_REGISTER   = 0b010111,
    OP_SR0_REGISTER   = 0b011000,
    OP_SL0_REGISTER   = 0b011001,
    OP_ROR_REGISTER   = 0b011010,
    OP_ROL_REGISTER   = 0b011011,
    
    OP_JUMP_UNCONDITIONAL = 0b100000,
    OP_JUMP_ZERO = 0b100001,
    OP_JUMP_NOT_ZERO = 0b100010,
    OP_JUMP_CARRY = 0b100011,
    OP_JUMP_NO_CARRY = 0b100100,
    OP_JUMP_NEGATIVE = 0b100101,
    OP_JUMP_NOT_NEGATIVE = 0b100110,
    OP_JUMP_POSITIVE = 0b100111,
    OP_JUMP_NOT_POSITIVE = 0b101000,
    OP_JUMP_OVERFLOW = 0b101001,
    OP_JUMP_NO_OVERFLOW = 0b101010,
    
    OP_JUMP_REGISTER = 0b101011,
    OP_CALL = 0b101100,
    OP_RETURN = 0b101101,
};