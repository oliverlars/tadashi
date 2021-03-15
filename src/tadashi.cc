#include "tadashi.h"
#include "memory.h"
#include "lexer.h"
#include "parser.h"
#include "vm.h"


#include "memory.cc"
#include "lexer.cc"
#include "parser.cc"

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

int main(){
    Lexer lexer = {};
    lexer.at = open_source("test.td");
    Parser p = {lexer};
    global_node_arena = make_arena(sizeof(Ast_Node)*1024);
    auto scope = parse_scope(&p);
    
    VM vm = {};
    
    vm.memory = (u8*)malloc(MEMORY_SIZE);
    vm.memory[0] = 0b000000000000000000000111;
    
    while(vm.pc < 1){
        
        instruction instr = vm.memory[vm.pc];
        
        int opcode = get_opcode(instr);
        
        switch(opcode){
            case OP_MOVE_ABSOLUTE:{
                int r = get_register(instr);
                int operand = get_operand(instr);
                vm.registers[r] = get_operand(instr);
            }break;
            
            case OP_CALL:{
                
            }break;
            
            case OP_RETURN:{
                
            }break;
            
            case OP_ADD_REGISTER:{
            }break;
        }
        
        vm.pc++;
    }
    printf("RA: %d\n", vm.RA);
    printf("RB: %d\n", vm.RB);
    
    return 0;
}