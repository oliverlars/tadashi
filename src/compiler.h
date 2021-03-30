
struct Variable {
    Token name;
    Token string;
    int address;
    bool is_array;
    bool is_string;
    int array_length;
};

struct Function {
    Token name;
    int address;
    int stack_ptr;
};

struct Compiler {
    instruction* at;
    instruction* start;
    
    Variable variables[256]; //arbitrary limit
    int variable_count;
    
    Function functions[256];
    int function_count;
    
    int stack_ptr = 512;
};


enum Register {
    RA = 0,
    RB,
    RC,
    RD,
};