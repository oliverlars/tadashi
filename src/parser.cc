
internal bool
ast_has_property(Ast_Node* ast, Ast_Property property){
    return !!(ast->properties[property / 64] & (1ll << (property % 64)));
}

internal void
ast_set_property(Ast_Node* ast, Ast_Property property){
    ast->properties[property / 64] |= (1ll << (property % 64));
}

internal void
ast_remove_property(Ast_Node* ast, Ast_Property property){
    ast->properties[property / 64] &= ~(1ll << (property % 64));
}

internal Ast_Node*
make_binary_node(){
    auto binary = push_type_zero(&global_node_arena, Ast_Node);
    ast_set_property(binary, AST_HAS_LEFT);
    ast_set_property(binary, AST_HAS_RIGHT);
    return binary;
}

internal Ast_Node*
make_function_node(){
    auto func = push_type_zero(&global_node_arena, Ast_Node);
    ast_set_property(func, AST_HAS_PARAMETERS);
    ast_set_property(func, AST_HAS_SCOPE);
    return func;
}

internal Ast_Node*
make_declaration_node(){
    auto decl = push_type_zero(&global_node_arena, Ast_Node);
    ast_set_property(decl, AST_HAS_EXPRESSION);
    return decl;
}

