struct Parser {
    
};

enum Ast_Property {
    AST_HAS_IDENTIFIER,,
    AST_HAS_EXPRESSION,
    AST_HAS_MEMBERS,
    AST_HAS_PARAMETERS,
    AST_HAS_SCOPE,
    AST_HAS_LEFT,
    AST_HAS_RIGHT,
};

#define AST_PROPTIES_MAX 256

struct Ast_Node {
    
    Token identifier;
    
    Ast_Node* node;
    Ast_Node* next;
    Ast_Node* prev;
    
    Ast_Node* parameters;
    Ast_Node* members;
    Ast_Node* scope;
    Ast_Node* expr;
    Ast_Node* left;
    Ast_Node* right;
    
    u64 properties[(AST_PROPTIES_MAX+63)/64];
};

global Arena global_node_arena;
