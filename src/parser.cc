
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

internal Ast_Node*
make_for_node(){
    auto _for = make_ast_node();
    _for->type = AST_FOR;
    return _for;
}

internal Ast_Node*
make_index_node(){
    auto index = make_ast_node();
    index->type = AST_INDEX;
    return index;
}

internal Ast_Node*
make_if_node(){
    auto _if = make_ast_node();
    _if->type = AST_IF;
    return _if;
}

internal Ast_Node*
make_while_node(){
    auto _while = make_ast_node();
    _while->type = AST_WHILE;
    return _while;
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
            expect_token(&p->l, TOKEN_LEFT_PAREN);
            
            auto args = &call->call.arguments;
            while(!(peek_token(&p->l).type == TOKEN_RIGHT_PAREN)){
                auto arg = parse_expr(p);
                *args = arg;
                args = &(*args)->next;
                
                if(!token_equals_string(peek_token(&p->l), ",")) {
                    break;
                }else {
                    get_token(&p->l);
                }
            }
            expect_token(&p->l, TOKEN_RIGHT_PAREN);
            
            return call;
            // should parse arguments properly
        }else if(peek_token(&p->l).type == TOKEN_LEFT_BRACKET){
            get_token(&p->l);
            auto index = make_index_node();
            index->name = token;
            index->index.offset = parse_expr(p);
            expect_token(&p->l, TOKEN_RIGHT_BRACKET);
            return index;
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
    }else if (token.type == TOKEN_CHAR){
        auto value = make_value_node();
        value->value.number = *token.at;
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
    auto peek = peek_token(&p->l);
    
    while(peek.type == TOKEN_ASTERISK ||
          (peek.type == TOKEN_FORWARD_SLASH) ||
          (peek.type == TOKEN_RIGHT_ANGLE_RIGHT_ANGLE) ||
          (peek.type == TOKEN_LEFT_ANGLE_LEFT_ANGLE) ||
          (peek.type == TOKEN_HAT)){
        auto bin = make_binary_node();
        if(peek.type == TOKEN_ASTERISK){
            bin->binary.op_type = OP_MUL;
        }
        else if(peek.type == TOKEN_LEFT_ANGLE_LEFT_ANGLE){
            bin->binary.op_type = OP_SL;
        }
        else if(peek.type == TOKEN_RIGHT_ANGLE_RIGHT_ANGLE){
            bin->binary.op_type = OP_SR;
        }else if(peek.type == TOKEN_HAT) {
            bin->binary.op_type = OP_XOR;
        }else {
            bin->binary.op_type = OP_DIV;
        }
        
        get_token(&p->l);
        
        bin->binary.left = expr;
        bin->binary.right = parse_unary_expr(p);
        
        expr = bin;
        peek = peek_token(&p->l);
    }
    
    return expr;
}

