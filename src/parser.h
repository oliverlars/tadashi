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
};

enum Op_Type {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
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
            Ast_Node* right;
        } unary;
        struct {
            Ast_Node* expr;
        } decl;
        struct {
            Ast_Node* members;
        } scope;
        struct {
            Ast_Node* func;
        } call;
        struct {
            Ast_Node* parameters;
            Ast_Node* body;
        } func;
        
        struct {
            char number; 
        }value;
    };
};

global Arena global_node_arena;
