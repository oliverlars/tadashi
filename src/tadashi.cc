#include "tadashi.h"
#include "memory.h"
#include "lexer.h"
#include "parser.h"


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
    Token token = {};
    do {
        token = get_token(&lexer);
        printf("%.*s\n", token.length, token.at);
    }while(token.type != TOKEN_EOF);
}