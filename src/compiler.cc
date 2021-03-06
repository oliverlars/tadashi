

internal Scope*
make_compiler_scope(){
    auto scope = push_type_zero(&global_arena, Scope);
    return scope;
}

//push a named variable onto the stack
//these variables are ones written in code such as a = 10;
//they are the only ones that get referred to
internal int
push_scope_variable(Compiler* compiler, Token name){
    //fetch the current scope (anything between curly braces) and push it onto the scope's stack
    //its location in memory is where the stack pointer currently points to.
    compiler->current_scope->variables[compiler->current_scope->variable_count].name = name;
    compiler->current_scope->variables[compiler->current_scope->variable_count].address = compiler->stack_ptr;
    compiler->current_scope->variable_count++;
    return compiler->stack_ptr++;
}

//take a given instruction and output the assembly for it
// such as move RA, 10
//it will also print out the associated comment with it
internal void
dissassemble(Compiler* compiler) {
    
    char* reg[4] = {"RA", "RB", "RC", "RD"};
    auto instr = *compiler->at;
    int opcode = get_opcode(instr);
    printf("%d: ", (int)(compiler->at - compiler->start));
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
            printf("sr %s, 1", reg[x]);
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
//set the comment for the current instruction in the compiler
//for when it gets dissassembled
internal void
set_comment(Compiler* compiler, char* comment){
    compiler->has_comment = true;
    compiler->comment = comment;
}

//set the comment, but with a format specifier
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

//Below are all the emit instruction functions
//they all work similar, opcode in the most signifcant bits
// and registers after or operands/addresses
//the instruction set used is the one on http://www.simplecpudesign.com/

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
emit_and_register(Compiler* compiler, Register x, Register y){
    *compiler->at = OP_AND_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
    *compiler->at += (int)x << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH);
    *compiler->at += (int)y << (INSTRUCTION_LENGTH-OPCODE_LENGTH-REGISTER_LENGTH*2);
    dissassemble(compiler);
    compiler->at++;
}

