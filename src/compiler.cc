

internal void
dissassemble(Compiler* compiler) {
    
    char* reg[4] = {"RA", "RB", "RC", "RD"};
    auto instr = *compiler->at;
    int opcode = get_opcode(instr);
    printf("%d: ", compiler->at - compiler->start);
    switch(opcode){
        
        case OP_MOVE_ABSOLUTE:{
            int r = get_register_x(instr);
            int operand = get_operand(instr);
            printf("move %s, %d", reg[r], operand);
        }break;
        
        case OP_ADD_ABSOLUTE:{
            int r = get_register_x(instr);
            int operand = get_operand(instr);
            printf("add %s, %d", reg[r], operand);
        }break;
        
        case OP_SUB_ABSOLUTE:{
            int r = get_register_x(instr);
            int operand = get_operand(instr);
            printf("sub %s, %d", reg[r], operand);
        }break;
        
        case OP_LOAD_ABSOLUTE:{
            int r = get_register_x(instr);
            int address = get_operand(instr);
            printf("load %s, $%d", reg[r], address);
        }break;
        
        case OP_STORE_ABSOLUTE:{
            int r = get_register_x(instr);
            int address = get_operand(instr);
            printf("store %s, $%d", reg[r], address);
        }break;
        
        case OP_AND_ABSOLUTE:{
            int r = get_register_x(instr);
            int operand = get_operand(instr);
            printf("and %s, %d", reg[r], operand);
        }break;
        
        case OP_OR_ABSOLUTE:{
            int r = get_register_x(instr);
            int operand = get_operand(instr);
            printf("or %s, %d", reg[r], operand);
        }break;
        
        case OP_XOR_ABSOLUTE:{
            int r = get_register_x(instr);
            int operand = get_operand(instr);
            printf("xor %s, %d", reg[r], operand);
        }break;
        
        case OP_MOVE_REGISTER:{
            int x = get_register_x(instr);
            int y = get_operand(instr);
            printf("move %s, %s", reg[x], reg[y]);
        }break;
        
        case OP_ADD_REGISTER:{
            int x = get_register_x(instr);
            int y = get_register_y(instr);
            printf("add %s, %s", reg[x], reg[y]);
        }break;
        
        case OP_SUB_REGISTER:{
            int x = get_register_x(instr);
            int y = get_register_y(instr);
            printf("sub %s, %s", reg[x], reg[y]);
        }break;
        
        case OP_LOAD_REGISTER:{
            int x = get_register_x(instr);
            int y = get_register_y(instr);
            printf("load %s, %s", reg[x], reg[y]);
        }break;
        
        case OP_STORE_REGISTER:{
            int x = get_register_x(instr);
            int y = get_register_y(instr);
            printf("store %s, %s", reg[x], reg[y]);
        }break;
        
        case OP_AND_REGISTER:{
            int x = get_register_x(instr);
            int y = get_register_y(instr);
            printf("and %s, %s", reg[x], reg[y]);
        }break;
        
        case OP_OR_REGISTER:{
            int x = get_register_x(instr);
            int y = get_register_y(instr);
            printf("or %s, %s", reg[x], reg[y]);
        }break;
        
        case OP_XOR_REGISTER:{
            int x = get_register_x(instr);
            int y = get_register_y(instr);
            printf("xor %s, %s", reg[x], reg[y]);
        }break;
        
        case OP_SR0_REGISTER:{
            int x = get_register_x(instr);
            printf("sr %s, %s", reg[x]);
        }break;
        
        case OP_SL0_REGISTER:{
            int x = get_register_x(instr);
            printf("sl %s, 1", reg[x]);
        }break;
        
        case OP_ROR_REGISTER:{
            
        }break;
        
        case OP_ROL_REGISTER:{
        }break;
        
        case OP_JUMP_UNCONDITIONAL:{
            int address = get_address(instr);
            printf("jumpu %d", address);
        }break;
        
        case OP_JUMP_ZERO:{
            int address = get_address(instr);
            printf("jumpz %d", address);
        }break;
        
        case OP_JUMP_NOT_ZERO:{
            int address = get_address(instr);
            printf("jumpnz %d", address);
        }break;
        
        case OP_JUMP_CARRY:{
            int address = get_address(instr);
            printf("jumpc %d", address);
        }break;
        
        case OP_JUMP_NO_CARRY:{
            int address = get_address(instr);
            printf("jumpnc %d", address);
        }break;
        
        case OP_JUMP_NEGATIVE:{
            int address = get_address(instr);
            printf("jumpn %d", address);
        }break;
        
        case OP_JUMP_NOT_NEGATIVE:{
            int address = get_address(instr);
            printf("jumpnn %d", address);
        }break;
        
        case OP_JUMP_POSITIVE:{
            int address = get_address(instr);
            printf("jumpp %d", address);
        }break;
        
        case OP_JUMP_NOT_POSITIVE:{
            int address = get_address(instr);
            printf("jumpnp %d", address);
        }break;
        
        case OP_JUMP_OVERFLOW:{
            int address = get_address(instr);
            printf("jumpo %d", address);
        }break;
        
        case OP_JUMP_NO_OVERFLOW:{
            int address = get_address(instr);
            printf("jumpno %d", address);
        }break;
        
        case OP_JUMP_REGISTER:{
            int y = get_register_y(instr);
            printf("jumpr %s", reg[y]);
        }break;
        
        case OP_CALL:{
            int address = get_call_address(instr);
            printf("call %d", address);
        }break;
        
        case OP_RETURN:{
            printf("ret");
        }break;
    }
    if(compiler->has_comment){
        printf(" ; %s\n", compiler->comment);
        compiler->has_comment = false;
    }else {
        printf("\n");
    }
}

