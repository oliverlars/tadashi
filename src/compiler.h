
struct Variable {
    Token name;
    Token string;
    int address;
    bool is_array;
    bool is_string;
    u64 array_length;
};

struct Scope {
    Variable variables[256];
    int variable_count;
    Scope* parent;
};

struct Function {
    Token name;
    int address;
    int stack_ptr;
    Ast_Node* params;
    Scope* scope;
    Scope* top_level_scope;
};

#define MAX_VARIABLE (2048)
#define MAX_FUNCTION (1024)

struct Compiler {
    instruction* at;
    instruction* start;
    
    bool has_comment;
    char* comment;
    
    Variable variables[MAX_VARIABLE]; //arbitrary limit
    int variable_count;
    int variable_start;
    
    Scope* current_scope;
    
    Function functions[MAX_FUNCTION];
    int function_count;
    
    int stack_ptr = 8192;
    
    Arena comment_arena;
    
    
};


enum Register {
    RA = 0,
    RB,
    RC,
    RD,
};