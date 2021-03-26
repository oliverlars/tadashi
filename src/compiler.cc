

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

internal void
emit_ret(Compiler* compiler){
    *compiler->at = OP_RETURN << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    compiler->at++;
}

internal void
emit_call(Compiler* compiler, int address){
    *compiler->at = OP_CALL << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
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
    emit_store_absolute(compiler, RA,  compiler->stack_ptr);
    return compiler->stack_ptr++;
}

internal int
push_temporary_from_register(Compiler* compiler, Register r){
    emit_store_absolute(compiler, r,  compiler->stack_ptr);
    return compiler->stack_ptr++;
}

internal int
duplicate_temporary(Compiler* compiler, int address){
    emit_load_absolute(compiler, RA,  address); 
    return push_temporary_from_register(compiler, RA);
}

internal int
pop_temporary(Compiler* compiler, Register r, int value){
    compiler->stack_ptr--;
}

internal int
push_local(Compiler* compiler, Token name, int value){
    emit_move_absolute(compiler, RA, value);
    emit_store_absolute(compiler, RA,  compiler->stack_ptr);
    compiler->variables[compiler->variable_count].name = name;
    compiler->variables[compiler->variable_count].address = compiler->stack_ptr;
    compiler->variable_count++;
    return compiler->stack_ptr++;
}

internal void
push_local_address(Compiler* compiler, Token name, int address){
    emit_store_absolute(compiler, RA,  address);
    compiler->variables[compiler->variable_count++].name = name;
}

internal void
add_temporary_absolute(Compiler* compiler, int address, int value){
    emit_load_absolute(compiler, RA,  address);
    emit_add_absolute(compiler, RA, value);
    emit_store_absolute(compiler, RA,  address);
}

internal void
sub_temporary_absolute(Compiler* compiler, int address, int value){
    emit_load_absolute(compiler, RA,  address);
    emit_sub_absolute(compiler, RA, value);
    emit_store_absolute(compiler, RA,  address);
}

internal void
add_temporaries(Compiler* compiler, int address_x, int address_y){
    emit_load_absolute(compiler, RA,  address_x);
    emit_load_absolute(compiler, RB,  address_y);
    emit_add_register(compiler, RA, RB);
    emit_store_absolute(compiler, RA,  address_x);
}

internal void
sub_temporaries(Compiler* compiler, int address_x, int address_y){
    emit_load_absolute(compiler, RA,  address_x);
    emit_load_absolute(compiler, RB,  address_y);
    emit_sub_register(compiler, RA, RB);
    emit_store_absolute(compiler, RA,  address_x);
}

internal void
copy_temporary(Compiler* compiler, int address_x, int address_y){
    emit_load_absolute(compiler, RA, address_y);
    emit_store_absolute(compiler, RA, address_x);
}

internal int
find_function(Compiler* compiler, Token name){
    int i = 0;
    for(; i < 256; i++){
        if(tokens_equal(name, compiler->functions[i].name)){
            return i;
        }
    }
    return -1;
}

internal int
find_function(Compiler* compiler, char* name){
    int i = 0;
    for(; i < 256; i++){
        if(token_equals_string(compiler->functions[i].name, name)){
            return i;
        }
    }
    return -1;
}

//returns stack positions for variables/temporaries
internal int
compile_expression(Ast_Node* root, Register r, Compiler* compiler){
    assert(root);
    switch(root->type){
        case AST_FUNCTION_CALL: {
            emit_call(compiler, find_function(compiler, root->name));
            
        }break;
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
                    auto jump = compiler->at;
                    
                    add_temporaries(compiler, result, a);
                    
                    sub_temporary_absolute(compiler, b, 1);
                    
                    emit_jump_not_zero(compiler, jump - compiler->start);
                }break;
                
            }
            
            return result;
        }break;
        
        case AST_IDENTIFIER: {
            int local = find_local(compiler, root->name);
            assert(local >= 0);
            printf("line number for load a %d\n", compiler->at - compiler->start);
            return compiler->variables[local].address;
        }break;
    }
    return -1;
}


internal void
compile_declaration(Ast_Node* root, Compiler* compiler){
    int variable = find_local(compiler, root->name);
    if(variable < 0){
        auto decl = root->decl;
        auto expr = decl.expr;
        int local = push_local(compiler, root->name, 0);
        int init = compile_expression(expr, RA, compiler);
        copy_temporary(compiler, local, init);
    }else {
        auto decl = root->decl;
        auto expr = decl.expr;
        int init = compile_expression(expr, RA, compiler);
        copy_temporary(compiler, compiler->variables[variable].address, init);
    }
}

internal void
compile_function(Ast_Node* root, Compiler* compiler){
    auto function = &compiler->functions[compiler->function_count++];
    function->name = root->name;
    function->address = compiler->at - compiler->start;
    
    auto scope = root->func.body;
    auto member = scope->scope.members;
    while(member){
        switch(member->type){
            case AST_DECLARATION:{
                compile_declaration(member, compiler);
            }break;
            case AST_RETURN:{
                compile_expression(member->ret.expr, RA, compiler);
            }break;
        }
        member = member->next;
    }
    emit_ret(compiler);
}