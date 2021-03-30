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
    AST_IF,
    AST_WHILE,
};

enum Op_Type {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_LT,
    OP_GT,
    OP_LTE,
    OP_GTE,
    OP_NOT_EQ,
    OP_EQ,
    OP_LOGICAL_AND,
    OP_LOGICAL_OR,
    OP_SL,
    OP_SR,
    OP_XOR,
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
    int line_number;
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
            bool is_array;
            Ast_Node* offset;
            Token string;
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
            Ast_Node* decl;
            Ast_Node* cond;
            Ast_Node* stmt;
            Ast_Node* body;
        }_for;
        struct {
            Ast_Node* offset;
        }index;
        struct {
            Ast_Node* expr;
            Ast_Node* body;
            Ast_Node* else_if;
            Ast_Node* _else;
        }_if;
        struct {
            Ast_Node* expr;
            Ast_Node* body;
        }_while;
    };
};

global Arena global_node_arena;
