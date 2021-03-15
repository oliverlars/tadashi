

internal void
emit_move_absolute(Byte_Code* bc, Register r, int value){
    *bc->at = OP_MOVE_ABSOLUTE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *bc->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *bc->at += value;
    bc->at++;
}

internal void
emit_store_absolute(Byte_Code* bc, Register r, int value){
    *bc->at = OP_STORE_ABSOLUTE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *bc->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *bc->at += value;
    bc->at++;
}

internal void
emit_add_absolute(Byte_Code* bc, Register r, int value){
    *bc->at = OP_ADD_ABSOLUTE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *bc->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *bc->at += value;
    bc->at++;
}

internal void
emit_store_register(Byte_Code* bc, Register x, Register y){
    *bc->at = OP_STORE_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *bc->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *bc->at += (int)y << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
    bc->at++;
}

internal void
compile_expression(Ast_Node* root, Byte_Code* bc){
    
    switch(root->type){
        case AST_VALUE: {
        }break;
    }
    
}

internal void
compile_declaration(Ast_Node* root, Byte_Code* bc){
    auto decl = root->decl;
    auto expr = decl.expr;
    switch(expr->type){
        case AST_VALUE: {
            emit_move_absolute(bc, RA, expr->value.number);
            emit_store_register(bc, RA, RC);
            emit_add_absolute(bc, RC, 1);
        }break;
    }
}