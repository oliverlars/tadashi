struct Parser {
    Lexer l;
};

enum Ast_Type {
    AST_INVALID,
    AST_IDENTIFIER,
    AST_BINARY,
    AST_LITERAL,
    AST_FUNCTION,
    AST_FUNCTION_CALL,
    AST_UNARY,
    AST_DECLARATION,
    AST_SCOPE,
    AST_VALUE,
    AST_RETURN,
    AST_FOR,
    AST_INDEX,
};

enum Op_Type {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
};

enum Prim_Type {
    PRIM_INT,
    PRIM_ARRAY,
};

struct Ast_Node {
    Token name;
    Ast_Type type;
    
    Ast_Node* next;
    Ast_Node* prev;
    
    union {
        struct {
            Op_Type op_type;
            Ast_Node* left;
            Ast_Node* right;
        }binary;
        struct {
            Op_Type op_type;
            Ast_Node* right;
        } unary;
        struct {
            Ast_Node* expr;
            int array_length;
            Ast_Node* offset;
        } decl;
        struct {
            Ast_Node* members;
        } scope;
        struct {
            Ast_Node* func;
            Ast_Node* arguments;
        } call;
        struct {
            Ast_Node* parameters;
            Ast_Node* body;
        } func;
        struct {
            int number; 
        }value;
        struct {
            Ast_Node* expr;
        }ret;
        struct {
            Ast_Node* min;
            Ast_Node* max;
            Ast_Node* body;
        }_for;
        struct {
            Ast_Node* offset;
        }index;
    };
};

global Arena global_node_arena;
