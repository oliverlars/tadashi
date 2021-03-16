

internal void
emit_move_absolute(Compiler* compiler, Register r, int value){
    *compiler->at = OP_MOVE_ABSOLUTE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += value;
    compiler->at++;
}

internal void
emit_store_absolute(Compiler* compiler, Register r, int address){
    *compiler->at = OP_STORE_ABSOLUTE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += address;
    compiler->at++;
}

internal void
emit_load_absolute(Compiler* compiler, Register r, int address){
    *compiler->at = OP_LOAD_ABSOLUTE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += address;
    compiler->at++;
}

internal void
emit_add_absolute(Compiler* compiler, Register r, int value){
    *compiler->at = OP_ADD_ABSOLUTE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += value;
    compiler->at++;
}

internal void
emit_sub_absolute(Compiler* compiler, Register r, int value){
    *compiler->at = OP_SUB_ABSOLUTE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += value;
    compiler->at++;
}

internal void
emit_move_register(Compiler* compiler, Register x, Register y){
    *compiler->at = OP_MOVE_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += (int)y << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
    compiler->at++;
}

internal void
emit_store_register(Compiler* compiler, Register x, Register y){
    *compiler->at = OP_STORE_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += (int)y << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
    compiler->at++;
}

internal void
emit_add_register(Compiler* compiler, Register x, Register y){
    *compiler->at = OP_ADD_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += (int)y << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
    compiler->at++;
}

internal void
emit_sub_register(Compiler* compiler, Register x, Register y){
    *compiler->at = OP_SUB_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += (int)y << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
    compiler->at++;
}

internal void
emit_jump_not_zero(Compiler* compiler, int address){
    *compiler->at = OP_JUMP_NOT_ZERO << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += address;
    compiler->at++;
}

internal int
find_local(Compiler* compiler, Token name){
    int result = 0;
    for(; result < 256; result++){
        if(tokens_equal(name, compiler->variables[result].name)){
            return result;
        }
    }
    return -1;
}

internal int
push_temporary(Compiler* compiler, int value){
    emit_move_absolute(compiler, RA, value);
    emit_store_absolute(compiler, RA, STACK_START + compiler->stack_ptr);
    return compiler->stack_ptr++;
}

internal int
push_temporary_from_register(Compiler* compiler, Register r){
    emit_store_absolute(compiler, r, STACK_START + compiler->stack_ptr);
    return compiler->stack_ptr++;
}

internal int
duplicate_temporary(Compiler* compiler, int address){
    emit_load_absolute(compiler, RA, STACK_START + address); 
    return push_temporary_from_register(compiler, RA);
}

internal int
pop_temporary(Compiler* compiler, Register r, int value){
    compiler->stack_ptr--;
}

internal int
push_local(Compiler* compiler, Token name, int value){
    emit_move_absolute(compiler, RA, value);
    emit_store_absolute(compiler, RA, STACK_START + compiler->stack_ptr);
    compiler->variables[compiler->variable_count++].name = name;
    return compiler->stack_ptr++;
}

internal void
push_local_address(Compiler* compiler, Token name, int address){
    emit_store_absolute(compiler, RA, STACK_START + address);
    compiler->variables[compiler->variable_count++].name = name;
}

internal void
add_temporary_absolute(Compiler* compiler, int address, int value){
    emit_load_absolute(compiler, RA,STACK_START +  address);
    emit_add_absolute(compiler, RA, value);
    emit_store_absolute(compiler, RA, STACK_START + address);
}

internal void
sub_temporary_absolute(Compiler* compiler, int address, int value){
    emit_load_absolute(compiler, RA, STACK_START + address);
    emit_sub_absolute(compiler, RA, value);
    emit_store_absolute(compiler, RA, STACK_START + address);
}

internal void
add_temporaries(Compiler* compiler, int address_x, int address_y){
    emit_load_absolute(compiler, RA, STACK_START + address_x);
    emit_load_absolute(compiler, RB, STACK_START + address_y);
    emit_add_register(compiler, RA, RB);
    emit_store_absolute(compiler, RA,STACK_START +  address_x);
}

internal void
sub_temporaries(Compiler* compiler, int address_x, int address_y){
    emit_load_absolute(compiler, RA, STACK_START + address_x);
    emit_load_absolute(compiler, RB, STACK_START + address_y);
    emit_sub_register(compiler, RA, RB);
    emit_store_absolute(compiler, RA,STACK_START +  address_x);
}

internal void
copy_temporary(Compiler* compiler, int address_x, int address_y){
    emit_load_absolute(compiler, RA, STACK_START + address_y);
    emit_store_absolute(compiler, RA, STACK_START + address_x);
}

//returns stack positions for variables/temporaries
internal int
compile_expression(Ast_Node* root, Register r, Compiler* compiler){
    
    switch(root->type){
        case AST_VALUE: {
            //emit_move_absolute(compiler, r, root->value.number);
            return push_temporary(compiler, root->value.number);
        }break;
        case AST_BINARY: {
            int a = compile_expression(root->binary.left, RA, compiler);
            int b = compile_expression(root->binary.right, RB, compiler);
            
            int result = duplicate_temporary(compiler, a);
            
            switch(root->binary.op_type){
                case OP_ADD:{
                    add_temporaries(compiler, result, b);
                }break;
                case OP_SUB:{
                    sub_temporaries(compiler, result, b);
                }break;
                case OP_MUL:{
                    sub_temporary_absolute(compiler, b, 1);
                    auto jump = compiler->at-1;
                    
                    add_temporaries(compiler, result, a);
                    
                    sub_temporary_absolute(compiler, b, 1);
                    
                    emit_jump_not_zero(compiler, jump - compiler->start);
                }break;
                case OP_DIV: {
                    
                }break;
            }
            
            return result;
        }break;
        
        case AST_IDENTIFIER: {
            int local = find_local(compiler, root->name);
            assert(local >= 0);
            return local;
        }break;
    }
    return -1;
}


internal void
compile_declaration(Ast_Node* root, Compiler* compiler){
    auto decl = root->decl;
    auto expr = decl.expr;
    int local = push_local(compiler, root->name, 0);
    
    int stack_ptr = compiler->stack_ptr;
    
    int init = compile_expression(expr, RA, compiler);
    copy_temporary(compiler, local, init);
    compiler->stack_ptr = stack_ptr;
}