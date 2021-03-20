
struct Variable {
    Token name;
    int address;
};

struct Function {
    Token name;
    int address;
};

struct Compiler {
    instruction* at;
    instruction* start;
    
    Variable variables[256]; //arbitrary limit
    int variable_count;
    
    Function functions[256];
    int function_count;
    
    int stack_ptr = 256;
};


enum Register {
    RA = 0,
    RB,
    RC,
    RD,
};