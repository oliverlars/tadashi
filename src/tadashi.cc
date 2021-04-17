#include <windows.h>
#include "tadashi.h"
#include "memory.h"
#include "lexer.h"
#include "parser.h"
#include "vm.h"
#include "compiler.h"

#include "memory.cc"
#include "lexer.cc"
#include "parser.cc"
#include "compiler.cc"



//Open source code from disc, 0 allocate array big enough to hold text
internal char*
open_source(char* filename){
    char* result = 0;
    FILE* file_pointer = fopen(filename, "r");
    
    if(file_pointer){
        fseek(file_pointer, 0, SEEK_END);
        size_t size = ftell(file_pointer);
        fseek(file_pointer, 0, SEEK_SET);
        
        result = (char*)calloc(size+2, sizeof(char));
        result[size] = 0;
        result[size-1] = '\n';
        fread(result, size, 1, file_pointer);
        fclose(file_pointer);
    }
    return result;
}


int main(int argc, char** args){
    
    bool should_print_ast = 0;
    char* source = nullptr;
    
    //read command line arguments
    for(int i = 1; i < argc; i++){
        if(strcmp(args[i], "-file") == 0){
            source = args[i+1];
            i++;
        }
        if(strcmp(args[i], "-print_ast") == 0){
            should_print_ast = 1;
        }
    }
    
    if(!source){ 
        printf("please enter a source file!");
        exit(0);
    }
    
    
    Lexer lexer = {};
    lexer.at = open_source(source);
    Parser p = {lexer};
    global_arena = make_arena(); //arena used for most compiler related allocations
    auto scope = parse_global_scope(&p); //global scope, it's a normal socpe node that only parses functions
    
    if(should_print_ast){
        FILE* file = fopen("ast.txt", "w");
        pretty_print(file, scope);
    }
    
    Compiler compiler = {};
    // TODO(Oliver): use arena
    compiler.at = push_size(&global_arena, MEMORY_SIZE, instruction); //allocate enough to store instructions an data (same as FPGA)
    compiler.start = compiler.at;
    compiler.comment_arena = make_arena(); //arena dedicated to just comments
    
    
    //compile all functions up front
    auto member = scope->scope.members;
    while(member){
        compile_function(member, &compiler);
        member = member->next;
    }
    
    VM vm = {};
    
    //find where the function is, we should start executing from here
    int f = find_function(&compiler, "entry");
    Function entry = compiler.functions[f];
    
    auto start = compiler.at - compiler.start;
    
    emit_move_absolute(&compiler, RC, 55000); //function address pointer
    
    emit_jump_function(&compiler, "entry"); //first thing we do is jump to this function
    
    vm.memory = compiler.start;
    vm.pc = start;
    
    
    //this is where the virtual machine runs
    //we stop executing when the program counter (pc) runs past the compiler
    while(vm.pc < (compiler.at - compiler.start)){
        
        instruction instr = vm.memory[vm.pc]; //fetch current instruction that the program counter points to
        
        int opcode = get_opcode(instr); //extract the first 6 bits (which are the opcode)
        
        bool jumped = 0;
        
        switch(opcode){ //which instruction are we looking at
            
            case OP_MOVE_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = operand;
            }break;
            
            case OP_ADD_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.registers[r] + operand;
                vm.zero = vm.registers[r] == 0;
                vm.positive = vm.registers[r] > 0;
                vm.negative = vm.registers[r] < 0;
            }break;
            
            case OP_SUB_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.registers[r] - operand;
                vm.zero = vm.registers[r] == 0;
                vm.positive = vm.registers[r] > 0;
                vm.negative = vm.registers[r] < 0;
            }break;
            
            case OP_LOAD_ABSOLUTE:{
                int r = get_register_x(instr);
                int address = get_address(instr);
                vm.registers[r] = vm.memory[address];
            }break;
            
            case OP_STORE_ABSOLUTE:{
                int r = get_register_x(instr);
                int address = get_operand(instr);
                vm.memory[address] = vm.registers[r];
            }break;
            
            case OP_AND_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.registers[r] & operand;
            }break;
            
            case OP_OR_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.registers[r] | operand;
            }break;
            
            case OP_XOR_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.registers[r] ^ operand;
            }break;
            
            case OP_MOVE_REGISTER:{
                int rx = get_register_x(instr);
                int ry = get_register_y(instr);
                vm.registers[rx] = vm.registers[ry];
            }break;
            
            case OP_ADD_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.registers[x] = vm.registers[x] + vm.registers[y];
                vm.zero = vm.registers[x] == 0;
                vm.positive = vm.registers[x] > 0;
                vm.negative = vm.registers[x] < 0;
            }break;
            
            case OP_SUB_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.registers[x] = vm.registers[x] - vm.registers[y];
                vm.zero = vm.registers[x] == 0;
                vm.positive = vm.registers[x] > 0;
                vm.negative = vm.registers[x] < 0;
            }break;
            
            case OP_LOAD_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.registers[x] = vm.memory[vm.registers[y]];
            }break;
            
            case OP_STORE_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.memory[vm.registers[y]] = vm.registers[x];
            }break;
            
            case OP_AND_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.registers[x] = vm.registers[x] & vm.registers[y];
                vm.zero = vm.registers[x] == 0;
                vm.positive = vm.registers[x] > 0;
                vm.negative = vm.registers[x] < 0;
            }break;
            
            case OP_OR_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.registers[x] = vm.registers[x] | vm.registers[y];
                vm.zero = vm.registers[x] == 0;
                vm.positive = vm.registers[x] > 0;
                vm.negative = vm.registers[x] < 0;
            }break;
            
            case OP_XOR_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.registers[x] = vm.registers[x] ^ vm.registers[y];
                vm.zero = vm.registers[x] == 0;
                vm.positive = vm.registers[x] > 0;
                vm.negative = vm.registers[x] < 0;
            }break;
            
            case OP_SR0_REGISTER:{
                int x = get_register_x(instr);
                vm.registers[x] = vm.registers[x] >> 1;
                vm.zero = vm.registers[x] == 0;
                vm.positive = vm.registers[x] > 0;
                vm.negative = vm.registers[x] < 0;
            }break;
            
            case OP_SL0_REGISTER:{
                int x = get_register_x(instr);
                vm.registers[x] = vm.registers[x] << 1;
                vm.zero = vm.registers[x] == 0;
                vm.positive = vm.registers[x] > 0;
                vm.negative = vm.registers[x] < 0;
            }break;
            
            case OP_ROR_REGISTER:{
                
            }break;
            
            case OP_ROL_REGISTER:{
            }break;
            
            case OP_JUMP_UNCONDITIONAL:{
                jumped = true;
                int address = get_address(instr);
                vm.pc = address;
            }break;
            
            case OP_JUMP_ZERO:{
                if(vm.zero){
                    jumped = true;
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_NOT_ZERO:{
                if(!vm.zero){
                    jumped = true;
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_CARRY:{
                if(vm.carry){
                    jumped = true;
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_NO_CARRY:{
                if(!vm.carry){
                    jumped = true;
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_NEGATIVE:{
                if(vm.negative){
                    jumped = true;
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_NOT_NEGATIVE:{
                if(!vm.negative){
                    jumped = true;
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_POSITIVE:{
                if(vm.positive){
                    jumped = true;
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_NOT_POSITIVE:{
                if(!vm.positive){
                    jumped = true;
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_OVERFLOW:{
                if(vm.overflow){
                    jumped = true;
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_NO_OVERFLOW:{
                if(!vm.overflow){
                    jumped = true;
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_REGISTER:{
                jumped = true;
                int y = get_register_y(instr);
                vm.pc = vm.registers[y];
            }break;
            
            case OP_CALL:{
                jumped = true;
                int address = get_call_address(instr);
                vm.RD = vm.pc;
                vm.pc = address;
            }break;
            
            case OP_RETURN:{
                vm.pc = vm.RD;
            }break;
            
        }
        if(!jumped) {
            vm.pc += 1;
        }
    }
    
    printf("\nregisters and their final state\n");
    printf("========================================\n");
    
    printf("RA: %d\n", vm.RA);
    printf("RB: %d\n", vm.RB);
    printf("RC: %d\n", vm.RC);
    printf("RD: %d\n", vm.RD);
    
    printf("\n");
    printf("entry() variables and their final state\n");
    printf("========================================\n");
    {
        auto scope = entry.scope;
        while(scope){
            for(int i = 0; i < scope->variable_count; i++){
                auto v = scope->variables[i];
                if(v.is_array && v.array_length < 64){
                    printf("$%d | %.*s: %d\n", v.address, v.name.length, v.name.at, vm.memory[v.address]);
                    for(int j = 1; j <= v.array_length; j++){
                        printf("$%d | %.*s[%d]: %d\n", v.address+j, v.name.length, v.name.at, j-1, vm.memory[v.address+j]);
                    }
                }else if(v.is_string){
                    printf("$%d | %.*s: \"", v.address, v.name.length, v.name.at);
                    for(int j = 0; j < v.array_length; j++){
                        printf("%c", vm.memory[v.address+j]);
                    }
                    printf("\"\n");
                }
                else{
                    printf("$%d | %.*s: %d\n", v.address, v.name.length, v.name.at, vm.memory[v.address]);
                }
            }
            scope = scope->parent;
        }
    }
    return 0;
}
