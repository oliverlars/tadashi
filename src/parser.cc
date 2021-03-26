
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

internal Ast_Node*
make_call_node(){
    auto call = make_ast_node();
    call->type = AST_FUNCTION_CALL;
    return call;
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
        if(peek_token(&p->l).type == TOKEN_LEFT_PAREN){
            auto call = make_call_node();
            call->name = token;
            get_token(&p->l);
            get_token(&p->l); //HACK(Oliver): just to make the parser happy, 
            // should parse arguments properly
        }else {
            auto identifier = make_identifier_node();
            identifier->name = token;
            return identifier;
        }
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
    auto decl = make_declaration_node();
    //get_token(&p->l); // equals
    expect_token(&p->l, TOKEN_EQUALS);
    decl->decl.expr = parse_expr(p);
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
parse_call(Parser* p){
    return nullptr;
}

internal Ast_Node*
parse_stmt(Parser* p){
    Ast_Node* stmt;
    Token peek = peek_token(&p->l);
    if(token_equals_string(peek, "return")){
        stmt = parse_return(p);
    }else if(peek.type == TOKEN_IDENTIFIER){
        auto name = get_token(&p->l);
        if(peek_token(&p->l).type == TOKEN_LEFT_PAREN){
            stmt = parse_call(p);
        }else {
            stmt = parse_decl(p);
        }
        stmt->name = name;
    }
    expect_token(&p->l, TOKEN_SEMICOLON); //semicolon
    return stmt;
}

Ast_Node* parse_scope(Parser* p);

internal Ast_Node*
parse_function(Parser* p){
    get_token(&p->l); //skip fn
    auto name = get_token(&p->l);
    
    auto func = make_function_node();
    func->name = name;
    
    expect_token(&p->l, TOKEN_LEFT_PAREN);
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
    auto left_bracket = expect_token(&p->l, TOKEN_LEFT_BRACKET);
    auto scope = parse_scope(p);
    auto right_bracket = expect_token(&p->l, TOKEN_RIGHT_BRACKET);
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
    scope->scope.members->next = parse_function(p);
    return scope;
}


internal void
pretty_print(FILE* file, Ast_Node* root, int indent=-1){
    if(!root) return;
    for(int i = 0; i < indent; i++){
        fprintf(file, "  ");
    }
    switch(root->type){
        
        case AST_IDENTIFIER:{
            fprintf(file, "%.*s", root->name.length, root->name.at);
        }break;
        
        case AST_BINARY:{
            fprintf(file, "(");
            char* ops[] = { "+", "-", "*", "/" };
            fprintf(file, "%s ", ops[root->binary.op_type]);
            pretty_print(file, root->binary.left, indent);
            pretty_print(file, root->binary.right, indent);
            fprintf(file, ")");
            
        }break;
        
        case AST_FUNCTION: {
            fprintf(file, "(");
            fprintf(file, "function %.*s\n", root->name.length, root->name.at);
            pretty_print(file, root->func.parameters);
            pretty_print(file, root->func.body, indent);
            fprintf(file, ")\n");
        }break;
        
        case AST_FUNCTION_CALL: {
            fprintf(file, "(");
            fprintf(file, "call %.*s ", root->name.length, root->name.at);
            pretty_print(file, root->call.arguments, indent);
            fprintf(file, ")");
        }break;
        
        case AST_UNARY: {
            fprintf(file, "(");
            char* ops[] = { "+", "-" };
            fprintf(file, "%s ", ops[root->unary.op_type]);
            pretty_print(file, root->unary.right, indent);
            fprintf(file, ")");
        }break;
        
        case AST_DECLARATION: {
            fprintf(file, "(");
            fprintf(file, "decl ");
            fprintf(file, "%.*s", root->name.length, root->name.at);
            pretty_print(file, root->decl.expr, indent);
            fprintf(file, ")\n");
        }break;
        
        case AST_SCOPE: {
            auto member = root->scope.members;
            for(;member; member = member->next){
                pretty_print(file, member, indent+1);
            }
            
        }break;
        
        case AST_VALUE: {
            fprintf(file, "%d", root->value.number, indent);
        }break;
        
        case AST_RETURN: {
            fprintf(file, "(");
            fprintf(file, "return ");
            pretty_print(file, root->ret.expr, indent);
            fprintf(file, ")");
        }break;
    }
}
