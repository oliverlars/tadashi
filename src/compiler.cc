

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
emit_load_register(Compiler* compiler, Register x, Register y){
    *compiler->at = OP_LOAD_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
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
emit_jump_positive(Compiler* compiler, int address){
    *compiler->at = OP_JUMP_POSITIVE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += address;
    compiler->at++;
}

internal void
emit_jump_register(Compiler* compiler, Register r){
    *compiler->at = OP_JUMP_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
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

internal int
push_array(Compiler* compiler, Token name, int size){
    int array = compiler->stack_ptr;
    for(int i = 0; i < size; i++){
        emit_move_absolute(compiler, RA, 0);
        emit_store_absolute(compiler, RA,  compiler->stack_ptr++);
    }
    compiler->variables[compiler->variable_count].name = name;
    compiler->variables[compiler->variable_count].address = array;
    compiler->variables[compiler->variable_count].is_array = true;
    compiler->variables[compiler->variable_count].array_length = size;
    compiler->variable_count++;
    return array;
}

internal void
push_local_address(Compiler* compiler, Token name, int address){
    emit_store_absolute(compiler, RA,  address);
    compiler->variables[compiler->variable_count++].name = name;
}

internal void
pop_local(Compiler* compiler, int size = 1){
    compiler->variable_count -= size;
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

internal void
emit_jump_function(Compiler* compiler, char* function){
    int function_index = find_function(compiler, function);
    assert(function_index >= 0);
    auto f = compiler->functions[function_index];
    emit_move_absolute(compiler, RD, compiler->at - compiler->start + 5);
    emit_store_register(compiler, RD, RC);
    emit_add_absolute(compiler, RC, 1);
    emit_move_absolute(compiler, RD, f.address);
    emit_jump_register(compiler, RD);
}

internal void
emit_jump_function(Compiler* compiler, Token function){
    int function_index = find_function(compiler, function);
    assert(function_index >= 0);
    auto f = compiler->functions[function_index];
    emit_move_absolute(compiler, RD, compiler->at - compiler->start + 5);
    emit_store_register(compiler, RD, RC);
    emit_add_absolute(compiler, RC, 1);
    emit_move_absolute(compiler, RD, f.address);
    emit_jump_register(compiler, RD);
}

internal void
emit_return_function(Compiler* compiler){
    emit_sub_absolute(compiler, RC, 1);
    emit_load_register(compiler, RD, RC);
    emit_jump_register(compiler, RD);
}

//returns stack positions for variables/temporaries
internal int
compile_expression(Ast_Node* root, Register r, Compiler* compiler){
    assert(root);
    switch(root->type){
        case AST_FUNCTION_CALL: {
            emit_jump_function(compiler, root->name);
            return push_temporary_from_register(compiler, RA);
            
        }break;
        case AST_INDEX: {
            int variable = find_local(compiler, root->name);
            auto offset = push_temporary(compiler, 0);
            auto offset_expr = compile_expression(root->index.offset, RA, compiler);
            copy_temporary(compiler, offset, offset_expr);
            add_temporary_absolute(compiler, offset, compiler->variables[variable].address);
            emit_load_absolute(compiler, RB, offset);
            emit_load_register(compiler, RA, RA);
            return push_temporary_from_register(compiler, RA);
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
                    int count = duplicate_temporary(compiler, b);
                    sub_temporary_absolute(compiler, count, 1);
                    auto jump = compiler->at;
                    
                    add_temporaries(compiler, result, a);
                    
                    sub_temporary_absolute(compiler, count, 1);
                    
                    emit_jump_positive(compiler, jump - compiler->start);
                    printf("mul end: %d\n", compiler->at - compiler->start);
                }break;
                case OP_LT:{
                    sub_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_positive(compiler, compiler->at - compiler->start +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
            }
            
            return result;
        }break;
        
        case AST_IDENTIFIER: {
            int local = find_local(compiler, root->name);
            if(local < 0){
                printf("variable \"%.*s\" is not defined in this scope", root->name.length, root->name.at);
                exit(0);
            }
            return compiler->variables[local].address;
        }break;
    }
    return -1;
}

internal void
compile_declaration(Ast_Node* root, Compiler* compiler);

internal void
compile_for(Ast_Node* root, Compiler* compiler);

internal void
compile_scope(Ast_Node* scope, Compiler* compiler){
    auto member = scope->scope.members;
    while(member){
        switch(member->type){
            case AST_DECLARATION:{
                compile_declaration(member, compiler);
            }break;
            case AST_RETURN:{
                int ret = compile_expression(member->ret.expr, RA, compiler);
                emit_load_absolute(compiler, RA, ret);
            }break;
            case AST_FOR: {
                compile_for(member, compiler);
            }break;
        }
        member = member->next;
    }
}

internal void
compile_for(Ast_Node* root, Compiler* compiler){
    int min = compile_expression(root->_for.min, RA, compiler);
    int max = compile_expression(root->_for.max, RA, compiler);
    sub_temporaries(compiler, max, min);
    int count = max; //result of sub_temporaries is stored in min
    int jump_location = compiler->at - compiler->start;
    compile_scope(root->_for.body, compiler);
    sub_temporary_absolute(compiler, count, 1);
    emit_jump_not_zero(compiler, jump_location);
}


internal void
compile_declaration(Ast_Node* root, Compiler* compiler){
    int variable = find_local(compiler, root->name);
    if(variable < 0){
        auto decl = root->decl;
        auto expr = decl.expr;
        if(expr){
            int local = push_local(compiler, root->name, 0);
            int init = compile_expression(expr, RA, compiler);
            copy_temporary(compiler, local, init);
        }else {
            push_array(compiler, root->name, decl.array_length);
        }
    }else {
        auto decl = root->decl;
        
        if(compiler->variables[variable].is_array){
            auto offset = push_temporary(compiler, 0);
            auto offset_expr = compile_expression(decl.offset, RA, compiler);
            copy_temporary(compiler, offset, offset_expr);
            add_temporary_absolute(compiler, offset, compiler->variables[variable].address);
            auto init = compile_expression(decl.expr, RA, compiler);
            emit_load_absolute(compiler, RA, offset);
            emit_load_absolute(compiler, RB, init);
            emit_store_register(compiler, RB, RA);
        }else {
            auto expr = decl.expr;
            int init = compile_expression(expr, RA, compiler);
            copy_temporary(compiler, compiler->variables[variable].address, init);
        }
    }
}

internal void
compile_function(Ast_Node* root, Compiler* compiler){
    auto function = &compiler->functions[compiler->function_count++];
    function->name = root->name;
    function->address = compiler->at - compiler->start;
    int variables = compiler->variable_count;
    compile_scope(root->func.body, compiler);
    emit_return_function(compiler);
    if(!token_equals_string(root->name, "entry")){
        compiler->variable_count = variables;
    }
}