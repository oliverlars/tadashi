
internal Ast_Node*
make_ast_node(){
    auto ast = push_type_zero(&global_node_arena, Ast_Node);
    return ast;
}

internal Ast_Node*
make_binary_node(){
    auto binary = make_ast_node();
    binary->type = AST_BINARY;
    return binary;
}

internal Ast_Node*
make_function_node(){
    auto func = make_ast_node();
    func->type = AST_FUNCTION;
    return func;
}

internal Ast_Node*
make_declaration_node(){
    auto decl = make_ast_node();
    decl->type = AST_DECLARATION;
    return decl;
}

internal Ast_Node*
make_identifier_node(){
    auto id = make_ast_node();
    id->type = AST_IDENTIFIER;
    return id;
}

internal Ast_Node*
make_scope_node(){
    auto scope = make_ast_node();
    scope->type = AST_SCOPE;
    return scope;
}

internal Ast_Node*
make_value_node(){
    auto value = make_ast_node();
    value->type = AST_VALUE;
    return value;
}

internal Ast_Node*
make_return_node(){
    auto ret = make_ast_node();
    ret->type = AST_RETURN;
    return ret;
}

internal int
token_to_value(Token token){
    int result = 0;
    for(int i = 0; i < token.length; i++){
        result = token.at[i] - '0' + result*10;
    }
    return result;
}

Ast_Node* parse_expr(Parser* p);

internal Ast_Node*
parse_base_expr(Parser* p){
    auto token = get_token(&p->l);
    if(token.type == TOKEN_LEFT_PAREN){
        auto expr = parse_expr(p);
        get_token(&p->l);
        return expr;
    }
    if(token.type == TOKEN_IDENTIFIER){
        auto identifier = make_identifier_node();
        identifier->name = token;
        return identifier;
    }
    
    if(token.type == TOKEN_NUMBER){
        auto value = make_value_node();
        value->value.number = token_to_value(token);
        return value;
    }
    
    return nullptr;
}

internal Ast_Node*
parse_unary_expr(Parser* p){
    auto unary = parse_base_expr(p);
    return unary;
}

internal Ast_Node*
parse_mul_expr(Parser* p){
    auto expr = parse_unary_expr(p);
    
    while((peek_token(&p->l).type == TOKEN_ASTERISK ||
           ((peek_token(&p->l).type == TOKEN_FORWARD_SLASH)))){
        auto bin = make_binary_node();
        if(peek_token(&p->l).type == TOKEN_ASTERISK){
            bin->binary.op_type = OP_MUL;
        }else {
            bin->binary.op_type = OP_DIV;
        }
        
        get_token(&p->l);
        
        bin->binary.left = expr;
        bin->binary.right = parse_unary_expr(p);
        
        expr = bin;
    }
    
    return expr;
}

internal Ast_Node*
parse_binary_expr(Parser* p){
    auto expr = parse_mul_expr(p);
    
    while((peek_token(&p->l).type == TOKEN_PLUS) || 
          (peek_token(&p->l).type == TOKEN_MINUS)){
        auto bin = make_binary_node();
        if(peek_token(&p->l).type == TOKEN_PLUS){
            bin->binary.op_type = OP_ADD;
        }else {
            bin->binary.op_type = OP_SUB;
        }
        
        get_token(&p->l);
        
        bin->binary.left = expr;
        bin->binary.right = parse_mul_expr(p);
        
        expr = bin;
    }
    
    return expr;
}

internal Ast_Node*
parse_expr(Parser* p){
    auto expr = parse_binary_expr(p);
    return expr;
}

internal Ast_Node*
parse_decl(Parser* p){
    auto token = get_token(&p->l);
    auto decl = make_declaration_node();
    decl->name = token;
    //get_token(&p->l); // equals
    if(token_equals_string(peek_token(&p->l), "=")){
        get_token(&p->l);
        decl->decl.expr = parse_expr(p);
    }
    return decl;
}

internal Ast_Node*
parse_return(Parser* p){
    get_token(&p->l); //return
    auto ret = make_return_node();
    auto expr = parse_expr(p);
    ret->ret.expr = expr;
    return ret;
}

internal Ast_Node*
parse_stmt(Parser* p){
    Ast_Node* stmt;
    if(token_equals_string(peek_token(&p->l), "return")){
        stmt = parse_return(p);
    }else {
        stmt = parse_decl(p);
    }
    get_token(&p->l); //semicolon
    return stmt;
}

Ast_Node* parse_scope(Parser* p);

internal Ast_Node*
parse_function(Parser* p){
    get_token(&p->l); //skip fn
    auto name = get_token(&p->l);
    
    auto func = make_function_node();
    func->name = name;
    
    get_token(&p->l); //left paren
    Token right_paren = {};
    right_paren.at = ")"; 
    right_paren.length = 1;
    
    auto params = &func->func.parameters;
    while(!tokens_equal(peek_token(&p->l), right_paren)){
        auto param = parse_decl(p);
        *params = param;
        params = &(*params)->next;
        
        if(!token_equals_string(peek_token(&p->l), ",")) {
            break;
        }
    }
    get_token(&p->l);
    auto left_bracket = get_token(&p->l);
    auto scope = parse_scope(p);
    auto right_bracket = get_token(&p->l);
    func->func.body = scope;
    
    return func;
}


internal Ast_Node*
parse_scope(Parser* p){
    Ast_Node* scope = make_scope_node();
    //scope->scope.members = parse_stmt(p);
    //scope->scope.members->next = parse_stmt(p);
    //scope->scope.members->next->next = parse_stmt(p);
    
    auto stmts = &scope->scope.members;
    while(!token_equals_string(peek_token(&p->l), "}")){
        auto stmt = parse_stmt(p);
        *stmts = stmt;
        stmts = &(*stmts)->next;
    }
    
    //scope->scope.members = parse_function(p);
    //scope->scope.members->next = parse_function(p);
    
    return scope;
}

internal Ast_Node*
parse_global_scope(Parser* p){
    Ast_Node* scope = make_scope_node();
    
    scope->scope.members = parse_function(p);
    //scope->scope.members->next = parse_function(p);
    return scope;
}