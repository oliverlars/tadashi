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

accessing variables just takes stack pointer - location on stack


remove real stack pointer and just use direct addressing but manage stack
in compiler


register allocation: just make all variables dirty and slap a bunch of 
load/stores on everything
*/


void dissassemble(instruction* start, instruction* end);

int main(){
    Lexer lexer = {};
    lexer.at = open_source("test.td");
    Parser p = {lexer};
    global_node_arena = make_arena(sizeof(Ast_Node)*1024);
    auto scope = parse_scope(&p);
    
    Compiler bc = {};
    // TODO(Oliver): use arena
    bc.at = (instruction*)malloc(sizeof(instruction)*MEMORY_SIZE);
    bc.start = bc.at;
    bc.stack_ptr = 0;
    
    compile_declaration(scope->scope.members, &bc);
    compile_declaration(scope->scope.members->next, &bc);
    compile_declaration(scope->scope.members->next->next, &bc);
    
    VM vm = {};
    
    vm.memory = bc.start;
    
    dissassemble(bc.start, bc.at);
    
    while(vm.pc < (bc.at - bc.start)){
        
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
                vm.zero = vm.registers[r] == 0;
            }break;
            
            case OP_SUB_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.registers[r] - operand;
                vm.zero = vm.registers[r] == 0;
            }break;
            
            case OP_LOAD_ABSOLUTE:{
                int r = get_register_x(instr);
                int operand = get_operand(instr);
                vm.registers[r] = vm.memory[operand];
            }break;
            
            case OP_STORE_ABSOLUTE:{
                int r = get_register_x(instr);
                int address = get_operand(instr);
                vm.memory[address] = vm.registers[r];
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
                vm.zero = vm.registers[x] == 0;
            }break;
            
            case OP_SUB_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                vm.registers[x] = vm.registers[x] - vm.registers[y];
                vm.zero = vm.registers[x] == 0;
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
    
    for(int i = 0; i < 20; i++){
        printf("stack: %d\n", vm.memory[256+i]);
    }
    
    return 0;
}

internal void
dissassemble(instruction* start, instruction* end) {
    
    char* reg[4] = {"RA", "RB", "RC", "RD"};
    int address = 0;
    while(start != end){
        auto instr = *start;
        int opcode = get_opcode(instr);
        printf("%d: ", address++);
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
                int y = get_register_y(instr);
                printf("sr0 %s, %s", reg[x], reg[y]);
            }break;
            
            case OP_SL0_REGISTER:{
                int x = get_register_x(instr);
                int y = get_register_y(instr);
                printf("sl0 %s, %s", reg[x], reg[y]);
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
                int y = get_address(instr);
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
        printf("\n");
        start++;
    }
    printf("\n");
}
