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

/*
RA
RB
RC stack pointer
RD return address

declarations work by writing to stack pointer location in memory

accessing variables just takes stack pointer - things pushed on
*/

int main(){
    Lexer lexer = {};
    lexer.at = open_source("test.td");
    Parser p = {lexer};
    global_node_arena = make_arena(sizeof(Ast_Node)*1024);
    auto scope = parse_scope(&p);
    
    Byte_Code bc = {};
    bc.at = (instruction*)malloc(sizeof(instruction)*MEMORY_SIZE);
    bc.start = bc.at;
    
    compile_declaration(scope->scope.members, &bc);
    compile_declaration(scope->scope.members->next, &bc);
    
    VM vm = {};
    vm.RC = 256; //stack pointer
    
    vm.memory = bc.start;
    
    while(vm.pc < 6){
        
        instruction instr = vm.memory[vm.pc];
        
        int opcode = get_opcode(instr);
        
        switch(opcode){
            
            case OP_MOVE_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = operand;
            }break;
            
            case OP_ADD_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.registers[r] + operand;
            }break;
            
            case OP_SUB_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.registers[r] - operand;
            }break;
            
            case OP_LOAD_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.memory[operand];
            }break;
            
            case OP_STORE_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.memory[r] = vm.registers[r];
            }break;
            
            case OP_AND_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.registers[r] + operand;
            }break;
            
            case OP_OR_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.registers[r] + operand;
            }break;
            
            case OP_XOR_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.registers[r] + operand;
            }break;
            
            case OP_MOVE_REGISTER:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.registers[r] + operand;
            }break;
            
            case OP_ADD_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.registers[x] = vm.registers[x] + vm.registers[y];
            }break;
            
            case OP_SUB_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.registers[x] = vm.registers[x] - vm.registers[y];
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
            }break;
            
            case OP_OR_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.registers[x] = vm.registers[x] | vm.registers[y];
            }break;
            
            case OP_XOR_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.registers[x] = vm.registers[x] ^ vm.registers[y];
            }break;
            
            case OP_SR0_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.registers[x] = vm.registers[x] >> vm.registers[y];
            }break;
            
            case OP_SL0_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.registers[x] = vm.registers[x] << vm.registers[y];
            }break;
            
            case OP_ROR_REGISTER:{
                
            }break;
            
            case OP_ROL_REGISTER:{
            }break;
            
            case OP_JUMP_UNCONDITIONAL:{
                int address = get_address(instr);
                vm.pc = address;
            }break;
            
            case OP_JUMP_ZERO:{
                if(vm.zero){
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_NOT_ZERO:{
                if(!vm.zero){
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_CARRY:{
                if(vm.carry){
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_NO_CARRY:{
                if(!vm.carry){
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_NEGATIVE:{
                if(vm.negative){
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_NOT_NEGATIVE:{
                if(!vm.negative){
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_POSITIVE:{
                if(vm.positive){
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_NOT_POSITIVE:{
                if(!vm.positive){
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_OVERFLOW:{
                if(vm.overflow){
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_NO_OVERFLOW:{
                if(!vm.overflow){
                    int address = get_address(instr);
                    vm.pc = address;
                }
            }break;
            
            case OP_JUMP_REGISTER:{
                int y = get_address(instr);
                vm.pc = vm.registers[y];
            }break;
            
            case OP_CALL:{
                int address = get_call_address(instr);
                vm.RD = vm.pc;
                vm.pc = address;
            }break;
            
            case OP_RETURN:{
                vm.pc = vm.RD;
            }break;
            
        }
        
        vm.pc++;
    }
    printf("RA: %d\n", vm.RA);
    printf("RB: %d\n", vm.RB);
    printf("RC: %d\n", vm.RC);
    printf("RD: %d\n", vm.RD);
    
    printf("stack: %d\n", vm.memory[256]);
    printf("stack: %d\n", vm.memory[257]);
    
    return 0;
}