internal void
set_comment(Compiler* compiler, char* comment){
    compiler->has_comment = true;
    compiler->comment = comment;
}

internal void
set_commentf(Compiler* compiler, char* fmt, ...){
    
    char* buffer = push_size(&compiler->comment_arena, 256, char);
    va_list args;
    va_start (args, fmt);
    int length = vsnprintf (buffer,256,fmt, args);
    buffer[length] = 0;
    va_end (args);
    
    compiler->has_comment = true;
    compiler->comment = buffer;
}

internal void
emit_move_absolute(Compiler* compiler, Register r, int value){
    *compiler->at = OP_MOVE_ABSOLUTE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += value;
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_store_absolute(Compiler* compiler, Register r, int address){
    *compiler->at = OP_STORE_ABSOLUTE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += address;
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_load_absolute(Compiler* compiler, Register r, int address){
    *compiler->at = OP_LOAD_ABSOLUTE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += address;
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_add_absolute(Compiler* compiler, Register r, int value){
    *compiler->at = OP_ADD_ABSOLUTE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += value;
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_sub_absolute(Compiler* compiler, Register r, int value){
    *compiler->at = OP_SUB_ABSOLUTE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += value;
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_move_register(Compiler* compiler, Register x, Register y){
    *compiler->at = OP_MOVE_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += (int)y << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_store_register(Compiler* compiler, Register x, Register y){
    *compiler->at = OP_STORE_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += (int)y << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_load_register(Compiler* compiler, Register x, Register y){
    *compiler->at = OP_LOAD_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += (int)y << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_add_register(Compiler* compiler, Register x, Register y){
    *compiler->at = OP_ADD_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += (int)y << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_sl_register(Compiler* compiler, Register x){
    *compiler->at = OP_SL0_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_sr_register(Compiler* compiler, Register x){
    *compiler->at = OP_SL0_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_sub_register(Compiler* compiler, Register x, Register y){
    *compiler->at = OP_SUB_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += (int)y << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_xor_register(Compiler* compiler, Register x, Register y){
    *compiler->at = OP_XOR_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += (int)y << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_jump_not_zero(Compiler* compiler, int address){
    *compiler->at = OP_JUMP_NOT_ZERO << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += address;
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_jump_zero(Compiler* compiler, int address){
    *compiler->at = OP_JUMP_ZERO << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += address;
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_jump_positive(Compiler* compiler, int address){
    *compiler->at = OP_JUMP_POSITIVE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += address;
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_jump_negative(Compiler* compiler, int address){
    *compiler->at = OP_JUMP_NEGATIVE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += address;
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_jump_not_positive(Compiler* compiler, int address){
    *compiler->at = OP_JUMP_NOT_POSITIVE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += address;
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_jump_not_negative(Compiler* compiler, int address){
    *compiler->at = OP_JUMP_NOT_NEGATIVE << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += address;
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_jump_register(Compiler* compiler, Register r){
    *compiler->at = OP_JUMP_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)r << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_jump_unconditional(Compiler* compiler, int address){
    *compiler->at = OP_JUMP_UNCONDITIONAL << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += address;
    dissassemble(compiler);
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
        auto v = compiler->variables[result];
        if(tokens_equal(name, v.name)){
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
    auto ptr = compiler->stack_ptr;
    set_commentf(compiler, "array set: %.*s", name.length, name.at);
    emit_move_absolute(compiler, RA, compiler->stack_ptr+1);
    emit_store_absolute(compiler, RA,  compiler->stack_ptr++);
    compiler->variables[compiler->variable_count].name = name;
    compiler->variables[compiler->variable_count].address = ptr;
    compiler->variables[compiler->variable_count].is_array = true;
    compiler->variables[compiler->variable_count].array_length = size;
    compiler->variable_count++;
    
    for(int i = 0; i < size; i++){
        emit_move_absolute(compiler, RA, 0);
        emit_store_absolute(compiler, RA,  compiler->stack_ptr++);
    }
    
    return ptr;
}

internal int
push_string(Compiler* compiler, Token name, Token string){
    int array = compiler->stack_ptr;
    for(int i = 0; i < string.length; i++){
        emit_move_absolute(compiler, RA, string.at[i]);
        emit_store_absolute(compiler, RA,  compiler->stack_ptr++);
    }
    compiler->variables[compiler->variable_count].name = name;
    compiler->variables[compiler->variable_count].string = string;
    compiler->variables[compiler->variable_count].address = array;
    compiler->variables[compiler->variable_count].is_string = true;
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
sl_temporary(Compiler* compiler, int address_x){
    emit_load_absolute(compiler, RA,  address_x);
    emit_sl_register(compiler, RA);
    emit_store_absolute(compiler, RA,  address_x);
}

internal void
sr_temporary(Compiler* compiler, int address_x){
    emit_load_absolute(compiler, RA,  address_x);
    emit_sr_register(compiler, RA);
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
xor_temporaries(Compiler* compiler, int address_x, int address_y){
    emit_load_absolute(compiler, RA,  address_x);
    emit_load_absolute(compiler, RB,  address_y);
    emit_xor_register(compiler, RA, RB);
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
            
            int function_index = find_function(compiler, root->name);
            assert(function_index >= 0);
            auto f = compiler->functions[function_index];
            
            auto arg = root->call.arguments;
            int stack_ptr = f.stack_ptr;
            while(arg){
                emit_load_absolute(compiler, RA, compile_expression(arg, RA, compiler));
                emit_store_absolute(compiler, RA, stack_ptr++);
                arg = arg->next;
            }
            
            emit_jump_function(compiler, root->name);
            return push_temporary_from_register(compiler, RA);
            
        }break;
        case AST_INDEX: {
            set_commentf(compiler, "index start: %.*s", root->name.length, root->name.at);
            int variable = find_local(compiler, root->name);
            assert(variable >= 0);
            auto offset = push_temporary(compiler, 0);
            auto offset_expr = compile_expression(root->index.offset, RA, compiler);
            copy_temporary(compiler, offset, offset_expr);
            add_temporaries(compiler, offset, compiler->variables[variable].address);
            emit_load_absolute(compiler, RB, offset);
            emit_add_absolute(compiler, RB, 0);
            emit_load_register(compiler, RA, RB);
            set_commentf(compiler, "index end: %.*s", root->name.length, root->name.at);
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
                    emit_load_absolute(compiler, RB, b);
                    auto jump = compiler->at - compiler->start;
                    sub_temporary_absolute(compiler, count, 1);
                    auto temp_compiler = *compiler;
                    compiler->at++;
                    
                    add_temporaries(compiler, result, a);
                    
                    emit_jump_unconditional(compiler, jump);
                    auto end = compiler->at - compiler->start;
                    emit_jump_not_positive(&temp_compiler, end);
                }break;
                
                case OP_LT:{
                    sub_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_not_negative(compiler, compiler->at - compiler->start +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
                
                case OP_LTE:{
                    sub_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_positive(compiler, compiler->at - compiler->start +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
                
                case OP_GT:{
                    sub_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_not_positive(compiler, compiler->at - compiler->start +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
                
                case OP_GTE:{
                    sub_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_negative(compiler, compiler->at - compiler->start +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
                
                case OP_LOGICAL_AND: {
                    add_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                }break;
                
                case OP_SL:{
                    
                    
                    int count = duplicate_temporary(compiler, b);
                    auto jump = compiler->at;
                    
                    sl_temporary(compiler, result);
                    
                    sub_temporary_absolute(compiler, count, 1);
                    
                    emit_jump_positive(compiler, jump - compiler->start);
                    
                }break;
                
                case OP_SR:{
                    
                    int count = duplicate_temporary(compiler, b);
                    auto jump = compiler->at;
                    
                    sr_temporary(compiler, result);
                    
                    sub_temporary_absolute(compiler, count, 1);
                    
                    emit_jump_positive(compiler, jump - compiler->start);
                }break;
                
                case OP_XOR:{
                    xor_temporaries(compiler, result, b);
                }break;
                
            }
            
            return result;
        }break;
        
        case AST_IDENTIFIER: {
            int local = find_local(compiler, root->name);
            set_commentf(compiler, "ref identifier: %.*s", root->name.length, root->name.at);
            if(local < 0){
                printf("variable \"%.*s\" is not defined in this scope", root->name.length, root->name.at);
                exit(0);
            }
            if(compiler->variables[local].is_array){
                return compiler->variables[local].address;
            }else {
                return compiler->variables[local].address;
            }
        }break;
    }
    return -1;
}

internal void
compile_declaration(Ast_Node* root, Compiler* compiler);

internal void
compile_for(Ast_Node* root, Compiler* compiler);

internal void
compile_if(Ast_Node* root, Compiler* compiler);


internal void
compile_scope_keep_variables(Ast_Node* scope, Compiler* compiler){
    
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
            case AST_IF: {
                compile_if(member, compiler);
            }break;
        }
        member = member->next;
    }
}

internal void
compile_scope(Ast_Node* scope, Compiler* compiler){
    int variable_count = compiler->variable_count;
    compile_scope_keep_variables(scope, compiler);
    compiler->variable_count = variable_count;
}

internal void
compile_for(Ast_Node* root, Compiler* compiler){
    compile_declaration(root->_for.decl, compiler);
    auto start = compiler->at - compiler->start;
    int cond = compile_expression(root->_for.cond, RA, compiler);
    emit_add_absolute(compiler, RA, 0);
    auto temp_compiler = *compiler;
    compiler->at++;
    compile_scope(root->_for.body, compiler);
    compile_declaration(root->_for.stmt, compiler);
    emit_jump_unconditional(compiler, start);
    auto end = compiler->at - compiler->start;
    emit_jump_zero(&temp_compiler, end);
}

internal void
compile_if(Ast_Node* root, Compiler* compiler){
    auto expr = compile_expression(root->_if.expr, RA, compiler);
    emit_add_absolute(compiler, RA, 0);
    
    auto temp_compiler = *compiler; //we don't know where we will need to jump to yet
    compiler->at++;
    
    compile_scope(root->_if.body, compiler);
    if(root->_if._else){
        auto temp_compiler2 = *compiler; //we don't know where we will need to jump to yet
        compiler->at++;
        
        auto _else = compiler->at - compiler->start;
        emit_jump_zero(&temp_compiler, _else);
        compile_scope(root->_if._else, compiler);
        auto end = compiler->at - compiler->start;
        emit_jump_unconditional(&temp_compiler2, end);
    }else {
        auto end = compiler->at - compiler->start;
        emit_jump_zero(&temp_compiler, end);
    }
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
            if(decl.is_array){
                push_array(compiler, root->name, decl.array_length);
            }else{
                push_string(compiler, root->name, decl.string);
            }
        }
    }else {
        auto decl = root->decl;
        
        if(compiler->variables[variable].is_array || compiler->variables[variable].is_string){
            
            auto offset = push_temporary(compiler, 0);
            auto offset_expr = compile_expression(decl.offset, RA, compiler);
            copy_temporary(compiler, offset, offset_expr);
            add_temporary_absolute(compiler, offset, compiler->variables[variable].address);
            auto init = compile_expression(decl.expr, RA, compiler);
            emit_load_absolute(compiler, RA, offset);
            emit_add_absolute(compiler, RA, 1);
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
    
    set_commentf(compiler, "function start: %.*s", root->name.length, root->name.at);
    
    auto function = &compiler->functions[compiler->function_count++];
    function->name = root->name;
    function->address = compiler->at - compiler->start;
    function->stack_ptr = compiler->stack_ptr;
    int variables = compiler->variable_count;
    
    auto param = root->func.parameters;
    while(param){
        compiler->variables[compiler->variable_count].name = param->name;
        compiler->variables[compiler->variable_count].address = compiler->stack_ptr;
        compiler->variable_count++;
        compiler->stack_ptr++;
        param = param->next;
    }
    
    if(token_equals_string(root->name, "entry")){
        compile_scope_keep_variables(root->func.body, compiler);
    }else {
        compile_scope(root->func.body, compiler);
    }
    set_commentf(compiler, "function return: %.*s", root->name.length, root->name.at);
    emit_sub_absolute(compiler, RC, 1);
    emit_load_register(compiler, RD, RC);
    emit_jump_register(compiler, RD);
    
}