internal void
emit_or_register(Compiler* compiler, Register x, Register y){
    *compiler->at = OP_OR_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
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
    *compiler->at = OP_SR0_REGISTER << (INSTRUCTION_LENGTH-OPCODE_LENGTH);
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

//to find a named variable, we first search the current scope, but if it's not found
//we go up to the parent scope and search that until either we find the  variable or we don't
//return true or false depending on whether we find it, and then pass the result into the last parameter
internal bool
find_local(Compiler* compiler, Token name, Variable* result){
    auto scope = compiler->current_scope;
    while(scope){
        for(int i = 0; i < scope->variable_count; i++){
            auto v = scope->variables[i];
            if(tokens_equal(name, v.name)){
                *result = v;
                return true;
            }
        }
        scope = scope->parent;
    }
    return false;
}


//Same as previous  find local except this takes a specific scope start
internal bool
find_local(Scope* scope, Token name, Variable* result){
    while(scope){
        for(int i = 0; i < scope->variable_count; i++){
            auto v = scope->variables[i];
            if(tokens_equal(name, v.name)){
                *result = v;
                return true;
            }
        }
        scope = scope->parent;
    }
    return false;
}

//temporaries are used in calculations as storage for variables that are not user created
//we move it to a register first, then store it into main memory at the current stack location
//a situation we may use this would be to duplicate a value to avoid changing the original variable
internal int
push_temporary(Compiler* compiler, int value){
    emit_move_absolute(compiler, RA, value);
    emit_store_absolute(compiler, RA,  compiler->stack_ptr);
    return compiler->stack_ptr++;
}

//if we have a variable in memory, we can copy the variable elsewhere
internal int
push_temporary_from_address(Compiler* compiler, int address){
    emit_load_absolute(compiler, RA, address);
    emit_store_absolute(compiler, RA,  compiler->stack_ptr);
    return compiler->stack_ptr++;
}

//if we have a value in a register, we can make a temporary from it
internal int
push_temporary_from_register(Compiler* compiler, Register r){
    emit_store_absolute(compiler, r,  compiler->stack_ptr);
    return compiler->stack_ptr++;
}

//copy a temporary into main memory
internal int
duplicate_temporary(Compiler* compiler, int address){
    emit_load_absolute(compiler, RA,  address); 
    return push_temporary_from_register(compiler, RA);
}

//currently not used, but it just decrements the stack pointer
internal int
pop_temporary(Compiler* compiler, Register r, int value){
    compiler->stack_ptr--;
}

//push local will store a value in a register, then push it to main memory and we also keep track of it
internal int
push_local(Compiler* compiler, Token name, int value){
    emit_move_absolute(compiler, RA, value);
    emit_store_absolute(compiler, RA,  compiler->stack_ptr);
    return push_scope_variable(compiler, name);
}

//push array to the current scope, stack pointer is incremented by the number of elements in the array + 1
//this is because for an array a = [10]; the identifier 'a' actually points to the start of the array which
//is the next contiguous memory location. Useful when passing the address to a function
internal int
push_array(Compiler* compiler, Token name, int size){
    int ptr = compiler->stack_ptr;
    set_commentf(compiler, "array set: %.*s", name.length, name.at);
    emit_move_absolute(compiler, RA, compiler->stack_ptr + 1);
    emit_store_absolute(compiler, RA,  compiler->stack_ptr++);
    
    compiler->current_scope->variables[compiler->current_scope->variable_count].name = name;
    compiler->current_scope->variables[compiler->current_scope->variable_count].address = ptr;
    compiler->current_scope->variables[compiler->current_scope->variable_count].is_array = true;
    compiler->current_scope->variables[compiler->current_scope->variable_count].array_length = size;
    compiler->current_scope->variable_count++;
    
    compiler->stack_ptr += size;
    
    return ptr;
}

//very similar as push_array except we initialise this array with the value of each character in the string
//for example: string = "potato";
//the ascii value for each character is copied into the arrays contents
internal int
push_string(Compiler* compiler, Token name, Token string){
    
    auto ptr = compiler->stack_ptr;
    set_commentf(compiler, "string set: %.*s", name.length, name.at);
    emit_move_absolute(compiler, RA, compiler->stack_ptr+1);
    emit_store_absolute(compiler, RA,  compiler->stack_ptr++);
    
    compiler->current_scope->variables[compiler->current_scope->variable_count].name = name;
    compiler->current_scope->variables[compiler->current_scope->variable_count].address = compiler->stack_ptr;
    compiler->current_scope->variables[compiler->current_scope->variable_count].is_string = true;
    compiler->current_scope->variables[compiler->current_scope->variable_count].string = string;
    compiler->current_scope->variables[compiler->current_scope->variable_count].array_length = string.length;
    compiler->current_scope->variable_count++;
    
    
    for(int i = 0; i < string.length; i++){
        emit_move_absolute(compiler, RA, string.at[i]);
        emit_store_absolute(compiler, RA,  compiler->stack_ptr++);
    }
    emit_move_absolute(compiler, RA, 0);
    emit_store_absolute(compiler, RA,  compiler->stack_ptr++);
    return ptr;
}

//adding to temporaries is not as easy as just emitting an add onto two registers
//without register allocation, adding two numbers involves fetching the temporary from main memory
//into a register
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
and_temporaries(Compiler* compiler, int address_x, int address_y){
    emit_load_absolute(compiler, RA,  address_x);
    emit_load_absolute(compiler, RB,  address_y);
    emit_and_register(compiler, RA, RB);
    emit_store_absolute(compiler, RA,  address_x);
}

internal void
or_temporaries(Compiler* compiler, int address_x, int address_y){
    emit_load_absolute(compiler, RA,  address_x);
    emit_load_absolute(compiler, RB,  address_y);
    emit_or_register(compiler, RA, RB);
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

//very similar to find_local but this works on functions. all functions are declared in global scope
// and functions can't be declared within functions
internal int
find_function(Compiler* compiler, Token name){
    int i = 0;
    for(; i < MAX_FUNCTION; i++){
        if(tokens_equal(name, compiler->functions[i].name)){
            return i;
        }
    }
    return -1;
}

internal int
find_function(Compiler* compiler, char* name){
    int i = 0;
    for(; i < MAX_FUNCTION; i++){
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
    emit_move_absolute(compiler, RD, (int)(compiler->at - compiler->start) + 5);
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
    
    emit_move_absolute(compiler, RD, (int)(compiler->at - compiler->start) + 5);
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

//this is one of the larger functions
//it takes an ast node and switches over the type of that node
// if it's a binary node, then it will have a left and right child which it will recursively call
// and further get evaluated until the final temporaries are returned which can then be operated on
//appropriately.
// the leaves of the tree will either be identifiers, function calls, array/string indices or number literals

internal int
compile_expression(Ast_Node* root, Register r, Compiler* compiler){
    assert(root);
    switch(root->type){
        case AST_FUNCTION_CALL: {
            
            int function_index = find_function(compiler, root->name);
            assert(function_index >= 0);
            auto f = compiler->functions[function_index];
            
            auto arg = root->call.arguments;
            auto param = f.params;
            auto scope = f.top_level_scope;
            int stack_ptr = f.stack_ptr;
            //copy the function arguments to the functions stack locations so it can be read by the function
            while(arg){
                Variable variable = {};
                bool found = find_local(scope, param->name, &variable);
                assert(found);
                //push_local_address(compiler, param->name, compile_expression(arg, RA, compiler));
                int expr = compile_expression(arg, RA, compiler);
                copy_temporary(compiler,  variable.address, expr);
                arg = arg->next;
                param = param->next;
            }
            
            emit_jump_function(compiler, root->name);
            return push_temporary_from_register(compiler, RA);
            
        }break;
        case AST_INDEX: {
            set_commentf(compiler, "index start: %.*s", root->name.length, root->name.at);
            Variable variable = {};
            int result = find_local(compiler, root->name, &variable);
            assert(result);
            assert(variable.is_array); //make sure they're valid
            auto offset = push_temporary(compiler, 0);
            auto offset_expr = compile_expression(root->index.offset, RA, compiler);  
            copy_temporary(compiler, offset, offset_expr); //we copy the offset (a[offset]) to avoid making changes to it
            add_temporaries(compiler, offset, variable.address);
            emit_load_absolute(compiler, RB, offset);
            emit_load_register(compiler, RA, RB);
            set_commentf(compiler, "index end: %.*s", root->name.length, root->name.at);
            return push_temporary_from_register(compiler, RA);
        }break;
        case AST_VALUE: {
            //just make a temporary from the number literal
            return push_temporary(compiler, root->value.number);
        }break;
        case AST_UNARY: {
            //here we take a number and either negate it or invert it
            //numbers are by default positive so having +6 will still be 6 so we leave it
            int right = compile_expression(root->unary.right, RA, compiler);
            int result = duplicate_temporary(compiler, right);
            switch(root->unary.op_type){
                case OP_SUB: {
                    auto zero = push_temporary(compiler, 0);
                    emit_move_absolute(compiler, RA, 0);
                    emit_load_absolute(compiler, RB, result);
                    emit_sub_register(compiler, RA, RB);
                    emit_store_absolute(compiler, RA, result);
                }break;
                case OP_ADD: {
                }break;
                case OP_NOT: {
                    add_temporary_absolute(compiler, right, 0);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_positive(compiler, (int)(compiler->at - compiler->start) +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
            }
            return result;
        }break;
        case AST_BINARY: {
            
            int a = compile_expression(root->binary.left, RA, compiler);
            int b = compile_expression(root->binary.right, RB, compiler);
            //we have now address for the left and right children of the binary node
            // for 6 + 7 the tree will look like
            //    +
            //   / \
//  6   7
            //recursively calling on the children will then evaluate and emit the code to fold the left and right of the tree
            
            int result = duplicate_temporary(compiler, a); //duplicate a and use as result to avoid tampering with original a
            
            //depending on the type, emit the necessary instructions
            switch(root->binary.op_type){
                case OP_ADD:{
                    add_temporaries(compiler, result, b);
                }break;
                case OP_SUB:{
                    sub_temporaries(compiler, result, b);
                }break;
                case OP_MUL:{
                    //there are no multiply instructions so we just use repeated addition
                    //very slow for large numbers
                    int count = duplicate_temporary(compiler, b);
                    emit_load_absolute(compiler, RB, b);
                    auto jump = (int)(compiler->at - compiler->start);
                    sub_temporary_absolute(compiler, count, 1);
                    auto temp_compiler = *compiler;
                    compiler->at++;
                    
                    add_temporaries(compiler, result, a);
                    
                    emit_jump_unconditional(compiler, jump);
                    auto end = (int)(compiler->at - compiler->start);
                    emit_jump_not_positive(&temp_compiler, end);
                }break;
                
                case OP_DIV: {
                    //as per the multiplys, there are no divides so similarly we just do repeated subtraction
                    result = push_temporary(compiler, 0);
                    auto count = duplicate_temporary(compiler, a);
                    auto jump = (int)(compiler->at - compiler->start); //temp compiler needed 
                    auto temp_compiler = *compiler;
                    compiler->at++;
                    add_temporary_absolute(compiler, result, 1);
                    sub_temporaries(compiler, count, b);
                    
                    emit_jump_unconditional(compiler, jump);
                    auto end = (int)(compiler->at - compiler->start);
                    emit_jump_not_positive(&temp_compiler, end);
                }break;
                
                
                //boolean operators will evaluate always to either 1 or 0
                case OP_LT:{
                    sub_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_not_negative(compiler, (int)(compiler->at - compiler->start) +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
                
                case OP_LTE:{
                    sub_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_positive(compiler, (int)(compiler->at - compiler->start) +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
                
                case OP_GT:{
                    sub_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_not_positive(compiler, (int)(compiler->at - compiler->start) +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
                
                case OP_GTE:{
                    sub_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_negative(compiler, (int)(compiler->at - compiler->start) +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
                
                case OP_EQ: {
                    sub_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_not_zero(compiler, (int)(compiler->at - compiler->start) +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
                
                case OP_NOT_EQ: {
                    sub_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_zero(compiler, (int)(compiler->at - compiler->start) +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
                
                case OP_LOGICAL_AND: {
                    and_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_zero(compiler, (int)(compiler->at - compiler->start) +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
                
                case OP_BITWISE_AND: {
                    and_temporaries(compiler, result, b);
                }break;
                
                case OP_LOGICAL_OR: {
                    or_temporaries(compiler, result, b);
                    emit_move_absolute(compiler, RA, 0);
                    emit_jump_zero(compiler, (int)(compiler->at - compiler->start) +2);
                    emit_move_absolute(compiler, RA, 1);
                    emit_store_absolute(compiler, RA, result);
                }break;
                
                case OP_BITWISE_OR: {
                    or_temporaries(compiler, result, b);
                }break;
                
                
                //thers only a shift left 1, so we repeat it
                case OP_SL:{
                    int count = duplicate_temporary(compiler, b);
                    auto jump = compiler->at;
                    sl_temporary(compiler, result);
                    sub_temporary_absolute(compiler, count, 1);
                    emit_jump_positive(compiler, (int)(jump - compiler->start));
                }break;
                
                case OP_SR:{
                    int count = duplicate_temporary(compiler, b);
                    auto jump = compiler->at;
                    sr_temporary(compiler, result);
                    sub_temporary_absolute(compiler, count, 1);
                    emit_jump_positive(compiler, (int)(jump - compiler->start));
                }break;
                
                case OP_XOR:{
                    xor_temporaries(compiler, result, b);
                }break;
                
            }
            
            return result;
        }break;
        
        case AST_IDENTIFIER: {
            //find local and return the address of it
            Variable local = {};
            int result = find_local(compiler, root->name, &local);
            set_commentf(compiler, "ref identifier: %.*s", root->name.length, root->name.at);
            if(!result){
                printf("variable \"%.*s\" is not defined in this scope", (int)root->name.length, root->name.at);
                exit(0);
            }
            if(local.is_array){
                return local.address;
            }else {
                return local.address;
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
compile_while(Ast_Node* root, Compiler* compiler);


//this compile scope is used for anything but the global scope, as this supports all executable code
//global scope is purely for function definitions
//scopes are a linked list of nodes so we traverse that as such
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
            case AST_IF: {
                compile_if(member, compiler);
            }break;
            case AST_WHILE: {
                compile_while(member, compiler);
            }break;
        }
        member = member->next;
    }
}

//make a new scope, scopes are also linked lists
internal void
push_scope(Compiler* compiler) {
    auto parent = compiler->current_scope;
    compiler->current_scope = make_compiler_scope();
    compiler->current_scope->parent = parent;
}

//pop the scope, as we are done with it and don't care about the variables there anymore as no one can refer to them
internal void
pop_scope(Compiler* compiler) {
    compiler->current_scope = compiler->current_scope->parent;
}

internal void
compile_for(Ast_Node* root, Compiler* compiler){
    
    push_scope(compiler); //make a new scope for inside the for loop
    compile_declaration(root->_for.decl, compiler); //this is the variable that the loop iterates over
    auto start = (int)(compiler->at - compiler->start); //this is the number it starts at 
    int cond = compile_expression(root->_for.cond, RA, compiler); //this is the condition that has to be met to iterate
    emit_add_absolute(compiler, RA, 0); //just a hack to make sure the cpu sets the flags for the previous operation
    auto temp_compiler = *compiler; //copy compiler at this position to insert a jump once we know which address to jump to
    compiler->at++;
    compile_scope(root->_for.body, compiler); //compile the for loop's scope
    compile_declaration(root->_for.stmt, compiler); //this is how how much to vary the variable each iteration
    emit_jump_unconditional(compiler, start); //keep jumping back to the start
    auto end = (int)(compiler->at - compiler->start); //insert jump at start incase condition is met, then jump to here
    emit_jump_zero(&temp_compiler, end);
    pop_scope(compiler); //we are done with scope
}

internal void
compile_if(Ast_Node* root, Compiler* compiler){
    push_scope(compiler); //make new scope for inside the if
    auto expr = compile_expression(root->_if.expr, RA, compiler); //expression to evaluate
    //anything non-zero is true
    emit_add_absolute(compiler, RA, 0);
    
    auto temp_compiler = *compiler; //we don't know where we will need to jump to yet
    compiler->at++;
    
    compile_scope(root->_if.body, compiler);
    if(root->_if._else){
        auto temp_compiler2 = *compiler; //we don't know where we will need to jump to yet
        compiler->at++;
        
        auto _else = (int)(compiler->at - compiler->start); //else condition for ifs 
        emit_jump_zero(&temp_compiler, _else);
        compile_scope(root->_if._else, compiler);
        auto end = (int)(compiler->at - compiler->start);
        emit_jump_unconditional(&temp_compiler2, end);
    }else {
        auto end = (int)(compiler->at - compiler->start);
        emit_jump_zero(&temp_compiler, end);
    }
    pop_scope(compiler);
}

internal void
compile_while(Ast_Node* root, Compiler* compiler) {
    push_scope(compiler);
    auto start = (int)(compiler->at - compiler->start);
    auto cond = compile_expression(root->_while.expr, RA, compiler);
    emit_add_absolute(compiler, RA, 0);
    
    auto temp_compiler = *compiler;
    compiler->at++;
    
    
    compile_scope(root->_while.body, compiler);
    emit_jump_unconditional(compiler, start);
    
    auto end = (int)(compiler->at - compiler->start);
    emit_jump_zero(&temp_compiler, end);
    pop_scope(compiler);
}


internal void
compile_declaration(Ast_Node* root, Compiler* compiler){
    Variable variable = {};
    bool result = find_local(compiler, root->name, &variable); //if it exists, reassign it
    if(result == false){ //time to make a new variable
        auto decl = root->decl; 
        auto expr = decl.expr; //initialisation expression e.g. a = 1*2 + 4;
        if(expr){
            int local = push_local(compiler, root->name, 0); //new local initialised to 0
            int init = compile_expression(expr, RA, compiler); //compile expression and store result in temporary "init"
            copy_temporary(compiler, local, init); //copy it to the new local
        }else {
            if(decl.is_array){
                push_array(compiler, root->name, decl.array_length);
            }else {
                push_string(compiler, root->name, decl.string);
            }
        }
    }else {
        auto decl = root->decl;
        
        if(variable.is_array || variable.is_string){
            set_commentf(compiler, "start of index assignment %.*s", root->name.length, root->name.at);
            auto offset = push_temporary(compiler, 0);
            auto offset_expr = compile_expression(decl.offset, RA, compiler);
            copy_temporary(compiler, offset, offset_expr);
            add_temporaries(compiler, offset, variable.address);
            
            auto init = compile_expression(decl.expr, RA, compiler);
            set_commentf(compiler, "end of index assignment %.*s", root->name.length, root->name.at);
            emit_load_absolute(compiler, RA, offset);
            emit_load_absolute(compiler, RB, init);
            emit_store_register(compiler, RB, RA);
            
        }else {
            auto expr = decl.expr;
            int init = compile_expression(expr, RA, compiler);
            copy_temporary(compiler, variable.address, init); //reassign to new expression
        }
    }
}

internal void
compile_function(Ast_Node* root, Compiler* compiler){
    set_commentf(compiler, "function start: %.*s", root->name.length, root->name.at);
    //push new function to be able to refer to it elsewhere
    auto function = &compiler->functions[compiler->function_count++];
    function->name = root->name;
    function->address = (int)(compiler->at - compiler->start);
    function->stack_ptr = compiler->stack_ptr;
    function->params = root->func.parameters;
    function->scope = make_compiler_scope();
    function->top_level_scope = function->scope;
    compiler->current_scope = function->scope;
    int variables = compiler->variable_count;
    
    
    int variable_count = compiler->variable_count;
    auto param = root->func.parameters;
    while(param){
        push_scope_variable(compiler, param->name); //push all parameters as new variables
        param = param->next;
    }
    compile_scope(root->func.body, compiler); //new scope for function body
    set_commentf(compiler, "function return: %.*s", root->name.length, root->name.at); //comment for return
    emit_sub_absolute(compiler, RC, 1);
    emit_load_register(compiler, RD, RC);
    emit_jump_register(compiler, RD);
    //store return value in RD
    compiler->variable_start = compiler->variable_count;
    function->scope = compiler->current_scope;
}