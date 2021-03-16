
struct Variable {
    Token name;
};

struct Compiler {
    instruction* at;
    instruction* start;
    
    Variable variables[256]; //arbitrary limit
    int variable_count;
    
    int stack_ptr;
};

#define STACK_START (256)

enum Register {
    RA = 0,
    RB,
    RC,
    RD,
};