internal Ast_Node*
parse_binary_expr(Parser* p){
    auto expr = parse_mul_expr(p);
    
    auto peek = peek_token(&p->l);
    while((peek.type == TOKEN_PLUS) || 
          (peek.type == TOKEN_MINUS) ||
          (peek.type == TOKEN_LEFT_ANGLE) ||
          (peek.type == TOKEN_RIGHT_ANGLE) ||
          (peek.type == TOKEN_LEFT_ANGLE_EQUAL) ||
          (peek.type == TOKEN_RIGHT_ANGLE_EQUAL) ||
          (peek.type == TOKEN_EQUALS_EQUALS) ||
          (peek.type == TOKEN_BANG_EQUALS) ||
          (peek.type == TOKEN_AMPERSAND_AMPERSAND) ||
          (peek.type == TOKEN_BAR_BAR)){
        auto bin = make_binary_node();
        if(peek.type == TOKEN_PLUS){
            bin->binary.op_type = OP_ADD;
        }else if(peek.type == TOKEN_MINUS){
            bin->binary.op_type = OP_SUB;
        }else if(peek.type == TOKEN_LEFT_ANGLE){
            bin->binary.op_type = OP_LT;
        }else if(peek.type == TOKEN_RIGHT_ANGLE){
            bin->binary.op_type = OP_GT;
        }else if(peek.type == TOKEN_LEFT_ANGLE_EQUAL){
            bin->binary.op_type = OP_LTE;
        }else if(peek.type == TOKEN_RIGHT_ANGLE_EQUAL){
            bin->binary.op_type = OP_GTE;
        }else if(peek.type == TOKEN_AMPERSAND_AMPERSAND){
            bin->binary.op_type = OP_LOGICAL_AND;
        }else if(peek.type == TOKEN_BAR_BAR){
            bin->binary.op_type = OP_LOGICAL_OR;
        }else if(peek.type == TOKEN_EQUALS_EQUALS){
            bin->binary.op_type = OP_EQ;
        }else if(peek.type == TOKEN_BANG_EQUALS){
            bin->binary.op_type = OP_NOT_EQ;
        }
        
        get_token(&p->l);
        
        bin->binary.left = expr;
        bin->binary.right = parse_mul_expr(p);
        
        expr = bin;
        peek = peek_token(&p->l);
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
    
    if(peek_token(&p->l).type == TOKEN_LEFT_BRACKET){
        get_token(&p->l);
        decl->decl.offset = parse_expr(p);
        expect_token(&p->l, TOKEN_RIGHT_BRACKET);
    }
    
    expect_token(&p->l, TOKEN_EQUALS);
    if(peek_token(&p->l).type == TOKEN_LEFT_BRACKET){
        get_token(&p->l);
        decl->decl.array_length = token_to_value(expect_token(&p->l, TOKEN_NUMBER));
        decl->decl.is_array = true;
        expect_token(&p->l, TOKEN_RIGHT_BRACKET);
    }else if(peek_token(&p->l).type == TOKEN_STRING){
        auto string = get_token(&p->l);
        decl->decl.string = string;
    }else {
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
parse_call(Parser* p){
    return nullptr;
}

Ast_Node* parse_scope(Parser* p);

internal Ast_Node*
parse_stmt(Parser* p);

internal Ast_Node*
parse_for(Parser* p){
    get_token(&p->l);
    auto name = get_token(&p->l);
    auto decl = parse_decl(p);
    decl->name = name;
    
    expect_token(&p->l, TOKEN_SEMICOLON);
    auto cond = parse_expr(p);
    expect_token(&p->l, TOKEN_SEMICOLON);
    name = get_token(&p->l);
    auto stmt = parse_decl(p);
    stmt->name = name;
    expect_token(&p->l, TOKEN_LEFT_BRACE);
    auto body = parse_scope(p);
    
    expect_token(&p->l, TOKEN_RIGHT_BRACE);
    auto _for = make_for_node();
    _for->_for.decl = decl;
    _for->_for.cond = cond;
    _for->_for.stmt = stmt;
    _for->_for.body = body;
    return _for;
}

internal Ast_Node*
parse_if(Parser* p){
    auto _if = make_if_node();
    get_token(&p->l);
    auto expr = parse_expr(p);
    expect_token(&p->l, TOKEN_LEFT_BRACE);
    auto scope = parse_scope(p);
    expect_token(&p->l, TOKEN_RIGHT_BRACE);
    auto peek = peek_token(&p->l);
    _if->_if.expr = expr;
    _if->_if.body = scope;
    if(token_equals_string(peek, "else")){
        get_token(&p->l);
        expect_token(&p->l, TOKEN_LEFT_BRACE);
        auto else_scope = parse_scope(p);
        _if->_if._else = else_scope;
        expect_token(&p->l, TOKEN_RIGHT_BRACE);
    }
    return _if;
}

internal Ast_Node*
parse_while(Parser* p) {
    auto _while = make_while_node();
    get_token(&p->l);
    auto expr = parse_expr(p);
    expect_token(&p->l, TOKEN_LEFT_BRACE);
    auto scope = parse_scope(p);
    _while->_while.expr = expr;
    _while->_while.body = scope;
    expect_token(&p->l, TOKEN_RIGHT_BRACE);
    return _while;
}

internal Ast_Node*
parse_stmt(Parser* p){
    Ast_Node* stmt;
    Token peek = peek_token(&p->l);
    if(token_equals_string(peek, "return")){
        stmt = parse_return(p);
        expect_token(&p->l, TOKEN_SEMICOLON); //semicolon
    }else if(token_equals_string(peek, "for")){
        stmt = parse_for(p);
    }
    else if(token_equals_string(peek, "if")){
        stmt = parse_if(p);
    }else if(token_equals_string(peek, "while")){
        stmt = parse_while(p);
    }else if(peek.type == TOKEN_IDENTIFIER){
        auto name = get_token(&p->l);
        if(peek_token(&p->l).type == TOKEN_LEFT_PAREN){
            stmt = parse_call(p);
            expect_token(&p->l, TOKEN_SEMICOLON); //semicolon
        }else {
            stmt = parse_decl(p);
            expect_token(&p->l, TOKEN_SEMICOLON); //semicolon
        }
        stmt->name = name;
    }
    return stmt;
}

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
        //auto param = parse_decl(p);
        auto param = make_declaration_node();
        param->name = get_token(&p->l);
        
        if(peek_token(&p->l).type == TOKEN_LEFT_BRACKET){
            get_token(&p->l);
            expect_token(&p->l, TOKEN_RIGHT_BRACKET);
        }
        
        *params = param;
        params = &(*params)->next;
        
        if(!token_equals_string(peek_token(&p->l), ",")) {
            break;
        }else {
            get_token(&p->l);
        }
    }
    get_token(&p->l);
    auto left_bracket = expect_token(&p->l, TOKEN_LEFT_BRACE);
    auto scope = parse_scope(p);
    auto right_bracket = expect_token(&p->l, TOKEN_RIGHT_BRACE);
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
    auto funcs = &scope->scope.members;
    while(token_equals_string(peek_token(&p->l), "fn")){
        auto func = parse_function(p);
        *funcs = func;
        funcs = &(*funcs)->next;
    }
    return scope;
}


internal void
pretty_print(FILE* file, Ast_Node* root, int indent=0){
    if(!root) return;
    if(root->type != AST_SCOPE){
        for(int i = 0; i < indent; i++){
            fprintf(file, "  ");
        }
    }
    switch(root->type){
        
        case AST_IDENTIFIER:{
            fprintf(file, "%.*s ", root->name.length, root->name.at);
        }break;
        
        case AST_BINARY:{
            fprintf(file, "(");
            char* ops[] = { "+", "-", "*", "/", "<", ">", "<=", ">=", "!=", "==" };
            fprintf(file, "%s ", ops[root->binary.op_type]);
            pretty_print(file, root->binary.left);
            pretty_print(file, root->binary.right);
            fprintf(file, ")");
            
        }break;
        
        case AST_FUNCTION: {
            fprintf(file, "(");
            fprintf(file, "function %.*s\n", root->name.length, root->name.at);
            pretty_print(file, root->func.parameters);
            pretty_print(file, root->func.body, indent+1);
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
            pretty_print(file, root->decl.expr, 0);
            fprintf(file, ")\n");
        }break;
        
        case AST_SCOPE: {
            auto member = root->scope.members;
            for(;member; member = member->next){
                pretty_print(file, member, indent);
            }
            
        }break;
        
        case AST_VALUE: {
            fprintf(file, "%d ", root->value.number);
        }break;
        
        case AST_RETURN: {
            fprintf(file, "(");
            fprintf(file, "return ");
            pretty_print(file, root->ret.expr, 0);
            fprintf(file, ")");
        }break;
        
        case AST_FOR: {
            fprintf(file, "(");
            fprintf(file, "for ");
            pretty_print(file, root->_for.decl);
            pretty_print(file, root->_for.cond);
            pretty_print(file, root->_for.stmt);
            fprintf(file, "\n");
            pretty_print(file, root->_for.body, indent+1);
            fprintf(file, ")");
        }break;
    }
}
